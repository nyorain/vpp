// Copyright (c) 2016-2018 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vpp/fwd.hpp>
#include <vpp/resource.hpp>
#include <vpp/util/nonCopyable.hpp>

#include <vector> // std::vector
#include <functional> // std::function

namespace vpp {

extern const vk::CommandBufferLevel primaryCmdBufLevel;

/// RAII vulkan CommandPool wrapper.
/// CommanPools belong to the thread in which they were created. Theyself as well as
/// CommandBuffer they allocated must only be used in this thread.
/// The CommandPool object must be destroyed in the thread that created it.
/// See the vulkan spec about command pool synchronization for more information.
/// Cannot be moved since the allocated CommandBuffers reference their
/// CommandPool.
class CommandPool : public ResourceHandle<vk::CommandPool>, public nytl::NonMovable {
public:
	CommandPool() = default;
	CommandPool(const Device& dev, uint32_t qfam, vk::CommandPoolCreateFlags flags = {});
	~CommandPool();

	CommandPool(CommandPool&& rhs) noexcept { swap(*this, rhs); }
	CommandPool& operator=(CommandPool rhs) noexcept { 
		swap(*this, rhs); 
		return *this; 
	}

	CommandBuffer allocate(vk::CommandBufferLevel lvl = primaryCmdBufLevel);
	std::vector<CommandBuffer> allocate(size_t count,
		vk::CommandBufferLevel = primaryCmdBufLevel);

	uint32_t queueFamily() const { return qFamily_; }
	vk::CommandPoolCreateFlags flags() const { return flags_; }

	friend void swap(CommandPool& a, CommandPool& b) noexcept;

protected:
	vk::CommandPoolCreateFlags flags_ {};
	uint32_t qFamily_ {};
};

// TODO: make resource reference to command pool?
/// RAII vulkan CommandBuffer wrapper.
/// Note that the CommandBuffer must not be in use by any device when it is destructed.
/// Keeps a reference to the CommandPool it was allocated from.
class CommandBuffer : public ResourceHandle<vk::CommandBuffer> {
public:
	CommandBuffer() = default;
	CommandBuffer(const CommandPool&); // primary level
	CommandBuffer(const CommandPool&, vk::CommandBufferLevel);
	CommandBuffer(const CommandPool&, vk::CommandBuffer);
	CommandBuffer(const Device&, vk::CommandPool, vk::CommandBuffer);
	~CommandBuffer();

	CommandBuffer(CommandBuffer&& rhs) noexcept { swap(*this, rhs); }
	CommandBuffer& operator=(CommandBuffer rhs) noexcept { 
		swap(*this, rhs); 
		return *this; 
	}

	const vk::CommandPool& commandPool() const { return commandPool_; }
	friend void swap(CommandBuffer& a, CommandBuffer& b) noexcept;

protected:
	vk::CommandPool commandPool_;
};

/// Owns a pool of commandPools that allow to allocate commandBuffers of all kind.
/// Not synchronized in any way.
class CommandAllocator : public Resource {
public:
	CommandAllocator(const Device& dev);
	~CommandAllocator() = default;

	/// Allocates a commandBuffer for the calling thread that matches 
	/// the given requirements.
	CommandBuffer get(uint32_t qfamily, vk::CommandPoolCreateFlags = {},
		vk::CommandBufferLevel = primaryCmdBufLevel);

	/// Allocates multiple commandBuffers for the calling thread with 
	/// the given requirements.
	/// Might be more efficient than multiple calls to get.
	std::vector<CommandBuffer> get(uint32_t qfamily, unsigned int count,
		vk::CommandPoolCreateFlags = {},
		vk::CommandBufferLevel = primaryCmdBufLevel);

	const auto& pools() const { return pools_; }

protected:
	std::vector<CommandPool> pools_;
};

} // namespace vpp
