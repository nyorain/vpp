// Copyright (c) 2016-2018 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#include <vpp/surface.hpp>
#include <vpp/vk.hpp>
#include <vpp/procAddr.hpp>

using vk::call::call;

namespace vpp {

Surface::Surface(vk::Instance instance, vk::SurfaceKHR surface)
	: instance_(instance), surface_(surface) {
}

Surface::~Surface() {
	if(vkHandle()) {
		VPP_LOAD_PROC(vkInstance(), DestroySurfaceKHR);
		call(pfDestroySurfaceKHR, vkInstance(), vkHandle(), nullptr);
	}
}

Surface::Surface(Surface&& other) noexcept {
	swap(*this, other);
}

Surface& Surface::operator=(Surface other) noexcept {
	swap(*this, other);
	return *this;
}

void swap(Surface& a, Surface& b) noexcept {
	using std::swap;

	std::swap(a.instance_, b.instance_);
	std::swap(a.surface_, b.surface_);
}

// free functions
bool queueFamilySupported(vk::Instance ini, vk::SurfaceKHR surf, vk::PhysicalDevice phdev,
		unsigned int qFamiliyIndex) {
	VPP_LOAD_PROC(ini, GetPhysicalDeviceSurfaceSupportKHR);
	vk::Bool32 ret;
	VKPP_CHECK(call(pfGetPhysicalDeviceSurfaceSupportKHR, phdev,
		qFamiliyIndex, surf, &ret));
	return ret == VK_TRUE;
}

std::vector<unsigned int> supportedQueueFamilies(vk::Instance ini,
		vk::SurfaceKHR surf, vk::PhysicalDevice phdev) {
	VPP_LOAD_PROC(ini, GetPhysicalDeviceSurfaceSupportKHR);
	std::uint32_t count;
	vk::getPhysicalDeviceQueueFamilyProperties(phdev, count, nullptr);

	std::vector<unsigned int> ret;
	ret.reserve(count);
	for(auto i = 0u; i < count; ++i) {
		vk::Bool32 supported;
		VKPP_CHECK(call(pfGetPhysicalDeviceSurfaceSupportKHR, phdev,
			i, surf, &supported));
		if(supported == VK_TRUE) {
			ret.push_back(i);
		}
	}

	return ret;
}

vk::SurfaceCapabilitiesKHR capabilities(vk::Instance ini, vk::SurfaceKHR surface,
		vk::PhysicalDevice phdev) {
	VPP_LOAD_PROC(ini, GetPhysicalDeviceSurfaceCapabilitiesKHR);
	vk::SurfaceCapabilitiesKHR surfCaps;
	VKPP_CHECK(call(pfGetPhysicalDeviceSurfaceCapabilitiesKHR, phdev,
		surface, &surfCaps));
	return surfCaps;
}

std::vector<vk::SurfaceFormatKHR> formats(vk::Instance ini,
	vk::SurfaceKHR surface, vk::PhysicalDevice phdev) {

	VPP_LOAD_PROC(ini, GetPhysicalDeviceSurfaceFormatsKHR);
	uint32_t count;
	VKPP_CHECK(call(pfGetPhysicalDeviceSurfaceFormatsKHR, phdev, surface,
		&count, nullptr));

	std::vector<vk::SurfaceFormatKHR> ret(count);
	VKPP_CHECK(call(pfGetPhysicalDeviceSurfaceFormatsKHR, phdev, surface,
		&count, ret.data()));

	return ret;
}

std::vector<vk::PresentModeKHR> presentModes(vk::Instance ini,
		vk::SurfaceKHR surface, vk::PhysicalDevice phdev) {

	VPP_LOAD_PROC(ini, GetPhysicalDeviceSurfacePresentModesKHR);
	uint32_t count;
	VKPP_CHECK(call(pfGetPhysicalDeviceSurfacePresentModesKHR, phdev, surface,
		&count, nullptr));

	std::vector<vk::PresentModeKHR> ret(count);
	VKPP_CHECK(call(pfGetPhysicalDeviceSurfacePresentModesKHR, phdev, surface,
		&count, ret.data()));

	return ret;
}

bool queueFamilySupported(const Surface& surface, vk::PhysicalDevice phdev,
		unsigned int qFamily) {
	return queueFamilySupported(surface.vkInstance(), surface, phdev, qFamily);
}

std::vector<unsigned int> supportedQueueFamilies(const Surface& surface,
		vk::PhysicalDevice phdev) {
	return supportedQueueFamilies(surface.vkInstance(), surface, phdev);
}

vk::SurfaceCapabilitiesKHR capabilities(const Surface& surface,
		vk::PhysicalDevice phdev) {
	return capabilities(surface.vkInstance(), surface, phdev);
}

std::vector<vk::SurfaceFormatKHR> formats(const Surface& surface,
		vk::PhysicalDevice phdev) {
	return formats(surface.vkInstance(), surface, phdev);
}

std::vector<vk::PresentModeKHR> presentModes(const Surface& surface,
		vk::PhysicalDevice phdev) {
	return presentModes(surface.vkInstance(), surface, phdev);
}

} // namespace vpp
