#pragma once

#include "observer.h"
#include "shapemanager.h"
#include "actormanager.h"
#include <unordered_map>

class BallController : public Observer<PosMsg> {
private:
	const ShapeManager& shapes;
	Point last_collision;
public:
	BallController(const ShapeManager& _shapes);
	virtual void update(Observable<PosMsg>& obj, const PosMsg&);
};

class Xonix : public Observer<ShapeMessage>, public Observer<ActorMessage> {
public:
	enum Mode {
		MASTER,
		SLAVE
	};
private:
	Mode mode;
	int tick;
	ShapeManager shapes;
	ActorManager actors;
	BallController ball_controller;
public:
	Xonix(Mode _mode);
	~Xonix();
	virtual void update(Observable<ShapeMessage>& obj, const ShapeMessage& msg);
	virtual void update(Observable<ActorMessage>& obj, const ActorMessage& msg);
	void add_actor(const Actor& actor);
	/* temporaty */
	ShapeManager& get_shapes() { return shapes; }
	ActorManager& get_actors() { return actors; }
	void step();
};
