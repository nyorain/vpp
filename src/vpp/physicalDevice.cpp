// Copyright (c) 2016-2018 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#include <vpp/physicalDevice.hpp>
#include <vpp/procAddr.hpp>
#include <vpp/vk.hpp>
#include <vector>

namespace vpp {

vk::PhysicalDevice choose(nytl::Span<vk::PhysicalDevice> phdevs)
{
	vk::PhysicalDevice best = {};
	auto bestScore = 0u;

	// TODO: better querying! consider memory size and stuff
	for(auto& phdev : phdevs) {
		auto props = vk::getPhysicalDeviceProperties(phdev);
		auto score = 0u;

		if(props.deviceType == vk::PhysicalDeviceType::discreteGpu) {
			score = 5;
		} else if(props.deviceType == vk::PhysicalDeviceType::integratedGpu) {
			score = 4;
		} else if(props.deviceType == vk::PhysicalDeviceType::virtualGpu) {
			score = 3;
		} else if(props.deviceType == vk::PhysicalDeviceType::cpu) {
			score = 2;
		} else {
			score = 1u;
		}

		if(score > bestScore) {
			bestScore = score;
			best = phdev;
		}
	}

	return best;
}

vk::PhysicalDevice choose(nytl::Span<vk::PhysicalDevice> phdevs, 
	vk::Instance instance, vk::SurfaceKHR surface)
{
	std::vector<vk::PhysicalDevice> supported;
	supported.reserve(phdevs.size());

	// first check for all passed physical devices if it has any
	// queue that can present on the passed device
	// if so, insert it into the supported vector
	for(auto phdev : phdevs) {
		if(findQueueFamily(phdev, instance, surface) != -1) {
			supported.push_back(phdev);
		}
	}

	// default choose by usability from the phdevs that support presenting on the surface
	return choose(supported);
}

const char* name(vk::PhysicalDeviceType type)
{
	switch(type) {
		case vk::PhysicalDeviceType::discreteGpu: 
			return "discreteGpu";
		case vk::PhysicalDeviceType::integratedGpu: 
			return "integratedGpu";
		case vk::PhysicalDeviceType::virtualGpu: 
			return "virtualGpu";
		case vk::PhysicalDeviceType::other: 
			return "other";
		default: 
			return "<unknown>";
	}
}

std::array<unsigned int, 3> apiVersion(uint32_t v)
{
	return {v >> 22, (v >> 12) & 0x3ff, v & 0xfff};
}

std::string description(vk::PhysicalDevice phdev, const char* sep)
{
	std::string ret;
	auto props = vk::getPhysicalDeviceProperties(phdev);

	ret += "Name: ";
	ret += props.deviceName.data();
	ret += sep;

	ret += "Type: ";
	ret += name(props.deviceType);
	ret += sep;

	auto v = apiVersion(props.apiVersion);
	ret += "Api version: ";
	ret += std::to_string(v[0]);
	ret += ".";
	ret += std::to_string(v[1]);
	ret += ".";
	ret += std::to_string(v[2]);
	ret += " (" + std::to_string(props.apiVersion) + ")";
	ret += sep;

	ret += "Device ID: ";
	ret += std::to_string(props.deviceID);
	ret += sep;

	ret += "Vendor ID: ";
	ret += std::to_string(props.vendorID);
	ret += sep;

	ret += "Driver version: ";
	ret += std::to_string(props.driverVersion);

	return ret;
}

int findQueueFamily(vk::PhysicalDevice phdev, vk::QueueFlags flags, OptimizeQueueFamily optimize)
{
	auto queueProps = vk::getPhysicalDeviceQueueFamilyProperties(phdev);

	auto highestCount = 0u;
	auto minGranSum = 0u;
	int best = -1;

	// iterator though all families, check if flags are supported
	// if so, check if better than current optimum value
	for(auto i = 0u; i < queueProps.size(); ++i) {
		if((queueProps[i].queueFlags & flags) != flags) continue;
		if(optimize == OptimizeQueueFamily::none) return i;

		if(optimize == OptimizeQueueFamily::highestCount) {
			auto count = queueProps[i].queueCount;
			if(count > highestCount) {
				highestCount = count;
				best = i;
			}
		} else if(optimize == OptimizeQueueFamily::minImageGranularity) {
			auto gran = queueProps[i].minImageTransferGranularity;
			auto granSum = gran.width + gran.height + gran.depth;
			if(granSum != 0 && (granSum < minGranSum || minGranSum == 0)) {
				minGranSum = granSum;
				best = i;
			}
		}
	}

	return best;
}

int findQueueFamily(vk::PhysicalDevice phdev, vk::Instance instance, vk::SurfaceKHR surface,
	vk::QueueFlags flags, OptimizeQueueFamily optimize)
{
	VPP_LOAD_PROC(instance, GetPhysicalDeviceSurfaceSupportKHR);
	if(!pfGetPhysicalDeviceSurfaceSupportKHR) return -1;

	auto queueProps = vk::getPhysicalDeviceQueueFamilyProperties(phdev);

	auto highestCount = 0u;
	auto minGranSum = 0u;
	int best = -1;

	// iterate though all families, check if flags and surface are supported
	// if so, check if better than current optimum value
	for(auto i = 0u; i < queueProps.size(); ++i) {
		if((queueProps[i].queueFlags & flags) != flags) continue;

		vk::Bool32 ret;
		VKPP_CALL(pfGetPhysicalDeviceSurfaceSupportKHR(phdev, i, surface, &ret));
		if(!ret) continue;

		if(optimize == OptimizeQueueFamily::none) return i;

		if(optimize == OptimizeQueueFamily::highestCount) {
			auto count = queueProps[i].queueCount;
			if(count > highestCount) {
				highestCount = count;
				best = i;
			}
		} else if(optimize == OptimizeQueueFamily::minImageGranularity) {
			auto gran = queueProps[i].minImageTransferGranularity;
			auto granSum = gran.width + gran.height + gran.depth;
			if(granSum != 0 && (granSum < minGranSum || minGranSum == 0)) {
				minGranSum = granSum;
				best = i;
			}
		}
	}

	return best;
}

} // namespace vpp
