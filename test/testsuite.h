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
	os << e.testname << " failed: [" << e.lineno << "] " << e.cond_str << " -> " << e.msg;
	return os;
}

class TestSuite;
typedef void (TestSuite::*test_fun)();

class Test {
private:
	std::string name;
	test_fun method;
public:
	Test(const std::string &_name, test_fun _method)
		: name(_name)
		, method(_method)
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
	void run_tests(std::ostream& logger)
	{
		for (auto i = tests.begin(); i != tests.end(); ++i) {
			bool was_error = false;
			try {
				(this->*(i->method))();
			} catch (Error e) {
				e.testname = i->name;
				errors.push_back(e);
				was_error = true;
			}
			if (was_error)
				logger << "F";
			else
				logger << ".";
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
	}
	template <typename T, typename U>
	void assert_eq_impl(T a, U b, int lineno, const std::string &cond_str)
	{
		if (!(a == b)) {
			std::stringstream ss;
			ss << a;
			throw Error(std::string("wrong result, got: ") + ss.str(), lineno, cond_str);
		}
	}
public:
	TestSuite(const std::string &_name)
		: name(_name)
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


