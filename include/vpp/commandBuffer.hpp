// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vpp/fwd.hpp>
#include <vpp/resource.hpp>
#include <vpp/vulkan/enums.hpp>

#include <memory>
#include <map>
#include <vector>

// TODO: Document somewhere the synchorinzation requirements of these classes.
// e.g. CommandPools must be destroyed in the same thread as they were constructed.
// directly from the vulkan spec.


namespace vpp {

/// RAII vulkan CommandPool wrapper.
class CommandPool : public ResourceHandle<vk::CommandPool> {
public:
	CommandPool() = default;
	CommandPool(const Device& dev, std::uint32_t qfam, vk::CommandPoolCreateFlags flags = {});
	~CommandPool();

	CommandPool(CommandPool&& lhs) noexcept { swap(lhs); }
	CommandPool& operator=(CommandPool lhs) noexcept { swap(lhs); return *this; }

	CommandBuffer allocate(vk::CommandBufferLevel lvl = vk::CommandBufferLevel::primary);
	std::vector<CommandBuffer> allocate(std::size_t count,
		vk::CommandBufferLevel lvl = vk::CommandBufferLevel::primary);

	void reset(vk::CommandPoolResetFlags flags) const;

	const std::uint32_t& queueFamily() const { return qFamily_; }
	const vk::CommandPoolCreateFlags& flags() const { return flags_; }

	void swap(CommandPool& lhs) noexcept;

protected:
	vk::CommandPoolCreateFlags flags_ {};
	std::uint32_t qFamily_ {};
};

/// RAII vulkan CommandBuffer wrapper.
class CommandBuffer : public ResourceReferenceHandle<CommandBuffer, vk::CommandBuffer> {
public:
	CommandBuffer() = default;
	CommandBuffer(vk::CommandBuffer buffer, const CommandPool& pool);
	~CommandBuffer();

	CommandBuffer(CommandBuffer&& lhs) noexcept { swap(lhs); }
	CommandBuffer& operator=(CommandBuffer lhs) noexcept { swap(lhs); return *this; }

	const CommandPool& commandPool() const { return *commandPool_; }
	const CommandPool& resourceRef() const { return *commandPool_; }

	void swap(CommandBuffer& lhs) noexcept;

protected:
	const CommandPool* commandPool_ {};
};

/// Able to efficiently provide commandBuffer of all types for all threads by holding an internal
/// pool of commandPools.
class CommandProvider : public Resource {
public:
	CommandProvider(const Device& dev);
	~CommandProvider() = default;

	/// Allocates a commandBuffer for the calling thread that matches the given requirements.
	CommandBuffer get(std::uint32_t qfamily, vk::CommandPoolCreateFlags flags = {},
		vk::CommandBufferLevel lvl = vk::CommandBufferLevel::primary);

	/// Allocates multiple commandBuffers for the calling thread with the given requirements.
	/// Might be more efficient than multiple calls to get.
	std::vector<CommandBuffer> get(std::uint32_t qfamily, unsigned int count,
		vk::CommandPoolCreateFlags flags = {},
		vk::CommandBufferLevel lvl = vk::CommandBufferLevel::primary);

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
