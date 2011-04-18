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
	}

	void test_line_segment_collision()
	{
		Point la(3, 2);
		Point lb(6, 5);
		assert(line_segment_collision(la, lb, Point(8, 4), Point(8, 6)) == false, "false positive");
		assert(line_segment_collision(la, lb, Point(8, 4), Point(8, 7)) == false, "false positive");
		assert(line_segment_collision(la, lb, Point(8, 4), Point(8, 8)) == true, "collision not detected");
		assert(line_segment_collision(la, lb, Point(8, 4), Point(4, 6)) == true, "collision not detected");
		assert(line_segment_collision(la, lb, Point(2, 3), Point(8, 4)) == true, "collision not detected");

	}
	void test_line_segment_collision_degen()
	{
		Point la(3, 2);
		Point lb(6, 5);
		assert(line_segment_collision(la, lb, la, la) == true, "collision not detected");
		assert(line_segment_collision(la, lb, la, lb) == true, "collision not detected");
		assert(line_segment_collision(la, lb, la, Point(2, 4)) == true, "collision not detected");
		assert(line_segment_collision(la, lb, Point(2, 4), la) == false, "false positive");
	}
};
