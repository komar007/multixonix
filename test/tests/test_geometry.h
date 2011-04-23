#include "../testsuite.h"

#include "geometry.h"

class GeometryTestSuite : public TestSuite {
public:
	GeometryTestSuite()
		: TestSuite("Geometry")
	{
		add_test("turning_determinant", (test_fun)&GeometryTestSuite::test_turning_determinant);
		add_test("turning_determinant degenerated", (test_fun)&GeometryTestSuite::test_turning_determinant_degen);
	}

	void test_turning_determinant()
	{
		assert(turning_determinant(Point(3, 2), Point(6, 5), Point(8, 8)) > 0, "wrong result");
		assert(turning_determinant(Point(3, 2), Point(6, 5), Point(9, 7)) < 0, "wrong result");
		assert(turning_determinant(Point(3, 2), Point(6, 5), Point(7, 6)) == 0, "wrong result");
		assert(turning_determinant(Point(8, 8), Point(6, 5), Point(3, 2)) < 0, "wrong result");
		assert(turning_determinant(Point(6, 5), Point(8, 8), Point(3, 2)) > 0, "wrong result");
	}
	void test_turning_determinant_degen()
	{
		assert(turning_determinant(Point(8, 8), Point(6, 5), Point(6, 5)) == 0, "wrong result");
		assert(turning_determinant(Point(8, 8), Point(8, 8), Point(6, 5)) == 0, "wrong result");
		assert(turning_determinant(Point(8, 8), Point(6, 5), Point(8, 8)) == 0, "wrong result");
		assert(turning_determinant(Point(0, 0), Point(0, 0), Point(0, 0)) == 0, "wrong result");
	}
};
