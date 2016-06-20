#pragma once

#include <vpp/fwd.hpp>
#include <vpp/device.hpp>
#include <vpp/resource.hpp>

#include <unordered_map>
#include <vector>
#include <mutex>

namespace vpp
{

///A submission for executing work on the device.
///If the submission was not yet submitted the fence member is 0.
///Internally used by CommandExecutionState and SubmitManager.
class CommandSubmission;

///Typedef for a shared Submission ptr, since this class is ususally used as shared ptr.
using CommandSubmissionPtr = std::shared_ptr<CommandSubmission>;

///Can be used to track the state of a queued command buffer or to submit it to the device.
class CommandExecutionState : public ResourceReference<CommandExecutionState>
{
public:
	CommandExecutionState() = default;
	CommandExecutionState(CommandSubmissionPtr subm) : submission_(std::move(subm)) {}
	~CommandExecutionState() = default;

	CommandExecutionState(CommandExecutionState&& other) noexcept = default;
	CommandExecutionState& operator=(CommandExecutionState&& other) noexcept = default;

	///Makes sure the commands associated with this control are submitted to the gpu.
	void submit();

	///Waits for the commands associated with this control to finish.
	///Will wait at least for the given timeout on nanoseconds.
	void wait(std::uint64_t timeout = ~std::uint64_t(0));

	///Returns whether the commands were submitted to the gpu.
	bool submitted() const;

	///Returns whether execution of the associated commands have been finished.
	bool completed() const;

	const Resource& resourceRef() const;
	const CommandSubmissionPtr& submission() const { return submission_; }

protected:
	CommandSubmissionPtr submission_;
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
	CommandExecutionState add(vk::Queue, const vk::SubmitInfo& info);
	CommandExecutionState add(vk::Queue, const std::vector<vk::CommandBuffer>& buffer);

	///Function for ExecutionState
	void submit(const CommandSubmissionPtr& ptr);

protected:
	friend class Device;
	SubmitManager(const Device& dev) : Resource(dev) {}

protected:
	std::mutex mutex_;
	std::size_t autoSubmitThreshold_; //XXX: needed?
	std::unordered_map<vk::Queue, std::vector<CommandSubmissionPtr>> submissions_;
};

}
