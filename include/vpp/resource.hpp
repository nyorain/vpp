#pragma once

#include <vpp/fwd.hpp>
#include <vpp/device.hpp>

namespace vpp
{

///Resource
class Resource
{
protected:
	const Device* device_{nullptr};

protected:
	Resource() = default;
	void create(const Device& device) { device_ = &device; };

public:
	Resource(const Device& device) : device_(&device) {}
	~Resource() = default;

	const Device& device() const { return *device_; }

	VkInstance vkInstance() const { return device().vkInstance(); }
	VkPhysicalDevice vkPhysicalDevice() const { return device().vkPhysicalDevice(); }
	VkDevice vkDevice() const { return device().vkDevice(); }
};


}
