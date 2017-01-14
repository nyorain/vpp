// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vpp/fwd.hpp>
#include <vpp/util/nonCopyable.hpp> // nytl::NonMovable
#include <vpp/util/span.hpp> // nytl::Span
#include <vpp/util/threadlocal.hpp> // vpp::ThreadLocalStorage

#include <memory> // std::unique_ptr
#include <vector> // std::vector
#include <any> // std::any

namespace vpp {

//TODO: creation abstraction to easier create queues (just pass reqs, needed queues will be queryed)

/// Represents a Vulkan Device.
/// The centreal objects that all resources are created from.
/// Holds important device-related resources such as queried properties and queues, as
/// well as thread local storage with thread-specific managers and providers.
/// Allows applications and vpp classes to associate thread-specific variables with
/// the device (it can be used for pseudo-threadlocal storage).
/// When a DeviceLost vulkan error occures, the program can try to create a new Device object
/// for the same PhysicalDevice, if this fails again with DeviceLost, then the physical device
/// is not longer valid.
/// Notice that Device is one of the few classes that are NOT movable since it is referenced by
/// all resources.
/// Creating multiple Device objects for the same vk::Device will reult in undefined behaviour.
class Device : public NonMovable {
public:
	/// Creates a new vulkan device from the given device create info.
	/// If anything goes wrong while creatin the vulkan device this function
	/// will throw a std::excpetion.
	Device(vk::Instance ini, vk::PhysicalDevice phdev, const vk::DeviceCreateInfo& info);

	/// Creates the object from the given device and stores the given queues.
	/// Note that this function call transfers ownership of the given vulkan device to the
	/// created vpp::Device object i.e. it will be destroyed in the Device objects desctructor.
	/// This function is especially useful with the NonOwned template to create a Device object
	/// from an elsewhere created vulkan device.
	/// \param queues A vector holding all queues that should be retrieved.
	/// For every entry in the given vector, the first member of the pair represents the queue
	/// family and the second member the queue id. Note that there is no way in vulkan to check
	/// for created queues of a device so this information must be valid.
	/// \sa NonOwned
	Device(vk::Instance ini, vk::PhysicalDevice phdev, vk::Device device,
		nytl::Span<const std::pair<unsigned int, unsigned int>> queues);

	/// Transfers ownership of the given vk::Device to this object.
	/// \param queues A span of queues this Device should manage. The second member of
	/// the pair of each range value holds the queue family of the given queue.
	/// Note that there is no possibility in vulkan to query device queues, so this
	/// class will only be able to operate on the given queues.
	/// Passing invalid queues or queue families results in undefined behaviour.
	Device(vk::Instance ini, vk::PhysicalDevice phdev, vk::Device device,
		nytl::Span<const std::pair<vk::Queue, unsigned int>> queues);

	/// Destructs the owned vk::Device.
	/// This should only be called if there are no other resources left that were created
	/// from this device.
	~Device();

	/// Returns all available queues for the created device.
	/// Note that if the Device was created from an external device (and therefore given
	/// information about the queues) this function call will only return the queues given to
	/// the constructor.
	/// The returned span and the references queues are guaranteed to be valid until the
	/// device object is destructed.
	nytl::Span<const Queue*> queues() const;

	/// Returns a queue for the given family or nullptr if there is none.
	/// Searches all queues return from queues().
	const Queue* queue(std::uint32_t family) const;

	/// Returns the queue for the given family with the given id or nullptr if there is none.
	/// Searches all queues return from queues().
	const Queue* queue(std::uint32_t family, std::uint32_t id) const;

	/// Returns a queue that matches the given flags or nullptr if there is none.
	/// Searches all queues return from queues().
	const Queue* queue(vk::QueueFlags flags) const;

	/// Returns the memoryProperties of the physical device this device was created on.
	/// Guaranteed to be valid until this Device object is destroyed.
	const vk::PhysicalDeviceMemoryProperties& memoryProperties() const;

	/// Returns the properties of the physical device this device was created on.
	/// Guaranteed to be valid until this Device object is destroyed.
	const vk::PhysicalDeviceProperties& properties() const;

	/// Returns the queue properties for the given queue family.
	/// Guaranteed to be valid until this Device object is destroyed.
	const vk::QueueFamilyProperties& queueFamilyProperties(std::uint32_t qFamily) const;

	/// Returns the first memoryType that fulfills the given flags and for which the bit of its
	/// id of typeBits set to 1. TypeBits is defaulted to a value with all bits set (~0u), so
	/// it will by default accept all memoryTypes.
	/// If e.g. typeBits only has the 3rd and 5th least significant bits set, all memory types
	/// other than those with id 3 or 5 will not be considered (no matter if they fulfill the
	/// given flags).
	/// Can be useful to query a memoryType to bind a resource on.
	/// If no matching memory type is found, returns -1.
	int memoryType(vk::MemoryPropertyFlags mflags, std::uint32_t typeBits = ~0u) const;

	/// Returns a bitmask of memoryTypes that match the given parameters, i.e. all memory
	/// types that fulfill the given flags and which bit is set to 1 in the passed typeBits.
	/// If the return value has e.g. the 2nd bit set to 1 this means that memory type
	/// 2 fulfills the given flags and was not sorted out by the passed typeBits.
	/// \sa memoryType
	std::uint32_t memoryTypeBits(vk::MemoryPropertyFlags mflags,
		std::uint32_t typeBits = ~0u) const;

	/// Returns a CommandBufferProvider that can be used to easily allocate command buffers.
	/// The returned CommandProvider will be specific for the calling thread.
	/// \sa CommandProvider
	CommandProvider& commandProvider() const;

	/// Returns the submit manager for this device.
	/// \sa SubmitManager
	SubmitManager& submitManager() const;

	/// Return the default transferManager for this device.
	/// \sa TransferManager
	TransferManager& transferManager() const;

	/// Returns a deviceMemory allocator for this device and the calling thread.
	/// \sa DeviceMemoryAllocator
	DeviceMemoryAllocator& deviceAllocator() const;

	/// Returns the ThreadLocalStorage object that is used for all thread specific state.
	/// Can be used to associate custom thread specific objects with this device.
	ThreadLocalStorage<std::any>& threadLocalStorage() const;

	vk::Instance vkInstance() const { return instance_; }
	vk::PhysicalDevice vkPhysicalDevice() const { return physicalDevice_; }
	vk::Device vkDevice() const { return device_; }
	operator vk::Device() const { return vkDevice(); }

protected:
	struct Impl;
	struct TLStorage; // there are a couple of thread dependent variables stored
	friend class CommandProvider; // must acces threadLocalPools

protected:
	// std::vector<CommandPool>& tlCommandPools() const;
	// TLStorage& tlStorage() const;
	void release();

protected:
	vk::Instance instance_ {};
	vk::PhysicalDevice physicalDevice_ {};
	vk::Device device_ {};

	/// Device uses the pimpl idion since it holds internally many (partly thread-speciic) object
	/// that would pull a lot of huge headers or simply more an implementation detail.
	std::unique_ptr<Impl> impl_ {};
};

}
