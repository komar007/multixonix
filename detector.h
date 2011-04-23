#pragma once

#include "geometry.h"
#include <unordered_map>
#include <unordered_set>
#include <deque>
#include <algorithm>
#include <stdexcept>

class Detector : private std::unordered_multimap<Location, Path::iterator, Location::hash> {
private:
	const Path& p;
public:
	const Vector offset;
	const double block_size;
	Detector(const Path& _p, const Vector& _offset, double _block_size);
	Location to_location(const Point& p) const;
	Point to_point(const Location& l) const;
};

class dfs_painter {
private:
	const Detector& detector;
	Point segment_a,
	      segment_b;
	Location loc_beg,
	         loc_end;
	int min_x, max_x;
	std::deque<Location> queue;
	std::unordered_set<Location, Location::hash> visited;
	double dist;
	Location next();
public:
	dfs_painter(const Point& a, const Point& b, const Detector& _detector);

	class iterator {
	private:
		dfs_painter* dp;
	public:
		iterator(dfs_painter* _dp)
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
