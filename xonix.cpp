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
	Actor& actor = dynamic_cast<Actor&>(obj);
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
		actor.angle = normalized_angle(2*wall_angle - actor.angle);
		actor.set_pos(inter.second);
		actor.set_pos(inter.second + Vector::dir(actor.angle) * post_dist);
	}
}

Xonix::Xonix(Mode _mode)
	: mode(_mode)
	, shapes(mode == MASTER)
	, actors()
	, ball_controller(shapes)
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

void Xonix::add_actor(const Actor& actor)
{
	int id = actors.add_actor(actor);
	Actor& new_actor = actors.get_actor_ref(id);
	if (dynamic_cast<const Ball*>(&actor))
		new_actor.Observable<PosMsg>::attach(ball_controller);
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
