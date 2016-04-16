#pragma once

#include <vpp/fwd.hpp>
#include <vpp/resource.hpp>
#include <memory>

namespace vpp
{

class CommandPool;

///XXX: this class really needed?
//CommandBuffer
class CommandBuffer : public ResourceReference<CommandBuffer>
{
protected:
	vk::CommandBuffer commandBuffer_ {};
	const CommandPool* commandPool_ {};

protected:
	void destroy();

public:
	CommandBuffer() = default;
	CommandBuffer(vk::CommandBuffer buffer, const CommandPool* pool);
	~CommandBuffer();

	CommandBuffer(CommandBuffer&& other) noexcept;
	CommandBuffer& operator=(CommandBuffer&& other) noexcept;

	void swap(CommandBuffer& other) noexcept;

	const CommandPool& resourceRef() const { return *commandPool_; }

	const CommandPool& commandPool() const { return *commandPool_; }
	vk::CommandBuffer vkCommandBuffer() const { return commandBuffer_; }
};

///Useful for executing setup commands on the vulkan device.
class SetupCommandBuffer : public ResourceReference<SetupCommandBuffer>
{
protected:
	CommandBuffer commandBuffer_;

public:
	SetupCommandBuffer() = default;
	SetupCommandBuffer(CommandBuffer&& buffer);
	~SetupCommandBuffer();

	SetupCommandBuffer(SetupCommandBuffer&& other) noexcept = default;
	SetupCommandBuffer& operator=(SetupCommandBuffer&& other) noexcept = default;

	const CommandBuffer& resourceRef() const { return commandBuffer_; }

	vk::CommandBuffer vkCommandBuffer() const { return commandBuffer_.vkCommandBuffer(); }

	///Conversion operator to make vulkan operations more convinient.
	operator vk::CommandBuffer() const { return vkCommandBuffer(); }
};

//CommandPool
class CommandPool : public Resource
{
protected:
	vk::CommandPool commandPool_ {};
	vk::CommandPoolCreateFlags flags_ {};
	std::uint32_t qFamily_ {};

protected:
	void destroy();

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

///XXX: experimental concept
class CommandBufferExecutioner : public Resource
{
public:
	struct Entry
	{
		vk::CommandBuffer buffer {};
		vk::SubmitInfo info {};
		vk::Queue queue {};
		vk::Fence fence {};
		bool destroy {true};
	};

	class ExecutionState
	{
	protected:
		std::shared_ptr<Entry> entry_;

	public:
		bool finished() const;
		void wait();
	};

protected:
	std::vector<std::shared_ptr<Entry>> submitted_;

public:
	ExecutionState execute(vk::CommandBuffer buffer, vk::Queue queue, vk::SubmitInfo info);
	void wait();
};

}
