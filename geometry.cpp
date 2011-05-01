#include "geometry.h"
#include "utility.h"

using namespace std;

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

bool point_in_rect(const Point& p, const Point& r1, const Point& r2)
{
	return r1.x <= p.x && p.x <= r2.x && r1.y <= p.y && p.y <= r2.y;
}
