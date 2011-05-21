#include "collision.h"

bool point_in_rect(const Point& p, const Point& r1, const Point& r2)
{
	return r1.x <= p.x && p.x <= r2.x && r1.y <= p.y && p.y <= r2.y;
}
bool point_in_rect(const Point& p, const Rect& r)
{
	return point_in_rect(p, r.first, r.second);
}

bool line_segment_collision(const Point &l1, const Point &l2, const Point &s1, const Point &s2)
{
	const int h1 = turning_determinant(l1, l2, s1);
	return h1 == 0 || h1 * turning_determinant(l1, l2, s2) < 0;
}

bool segment_path_segment_collision(const Point& s1, const Point& s2, const Point& p1, const Point& p2)
{
	const double t1 = turning_determinant(p1, p2, s1),
	             t2 = turning_determinant(p1, p2, s2);
	const double t3 = turning_determinant(s1, s2, p1),
	             t4 = turning_determinant(s1, s2, p2);
	return (t1 * t2 < 0 || (t1 == 0 && t2 != 0)) &&
	       (t3 * t4 < 0 || (t3 == 0 && t4 > 0) || (t4 == 0 && t3 > 0));
}

Point segment_segment_intersection(const Point& a1, const Point& a2, const Point& b1, const Point& b2)
{
	// from: http://paulbourke.net/geometry/lineline2d/
	const double ua = ((b2.x - b1.x)*(a1.y - b1.y) - (b2.y - b1.y)*(a1.x - b1.x)) /
		((b2.y - b1.y)*(a2.x - a1.x) - (b2.x - b1.x)*(a2.y - a1.y));
	return Point(a1.x + ua*(a2.x - a1.x), a1.y + ua*(a2.y - a1.y));
}
