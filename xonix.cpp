#include "xonix.h"

using namespace std;

Xonix::Xonix(bool with_detector)
	: shapes(with_detector)
	, actors()
{
	shapes.attach(*this);
	actors.attach(*this);
	Path start_shape(true);
	start_shape.push_back(Point(10, 10));
	start_shape.push_back(Point(50, 10));
	start_shape.push_back(Point(50, 40));
	start_shape.push_back(Point(10, 40));
	shapes.add_shape(start_shape);
}

Xonix::~Xonix()
{
	shapes.detach(*this);
	shapes.detach(*this);
}

void Xonix::step()
{
	for (auto actor = actors.begin(); actor != actors.end(); ++actor)
		actor->step();
}

void Xonix::update(const Observable<ShapeMessage>& obj, const ShapeMessage& msg)
{
}
void Xonix::update(const Observable<ActorMessage>& obj, const ActorMessage& msg)
{
}
