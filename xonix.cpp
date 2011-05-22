#include "xonix.h"
#include <vector>

using namespace std;

BallController::BallController(const ShapeManager& _shapes)
	: shapes(_shapes)
	, last_collision(pinf, pinf)
{
}

void BallController::update(Observable<PosMsg>& obj, const PosMsg& msg)
{
	Ball& ball = dynamic_cast<Ball&>(obj);
	for (auto shape_it = shapes.begin(); shape_it != shapes.end(); ++shape_it) {
		vector<Detector::Intersection> ints;
		if (shape_it->intersections(msg.old_pos, msg.cur_pos, ints) == 0)
			continue;
		vector<Detector::Intersection>::const_iterator int_it;
		for (int_it = ints.begin(); int_it != ints.end(); ++int_it)
			if (Vector(int_it->second, last_collision).length() > 0.00001)
				break;
		if (int_it == ints.end())
			continue;
		const Detector::Intersection& inter = *int_it;
		const Vector wall(*inter.first, *(inter.first+1));
		const double wall_angle = atan(wall.dy / wall.dx);
		const double post_dist = Vector(inter.second, msg.cur_pos).length();
		last_collision = inter.second;
		ball.angle = normalized_angle(2*wall_angle - ball.angle);
		ball.set_pos(inter.second);
		ball.set_pos(inter.second + Vector::dir(ball.angle) * post_dist);
	}
}

PlayerController::PlayerController(ShapeManager& _shapes)
	: shapes(_shapes)
	, last_collision(pinf, pinf)
{
}

void PlayerController::update(Observable<PosMsg>& obj, const PosMsg& msg)
{
	Player& player = dynamic_cast<Player&>(obj);
	int ncollisions = 0;
	for (auto shape_it = shapes.begin(); shape_it != shapes.end(); ++shape_it) {
		if (!shape_it->get_path().closed)
			continue;
		vector<Detector::Intersection> ints;
		if (shape_it->intersections(msg.old_pos, msg.cur_pos, ints) == 0)
			continue;
		vector<Detector::Intersection>::const_iterator int_it;
		for (int_it = ints.begin(); int_it != ints.end(); ++int_it)
			if (Vector(int_it->second, last_collision).length() > 0.00001)
				break;
		if (int_it == ints.end())
			continue;
		if (player.trace_id == -1) {
			player.trace_id = shapes.start_trace(int_it->second);
			player.trace_beg = int_it->first.get_index();
		} else {
			const Shape& trace = shapes.get_shape_const_ref(player.trace_id);
			shapes.extend_trace(player.trace_id, int_it->second);
			shapes.cut_shape(
					player.trace_id,
					shape_it->get_id(),
					player.trace_beg,
					int_it->first.get_index()
					);
			shapes.destroy_shape(player.trace_id);
			shapes.destroy_shape(shape_it->get_id());
			player.trace_id = -1;
			last_collision = int_it->second;
			player.set_pos(int_it->second);
			player.set_pos(msg.cur_pos);
		}
	}
	if (ncollisions == 0 && player.trace_id != -1)
		shapes.extend_trace(player.trace_id, msg.cur_pos);
}

Xonix::Xonix(Mode _mode)
	: mode(_mode)
	, shapes(mode == MASTER)
	, actors()
	, ball_controller(shapes)
	, player_controller(shapes)
{
	shapes.attach(*this);
	actors.attach(*this);
	Path start_shape(true);
	start_shape.push_back(Point(2, 1));
	start_shape.push_back(Point(9, 5));
	start_shape.push_back(Point(9, 6));
	start_shape.push_back(Point(1, 9));
	shapes.add_shape(start_shape);
}

Xonix::~Xonix()
{
	shapes.detach(*this);
	actors.detach(*this);
}

int Xonix::add_actor(const Actor& actor)
{
	int id = actors.add_actor(actor);
	Actor& new_actor = actors.get_actor_ref(id);
	if (dynamic_cast<const Ball*>(&actor))
		new_actor.Observable<PosMsg>::attach(ball_controller);
	if (dynamic_cast<const Player*>(&actor))
		new_actor.Observable<PosMsg>::attach(player_controller);
	return id;
}

void Xonix::step()
{
	for (auto actor = actors.begin(); actor != actors.end(); ++actor) {
		actor->begin();
		actor->step();
		actor->commit();
	}
}

void Xonix::update(Observable<ShapeMessage>& obj, const ShapeMessage& msg)
{
}
void Xonix::update(Observable<ActorMessage>& obj, const ActorMessage& msg)
{
}
