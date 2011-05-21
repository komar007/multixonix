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

double path_area(const Path& path)
{
	if (!path.closed)
		throw domain_error("open path in path_area");
	double area = .0;
	for (auto i = path.begin(), j = i+1; i != path.end(); ++i, ++j)
		area += (i->y + j->y)/2 * (j->x - i->x);
	return abs(area);
}

double normalized_angle(double angle)
{
	if (angle < .0)
		return angle + 2*M_PI;
	else if (angle >= 2*M_PI)
		return angle - 2*M_PI;
	else
		return angle;
}
