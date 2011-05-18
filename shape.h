#pragma once

#include "geometry.h"
#include "detector.h"

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
