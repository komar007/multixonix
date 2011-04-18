#include "../testsuite.h"

#include "geometry.h"

class GeometryTestSuite : public TestSuite {
public:
	GeometryTestSuite()
		: TestSuite("Geometry")
	{
		add_test("helicity", (test_fun)&GeometryTestSuite::test_helicity);
		add_test("helicity degenerated", (test_fun)&GeometryTestSuite::test_helicity_degen);
	}

	void test_helicity()
	{
		assert(helicity(Point(3, 2), Point(6, 5), Point(8, 8)) > 0, "wrong result");
		assert(helicity(Point(3, 2), Point(6, 5), Point(9, 7)) < 0, "wrong result");
		assert(helicity(Point(3, 2), Point(6, 5), Point(7, 6)) == 0, "wrong result");
		assert(helicity(Point(8, 8), Point(6, 5), Point(3, 2)) < 0, "wrong result");
		assert(helicity(Point(6, 5), Point(8, 8), Point(3, 2)) > 0, "wrong result");
	}
	void test_helicity_degen()
	{
		assert(helicity(Point(8, 8), Point(6, 5), Point(6, 5)) == 0, "wrong result");
		assert(helicity(Point(8, 8), Point(8, 8), Point(6, 5)) == 0, "wrong result");
		assert(helicity(Point(8, 8), Point(6, 5), Point(8, 8)) == 0, "wrong result");
		assert(helicity(Point(0, 0), Point(0, 0), Point(0, 0)) == 0, "wrong result");
	}
};
