#pragma once

#include "geometry.h"
#include "observer.h"

class ActorManager;

class Actor : public Observable<Point> {
private:
	int id;
	Point pos;
	Point old_pos;
public:
	double angle;
	double speed;

	Actor(const Point _pos = Point(.0, .0));
	virtual ~Actor();
	void step();
	bool has_moved();
	friend class ActorManager;
};

class Player : public Actor {
	std::string name;
};

class Ball : public Actor {
};
