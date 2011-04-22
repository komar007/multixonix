#pragma once

#include "testsuite.h"

class Tester {
private:
	std::vector<TestSuite*> testsuites;
public:
	Tester()
	{
	}
	void add_suite(TestSuite &ts)
	{
		testsuites.push_back(&ts);
	}
	void run()
	{
		int passed = 0;
		for (auto i = testsuites.begin(), num = 1; i != testsuites.end(); ++i, ++num) {
			TestSuite &ts = **i;
			std::cout << "[" << num << "/" << testsuites.size() << "] " << ts.name << ":";
			ts.prepare();
			ts.run_tests(std::cout);
			if (ts.errors.size() == 0) {
				std::cout << " passed";
				++passed;
			} else {
				std::cout << " " << ts.errors.size() << "/" << ts.tests.size() << " tests failed";
			}
			std::cout << std::endl;
			for (auto e = ts.errors.begin(); e != ts.errors.end(); ++e)
				std::cout << "\t" << *e << std::endl;
		}
		std::cout << "Test suites passed: " << passed << "/" << testsuites.size() << "." << std::endl;
	}
	~Tester()
	{
	}
};
