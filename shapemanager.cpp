#include "shapemanager.h"

using namespace std;

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

const Shape& ShapeManager::get_shape_const_ref(int id) const
throw (out_of_range)
{
	const auto trace_it = shapes.find(id);
	if (trace_it == shapes.end())
		throw out_of_range("no such shape in ShapeManager");
	return *trace_it->second;
}

Shape& ShapeManager::get_shape_ref(int id)
throw (out_of_range)
{
	const auto trace_it = shapes.find(id);
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
	const int id = add_shape_impl(path);
	notify(ShapeMessage(ShapeMessage::CREATED, id, ShapeCreationInfo(path)));
	return id;
}

int ShapeManager::start_trace(const Point& point)
{
	Path p(false);
	p.push_back(point);
	const int id = add_shape(p);
	return id;
}

void ShapeManager::extend_trace_impl(int id, const Point& point)
throw (out_of_range)
{
	Shape& shape = get_shape_ref(id);
	shape.extend(point);
}
void ShapeManager::extend_trace(int id, const Point& point)
throw (out_of_range)
{
	extend_trace_impl(id, point);
	notify(ShapeMessage(ShapeMessage::EXTENDED, id, point));
}

int ShapeManager::cut_shape_impl(const Path& trace, const Path& shape, int s1, int s2, Info::ShapeDirection dir, int id)
{
	Path p(trace);
	p.closed = true;
	bool only_forward = false,
	     only_reverse = false;
	if (s1 == s2) {
		// distances between endpoints (beginning, end) of the trace and s1(=s2)
		const double beg_dist = (*shape.nth_point(s1) - *trace.begin()).length(),
		             end_dist = (*shape.nth_point(s1) - *trace.nth_point(trace.size()-1)).length();
		if (beg_dist < end_dist)
			only_forward = true;
		else
			only_reverse = true;
	}
	if (dir == Direction::FORWARD && !only_reverse) {
		const Path::const_iterator it_end = s1 != s2 ?
			shape.nth_point(s1+1) :
			shape.nth_point(s1+1).next_cycle();
		for (Path::const_iterator it = shape.nth_point(s2+1); it != it_end; ++it)
			p.push_back(*it);
	} else if (dir == Direction::REVERSE && !only_forward) {
		const Path::const_reverse_iterator it_end = s1 != s2 ?
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
throw (out_of_range)
{
	const Path& trace = get_shape_ref(trace_id).get_path();
	const Path& shape = get_shape_ref(id).get_path();
	pair<int, int> ids(-1, -1);
	ids.first  = cut_shape_impl(trace, shape, s1, s2, Direction::FORWARD),
	notify(ShapeMessage(ShapeMessage::CREATED, ids.first,
				ShapeCreationInfo(trace_id, id, s2, s1, Direction::FORWARD)));
	ids.second = cut_shape_impl(trace, shape, s1, s2, Direction::REVERSE);
	notify(ShapeMessage(ShapeMessage::CREATED, ids.second,
				ShapeCreationInfo(trace_id, id, s2, s1, Direction::REVERSE)));
	return ids;
}

void ShapeManager::destroy_shape_impl(int id)
throw (out_of_range)
{
	const auto shape_it = shapes.find(id);
	if (shape_it == shapes.end())
		throw out_of_range("No such shape in destroy_shape");
	delete shape_it->second;
	shapes.erase(shape_it);
}
void ShapeManager:: destroy_shape(int id)
throw (out_of_range)
{
	destroy_shape_impl(id);
	notify(ShapeMessage(ShapeMessage::DESTROYED, id));
}

void ShapeManager::update(Observable<ShapeMessage>& obj, const ShapeMessage& msg)
{
	switch (msg.type) {
	case ShapeMessage::CREATED:
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
	case ShapeMessage::DESTROYED:
		destroy_shape_impl(msg.id);
		break;
	case ShapeMessage::EXTENDED:
		extend_trace_impl(msg.id, *msg.extension_point);
		break;
	}
}
