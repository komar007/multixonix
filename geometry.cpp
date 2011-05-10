#include "geometry.h"
#include "utility.h"

using namespace std;

double turning_determinant(const Point &p1, const Point &p2, const Point &p3)
{
	return p1.x*p2.y - p2.x*p1.y +
	       p3.x*p1.y - p1.x*p3.y +
	       p2.x*p3.y - p3.x*p2.y;
}

double line_point_distance(const Point& l1, const Point& l2, const Point& p)
{
	return abs(turning_determinant(l1, l2, p)) / (l2 - l1).length();
}
