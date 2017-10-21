// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#include <vpp/commandBuffer.hpp>
#include <vpp/vk.hpp>
#include <vpp/util/threadStorage.hpp>
#include <deque>

namespace vpp {

// CommandPool
CommandPool::CommandPool(const Device& dev, uint32_t qfam, vk::CommandPoolCreateFlags flags)
	: ResourceHandle(dev)
{
	vk::CommandPoolCreateInfo info;
	info.flags = flags;
	info.queueFamilyIndex = qfam;

	handle_ = vk::createCommandPool(device(), info);
}

CommandPool::~CommandPool()
{
	if(vkHandle()) {
		vk::destroyCommandPool(vkDevice(), vkHandle(), nullptr);
	}
}

void swap(CommandPool& a, CommandPool& b) noexcept
{
	using std::swap;
	using RH = ResourceHandle<vk::CommandPool>;

	swap(static_cast<RH&>(a), static_cast<RH&>(b));
	swap(a.flags_, b.flags_);
	swap(a.qFamily_, b.qFamily_);
}

std::vector<CommandBuffer> CommandPool::allocate(size_t count, vk::CommandBufferLevel lvl)
{
	vk::CommandBufferAllocateInfo info;
	info.commandPool = vkHandle();
	info.level = lvl;
	info.commandBufferCount = count;

	std::vector<vk::CommandBuffer> buffers(count);
	vk::allocateCommandBuffers(device(), info, *buffers.data());

	std::vector<CommandBuffer> ret;
	ret.reserve(count);

	for(auto& buf : buffers) {
		ret.emplace_back(*this, buf);
	}

	return ret;
}

CommandBuffer CommandPool::allocate(vk::CommandBufferLevel lvl)
{
	vk::CommandBufferAllocateInfo info;
	info.commandPool = vkHandle();
	info.level = lvl;
	info.commandBufferCount = 1;

	vk::CommandBuffer buffer;
	vk::allocateCommandBuffers(vkDevice(), info, buffer);

	return {*this, buffer};
}

// CommandBuffer
CommandBuffer::CommandBuffer(const CommandPool& pool)
	: CommandBuffer(pool, vk::CommandBufferLevel::primary)
{
}

CommandBuffer::CommandBuffer(const CommandPool& pool, vk::CommandBufferLevel lvl)
	: ResourceHandle(), commandPool_(pool)
{
	vk::allocateCommandBuffers(pool.device(), {pool, lvl, 1u}, handle_);
}

CommandBuffer::CommandBuffer(const CommandPool& pool, vk::CommandBuffer buf)
	: ResourceHandle(pool.device(), buf), commandPool_(pool)
{
}

CommandBuffer::CommandBuffer(const Device& dev, 
	vk::CommandPool pool, vk::CommandBuffer buf)
		: ResourceHandle(dev, buf), commandPool_(pool)
{
}

CommandBuffer::~CommandBuffer()
{
	if(vkHandle()) {
		vk::freeCommandBuffers(vkDevice(), commandPool(), {vkHandle()});
	}
}

void swap(CommandBuffer& a, CommandBuffer& b) noexcept
{
	using std::swap;
	using RH = ResourceHandle<vk::CommandBuffer>;

	swap(static_cast<RH&>(a), static_cast<RH&>(b));
	swap(a.commandPool_, b.commandPool_);
}

// CommandProvider
CommandProvider::CommandProvider(const Device& dev) : Resource(dev)
{
	tlsID_ = dev.threadStorage().add();
}

CommandBuffer CommandProvider::get(uint32_t family,
	 vk::CommandPoolCreateFlags flags, vk::CommandBufferLevel lvl)
{
	using Storage = ValueStorage<std::deque<CommandPool>>;
	auto ptr = device().threadStorage().get(tlsID_);
	if(!ptr->get())
		ptr->reset(new Storage());

	auto& pools = static_cast<Storage*>(ptr->get())->value;
	for(auto& pool : pools) {
		if(pool.queueFamily() == family && pool.flags() == flags) {
			return pool.allocate(lvl);
		}
	}

	 pools.emplace_back(device(), family, flags);
	 return pools.back().allocate(lvl);
}

std::vector<CommandBuffer> CommandProvider::get(uint32_t family, unsigned int count,
	vk::CommandPoolCreateFlags flags, vk::CommandBufferLevel lvl)
{
	using Storage = ValueStorage<std::deque<CommandPool>>;
	auto ptr = device().threadStorage().get(tlsID_);
	if(!ptr->get()) {
		ptr->reset(new Storage());
	}

	auto& pools = static_cast<Storage*>(ptr->get())->value;
	for(auto& pool : pools) {
		if(pool.queueFamily() == family && pool.flags() == flags) {
			return pool.allocate(count, lvl);
		}
	}

	 pools.emplace_back(device(), family, flags);
	 return pools.back().allocate(count, lvl);
}

} // namespace vpp
