#pragma once

#include <vpp/fwd.hpp>

#include <iostream>
#include <stdexcept>
#include <unordered_map>
#include <string>

namespace vpp
{

namespace detail
{
    template<typename T1, typename T2> using sup = std::unordered_map<T1, T2>;

    extern sup<VkInstance, sup<std::string, PFN_vkVoidFunction>> instanceProcs;
    extern sup<VkDevice, sup<std::string, PFN_vkVoidFunction>> deviceProcs;
}


inline vk::PfnVoidFunction instanceProc(VkInstance instance, const std::string& name)
{
    auto it = detail::instanceProcs[instance].find(name);
    if(it == detail::instanceProcs[instance].cend())
    {
        auto addr = vkGetInstanceProcAddr(instance, name.c_str());
		if(!addr)
		{
			std::cerr << "Failed to load proc " << name << " from instance " << instance << "\n";
			return nullptr;
		}

        it = detail::instanceProcs[instance].insert({name, addr}).first;
    }

    return it->second;
}

inline vk::PfnVoidFunction deviceProc(VkDevice device, const std::string& name)
{
    auto it = detail::deviceProcs[device].find(name);
    if(it == detail::deviceProcs[device].cend())
    {
        auto addr = vkGetDeviceProcAddr(device, name.c_str());
		if(!addr)
		{
			std::cerr << "Failed to load proc " << name << " from device " << device << "\n";
			return nullptr;
		}

        it = detail::deviceProcs[device].insert({name, addr}).first;
    }

    return it->second;
}

}

//macros for getting functions pointers
#define VPP_INSTANCE_PROC(inst, name) \
    reinterpret_cast<PFN_vk##name>(::vpp::instanceProc(inst, vk#name));


#define VPP_DEVICE_PROC(device, name) \
    reinterpret_cast<PFN_vk##name>(::vpp::deviceProc(device, vk#name));

//store them in a fp<> var
#define VPP_LOAD_INSTANCE_PROC(inst, name) \
        auto fp##name = reinterpret_cast<PFN_vk##name>(::vpp::instanceProc(inst, "vk"#name));

#define VPP_LOAD_DEVICE_PROC(device, name) \
        auto fp##name = reinterpret_cast<PFN_vk##name>(::vpp::deviceProc(device, "vk"#name));
