// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vpp/fwd.hpp>
#include <vpp/resource.hpp>
#include <vpp/util/nonCopyable.hpp>
#include <vpp/vulkan/enums.hpp>

#include <vector> // std::vector
#include <functional> // std::function

namespace vpp {

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

	CommandBuffer allocate(vk::CommandBufferLevel lvl = vk::CommandBufferLevel::primary);
	std::vector<CommandBuffer> allocate(size_t count,
		vk::CommandBufferLevel = vk::CommandBufferLevel::primary);

	uint32_t queueFamily() const { return qFamily_; }
	vk::CommandPoolCreateFlags flags() const { return flags_; }

	friend void swap(CommandPool& a, CommandPool& b) noexcept;

protected:
	vk::CommandPoolCreateFlags flags_ {};
	uint32_t qFamily_ {};
};

/// RAII vulkan CommandBuffer wrapper.
/// Note that the CommandBuffer must not be in use by any device when it is destructed.
/// Keeps a reference to the CommandPool it was allocated from.
class CommandBuffer : public ResourceReferenceHandle<CommandBuffer, vk::CommandBuffer> {
public:
	CommandBuffer() = default;
	CommandBuffer(vk::CommandBuffer buffer, const CommandPool& pool);
	~CommandBuffer();

	CommandBuffer(CommandBuffer&& lhs) noexcept { swap(*this, lhs); }
	CommandBuffer& operator=(CommandBuffer lhs) noexcept { swap(*this, lhs); return *this; }

	const CommandPool& commandPool() const { return *commandPool_; }
	const CommandPool& resourceRef() const { return *commandPool_; }

	friend void swap(CommandBuffer& a, CommandBuffer& b) noexcept;

protected:
	const CommandPool* commandPool_ {};
};

/// Able to efficiently provide commandBuffer of all types for all threads by holding an internal
/// pool of commandPools. Will use the thread specific storage mechanism of the
/// associated device.
/// Thought as a general command pool providing mechanism mainly useful for
/// initialization and general usage e.g. by vpp or other libraries where not every resource
/// can manage its own CommandPool. On hot paths one should still use fixed custom
/// CommandPools.
class CommandProvider : public Resource {
public:
	CommandProvider(const Device& dev);
	~CommandProvider() = default;

	/// Allocates a commandBuffer for the calling thread that matches the given requirements.
	CommandBuffer get(uint32_t qfamily, vk::CommandPoolCreateFlags = {},
		vk::CommandBufferLevel = vk::CommandBufferLevel::primary);

	/// Allocates multiple commandBuffers for the calling thread with the given requirements.
	/// Might be more efficient than multiple calls to get.
	std::vector<CommandBuffer> get(uint32_t qfamily, unsigned int count,
		vk::CommandPoolCreateFlags = {},
		vk::CommandBufferLevel = vk::CommandBufferLevel::primary);

	/// Allocates a CommandBuffer from the first CommandPool of the already existent
	/// commandBuffer for which the given predicate returns true.
	/// If there are no CommandPools yet, or the predicate returns false for all of
	/// them, a default-constructed CommandBuffer object is returned.
	CommandBuffer get(const std::function<bool(const CommandPool& pool)>& pred);

	/// Allcoates multiple CommandBuffers on a CommandPool that is accepted by the predicate.
	CommandBuffer get(const std::function<bool(const CommandPool& pool)>& pred, unsigned int count);

protected:
	unsigned int tlsID_ {};
};

} // namespace vpp
