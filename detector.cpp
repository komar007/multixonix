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
	, segment_a(a)
	, segment_b(b)
{
	if (segment_a.y > segment_b.y)
		swap(segment_a, segment_b);
	Vector hypotenuse = segment_b - segment_a;
	double len = hypotenuse.length();
	double sin = hypotenuse.dy / len,
	       cos = abs(hypotenuse.dx) / len;
	dist = detector.block_size / 2 * (sin + cos);
	loc_beg = detector.to_location(segment_a);
	loc_end = detector.to_location(segment_b);
	max_x = max(loc_beg.x, loc_end.x);
	min_x = min(loc_beg.x, loc_end.x);
	queue.push_back(loc_beg);
	visited.insert(loc_beg);
}

Location BfsPainter::next()
{
	Location ret = queue.front();
	queue.pop_front();
	Location nb;
	for (nb.y = ret.y; nb.y <= ret.y + 1; ++nb.y) {
		for (nb.x = ret.x - 1; nb.x <= ret.x + 1; ++nb.x) {
			if (nb.y > loc_end.y || nb.x < min_x || nb.x > max_x ||
					visited.find(nb) != visited.end())
				continue;
			visited.insert(nb);
			if (line_point_distance(segment_a, segment_b, detector.to_point(nb)) <= dist)
				queue.push_back(nb);
		}
	}
	return ret;
}
