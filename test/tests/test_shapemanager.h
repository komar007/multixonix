#include "../testsuite.h"

#include "shapemanager.h"
#include "debug.h"
#include <unordered_set>

using namespace std;

class Obs : public Observer<ShapeMessage> {
public:
	ShapeMessage last_m;
	const ShapeManager *last_manager;

	Obs()
		: last_m(ShapeMessage::DESTROYED, -1)
	{
	}

	virtual void update(Observable<ShapeMessage>& obj, const ShapeMessage& msg)
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
		add_test("traces",
				(test_fun)&ShapeManagerTestSuite::traces);
		add_test("synchronization",
				(test_fun)&ShapeManagerTestSuite::synchronization);
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
			pair<int, int> ids = m.cut_shape(tid, id, 0, 2);
			const Path& one = m.get_shape_const_ref(ids.first).get_path();
			const Path& two = m.get_shape_const_ref(ids.second).get_path();
			assert_eq(one, fixture1);
			assert_eq(two, fixture2);
		}
		{
			Path fixture1(true, vector<Point>{{5,0}, {5,5}, {0,5}, {0,0}});
			Path fixture2(true, vector<Point>{{5,0}, {5,5}, {0,5}, {0,10}, {10,10}, {10,0}});
			int id = m.add_shape(p2);
			int tid = m.start_trace(Point(5, 0));
			m.extend_trace(tid, Point(5, 5));
			m.extend_trace(tid, Point(0, 5));
			pair<int, int> ids = m.cut_shape(tid, id, 0, 3);
			const Path& one = m.get_shape_const_ref(ids.first).get_path();
			const Path& two = m.get_shape_const_ref(ids.second).get_path();
			assert_eq(one, fixture1);
			assert_eq(two, fixture2);
		}
		{
			Path fixture1(true, vector<Point>{{8,0}, {5,5}, {3,0}});
			Path fixture2(true, vector<Point>{{8,0}, {5,5}, {3,0}, {0,0}, {0,10}, {10,10}, {10,0}});
			int id = m.add_shape(p2);
			int tid = m.start_trace(Point(8, 0));
			m.extend_trace(tid, Point(5, 5));
			m.extend_trace(tid, Point(3, 0));
			pair<int, int> ids = m.cut_shape(tid, id, 0, 0);
			const Path& one = m.get_shape_const_ref(ids.first).get_path();
			const Path& two = m.get_shape_const_ref(ids.second).get_path();
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
			Obs w;
			m.attach(w);
			pair<int, int> ids = m.cut_shape(tid, id, 0, 0);
			const Path& one = m.get_shape_const_ref(ids.first).get_path();
			const Path& two = m.get_shape_const_ref(ids.second).get_path();
			assert_eq(one, fixture1);
			assert_eq(two, fixture2);
			assert_eq(w.last_m.id, ids.second);
			assert_eq(w.last_m.type, ShapeMessage::CREATED);
			assert_eq(w.last_m.info->trace_id, tid);
			assert_eq(w.last_m.info->shape_id, id);
			assert_eq(w.last_m.info->shape_start, 0);
			assert_eq(w.last_m.info->shape_end, 0);
			assert_eq(w.last_m.info->shape_dir, ShapeCreationInfo::ShapeDirection::REVERSE);
		}
	}

	void traces()
	{
		ShapeManager m(false);
		int id = m.start_trace(Point(1, 1));
		Obs w;
		m.attach(w);
		m.extend_trace(id, Point(2, 2));
		assert_eq(w.last_m.id, id);
		assert_eq(w.last_m.type, ShapeMessage::EXTENDED);
		assert_eq(*w.last_m.extension_point, Point(2, 2));
		m.extend_trace(id, Point(3, 3));
		assert_eq(*w.last_m.extension_point, Point(3, 3));
	}

	void notifications_create_new()
	{
		Obs w;
		ShapeManager m(false);
		m.attach(w);
		m.add_shape(p1);
		assert_eq(w.last_manager, &m);
		assert_eq(w.last_m.type, ShapeMessage::CREATED);
		assert_eq(w.last_m.id, 1);
		m.add_shape(p1);
		assert_eq(w.last_m.id, 2);
		assert_eq(w.last_m.info->trace_id, -1);
		assert_eq(w.last_m.info->shape_id, -1);
		assert_eq(w.last_m.info->path->size(), 3u);
	}

	void synchronization()
	{
		ShapeManager master_m(false);
		ShapeManager replica_m(false);
		master_m.attach(replica_m);
		int id = master_m.add_shape(p2);
		int pid = id;
		assert_eq(
			replica_m.get_shape_const_ref(id).get_path(),
			master_m.get_shape_const_ref(id).get_path()
		);
		id = master_m.start_trace(Point(5,0));
		assert_eq(
			replica_m.get_shape_const_ref(id).get_path(),
			master_m.get_shape_const_ref(id).get_path()
		);
		master_m.extend_trace(id, Point(5, 5));
		assert_eq(master_m.num_shapes(), replica_m.num_shapes());
		master_m.extend_trace(id, Point(5, 10));
		assert_eq(
			replica_m.get_shape_const_ref(id).get_path(),
			master_m.get_shape_const_ref(id).get_path()
		);
		pair<int, int> ids = master_m.cut_shape(id, pid, 0, 2);
		assert_eq(
			replica_m.get_shape_const_ref(ids.first).get_path(),
			master_m.get_shape_const_ref(ids.first).get_path()
		);
		assert_eq(
			replica_m.get_shape_const_ref(ids.second).get_path(),
			master_m.get_shape_const_ref(ids.second).get_path()
		);
		master_m.destroy_shape(pid);
		bool found_master = false;
		try { auto t = master_m.get_shape_const_ref(pid); found_master = true; } catch (...) {}
		assert_eq(found_master, false);
		bool found_replica = false;
		try {auto t = replica_m.get_shape_const_ref(pid); found_replica = true; } catch (...) {}
		assert_eq(found_replica, false);
		assert_eq(master_m.num_shapes(), replica_m.num_shapes());
	}
};
