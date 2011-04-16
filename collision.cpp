#include "collision.h"

bool line_segment_collision(const Point &l1, const Point &l2, const Point &s1, const Point &s2)
{
	int h1 = helicity(l1, l2, s1);
	return h1 == 0 || h1 * helicity(l1, l2, s2) < 0;
}
