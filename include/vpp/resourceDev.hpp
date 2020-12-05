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
	~Resource() noexcept = default;

	Resource(Resource&& other) noexcept;
	Resource& operator=(Resource&& other) noexcept;

	const Device& device() const noexcept { return *device_; }
	vk::Instance vkInstance() const noexcept { return device().vkInstance(); }
	vk::Device vkDevice() const noexcept { return device().vkDevice(); }
	vk::PhysicalDevice vkPhysicalDevice() const noexcept {
		return device().vkPhysicalDevice();
	}

protected:
	Resource() noexcept = default;
	Resource(const Device& device) noexcept : device_(&device) {}

	void init(const Device& dev) noexcept { device_ = &dev; }
	friend VPP_API inline void swap(Resource& a, Resource& b) noexcept {
		std::swap(a.device_, b.device_);
	}

private:
	const Device* device_ {};
};

} // namespace vpp

