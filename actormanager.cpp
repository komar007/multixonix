#include "actormanager.h"

using namespace std;

ActorManager::ActorManager()
	: last_id(0)
{
}

ActorManager::~ActorManager()
{
	for (auto it = actors.begin(); it != actors.end(); ++it) {
		it->second->Observable<CommitMsg>::detach(*this);
		delete it->second;
	}
}

Actor& ActorManager::get_actor_ref(int id) throw (out_of_range)
{
	const auto actor_it = actors.find(id);
	if (actor_it == actors.end())
		throw out_of_range("no such actor in ShapeManager");
	return *actor_it->second;
}
const Actor& ActorManager::get_actor_ref(int id) const throw (out_of_range)
{
	const auto actor_it = actors.find(id);
	if (actor_it == actors.end())
		throw out_of_range("no such actor in ShapeManager");
	return *actor_it->second;
}

int ActorManager::add_actor_impl(const Actor& actor)
{
	Actor *new_actor = NULL;
	if (const Player *p = dynamic_cast<const Player*>(&actor))
		new_actor = new Player(*p);
	else if (const Ball *b = dynamic_cast<const Ball*>(&actor))
		new_actor = new Ball(*b);
	new_actor->Observable<CommitMsg>::attach(*this);
	actors.insert(make_pair(++last_id, new_actor));
	return new_actor->id = last_id;
}
int ActorManager::add_actor(const Actor& actor)
{
	const int id = add_actor_impl(actor);
	const Actor& new_actor = get_actor_ref(id);
	notify(ActorMessage(ActorMessage::CREATED, id, new_actor));
	return id;
}

void ActorManager::destroy_actor_impl(int id) throw (out_of_range)
{
	const auto actor_it = actors.find(id);
	if (actor_it == actors.end())
		throw out_of_range("No such actor in destroy_actor");
	actor_it->second->Observable<CommitMsg>::detach(*this);
	delete actor_it->second;
	actors.erase(actor_it);
}
void ActorManager::destroy_actor(int id) throw (out_of_range)
{
	destroy_actor_impl(id);
	notify(ActorMessage(ActorMessage::DESTROYED, id));
}

void ActorManager::update(Observable<CommitMsg>& obj, const CommitMsg&)
{
	const Actor& actor = dynamic_cast<const Actor&>(obj);
	notify(ActorMessage(ActorMessage::MOVED, actor.id, actor.pos));
}
