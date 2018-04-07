// Copyright (c) 2016-2018 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vpp/fwd.hpp>
#include <vpp/util/nonCopyable.hpp> // nytl::NonMovable
#include <vpp/util/span.hpp> // nytl::Span

#include <memory> // std::unique_ptr
#include <cstdint> // std::uint32_t
#include <shared_mutex> // std::shared_mutex

namespace vpp {

// Taken from vpp/util/threadStorage.hpp
using DynamicStoragePtr = std::unique_ptr<DynamicStorageBase>;
using DynamicThreadStorage = ThreadStorage<DynamicStoragePtr>;

/// Represents a Vulkan Device.
/// The central objects that all resources are created from.
/// Holds important device-related resources such as queried properties and queues, as
/// well as thread local storage with thread-specific managers and providers.
/// Allows applications and vpp classes to associate thread-specific variables with
/// the device (it can be used for pseudo-threadlocal storage).
/// When a DeviceLost vulkan error occurs, the program can try to create a new Device object
/// for the same PhysicalDevice, if this fails again with DeviceLost, then the physical device
/// is not longer valid.
/// Notice that Device is one of the few classes that are not movable since it is referenced by
/// all resources.
class Device : public nytl::NonMovable {
public:
	/// Creates a new vulkan device from the given device create info.
	/// Might (as all vpp classes) throw a vk::VulkanError from the underlaying api calls.
	Device(vk::Instance, vk::PhysicalDevice, const vk::DeviceCreateInfo&);

	/// Creates a new vulkan physical device.
	/// If no physical device is given, will try to choose the best from the available ones.
	/// Might (as all vpp classes) throw a vk::VulkanError from the underlaying api calls.
	/// Will throw std::runtime_error if valid physical device can be found.
	/// Will automatically create a compute and graphics queue (if possible).
	/// \param extensions The extensions to be enabled.
	Device(vk::Instance, vk::PhysicalDevice = {},
		nytl::Span<const char* const> extensions = {});

	/// Creates a new vulkan device that can be used to present on the given surface.
	/// Will try to automatically select the best physical device. The present queue
	/// parameter will be set to the queue that can be used to present on the given
	/// surface. Will automatically also create one graphics and compute queue
	/// which can be retrieved using the queue() functions.
	/// Might (as all vpp classes) throw a vk::VulkanError from the underlaying api calls.
	/// Will throw std::runtime_error if valid physical device can be found.
	/// \param extensions The extensions to be enabled. Will automatically
	/// activate the swapchain extension.
	Device(vk::Instance, vk::SurfaceKHR, const Queue*& present,
		nytl::Span<const char* const> extensions = {});

	/// Creates the object from the given device and stores the given queues.
	/// Note that this function call transfers ownership of the given vulkan device to the
	/// created vpp::Device object i.e. it will be destroyed in the Device objects desctructor.
	/// This function is e.g. useful with the NonOwned template to create a Device object
	/// from an elsewhere created vulkan device.
	/// \param queues A vector holding all queues that should be retrieved.
	/// For every entry in the given vector, the first member of the pair represents the queue
	/// family and the second member the queue id. Note that there is no way in vulkan to check
	/// for created queues of a device so this information must be valid.
	/// The device is guaranteed to only use the passed queues.
	/// \sa NonOwned
	Device(vk::Instance ini, vk::PhysicalDevice phdev, vk::Device device,
		nytl::Span<const std::pair<unsigned int, unsigned int>> queues);

	/// Creates the object from the given device and stores the given queues.
	/// Note that this function call transfers ownership of the given vulkan device to the
	/// created vpp::Device object i.e. it will be destroyed in the Device objects desctructor.
	/// This function is e.g. useful with the NonOwned template to create a Device object
	/// from an elsewhere created vulkan device.
	/// \param queues A span of queues this Device should manage. The second member of
	/// the pair of each range value holds the queue family of the given queue.
	/// Note that there is no possibility in vulkan to query device queues, so this
	/// class will only be able to operate on the given queues.
	/// Passing invalid queues or queue families results in undefined behaviour.
	/// \sa NonOwned
	Device(vk::Instance ini, vk::PhysicalDevice phdev, vk::Device device,
		nytl::Span<const std::pair<vk::Queue, unsigned int>> queues);

	/// Destructs the owned vk::Device.
	/// This should only be called if there are no other resources left that were created
	/// from this device.
	/// Will also destruct all thread specific resources that were created for this device.
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
	const Queue* queue(unsigned int family) const;

	/// Returns the queue for the given family with the given id or nullptr if there is none.
	/// Searches all queues return from queues().
	const Queue* queue(unsigned int family, unsigned int id) const;

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
	const vk::QueueFamilyProperties& queueFamilyProperties(unsigned int qFamily) const;

	/// Returns the first memoryType that fulfills the given flags and for which the bit of its
	/// id of typeBits set to 1. TypeBits is defaulted to a value with all bits set (~0u), so
	/// it will by default accept all memoryTypes.
	/// If e.g. typeBits only has the 3rd and 5th least significant bits set, all memory types
	/// other than those with id 3 or 5 will not be considered (no matter if they fulfill the
	/// given flags).
	/// Can be useful to query a memoryType to bind a resource on.
	/// If no matching memory type is found, returns -1.
	int memoryType(vk::MemoryPropertyFlags mflags, unsigned int typeBits = ~0u) const;

	/// Returns a bitmask of memoryTypes that match the given parameters, i.e. all memory
	/// types that fulfill the given flags and which bit is set to 1 in the passed typeBits.
	/// If the return value has e.g. the 2nd bit set to 1 this means that memory type
	/// 2 fulfills the given flags and was not sorted out by the passed typeBits.
	/// \sa memoryType
	unsigned int memoryTypeBits(vk::MemoryPropertyFlags mflags,
		unsigned int typeBits = ~0u) const;

	// hostVisible/deviceLocal memory types as bitmask
	unsigned int hostMemoryTypes(unsigned int typeBits = ~0u) const;
	unsigned int deviceMemoryTypes(unsigned int typeBits = ~0u) const;

	/// Thread-specific resources. See their respectives classes.
	CommandAllocator& commandAllocator() const;
	BufferAllocator& bufferAllocator() const;
	DeviceMemoryAllocator& deviceAllocator() const;
	DescriptorAllocator& descriptorAllocator() const;
	QueueSubmitter& queueSubmitter() const;

	/// Returns the ThreadStorage object that is used for all thread specific state.
	/// Can be used to associate custom thread specific objects with this device.
	/// \sa ThreadStorage
	DynamicThreadStorage& threadStorage() const;

	/// Returns the shared mutex for all queues.
	/// Should not be used directly, but rather using a QueueLock object.
	/// Used for queue operation synchronization.
	/// \sa QueueManager
	std::shared_mutex& sharedQueueMutex() const;

	vk::Instance vkInstance() const { return instance_; }
	vk::PhysicalDevice vkPhysicalDevice() const { return physicalDevice_; }
	vk::Device vkDevice() const { return device_; }
	vk::Device vkHandle() const { return device_; }
	operator vk::Device() const { return device_; }

protected:
	struct Impl;
	struct Provider;
	struct QueueDeleter;

protected:
	void release();
	void init(nytl::Span<const std::pair<vk::Queue, unsigned int>> queues);

protected:
	vk::Instance instance_ {};
	vk::PhysicalDevice physicalDevice_ {};
	vk::Device device_ {};

	// Device uses the pimpl idion since it holds internally many (partly
	// thread-speciic) object that would pull a lot of huge headers or
	// are simply more an implementation detail.
	std::unique_ptr<Impl> impl_;
};

} // namespace vpp
