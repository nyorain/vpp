#pragma once

#include <vpp/fwd.hpp>
#include <vpp/resource.hpp>
#include <thread>

namespace vpp
{

//CommandPool
class CommandPool : public Resource
{
protected:
	vk::CommandPool commandPool_;
	vk::CommandPoolCreateFlags flags_;
	std::uint32_t family_;
};

//CommandBuffer
class CommandBuffer
{
protected:
	vk::CommandBuffer commandBuffer_;
};

//Able to efficiently provide commandBuffer of all types for all threads by holding an internal
//pool of commandPools.
class CommandBufferProvider
{
protected:
	std::map<std::thread::id, std::vector<CommandPool>> commandPools_;

public:
	std::vector<CommandBuffer> get(vk::CommandPoolCreateFlags flags, std::uint32_t family,
		vk::CommandBufferLevel lvl, unsigned int count = 1);
};


}
