#pragma once

//! @file
//! \brief Actor class, responsible for representing a movable object in the game

#include "geometry.h"
#include "observer.h"

class CommitMsg {
};

class PosMsg {
public:
	Point old_pos, cur_pos;
	PosMsg(const Point& _old_pos, const Point& _cur_pos)
		: old_pos(_old_pos)
		, cur_pos(_cur_pos)
	{
	}
};

class ActorManager;

class Actor : public Observable<CommitMsg>, public Observable<PosMsg> {
private:
	int id;
	Point pos;
	Point old_pos;
	bool committed;
public:
	double angle;
	double speed;

	Actor(const Point _pos);
	virtual ~Actor();
	const Point& get_oldpos() const { return old_pos; }
	const Point& get_pos() const { return pos; }
	const Point& set_pos(const Point& p);
	void step();
	void begin();
	void commit();
	bool has_moved();
	friend class ActorManager;
};

class Player : public Actor {
private:
	std::string name;
public:
	int trace_id;
	int trace_beg;

	Player(const Point _pos = Point(.0, .0));
};

class Ball : public Actor {
public:
	Ball(const Point _pos = Point(.0, .0));
};
