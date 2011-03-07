#include "geometry.h"

using namespace std;

int helicity(const Point &p1, const Point &p2, const Point &p3)
{
	double h = p1.x*p2.y - p2.x*p1.y +
	           p3.x*p1.y - p1.x*p3.y +
	           p2.x*p3.y - p3.x*p2.y;
	return h < 0 ? -1 : h == 0 ? 0 : 1;
}

bool point_in_path(const Point &p, const Path &path) throw (domain_error)
{
	if (!path.closed)
		throw domain_error("open path in point_in_path");
	if (path.size() < 3)
		throw domain_error("degenerated path in point_in_path");
	int intersections = 0;
	for (unsigned i = 0; i < path.size(); ++i) {
		const Point *p1 = &path[i], *p2 = &path[(i+1) % path.size()];
		if (p1->y > p2->y)
			swap(p1, p2);
		int h = helicity(*p1, *p2, p);
		if (h == 0)
			return true;
		if (p1->y <= p.y && p2->y > p.y && h == -1)
			++intersections;
	}
	return intersections % 2 == 1;
}
