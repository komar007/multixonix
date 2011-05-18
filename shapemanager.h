#pragma once

//! @file
//! \brief Implementation of a shape manager which can notify about changes in
//! its shape repository using the observer pattern

#include "geometry.h"
#include "detector.h"
#include "observer.h"
#include <unordered_map>
#include <stdexcept>

class ShapeManager;

//! A path with an optional detector
class Shape {
private:
	int id;
	Path path;
	Detector *detector;

	void initialize(bool with_detector);
	void set_id(int id);
public:
	Shape(const Path& _path, bool with_detector);
	Shape(Path&& _path, bool with_detector);
	~Shape();
	int get_id() const { return id; }
	const Path& get_path() const { return path; }
	const Detector& get_detector() const { return *detector; }
	void extend(const Point& point) throw (std::domain_error);
	friend class ShapeManager;
};

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

class ShapeManager : public Observable<ShapeMessage>, public Observer<ShapeMessage> {
private:
	typedef ShapeCreationInfo Info;
	typedef Info::ShapeDirection Direction;
	bool with_detector;
	std::unordered_map<int, Shape*> shapes;
	int last_id;

	int add_shape_impl(const Path& path, int id = -1);
	void destroy_shape_impl(int id);
	void extend_trace_impl(int id, const Point& point);
	int cut_shape_impl(const Path& trace, const Path& shape, int s1, int s2, ShapeCreationInfo::ShapeDirection dir, int id = -1);
	Shape& get_shape_ref(int id) throw (std::out_of_range);
public:
	ShapeManager(bool _with_detector);
	~ShapeManager();
	virtual void update(const Observable<ShapeMessage>& obj, const ShapeMessage& msg);

	int num_shapes() const { return shapes.size(); }
	const Shape& get_shape_const_ref(int id) const throw (std::out_of_range);
	int add_shape(const Path& path);
	void destroy_shape(int id);
	int start_trace(const Point& point);
	void extend_trace(int id, const Point& point);
	std::pair<int, int> cut_shape(int trace_id, int id, int s1, int s2);

	class iterator {
	private:
		std::unordered_map<int, Shape*>::const_iterator i;
	public:
		iterator(const std::unordered_map<int, Shape*>::const_iterator& _i)
			: i(_i)
		{
		}
		iterator& operator++()
		{
			++i;
			return *this;
		}
		const Shape& operator*()
		{
			return *i->second;
		}
		const Shape* operator->()
		{
			return i->second;
		}
		bool operator==(const iterator& o)
		{
			return i == o.i;
		}
		bool operator!=(const iterator& o)
		{
			return i != o.i;
		}
	};
	iterator begin() const { return iterator(shapes.begin()); }
	iterator end() const { return iterator(shapes.end()); }
};
