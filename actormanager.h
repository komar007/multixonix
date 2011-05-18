#pragma once

#include <string>
#include "geometry.h"
#include "observer.h"

class Actor : public Observable<Point> {
private:
	int id;
	double angle;
	double speed;
	Point pos;
	Point old_pos;
public:
	Actor(const Point _pos = Point(.0, .0));
	virtual ~Actor();
	void step();
	bool has_moved();
};

class Player : public Actor {
	std::string name;
};

class Ball : public Actor {
};
