#include "actor.h"

Actor::Actor(const Point _pos)
	: id(-1)
	, pos(_pos)
	, old_pos(_pos)
	, angle(.0)
	, speed(.0)
{
}

Actor::~Actor()
{
}

void Actor::step()
{
	old_pos = pos;
	pos += Vector(cos(angle), sin(angle)) * speed;
	if (has_moved())
		notify(pos);
}

bool Actor::has_moved()
{
	return pos != old_pos;
}
