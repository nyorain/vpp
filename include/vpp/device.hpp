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
class Device : public NonMovable
{
public:
	Device();
	Device(vk::Instance ini, vk::PhysicalDevice phdev, const vk::DeviceCreateInfo& info);
	~Device();

	///Waits until all operations on this device are finished.
	void waitIdle() const;

	///Returns all available queues for the created device.
	Range<std::unique_ptr<Queue>> queues() const;

	///Returns a queue for the given family or nullptr if there is none.
	const Queue* queue(std::uint32_t family) const;

	///Returns the queue for the given family with the given id or nullptr if there is none.
	const Queue* queue(std::uint32_t family, std::uint32_t id) const;

	///Returns a queue that matches the given flags or nullptr if there is none.
	const Queue* queue(vk::QueueFlags flags) const;

	const vk::PhysicalDeviceMemoryProperties& memoryProperties() const;
	const vk::PhysicalDeviceProperties& properties() const;

	///Returns the queue properties for the given queue family.
	const vk::QueueFamilyProperties& queueFamilyProperties(std::uint32_t qFamily) const;

	///Returns the first memoryType for the given memoryTypeBits and flags or -1 if there is none.
	int memoryType(vk::MemoryPropertyFlags mflags, std::uint32_t typeBits = ~0u) const;

	///Returns a bitmask of memoryTypes that match the given parameters.
	std::uint32_t memoryTypeBits(vk::MemoryPropertyFlags mflags,
		std::uint32_t typeBits = ~0u) const;

	///Returns a CommandBufferProvider that can be used to easily allocate a command buffer in the
	///current thread.
	CommandProvider& commandProvider() const;

	///Returns a DeviceMemoryProvider that can be used to easily allocate vulkan device memory in the
	///current thread.
	DeviceMemoryProvider& memoryProvider() const;

	///Returns the host memory provider/ memory pool.
	HostMemoryProvider& hostMemoryProvider() const;

	///Returns the submit manager for this device.
	SubmitManager& submitManager() const;

	///Return the default transferManager for this device.
	TransferManager& transferManager() const;

	///Makes sure that all queued setup commandBuffers have been executed.
	void finishSetup() const;

	///Returns a deviceMemory allocator for the calling thread.
	DeviceMemoryAllocator& deviceAllocator() const;

	///Returns a HostMemoryAllocator for the calling thread.
	std::pmr::memory_resource& hostMemoryResource() const;

	const vk::Instance& vkInstance() const { return instance_; }
	const vk::PhysicalDevice& vkPhysicalDevice() const { return physicalDevice_; }
	const vk::Device& vkDevice() const { return device_; }

	operator vk::Device() const { return vkDevice(); }

protected:
	vk::Instance instance_ {};
	vk::PhysicalDevice physicalDevice_ {};
	vk::Device device_ {};

	struct Impl;
	std::unique_ptr<Impl> impl_;
};

}
