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
///Creating multiple Device objects for the same vk::Device will reult in undefined behaviour.
class Device : public NonMovable
{
public:
	///Creates a new vulkan device from the given create info.
	Device(vk::Instance ini, vk::PhysicalDevice phdev, const vk::DeviceCreateInfo& info);

	///Creates the object from the given device and stores the given queues.
	///Note that this function call transfers ownership of the given vulkan device to the
	///created vpp::Device object i.e. it will be destroyed in the Device objects desctructor.
	///This function is especially useful with the NonOwned template to create a Device object
	///from an elsewhere created vulkan device.
	///\param queues A vector holding all queues that should be retrieved.
	///For every entry in the given vector, the first member of the pair represents the queue
	///family and the second member the queue id. Note that there is no way in vulkan to check
	///for created queues of a device so this information must be valid.
	///\sa NonOwned
	Device(vk::Instance ini, vk::PhysicalDevice phdev, vk::Device device,
		const Range<std::pair<unsigned int, unsigned int>>& queues);

	///Transfers ownership of the given devic to this object.
	///\param queues A range of queues this Device should manage. The second member of 
	///the pair of each range value holds the queue family of the given queue.
	Device(vk::Instance ini, vk::PhysicalDevice phdev, vk::Device device,
		const Range<std::pair<vk::Queue, unsigned int>>& queues);

	///By default destructs the owned vk::Device.
	~Device();

	///Waits until all operations on this device are finished.
	void waitIdle() const;

	///Returns all available queues for the created device.
	///Note that if the Device was created from an external device (and therefore given
	///information about the queues) this function call will only return the queues given to
	///the constructor.
	Range<std::unique_ptr<Queue>> queues() const;

	///Returns a queue for the given family or nullptr if there is none.
	///Searches all queues return from queues().
	const Queue* queue(std::uint32_t family) const;

	///Returns the queue for the given family with the given id or nullptr if there is none.
	///Searches all queues return from queues().
	const Queue* queue(std::uint32_t family, std::uint32_t id) const;

	///Returns a queue that matches the given flags or nullptr if there is none.
	///Searches all queues return from queues().
	const Queue* queue(vk::QueueFlags flags) const;

	///Returns the memoryProperties of the physical device this device was created on.
	const vk::PhysicalDeviceMemoryProperties& memoryProperties() const;

	///Returns the properties of the physical device this device was created on.
	const vk::PhysicalDeviceProperties& properties() const;

	///Returns the queue properties for the given queue family.
	const vk::QueueFamilyProperties& queueFamilyProperties(std::uint32_t qFamily) const;

	///Returns the first memoryType that fulfills the given flags and for which the bit of its
	///id of type Bits set to 0. TypeBits is defaulted to accept all memoryTypes.
	///Can be e.g. useful to query a memoryType to bind a resource on.
	int memoryType(vk::MemoryPropertyFlags mflags, std::uint32_t typeBits = ~0u) const;

	///Returns a bitmask of memoryTypes that match the given parameters.
	///\sa memoryType
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
	void release();

protected:
	vk::Instance instance_ {};
	vk::PhysicalDevice physicalDevice_ {};
	vk::Device device_ {};

	///Device uses the pimpl idion since it holds internally many objects (some threadlocal) that would
	///pull a lot of unacceptabl headers.
	std::unique_ptr<Impl> impl_;
};

}
