#include <vpp/submit.hpp>
#include <vpp/vk.hpp>
#include <algorithm>

namespace vpp
{

///CommandSubmission declaration
class CommandSubmission : public Resource
{
public:
	vk::Queue queue;
	vk::SubmitInfo info;
	vk::Fence fence {};
	bool completed {}; //fence status cache

public:
	using Resource::Resource;
	~CommandSubmission();
};

//lock typedef for easier lock_guard using
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
	if(!submission_->fence) submit();
	vk::waitForFences(vkDevice(), 1, &submission_->fence, 0, timeout);
}

bool CommandExecutionState::submitted() const
{
	return (submission_->fence);
}

bool CommandExecutionState::completed() const
{
	if(!submitted()) return false;
	if(submission_->completed) return true;

	auto result = vk::getFenceStatus(vkDevice(), submission_->fence);
	if(result == vk::Result::success) submission_->completed = true;

	return submission_->completed;
}

const Resource& CommandExecutionState::resourceRef() const
{
	return *submission_;
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

CommandExecutionState SubmitManager::add(vk::Queue queue, const std::vector<vk::CommandBuffer>& buffer)
{
	vk::SubmitInfo info;
	info.commandBufferCount = buffer.size();
	info.pCommandBuffers = buffer.data();
	add(queue, info);
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

	std::cerr << "vpp::SubmitManager::submit: could not find the given commandSubmission\n";
}

}
