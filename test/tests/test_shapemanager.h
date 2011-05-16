#include "../testsuite.h"

#include "shapemanager.h"
#include "debug.h"
#include <unordered_set>

using namespace std;

class Obs : public Observer<ShapeMessage> {
public:
	ShapeMessage last_m;
	const ShapeManager *last_manager;

	virtual void update(const Observable<ShapeMessage>& obj, const ShapeMessage& msg)
	{
		last_manager = dynamic_cast<const ShapeManager*>(&obj);
		last_m = msg;
	}
};

class ShapeManagerTestSuite : public TestSuite {
private:
	Path p1;
	Path p2;
public:
	ShapeManagerTestSuite()
		: TestSuite("ShapeManager")
		, p1(true)
		, p2(true)
	{
		p1.push_back(Point(1, 1));
		p1.push_back(Point(2, 2));
		p1.push_back(Point(3, 0));
		p2.push_back(Point(0, 0));
		p2.push_back(Point(10, 0));
		p2.push_back(Point(10, 10));
		p2.push_back(Point(0, 10));
		add_test("notifications_create_new",
				(test_fun)&ShapeManagerTestSuite::notifications_create_new);
		add_test("cut_path",
				(test_fun)&ShapeManagerTestSuite::cut_path);
	}

	void cut_path()
	{
		ShapeManager m(false);
		{
			Path fixture1(true, vector<Point>{{5,0}, {5,5}, {5,10}, {0,10}, {0,0}});
			Path fixture2(true, vector<Point>{{5,0}, {5,5}, {5,10}, {10,10}, {10,0}});
			int id = m.add_shape(p2);
			int tid = m.start_trace(Point(5, 0));
			m.extend_trace(tid, Point(5, 5));
			m.extend_trace(tid, Point(5, 10));
			pair<int, int> ids = m.cut_shape(id, 0, 2, tid);
			const Path& one = m.get_shape(ids.first).get_path();
			const Path& two = m.get_shape(ids.second).get_path();
			assert_eq(one, fixture1);
			assert_eq(two, fixture2);
		}
		{
			Path fixture1(true, vector<Point>{{3,0}, {5,5}, {8,0}, {10,0}, {10,10}, {0,10}, {0,0}});
			Path fixture2(true, vector<Point>{{3,0}, {5,5}, {8,0}});
			int id = m.add_shape(p2);
			int tid = m.start_trace(Point(3, 0));
			m.extend_trace(tid, Point(5, 5));
			m.extend_trace(tid, Point(8, 0));
			pair<int, int> ids = m.cut_shape(id, 0, 0, tid);
			const Path& one = m.get_shape(ids.first).get_path();
			const Path& two = m.get_shape(ids.second).get_path();
			assert_eq(one, fixture1);
			assert_eq(two, fixture2);
		}
	}

	void notifications_create_new()
	{
		Obs w;
		ShapeManager m(false);
		m.attach(w);
		m.add_shape(p1);
		assert_eq(w.last_manager, &m);
		assert_eq(w.last_m.type, CREATED);
		assert_eq(w.last_m.id, 1);
		m.add_shape(p1);
		assert_eq(w.last_m.id, 2);
		assert_eq(w.last_m.info->trace_id, -1);
		assert_eq(w.last_m.info->shape_id, -1);
		assert_eq(w.last_m.info->path->size(), 3u);
	}
};
