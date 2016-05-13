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
	CommandBuffer(vk::CommandBuffer buffer, const CommandPool& pool);
	~CommandBuffer();

	CommandBuffer(CommandBuffer&& other) noexcept;
	CommandBuffer& operator=(CommandBuffer&& other) noexcept;

	void swap(CommandBuffer& other) noexcept;

	const CommandPool& resourceRef() const { return *commandPool_; }

	const CommandPool& commandPool() const { return *commandPool_; }
	vk::CommandBuffer vkCommandBuffer() const { return commandBuffer_; }
};

//CommandManagerEntry
struct CommandManagerEntry
{
	vk::CommandBuffer buffer {};
	vk::Fence fence {};
	bool destroy {false};
};

//Commandbuffer exectuion state.
class CommandExecutionState
{
protected:
	std::shared_ptr<CommandManagerEntry> entry_;

public:
	CommandExecutionState(const std::shared_ptr<CommandManagerEntry>& ptr) : entry_(ptr) {}
	~CommandExecutionState() = default;

	///Returns whether execution of the associated commandBuffer has been finished.
	bool finished() const;

	///Waits until the associated commandBuffer is fully executed.
	void wait() const;
};

///CommandManager
class CommandManager : public Resource
{
protected:
	std::vector<std::shared_ptr<CommandManagerEntry>> buffers_;

public:
	CommandManager(const Device& dev) : Resource(dev) {};

	CommandExecutionState execute(vk::CommandBuffer buffer, vk::Queue queue, vk::SubmitInfo info);
	void execute(vk::CommandBuffer buffer, vk::Queue queue, vk::SubmitInfo info,
		const std::shared_ptr<CommandManagerEntry>& entry);

	void wait();
};

///Useful for executing setup commands on the vulkan device.
class SetupCommandBuffer : public ResourceReference<SetupCommandBuffer>
{
protected:
	CommandBuffer commandBuffer_;
	std::shared_ptr<CommandManagerEntry> entry_;

public:
	SetupCommandBuffer() = default;
	SetupCommandBuffer(CommandBuffer&& buffer);
	~SetupCommandBuffer();

	SetupCommandBuffer(SetupCommandBuffer&& other) noexcept = default;
	SetupCommandBuffer& operator=(SetupCommandBuffer&& other) noexcept = default;

	const CommandBuffer& resourceRef() const { return commandBuffer_; }

	CommandExecutionState state() const { return {entry_}; };
	const CommandBuffer& commandBuffer() const { return commandBuffer_; }
	vk::CommandBuffer vkCommandBuffer() const { return commandBuffer_.vkCommandBuffer(); }

	///Conversion operator to make vulkan operations more convinient.
	operator vk::CommandBuffer() const { return vkCommandBuffer(); }
};

//CommandPool
//XXX: needed?
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
	CommandPool(const Device& dev, std::uint32_t qfam, vk::CommandPoolCreateFlags flags = {});
	~CommandPool();

	CommandPool(CommandPool&& other) noexcept;
	CommandPool& operator=(CommandPool&& other) noexcept;

	void swap(CommandPool& other) noexcept;

	std::vector<CommandBuffer> allocate(std::size_t count,
		vk::CommandBufferLevel lvl = vk::CommandBufferLevel::Primary);
	CommandBuffer allocate(vk::CommandBufferLevel lvl = vk::CommandBufferLevel::Primary);

	void reset(vk::CommandPoolResetFlags flags) const;

	std::uint32_t queueFamily() const { return qFamily_; }
	vk::CommandPoolCreateFlags flags() const { return flags_; }
	vk::CommandPool vkCommandPool() const { return commandPool_; }
};

///Can be used to track the state of a queued command buffer or to submit it to the device.
class ExecutionState : public Resource
{
public:
	void submit();
	void wait();
	bool submitted() const { return submitted_; }

protected:
	bool submitted_ = false;
	vk::Queue queue_; //queue the command buffer is submitted to
};

///Class that manages all commands submitted to the gpu.
///In vulkan, submitting work to the device is a pretty heavy operation and must be synchronized
///(i.e. there should always only be one thread calling vkQueueSubmit no matter on which queue).
///This class threadsafely manages this submissions and also batches mulitple command buffers
///together which will increase performance.
///There is always only one SubmitManager for a vulkan device and if vkQueueSumit is called
///maually, it must be assured that no other thread calls this function or uses the submitManager
///for the same device at the same moment.
class SubmitManager : public Resource
{
public:
	///Submits all CommandBuffers in the submission queue.
	///To wait for their completion, one can simply wait for the device to become idle.
	void submit();

	///Submits all command buffers waiting for submission for the given queue.
	///Returns the associated vulkan fence.
	vk::Fence submit(vk::Queue queue);

	///Adds a given vulkan submit info for exection of a commandBuffer on the given queue.
	///Note that this function does NOT directly submits the given info. It will wait until there
	///are many submissions batched together or a submit member function is called.
	ExecutionState add(vk::Queue, vk::SubmitInfo info);

protected:
	std::size_t autoSubmitThreshold_;
	std::map<vk::Queue, std::vector<vk::SubmitInfo>> submissions_;
};

}
