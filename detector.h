#pragma once

//! @file
//! \brief Path-segment collision detectection structure

#include "geometry.h"
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <deque>

class BfsPainter;

//! \brief Collision detection structure
//!
//! Structure used to quickly detect collisions between a path and a segment
class Detector {
public:
	typedef std::pair<Path::const_iterator, Point> Intersection;
private:
	typedef std::unordered_multimap<Location, Path::const_iterator> hash_type;
	class manhattan_point_sorter;
	hash_type hash;
	const Path& p;
	const Vector offset;
	const double block_size;
	Rect box;

	void add_segment_impl(const Path::const_iterator& pit);
	void update_bounding_box(const Path::const_iterator& pit);
public:
	//! Construct a detector for a path
	//!
	//! @param _p path
	//! @param _offset point in which the relative block's coordinates
	//! will start
	//! @param _block_size size of block's side
	Detector(const Path& _p, const Vector& _offset, double _block_size);
	//! \brief Add path's segment to the structure
	//!
	//! This is used to add a segment to the structure if the path has
	//! changed since the time the Detector was created (this happens in
	//! case of traces, which expand)
	//! @param pit iterator to segment's start point
	void add_segment(const Path::const_iterator& pit);
	const Rect& get_box() const { return box; }
	//! \brief find intersections between traced path and a segment
	//!
	//! @param p1,p2 start and end point of the segment
	//! @param out_edges reference to vector of intersections, where iterators
	//! to intersected edges will be stored together with intersection
	//! points
	//! @return number of intersections
	int segment_intersections(const Point& s1, const Point& s2, std::vector<Intersection>& out_edges) const;
	//! \brief Find location (coordinates) of a block containing a point
	//!
	//! @param p point
	//! @return block's location
	Location to_location(const Point& p) const;
	//! \brief Find coordinates of a block's origin
	//!
	//! @param l block's location
	//! @return block's origin's coordinates
	Point to_point(const Location& l) const;
	friend class BfsPainter;
};

//! \brief A BFS rasterizer state machine
//!
//! A BFS segment drawing algorithm implementation as a state machine used to
//! iterate over all squares which intersect with a line segment
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
	//! \brief Construct a BfsPainter
	//!
	//! @param a,b endpoints of the segment to rasterize
	//! @param _detector Detector from which to take information about
	//! block size and offset
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
