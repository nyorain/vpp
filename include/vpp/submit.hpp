// Copyright (c) 2016-2018 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vpp/fwd.hpp>
#include <vpp/resource.hpp>
#include <vpp/sync.hpp>
#include <vpp/util/span.hpp>

#include <deque>
#include <vector>

namespace vpp {

/// Bundles multiple SubmitInfos into few queueSubmit calls.
/// Can be used to track the submit state using an id.
/// Is bound to a fixed queue and not synchronized in any way.
class QueueSubmitter : public ResourceReference<QueueSubmitter> {
public:
	QueueSubmitter() = default;
	QueueSubmitter(const Queue& queue);
	~QueueSubmitter();

	/// Adds a submission.
	/// Returns the id associated with this submission. The id
	/// will usually not be unique.
	/// All values referenced by the given SubmitInfo must
	/// stay valid until the returned id was submitted.
	/// It will not be possible to remove this submitInfo.
	/// If the given second argument is not null, sets it to the specific of
	/// this submission. Until the returned id is not submitted,
	/// pendingInfos()[specificID] can be used to refer to (and change)
	/// this submit info (since submissions cannot be deleted).
	uint64_t add(const vk::SubmitInfo& info, unsigned int* specificID = {});

	/// Submits all pending submissions.
	/// Returns the number of submissions.
	unsigned int submit();

	/// Makes sure the given id is submitted to the device.
	/// The id must have been returned by add.
	void submit(uint64_t);

	/// Returns whether the given id was already submitted.
	/// Also returns true if it has already completed.
	/// The id must have been returned by add.
	bool submitted(uint64_t) const;

	/// Returns if the submission associated with the given id
	/// has completed on the device.
	/// The id must have been returned by add.
	bool completed(uint64_t) const;

	/// Waits for the submission associated with the given id to complete.
	/// Will submit it if it hasn't been yet submitted.
	/// Returns whether the id is now completed (e.g. if false is
	/// returned, the timeout triggered the return).
	bool wait(uint64_t id, uint64_t timeout = UINT64_MAX);

	/// Returns the number of pending submissions.
	unsigned int pending() const;

	/// Can (but does not have to) be called at any time to
	/// check all possibly signaled fences. Will be automatically
	/// called on submit.
	void update();

	/// Returns the current id.
	/// Could be used to observe its state.
	uint64_t current() const { return id_; }

	const auto& pendingInfos() const { return pending_; }
	auto& pendingInfos() { return pending_; }

	const auto& queue() const { return *queue_; }
	const auto& resourceRef() const { return *queue_; }

protected:
	std::vector<vk::SubmitInfo> pending_;
	const vpp::Queue* queue_ {};

	struct NamedFence {
		uint64_t id;
		vpp::Fence fence;
	};

	mutable std::deque<NamedFence> fences_;
	mutable std::vector<vpp::Fence> unusedFences_;
	uint64_t id_ {1u};
	bool wrapped_ {};
};

} // namespace vpp
