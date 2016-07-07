#pragma once

//experimental feature, may not be fully working

#include <vpp/fwd.hpp>
#include <vpp/resource.hpp>

#include <memory>
#include <map>
#include <vector>

namespace vpp
{

//Document somewhere the synchorinzation requirements of these classes.
//e.g. CommandBuffers must be destroyed in the same thread as they were constructed.

class CommandPool;
namespace fwd { extern const vk::CommandBufferLevel commandBufferLevelPrimary; }

///XXX: this class really needed?
//CommandBuffer
class CommandBuffer : public ResourceReference<CommandBuffer>
{
public:
	CommandBuffer() = default;
	CommandBuffer(vk::CommandBuffer buffer, const CommandPool& pool);
	~CommandBuffer();

	CommandBuffer(CommandBuffer&& other) noexcept;
	CommandBuffer& operator=(CommandBuffer other) noexcept;

	const CommandPool& commandPool() const { return *commandPool_; }
	const vk::CommandBuffer& vkCommandBuffer() const { return commandBuffer_; }

	const CommandPool& resourceRef() const { return *commandPool_; }
	operator vk::CommandBuffer() const { return vkCommandBuffer(); }
	friend void swap(CommandBuffer& a, CommandBuffer& b) noexcept;

protected:
	vk::CommandBuffer commandBuffer_ {};
	const CommandPool* commandPool_ {};
};

//CommandPool
//XXX: needed?
class CommandPool : public Resource
{
public:
	CommandPool() = default;
	CommandPool(const Device& dev, std::uint32_t qfam, vk::CommandPoolCreateFlags flags = {});
	~CommandPool();

	CommandPool(CommandPool&& other) noexcept;
	CommandPool& operator=(CommandPool other) noexcept;

	std::vector<CommandBuffer> allocate(std::size_t count,
		vk::CommandBufferLevel lvl = fwd::commandBufferLevelPrimary);
	CommandBuffer allocate(vk::CommandBufferLevel lvl = fwd::commandBufferLevelPrimary);

	void reset(vk::CommandPoolResetFlags flags) const;

	const std::uint32_t& queueFamily() const { return qFamily_; }
	const vk::CommandPoolCreateFlags& flags() const { return flags_; }
	const vk::CommandPool& vkCommandPool() const { return commandPool_; }

	operator vk::CommandPool() const { return vkCommandPool(); }
	friend void swap(CommandPool& a, CommandPool& b) noexcept;

protected:
	vk::CommandPool commandPool_ {};
	vk::CommandPoolCreateFlags flags_ {};
	std::uint32_t qFamily_ {};
};

}
