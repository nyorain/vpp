#pragma once

#include <sstream>
#include <stdexcept>
#include <iostream>

namespace vpp
{

template<typename... Args>
void outputDebugMsg(Args... args)
{
	std::stringstream stream;
	int e1[] = {(stream << args, 0)...};
	std::cerr << stream.str() << '\n';

	#ifdef VPP_DEBUG_THROW
	 throw std::runtime_error(stream.str());
	#endif //throw
}

}

#ifdef VPP_DEBUG
 #define VPP_DEBUG_CHECK(name, code) auto vpp_local_debug_check_name_ = #name ": "; code
 #define VPP_DEBUG_OUTPUT(...) outputDebugMsg(vpp_local_debug_check_name_, __VA_ARGS__)
 #define VPP_DEBUG_OUTPUT_NOCHECK(...) outputDebugMsg(__VA_ARGS__)
#else
 #define VPP_DEBUG_CHECK(...)
 #define VPP_DEBUG_OUTPUT(...)
 #define VPP_DEBUG_OUTPUT_NOCHECK(...)
#endif
