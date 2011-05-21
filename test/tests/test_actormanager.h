#include "../testsuite.h"

#include "actormanager.h"
#include "debug.h"
#include <unordered_set>

using namespace std;

class ActObs : public Observer<ActorMessage> {
public:
	ActorMessage last_m;
	const ActorManager *last_manager;

	ActObs()
		: last_m(ActorMessage::DESTROYED, -1)
	{
	}

	virtual void update(Observable<ActorMessage>& obj, const ActorMessage& msg)
	{
		last_manager = dynamic_cast<const ActorManager*>(&obj);
		last_m = msg;
	}
};

class ActorManagerTestSuite : public TestSuite {
public:
	ActorManagerTestSuite()
		: TestSuite("ActorManager")
	{
		add_test("test_notifications",
				(test_fun)&ActorManagerTestSuite::test_notifications);
	}

	void test_notifications()
	{
		ActorManager m;
		ActObs obs;
		m.attach(obs);
		Ball b;
		m.add_actor(b);
		assert_eq(obs.last_m.type, ActorMessage::CREATED);
		assert_eq(obs.last_m.id, 1);
		Player p;
		m.add_actor(p);
		assert_eq(obs.last_m.type, ActorMessage::CREATED);
		assert_eq(obs.last_m.id, 2);
		Actor& m_b = m.get_actor_ref(2);
		// should not move, no speed
		m_b.step();
		assert_eq(obs.last_m.type, ActorMessage::CREATED);
		m_b.speed = 1;
		m_b.begin();
		m_b.step();
		m_b.commit();
		assert_eq(obs.last_m.type, ActorMessage::MOVED);
		assert_eq(obs.last_m.id, 2);
		assert_eq(*obs.last_m.pos, Point(1,0));
		m.destroy_actor(2);
		assert_eq(obs.last_m.type, ActorMessage::DESTROYED);
		assert_eq(obs.last_m.id, 2);
	}
};
