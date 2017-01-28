// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vpp/fwd.hpp>
#include <vpp/resource.hpp>
#include <vpp/util/span.hpp>

#include <vector>
#include <mutex>

namespace vpp {

/// Class that manages all commands submitted to the device.
/// In vulkan, submitting work to the device is a pretty heavy operation and must be synchronized
/// (i.e. there should always only be one thread calling vkQueueSubmit no matter on which queue).
/// This class threadsafely manages this submissions and also batches mulitple command buffers
/// together which will increase performance.
/// There is always only one SubmitManager for a vulkan device and if vkQueueSumit is called
/// maually, it must be assured that no other thread calls this function or uses the submitManager
/// for the same device at the same moment.
class SubmitManager : public Resource {
public:
	/// Submits all CommandBuffers in the submission queue.
	/// Is the same as calling submit with all queues that have pending submissions.
	void submit();

	/// Submits all command buffers waiting for submission for the given queue.
	void submit(const vpp::Queue&);

	/// Adds a given vulkan submit info for exection of a commandBuffer on the given queue.
	/// Note that this function does not directly submits the given info.
	/// All pointers in the vk::SubmitInfo must remain valid until the submission gets
	/// submitted to the device (which can be assured by using an passed CommandExecutionState
	/// or calling submit on this SubmitManager).
	void add(const vpp::Queue&, nytl::Span<const vk::CommandBuffer>, CommandExecutionState* = {});
	void add(const vpp::Queue&, nytl::Span<const vk::CommandBuffer>,
		const vk::SubmitInfo&, CommandExecutionState* = {});


	/// Makes sure the command buffers associated with the given state are submitted to
	/// the device. Will output a warning in debug mode if the given state is invalid.
	/// Usually not called manually. Prefer to use CommandExecutionState::submit.
	void submit(const CommandExecutionState&);

	/// Removed the given CommandExecutionState from the potential observers.
	/// Usually not called manually, triggered on CommandExeuctionState destruction.
	/// Note that this function does not remove the associated command buffers.
	void removeStateObserver(const CommandExecutionState&);

	/// Moves the given CommandExeuctionState observer to another object.
	/// Usually not called manually, triggered by the CommandExeuctionState move
	/// constructor and assignment operator.
	void moveStateObserver(const CommandExecutionState& old, CommandExecutionState& newOne);

protected:
	struct Submission;
	friend class Device;

	SubmitManager(const Device& dev);
	~SubmitManager();

protected:
	std::vector<Submission> pending_;
	std::mutex mutex_; // internal mutex
};

/// Can be used to track the state of a queued command buffer.
/// Also useful to make sure the associated command buffers were submitted
/// to the device or wait for them to complete execution.
/// Created with default constructor and then passed to SubmitManager when
/// adding a pending command buffer submission.
/// Internally uses a shared fence pointer as soon as the command buffers are submitted
/// since multiple CommandExeuctionState objects might be associated with the
/// same fence.
class CommandExecutionState {
public:
	CommandExecutionState(); // = default
	~CommandExecutionState();

	CommandExecutionState(CommandExecutionState&& other) noexcept;
	CommandExecutionState& operator=(CommandExecutionState&& other) noexcept;

	/// Makes sure the associated command buffers are submitted to the device.
	/// Will have no effect if they were already submitted.
	void submit();

	/// Waits until execution of the associated command buffers has finished.
	/// Has no effect and returns immediatly if they already have finished.
	/// Will submit them if they are not already submitted.
	/// Will wait at least for the given timeout in nanoseconds or until the
	/// command buffers have finished otherwise.
	/// Can be called with timeout 0 to simply update the state.
	/// The default timeout value (i.e. all bits set) will wait without timeout.
	/// Returns true if the command buffers have finished execution after this
	/// call, false otherwise.
	bool wait(std::uint64_t timeout = ~std::uint64_t(0));

	/// Returns whether the associated command buffers were submitted to the device.
	bool submitted() const;

	/// Returns whether the associated command buffers have finished their
	/// execution on the device.
	/// Will update the state if it is not already completed.
	bool completed() const;

	/// Returns whether this object is valid, i.e. if there is
	/// a command buffer submissions associated with it.
	/// Is invalid if only defaulted constructed and valid if it was
	/// passed to a SubmitManager when adding a submission.
	/// Will return true even if the associated command buffers have finished.
	bool valid() const { return (submitManager_ || completed_ || fence_); }

	/// Returns the fence associated with the exeuction of the associated command buffers.
	/// If this object is invalid, the associated command buffers were not yet submitted or
	/// have already finished returns a nullHandle.
	vk::Fence fence() const;

protected:
	friend class SubmitManager;
	void init(SubmitManager&);

	SubmitManager* submitManager_ {};

	// mutable since changed by completed()
	mutable std::shared_ptr<Fence> fence_ {};
	mutable bool completed_ {};
};


} // namespace vpp
