#pragma once

#include <type_traits>
#include <limits>
#include <cmath>
#include <iostream>
#include <cstring>
#include <vector>
#include <functional>

namespace test {

template<typename... T> constexpr void unused(T&&...) {}
template<typename... T> using void_t = void;

auto width = 50u;
auto addWidth = 16u;
const char* currentTestName = "<no testing function>";

int failed; // number of tests failed
std::vector<std::pair<std::function<void()>, const char*>> tests;

int add(std::function<void()> f, const char* name)
{
	tests.push_back({std::move(f), name});
	return 0;
}

#define CAT_IMPL(A, B) A ## B
#define CAT(A, B) CAT_IMPL(A, B)

#define TEST_METHOD_IMPL(name, testname) \
	static void testname(); \
	namespace { const auto CAT(testname, reg) = ::test::add(testname, name); } \
	static void testname()

#define TEST_METHOD(name) TEST_METHOD_IMPL(name, CAT(test, __LINE__))

#define EXPECT(expr, expect) { \
		auto&& test_ce_a = expr; \
		auto&& test_ce_b = expect; \
		if(test_ce_a != test_ce_b) \
			test::checkExpectFailed(::test::currentTestName, __FILE__, __LINE__,  \
				#expr, #expect, test_ce_a, test_ce_b); \
	}

#define EXPECT_ERROR(expr, error) { \
		bool test_thrown {}; \
		bool test_sthelse {}; \
		try{ expr; } \
		catch(const error&) { test_thrown = true; } \
		catch(...) { test_sthelse = true; } \
		if(!test_thrown) \
			test::checkErrorFailed(::test::currentTestName, __FILE__, __LINE__, #expr, \
				#error, test_sthelse); \
	}

template<typename T, typename = void>
struct Printable {
	static const char* call(const T&)
	{
		static auto txt = std::string("<not printable : ") + typeid(T).name() + std::string(">");
		return txt.c_str();
	}
};

template<typename T>
struct Printable<T, void_t<decltype(std::declval<std::ostream&>() << std::declval<T>())>> {
	static const auto& call(const T& obj) { return obj; }
};

template<typename T>
decltype(auto) print(const T& obj) { return Printable<T>::call(obj); }

template<typename A, typename B>
void checkExpectFailed(const char* test, const char* file, int line, const char* expression,
	const char* expect, const A& a, const B& b)
{
	unused(expression, expect);

	std::cout << "\t";
	for(auto i = 0u; i < width; ++i) std::cout << "<";
	std::cout << "\n";

	std::cout << "\tCheck Expect failed in test " << test << "\n\t";
	std::cout << file << ":" << line << "\n";
	std::cout << "\tExpected " << print(b) << ", got " << print(a) << "\n";

	std::cout << "\t";
	for(auto i = 0u; i < width; ++i) std::cout << ">";
	std::cout << "\n\n";

	++failed;
}

void checkErrorFailed(const char* test, const char* file, int line, const char* expression,
	const char* error, bool otherError)
{
	unused(expression);

	std::cout << "\t";
	for(auto i = 0u; i < width; ++i) std::cout << "<";
	std::cout << "\n";

	std::cout << "\tCheck Error failed in test " << test << "\n\t";
	std::cout << file << ":" << line << "\n";
	std::cout << "\tExpected Error " << error;
	if(otherError) std::cout << ", other error was thrown instead!\n";
 	else std::cout << ", no error was thrown\n";

	std::cout << "\t";
	for(auto i = 0u; i < width; ++i) std::cout << ">";
	std::cout << "\n\n";

	++failed;
}

std::string failString(int count)
{
	if(count == 0) {
		return "All tests succeeded!\n";
	} else if(count == 1) {
		return "1 test failed!\n";
	} else {
		return std::to_string(count) + " tests failed!\n";
	}
}

} // namespace test

int main()
{
	using namespace test;
	auto aw = width + addWidth;

	for(auto t : test::tests) {
		auto prev = failed;

		auto length = std::strlen(t.second);

		for(auto i = 0u; i < std::floor((aw - length) / 2.0) - 1; ++i) std::cout << "=";
		std::cout << " ";
		std::cout << t.second;
		std::cout << " ";

		for(auto i = 0u; i < std::ceil((aw - length) / 2.0) - 1; ++i) std::cout << "=";
		std::cout << "\n\n";

		test::currentTestName = t.second;
		t.first();
		std::cout << "\t" << failString(failed - prev) << "\n";
	}

	// for(auto i = 0u; i < aw; ++i) std::cout << "*";
	// std::cout << "\n";

	auto str = "[Total]";

	auto length = std::strlen(str);
	for(auto i = 0u; i < std::floor((aw - length) / 2.0) - 1; ++i) std::cout << "*";
	std::cout << " ";
	std::cout << str;
	std::cout << " ";
	for(auto i = 0u; i < std::ceil((aw - length) / 2.0) - 1; ++i) std::cout << "*";

	std::cout << "\n" << failString(failed);
	return failed;
}
