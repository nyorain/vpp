// Copyright (c) 2016-2020 Jan Kelling
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vpp/fwd.hpp>
#include <vpp/resource.hpp>
#include <vpp/handles.hpp>
#include <vpp/util/nonCopyable.hpp>
#include <vector>

namespace vpp {
namespace fwd { extern const vk::CommandBufferLevel primaryCmdBufLevel; }

// Owns a pool of commandPools that allow to allocate commandBuffers of all
// kind. Not synchronized in any way, must always be used from only
// one thread.
class CommandAllocator : public Resource {
public:
	// Command Pool that stores its own flags and queue family.
	class TrCmdPool : public CommandPool {
	public:
		uint32_t queueFamily {};
		vk::CommandPoolCreateFlags flags {};

	public:
		TrCmdPool() = default;
		TrCmdPool(const Device&, uint32_t qfamily, vk::CommandPoolCreateFlags);
		~TrCmdPool() = default;

		TrCmdPool(TrCmdPool&&) noexcept = default;
		TrCmdPool& operator=(TrCmdPool&&) noexcept = default;
	};

public:
	CommandAllocator(const Device& dev);
	~CommandAllocator() = default;

	// Allocates a commandBuffer for the calling thread that matches
	// the given requirements.
	CommandBuffer get(uint32_t qfamily, vk::CommandPoolCreateFlags = {},
		vk::CommandBufferLevel = fwd::primaryCmdBufLevel);

	// Allocates multiple commandBuffers for the calling thread with
	// the given requirements.
	// Might be more efficient than multiple calls to get.
	std::vector<CommandBuffer> get(uint32_t qfamily, unsigned int count,
		vk::CommandPoolCreateFlags = {},
		vk::CommandBufferLevel = fwd::primaryCmdBufLevel);

	const auto& pools() const { return pools_; }

protected:
	std::vector<TrCmdPool> pools_;
};

} // namespace vpp
