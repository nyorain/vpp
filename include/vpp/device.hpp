// Copyright (c) 2016-2020 Jan Kelling
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vpp/fwd.hpp>
#include <vpp/util/span.hpp> // nytl::Span

#include <memory> // std::unique_ptr
#include <cstdint> // std::uint32_t
#include <shared_mutex> // std::shared_mutex

namespace vpp {

// TODO: since Device is not movable, offer constructor-like 'init' functions.
// Makes initialization of classes easier that create a Device.

/// Represents a Vulkan Device.
/// The central objects that all resources are created from.
/// Holds important device-related resources such as queried properties and
/// queues, as well as default allocators.
/// When a DeviceLost vulkan error occurs, the program can try to create a new
/// Device object for the same PhysicalDevice, if this fails again with
/// DeviceLost, then the physical device is not longer valid.
/// Device is one of the few classes that are not movable since it
/// is referenced by all resources.
class Device {
public:
#ifdef VPP_ONE_DEVICE_OPTIMIZATION
	/// If VPP_ONE_DEVICE_OPTIMIZATION is defined, vpp must only be used
	/// with one Device instance (at a time), which can be retrieved via
	/// this function. Will be automatically set when a Device object is
	/// constructed and unset when it's destructed.
	static Device* instance();
#endif

	/// Whether the Device's instance has the VK_EXT_debug_utils extension.
	/// If this is set to false, the utility functions in vpp/debug.hpp will
	/// have no effect.
	/// This has to be set after device creation if the user cares about it.
	bool hasDebugUtils {false};

public:
	/// Creates a new vulkan device from the given device create info.
	/// Might (as all vpp classes) throw a vk::VulkanError from the underlaying
	/// api calls.
	Device(vk::Instance, vk::PhysicalDevice, const vk::DeviceCreateInfo&);

	/// Creates a new vulkan physical device.
	/// Might (as all vpp classes) throw a vk::VulkanError from the
	/// underlaying api calls. Will automatically create a compute and graphics
	/// queue (if possible).
	/// - phdev: The physical device to use. If not given, will try to choose
	///   the best from the avilable ones. Will throw std::runtime_error if
	///   no valid physical device can be found in that case.
	/// - extensions: The extensions to be enabled.
	Device(vk::Instance, vk::PhysicalDevice = {},
		nytl::Span<const char* const> extensions = {});

	/// Creates a vulkan device that can be used to present on the given surface.
	/// Will try to automatically select the best physical device that
	/// can render on the given surface. Will throw std::runtime_error
	/// if there is no such physical device.
	/// Will automatically also create one graphics and compute queue
	/// which can be retrieved using the queue() functions.
	/// - present: Will be set to the queue that can be used to presenting
	///   on the given surface.
	/// - extensions: The extensions to be enabled. Will automatically
	///   activate the swapchain extension.
	Device(vk::Instance, vk::SurfaceKHR, const Queue*& present,
		nytl::Span<const char* const> extensions = {});

	/// Creates the object from the given device and stores the given queues.
	/// Note that this function call transfers ownership of the given vulkan
	/// device to the created vpp::Device object i.e. it will be destroyed in
	/// the Device objects desctructor. This function is e.g. useful with the
	/// NonOwned template to create a Device object from an elsewhere created
	/// vulkan device.
	/// - queues: A vector holding all queues that should be retrieved.
	///   For every entry in the given vector, the first member of the pair
	///   represents the queue family and the second member the queue id.
	///   Note that there is no way in vulkan to check for created queues of a
	///   device so this information must be valid.
	Device(vk::Instance ini, vk::PhysicalDevice phdev, vk::Device device,
		nytl::Span<const std::pair<unsigned int, unsigned int>> queues);

	/// Creates the object from the given device and stores the given queues.
	/// Note that this function call transfers ownership of the given vulkan
	/// device to the created vpp::Device object i.e. it will be destroyed in
	/// the Device objects desctructor.  This function is e.g. useful with the
	/// NonOwned template to create a Device object from an elsewhere created
	/// vulkan device.
	/// - queues: A span of queues this Device should manage. The second member
	///   of the pair of each range value holds the queue family of the given
	///   queue. Note that there is no possibility in vulkan to query device
	///   queues, so this class will only be able to operate on the given queues.
	Device(vk::Instance ini, vk::PhysicalDevice phdev, vk::Device device,
		nytl::Span<const std::pair<vk::Queue, unsigned int>> queues);

	/// Destructs the owned vk::Device.
	/// This should only be called if there are no other resources left that
	/// were created from this device.
	~Device();

	/// Not movable since most resources reference it.
	Device(Device&&) = delete;
	Device& operator=(Device&&) = delete;

	/// Returns all available queues for the created device.
	/// Note that if the Device was created from an external device (and
	/// therefore given information about the queues) this function call will
	/// only return the queues given to the constructor.
	/// The returned span and the references queues are guaranteed to be valid
	/// until the device object is destructed.
	nytl::Span<const Queue> queues() const;

	/// Returns a queue for the given family or nullptr if there is none.
	/// Searches all queues return from queues().
	const Queue* queue(unsigned int family) const;

	/// Returns the queue for the given family with the given id or nullptr if
	/// there is none. Searches all queues return from queues().
	const Queue* queue(unsigned int family, unsigned int id) const;

	/// Returns a queue that matches the given flags or nullptr if there is none.
	/// Searches all queues return from queues().
	const Queue* queue(vk::QueueFlags flags) const;

	/// Returns the memoryProperties of the physical device.
	/// Guaranteed to be valid until this Device object is destroyed.
	const vk::PhysicalDeviceMemoryProperties& memoryProperties() const;

	/// Returns the properties of the physical device this device was created on.
	/// Guaranteed to be valid until this Device object is destroyed.
	const vk::PhysicalDeviceProperties& properties() const;

	/// Returns the queue properties for the given queue family.
	/// Guaranteed to be valid until this Device object is destroyed.
	const vk::QueueFamilyProperties& queueFamilyProperties(
		unsigned int family) const;

	/// Returns the first memoryType that fulfills the given flags and for which
	/// the bit of its id in typeBits set to 1. TypeBits is defaulted to a value
	/// with all bits set (~0u), so it will by default accept all memoryTypes.
	/// If e.g. typeBits only has the 3rd and 5th least significant bits set,
	/// all memory types other than those with id 3 or 5 will not be considered
	/// (no matter if they fulfill the given flags).
	/// Can be useful to query a memoryType to bind a resource on.
	/// If no matching memory type is found, returns -1.
	int memoryType(vk::MemoryPropertyFlags, unsigned int typeBits = ~0u) const;

	/// Returns a bitmask of memoryTypes that match the given parameters, i.e.
	/// all memory types that fulfill the given flags and which bit is set to 1
	/// in the passed typeBits.
	/// If the return value has e.g. the 2nd bit set to 1 this means that memory
	/// type 2 fulfills the given flags and was not sorted out by the passed
	/// typeBits.
	unsigned int memoryTypeBits(vk::MemoryPropertyFlags mflags,
		unsigned int typeBits = ~0u) const;

	/// hostVisible/deviceLocal memory types as bitmask
	unsigned int hostMemoryTypes(unsigned int typeBits = ~0u) const;
	unsigned int deviceMemoryTypes(unsigned int typeBits = ~0u) const;

	/// Default allocators and managers.
	/// Note that these are not thread specific or synchronized in any way,
	/// so if you work with multiple threads you either need to synchronize
	/// access to those or use multiple allocators (probably the best idea).
	CommandAllocator& commandAllocator() const;
	BufferAllocator& bufferAllocator() const;
	DeviceMemoryAllocator& devMemAllocator() const;
	DescriptorAllocator& descriptorAllocator() const;
	QueueSubmitter& queueSubmitter() const;

	/// Returns the shared mutex for all queues.
	/// Should not be used directly, but rather using a QueueLock object.
	/// Used for queue operation synchronization.
	std::shared_mutex& sharedQueueMutex() const;

	vk::Instance vkInstance() const { return instance_; }
	vk::PhysicalDevice vkPhysicalDevice() const { return phdev_; }
	vk::Device vkDevice() const { return device_; }
	vk::Device vkHandle() const { return device_; }
	operator vk::Device() const { return device_; }

	static vk::ObjectType vkObjectType();

protected:
	struct Impl;
	struct Provider;
	struct QueueDeleter;

protected:
	void release();
	void init(nytl::Span<const std::pair<vk::Queue, unsigned int>> queues);

protected:
	vk::Instance instance_ {};
	vk::PhysicalDevice phdev_ {};
	vk::Device device_ {};

	// Device uses the pimpl idion since it holds internally many object that
	// would pull a lot of headers (that in turn often use Device) or are
	// simply more an implementation detail.
	std::unique_ptr<Impl> impl_;
};

/// Returns a queue family that supports graphics, compute or transfer operations
/// and can therefore be used for transfer operations.
/// Guarantees that there exists at least one queue for the given device with
/// the returned queue family.
/// Returns -1 if there is no such family, although there usually should be.
/// If queue if not nullptr, will store a pointer to a queue of the returned
/// family into it.
int transferQueueFamily(const Device& dev, const Queue** queue);

} // namespace vpp
