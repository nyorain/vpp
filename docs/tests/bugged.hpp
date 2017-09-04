// This is free and unencumbered software released into the public domain.
//
// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software, either in source code form or as a compiled
// binary, for any purpose, commercial or non-commercial, and by any
// means.
//
// In jurisdictions that recognize copyright laws, the author or authors
// of this software dedicate any and all copyright interest in the
// software to the public domain. We make this dedication for the benefit
// of the public at large and to the detriment of our heirs and
// successors. We intend this dedication to be an overt act of
// relinquishment in perpetuity of all present and future rights to this
// software under copyright law.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
//
// For more information, please refer to <http://unlicense.org>

// Written by nyorain.
// For issues and (appreciated) help, see https://github.com/nyorain/bugged.
// For more information and usage, see example.cpp or README.md
// Extremely lightweight header-only unit testing for C++.
// Use the TEST(name), EXPECT(expression, expected) and ERROR(expression, error) macros.
// Configuration useful for inline testing (define before including the file):
//  - BUGGED_NO_MAIN: don't include the main function that just executes all tests.
//  - BUGGED_NO_IMPL: don't include the implementation.

#pragma once

#include <string> // std::string
#include <cstring> // std::strrchr
#include <typeinfo> // std::type_traits
#include <vector> // std::vector
#include <iostream> // std::cout
#include <sstream> // std::stringstream
#include <exception> // std::exception

namespace bugged {

// Utility
namespace {
	template<typename... T> constexpr void unused(T&&...) {}
	template<typename... T> using void_t = void;
}

/// Class used for printing objects to the debug output.
/// Provided implementations just return the object when it is printable
/// a string with type and address of the object.
/// The call operations retrieves something from the object that can be printed
/// ot an ostream (the object itself or an alternative descriptive string).
/// \tparam T The type of objects to print.
template<typename T, typename C = void>
struct Printable {
	static std::string call(const T&)
	{
		static auto txt = std::string("<not printable : ") + typeid(T).name() + std::string(">");
		return txt;
	}
};

/// Default printable specialization for types that can itself
template<typename T>
struct Printable<T, void_t<decltype(std::declval<std::ostream&>() << std::declval<T>())>> {
	static const T& call(const T& obj) { return obj; }
};

/// Uses the Printable template to retrieve something printable to an ostream
/// from the given object.
template<typename T>
auto printable(const T& obj) -> decltype(Printable<T>::call(obj))
	{ return Printable<T>::call(obj); }

/// Strips the path from the given filename
const char* stripPath(const char* path)
{
	auto pos = std::strrchr(path, '/');
	return pos ? pos + 1 : path;
}

/// Static class that holds all test to be run.
class Testing {
public:
	/// Holds all the available info for a failed test.
	struct FailInfo {
		int line;
		const char* file;
	};

	/// Represents a unit to test.
	struct Unit {
		using FuncPtr = void(*)();
		std::string name;
		FuncPtr func;
		const char* file;
		unsigned int line;
	};

public:
	// Config variables
	// the ostream to output to. Must not be set to nullptr. Defaulted to std::cout
	static std::ostream* output;

	// The width of the failure separator. Defaulted to 70.
	static unsigned int separationWidth;

	// The char to use in the failure separator line. Defaulted to '-'
	static char failSeparator;

	// The char to use in the bottom separator line. Defaulted to '='
	static char bottomSeparator;

	// The escape sequences to use to style the output.
	// Will all be empty if not on unix.
	struct Escape {
		static const char* testName;
		static const char* checkExpected;
		static const char* checkActual;
		static const char* errorExpected;
		static const char* exception;
		static const char* source;
		static const char* reset;
	};

public:
	/// Called when a check expect fails.
	/// Will increase the current fail count and print a debug message for
	/// the given information.
	/// Should be only called inside of a testing unit.
	template<typename V, typename E>
	static inline void expectFailed(const FailInfo&, const V& value, const E& expected);

	/// Called when a check error fails.
	/// Should be only called inside of a testing unit.
	/// \param error The name of the expected error type
	/// \param other empty if there was no other error, the type and message of the
	/// other error thrown otherwise.
	static inline void errorFailed(const FailInfo&, const char* error, const std::string& other);

	/// Adds the given unit to the list of units to test.
	/// Always returns 0, useful for static calling.
	static inline int add(const Unit&);

	/// Tests all registered units.
	/// Returns the number of units failed.
	static inline unsigned int run();

	/// Outputs a separation line.
	static void separationLine(bool beginning);

protected:
	/// Returns a string for the given number of failed tests.
	static inline std::string failString(unsigned int failCount, const char* type);

	/// Prints the error for an unexpected exception
	static inline void unexpectedException(const std::string& errorString);

	static std::vector<Unit> units;
	static unsigned int currentFailed;
	static unsigned int totalFailed;
	static unsigned int unitsFailed;
	static Unit* currentUnit;
};

} // namespace bugged

/// Declares a new testing unit. After this macro the function body should follow like this:
/// ``` TEST(SampleTest) { EXPECT(1 + 1, 2); } ```
#define TEST(name) \
	static void BUGGED_##name##_U(); \
	namespace { static const auto BUGGED_##name = ::bugged::Testing::add({#name, \
		BUGGED_##name##_U, ::bugged::stripPath(__FILE__), __LINE__}); } \
	static void BUGGED_##name##_U()

/// Expects the two given values to be equal.
#define EXPECT(expr, expected) \
	{ ::bugged::checkExpect({__LINE__, ::bugged::stripPath(__FILE__)}, expr, expected); }

/// Expects the given expression to throw an error of the given type when
/// evaluated.
#define ERROR(expr, error) { \
	std::string TEST_altMsg {}; \
	if(!::bugged::detail::ErrorTest<error>::call([&]{ expr; }, TEST_altMsg)) \
			::bugged::Testing::errorFailed({__LINE__, ::bugged::stripPath(__FILE__)}, \
			 	#error, TEST_altMsg.c_str()); \
	}

// Implementation
#ifndef BUGGED_NO_IMPL

namespace bugged {

unsigned int Testing::separationWidth = 55;
char Testing::failSeparator = '-';
char Testing::bottomSeparator = '=';

#if defined(__unix__) || defined(__linux__) || defined(__APPLE__)
	const char* Testing::Escape::testName = "\033[33m";
	const char* Testing::Escape::checkExpected = "\033[32m";
	const char* Testing::Escape::checkActual = "\033[31m";
	const char* Testing::Escape::errorExpected = "\033[32m";
	const char* Testing::Escape::exception = "\033[31m";
	const char* Testing::Escape::source = "\033[36m";
	const char* Testing::Escape::reset = "\033[0m";
#else
	const char* Testing::Escape::testName = "";
	const char* Testing::Escape::checkExpected = "";
	const char* Testing::Escape::checkActual = "";
	const char* Testing::Escape::errorExpected = "";
	const char* Testing::Escape::exception = "";
	const char* Testing::Escape::source = "";
	const char* Testing::Escape::reset = "";
#endif


std::vector<Testing::Unit> Testing::units {};
unsigned int Testing::currentFailed {};
unsigned int Testing::totalFailed {};
unsigned int Testing::unitsFailed {};
Testing::Unit* Testing::currentUnit {};
std::ostream* Testing::output = &std::cout;

// Utility method used by EXPECT to assure the given expressions are evaluated
// exactly once
template<typename V, typename E>
void checkExpect(const Testing::FailInfo& info, const V& value, const E& expected)
{
	if(value != expected)
		Testing::expectFailed(info, value, expected);
}

void Testing::separationLine(bool beginning)
{
	if(beginning && !totalFailed && !currentFailed && !unitsFailed)
		return;

	for(auto i = 0u; i < separationWidth; ++i)
		std::cout << failSeparator;

	std::cout << "\n";
}

template<typename V, typename E>
void Testing::expectFailed(const FailInfo& info, const V& value, const E& expected)
{
	separationLine(true);

	std::cout << "[" << Escape::source << info.file << ":" << info.line
			  << Escape::reset << " | " << Escape::testName << currentUnit->name
	  		  << Escape::reset << "] Check expect failed:\nGot: '"
		 	  << Escape::checkActual << printable(value)
			  << Escape::reset << "' instead of '" << Escape::checkExpected << printable(expected)
			  << Escape::reset << "'\n";

	++currentFailed;
}

void Testing::errorFailed(const FailInfo& info, const char* error, const std::string& other)
{
	separationLine(true);

	std::cout << "[" << Escape::source << info.file << ":" << info.line
			  << Escape::reset << " | " << Escape::testName << currentUnit->name
			  << Escape::reset << "] Check error failed:\n"
		 	  << "Expected '" << Escape::errorExpected << error << Escape::reset << "', ";

	if(!other.empty()) {
		std::cout << "got other error: \n"
			 	  << Escape::exception << other << Escape::reset << "\n";
	} else {
		std::cout << "no error was thrown\n";
	}

	++currentFailed;
}

void Testing::unexpectedException(const std::string& errorString)
{
	separationLine(true);

	std::cout << "[" << Escape::source << currentUnit->file << ":" << currentUnit->line
			  << Escape::reset << " | " << Escape::testName << currentUnit->name
			  << Escape::reset << "] Unexpected exception:\n"
	  		  << Escape::exception << errorString << Escape::reset << "\n";
}


namespace detail {

/// Tries to catch an error of the given type in the given function.
/// Specialization needed to surpress warnings when E == std::exception.
template<typename E>
struct ErrorTest {
	template<typename F>
	static bool call(const F& func, std::string& msg)
	{
		try {
			func();
		} catch(const E&) {
			return true;
		} catch(const std::exception& err) {
			msg = "std::exception: ";
			msg += err.what();
		} catch(...) {
			msg = "<Not a std::exception>";
		}

		return false;
	}
};

template<>
struct ErrorTest<std::exception> {
	template<typename F>
	static bool call(const F& func, std::string& msg)
	{
		try {
			func();
		} catch(const std::exception&) {
			return true;
		} catch(...) {
			msg = "<Not a std::exception>";
		}

		return false;
	}
};

} // namespace detail

int Testing::add(const Unit& unit)
{
	units.push_back(unit);
	return 0;
}

unsigned int Testing::run()
{
	for(auto unit : units) {
		currentFailed = 0;

		currentUnit = &unit;
		auto thrown = false;

		try {
			unit.func();
		} catch(const std::exception& exception) {
			thrown = true;
			unexpectedException(std::string("std::exception: ") + exception.what());
		} catch(...) {
			thrown = true;
			unexpectedException("<Not a std::exception object>");
		}

		if(thrown || currentFailed)
			++unitsFailed;
		totalFailed += currentFailed;
	}

	if(totalFailed) {
		for(auto i = 0u; i < separationWidth; ++i)
			*output << bottomSeparator;
		*output << "\n";
	}

	*output << failString(unitsFailed, "unit") << ", "
			<< failString(totalFailed, "call") << "\n";
	return unitsFailed;
}

std::string Testing::failString(unsigned int failCount, const char* type)
{
	if(failCount == 0) {
		return std::string("All ").append(type).append("s succeeded");
	} else if(failCount == 1) {
		return std::string("1 ").append(type).append(" failed");
	} else {
		return std::to_string(failCount).append(" ").append(type).append("s failed");
	}
}

} // namespace bugged

#endif // BUGGED_NO_IMPL

// Main function
#ifndef BUGGED_NO_MAIN
	int main() { return bugged::Testing::run(); }
#endif // BUGGED_NO_MAIN
