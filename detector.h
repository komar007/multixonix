#pragma once

#include "geometry.h"
#include <unordered_map>
#include <unordered_set>
#include <deque>
#include <algorithm>
#include <stdexcept>

class Detector {
private:
	typedef std::unordered_multimap<Location, Path::const_iterator> hash_type;
	hash_type hash;
	const Path& p;
public:
	const Vector offset;
	const double block_size;
	Detector(const Path& _p, const Vector& _offset, double _block_size);
	const hash_type& get_hash() { return hash; }
	Location to_location(const Point& p) const;
	Point to_point(const Location& l) const;
};

class BfsPainter {
private:
	const Detector& detector;
	Point segment_a,
	      segment_b;
	Location loc_beg,
	         loc_end;
	int min_x, max_x;
	std::deque<Location> queue;
	std::unordered_set<Location> visited;
	double dist;
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
