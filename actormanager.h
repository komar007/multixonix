#pragma once

//! @file
//! \brief ActorManager class responsible for managing and replicating a set of actors

#include <string>
#include "geometry.h"
#include "observer.h"
#include "actor.h"
#include <stdexcept>

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
	~ActorManager();
	Actor& get_actor_ref(int id) throw (std::out_of_range);
	const Actor& get_actor_ref(int id) const throw (std::out_of_range);
	int add_actor(const Actor& actor);
	void destroy_actor(int id);

	typedef pointer_map_iterator<Actor> iterator;
	iterator begin() { return iterator(actors.begin()); }
	iterator end() { return iterator(actors.end()); }
};
