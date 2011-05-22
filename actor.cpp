#include "actor.h"

Actor::Actor(const Point _pos)
	: id(-1)
	, pos(_pos)
	, old_pos(_pos)
	, committed(true)
	, angle(.0)
	, speed(.0)
{
}

Actor::~Actor()
{
}

void Actor::begin()
{
	committed = false;
	old_pos = pos;
}

const Point& Actor::set_pos(const Point& _pos)
{
	PosMsg m(pos, _pos);
	pos = _pos;
	if (!committed)
		Observable<PosMsg>::notify(m);
	return pos;
}
void Actor::step()
{
	set_pos(pos + Vector(cos(angle), sin(angle)) * speed);
}

void Actor::commit()
{
	committed = true;
	if (has_moved())
		Observable<CommitMsg>::notify(CommitMsg());
}

bool Actor::has_moved()
{
	return pos != old_pos;
}

Ball::Ball(const Point _pos)
	: Actor(_pos)
{
}

Player::Player(const Point _pos)
	: Actor(_pos)
	, trace_id(-1)
	, trace_beg(-1)
{
}
