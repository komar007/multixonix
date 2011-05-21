#include "xonix.h"

using namespace std;

BallController::BallController(const ShapeManager& _shapes)
	: shapes(_shapes)
{
}

void BallController::update(Observable<PosMsg>& obj, const PosMsg&)
{
	Actor& actor = dynamic_cast<Actor&>(obj);
	for (auto shape_it = shapes.begin(); shape_it != shapes.end(); ++shape_it) {
		vector<Detector::Intersection> ints;
		if (shape_it->intersections(actor.get_oldpos(), actor.get_pos(), ints) == 0)
			continue;
		const Detector::Intersection& inter = *ints.begin();
		const Vector wall(*inter.first, *(inter.first+1));
		const double wall_angle = asin(wall.dy / wall.length());
		const double extra_dist = Vector(inter.second, actor.get_pos()).length();
		actor.angle = 2*wall_angle - actor.angle;
		actor.set_pos(inter.second + Vector::dir(actor.angle) * extra_dist);
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
	start_shape.push_back(Point(1, 1));
	start_shape.push_back(Point(9, 1));
	start_shape.push_back(Point(9, 9));
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
