// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vkpp/enums.hpp>

#include <string>
#include <stdexcept>

namespace vk {

/// Returns the name of the given vulkan result.
/// Returns "<unknown>" if the error is not recognized.
/// Might handle for extension result codes that are newer than
/// the last vkpp version.
inline const char* name(Result result)
{
	switch(result) {
		case Result::success: return "success";
		case Result::notReady: return "notReady";
		case Result::timeout: return "timeout";
		case Result::eventSet: return "eventSet";
		case Result::eventReset: return "eventReset";
		case Result::incomplete: return "incomplete";
		case Result::suboptimalKHR: return "suboptimalKHR";

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
		case Result::errorOutOfDateKHR: return "errorOutOfDateKHR";
		case Result::errorIncompatibleDisplayKHR: return "errorIncompatibleDisplayKHR";
		case Result::errorValidationFailedEXT: return "errorValidationFailedEXT";
		default: return "<unknown>";
	}
}

/// Default exception class that will be thrown when a throw-checked vulkan function fails.
/// This exception will carry the return vulkan result code.
class VulkanError : public std::runtime_error {
public:
	VulkanError(Result err, const std::string& msg = "") : 
		std::runtime_error(msg), error(err) {}

	const Result error {};
};

namespace error {

/// Returns whether the error code inidicates success.
inline bool success(vk::Result result)
{
	return static_cast<std::int64_t>(result) >= 0;
}

/// Will check if the given result indicated failure and if so throw a VulkanError
/// containing information about the error code.
inline vk::Result checkResultThrow(vk::Result result, const char* function)
{
	if(success(result)) {
		return result;
	}

	auto msg = name(result);
	auto str = std::to_string(static_cast<std::int64_t>(result));
	auto err = "Vulkan Error Code " + str + ": " + msg + "\n"
		"\tin function: " + function;
	throw VulkanError(result, err);
}

} // namespace error
} // namespace vk

#ifndef VKPP_CALL_THROW
	#ifdef NDEBUG
		#define VKPP_CALL_THROW 0
	#else
		#define VKPP_CALL_THROW 1
	#endif
#endif

// Macro that can be wrapped around plain valkan api calls to throw if they return
// a failure result. When VKPP_CALL_THROW is not defined (which is by default the case
// for non debug builds) no check will be performed at all.
#ifndef VKPP_CALL
	#if VKPP_CALL_THROW
		#define VKPP_CALL(x) ::vk::error::checkResultThrow(static_cast<vk::Result>(x), __func__)
	#else
		#define VKPP_CALL(x) static_cast<vk::Result>(x)
	#endif
#endif
