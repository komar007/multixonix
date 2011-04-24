#include "../testsuite.h"

#include "detector.h"
#include "debug.h"
#include <unordered_set>

class DetectorTestSuite : public TestSuite {
public:
	void test(const Detector& d, const Point& a, const Point& b, const std::unordered_set<Location>& locs_ref)
	{
		BfsPainter dp(a, b, d);
		std::unordered_set<Location> locs(dp.begin(), dp.end());
		assert_eq(locs, locs_ref);
	}

	DetectorTestSuite()
		: TestSuite("Detector")
	{
		add_test("BfsPainter",
				(test_fun)&DetectorTestSuite::bfs_painter);
	}

	void bfs_painter()
	{
		Path p;
		Detector d(p, Vector(.0, .0), 1.0);
		test(d, {4, 3}, {8, 9.5}, std::unordered_set<Location>({{4,3}, {4,4}, {5, 4}, {5, 5}, {5, 6}, {6, 6}, {6, 7}, {7, 7}, {7, 8}, {7, 9}, {8, 9}}));
		test(d, {8, 9.5}, {4, 3}, std::unordered_set<Location>({{4,3}, {4,4}, {5, 4}, {5, 5}, {5, 6}, {6, 6}, {6, 7}, {7, 7}, {7, 8}, {7, 9}, {8, 9}}));
		test(d, {1.5, 1.5}, {1.6, 1.6}, std::unordered_set<Location>({{1,1}}));
		test(d, {0, 0}, {1, 1}, std::unordered_set<Location>({{0, 0}, {0, 1}, {1, 0}, {1,1}}));
	}
};
