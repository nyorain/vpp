// Copyright (c) 2016-2018 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vpp/fwd.hpp>

#include <vector>
#include <string>
#include <memory>

namespace vpp {

/// RAII vk::SurfaceKHR wrapper.
class Surface {
public:
	Surface() = default;
	Surface(vk::Instance instance, vk::SurfaceKHR surface);
	~Surface();

	Surface(Surface&& other) noexcept;
	Surface& operator=(Surface other) noexcept;

	vk::Instance vkInstance() const { return instance_; }
	vk::SurfaceKHR vkHandle() const { return surface_; }

	operator vk::SurfaceKHR() const { return surface_; }
	friend void swap(Surface& a, Surface& b) noexcept;

protected:
	void release() { instance_ = {}; surface_ = {}; }

protected:
	vk::Instance instance_ {};
	vk::SurfaceKHR surface_ {};
};

// free utility functions
bool queueFamilySupported(vk::Instance, vk::SurfaceKHR, vk::PhysicalDevice, unsigned int qFamily);
std::vector<unsigned int> supportedQueueFamilies(vk::Instance, vk::SurfaceKHR, vk::PhysicalDevice);
vk::SurfaceCapabilitiesKHR capabilities(vk::Instance, vk::SurfaceKHR, vk::PhysicalDevice);
std::vector<vk::SurfaceFormatKHR> formats(vk::Instance, vk::SurfaceKHR, vk::PhysicalDevice);
std::vector<vk::PresentModeKHR> presentModes(vk::Instance, vk::SurfaceKHR, vk::PhysicalDevice);

// overloads for surface class that don't take an additional instance parameter
bool queueFamilySupported(const Surface& surface, vk::PhysicalDevice, unsigned int qFamily);
std::vector<unsigned int> supportedQueueFamilies(const Surface& surface, vk::PhysicalDevice);
vk::SurfaceCapabilitiesKHR capabilities(const Surface& surface, vk::PhysicalDevice);
std::vector<vk::SurfaceFormatKHR> formats(const Surface& surface, vk::PhysicalDevice);
std::vector<vk::PresentModeKHR> presentModes(const Surface& surface, vk::PhysicalDevice);

} // namespace vpp
