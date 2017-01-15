// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <sstream> // std::stringstream
#include <stdexcept>
#include <iostream>

namespace vpp {

template<typename... Args>
void outputDebugMsg(Args... args)
{
	std::stringstream stream;
	(stream << ... << args);
	std::cerr << stream.str() << '\n';

	#ifdef VPP_DEBUG_THROW
		throw std::runtime_error(stream.str());
	#endif //throw
}

} // namespace vpp

#ifdef VPP_DEBUG
	#define VPP_DEBUG_CHECK(name, code) { auto vpp_local_debug_check_name_ = #name ": "; code }
	#define VPP_DEBUG_OUTPUT(...) outputDebugMsg(vpp_local_debug_check_name_, __VA_ARGS__)
	#define VPP_DEBUG_OUTPUT_NOCHECK(...) outputDebugMsg(__VA_ARGS__)
#else
	#define VPP_DEBUG_CHECK(...)
	#define VPP_DEBUG_OUTPUT(...)
	#define VPP_DEBUG_OUTPUT_NOCHECK(...)
#endif
