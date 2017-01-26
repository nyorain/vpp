// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vpp/fwd.hpp>
#include <string>

namespace vpp {

/// Returns the vulkan api function address for the given function name and instance.
/// Internally uses a map to cache the pointers.
/// If the pointer is not found, returns nullptr.
/// In debug mode, outputs an error message if it could not be found.
/// Prefer to use the macro versions to assure correct casting.
vk::PfnVoidFunction vulkanProc(vk::Instance instance, const char* name);

/// Returns the vulkan api function address for the given function name and device..
/// Internally uses a map to cache the pointers.
/// If the pointer is not found, returns nullptr.
/// In debug mode, outputs an error message if it could not be found.
/// Prefer to use the macro versions to assure correct casting.
vk::PfnVoidFunction vulkanProc(vk::Device device, const char* name);

} // namespace vpp

/// Macro for calling a vulkan function pointer.
/// \param iniOrDev vulkan instance or device (depending on which kind of function pointer).
/// \param name the name of the function without vk prefix (e.g. CreateSurfaceKHR)
/// Can be e.g. used like VPP_PROC(instance, CreateSurfaceKHR)(instance, info);
/// If the function pointer cannot be retrieved this macro will evaluate to nullptr,
/// so direct calling should be used with care.
#define VPP_PROC(iniOrDev, name) \
	reinterpret_cast<::vk::Pfn##name>(::vpp::vulkanProc(iniOrDev, "vk"#name))

/// Macro for storing a vulkan function pointer.
/// \param iniOrDev vulkan instance or device (depending on which kind of function pointer).
/// \param name the name of the function pointer without vk prefix.
/// This macro creates a variable in the current scope with the name pf<name> that
/// holds a pointer to the vulkan function or a nullptr if it could not be retrieved.
#define VPP_LOAD_PROC(iniOrDev, name) \
	auto pf##name = reinterpret_cast<::vk::Pfn##name>(::vpp::vulkanProc(iniOrDev, "vk"#name))
