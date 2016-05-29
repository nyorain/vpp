#pragma once

#include <vpp/fwd.hpp>
#include <vpp/utility/nonCopyable.hpp>

#include <memory>
#include <vector>

namespace vpp
{

//TODO: creation abstraction to easier create queues (just pass reqs, needed queues will be queryed)
///Vulkan Device.
///When a DeviceLost vulkan error occures, the program can try to create a new Device object for the
///same PhysicalDevice, if this fails again with the DeviceLost, the physical device is not longer
///valid.
///Notice that Device is one of the few classes that are NOT movable since it is references by
///all resources.
class Device : public NonMoveable
{
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
	const std::vector<Queue>& queues() const;

	///Returns a queue for the given family or nullptr if there is none.
	const Queue* queue(std::uint32_t family) const;

	///Returns the queue for the given family with the given id or nullptr if there is none.
	const Queue* queue(std::uint32_t family, std::uint32_t id) const;

	///Returns a queue that matches the given flags or nullptr if there is none.
	const Queue* queue(vk::QueueFlags flags) const;

	const vk::PhysicalDeviceMemoryProperties& memoryProperties() const;
	const vk::PhysicalDeviceProperties& properties() const;

	///Returns the first memoryType for the given memoryTypeBits and flags or -1 if there is none.
	int memoryType(vk::MemoryPropertyFlags mflags, std::uint32_t typeBits = ~0u) const;

	///Returns a bitmask of memoryTypes that match the given parameters.
	std::uint32_t memoryTypeBits(vk::MemoryPropertyFlags mflags, std::uint32_t typeBits = ~0u) const;

	///Returns a CommandBufferProvider that can be used to easily allocate a command buffer in the
	///current thread.
	CommandProvider& commandProvider() const;

	///Returns a DeviceMemoryProvider that can be used to easily allocate vulkan device memory in the
	///current thread.
	DeviceMemoryProvider& memoryProvider() const;

	///Returns the submit manager for this device.
	SubmitManager& submitManager() const;

	///Return the default transferManager for this device.
	TransferManager& transferManager() const;

	///Makes sure that all queued setup commandBuffers have been executed.
	void finishSetup() const;

	///Returns a deviceMemory allocator for the calling thread.
	DeviceMemoryAllocator& memoryAllocator() const;

protected:
    vk::Instance instance_ {};
    vk::PhysicalDevice physicalDevice_ {};
    vk::Device device_ {};

	//all retrieved queues for the device
	/*
	std::vector<Queue> queues_;

	//stored props
	vk::PhysicalDeviceMemoryProperties memoryProperties_ {};
	vk::PhysicalDeviceProperties physicalDeviceProperties_ {};

	//default provider and manager
	std::unique_ptr<CommandProvider> commandProvider_;
	std::unique_ptr<DeviceMemoryProvider> memoryProvider_;
	std::unique_ptr<SubmitManager> submitManager_;
	std::unique_ptr<TransferManager> transferManager_;
	*/

	struct Impl;
	std::unique_ptr<Impl> impl_;
};

}
