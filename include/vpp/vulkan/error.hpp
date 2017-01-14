// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include "enums.hpp"

#include <string>
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <system_error>

namespace vk {

/// Returns the name of the given vulkan result.
/// Returns "<unknown>" if the error is not recognized.
/// Might handle for extension result codes that are newer than
/// the last vpp version.
inline const char* name(Result result)
{
	switch(result) {
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
		case Result::errorInvalidShaderNV: return "errorInvalidShaderNV";
		case Result::nvExtension1Error: return "nvExtension1Error";
		default: return "<unknown>";
	}
}

/// Default exception class that will be thrown when a thow-checked vulkan function fails.
/// This exception will carry the return vulkan result code.
class VulkanError : public std::runtime_error {
public:
	VulkanError(Result err, const std::string& msg = "") : std::runtime_error(msg), error(err) {}
	const Result error {};
};

/// std::error_category implementation for vulkan result codes.
class VulkanErrorCategory : public std::error_category {
public:
	const char* name() const noexcept override { return "vk::VulkanErrorCateogry"; }
	std::string message(int code) const override
	{
		return ::vk::name(static_cast<vk::Result>(code));
	}
};

namespace error {

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

//throw
inline vk::Result checkResultThrow(vk::Result result, const char* function, const char* called)
{
	if(success(result)) return result;


	auto msg = name(result);
	auto ecode = static_cast<std::int64_t>(result);
	const std::string err = "Vulkan Error Code " + std::to_string(ecode) + ": " + msg +
		"\t\nin function " + function + ", calling " + called;

	std::cerr << "vk::call::checkResultThrow will throw " << err << std::endl;
	throw VulkanError(result, err);
}

//warn
inline vk::Result checkResultWarn(vk::Result result, const char* function, const char* called)
{
	if(success(result)) return result;

	auto msg = name(result);
	auto ecode = static_cast<std::int64_t>(result);
	const std::string err = "Vulkan Error Code " + std::to_string(ecode) + ": " + msg +
		"\n\tin function " + function + ", calling " + called;

	std::cerr << "vk::call::checkResultWarn: " << err << std::endl;
	return result;
}

} //namespace call
} //namespace vk

/// Specialize error code/condition type traits for the vulkan result codes.
/// This makes it possible to use them directly as error codes.
namespace std {
    template<>
    struct is_error_code_enum<vk::Result>
      : public true_type {};

    template<>
    struct is_error_condition_enum<vk::Result>
      : public true_type {};
} // namespace std

// macro for querying the current function name on all platforms
#if defined(__GNUC__) || defined(__clang__)
 #define VPP_FUNC_NAME __PRETTY_FUNCTION__
#elif defined(_MSC_VER)
 #define VPP_FUNC_NAME __FUNCTION__
#else
 #define VPP_FUNC_NAME __func__
#endif

// call warn and throw macros
#define VPP_CALL_W(x) ::vk::error::checkResultWarn(static_cast<vk::Result>(x), VPP_FUNC_NAME, #x)
#define VPP_CALL_T(x) ::vk::error::checkResultThrow(static_cast<vk::Result>(x), VPP_FUNC_NAME, #x)

// default selection
// if neither VPP_DEBUG or VPP_NDEBUG is defined, define it depending on NDEBUG
#if !defined(VPP_DEBUG) && !defined(VPP_NDEBUG)
 #ifndef NDEBUG
  #define VPP_DEBUG
 #endif
#endif

// if debug is defined but neither debugThrow or debugNothrow default it to defined
#if defined(VPP_DEBUG) && !defined(VPP_DEBUG_THROW) && !defined(VPP_DEBUG_NOTHROW)
 #define VPP_DEBUG_THROW
#endif

// if no call operation is defined, chose it depending on debug and debugThrow
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

// default call macro based on given option macros (throw/warn/nocheck)
#ifdef VPP_CALL_THROW
 #define VPP_CALL(x) VPP_CALL_T(x)
#elif defined(VPP_CALL_WARN)
 #define VPP_CALL(x) VPP_CALL_W(x)
#elif defined(VPP_CALL_NOCHECK)
 #define VPP_CALL(x) static_cast<vk::Result>(x)
#else
 // This error shoud not happen! Please report to the vpp maintainers
 #error Configuration error
#endif
