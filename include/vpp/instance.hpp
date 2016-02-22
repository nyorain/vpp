#pragma once

#include <vpp/vk.hpp>
#include <vpp/fwd.hpp>

namespace vpp
{

///Instance.
class Instance
{
protected:
	vk::Instance instance_ {};

public:
	Instance(const vk::InstanceCreateInfo& createInfo);
	~Instance();

	vk::Instance vkInstance() const { return instance_; }
	std::vector<vk::PhysicalDevice> enumeratePhysicalDevices() const;
};

}
