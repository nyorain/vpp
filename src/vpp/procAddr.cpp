// Copyright (c) 2016-2018 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#include <vpp/procAddr.hpp>
#include <vpp/vk.hpp>
#include <dlg/dlg.hpp>

#include <unordered_map>
#include <string>
#include <iostream>
#include <shared_mutex>
#include <mutex>

namespace vpp {
namespace {

template<typename... A> using um = std::unordered_map<A...>;

um<vk::Instance, um<std::string, vk::PfnVoidFunction>> instanceProcs;
um<vk::Device, um<std::string, vk::PfnVoidFunction>> deviceProcs;

std::shared_mutex instanceMutex;
std::shared_mutex deviceMutex;

}

using namespace std::literals::string_literals;

vk::PfnVoidFunction vulkanProc(vk::Instance instance, const char* name, bool except) {
	const static std::string err = "vpp::vulkanProc: Failed to load ini proc ";

	// try to find it
	{
		std::shared_lock lock(instanceMutex);
	    auto it = instanceProcs[instance].find(name);
	    if(it != instanceProcs[instance].cend()) {
			return it->second;
		}
	}

	// load
    auto addr = vk::getInstanceProcAddr(instance, name);
	if(!addr) {
		if(except) {
			throw std::runtime_error(err + name);
		}
		return nullptr;
	}

	// insert
	std::scoped_lock lock(instanceMutex);
	instanceProcs[instance].insert({name, addr});
	return addr;
}

vk::PfnVoidFunction vulkanProc(vk::Device device, const char* name, bool except) {
	const static std::string err = "vpp::vulkanProc: Failed to load dev proc ";

	// try to find it
	{
		std::shared_lock lock(deviceMutex);
	    auto it = deviceProcs[device].find(name);
	    if(it != deviceProcs[device].cend()) {
			return it->second;
		}
	}

	// load
    auto addr = vk::getDeviceProcAddr(device, name);
	if(!addr) {
		if(except) {
			throw std::runtime_error(err + name);
		}

		return nullptr;
	}

	// insert
	std::scoped_lock lock(deviceMutex);
	deviceProcs[device].insert({name, addr});
	return addr;
}

} // namespace vpp
