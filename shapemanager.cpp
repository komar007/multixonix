#include "shapemanager.h"

using namespace std;

Shape::Shape(const Path& _path, bool with_detector)
	: id(-1)
	, path(_path)
	, detector(NULL)
{
	initialize(with_detector);
}

Shape::Shape(Path&& _path, bool with_detector)
	: id(-1)
	, path(move(_path))
	, detector(NULL)
{
	initialize(with_detector);
}

void Shape::initialize(bool with_detector)
{
	if (with_detector)
		detector = new Detector(path, Vector(0, 0), 0.05); //FIXME: choose sensible values
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
ShapeMessage::ShapeMessage(ShapeMessageType _type, int _id, const ShapeCreationInfo& _info)
	: type(_type)
	, id(_id)
	, info(NULL)
	, extension_point(NULL)
{
	if (type != CREATED)
		throw domain_error("type != CREATED in creation message");
	info = new ShapeCreationInfo(_info);
}
ShapeMessage::ShapeMessage(ShapeMessageType _type, int _id, const Point& _extension_point)
	: type(_type)
	, id(_id)
	, info(NULL)
	, extension_point(NULL)
{
	if (type != EXTENDED)
		throw domain_error("type != EXTENDED in extension message");
	extension_point = new Point(_extension_point);
}
ShapeMessage::ShapeMessage(ShapeMessageType _type, int _id)
	: type(_type)
	, id(_id)
	, info(NULL)
	, extension_point(NULL)
{
	if (type != DESTROYED)
		throw domain_error("type != DESTROYED in destruction message");
}
ShapeMessage::~ShapeMessage()
{
	delete info;
	delete extension_point;
}

ShapeManager::ShapeManager(bool _with_detector)
	: with_detector(_with_detector)
	, last_id(0)
{
}

ShapeManager::~ShapeManager()
{
	for (auto it = shapes.begin(); it != shapes.end(); ++it)
		delete it->second;
}

const Shape& ShapeManager::get_shape_const_ref(int id) const throw (out_of_range)
{
	auto trace_it = shapes.find(id);
	if (trace_it == shapes.end())
		throw out_of_range("no such shape in ShapeManager");
	return *trace_it->second;
}
Shape& ShapeManager::get_shape_ref(int id) throw (out_of_range)
{
	auto trace_it = shapes.find(id);
	if (trace_it == shapes.end())
		throw out_of_range("no such shape in ShapeManager");
	return *trace_it->second;
}

int ShapeManager::add_shape_impl(const Path& path, int id)
{
	Shape *sh = new Shape(path, with_detector);
	if (id == -1) {
		shapes.insert(make_pair(++last_id, sh));
		return sh->id = last_id;
	} else {
		shapes.insert(make_pair(id, sh));
		return sh->id = id;
	}
}
int ShapeManager::add_shape(const Path& path)
{
	int id = add_shape_impl(path);
	notify(ShapeMessage(CREATED, id, ShapeCreationInfo(path)));
	return id;
}
int ShapeManager::start_trace(const Point& point)
{
	Path p(false);
	p.push_back(point);
	int id = add_shape(p);
	return id;
}
void ShapeManager::extend_trace_impl(int id, const Point& point)
{
	Shape& shape = get_shape_ref(id);
	shape.extend(point);
}
void ShapeManager::extend_trace(int id, const Point& point)
{
	extend_trace_impl(id, point);
	notify(ShapeMessage(EXTENDED, id, point));
}
int ShapeManager::cut_shape_impl(const Path& trace, const Path& shape, int s1, int s2, ShapeDirection dir, int id)
{
	Path p(trace);
	p.closed = true;
	bool only_forward = false,
	     only_reverse = false;
	if (s1 == s2) {
		// distances between endpoints (beginning, end) of the trace and s1=s2
		double beg_dist = (*shape.nth_point(s1) - *trace.begin()).length(),
		       end_dist = (*shape.nth_point(s1) - *trace.nth_point(trace.size()-1)).length();
		if (beg_dist < end_dist)
			only_forward = true;
		else
			only_reverse = true;
	}
	if (dir == FORWARD && !only_reverse) {
		Path::const_iterator it_end = s1 != s2 ?
			shape.nth_point(s1+1) :
			shape.nth_point(s1+1).next_cycle();
		for (Path::const_iterator it = shape.nth_point(s2+1); it != it_end; ++it)
			p.push_back(*it);
	} else if (dir == REVERSE && !only_forward) {
		Path::const_reverse_iterator it_end = s1 != s2 ?
			shape.nth_point(s1) :
			shape.nth_point(s1).next_cycle();
		for (Path::const_reverse_iterator it = shape.nth_point(s2); it != it_end; ++it)
			p.push_back(*it);
	}
	Shape *new_shape = new Shape(move(p), with_detector);
	if (id == -1) {
		shapes.insert(make_pair(++last_id, new_shape));
		return new_shape->id = last_id;
	} else {
		shapes.insert(make_pair(id, new_shape));
		return new_shape->id = id;
	}
}
pair<int, int> ShapeManager::cut_shape(int trace_id, int id, int s1, int s2)
{
	const Path& trace = get_shape_ref(trace_id).get_path();
	const Path& shape = get_shape_ref(id).get_path();
	pair<int, int> ids(-1, -1);
	ids.first  = cut_shape_impl(trace, shape, s1, s2, FORWARD),
	notify(ShapeMessage(CREATED, ids.first,
				ShapeCreationInfo(trace_id, id, s2, s1, FORWARD)));
	ids.second = cut_shape_impl(trace, shape, s1, s2, REVERSE);
	notify(ShapeMessage(CREATED, ids.second,
				ShapeCreationInfo(trace_id, id, s2, s1, REVERSE)));
	return ids;
}
void ShapeManager::destroy_shape_impl(int id)
{
	auto shape_it = shapes.find(id);
	if (shape_it == shapes.end())
		throw out_of_range("No such shape in destroy_shape");
	delete shape_it->second;
	shapes.erase(shape_it);
}
void ShapeManager:: destroy_shape(int id)
{
	destroy_shape_impl(id);
	notify(ShapeMessage(DESTROYED, id));
}

void ShapeManager::update(const Observable<ShapeMessage>& obj, const ShapeMessage& msg)
{
	switch (msg.type) {
	case CREATED:
		if (msg.info->path) {
			add_shape_impl(*msg.info->path, msg.id);
		} else {
			const Path& trace = get_shape_const_ref(msg.info->trace_id).get_path();
			const Path& shape = get_shape_const_ref(msg.info->shape_id).get_path();
			cut_shape_impl(
				trace, shape,
				msg.info->shape_end, msg.info->shape_start,
				msg.info->shape_dir,
				msg.id
			);
		}
		break;
	case DESTROYED:
		destroy_shape_impl(msg.id);
		break;
	case EXTENDED:
		extend_trace_impl(msg.id, *msg.extension_point);
		break;
	case DUMMY:
		break;
	}
}
