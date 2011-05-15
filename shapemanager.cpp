#include "shapemanager.h"

using namespace std;

Shape::Shape(const Path& _path, bool with_detector)
	: path(_path)
	, detector(NULL)
{
	initialize(with_detector);
}

Shape::Shape(Path&& _path, bool with_detector)
	: path(move(_path))
	, detector(NULL)
{
	initialize(with_detector);
}

void Shape::initialize(bool with_detector)
{
	if (with_detector)
		detector = new Detector(path, Vector(0, 0), 1); //FIXME: choose sensible values
}

void Shape::extend(const Point& point) throw (domain_error)
{
	if (path.closed)
		throw domain_error("closed path in Shape::extend");
	path.push_back(point);
	if (detector && path.size() >= 2)
		detector->add_segment(path.nth_point(path.size()-2));
}

Shape::~Shape()
{
	delete detector;
}

ShapeCreationInfo::ShapeCreationInfo(const ShapeCreationInfo& o)
	: trace_id(o.trace_id)
	, shape_id(o.shape_id)
	, shape_start(o.shape_start)
	, shape_end(o.shape_end)
	, path(NULL)
{
	if (o.path)
		path = new Path(*o.path);
}
ShapeCreationInfo::ShapeCreationInfo(int _trace_id, int _shape_id, int _shape_start, int _shape_end)
	: trace_id(_trace_id)
	, shape_id(_shape_id)
	, shape_start(_shape_start)
	, shape_end(_shape_end)
	, path(NULL)
{
}
ShapeCreationInfo::ShapeCreationInfo(const Path& _path)
	: trace_id(-1)
	, shape_id(-1)
	, shape_start(-1)
	, shape_end(-1)
	, path(NULL)
{
	path = new Path(_path);
}
ShapeCreationInfo::ShapeCreationInfo()
	: trace_id(-1)
	, shape_id(-1)
	, shape_start(-1)
	, shape_end(-1)
	, path(NULL)
{
}
const ShapeCreationInfo& ShapeCreationInfo::operator=(const ShapeCreationInfo& o)
{
	trace_id = o.trace_id;
	shape_id = o.shape_id;
	shape_start = o.shape_start;
	shape_end = o.shape_end;
	if (o.path) {
		if (path)
			*path = *o.path;
		else
			path = new Path(*o.path);
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
{
	if (o.info)
		info = new ShapeCreationInfo(*o.info);
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
	}
	return *this;
}
ShapeMessage::ShapeMessage(ShapeMessageType _type, int _id, const ShapeCreationInfo& _info)
	: type(_type)
	, id(_id)
	, info(NULL)
{
	if (type == CREATED)
		info = new ShapeCreationInfo(_info);
}
ShapeMessage::ShapeMessage()
	: type(DUMMY)
	, id(-1)
	, info(NULL)
{
}
ShapeMessage::~ShapeMessage()
{
	delete info;
}

ShapeManager::ShapeManager(bool _with_detector)
	: with_detector(_with_detector)
	, last_id(0)
{
}

int ShapeManager::add_shape(const Path& path)
{
	Shape *sh = new Shape(path, with_detector);
	++last_id;
	shapes.insert(make_pair(last_id, sh));
	notify(ShapeMessage(CREATED, last_id, ShapeCreationInfo(path)));
	return last_id;
}
int ShapeManager::start_trace(const Point& point)
{
	Path p(false);
	p.push_back(point);
	return add_shape(p);
}
void ShapeManager::extend_trace(int id, const Point& point)
{
	auto trace_it = shapes.find(id);
	if (trace_it == shapes.end())
		throw out_of_range("no such trace in ShapeManager::extend_trace");
	Shape *shape = trace_it->second;
	try {
		shape->extend(point);
	} catch (domain_error) {
		throw domain_error("closed path as trace in ShapeManager::extend_trace");
	}
}
pair<int, int> ShapeManager::cut_shape(int id, int trace_id)
{
	return {-1, -1};
}
