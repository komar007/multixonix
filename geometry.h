#pragma once

#include "utility.h"
#include "iterators.h"
#include <vector>
#include <unordered_map>
#include <limits>
#include <cmath>
#include <stdexcept>

//! @file
//! \brief Plane geometry primitives support

class Vector;

//! \brief A 2D point
//!
//! A basic data structure representing a point in a 2-dimensional space.
class Point {
public:
	double x;
	double y;

	Point()
		: x(.0f)
		, y(.0f)
	{
	}
	Point(float _x, float _y)
		: x(_x)
		, y(_y)
	{
	}
	bool operator==(const Point& o) const
	{
		return x == o.x && y == o.y;
	}
	bool operator!=(const Point& o) const
	{
		return !operator==(o);
	}
};

//! \brief A 2D integer point
//!
//! A basic data structure representing a pair of integer coordinates
class Location {
public:
	int x;
	int y;

	Location()
		: x(0)
		, y(0)
	{
	}
	Location(int _x, int _y)
		: x(_x)
		, y(_y)
	{
	}
	bool operator==(const Location& o) const
	{
		return x == o.x && y == o.y;
	}
	bool operator!=(const Location& o) const
	{
		return !operator==(o);
	}
};

namespace std {
	template <>
	class hash<Location> : private std::hash<long> {
	public:
		size_t operator()(const Location& loc) const
		{
			return std::hash<long>::operator()(
					(long)loc.y * std::numeric_limits<int>::max() + loc.x);
		}
	};
};

//! \brief A 2D vector
//!
//! A basic data structure representing a vector in a 2-dimensional space.
class Vector {
public:
	double dx;
	double dy;

	Vector()
		: dx(.0f)
		, dy(.0f)
	{
	}
	Vector(double _dx, double _dy)
		: dx(_dx)
		, dy(_dy)
	{
	}
	Vector(const Point& p1, const Point& p2)
		: dx(p2.x - p1.x)
		, dy(p2.y - p1.y)
	{
	}

	//! Vector sum
	Vector operator +(const Vector& o)
	{
		return Vector(dx + o.dx, dy + o.dy);
	}
	//! Vector difference
	Vector operator -(const Vector& o)
	{
		return Vector(dx - o.dx, dy - o.dy);
	}
	//! Scale by a scalar
	Vector operator*(double k)
	{
		return Vector(dx*k, dy*k);
	}
	double length() const
	{
		return hypot(dx, dy);
	}
	static Vector dir(double angle)
	{
		return Vector(cos(angle), sin(angle));
	}
};

//! @name Basic binary operations
//@{
//! Find vector between 2 points
inline Vector operator -(const Point& l, const Point& r)
{
	return Vector(l.x - r.x, l.y - r.y);
}
//! Translate point by vector
inline Point operator +(const Point& p, const Vector& v)
{
	return Point(p.x + v.dx, p.y + v.dy);
}
//! Translate point by vector
inline Point& operator +=(Point& p, const Vector& v)
{
	p.x += v.dx;
	p.y += v.dy;
	return p;
}
//! Translate point by minus vector
inline Point operator -(const Point& p, const Vector& v)
{
	return Point(p.x - v.dx, p.y - v.dy);
}
//! Translate point by minus vector
inline Point& operator -=(Point& p, const Vector& v)
{
	p.x -= v.dx;
	p.y -= v.dy;
	return p;
}
//@}

typedef std::pair<Point, Point> Rect;

//! \brief Geometric path/shape
//!
//! Defines a geometric path/shape as a set of points
class Path : private std::vector<Point> {
public:
	bool closed;
	using std::vector<Point>::size;
	using std::vector<Point>::push_back;
	using std::vector<Point>::reserve;

	typedef forward_cyclic_iterator<Point> const_iterator;
	typedef reverse_cyclic_iterator<Point> const_reverse_iterator;

	Path(bool _closed = true, const std::vector<Point>& v = std::vector<Point>())
		: std::vector<Point>(v)
		, closed(_closed)
	{
	}
	Path(const Path& o)
		: std::vector<Point>(o)
		, closed(o.closed)
	{
	}
	Path(Path&& o)
		: std::vector<Point>(move(o))
		, closed(o.closed)
	{
	}
	//! \brief iterator to point number n
	//!
	//! @param n point's index
	//! @return iterator to the nth point
	const_iterator nth_point(size_t n) const
	{
		return const_iterator(*this, 0) + n;
	}
	//! \brief end of the path
	//!
	//! @return if the path is closed, const_iterator to element one behind the end
	//! @return if the path is open, const_iterator to the last element
	const_iterator end() const
	{
		if (closed) {
			if (size() > 0)
				return nth_point(0).next_cycle();
			else
				return nth_point(0);
		} else {
			return nth_point(size() - 1);
		}
	}
	//! \brief beginning of the path
	//!
	//! @return const_iterator to the first point
	const_iterator begin() const
	{
		return nth_point(0);
	}
};

//! Path comparison
//! \relates Path
inline bool operator==(const Path& p1, const Path& p2)
{
	if (p1.size() != p2.size())
		return false;
	if (p1.closed != p2.closed)
		return false;
	for (auto i = p1.begin(), j = p2.begin(); i != p1.end() && j != p2.end(); ++i, ++j)
		if (*i != *j)
			return false;

	return true;
}

//! \brief Counts double signed area of a triangle described by three points
//!
//! @return positive value, if points p1, p2, p3 turn clockwise, negative if
//! they turn counter-clockwise, and 0 if points are colinear
double turning_determinant(const Point &p1, const Point &p2, const Point &p3);

//! \brief Distance between line and point;
//!
//! @param l1, l2 two distinct points on line
//! @param p point
//! @return distance between line l1l2 and point p
double line_point_distance(const Point& l1, const Point& l2, const Point& p);

//! \brief couns the area of a path
//!
//! @param path input path
//! @return path's nonnegative area
double path_area(const Path& path);

//! \brief converts an angle to a range from 0 to 2*M_PI
//!
//! @param angle input angle
//! @return angle in range [0, 2*M_PI)
double normalized_angle(double angle);
