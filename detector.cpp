#include "detector.h"
#include "collision.h"
#include <algorithm>

using namespace std;

Detector::Detector(const Path& _p, const Vector& _offset, double _block_size)
	: hash()
	, p(_p)
	, offset(_offset)
	, block_size(_block_size)
	, box(Point(pinf, pinf), Point(ninf, ninf))
{
	for (Path::const_iterator i = p.begin(); i != p.end(); ++i) {
		add_segment_impl(i);
		update_bounding_box(i);
	}
	if (!p.closed && p.size() > 0)
		update_bounding_box(p.end());
}

void Detector::add_segment_impl(const Path::const_iterator& pit)
{
	BfsPainter pt(*pit, *(pit+1), *this);
	for (BfsPainter::iterator l = pt.begin(); l != pt.end(); ++l)
		hash.insert(hash_type::value_type(*l, pit));
}
void Detector::add_segment(const Path::const_iterator& pit)
{
	add_segment_impl(pit);
	update_bounding_box(pit+1);
}

void Detector::update_bounding_box(const Path::const_iterator& pit)
{
	if (pit->x < box.first.x)
		box.first.x = pit->x;
	if (pit->x > box.second.x)
		box.second.x = pit->x;
	if (pit->y < box.first.y)
		box.first.y = pit->y;
	if (pit->y > box.second.y)
		box.second.y = pit->y;
}

typedef take_second<Location, Path::const_iterator> take_iter;
typedef unordered_set<Path::const_iterator> iterator_set;

class Detector::manhattan_point_sorter {
private:
	Point origin;
public:
	manhattan_point_sorter(const Point& _origin)
		: origin(_origin)
	{
	}
	bool operator()(const Intersection& i1, const Intersection& i2)
	{
		const double dist1 = abs(i1.second.x - origin.x) + abs(i1.second.y - origin.y),
		             dist2 = abs(i2.second.x - origin.x) + abs(i2.second.y - origin.y);
		return dist1 < dist2;
	}
};

int Detector::segment_intersections(const Point& s1, const Point& s2, vector<Intersection>& out_edges) const
{
	out_edges.clear();
	if (!point_in_rect(s1, box) && !point_in_rect(s2, box))
		return 0;
	BfsPainter pt(s1, s2, *this);
	iterator_set p_iterators;
	insert_iterator<iterator_set> inserter(p_iterators, p_iterators.end());
	for (BfsPainter::iterator b = pt.begin(); b != pt.end(); ++b) {
		auto range = hash.equal_range(*b);
		transform(range.first, range.second, inserter, take_iter());
	}
	int nintersections = 0;
	for (auto i = p_iterators.begin(); i != p_iterators.end(); ++i) {
		const Path::const_iterator& p1 = *i;
		const Path::const_iterator& p2 = *i + 1;
		if (segment_path_segment_collision(s1, s2, *p1, *p2)) {
			++nintersections;
			Point inter = segment_segment_intersection(s1, s2, *p1, *p2);
			out_edges.push_back(make_pair(p1, inter));
		}
	}
	sort(out_edges.begin(), out_edges.end(), manhattan_point_sorter(s1));
	return nintersections;
}

Location Detector::to_location(const Point& p) const
{
	const Point rel_p = p - offset;
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

// FIXME: optimize
Location BfsPainter::next()
{
	const Location ret = queue.front();
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
