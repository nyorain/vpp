// Copyright © 2016 nyorain
//
// Permission is hereby granted, free of charge, to any person
// obtaining a copy of this software and associated documentation
// files (the “Software”), to deal in the Software without
// restriction, including without limitation the rights to use,
// copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following
// conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.

#pragma once

#include "enums.hpp"

#include <string>
#include <stdexcept>
#include <iostream>
#include <sstream>

namespace vk
{

///Default exception class that will be thrown when a thow-checked vulkan function fails.
///This exception will carry the return vulkan result code.
class VulkanError : public std::runtime_error
{
public:
	VulkanError(Result err, const std::string& msg = "") : std::runtime_error(msg), error(err) {}
	const vk::Result error;
};

namespace call
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

inline bool success(Result result)
{
	return (static_cast<std::int64_t>(result) >= 0);
}

inline std::string to_string(Result result)
{
    switch(result)
    {
	    case Result::success: return "success";
	    case Result::notReady: return "notReady";
	    case Result::timeout: return "timeout";
	    case Result::eventSet: return "eventSet";
	    case Result::eventReset: return "eventReset";
	    case Result::incomplete: return "incomplete";
	    case Result::errorOutOfHostMemory: return "errorOutOfHostMemory";
	    case Result::errorOutOfDeviceMemory: return "errorOutOfDeviceMemory";
	    case Result::errorInitializationFailed: return "errorInitializationFailed";
	    case Result::errorDeviceLost: return "errorDeviceLost";
	    case Result::errorMemoryMapFailed: return "errorMemoryMapFailed";
	    case Result::errorLayerNotPresent: return "errorLayerNotPresent";
	    case Result::errorExtensionNotPresent: return "errorExtensionNotPresent";
	    case Result::errorFeatureNotPresent: return "errorFeatureNotPresent";
	    case Result::errorIncompatibleDriver: return "errorIncompatibleDriver";
	    case Result::errorTooManyObjects: return "errorTooManyObjects";
	    case Result::errorFormatNotSupported: return "errorFormatNotSupported";
	    case Result::errorSurfaceLostKHR: return "errorSurfaceLostKHR";
	    case Result::errorNativeWindowInUseKHR: return "errorNativeWindowInUseKHR";
	    case Result::suboptimalKHR: return "suboptimalKHR";
	    case Result::errorOutOfDateKHR: return "errorOutOfDateKHR";
	    case Result::errorIncompatibleDisplayKHR: return "errorIncompatibleDisplayKHR";
	    case Result::errorValidationFailedEXT: return "errorValidationFailedEXT";
	    default: return "unknown";
    }
}

//throw
inline vk::Result checkResultThrow(vk::Result result, const char* function, const char* called)
{
	if(success(result)) return result;


	auto msg = to_string(result);
	auto ecode = static_cast<std::int64_t>(result);
	const std::string err = "Vulkan Error Code " + std::to_string(ecode) + ": " + msg +
		"\t\nin function " + function + " ,calling " + called;

    std::cerr << "vk::call::checkResultThrow will throw " << err << std::endl;
	throw VulkanError(result, err);
}

//warn
inline vk::Result checkResultWarn(vk::Result result, const char* function, const char* called)
{
	if(success(result)) return result;

	auto msg = to_string(result);
	auto ecode = static_cast<std::int64_t>(result);
	const std::string err = "Vulkan Error Code " + std::to_string(ecode) + ": " + msg +
		"\n\tin function " + function + " ,calling " + called;

	std::cerr << "vk::call::checkResultWarn: " << err << std::endl;
}

} //namespace call
} //namespace vk

//macro for querying the current function name on all platforms
#if defined(__GNUC__) || defined(__clang__)
 #define VPP_FUNC_NAME __PRETTY_FUNCTION__
#elif defined(_MSC_VER)
 #define VPP_FUNC_NAME __FUNCTION__
#else
 #define VPP_FUNC_NAME __func__
#endif

//call warn and throw macros
#define VPP_CALL_W(x) ::vk::call::checkResultWarn(static_cast<vk::Result>(x), VPP_FUNC_NAME, #x)
#define VPP_CALL_T(x) ::vk::call::checkResultThrow(static_cast<vk::Result>(x), VPP_FUNC_NAME, #x)

//default selection
//if neither debug or ndebug is defined, define it depending on NDEBUG
#if !defined(VPP_DEBUG) && !defined(VPP_NDEBUG)
 #ifndef NDEBUG
  #define VPP_DEBUG
 #endif
#endif

//if debug is defined but neither debugThrow or debugNothrow default it to defined
#if defined(VPP_DEBUG) && !defined(VPP_DEBUG_THROW) && !defined(VPP_DEBUG_NOTHROW)
 #define VPP_DEBUG_THROW
#endif

//if no call operation is defined, chose it depending on debug and debugThrow
#if !defined(VPP_CALL_THROW) && !defined(VPP_CALL_WARN) && !defined(VPP_CALL_NOCHECK)
 #ifdef VPP_DEBUG
  #define VPP_CALL_WARN
  #ifdef VPP_DEBUG_THROW
   #define VPP_CALL_THROW
  #endif
 #else
  #define VPP_CALL_NOCHECK
 #endif
#endif

//default call macro based on given option macros (throw/warn/nocheck)
#ifdef VPP_CALL_THROW
 #define VPP_CALL(x) VPP_CALL_T(x)
#elif defined(VPP_CALL_WARN)
 #define VPP_CALL(x) VPP_CALL_W(x)
#elif defined(VPP_CALL_NOCHECK)
 #define VPP_CALL(x) static_cast<vk::Result>(x)
#else //in this case neither warn, throw nor nocheck is defined.
 #error "Configuration error"
#endif
