#pragma once

//! @file
//! \brief Implementation of a shape manager which can notify about changes in
//! its shape repository using the observer pattern

#include "geometry.h"
#include "detector.h"
#include "observer.h"
#include <unordered_map>
#include <stdexcept>

//! A path with an optional detector
class Shape {
private:
	Path path;
	Detector *detector;
	void initialize(bool with_detector);
public:
	Shape(const Path& _path, bool with_detector);
	Shape(Path&& _path, bool with_detector);
	const Path& get_path() const { return path; }
	const Detector& get_detector() const { return *detector; }
	void extend(const Point& point) throw (std::domain_error);
	~Shape();
};

enum ShapeMessageType {
	CREATED,
	DESTROYED,
	EXTENDED,
	DUMMY
};
enum ShapeDirection {
	FORWARD,
	REVERSE
};
class ShapeCreationInfo {
public:
	int trace_id;
	int shape_id;
	int shape_start,
	    shape_end;
	ShapeDirection shape_dir;
	Path *path;
	ShapeCreationInfo(const ShapeCreationInfo& o);
	ShapeCreationInfo(int _trace_id, int _shape_id, int _shape_start, int _shape_end, enum ShapeDirection shape_dir);
	ShapeCreationInfo(const Path& _path);
	ShapeCreationInfo();
	const ShapeCreationInfo& operator=(const ShapeCreationInfo& o);
	~ShapeCreationInfo();
};
class ShapeMessage {
public:
	ShapeMessageType type;
	int id;
	ShapeCreationInfo* info;
	Point *extension_point;
	ShapeMessage(ShapeMessageType _type, int _id, const ShapeCreationInfo& _info);
	ShapeMessage(ShapeMessageType _type, int _id, const Point& _extension_point);
	ShapeMessage(ShapeMessageType _type, int _id);
	ShapeMessage();
	ShapeMessage(const ShapeMessage& o);
	const ShapeMessage& operator=(const ShapeMessage& o);
	~ShapeMessage();
};

class ShapeManager : public Observable<ShapeMessage>, public Observer<ShapeMessage> {
private:
	bool with_detector;
	std::unordered_map<int, Shape*> shapes;
	int last_id;

	int cut_shape_impl(const Path& trace, const Path& shape, int s1, int s2, ShapeDirection dir, int id = -1);
	int add_shape_impl(const Path& path, int id = -1);
	void extend_trace_impl(int id, const Point& point);
	void destroy_shape_impl(int id);
	Shape& get_shape_ref(int id) throw (std::out_of_range);
public:
	virtual void update(const Observable<ShapeMessage>& obj, const ShapeMessage& msg);
	int num_shapes() const { return shapes.size(); }
	const Shape& get_shape_const_ref(int id) const throw (std::out_of_range);
	ShapeManager(bool _with_detector);
	~ShapeManager();
	int add_shape(const Path& path);
	int start_trace(const Point& point);
	void extend_trace(int id, const Point& point);
	std::pair<int, int> cut_shape(int trace_id, int id, int s1, int s2);
	void destroy_shape(int id);
	std::unordered_map<int, Shape*>::const_iterator begin() const { return shapes.begin(); }
	std::unordered_map<int, Shape*>::const_iterator end() const { return shapes.end(); }
};
