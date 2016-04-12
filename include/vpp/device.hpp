#pragma once

#include <vpp/vk.hpp>
#include <vpp/fwd.hpp>

namespace vpp
{

///Can not be copied but moved.
class NonCopyable
{
private:
	NonCopyable(const NonCopyable&) = delete;
	NonCopyable& operator=(const NonCopyable&) = delete;

public:
	NonCopyable() = default;
	~NonCopyable() = default;

	NonCopyable(NonCopyable&& other) noexcept = default;
	NonCopyable& operator=(NonCopyable&& other) noexcept = default;
};

///Vulkan Device
class Device : public NonCopyable
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

	/*
	//concept for associating a device with threadlocal setup stuff provider
	CommandBufferProvider commandBufferProvder_;
	DeviceMemoryProvider deviceMemoryProvider_;

	//holds setup commands that are executed all together to achieve more efficiency
	//XXX: is this really more efficient? better use transient command buffers, so setup commands
	//can be on the device executed in parallel to the host.
	std::map<std::thread::id, vk::CommandBuffer> setupCommandBuffers_;
	*/

public:
	Device() = default;
    Device(vk::Instance ini, vk::PhysicalDevice phdev, const vk::DeviceCreateInfo& info);
    virtual ~Device();

	Device(Device&& other) noexcept;
	Device& operator=(Device&& other) noexcept;

	void swap(Device& other) noexcept;

    VkInstance vkInstance() const { return instance_; }
    VkPhysicalDevice vkPhysicalDevice() const { return physicalDevice_; }
    VkDevice vkDevice() const { return device_; }

	///Signals the device that a device lost vulkan error ocurred and it should try to fix it.
	void deviceLost();

	///Waits until all operations on this device are finished.
    void waitIdle() const;

	///Returns all available queues for the created device.
	const std::vector<Queue>& queues() const { return queues_; }
	const Queue* queue(std::uint32_t family) const;
	const Queue* queue(std::uint32_t family, std::uint32_t id) const;

	const vk::PhysicalDeviceMemoryProperties& memoryProperties() const { return memoryProperties_; }
	const vk::PhysicalDeviceProperties& properties() const { return physicalDeviceProperties_; }

	///Returns the first memoryType for the given memoryTypeBits and flags.
	int memoryType(std::uint32_t typeBits, vk::MemoryPropertyFlags mflags) const;

	///Returns a bitmask of memoryTypes that match the given parameters.
	std::uint32_t memoryTypeBits(std::uint32_t typeBits, vk::MemoryPropertyFlags mflags) const;

/*
	///Returns a command buffer in recording state that can be used to record setup commands.
	///Is internally synchronized, i.e. will return a different and valid command buffer for every
	///thread.
	vk::CommandBuffer setupCommandBuffer() const { return {}; };

	///Executes all setup command buffers for the current thread and wait for them to finish.
	void finishSetup() {  }
*/
};

}
