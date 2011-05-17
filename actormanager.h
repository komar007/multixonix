#pragma once

#include "geometry.h"

enum ActorType {
	BALL,
	PLAYER
};
class Actor {
private:
	ActorType type;
	int id;
	Point pos;
	Point old_pos;
public:
	Actor(ActorType _type, const Point _pos = Point(.0, .0));
	void step();
	bool has_moved();
};
