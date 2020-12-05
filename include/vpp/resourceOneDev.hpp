// Copyright (c) 2016-2020 Jan Kelling
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vpp/fwd.hpp>
#include <vpp/device.hpp>

namespace vpp {

class VPP_API Resource {
public:
	Resource(const Resource& other) noexcept = default;
	~Resource() = default;

	Resource(Resource&& other) noexcept = default;
	Resource& operator=(Resource&& other) noexcept = default;

	const Device& device() const noexcept { return *Device::instance(); }
	vk::Instance vkInstance() const noexcept { return device().vkInstance(); }
	vk::Device vkDevice() const noexcept { return device().vkDevice(); }
	vk::PhysicalDevice vkPhysicalDevice() const noexcept {
		return device().vkPhysicalDevice();
	}

protected:
	Resource() = default;
	Resource(const Device& dev) { init(dev); }

	void init(const Device& dev);
	friend VPP_API void swap(Resource&, Resource&) noexcept {}
};

} // namespace vpp
