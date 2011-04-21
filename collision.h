#pragma once

#include "geometry.h"

bool line_segment_collision(const Point &s1, const Point &s2, const Point &l1, const Point &l2);
bool segment_segment_collision(const Point &a1, const Point &a2, const Point &b1, const Point &b2);
