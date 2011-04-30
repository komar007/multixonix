#pragma once

#include "utility.h"
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
};

bool operator==(const Point& a, const Point& b);

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
	Vector(float _dx, float _dy)
		: dx(_dx)
		, dy(_dy)
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
	double length() const
	{
		return hypot(dx, dy);
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
//! Translate point by minus vector
inline Point operator -(const Point& p, const Vector& v)
{
	return Point(p.x - v.dx, p.y - v.dy);
}
//@}

//! \brief Geometric path/shape
//!
//! Defines a geometric path/shape as a set of points
class Path : private std::vector<Point> {
public:
	typedef cyclic_iterator<std::vector<Point>::iterator> iterator;
	typedef cyclic_iterator<std::vector<Point>::const_iterator> const_iterator;
	bool closed;
	using std::vector<Point>::size;
	using std::vector<Point>::push_back;
	using std::vector<Point>::reserve;
	Path(bool _closed = true)
		: std::vector<Point>()
		, closed(_closed)
	{
	}
	iterator nth_point(size_t n)
	{
		return iterator(std::vector<Point>::begin(),
				std::vector<Point>::end(),
				std::vector<Point>::begin() + n);
	}
	iterator end()
	{
		return iterator(std::vector<Point>::begin(),
				std::vector<Point>::end(),
				std::vector<Point>::end());
	}
	iterator begin()
	{
		return nth_point(0);
	}
	const_iterator nth_point(size_t n) const
	{
		return const_iterator(std::vector<Point>::begin(),
				std::vector<Point>::end(),
				std::vector<Point>::begin() + n);
	}
	const_iterator end() const
	{
		return const_iterator(std::vector<Point>::begin(),
				std::vector<Point>::end(),
				std::vector<Point>::end());
	}
	const_iterator begin() const
	{
		return nth_point(0);
	}
};

double turning_determinant(const Point &p1, const Point &p2, const Point &p3);
bool point_in_path(const Point&, const Path&) throw (std::domain_error);
double line_point_distance(const Point& l1, const Point& l2, const Point& p);
