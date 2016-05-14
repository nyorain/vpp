#include <vpp/submit.hpp>

namespace vpp
{

using Lock = std::lock_guard<std::mutex>;

//ExecutionState
void CommandExecutionState::submit()
{
	device().submitManager().submit(*submission_);
}

void CommandExecutionState::wait(std::uint64_t timeout)
{
	if(submission_->fence) vk::waitForFences(vkDevice(), 1, &submission_->fence, 0, timeout);
	//else warning?
}

bool CommandExecutionState::submitted() const
{

}


//SubmitManager
void SubmitManager::submit()
{
	while(!submissions_.empty())
		submit(submissions_.begin()->first);
}

vk::Fence SubmitManager::submit(vk::Queue queue)
{
	Lock l(mutex_);

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
	auto submission = std::make_shared<CommandSubmission>();
	submission->info = info;
	auto ret = CommandExecutionState(submission);
	submissions_[queue].push_back(std::move(submission));
	return ret;
}

}
