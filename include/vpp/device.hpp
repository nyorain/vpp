#pragma once

#include <vpp/vk.hpp>
#include <vpp/fwd.hpp>
#include <vpp/utility/nonCopyable.hpp>

#include <memory>

namespace vpp
{

///Vulkan Device.
///When a DeviceLost vulkan error occures, the program can try to create a new Device object for the
///same PhysicalDevice, if this fails again with the DeviceLost, the physical device is not longer
///valid.
///Notice that Device is one of the few classes that are NOT movable since it is references by
///all resources.
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

public:
	Device();
    Device(vk::Instance ini, vk::PhysicalDevice phdev, const vk::DeviceCreateInfo& info);
    ~Device();

    const VkInstance& vkInstance() const { return instance_; }
    const VkPhysicalDevice& vkPhysicalDevice() const { return physicalDevice_; }
    const VkDevice& vkDevice() const { return device_; }

	///Waits until all operations on this device are finished.
    void waitIdle() const;

	///Returns all available queues for the created device.
	const std::vector<Queue>& queues() const { return queues_; }

	///Returns a queue for the given family or nullptr if there is none.
	const Queue* queue(std::uint32_t family) const;

	///Returns the queue for the given family with the given id or nullptr if there is none.
	const Queue* queue(std::uint32_t family, std::uint32_t id) const;

	///Returns a queue that matches the given flags or nullptr if there is none.
	const Queue* queue(vk::QueueFlags flags) const;

	const vk::PhysicalDeviceMemoryProperties& memoryProperties() const { return memoryProperties_; }
	const vk::PhysicalDeviceProperties& properties() const { return physicalDeviceProperties_; }

	///Returns the first memoryType for the given memoryTypeBits and flags or -1 if there is none.
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
};

}
