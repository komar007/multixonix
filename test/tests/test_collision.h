#include "../testsuite.h"

#include "collision.h"

class CollisionTestSuite : public TestSuite {
public:
	CollisionTestSuite()
		: TestSuite("Collision")
	{
		add_test("line_segment_collision",
				(test_fun)&CollisionTestSuite::test_line_segment_collision);
		add_test("line_segment_collision_degenerated",
				(test_fun)&CollisionTestSuite::test_line_segment_collision_degen);
		add_test("segment_segment_collision",
				(test_fun)&CollisionTestSuite::test_segment_segment_collision);
	}

	void test_line_segment_collision()
	{
		Point la(3, 2);
		Point lb(6, 5);
		assert_eq(line_segment_collision(la, lb, Point(8, 4), Point(8, 6)), false);
		assert_eq(line_segment_collision(la, lb, Point(8, 4), Point(8, 7)), false);
		assert_eq(line_segment_collision(la, lb, Point(8, 4), Point(8, 8)), true);
		assert_eq(line_segment_collision(la, lb, Point(8, 4), Point(4, 6)), true);
		assert_eq(line_segment_collision(la, lb, Point(2, 3), Point(8, 4)), true);

	}
	void test_line_segment_collision_degen()
	{
		Point la(3, 2);
		Point lb(6, 5);
		assert_eq(line_segment_collision(la, lb, la, la), true);
		assert_eq(line_segment_collision(la, lb, la, lb), true);
		assert_eq(line_segment_collision(la, lb, la, Point(2, 4)), true);
		assert_eq(line_segment_collision(la, lb, Point(2, 4), la), false);
	}
	void test_segment_segment_collision()
	{
		Point a(1, 1), b(2, 2);
		assert_eq(segment_segment_collision(a, b, Point(2, 1), Point(1, 2)), true);
		assert_eq(segment_segment_collision(a, b, Point(2, 1), Point(1.5, 1.5)), false);
	}
};
