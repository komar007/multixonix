#include "actor.h"

Actor::Actor(const Point _pos)
	: id(-1)
	, old_pos(_pos)
	, committed(true)
	, pos(_pos)
	, angle(.0)
	, speed(.0)
{
}

Actor::~Actor()
{
}

void Actor::step()
{
	if (committed) {
		old_pos = pos;
		committed = false;
	}
	pos += Vector(cos(angle), sin(angle)) * speed;
}

void Actor::commit()
{
	committed = true;
	if (has_moved())
		notify(CommitMsg());
}

bool Actor::has_moved()
{
	return pos != old_pos;
}
