#pragma once

#include "observer.h"
#include "shapemanager.h"
#include "actormanager.h"
#include <unordered_map>

class Xonix : public Observer<ShapeMessage>, public Observer<ActorMessage> {
private:
	int tick;
	ShapeManager shapes;
	ActorManager actors;
public:
	Xonix(bool with_detector);
	~Xonix();
	virtual void update(const Observable<ShapeMessage>& obj, const ShapeMessage& msg);
	virtual void update(const Observable<ActorMessage>& obj, const ActorMessage& msg);
	void step();
};
