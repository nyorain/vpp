// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#include <vpp/submit.hpp>
#include <vpp/queue.hpp>
#include <vpp/sync.hpp>
#include <vpp/vk.hpp>
#include <vpp/util/log.hpp>

#include <algorithm> // std::remove_if

namespace vpp {

struct SubmitManager::Submission {
	const vpp::Queue* queue {};
	vk::SubmitInfo info {};
	std::vector<vk::CommandBuffer> buffers {};
	CommandExecutionState* state {};
};

// SubmitManager
SubmitManager::SubmitManager(const Device& dev) : Resource(dev)
{
}

SubmitManager::~SubmitManager()
{
	dlg_check("~SubmitManager", {
		if(!pending_.empty())
			vpp_warn("There are ", pending_.size(), " pending submission left");
	});
}

void SubmitManager::submit()
{
	while(!pending_.empty())
		submit(*pending_.begin()->queue);
}

void SubmitManager::submit(const vpp::Queue& queue)
{
	dlg_check("SubmitManager::submit(queue)", {
		if(queue.device() != device())
			vpp_error("invalid queue given");
	});

	// lock own mutex and mutex of all queues
	std::lock_guard<std::mutex> internalLock(mutex_);

	// find pending submissions for the given queue
	// also check if a fence must be created
	// also make sure command buffer pointers are valid
	std::vector<vk::SubmitInfo> submitInfos;
	submitInfos.reserve(pending_.size());
	auto createFence = false;

	for(const auto& sub : pending_) {
		if(sub.queue != &queue) continue;
		submitInfos.push_back(sub.info);
		createFence = (sub.state != nullptr);

		submitInfos.back().pCommandBuffers = sub.buffers.data();
		submitInfos.back().commandBufferCount = sub.buffers.size();
	}

	if(submitInfos.empty())
		return;

	// create fence if needed
	std::shared_ptr<Fence> shfence;
	if(createFence) {
		shfence = std::make_shared<Fence>(device());

		for(const auto& sub : pending_)
			if(sub.queue == &queue && sub.state)
				sub.state->fence_ = shfence;
	}

	// Lock all queues for submission
	// submit the queue
	{
		QueueLock queueLock(device());
		if(shfence) vk::queueSubmit(queue, submitInfos, shfence->vkHandle());
		else vk::queueSubmit(queue, submitInfos);
	}

	// erase all submitted submissions
	auto pred = [&](const Submission& sub) { return sub.queue == &queue; };
	pending_.erase(std::remove_if(pending_.begin(), pending_.end(), pred), pending_.end());
}

void SubmitManager::add(const vpp::Queue& queue, nytl::Span<const vk::CommandBuffer> buffers,
	CommandExecutionState* state)
{
	add(queue, buffers, {}, state);
}

void SubmitManager::add(const vpp::Queue& queue, nytl::Span<const vk::CommandBuffer> buffers,
	const vk::SubmitInfo& info, CommandExecutionState* state)
{
	dlg_check("SubmitManager::add", {
		if(queue.device() != device())
			vpp_error("invalid queue given");
	});

	auto submission = Submission {};
	submission.queue = &queue;
	submission.info = info;
	submission.buffers = {buffers.begin(), buffers.end()};

	if(state) {
		submission.state = state;
		state->init(*this);
	}

	std::lock_guard<std::mutex> internalLock(mutex_);
	pending_.push_back(std::move(submission));
}

void SubmitManager::submit(const CommandExecutionState& state)
{
	const vpp::Queue* queue {};

	{
		std::lock_guard<std::mutex> internalLock(mutex_);
		auto pred = [&](const Submission& sub) { return (sub.state == &state); };
		auto it = std::find_if(pending_.begin(), pending_.end(), pred);
		if(it != pending_.end()) queue = it->queue;
	}

	dlg_check("SubmitManager::submit(state)", {
		if(!queue)
			vpp_error("::SubmitManager::submit(state)"_src, "could not find given state");
	});

	submit(*queue);
}

void SubmitManager::moveStateObserver(const CommandExecutionState& oldOne,
	CommandExecutionState& newOne)
{
	std::lock_guard<std::mutex> internalLock(mutex_);
	auto pred = [&](const Submission& sub) { return (sub.state == &oldOne); };
	auto it = std::find_if(pending_.begin(), pending_.end(), pred);

	dlg_check("SubmitManager::moveStateObserver", {
		if(it == pending_.end())
			vpp_warn("Could not find old state. Probably crashing now");
	});

	it->state = &newOne;
}

void SubmitManager::removeStateObserver(const CommandExecutionState& state)
{
	std::lock_guard<std::mutex> internalLock(mutex_);
	auto pred = [&](const Submission& sub) { return (sub.state == &state); };
	auto it = std::find_if(pending_.begin(), pending_.end(), pred);

	dlg_check("SubmitManager::removeStateObserver", {
		if(it == pending_.end())
			vpp_warn("Could not find command execution state");
	});

	pending_.erase(it);
}

// CommandExecutionState
CommandExecutionState::CommandExecutionState() = default;
CommandExecutionState::~CommandExecutionState()
{
	if(submitManager_ && !fence_ && !completed_)
		submitManager_->removeStateObserver(*this);
}

CommandExecutionState::CommandExecutionState(CommandExecutionState&& other) noexcept
	: submitManager_(other.submitManager_), fence_(std::move(other.fence_)),
		completed_(other.completed_)
{
	other.completed_ = {};
	other.submitManager_ = {};

	if(submitManager_)
		submitManager_->moveStateObserver(other, *this);
}

CommandExecutionState& CommandExecutionState::operator=(CommandExecutionState&& other) noexcept
{
	if(submitManager_)
		submitManager_->removeStateObserver(*this);

	submitManager_ = other.submitManager_;
	fence_ = std::move(other.fence_);
	completed_ = other.completed_;

	other.completed_ = {};
	other.submitManager_ = {};

	if(submitManager_)
		submitManager_->moveStateObserver(other, *this);

	return *this;
}

void CommandExecutionState::submit()
{
	if(fence_ || completed_) return;

	dlg_check("CommandExecutionState::submit", {
		if(!submitManager_)
			vpp_error("::CommandExecutionState::submit"_src, "invalid object");
	});

	submitManager_->submit(*this);
}

bool CommandExecutionState::wait(std::uint64_t timeout)
{
	if(completed_) return true;
	if(!fence_) submit();

	auto result = vk::waitForFences(submitManager_->device(), 1, *fence_, 0, timeout);
	if(result == vk::Result::success) {
		completed_ = true;
		fence_ = {};
	}

	return completed_;
}

bool CommandExecutionState::submitted() const
{
	return fence_.get() != nullptr;
}

bool CommandExecutionState::completed() const
{
	if(!submitted()) return false;
	if(completed_) return true;

	auto result = vk::getFenceStatus(submitManager_->device(), *fence_);
	if(result == vk::Result::success) {
		completed_ = true;
		fence_ = {};
	}

	return completed_;
}

void CommandExecutionState::init(SubmitManager& submitManager)
{
	if(submitManager_ && !fence_ && !completed_)
		submitManager_->removeStateObserver(*this);

	fence_ = {};
	completed_ = {};
	submitManager_ = &submitManager;
}

vk::Fence CommandExecutionState::fence() const
{
	if(fence_) return *fence_;
	return {};
}

} // namespace vpp
