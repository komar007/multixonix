#include "../testsuite.h"

#include "collision.h"

class CollisionTestSuite : public TestSuite {
private:
	const Point la, lb;
	const Point a, b, c, d, e, f, g;
public:
	CollisionTestSuite()
		: TestSuite("Collision")
		, la(3, 2)
		, lb(6, 5)
		, a(1, 1)
		, b(2, 2)
		, c(4, 1)
		, d(4, 2)
		, e(4, 3)
		, f(5, 3)
		, g(4, 4)
	{
		add_test("line_segment_collision",
				(test_fun)&CollisionTestSuite::test_line_segment_collision);
		add_test("line_segment_collision_degenerated",
				(test_fun)&CollisionTestSuite::test_line_segment_collision_degen);
	}

	void test_line_segment_collision()
	{
		assert_eq(line_segment_collision(la, lb, Point(8, 4), Point(8, 6)), false);
		assert_eq(line_segment_collision(la, lb, Point(8, 4), Point(8, 7)), false);
		assert_eq(line_segment_collision(la, lb, Point(8, 4), Point(8, 8)), true);
		assert_eq(line_segment_collision(la, lb, Point(8, 4), Point(4, 6)), true);
		assert_eq(line_segment_collision(la, lb, Point(2, 3), Point(8, 4)), true);

	}
	void test_line_segment_collision_degen()
	{
		assert_eq(line_segment_collision(la, lb, la, la), true);
		assert_eq(line_segment_collision(la, lb, la, lb), true);
		assert_eq(line_segment_collision(la, lb, la, Point(2, 4)), true);
		assert_eq(line_segment_collision(la, lb, Point(2, 4), la), false);
	}
};
