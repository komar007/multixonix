#include "collision.h"

bool line_segment_collision(const Point &l1, const Point &l2, const Point &s1, const Point &s2)
{
	int h1 = turning_determinant(l1, l2, s1);
	return h1 == 0 || h1 * turning_determinant(l1, l2, s2) < 0;
}

bool segment_segment_collision(const Point &a1, const Point &a2, const Point &b1, const Point &b2)
{
	return line_segment_collision(a1, a2, b1, b2) &&
		line_segment_collision(b1, b2, a1, a2);
}
