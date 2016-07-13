#include <vpp/submit.hpp>
#include <vpp/queue.hpp>
#include <vpp/vk.hpp>
#include <algorithm>

namespace vpp
{

struct SubmitManager::Submission
{
	vk::SubmitInfo info;
	std::vector<vk::CommandBuffer> buffers;
	std::unique_ptr<CommandExecutionState*> state;
};

//lock typedef for easier lock_guard using
using LockGuard = std::lock_guard<std::mutex>;

//Fence
Fence::Fence(const Device& dev) : Fence(dev, {})
{
}

Fence::Fence(const Device& dev, const vk::FenceCreateInfo& info) : Resource(dev)
{
	fence_ = vk::createFence(device(), info);
}

Fence::~Fence()
{
	if(fence_) vk::destroyFence(device(), fence_);
}

void swap(Fence& a, Fence& b) noexcept
{
	std::swap(a.fence_, b.fence_);
	std::swap(a.resourceBase(), b.resourceBase());
}

//ExecutionState
CommandExecutionState::CommandExecutionState(const Device& dev, CommandExecutionState** ptr)
	: Resource(dev), self_(ptr)
{
	*self_ = this;
}

CommandExecutionState::~CommandExecutionState()
{
	if(self_) self_ = nullptr;
}

CommandExecutionState::CommandExecutionState(CommandExecutionState&& other) noexcept
	: Resource(other), fence_(std::move(other.fence_)), self_(other.self_)
{
	if(self_) *self_ = this;
	other.self_ = nullptr;
}

CommandExecutionState& CommandExecutionState::operator=(CommandExecutionState&& other) noexcept
{
	Resource::init(other.device());
	if(self_) *self_ = nullptr;

	fence_ = std::move(other.fence_);
	self_ = other.self_;
	if(self_) *self_ = this;
	other.self_ = nullptr;
}

void CommandExecutionState::submit()
{
	if(submitted()) return;
	device().submitManager().submit(*this);
}

void CommandExecutionState::wait(std::uint64_t timeout)
{
	if(completed()) return;
	submit();
	vk::waitForFences(vkDevice(), 1, *fence_, 0, timeout);
}

bool CommandExecutionState::submitted() const
{
	return (fence_.get());
}

bool CommandExecutionState::completed() const
{
	if(!submitted()) return false;

	auto result = vk::getFenceStatus(vkDevice(), *fence_);
	return (result == vk::Result::success);
}

//SubmitManager
SubmitManager::SubmitManager(const Device& dev) : Resource(dev)
{
}

SubmitManager::~SubmitManager()
{
}

void SubmitManager::submit()
{
	while(!submissions_.empty())
		submit(submissions_.begin()->first);
}

void SubmitManager::submit(vk::Queue queue)
{
	//lock own mutex and mutex of all queues
	LockGuard lock(mutex_);

	auto it = submissions_.find(queue);
	if(it == submissions_.end()) return;

	std::vector<vk::SubmitInfo> submitInfos;
	submitInfos.reserve(it->second.size());

	bool createFence = false;
	for(auto& submission : it->second)
	{
		if(submission.state && *submission.state) createFence = true;
		submitInfos.push_back(submission.info);
	}

	std::shared_ptr<Fence> fence;
	vk::FenceCreateInfo fenceInfo {};

	{
		auto&& lock = acquire();
		if(createFence)
		{
			fence = std::make_shared<Fence>(device(), fenceInfo);
			vk::queueSubmit(queue, submitInfos, *fence);
		}
		else
		{
			vk::queueSubmit(queue, submitInfos);
		}
	}

	if(createFence)
	{
		for(auto& submission : it->second)
		{
			if(submission.state && *submission.state)
				(*submission.state)->fence_ = fence;
		}
	}

	submissions_.erase(it);
}

void SubmitManager::add(vk::Queue queue, const vk::SubmitInfo& info, CommandExecutionState* state)
{
	Submission submission;
	submission.info = info;
	if(state)
	{
		submission.state = std::make_unique<CommandExecutionState*>();
		*state = {device(), submission.state.get()};
	}

	LockGuard lock(mutex_);
	submissions_[queue].emplace_back(std::move(submission));
}

void SubmitManager::add(vk::Queue queue, const std::vector<vk::CommandBuffer>& bufs,
	CommandExecutionState* state)
{
	Submission submission;
	submission.buffers = bufs;
	if(state)
	{
		submission.state = std::make_unique<CommandExecutionState*>();
		*state = {device(), submission.state.get()};
	}

	vk::SubmitInfo info;
	info.commandBufferCount = submission.buffers.size();
	info.pCommandBuffers = submission.buffers.data();

	submission.info = info;

	LockGuard lock(mutex_);
	submissions_[queue].emplace_back(std::move(submission));
}

void SubmitManager::add(vk::Queue queue, vk::CommandBuffer buffer, CommandExecutionState* state)
{
	Submission submission;
	submission.buffers = {buffer};
	if(state)
	{
		submission.state = std::make_unique<CommandExecutionState*>();
		*state = {device(), submission.state.get()};
	}

	vk::SubmitInfo info;
	info.commandBufferCount = submission.buffers.size();
	info.pCommandBuffers = submission.buffers.data();

	submission.info = info;

	LockGuard lock(mutex_);
	submissions_[queue].emplace_back(std::move(submission));
}

bool SubmitManager::submit(const CommandExecutionState& id)
{
	std::unique_lock<std::mutex> lock(mutex_);

	for(auto& ent : submissions_)
	{
		auto it = std::find_if(ent.second.begin(), ent.second.end(),
			[&](const Submission& subm) { return(subm.state && *subm.state == &id); });

		if(it != ent.second.end())
		{
			lock.unlock(); //the next submit call will lock it again.
			submit(ent.first);
			return true;
		}
	}

#ifndef NDEBUG
	std::cerr << "vpp::SubmitManager::submit: could not find the given commandSubmission\n";
#endif

	return false;
}

SubmitManager::Lock SubmitManager::acquire() const
{
	return {device()};
}

//Lock
SubmitManager::Lock::Lock(const Device& dev) : Resource(dev)
{
	for(auto& q : device().queues()) q->mutex().lock();
}

SubmitManager::Lock::~Lock()
{
	for(auto& q : device().queues()) q->mutex().unlock();
}

}
