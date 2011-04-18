#include "../testsuite.h"

#include "utility.h"
#include <sstream>
#include <algorithm>

class UtilityTestSuite : public TestSuite {
public:
	UtilityTestSuite()
		: TestSuite("Utility")
	{
		add_test("cyclic_iterator",
				(test_fun)&UtilityTestSuite::test_cyclic_iterator);
	}

	void test_cyclic_iterator()
	{
		int x[] = {1,2,3,4,5,6};
		cyclic_iterator<int*> it1(x, x+6, x+4);
		cyclic_iterator<int*> it2(x, x+6, x+1);
		std::fill(it1, it2, 10);
		cyclic_iterator<int*> it(x, x+6, x);
		std::stringstream ss;
		for (int i = 0; i < 6; ++i) {
			ss << *it << " ";
			++it;
		}
		assert_eq(ss.str(), "10 2 3 4 10 10 ");
	}
};
