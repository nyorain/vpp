#pragma once

#include <vpp/fwd.hpp>
#include <vpp/resource.hpp>

namespace vpp
{

class CommandPool;

///XXX: this class really needed? pool useful for the provider. But this class...?
//CommandBuffer
class CommandBuffer : public ResourceReference<CommandBuffer>
{
protected:
	vk::CommandBuffer commandBuffer_;
	CommandPool* pool_;

public:
	CommandBuffer() = default;
	CommandBuffer(vk::CommandBuffer buffer, CommandPool* pool);
	~CommandBuffer();

	CommandBuffer(CommandBuffer&& other) noexcept;
	CommandBuffer& operator=(CommandBuffer&& other) noexcept;

	void swap(CommandBuffer& other) noexcept;

	const CommandPool& resourceRef() const { return *pool_; }

	const CommandPool& commandPool() const { return *pool_; }
	vk::CommandBuffer vkCommandBuffer() const { return commandBuffer_; }
};

///Useful for executing setup commands on the vulkan device.
class SetupCommandBuffer : public ResourceReference<SetupCommandBuffer>
{
protected:
	CommandBuffer cmdBuffer_;

public:
	SetupCommandBuffer() = default;
	SetupCommandBuffer(CommandBuffer&& buffer);
	~SetupCommandBuffer();

	SetupCommandBuffer(SetupCommandBuffer&& other) noexcept;
	SetupCommandBuffer& operator=(SetupCommandBuffer&& other) noexcept;

	const CommandBuffer& resourceRef() const { return cmdBuffer_; }

	///Conversion operator to make vulkan operations more convinient.
	operator vk::CommandBuffer() const { return cmdBuffer_.vkCommandBuffer(); }
};

//CommandPool
class CommandPool : public Resource
{
protected:
	vk::CommandPool commandPool_;
	vk::CommandPoolCreateFlags flags_;
	std::uint32_t qFamily_;

public:
	CommandPool() = default;
	CommandPool(const Device& dev);
	~CommandPool();

	CommandPool(CommandPool&& other) noexcept;
	CommandPool& operator=(CommandPool&& other) noexcept;

	void swap(CommandPool& other) noexcept;

	std::vector<CommandBuffer> allocate(std::size_t count,
		vk::CommandBufferLevel lvl = vk::CommandBufferLevel::Primary);

	CommandBuffer allocate(vk::CommandBufferLevel lvl = vk::CommandBufferLevel::Primary);

	std::uint32_t queueFamily() const { return qFamily_; }
	vk::CommandPoolCreateFlags flags() const { return flags_; }
	vk::CommandPool vkCommandPool() const { return commandPool_; }
};

}
