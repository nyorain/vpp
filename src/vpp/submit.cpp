#include <vpp/submit.hpp>
#include <algorithm>

namespace vpp
{

using Lock = std::lock_guard<std::mutex>;

//CommandSubmission
CommandSubmission::~CommandSubmission()
{
	if(fence) vk::destroyFence(vkDevice(), fence, nullptr);
}

//ExecutionState
void CommandExecutionState::submit()
{
	device().submitManager().submit(submission_);
}

void CommandExecutionState::wait(std::uint64_t timeout)
{
	if(submission_->fence) vk::waitForFences(vkDevice(), 1, &submission_->fence, 0, timeout);
	//else warning?
}

bool CommandExecutionState::submitted() const
{
	if(!submission_->fence) return false;
	if(submission_->completed) return true;

	auto result = vk::getFenceStatus(vkDevice(), submission_->fence);
	if(result == vk::Result::Success) submission_->completed = true;

	return submission_->completed;
}

//SubmitManager
void SubmitManager::submit()
{
	while(!submissions_.empty())
		submit(submissions_.begin()->first);
}

vk::Fence SubmitManager::submit(vk::Queue queue)
{
	Lock lock(mutex_);

	auto it = submissions_.find(queue);
	if(it == submissions_.end()) return {};

	std::vector<vk::SubmitInfo> submitInfos;
	submitInfos.reserve(it->second.size());

	for(auto& submission : it->second)
		submitInfos.push_back(submission->info);

	vk::Fence fence;
	vk::FenceCreateInfo fenceInfo {};
	vk::createFence(vkDevice(), &fenceInfo, nullptr, &fence);
	vk::queueSubmit(queue, submitInfos.size(), submitInfos.data(), fence);

	for(auto& submission : it->second)
		submission->fence = fence;

	submissions_.erase(it);
}

CommandExecutionState SubmitManager::add(vk::Queue queue, const vk::SubmitInfo& info)
{
	Lock lock(mutex_);

	auto submission = std::make_shared<CommandSubmission>();
	submission->info = info;
	auto ret = CommandExecutionState(submission);
	submissions_[queue].push_back(std::move(submission));
	return ret;
}

void SubmitManager::submit(const CommandSubmissionPtr& ptr)
{
	Lock lock(mutex_);

	for(auto entry : submissions_)
	{
		auto it = std::find(entry.second.begin(), entry.second.end(), ptr);
		if(it != entry.second.end())
		{
			submit((*it)->queue);
			return;
		}
	}

	//todo: warn?
}

}
