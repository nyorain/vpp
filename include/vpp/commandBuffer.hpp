#pragma once

#include <vpp/fwd.hpp>
#include <vpp/resource.hpp>
#include <vpp/vulkan/enums.hpp>

#include <memory>
#include <map>
#include <vector>

namespace vpp
{

//Document somewhere the synchorinzation requirements of these classes.
//e.g. CommandPools must be destroyed in the same thread as they were constructed.

class CommandPool;

//RAII vulkan CommandBuffer wrapper.
class CommandBuffer : public ResourceHandleReference<vk::CommandBuffer, CommandBuffer>
{
public:
	CommandBuffer() = default;
	CommandBuffer(vk::CommandBuffer buffer, const CommandPool& pool);
	~CommandBuffer();

	CommandBuffer(CommandBuffer&& other) noexcept = default;
	CommandBuffer& operator=(CommandBuffer&& other) noexcept = default;

	const CommandPool& commandPool() const { return *commandPool_; }
	const CommandPool& resourceRef() const { return *commandPool_; }

protected:
	const CommandPool* commandPool_ {};
};

//RAII vulkan CommandPool wrapper.
class CommandPool : public ResourceHandle<vk::CommandPool>
{
public:
	CommandPool() = default;
	CommandPool(const Device& dev, std::uint32_t qfam, vk::CommandPoolCreateFlags flags = {});
	~CommandPool();

	CommandPool(CommandPool&& other) noexcept = default;
	CommandPool& operator=(CommandPool&& other) noexcept = default;

	CommandBuffer allocate(vk::CommandBufferLevel lvl = vk::CommandBufferLevel::primary);
	std::vector<CommandBuffer> allocate(std::size_t count,
		vk::CommandBufferLevel lvl = vk::CommandBufferLevel::primary);

	void reset(vk::CommandPoolResetFlags flags) const;

	const std::uint32_t& queueFamily() const { return qFamily_; }
	const vk::CommandPoolCreateFlags& flags() const { return flags_; }

protected:
	vk::CommandPoolCreateFlags flags_ {};
	std::uint32_t qFamily_ {};
};

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

	//Allocates multiple commandBuffers for the calling thread with the given requirements.
	///Might be more efficient than multiple calls to get.
	std::vector<CommandBuffer> get(std::uint32_t qfamily, unsigned int count,
		vk::CommandPoolCreateFlags flags = {},
		vk::CommandBufferLevel lvl = vk::CommandBufferLevel::primary);

	///Allocates a CommandBuffer from the first CommandPool of the already existent
	///commandBuffer for which the given predicate returns true.
	///If there are no CommandPools yet, or the predicate returns false for all of
	///them, an default-constructed CommandBuffer object is returned.
	CommandBuffer get(const std::function<bool(const CommandPool& pool)>& pred);

	///Allcoated multiple CommandBuffers on a CommandPool that is accepted by the predicate.
	CommandBuffer get(const std::function<bool(const CommandPool& pool)>& pred, unsigned int count);
};

}
