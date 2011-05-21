#pragma once

//! @file
//! \brief ActorManager class responsible for managing and replicating a set of actors

#include <string>
#include "geometry.h"
#include "observer.h"
#include "actor.h"
#include "messages.h"
#include <stdexcept>

class ActorManager : public Observer<CommitMsg>, public Observable<ActorMessage> {
private:
	std::unordered_map<int, Actor*> actors;
	int last_id;

	int add_actor_impl(const Actor& actor);
	void destroy_actor_impl(int id) throw (std::out_of_range);
public:
	virtual void update(Observable<CommitMsg>& obj, const CommitMsg&);
	ActorManager();
	~ActorManager();
	//! \brief Returns reference to an actor of given id
	//!
	//! @param id actor's id in ActorManager
	//! @return reference to actor
	Actor& get_actor_ref(int id) throw (std::out_of_range);
	//! \brief Returns const-reference to an actor of given id
	//!
	//! @param id actor's id in ActorManager
	//! @return const reference to actor
	const Actor& get_actor_ref(int id) const throw (std::out_of_range);
	//! \brief Adds an actor to ActorManager, making a copy
	//!
	//! @param actor actor to be copied into ActorManager
	//! @return added actor's id
	int add_actor(const Actor& actor);
	//! \brief Removes an actor from ActorManager
	//!
	//! @param id actor's id in ActorManager
	void destroy_actor(int id) throw (std::out_of_range);

	typedef pointer_map_iterator<Actor> iterator;
	iterator begin() { return iterator(actors.begin()); }
	iterator end() { return iterator(actors.end()); }
};
