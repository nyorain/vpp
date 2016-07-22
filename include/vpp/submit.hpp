#pragma once

#include <vpp/fwd.hpp>
#include <vpp/device.hpp>
#include <vpp/resource.hpp>

#include <unordered_map>
#include <vector>
#include <mutex>

namespace vpp
{

class Fence : public Resource
{
public:
	Fence() = default;
	Fence(const Device& dev);
	Fence(const Device& dev, const vk::FenceCreateInfo& info);
	~Fence();

	Fence(Fence&& other) noexcept { swap(*this, other); }
	Fence& operator=(Fence other) noexcept { swap(*this, other); return *this; }

	operator vk::Fence() const { return fence_; }
	friend void swap(Fence& a, Fence& b) noexcept;

protected:
	vk::Fence fence_ {};
};

///Can be used to track the state of a queued command buffer or to submit it to the device.
class CommandExecutionState : public Resource
{
public:
	using FencePtr = std::shared_ptr<Fence>;

public:
	CommandExecutionState() = default;
	CommandExecutionState(const Device& dev, CommandExecutionState** ptr);
	~CommandExecutionState();

	CommandExecutionState(CommandExecutionState&& other) noexcept;
	CommandExecutionState& operator=(CommandExecutionState&& other) noexcept;

	///Makes sure the commands associated with this control are submitted to the gpu.
	void submit();

	///Waits for the commands associated with this control to finish.
	///Will wait at least for the given timeout on nanoseconds.
	void wait(std::uint64_t timeout = ~std::uint64_t(0));

	///Returns whether the commands were submitted to the gpu.
	bool submitted() const;

	///Returns whether execution of the associated commands have been finished.
	bool completed() const;

	bool valid() const { return self_; }

protected:
	friend class SubmitManager;
	FencePtr fence_;
	CommandExecutionState** self_ {}; //pointer to the a unique_ptr in SubmitManager (Submission)
};

//TODO: split off class QueueManager. SubmitManager will only use QueueManager for locking.
//This way there can be multiple classes like SubmitManager (e.g. SparseBinder in future).

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
	struct Lock : public NonMovable, public Resource
	{
		Lock(const Device& dev);
		~Lock();
	};

public:
	///Submits all CommandBuffers in the submission queue.
	///There is no way directly check for completion, but in a single-threaded application,
	///one could simply wait for the device to become idle.
	void submit();

	///Submits all command buffers waiting for submission for the given queue.
	void submit(vk::Queue queue);

	///Adds a given vulkan submit info for exection of a commandBuffer on the given queue.
	///Note that this function does NOT directly submits the given info. It will wait until there
	///are many submissions batched together or a submit member function is called.
	///Note that all pointers in the vk::SubmitInfo must remain valid until the submission
	///submitted to the gpu.
	void add(vk::Queue, const vk::SubmitInfo& info, CommandExecutionState* state = nullptr);
	void add(vk::Queue, const std::vector<vk::CommandBuffer>& bufs, CommandExecutionState* = nullptr);
	void add(vk::Queue, vk::CommandBuffer buffer, CommandExecutionState* state = nullptr);

	///Function for ExecutionState
	bool submit(const CommandExecutionState& state);

	///This function must be called before submitting command buffers to the device.
	///All queues will be acquired as long as the return Lock object is alive.
	Lock acquire() const;

protected:
	struct Submission;
	friend class Device;
	friend class Lock;

protected:
	SubmitManager(const Device& dev);
	~SubmitManager();

protected:
	std::mutex mutex_;
	std::unordered_map<vk::Queue, std::vector<Submission>> submissions_;
};

}
