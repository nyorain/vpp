// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vpp/fwd.hpp>
#include <vpp/resource.hpp>
#include <vpp/sync.hpp>
#include <vpp/vulkan/span.hpp>

#include <deque>
#include <vector>

namespace vpp {

// TODO: movable?
/// Bundles multiple SubmitInfos into few queueSubmit calls.
/// Can be used to track the submit state using an id.
/// Is bound to a fixed queue.
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
	uint64_t add(const vk::SubmitInfo& info);

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

	/// Submits all pending submissions.
	/// Returns the number of submissions.
	unsigned int submit();

	/// Returns the number of pending submissions.
	unsigned int pending() const;

	/// Can (but does not have to) be called at any time to
	/// check all possibly signaled fences. Will be automatically
	/// called on submit.
	void update();

	/// Returns the current id.
	/// Could be used to observe its state.
	uint64_t current() const { return id_; }

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

#include <vpp/queue.hpp>
#include <vpp/vk.hpp>
#include <dlg/dlg.hpp>
#include <algorithm>

namespace vpp {

QueueSubmitter::QueueSubmitter(const Queue& queue) : queue_(&queue)
{
}

QueueSubmitter::~QueueSubmitter()
{
	// make sure all fences have completed before destroying them
	std::vector<vk::Fence> fences;
	for(auto& fence : fences_) {
		auto status = vk::getFenceStatus(device(), fence.fence);
		if(status != vk::Result::success) {
			fences.push_back(fence.fence);
		}
	}

	vk::waitForFences(device(), fences, true, UINT64_MAX);
}

uint64_t QueueSubmitter::add(const vk::SubmitInfo& info)
{
	dlg_assert(queue_);
	pending_.push_back(info);
	return id_;
}

void QueueSubmitter::submit(uint64_t id)
{
	dlg_assert(queue_);
	dlg_assert(id != 0);
	dlg_assert(wrapped_ || id <= id_);

	if(id == id_) {
		submit();
	}
}

bool QueueSubmitter::submitted(uint64_t id) const
{
	dlg_assert(queue_);
	dlg_assert(id != 0);
	dlg_assert(wrapped_ || id <= id_);

	return id == id_;
}

bool QueueSubmitter::completed(uint64_t id) const
{
	dlg_assert(queue_);
	dlg_assert(id != 0);
	dlg_assert(wrapped_ || id <= id_);

	if(id == id_) {
		return false;
	}

	auto it = std::find_if(fences_.begin(), fences_.end(), 
		[&](auto& fence) { return fence.id == id; });
	if(it == fences_.end()) {
		return true;
	}

	auto status = vk::getFenceStatus(device(), it->fence);
	if(status == vk::Result::success) {
		unusedFences_.emplace_back(std::move(it->fence));
		fences_.erase(it);
		return true;
	}

	return false;
}

unsigned int QueueSubmitter::submit()
{
	dlg_assert(queue_);
	if(pending_.empty()) {
		return 0u;
	}

	shrink();

	// get fence
	// make sure everything is exception safe
	vpp::Fence fence;
	if(!unusedFences_.empty()) {
		fence = std::move(unusedFences_.back());
		unusedFences_.pop_back();
	} else {
		fence = {device()};
	}

	// submit
	{
		// lock all queues, submit must finish without other queue operation
		QueueLock lock(device());
		vk::queueSubmit(queue(), pending_, fence);
	}

	// set fence
	auto& entry = fences_.emplace_back();
	entry.id = id_;
	entry.fence = std::move(fence);

	// increase id
	if(id_ == UINT64_MAX) {
		id_ = 1;
		wrapped_= true;
	} else {
		++id_;
	}

	// clear pending
	auto ret = pending_.size();
	pending_.clear();
	return ret;
}

bool QueueSubmitter::wait(uint64_t id, uint64_t timeout)
{
	dlg_assert(queue_);
	dlg_assert(id != 0);
	dlg_assert(wrapped_ || id <= id_);

	submit(id);

	auto it = std::find_if(fences_.begin(), fences_.end(), 
		[&](auto& fence) { return fence.id == id; });
	if(it == fences_.end()) {
		return true;
	}

	auto fh = it->fence.vkHandle();
	dlg_assert(fh);
	auto res = vk::waitForFences(device(), {fh}, false, timeout);
	return res == vk::Result::success;
}

unsigned int QueueSubmitter::pending() const
{
	dlg_assert(queue_);
	return pending_.size();
}

void QueueSubmitter::update()
{
	dlg_assert(queue_);
	for(auto it = fences_.begin(); it < fences_.end();) {
		auto status = vk::getFenceStatus(device(), it->fence);
		if(status == vk::Result::success) {
			unusedFences_.emplace_back(std::move(it->fence));
			it = fences_.erase(it);
		} else {
			++it;
		}
	}
}

} // namespace vpp