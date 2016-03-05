#pragma once

#include <vpp/vk.hpp>
#include <vpp/fwd.hpp>

namespace vpp
{

///Device.
class Device
{
public:
	struct Queue
	{
		vk::Queue queue;
		vk::QueueFamilyProperties properties;
		unsigned int family;
		unsigned int id;
	};

protected:
    vk::Instance instance_ {};
    vk::PhysicalDevice physicalDevice_ {};
    vk::Device device_ {};

	std::vector<Queue> queues_;

	vk::PhysicalDeviceMemoryProperties memoryProperties_ {};
	vk::PhysicalDeviceProperties physicalDeviceProperties_ {};

public:
    Device(vk::Instance ini, vk::PhysicalDevice phdev, const vk::DeviceCreateInfo& info);
    virtual ~Device();

    VkInstance vkInstance() const { return instance_; }
    VkPhysicalDevice vkPhysicalDevice() const { return physicalDevice_; }
    VkDevice vkDevice() const { return device_; }

	///Signals the device that a device lost vulkan error ocurred and it should try to fix it.
	void deviceLost();
    void waitIdle() const;

	const std::vector<Queue>& queues() const { return queues_; }
	vk::Queue queue(std::uint32_t family) const;
	vk::Queue queue(std::uint32_t family, std::uint32_t id) const;

	const vk::PhysicalDeviceMemoryProperties& memoryProperties() const { return memoryProperties_; }
	const vk::PhysicalDeviceProperties& properties() const { return physicalDeviceProperties_; }

	///Returns the first memoryType for the given memoryTypeBits and flags.
	int memoryType(std::uint32_t typeBits, vk::MemoryPropertyFlags mflags) const;

	///Returns a bitmask of memoryTypes that match the given parameters.
	std::uint32_t memoryTypeBits(std::uint32_t typeBits, vk::MemoryPropertyFlags mflags) const;
};

}
