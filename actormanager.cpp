#include "actormanager.h"

using namespace std;

Actor::Actor(const Point _pos)
	: id(-1)
	, angle(.0)
	, speed(.0)
	, pos(_pos)
	, old_pos(_pos)
{
}

Actor::~Actor()
{
}

void Actor::step()
{
	old_pos = pos;
	pos += Vector(cos(angle), sin(angle)) * speed;
	if (has_moved())
		notify(pos);
}
bool Actor::has_moved()
{
	return pos != old_pos;
}

ActorMessage::ActorMessage(const ActorMessage& o)
	: type(o.type)
	, id(o.id)
	, actor(o.actor)
	, pos(NULL)
{
	if (o.pos)
		pos = new Point(*o.pos);
}
const ActorMessage& ActorMessage::operator=(const ActorMessage& o)
{
	type  = o.type;
	id    = o.id;
	actor = o.actor;
	if (o.pos) {
		if (pos)
			*pos = *o.pos;
		else
			pos = new Point(*o.pos);
	} else {
		delete pos;
		pos = NULL;
	}
	return *this;
}
ActorMessage::ActorMessage(ActorMessageType _type, int _id, const Actor& _actor)
	: type(_type)
	, id(_id)
	, actor(&_actor)
	, pos(NULL)
{
	if (type != ActorMessage::CREATED)
		throw domain_error("type != CREATED in creation message");
}
ActorMessage::ActorMessage(ActorMessageType _type, int _id, const Point& _pos)
	: type(_type)
	, id(_id)
	, actor(NULL)
	, pos(NULL)
{
	if (type != ActorMessage::MOVED)
		throw domain_error("type != MOVED in motion message");
	pos = new Point(_pos);
}
ActorMessage::ActorMessage(ActorMessageType _type, int _id)
	: type(_type)
	, id(_id)
	, actor(NULL)
	, pos(NULL)
{
	if (type != ActorMessage::DESTROYED)
		throw domain_error("type != DESTROYED in destruction message");
}

ActorManager::ActorManager()
	: last_id(0)
{
}

Actor& ActorManager::get_actor_ref(int id) throw (out_of_range)
{
	auto actor_it = actors.find(id);
	if (actor_it == actors.end())
		throw out_of_range("no such actor in ShapeManager");
	return *actor_it->second;
}
const Actor& ActorManager::get_actor_ref(int id) const throw (out_of_range)
{
	auto actor_it = actors.find(id);
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
	new_actor->attach(*this);
	actors.insert(make_pair(++last_id, new_actor));
	return new_actor->id = last_id;
}
int ActorManager::add_actor(const Actor& actor)
{
	int id = add_actor_impl(actor);
	const Actor& new_actor = get_actor_ref(id);
	notify(ActorMessage(ActorMessage::CREATED, id, new_actor));
	return id;
}

void ActorManager::destroy_actor_impl(int id)
{
	auto actor_it = actors.find(id);
	if (actor_it == actors.end())
		throw out_of_range("No such actor in destroy_actor");
	actor_it->second->detach(*this);
	delete actor_it->second;
	actors.erase(actor_it);
}
void ActorManager::destroy_actor(int id)
{
	destroy_actor_impl(id);
	notify(ActorMessage(ActorMessage::DESTROYED, id));
}

void ActorManager::update(const Observable<Point>& obj, const Point& msg)
{
	const Actor& actor = dynamic_cast<const Actor&>(obj);
	notify(ActorMessage(ActorMessage::MOVED, actor.id, actor.pos));
}
