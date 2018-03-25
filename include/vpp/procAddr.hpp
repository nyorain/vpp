// Copyright (c) 2016-2018 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vpp/fwd.hpp>
#include <string>

namespace vpp {

/// Returns the vulkan api function address for the given function name and instance.
/// Internally uses a map to cache the pointers.
/// If the pointer is not found, returns nullptr.
/// Prefer to use the macro versions to ensure correct casting.
/// \param except If this is true, throws an exception if the proc could not be loaded.
/// Otherwise, if in debug mode, outputs an error message if it could not be found.
vk::PfnVoidFunction vulkanProc(vk::Instance instance, const char* name, bool except = true);

/// Returns the vulkan api function address for the given function name and device..
/// Internally uses a map to cache the pointers.
/// If the pointer is not found, returns nullptr.
/// Prefer to use the macro versions to ensure correct casting.
/// \param except If this is true, throws an exception if the proc could not be loaded.
/// Otherwise, if in debug mode, outputs an error message if it could not be found.
vk::PfnVoidFunction vulkanProc(vk::Device device, const char* name, bool except = true);

} // namespace vpp

/// Macro for calling a vulkan function pointer.
/// \param iniOrDev vulkan instance or device (depending on which kind of function pointer).
/// \param name the name of the function without vk prefix (e.g. CreateSurfaceKHR)
/// Can be e.g. used like VPP_PROC(instance, CreateSurfaceKHR)(instance, info);
/// Throws an exception if the proc could not be loaded.
///
/// Examples:
/// VPP_LOAD_PROC(device, CreateSwapchainKHR)(...);
#define VPP_PROC(iniOrDev, name) \
	reinterpret_cast<::vk::Pfn##name>(::vpp::vulkanProc(iniOrDev, "vk"#name))

/// Like VPP_PROC but does not throw an exception if a function could not be loaded,
/// but instead simply evaluates to nullptr, so direct calling should be used with care.
#define VPP_PROC_NOTHROW(iniOrDev, name) \
	reinterpret_cast<::vk::Pfn##name>(::vpp::vulkanProc(iniOrDev, "vk"#name, false))


/// Macro for storing a vulkan function pointer.
/// \param iniOrDev vulkan instance or device (depending on which kind of function pointer).
/// \param name the name of the function pointer without vk prefix.
/// This macro creates a variable in the current scope with the name pf<name> that
/// holds a pointer to the vulkan function.
/// Throws an exception if the proc could not be loaded.
///
/// Examples:
/// VPP_LOAD_PROC(instance, DestroySurfaceKHR);
/// pfDestroySurfaceKHR(...);
#define VPP_LOAD_PROC(iniOrDev, name) \
	auto pf##name = reinterpret_cast<::vk::Pfn##name>(::vpp::vulkanProc(iniOrDev, "vk"#name))

/// Like VPP_PROC but does not throw an exception if a function could not be loaded,
/// but instead simply evaluates to nullptr, so direct calling should be used with care.
#define VPP_LOAD_PROC_NOTHROW(iniOrDev, name) \
	auto pf##name = reinterpret_cast<::vk::Pfn##name>(::vpp::vulkanProc(iniOrDev, "vk"#name, false))
