#pragma once

#include <vpp/fwd.hpp>
#include <vpp/vk.hpp> //TODO: get rid of this little piece of ...

#include <iostream>
#include <stdexcept>
#include <unordered_map>
#include <string>

namespace vpp
{

namespace detail
{
    template<typename T1, typename T2> using sup = std::unordered_map<T1, T2>;

    extern sup<vk::Instance, sup<std::string, vk::PfnVoidFunction>> instanceProcs;
    extern sup<vk::Device, sup<std::string, vk::PfnVoidFunction>> deviceProcs;
}


inline vk::PfnVoidFunction vulkanProc(vk::Instance instance, const std::string& name)
{
    auto it = detail::instanceProcs[instance].find(name);
    if(it == detail::instanceProcs[instance].cend())
    {
        auto addr = vk::getInstanceProcAddr(instance, name.c_str());
		if(!addr)
		{
			std::cerr << "Failed to load proc " << name << " from instance " << instance << "\n";
			return nullptr;
		}

        it = detail::instanceProcs[instance].insert({name, addr}).first;
    }

    return it->second;
}

inline vk::PfnVoidFunction vulkanProc(vk::Device device, const std::string& name)
{
    auto it = detail::deviceProcs[device].find(name);
    if(it == detail::deviceProcs[device].cend())
    {
        auto addr = vk::getDeviceProcAddr(device, name.c_str());
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
#define VPP_PROC(iniOrDev, name) \
    reinterpret_cast<vk::Pfn##name>(::vpp::vulkanProc(iniOrDev, "vk"#name))

//store them in a fp<> var
#define VPP_LOAD_PROC(iniOrDev, name) \
        auto pf##name = reinterpret_cast<vk::Pfn##name>(::vpp::vulkanProc(iniOrDev, "vk"#name))
