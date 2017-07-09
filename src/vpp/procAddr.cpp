// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#include <vpp/procAddr.hpp>
#include <vpp/util/sharedLock.hpp>
#include <vpp/util/log.hpp>
#include <vpp/vk.hpp>

#include <unordered_map>
#include <string>
#include <iostream>
#include <shared_mutex>

using namespace std::literals::string_literals;

namespace vpp {
namespace {

template<typename... A> using um = std::unordered_map<A...>;

um<vk::Instance, um<std::string, vk::PfnVoidFunction>> instanceProcs;
um<vk::Device, um<std::string, vk::PfnVoidFunction>> deviceProcs;

// TODO: C++17: use plain shared mutex for efficiency
std::shared_mutex instanceMutex;
std::shared_mutex deviceMutex;

}

vk::PfnVoidFunction vulkanProc(vk::Instance instance, const char* name, bool except)
{
	// try to find it
	{
		SharedLockGuard<std::shared_mutex> lock(instanceMutex);
	    auto it = instanceProcs[instance].find(name);
	    if(it != instanceProcs[instance].cend())
			return it->second;
	}

	// load
    auto addr = vk::getInstanceProcAddr(instance, name);
	if(!addr) {
		auto msg = "vpp::vulkanProc: Failed to load instance proc "s + name;
		if(except) throw std::runtime_error(msg);

		vpp_warn("vulkanProc"_scope, "Failed to load instance proc {}", name);
		return nullptr;
	}

	// insert
	{
		std::lock_guard<std::shared_mutex> lock(instanceMutex);
        instanceProcs[instance].insert({name, addr});
	}

	return addr;
}

vk::PfnVoidFunction vulkanProc(vk::Device device, const char* name, bool except)
{
	// try to find it
	{
		SharedLockGuard<std::shared_mutex> lock(deviceMutex);
	    auto it = deviceProcs[device].find(name);
	    if(it != deviceProcs[device].cend())
			return it->second;
	}

	// load
    auto addr = vk::getDeviceProcAddr(device, name);
	if(!addr) {
		auto msg = "vpp::vulkanProc: Failed to load device proc "s + name;
		if(except) throw std::runtime_error(msg);

		vpp_warn("vulkanProc"_scope, "Failed to load instance proc {}", name);
		return nullptr;
	}

	// insert
	{
		std::lock_guard<std::shared_mutex> lock(deviceMutex);
        deviceProcs[device].insert({name, addr});
	}

	return addr;
}

} // namespace vpp
