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
public:
	ShapeManagerTestSuite()
		: TestSuite("ShapeManager")
	{
		add_test("notifications",
				(test_fun)&ShapeManagerTestSuite::notifications);
	}

	void notifications()
	{
		Obs w;
		ShapeManager m(false);
		m.attach(w);
		Path p(true);
		p.push_back(Point(1, 1));
		p.push_back(Point(2, 2));
		p.push_back(Point(3, 0));
		m.add_shape(p);
		assert_eq(w.last_manager, &m);
		assert_eq(w.last_m.type, CREATED);
		assert_eq(w.last_m.id, 1);
		m.add_shape(p);
		assert_eq(w.last_m.id, 2);
		assert_eq(w.last_m.info->path->size(), 3u);
	}
};
