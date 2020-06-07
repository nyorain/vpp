// Copyright (c) 2016-2020 Jan Kelling
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#include <vpp/commandAllocator.hpp>
#include <vpp/vk.hpp>

namespace vpp {
namespace fwd {
	const vk::CommandBufferLevel primaryCmdBufLevel =
		vk::CommandBufferLevel::primary;
} // namespace fwd

// CommandAllocator
CommandAllocator::CommandAllocator(const Device& dev) : Resource(dev) {
}

// important to use '==' for flags instead of '&' since flags being
// not present is of equal importance
CommandBuffer CommandAllocator::get(uint32_t family,
	 vk::CommandPoolCreateFlags flags, vk::CommandBufferLevel lvl) {
	for(auto& pool : pools_) {
		if(pool.queueFamily == family && pool.flags == flags) {
			return pool.allocate(lvl);
		}
	}

	 pools_.emplace_back(device(), family, flags);
	 return pools_.back().allocate(lvl);
}

std::vector<CommandBuffer> CommandAllocator::get(uint32_t family,
		unsigned int count, vk::CommandPoolCreateFlags flags,
		vk::CommandBufferLevel lvl) {
	for(auto& pool : pools_) {
		if(pool.queueFamily == family && pool.flags == flags) {
			return pool.allocate(count, lvl);
		}
	}

	 pools_.emplace_back(device(), family, flags);
	 return pools_.back().allocate(count, lvl);
}

CommandAllocator::TrCmdPool::TrCmdPool(const Device& dev, uint32_t qfamily,
	vk::CommandPoolCreateFlags xflags) :
		CommandPool(dev, qfamily, xflags), queueFamily(qfamily), flags(xflags) {
}

} // namespace vpp
