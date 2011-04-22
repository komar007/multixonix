#pragma once

#include <string>
#include <vector>

#include <iostream>
#include <sstream>

#define assert(c, s) assert_impl(c, s, __LINE__, #c)
#define assert_eq(a, b) assert_eq_impl(a, b, __LINE__, #a " == " #b)

struct Error {
	std::string testname;
	std::string msg;
	int lineno;
	std::string cond_str;
	Error(const std::string& _msg, int _lineno, const std::string& _cond_str)
		: msg(_msg)
		, lineno(_lineno)
		, cond_str(_cond_str)
	{
	}
	friend std::ostream& operator<<(std::ostream &os, const Error &e);
};

std::ostream& operator<<(std::ostream &os, const Error &e)
{
	os << "test \"" << e.testname << "\" failed: [" << e.lineno << "] assertion " << e.cond_str << " -> " << e.msg;
	return os;
}

class TestSuite;
typedef void (TestSuite::*test_fun)();

class Test {
private:
	std::string name;
	test_fun method;
	int npassed;
public:
	Test(const std::string &_name, test_fun _method)
		: name(_name)
		, method(_method)
		, npassed(0)
	{
	}
	friend class TestSuite;
};

class Tester;

class TestSuite {
private:
	std::string name;
	std::vector<Error> errors;
	std::vector<Test> tests;
	std::vector<Test>::iterator cur_test;
	void run_tests(std::ostream& logger)
	{
		for (cur_test = tests.begin(); cur_test != tests.end(); ++cur_test) {
			bool was_error = false;
			try {
				(this->*(cur_test->method))();
			} catch (Error e) {
				e.testname = cur_test->name;
				errors.push_back(e);
				was_error = true;
			}
			logger << std::string(" ") + std::string(cur_test->npassed, '.');
			if (was_error)
				logger << "F";
		}
	}
protected:
	void add_test(const std::string &name, test_fun method)
	{
		tests.push_back(Test(name, method));
	}
	void assert_impl(bool condition, const std::string &fail_message, int lineno,
			const std::string &cond_str)
	{
		if (!condition)
			throw Error(fail_message, lineno, cond_str);
		++cur_test->npassed;
	}
	template <typename T, typename U>
	void assert_eq_impl(T a, U b, int lineno, const std::string &cond_str)
	{
		if (!(a == b)) {
			std::stringstream ss;
			ss << a;
			throw Error(std::string("wrong result, got: ") + ss.str(), lineno, cond_str);
		}
		++cur_test->npassed;
	}
	void assert_eq_impl(bool a, bool b, int lineno, const std::string &cond_str)
	{
		if (!(a == b)) {
			std::string res = a ? "true" : "false";
			throw Error(std::string("wrong result, got: ") + res, lineno, cond_str);
		}
		++cur_test->npassed;
	}
public:
	TestSuite(const std::string &_name)
		: name(_name)
		, cur_test(NULL)
	{
	}
	virtual void prepare()
	{
	}
	virtual ~TestSuite()
	{
	}
	friend class Tester;
};


