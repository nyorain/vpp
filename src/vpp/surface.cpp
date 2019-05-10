// Copyright (c) 2016-2019 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#include <vpp/surface.hpp>
#include <vpp/vk.hpp>

namespace vpp {

Surface::Surface(vk::Instance instance, vk::SurfaceKHR surface)
	: instance_(instance), surface_(surface) {
}

Surface::~Surface() {
	if(vkHandle()) {
		vk::destroySurfaceKHR(vkInstance(), vkHandle(), nullptr);
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
std::vector<unsigned int> supportedQueueFamilies(vk::SurfaceKHR surf,
		vk::PhysicalDevice phdev) {
	std::uint32_t count;
	vk::getPhysicalDeviceQueueFamilyProperties(phdev, count, nullptr);
	std::vector<unsigned int> ret;
	for(auto i = 0u; i < count; ++i) {
		if(vk::getPhysicalDeviceSurfaceSupportKHR(phdev, i, surf)) {
			ret.push_back(i);
		}
	}

	return ret;
}

} // namespace vpp
