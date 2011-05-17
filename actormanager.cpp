#include "actormanager.h"

Actor::Actor(ActorType _type, const Point _pos)
	: type(_type)
	, id(-1)
	, pos(_pos)
	, old_pos(_pos)
{
}

void Actor::step()
{
}
bool Actor::has_moved()
{
	return pos != old_pos;
}
