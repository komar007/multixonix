#pragma once

#include <string>
#include "geometry.h"
#include "observer.h"

class ActorManager;

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
	friend class ActorManager;
};

class Player : public Actor {
	std::string name;
};

class Ball : public Actor {
};

class ActorMessage {
public:
	enum ActorMessageType {
		CREATED,
		DESTROYED,
		MOVED
	};
private:
	ActorMessageType type;
	int id;
	const Actor *actor;
	Point *pos;
public:
	ActorMessage(const ActorMessage& o);
	const ActorMessage& operator=(const ActorMessage& o);
	ActorMessage(ActorMessageType _type, int _id, const Actor& _actor);
	ActorMessage(ActorMessageType _type, int _id, const Point& _pos);
	ActorMessage(ActorMessageType _type, int _id);
};

class ActorManager : public Observer<Point>, public Observable<ActorMessage> {
private:
	std::unordered_map<int, Actor*> actors;
	int last_id;

	int add_actor_impl(const Actor& actor);
	void destroy_actor_impl(int id);
public:
	virtual void update(const Observable<Point>& obj, const Point& msg);
	ActorManager();
	int add_actor(const Actor& actor);
	void destroy_actor(int id);
};
