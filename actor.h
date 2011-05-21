#pragma once

//! @file
//! \brief Actor class, responsible for representing a movable object in the game

#include "geometry.h"
#include "observer.h"

class CommitMsg
{
};

class ActorManager;

class Actor : public Observable<CommitMsg> {
private:
	int id;
	Point old_pos;
	bool committed;
public:
	Point pos;
	double angle;
	double speed;

	Actor(const Point _pos = Point(.0, .0));
	virtual ~Actor();
	void step();
	void commit();
	bool has_moved();
	friend class ActorManager;
};

class Player : public Actor {
	std::string name;
};

class Ball : public Actor {
};
