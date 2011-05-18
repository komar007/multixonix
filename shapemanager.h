#pragma once

//! @file
//! \brief Implementation of a shape manager which can notify about changes in
//! its shape repository using the observer pattern

#include "geometry.h"
#include "observer.h"
#include "shape.h"
#include "messages.h"
#include <unordered_map>
#include <stdexcept>

class ShapeManager : public Observable<ShapeMessage>, public Observer<ShapeMessage> {
private:
	typedef ShapeCreationInfo Info;
	typedef Info::ShapeDirection Direction;
	bool with_detector;
	std::unordered_map<int, Shape*> shapes;
	int last_id;

	Shape& get_shape_ref(int id) throw (std::out_of_range);
	int add_shape_impl(const Path& path, int id = -1);
	void destroy_shape_impl(int id);
	void extend_trace_impl(int id, const Point& point);
	int cut_shape_impl(const Path& trace, const Path& shape, int s1, int s2, Direction dir, int id = -1);
public:
	ShapeManager(bool _with_detector);
	~ShapeManager();

	int num_shapes() const { return shapes.size(); }
	const Shape& get_shape_const_ref(int id) const throw (std::out_of_range);
	int add_shape(const Path& path);
	void destroy_shape(int id);
	int start_trace(const Point& point);
	void extend_trace(int id, const Point& point);
	std::pair<int, int> cut_shape(int trace_id, int id, int s1, int s2);
	virtual void update(const Observable<ShapeMessage>& obj, const ShapeMessage& msg);

	typedef const_pointer_map_iterator<Shape> iterator;
	iterator begin() const { return iterator(shapes.begin()); }
	iterator end() const { return iterator(shapes.end()); }
};
