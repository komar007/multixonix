#pragma once

//! @file
//! \brief Basic collision detection suite

#include "geometry.h"

//! Checks if point p is inside a rectangle, whose upper-left corner is r1 and lower-right corner is \param r2
bool point_in_rect(const Point& p, const Point& r1, const Point& r2);
//! Checks if point p is inside rectangle \param r
bool point_in_rect(const Point& p, const Rect& r);

//! \brief Tests intersection between line and segment
//!
//! @return true if there is exactly one point of intersection, false otherwise
bool line_segment_collision(const Point &s1, const Point &s2, const Point &l1, const Point &l2);

//! \brief Tests intersection between a free segment and a segment belonging
//! to a path
//!
//! This function tests for intersection between segment s1s2 belonging to a
//! path and segment p1p2 rotated around p1 by infinitely small angle. This
//! allows this function to be used to test intersection of a path and segment
//! by counting the number of intersections with edges without complicated
//! special cases.
//! @param s1 non-path segment's start point
//! @param s2 non-path segment's end point
//! @param p1 path segment's start point
//! @param p2 path segment's end point
//! @return true if there is exactly one point of intersection, false otherwise
bool segment_path_segment_collision(const Point& s1, const Point& s2, const Point& p1, const Point& p2);

//! \brief Finds intersection point between two non-parallel segments a1a2 and b1b2
//!
//! @param a1 first segment's start point
//! @param a2 first segment's end point
//! @param b1 second segment's start point
//! @param b2 second segment's end point
//! @return Point of intersection
Point segment_segment_intersection(const Point& a1, const Point& a2, const Point& b1, const Point& b2);
