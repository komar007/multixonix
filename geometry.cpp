#include "geometry.h"
#include "utility.h"

using namespace std;

bool operator==(const Point& a, const Point& b)
{
	return a.x == b.x && a.y == b.y;
}

double turning_determinant(const Point &p1, const Point &p2, const Point &p3)
{
	double h = p1.x*p2.y - p2.x*p1.y +
	           p3.x*p1.y - p1.x*p3.y +
	           p2.x*p3.y - p3.x*p2.y;
	return h;
}

double line_point_distance(const Point& l1, const Point& l2, const Point& p)
{
	return abs(turning_determinant(l1, l2, p)) / (l2 - l1).length();
}

bool point_in_path(const Point &p, const Path &path) throw (domain_error)
{
	if (!path.closed)
		throw domain_error("open path in point_in_path");
	if (path.size() < 3)
		throw domain_error("degenerated path in point_in_path");
	int intersections = 0;
	for (auto i = path.begin(), j = path.nth_point(1); i != path.end(); ++i, ++j) {
		const Point *p1 = &(*i), *p2 = &(*j);
		if (p1->y > p2->y)
			swap(p1, p2);
		int h = turning_determinant(*p1, *p2, p);
		if (h == 0 && p1->y <= p.y && p.y <= p2->y &&
				min(p1->x, p2->x) <= p.x && p.x <= max(p1->x, p2->x))
			return true;
		if (p1->y <= p.y && p2->y > p.y && h == -1)
			++intersections;
	}
	return intersections % 2 == 1;
}
