#include "messages.h"
#include <stdexcept>

using namespace std;

// ShapeManager's messages begin

ShapeCreationInfo::ShapeCreationInfo(const ShapeCreationInfo& o)
	: trace_id(o.trace_id)
	, shape_id(o.shape_id)
	, shape_start(o.shape_start)
	, shape_end(o.shape_end)
	, shape_dir(o.shape_dir)
	, path(NULL)
{
	if (o.path)
		path = new Path(*o.path);
}

ShapeCreationInfo::ShapeCreationInfo(int _trace_id, int _shape_id, int _shape_start, int _shape_end, enum ShapeDirection _shape_dir)
	: trace_id(_trace_id)
	, shape_id(_shape_id)
	, shape_start(_shape_start)
	, shape_end(_shape_end)
	, shape_dir(_shape_dir)
	, path(NULL)
{
}

ShapeCreationInfo::ShapeCreationInfo(const Path& _path)
	: trace_id(-1)
	, shape_id(-1)
	, shape_start(-1)
	, shape_end(-1)
	, shape_dir(FORWARD)
	, path(NULL)
{
	path = new Path(_path);
}

const ShapeCreationInfo& ShapeCreationInfo::operator=(const ShapeCreationInfo& o)
{
	trace_id    = o.trace_id;
	shape_id    = o.shape_id;
	shape_start = o.shape_start;
	shape_end   = o.shape_end;
	shape_dir   = o.shape_dir;
	if (o.path) {
		if (path)
			*path = *o.path;
		else
			path = new Path(*o.path);
	} else {
		delete path;
		path = NULL;
	}
	return *this;
}

ShapeCreationInfo::~ShapeCreationInfo()
{
	delete path;
}

ShapeMessage::ShapeMessage(const ShapeMessage& o)
	: type(o.type)
	, id(o.id)
	, info(NULL)
	, extension_point(NULL)
{
	if (o.info)
		info = new ShapeCreationInfo(*o.info);
	if (o.extension_point)
		extension_point = new Point(*o.extension_point);
}

ShapeMessage::ShapeMessage(ShapeMessageType _type, int _id, const ShapeCreationInfo& _info) throw (domain_error)
	: type(_type)
	, id(_id)
	, info(NULL)
	, extension_point(NULL)
{
	if (type != ShapeMessage::CREATED)
		throw domain_error("type != CREATED in creation message");
	info = new ShapeCreationInfo(_info);
}

ShapeMessage::ShapeMessage(ShapeMessageType _type, int _id, const Point& _extension_point) throw (domain_error)
	: type(_type)
	, id(_id)
	, info(NULL)
	, extension_point(NULL)
{
	if (type != ShapeMessage::EXTENDED)
		throw domain_error("type != EXTENDED in extension message");
	extension_point = new Point(_extension_point);
}

ShapeMessage::ShapeMessage(ShapeMessageType _type, int _id) throw (domain_error)
	: type(_type)
	, id(_id)
	, info(NULL)
	, extension_point(NULL)
{
	if (type != ShapeMessage::DESTROYED)
		throw domain_error("type != DESTROYED in destruction message");
}

const ShapeMessage& ShapeMessage::operator=(const ShapeMessage& o)
{
	type = o.type;
	id = o.id;
	if (o.info) {
		if (info)
			*info = *o.info;
		else
			info = new ShapeCreationInfo(*o.info);
	} else {
		delete info;
		info = NULL;
	}
	if (o.extension_point) {
		if (extension_point)
			*extension_point = *o.extension_point;
		else
			extension_point = new Point(*o.extension_point);
	} else {
		delete extension_point;
		extension_point = NULL;
	}
	return *this;
}

ShapeMessage::~ShapeMessage()
{
	delete info;
	delete extension_point;
}

// ShapeManager's messages end
// ActorManager's messages begin

ActorMessage::ActorMessage(const ActorMessage& o)
	: type(o.type)
	, id(o.id)
	, actor(o.actor)
	, pos(NULL)
{
	if (o.pos)
		pos = new Point(*o.pos);
}

ActorMessage::ActorMessage(ActorMessageType _type, int _id, const Actor& _actor) throw (domain_error)
	: type(_type)
	, id(_id)
	, actor(&_actor)
	, pos(NULL)
{
	if (type != ActorMessage::CREATED)
		throw domain_error("type != CREATED in creation message");
}

ActorMessage::ActorMessage(ActorMessageType _type, int _id, const Point& _pos) throw (domain_error)
	: type(_type)
	, id(_id)
	, actor(NULL)
	, pos(NULL)
{
	if (type != ActorMessage::MOVED)
		throw domain_error("type != MOVED in motion message");
	pos = new Point(_pos);
}

ActorMessage::ActorMessage(ActorMessageType _type, int _id) throw (domain_error)
	: type(_type)
	, id(_id)
	, actor(NULL)
	, pos(NULL)
{
	if (type != ActorMessage::DESTROYED)
		throw domain_error("type != DESTROYED in destruction message");
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

// ActorManager's messages end
