#pragma once

#include "shapemanager.h"
#include <unordered_set>
#include <iterator>

template <typename T, typename H>
inline std::ostream& operator<<(std::ostream& o, const std::unordered_set<T, H>& s)
{
	std::ostream_iterator<T> it(o, ", ");
	std::copy(s.begin(), s.end(), it);
	return o;
}

inline std::ostream& operator<<(std::ostream& o, const Location& loc)
{
	o << "(" << loc.x << ", " << loc.y << ")";
	return o;
}

inline std::ostream& operator<<(std::ostream& o, const Point& p)
{
	o << "(" << p.x << ", " << p.y << ")";
	return o;
}

inline std::ostream& operator<<(std::ostream& o, const ShapeMessageType& m)
{
	switch (m) {
	case CREATED:
		o << "CREATED"; break;
	case DESTROYED:
		o << "DESTROYED"; break;
	case DUMMY:
		o << "DUMMY"; break;
	}
	return o;
}

template <typename T, typename H>
inline bool operator==(const std::unordered_set<T, H>& s1, const std::unordered_set<T, H>& s2)
{
	if (s1.size() != s2.size())
		return false;
	for (auto i = s1.begin(); i != s1.end(); ++i)
		if (s2.find(*i) == s2.end())
			return false;
	return true;
}
