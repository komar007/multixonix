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
	void destroy_shape_impl(int id) throw (std::out_of_range);
	void extend_trace_impl(int id, const Point& point) throw (std::out_of_range);
	int cut_shape_impl(const Path& trace, const Path& shape, int s1, int s2, Direction dir, int id = -1);
public:
	ShapeManager(bool _with_detector);
	~ShapeManager();
	int num_shapes() const { return shapes.size(); }
	//! \brief Returns const-reference to a shape of given id
	//!
	//! @param id shape's id in ShapeManager
	//! @return const-reference to shape
	const Shape& get_shape_const_ref(int id) const throw (std::out_of_range);
	//! \brief Adds a new shape made of a path to ShapeManager
	//!
	//! @param path to be used ad new shape in ShapeManager
	//! @return added shape's id
	int add_shape(const Path& path);
	//! \brief Removes a shape from ShapeManager
	//!
	//! @param id shape's id in ShapeManager
	void destroy_shape(int id) throw (std::out_of_range);
	//! \brief Adds a trace with one point to ShapeManager
	//!
	//! @param point trace's starting point
	//! @return created trace's id
	int start_trace(const Point& point);
	//! \brief Adds a point to the end of a trace
	//!
	//! @param point point to be added
	void extend_trace(int id, const Point& point) throw (std::out_of_range);
	//! \brief Cuts a shape into two
	//!
	//! @param trace_id id of the cutting trace
	//! @param id id of cut shape
	//! @param s1 index of shape's edge, where the trace starts
	//! @param s2 index of shape's edge, where the trace ends
	//! @return created trace's id
	std::pair<int, int> cut_shape(int trace_id, int id, int s1, int s2) throw (std::out_of_range);

	virtual void update(Observable<ShapeMessage>& obj, const ShapeMessage& msg);

	typedef const_pointer_map_iterator<Shape> iterator;
	iterator begin() const { return iterator(shapes.begin()); }
	iterator end() const { return iterator(shapes.end()); }
};
