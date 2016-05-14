#pragma once

#include <vpp/vk.hpp>
#include <vpp/fwd.hpp>
#include <vpp/utility/nonCopyable.hpp>

#include <memory>

namespace vpp
{

///Vulkan Device.
///When a DeviceLost vulkan error occures, the program can try to create a new Device object for the
///valid.
///same PhysicalDevice, if this fails again with the DeviceLost, the physical device is not longer
class Device : public NonMoveable
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

	std::unique_ptr<CommandBufferProvider> cbProvider_;
	std::unique_ptr<DeviceMemoryProvider> dmProvider_;
	std::unique_ptr<SubmitManager> submitManager_;

public:
	Device();
    Device(vk::Instance ini, vk::PhysicalDevice phdev, const vk::DeviceCreateInfo& info);
    ~Device();

    VkInstance vkInstance() const { return instance_; }
    VkPhysicalDevice vkPhysicalDevice() const { return physicalDevice_; }
    VkDevice vkDevice() const { return device_; }

	///Waits until all operations on this device are finished.
    void waitIdle() const;

	///Returns all available queues for the created device.
	const std::vector<Queue>& queues() const { return queues_; }
	const Queue* queue(std::uint32_t family) const;
	const Queue* queue(std::uint32_t family, std::uint32_t id) const;

	const vk::PhysicalDeviceMemoryProperties& memoryProperties() const { return memoryProperties_; }
	const vk::PhysicalDeviceProperties& properties() const { return physicalDeviceProperties_; }

	///Returns the first memoryType for the given memoryTypeBits and flags.
	int memoryType(vk::MemoryPropertyFlags mflags, std::uint32_t typeBits = ~0u) const;

	///Returns a bitmask of memoryTypes that match the given parameters.
	std::uint32_t memoryTypeBits(vk::MemoryPropertyFlags mflags, std::uint32_t typeBits = ~0u) const;

	///Returns a CommandBufferProvider that can be used to easily allocate a command buffer in the
	///current thread.
	CommandBufferProvider& commandBufferProvider() const;

	///Makes sure that all queues setup commandBuffers have been executed.
	void finishSetup() const;

	///Returns a DeviceMemoryProvider that can be used to easily allocate vulkan device memory in the
	///current thread.
	DeviceMemoryProvider& deviceMemoryProvider() const;

	///Returns a deviceMemory allocator for the current thread.
	DeviceMemoryAllocator& deviceMemoryAllocator() const;

	///Returns the submit manager for this device.
	SubmitManager& submitManager() const;
};

}
