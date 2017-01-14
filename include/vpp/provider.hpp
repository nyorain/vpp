#pragma once

#include <vpp/fwd.hpp>
#include <vpp/allocator.hpp>
#include <vpp/commandBuffer.hpp>
// #include <vpp/util/memory_resource.hpp> //really ugly. Can this be avoided?

#include <vector>
#include <string>
#include <memory>
#include <mutex>
#include <thread>

namespace vpp
{


// ///Able to provide DeviceMemory for the calling thread.
// class DeviceMemoryProvider : public Resource
// {
// public:
// 	DeviceMemoryProvider(const Device& dev);
// 	~DeviceMemoryProvider() = default;
//
// 	///Returns a DeviceMemoryAllocator that can be used for allocating memory in the calling thread.
// 	///Sett DeviceMemoryAllocator for more information.
// 	DeviceMemoryAllocator& get();
//
// protected:
// 	std::map<std::thread::id, DeviceMemoryAllocator> allocatorMap_;
// 	std::mutex mutex_;
// };

// ///Threadsafe host memory pool manager.
// class HostMemoryProvider : public NonCopyable
// {
// public:
// 	std::pmr::memory_resource& get();
//
// protected:
// 	std::map<std::thread::id, std::pmr::unsynchronized_pool_resource> resources_;
// 	std::mutex mutex_;
//
// 	//alternative:
// 	//std::pmr::synchronized_memory_resource resource_;
// };

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
