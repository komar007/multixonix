#pragma once

#include "geometry.h"

bool line_segment_collision(const Point &s1, const Point &s2, const Point &l1, const Point &l2);
bool segment_path_segment_collision(const Point& s1, const Point& s2, const Point& p1, const Point& p2);
