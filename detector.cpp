#include "detector.h"

using namespace std;

Detector::Detector(const Path& _p, const Vector& _offset, double _block_size)
	: hash()
	, p(_p)
	, offset(_offset)
	, block_size(_block_size)
{
	for (Path::const_iterator i = p.begin(), j = i+1; i != p.end(); ++i, ++j) {
		BfsPainter pt(*i, *j, *this);
		for (BfsPainter::iterator l = pt.begin(); l != pt.end(); ++l)
			hash.insert(hash_type::value_type(*l, i));
	}
}

Location Detector::to_location(const Point& p) const
{
	Point rel_p = p - offset;
	return Location(rel_p.x / block_size, rel_p.y / block_size);
}

Point Detector::to_point(const Location& l) const
{
	return Point(l.x * block_size, l.y * block_size) + offset +
		Vector(block_size/2, block_size/2);
}

BfsPainter::BfsPainter(const Point& a, const Point& b, const Detector& _detector)
	: detector(_detector)
	, segment_a(a.y <= b.y ? a : b)
	, segment_b(a.y <= b.y ? b : a)
	, rect_p1(min(segment_a.x, segment_b.x) - detector.block_size/2,
	          segment_a.y - detector.block_size/2)
	, rect_p2(max(segment_a.x, segment_b.x) + detector.block_size/2,
	          segment_b.y + detector.block_size/2)
{
	const Vector hypotenuse = segment_b - segment_a;
	const double len = hypotenuse.length();
	const double sin = hypotenuse.dy / len;
	const double cos = abs(hypotenuse.dx) / len;
	dist = detector.block_size / 2 * (sin + cos);
	Location loc_beg = detector.to_location(segment_a);
	queue.push_back(loc_beg);
	visited.insert(loc_beg);
}

Location BfsPainter::next()
{
	Location ret = queue.front();
	queue.pop_front();
	Location nb;
	for (nb.y = ret.y - 1; nb.y <= ret.y + 1; ++nb.y) {
		for (nb.x = ret.x - 1; nb.x <= ret.x + 1; ++nb.x) {
			const Point center = detector.to_point(nb);
			if (!point_in_rect(center, rect_p1, rect_p2) || visited.find(nb) != visited.end())
				continue;
			visited.insert(nb);
			if (line_point_distance(segment_a, segment_b, center) <= dist)
				queue.push_back(nb);
		}
	}
	return ret;
}
