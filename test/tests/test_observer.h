#include "../testsuite.h"

#include "observer.h"

class State : public Observable<bool> {
public:
	int i;
	void change(int x)
	{
		i = x;
		notify(i != 0);
	}
};

class Watcher : public Observer<bool> {
public:
	const State *last_state;
private:
	std::ostream& ostr;
	virtual void update(Observable<bool>& obj, const bool& msg)
	{
		const State& s = dynamic_cast<const State&>(obj);
		last_state = &s;
		ostr << msg << ", o.i = " << s.i << " ";
	}
public:
	Watcher(std::ostream& _ostr)
		: ostr(_ostr)
	{
	}
};

class ObserverTestSuite : public TestSuite {
public:
	ObserverTestSuite()
		: TestSuite("Observer")
	{
		add_test("observer_observable_test",
				(test_fun)&ObserverTestSuite::observer_observable_test);
	}

	void observer_observable_test()
	{
		State s;
		s.change(3);
		std::stringstream str;
		Watcher w(str);
		s.attach(w);
		s.change(7);
		s.change(0);
		assert_eq(w.last_state, &s);
		assert_eq(str.str(), "1, o.i = 7 0, o.i = 0 ");
	}
};
