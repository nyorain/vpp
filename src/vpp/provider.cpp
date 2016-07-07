#include <vpp/provider.hpp>

namespace vpp
{

//DMProvider
DeviceMemoryProvider::DeviceMemoryProvider(const Device& dev) : Resource(dev)
{
}

DeviceMemoryAllocator& DeviceMemoryProvider::get()
{
	std::lock_guard<std::mutex> guard(mutex_);

	if(allocatorMap_.find(std::this_thread::get_id()) == allocatorMap_.end())
		allocatorMap_[std::this_thread::get_id()] = DeviceMemoryAllocator(device());

	return allocatorMap_[std::this_thread::get_id()];
}

//CBProvider
CommandProvider::CommandProvider(const Device& dev) : Resource(dev)
{
}

CommandBuffer CommandProvider::get(std::uint32_t family,
 	vk::CommandPoolCreateFlags flags, vk::CommandBufferLevel lvl)
{
	std::lock_guard<std::mutex> guard(mutex_);
	auto& pools = commandPools_[std::this_thread::get_id()];
	for(auto& pool : pools)
	{
		if(pool.queueFamily() == family && pool.flags() == flags)
		{
			return pool.allocate(lvl);
		}
	}

	 pools.emplace_back(device(), family, flags);
	 return pools.back().allocate(lvl);
}


std::vector<CommandBuffer> CommandProvider::get(std::uint32_t family, unsigned int count,
	vk::CommandPoolCreateFlags flags, vk::CommandBufferLevel lvl)
{
	std::lock_guard<std::mutex> guard(mutex_);
	auto& pools = commandPools_[std::this_thread::get_id()];
	for(auto& pool : pools)
	{
		if(pool.queueFamily() == family && pool.flags() == flags)
		{
			return pool.allocate(count, lvl);
		}
	}

	 pools.emplace_back(device(), family, flags);
	 return pools.back().allocate(count, lvl);
}

//HMProvider
std::pmr::memory_resource& HostMemoryProvider::get()
{
	std::lock_guard<std::mutex> guard(mutex_);
	// if(resources_.find(std::this_thread::get_id()) == resources_.end())
	// 	resources_[std::this_thread::get_id()] = {};

	return resources_[std::this_thread::get_id()];
}

}
