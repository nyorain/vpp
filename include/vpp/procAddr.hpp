#pragma once

#include <vpp/fwd.hpp>
#include <string>

namespace vpp
{

vk::PfnVoidFunction vulkanProc(vk::Instance instance, const char* name);
vk::PfnVoidFunction vulkanProc(vk::Device device, const char* name);

}

///Macro for calling a vulkan function pointer.
///\param iniOrDev vulkan instance or device (depending on which kind of function pointer).
///\param name the name of the function without vk prefix (e.g. CreateSurfaceKHR)
///Can be e.g. used like VPP_PROC(instance, CreateSurfaceKHR)(instance, info);
///If the function pointer cannot be retrieved this macro will evaluate to nullptr,
///so direct calling should be used with care.
#define VPP_PROC(iniOrDev, name) \
	reinterpret_cast<::vk::Pfn##name>(::vpp::vulkanProc(iniOrDev, "vk"#name))

///Macro for storing a vulkan function pointer.
///\param iniOrDev vulkan instance or device (depending on which kind of function pointer).
///\param name the name of the function pointer without vk prefix.
///This macro creates a variable in the current scope with the name pf<name> that
///holds a pointer to the vulkan function or a nullptr if it could not be retrieved.
#define VPP_LOAD_PROC(iniOrDev, name) \
	auto pf##name = reinterpret_cast<::vk::Pfn##name>(::vpp::vulkanProc(iniOrDev, "vk"#name))
