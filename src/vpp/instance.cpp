#include <vpp/instance.hpp>
#include <vpp/call.hpp>

namespace vpp
{

//non-static
Instance::Instance(const vk::InstanceCreateInfo& createInfo)
{
	VPP_CALL(vk::createInstance(createInfo, nullptr, &instance_));
}

Instance::~Instance()
{
	if(vkInstance()) vk::destroyInstance(instance_, nullptr);
}

std::vector<vk::PhysicalDevice> Instance::enumeratePhysicalDevices() const
{
	std::vector<VkPhysicalDevice> phdevs{};
    VPP_CALL(vk::enumeratePhysicalDevices(vkInstance(), phdevs));
    return phdevs;
}

}
