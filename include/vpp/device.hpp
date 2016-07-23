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

	///Returns a CommandBufferProvider that can be used to easily allocate command buffers.
	///\sa CommandProvider
	CommandProvider& commandProvider() const;

	///Returns the submit manager for this device.
	///\sa SubmitManager
	SubmitManager& submitManager() const;

	///Return the default transferManager for this device.
	///\sa TransferManager
	TransferManager& transferManager() const;

	///Returns a deviceMemory allocator for the calling thread.
	///\sa DeviceMemoryAllocator
	DeviceMemoryAllocator& deviceAllocator() const;

	///Returns a HostMemoryAllocator for the calling thread.
	std::pmr::memory_resource& hostMemoryResource() const;

	//TODO: see doc/old/allocationCallbacks.cpp
	//A bit problematic to use a memory_resource since it requires the size when freeing
	//but vulkan does not provide it.
	
	///Returns an allocationCallbacks object that can be used for more efficient host allocations
	///inside the vulkan api. Usually this will not have a real performance impact.
	///Note that the returned AllocationCallbacks structure should only be used in the calling
	///thread.
	// const vk::AllocationCallbacks& allocationCallbacks() const;

	const vk::Instance& vkInstance() const { return instance_; }
	const vk::PhysicalDevice& vkPhysicalDevice() const { return physicalDevice_; }
	const vk::Device& vkDevice() const { return device_; }

	operator vk::Device() const { return vkDevice(); }

protected:
	struct Impl;
	struct TLStorage; //there are a couple of thread dependent variables stored
	friend class CommandProvider; //must acces threadLocalPools

protected:
	std::vector<CommandPool>& tlCommandPools() const;
	TLStorage& tlStorage() const;

protected:
	vk::Instance instance_ {};
	vk::PhysicalDevice physicalDevice_ {};
	vk::Device device_ {};

	std::unique_ptr<Impl> impl_;
};

}
