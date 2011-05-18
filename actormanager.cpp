#include "actormanager.h"

using namespace std;

Actor::Actor(const Point _pos)
	: id(-1)
	, angle(.0)
	, speed(.0)
	, pos(_pos)
	, old_pos(_pos)
{
}

Actor::~Actor()
{
}

void Actor::step()
{
	old_pos = pos;
	pos += Vector(cos(angle), sin(angle)) * speed;
}
bool Actor::has_moved()
{
	return pos != old_pos;
}
