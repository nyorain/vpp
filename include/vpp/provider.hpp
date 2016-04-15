#pragma once

#include <vpp/vk.hpp>
#include <vpp/memory.hpp>
#include <vpp/commandBuffer.hpp>

#include <vector>
#include <string>
#include <memory>
#include <mutex>
#include <thread>

namespace vpp
{

///Able to provide DeviceMemory for the calling thread.
class DeviceMemoryProvider : public ResourceReference<DeviceMemoryProvider>
{
protected:
	std::map<std::thread::id, DeviceMemoryAllocator> allocatorMap_;
	std::mutex mutex_;

public:
	DeviceMemoryProvider(Device& dev);
	~DeviceMemoryProvider() = default;

	///Returns a DeviceMemoryAllocator that can be used for allocating memory in the calling thread.
	///Sett DeviceMemoryAllocator for more information.
	DeviceMemoryAllocator& get();

	///Allocates all requested resources from the current thread.
	void allocate();
};

//Able to efficiently provide commandBuffer of all types for all threads by holding an internal
//pool of commandPools.
class CommandBufferProvider : public ResourceReference<CommandBufferProvider>
{
protected:
	std::map<std::thread::id, std::vector<CommandPool>> commandPools_;
	std::mutex mutex_;

public:
	CommandBufferProvider(Device& dev);
	~CommandBufferProvider() = default;

	///Allocates a commandBuffer for the calling thread that matches the given requirements.
	CommandBuffer get(vk::CommandPoolCreateFlags flags, std::uint32_t family,
		vk::CommandBufferLevel lvl = vk::CommandBufferLevel::Primary);

	//Allocates multiple commandBuffers for the calling thread with the given requirements.
	///Might be more efficient than multiple calls to get.
	std::vector<CommandBuffer> getMultiple(vk::CommandPoolCreateFlags flags, std::uint32_t family,
		vk::CommandBufferLevel lvl = vk::CommandBufferLevel::Primary, unsigned int count = 1);
};

}
