#pragma once

//! @file
//! \brief Shape class, representing the game's world

#include "geometry.h"
#include "detector.h"
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
	bool point_inside(const Point& p);
	int intersections(const Point& s1, const Point& s2, std::vector<Detector::Intersection>& out_edges) const throw (std::logic_error);
	void extend(const Point& point) throw (std::domain_error);
	friend class ShapeManager;
};
