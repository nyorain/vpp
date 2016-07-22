#pragma once

#include <vpp/fwd.hpp>
#include <vpp/allocator.hpp>
#include <vpp/commandBuffer.hpp>
#include <vpp/utility/memory_resource.hpp> //really ugly. Can this be avoided?

#include <vector>
#include <string>
#include <memory>
#include <mutex>
#include <thread>

namespace vpp
{

//TODO: improve implementation critical sections (?)

///Able to provide DeviceMemory for the calling thread.
class DeviceMemoryProvider : public Resource
{
public:
	DeviceMemoryProvider(const Device& dev);
	~DeviceMemoryProvider() = default;

	///Returns a DeviceMemoryAllocator that can be used for allocating memory in the calling thread.
	///Sett DeviceMemoryAllocator for more information.
	DeviceMemoryAllocator& get();

protected:
	std::map<std::thread::id, DeviceMemoryAllocator> allocatorMap_;
	std::mutex mutex_;
};

//TODO: allocate for more abstract given requirements
//Able to efficiently provide commandBuffer of all types for all threads by holding an internal
//pool of commandPools.
class CommandProvider : public Resource
{
public:
	CommandProvider(const Device& dev);
	~CommandProvider() = default;

	///Allocates a commandBuffer for the calling thread that matches the given requirements.
	CommandBuffer get(std::uint32_t qfamily, vk::CommandPoolCreateFlags flags = {},
		vk::CommandBufferLevel lvl = vk::CommandBufferLevel::primary);

	///Allocates a commandBuffer that matches the given flags.
	// CommandBuffer get(vk::QueueFlags qflags, vk::CommandPoolCreateFlags flags = {},
		// vk::CommandBufferLevel lvl = vk::CommandBufferLevel::Primary);

	//Allocates multiple commandBuffers for the calling thread with the given requirements.
	///Might be more efficient than multiple calls to get.
	std::vector<CommandBuffer> get(std::uint32_t qfamily, unsigned int count,
		vk::CommandPoolCreateFlags flags = {},
		vk::CommandBufferLevel lvl = vk::CommandBufferLevel::primary);

protected:
	std::map<std::thread::id, std::vector<CommandPool>> commandPools_;
	std::mutex mutex_;
};

///Threadsafe host memory pool manager.
class HostMemoryProvider : public NonCopyable
{
public:
	std::pmr::memory_resource& get();

protected:
	std::map<std::thread::id, std::pmr::synchronized_pool_resource> resources_;
	std::mutex mutex_;

	//alternative:
	//std::pmr::unsynchronized_memory_resource resource_;
};

// ///XXX: concept atm
// class DescriptorPool : public Resource
// {
// public:
//
// };
//
// class DescriptorProvider : public Resource
// {
// public:
// 	DescriptorPool& get();
//
// protected:
// 	std::map<std::thread::id, DescriptorPool> pools_;
// 	std::mutex mutex_;
// };

}
