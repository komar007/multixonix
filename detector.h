#pragma once

//! @file
//! \brief Path-segment collision detectection structure

#include "geometry.h"
#include <unordered_map>
#include <unordered_set>
#include <deque>
#include <algorithm>
#include <stdexcept>

//! \brief Collision detection structure
//!
//! Structure used to quickly detect collisions between a path and a segment
class Detector {
private:
	typedef std::unordered_multimap<Location, Path::const_iterator> hash_type;
	hash_type hash;
	const Path& p;
	void add_segment(const Path::const_iterator& pit);
public:
	const Vector offset;
	const double block_size;
	Detector(const Path& _p, const Vector& _offset, double _block_size);
	int segment_intersections(const Point& p1, const Point& p2);
	const hash_type& get_hash() { return hash; }
	Location to_location(const Point& p) const;
	Point to_point(const Location& l) const;
};

//! \brief A BFS rasterizer state machine
//!
//! A BFS segment drawing algorithm implementation as a state machine used to
//! iterate over all squares which intersect with a segment
class BfsPainter {
private:
	const Detector& detector;
	const Point segment_a,
	            segment_b;
	const Point rect_p1, rect_p2;
	double dist;
	std::deque<Location> queue;
	std::unordered_set<Location> visited;
	Location next();
public:
	BfsPainter(const Point& a, const Point& b, const Detector& _detector);

	class iterator : public std::iterator<std::input_iterator_tag, Location> {
	private:
		BfsPainter* dp;
	public:
		iterator(BfsPainter* _dp)
			: dp(_dp)
		{
		}
		const Location& operator*() const
		{
			return dp->queue.front();
		}
		bool operator==(const iterator& o) const
		{
			return (!o.dp && !dp) || (o.dp && dp && **this == *o);
		}
		bool operator!=(const iterator& o) const
		{
			return !operator==(o);
		}
		iterator& operator++()
		{
			dp->next();
			if (dp->queue.empty())
				dp = NULL;
			return *this;
		}
	};
	iterator begin()
	{
		return iterator(this);
	}
	iterator end()
	{
		return iterator(NULL);
	}
	friend class iterator;
};
