// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include "enums.hpp"

#include <string>
#include <stdexcept>
#include <iostream>
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

/// Returns whether the error code inidicates success.
inline bool success(vk::Result result)
{
	return static_cast<std::int64_t>(result) >= 0;
}

/// Will check if the given result indicated failure and if so throw a VulkanError
/// containing information about the error code.
inline vk::Result checkResultThrow(vk::Result result, const char* function, const char* called)
{
	auto ecode = static_cast<std::int64_t>(result);
	if(ecode >= 0) return result;

	auto msg = name(result);
	const std::string err = "Vulkan Error Code " + std::to_string(ecode) + ": " + msg +
		"\n\tin function: " + function + "\n\tcalling: " + called;
	throw VulkanError(result, err);
}

} //namespace error
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

// Macro for querying a descriptive current function name.
#if defined(__GNUC__) || defined(__clang__)
	#define VPP_FUNC_NAME __PRETTY_FUNCTION__
#elif defined(_MSC_VER)
	#define VPP_FUNC_NAME __FUNCTION__
#else
	#define VPP_FUNC_NAME __func__
#endif

// - Default selection -
// If neither VPP_DEBUG or VPP_NDEBUG is defined, define it depending on NDEBUG
#if !defined(VPP_DEBUG) && !defined(VPP_NDEBUG)
	#ifndef NDEBUG
		#define VPP_DEBUG
	#else
		#define VPP_NDEBUG
	#endif
#endif

// If no call operation is defined, it is chosen depending on VPP_DEBUG.
#if !defined(VPP_CALL_THROW) && !defined(VPP_CALL_NOCHECK)
	#ifdef VPP_DEBUG
		#define VPP_CALL_THROW
	#else
		#define VPP_CALL_NOCHECK
	#endif
#endif

// Macro that can be wrapped around plain valkan api calls to throw if they return
// a failure result. When VPP_CALL_THROW is not defined (which is by default the case
// for non debug builds) no check will be performed at all.
#if defined(VPP_CALL_THROW) && !defined(VPP_CALL)
	#define VPP_CALL(x) ::vk::error::checkResultThrow(static_cast<vk::Result>(x), VPP_FUNC_NAME, #x)
#elif !defined(VPP_CALL)
	#define VPP_CALL(x) static_cast<vk::Result>(x)
#endif
