// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <sstream> // std::stringstream
#include <stdexcept> // std::runtime_error
#include <iostream> // std:;cerr

namespace vpp {

template<typename... Args>
void throwRuntime(Args... args)
{
	std::stringstream stream;
	(stream << ... << args);
	throw std::runtime_error(stream.str());
}

template<typename... Args>
void warn(Args... args)
{
	std::stringstream stream;
	(stream << ... << args);
	std::cerr << "Warning: " << stream.str() << '\n';
}

} // namespace vpp

/// Macros for additional debug build checks.
/// Can be used like this:
/// ```cpp
/// void foo()
/// {
/// 	VPP_DEBUG_CHECK("foo", {
///			VPP_CHECK_THROW("critical error!");
///			VPP_CHECK_WARN("just a warning");
/// 	});
///
/// 	VPP_DEBUG_THROW("foo: this is thrown in debug mode");
/// 	VPP_DEBUG_WARN("foo: this is printed in debug mode");
/// }
/// ```
/// The first parameter of VPP_DEBUG_CHECK should be a name for the current function.
/// Prefer to use the debug and debug check throw versions over the warn versions since
/// these macros only have an effect in debug mode anyways.
/// The warn macros must be used in destructor or noexcept functions.
/// Using macros allows to achieve zero overhead for release builds.
/// Note that debug-depenent behaviour should only be used to checks and not alter
/// other behaviour of a function or class. If a check fails, the function should e.g.
/// not return early (except throwing an error).
#ifdef VPP_DEBUG
	#define VPP_DEBUG_CHECK(name, code) { auto vpp_local_debug_check_name_ = name; code }
	#define VPP_CHECK_THROW(...) vpp::throwRuntime(vpp_local_debug_check_name_, ": ", __VA_ARGS__)
	#define VPP_CHECK_WARN(...) vpp::warn(vpp_local_debug_check_name_, ": ", __VA_ARGS__)
	#define VPP_DEBUG_THROW(...) vpp::throwRuntime(__VA_ARGS__)
	#define VPP_DEBUG_WARN(...) vpp::warn(__VA_ARGS__)
#else
	#define VPP_DEBUG_CHECK(...)
	#define VPP_CHECK_THROW(...)
	#define VPP_CHECK_WARN(...)
	#define VPP_DEBUG_THROW(...)
	#define VPP_DEBUG_WARN(...)
#endif
