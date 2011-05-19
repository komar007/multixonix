#include "../testsuite.h"

#include "detector.h"
#include "debug.h"
#include <unordered_set>

using namespace std;

class DetectorTestSuite : public TestSuite {
public:
	void bfstest(const Detector& d, const Point& a, const Point& b, const std::unordered_set<Location>& locs_ref)
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
		add_test("detector",
				(test_fun)&DetectorTestSuite::detector);
	}

	void bfs_painter()
	{
		Path p;
		Detector d(p, Vector(.0, .0), 1.0);
		bfstest(d, {4, 3}, {8, 9.5}, std::unordered_set<Location>({{3,2}, {3,3}, {4,2}, {4,3}, {4,4}, {5, 4}, {5, 5}, {5, 6}, {6, 6}, {6, 7}, {7, 7}, {7, 8}, {7, 9}, {8, 9}}));
		bfstest(d, {8, 9.5}, {4, 3}, std::unordered_set<Location>({{3,2}, {3,3}, {4,2}, {4,3}, {4,4}, {5, 4}, {5, 5}, {5, 6}, {6, 6}, {6, 7}, {7, 7}, {7, 8}, {7, 9}, {8, 9}}));
		bfstest(d, {1.5, 1.5}, {1.6, 1.6}, std::unordered_set<Location>({{1,1}}));
		bfstest(d, {0, 0}, {1, 1}, std::unordered_set<Location>({{0, -1}, {-1, 0}, {-1, -1}, {0, 0}, {0, 1}, {1, 0}, {1,1}}));
	}

	void detector()
	{
		Path p(true);
		Point p1(2, 2);
		Point p2(2, 6);
		Point p3(4, 3);
		Point p4(3, 8);
		Point p5(5, 6);
		Point p6(5, 11);
		Point p7(3, 9);
		Point p8(5, 12);
		Point p9(7, 10);
		Point p10(8, 11);
		Point p11(9, 9);
		Point p12(7, 8);
		Point p13(8, 9);
		Point p14(6, 10);
		Point p15(6, 7);
		Point p16(8, 7);
		Point p17(6, 5);
		Point p18(8, 3);
		Point p19(8, 5);
		Point p20(9, 5);
		Point p21(9, 2);
		Point p22(6, 4);
		Point p23(5, 5);
		Point p24(6, 2);
		Point p25(5, 3);
		Point p26(4, 1);
 		p.push_back(p1); p.push_back(p2); p.push_back(p3); p.push_back(p4); p.push_back(p5); p.push_back(p6); p.push_back(p7); p.push_back(p8); p.push_back(p9); p.push_back(p10); p.push_back(p11); p.push_back(p12); p.push_back(p13); p.push_back(p14); p.push_back(p15); p.push_back(p16); p.push_back(p17); p.push_back(p18); p.push_back(p19); p.push_back(p20); p.push_back(p21); p.push_back(p22); p.push_back(p23); p.push_back(p24); p.push_back(p25); p.push_back(p26);
		vector<Detector::Intersection> x;
		Detector d(p, Vector(.0, .0), 1.0);
		assert_eq(d.segment_intersections(Point(6, 1), Point(1, 3), x), 2);
		assert_eq(d.segment_intersections(Point(6, 1), Point(4, 2), x), 1);
		assert_eq(d.segment_intersections(Point(6, 1), Point(2, 3), x), 1);

		assert_eq(d.segment_intersections(Point(3, 11), Point(9, 1), x), 6);

		assert_eq(d.segment_intersections(Point(4, 0), Point(4, 2), x), 1);
		assert_eq(d.segment_intersections(Point(5, 1), Point(7, 3), x), 2);
		assert_eq(d.segment_intersections(Point(7, 3), Point(5, 1), x), 0);
		assert_eq(d.segment_intersections(Point(8, 1), Point(8, 6), x), 2);
		assert_eq(d.segment_intersections(Point(8, 6), Point(8, 1), x), 2);
		assert_eq(d.segment_intersections(Point(7, 5), Point(5.5, 5), x), 1);

		Path o(false);
		o.push_back(p26);
		o.push_back(p1);
		o.push_back(p2);
		o.push_back(p3);
		Path c(true);
		c.push_back(p26);
		c.push_back(p1);
		c.push_back(p2);
		c.push_back(p3);
		Detector od(o, Vector(.0, .0), 1.0);
		Detector cd(c, Vector(.0, .0), 1.0);
		assert_eq(cd.segment_intersections(Point(6, 1), Point(0, 4), x), 2);
		assert_eq(od.segment_intersections(Point(6, 1), Point(0, 4), x), 1);
	}
};
