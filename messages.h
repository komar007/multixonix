#pragma once

//! @file
//! \brief all messages used for synchronization

#include "geometry.h"
#include "shape.h"
#include "actor.h"

//! \defgroup shapamessage ShapeManager's message type
//! @{
class ShapeCreationInfo {
public:
	enum ShapeDirection {
		FORWARD,
		REVERSE
	};
private:
	int trace_id;
	int shape_id;
	int shape_start,
	    shape_end;
	ShapeDirection shape_dir;
	Path *path;
public:
	ShapeCreationInfo(const ShapeCreationInfo& o);
	//! ShapeCreationInfo informing that a shape was created from trace and shape
	ShapeCreationInfo(int _trace_id, int _shape_id, int _shape_start, int _shape_end, ShapeDirection shape_dir);
	//! ShapeCreationInfo informing that a shape was created from scratch
	ShapeCreationInfo(const Path& _path);
	const ShapeCreationInfo& operator=(const ShapeCreationInfo& o);
	~ShapeCreationInfo();
	friend class ShapeManager;
};

class ShapeMessage {
public:
	enum ShapeMessageType {
		CREATED,
		DESTROYED,
		EXTENDED
	};
private:
	ShapeMessageType type;
	int id;
	ShapeCreationInfo* info;
	Point *extension_point;
public:
	//! ShapeMessage about shape creation
	ShapeMessage(ShapeMessageType _type, int _id, const ShapeCreationInfo& _info);
	//! ShapeMessage about trace extension
	ShapeMessage(ShapeMessageType _type, int _id, const Point& _extension_point);
	//! ShapeMessage about shape deletion
	ShapeMessage(ShapeMessageType _type, int _id);
	ShapeMessage(const ShapeMessage& o);
	const ShapeMessage& operator=(const ShapeMessage& o);
	~ShapeMessage();
	friend class ShapeManager;
};
//! @}

//! \defgroup actormessage ActorManager's message type
//! @{
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
//! @}
