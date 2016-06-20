#include <vpp/procAddr.hpp>
#include <vpp/vk.hpp>

#include <unordered_map>
#include <iostream>

namespace vpp
{

namespace
{
	template<typename... A> using um = std::unordered_map<A...>;
    um<vk::Instance, um<std::string, vk::PfnVoidFunction>> instanceProcs;
    um<vk::Device, um<std::string, vk::PfnVoidFunction>> deviceProcs;
}

vk::PfnVoidFunction vulkanProc(vk::Instance instance, const char* name)
{
    auto it = instanceProcs[instance].find(name);
    if(it == instanceProcs[instance].cend())
    {
        auto addr = vk::getInstanceProcAddr(instance, name);
		if(!addr)
		{
			std::cerr << "Failed to load proc " << name << " from instance " << instance << "\n";
			return nullptr;
		}

        it = instanceProcs[instance].insert({name, addr}).first;
    }

    return it->second;
}

vk::PfnVoidFunction vulkanProc(vk::Device device, const char* name)
{
    auto it = deviceProcs[device].find(name);
    if(it == deviceProcs[device].cend())
    {
        auto addr = vk::getDeviceProcAddr(device, name);
		if(!addr)
		{
			std::cerr << "Failed to load proc " << name << " from device " << device << "\n";
			return nullptr;
		}

        it = deviceProcs[device].insert({name, addr}).first;
    }

    return it->second;
}

}
