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

const Path& Shape::get_path() const
{
	return path;
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

const Shape& ShapeManager::get_shape(int id) const throw (out_of_range)
{
	auto trace_it = shapes.find(id);
	if (trace_it == shapes.end())
		throw out_of_range("no such trace in ShapeManager");
	return *trace_it->second;
}
Shape& ShapeManager::get_shape(int id) throw (out_of_range)
{
	auto trace_it = shapes.find(id);
	if (trace_it == shapes.end())
		throw out_of_range("no such trace in ShapeManager");
	return *trace_it->second;
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
	Shape& shape = get_shape(id);
	try {
		shape.extend(point);
	} catch (domain_error) {
		throw domain_error("closed path as trace in ShapeManager::extend_trace");
	}
}
int ShapeManager::make_cut_shape_forward(const Path& trace, const Path& shape, int s1, int s2)
{
	Path p(trace);
	p.closed = true;
	Path::const_iterator it_end1 = s1 != s2 ?
		shape.nth_point(s1+1) :
		shape.nth_point(s1+1).next_cycle();
	for (Path::const_iterator it = shape.nth_point(s2+1); it != it_end1; ++it)
		p.push_back(*it);
	Shape *new_shape = new Shape(move(p), with_detector);
	shapes.insert(make_pair(++last_id, new_shape));
	return last_id;
}
int ShapeManager::make_cut_shape_reverse(const Path& trace, const Path& shape, int s1, int s2)
{
	Path p(trace);
	p.closed = true;
	Path::const_reverse_iterator it_end2 = shape.nth_point(s1);
	for (Path::const_reverse_iterator it = shape.nth_point(s2); it != it_end2; ++it)
		p.push_back(*it);
	Shape *new_shape = new Shape(move(p), with_detector);
	shapes.insert(make_pair(++last_id, new_shape));
	return last_id;
}
pair<int, int> ShapeManager::cut_shape(int trace_id, int id, int s1, int s2)
{
	const Path& trace = get_shape(trace_id).get_path();
	const Path& shape = get_shape(id).get_path();
	pair<int, int> ids(-1, -1);
	ids.first  = make_cut_shape_forward(trace, shape, s1, s2),
	notify(ShapeMessage(CREATED, ids.first,
				ShapeCreationInfo(trace_id, id, s2, s1, FORWARD)));
	ids.second = make_cut_shape_reverse(trace, shape, s1, s2);
	notify(ShapeMessage(CREATED, ids.second,
				ShapeCreationInfo(trace_id, id, s2, s1, REVERSE)));
	return ids;
}
