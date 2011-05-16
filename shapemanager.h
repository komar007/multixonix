#pragma once

#include "geometry.h"
#include "detector.h"
#include "observer.h"
#include <unordered_map>
#include <stdexcept>

class Shape {
private:
	Path path;
	Detector *detector;
	void initialize(bool with_detector);
public:
	Shape(const Path& _path, bool with_detector);
	Shape(Path&& _path, bool with_detector);
	const Path& get_path() const;
	void extend(const Point& point) throw (std::domain_error);
	~Shape();
};

enum ShapeMessageType {
	CREATED,
	DESTROYED,
	DUMMY
};
class ShapeCreationInfo {
public:
	int trace_id;
	int shape_id;
	int shape_start,
	    shape_end;
	Path *path;
	ShapeCreationInfo(const ShapeCreationInfo& o);
	ShapeCreationInfo(int _trace_id, int _shape_id, int _shape_start, int _shape_end);
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
	ShapeMessage(ShapeMessageType _type, int _id, const ShapeCreationInfo& _info = ShapeCreationInfo());
	ShapeMessage();
	ShapeMessage(const ShapeMessage& o);
	const ShapeMessage& operator=(const ShapeMessage& o);
	~ShapeMessage();
};

class ShapeManager : public Observable<ShapeMessage> {
private:
	bool with_detector;
	std::unordered_map<int, Shape*> shapes;
	int last_id;

public:
	const Shape& get_shape(int id) const throw (std::out_of_range);
	Shape& get_shape(int id) throw (std::out_of_range);
	ShapeManager(bool _with_detector);
	int add_shape(const Path& path);
	int start_trace(const Point& point);
	void extend_trace(int, const Point& point);
	std::pair<int, int> cut_shape(int id, int s1, int s2, int trace_id);
};
