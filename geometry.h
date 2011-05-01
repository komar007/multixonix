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
	bool operator==(const Point& o)
	{
		return x == o.x && y == o.y;
	}
};

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
	bool closed;
	using std::vector<Point>::size;
	using std::vector<Point>::push_back;
	using std::vector<Point>::reserve;

	class cyclic_iterator {
	private:
		const std::vector<Point>& p;
		int i;
		short cycles;
		cyclic_iterator(const std::vector<Point>& _p, int start, short _cycles)
			: p(_p)
			, i(start)
			, cycles(_cycles)
		{
		}
	public:
		cyclic_iterator(const std::vector<Point>& _p, int start)
			: p(_p)
			, i(start)
			, cycles(0)
		{
		}
		cyclic_iterator& operator++()
		{
			if (++i == (int)p.size()) {
				i = 0;
				++cycles;
			}
			return *this;
		}
		const Point& operator*() const
		{
			return p[i];
		}
		bool operator==(const cyclic_iterator& o) const
		{
			return  i == o.i ||
				(o.i == (int)p.size() && i == 0 && cycles > 0) ||   	// o is end
				(i == (int)o.p.size() && o.i == 0 && o.cycles > 0);	// *this is end
		}
		bool operator!=(const cyclic_iterator& o) const
		{
			return !operator==(o);
		}

		//! Iterator move.
		cyclic_iterator operator+(size_t o) const
		{
			int it = i + o;
			if (it >= (int)p.size())
				return cyclic_iterator(p, it - p.size(), cycles + 1);
			else
				return cyclic_iterator(p, it);
		}
	};
	typedef cyclic_iterator const_iterator;

	Path(bool _closed = true)
		: std::vector<Point>()
		, closed(_closed)
	{
	}
	const_iterator nth_point(size_t n) const
	{
		return const_iterator(*this, n);
	}
	const_iterator end() const
	{
		return nth_point(size());
	}
	const_iterator begin() const
	{
		return nth_point(0);
	}
};

double turning_determinant(const Point &p1, const Point &p2, const Point &p3);
double line_point_distance(const Point& l1, const Point& l2, const Point& p);
bool point_in_rect(const Point& p, const Point& r1, const Point& r2);
