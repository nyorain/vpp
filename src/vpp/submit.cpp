// Copyright (c) 2016-2018 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#include <vpp/submit.hpp>
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

	if(!fences.empty()) {
		vk::waitForFences(device(), fences, true, UINT64_MAX);
	}
}

uint64_t QueueSubmitter::add(const vk::SubmitInfo& info, unsigned int* specificID)
{
	dlg_assert(queue_);
	if(specificID) {
		*specificID = pending_.size();
	}

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

	return id != id_;
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
		vk::resetFences(device(), {it->fence});
		unusedFences_.emplace_back(std::move(it->fence));
		fences_.erase(it);
		return true;
	}

	return false;
}

unsigned int QueueSubmitter::submit()
{
	// this function has a special focus on exception safety
	dlg_assert(queue_);
	if(pending_.empty()) {
		return 0u;
	}

	update();

	// get fence
	vpp::Fence fence;
	if(!unusedFences_.empty()) {
		fence = std::move(unusedFences_.back());
		unusedFences_.pop_back();
	} else {
		fence = {device()};
	}

	// move (clear) pending & increase id
	auto pending = std::move(pending_);
	auto id = id_;
	if(id_ == UINT64_MAX) {
		id_ = 1;
		wrapped_= true;
	} else {
		++id_;
	}

	// submit
	{
		// lock all queues, submit must finish without other queue operation
		// Note that vk::queueSubmit might throw
		QueueLock lock(device());
		vk::queueSubmit(queue(), pending, fence);
	}

	// set fence
	auto& entry = fences_.emplace_back();
	entry.id = id;
	entry.fence = std::move(fence);
	return pending.size();
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
	std::vector<vk::Fence> reset;
	for(auto it = fences_.begin(); it != fences_.end();) {
		auto status = vk::getFenceStatus(device(), it->fence);
		if(status == vk::Result::success) {
			reset.push_back(it->fence);
			unusedFences_.emplace_back(std::move(it->fence));
			it = fences_.erase(it);
		} else {
			++it;
		}
	}

	if(!reset.empty()) {
		vk::resetFences(device(), reset);
	}
}

} // namespace vpp
