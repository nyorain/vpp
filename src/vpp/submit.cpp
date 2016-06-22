#include <vpp/submit.hpp>
#include <vpp/vk.hpp>
#include <algorithm>

namespace vpp
{

///CommandSubmission declaration
class CommandSubmission : public Resource
{
public:
	CommandSubmission(const Device& dev) : Resource(dev) {}
	~CommandSubmission();

	vk::Queue queue {};
	vk::SubmitInfo info;
	vk::Fence fence {};
	bool completed {}; //fence status cache
	std::vector<vk::CommandBuffer> buffers {}; //to make the cmdBuffer member of info stay vaid
};

//lock typedef for easier lock_guard using
using Lock = std::lock_guard<std::mutex>;

//CommandSubmission
CommandSubmission::~CommandSubmission()
{
	//if(fence) vk::destroyFence(vkDevice(), fence, nullptr);
}

//ExecutionState
void CommandExecutionState::submit()
{
	if(submitted()) return;
	device().submitManager().submit(submission_);
}

void CommandExecutionState::wait(std::uint64_t timeout)
{
	if(completed()) return;
	submit();
	vk::waitForFences(vkDevice(), 1, submission_->fence, 0, timeout);
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

	vk::FenceCreateInfo fenceInfo {};
	auto fence = vk::createFence(vkDevice(), fenceInfo);
	vk::queueSubmit(queue, submitInfos, fence);

	for(auto& submission : it->second)
		submission->fence = fence;

	submissions_.erase(it);
	return fence;
}

CommandExecutionState SubmitManager::add(vk::Queue queue, const vk::SubmitInfo& info)
{
	auto submission = std::make_shared<CommandSubmission>(device());
	submission->info = info;
	submission->queue = queue;

	Lock lock(mutex_);
	submissions_[queue].push_back(submission);

	return {submission};
}

CommandExecutionState SubmitManager::add(vk::Queue queue, const std::vector<vk::CommandBuffer>& bufs)
{
	auto submission = std::make_shared<CommandSubmission>(device());
	submission->buffers = bufs;

	vk::SubmitInfo info;
	info.commandBufferCount = submission->buffers.size();
	info.pCommandBuffers = submission->buffers.data();

	submission->info = info;
	submission->queue = queue;

	Lock lock(mutex_);
	submissions_[queue].push_back(submission);

	return {submission};
}

CommandExecutionState SubmitManager::add(vk::Queue queue, vk::CommandBuffer buffer)
{
	auto submission = std::make_shared<CommandSubmission>(device());
	submission->buffers = {buffer};

	vk::SubmitInfo info;
	info.commandBufferCount = submission->buffers.size();
	info.pCommandBuffers = submission->buffers.data();

	submission->info = info;
	submission->queue = queue;

	Lock lock(mutex_);
	submissions_[queue].push_back(submission);

	return {submission};
}

void SubmitManager::submit(const CommandSubmissionPtr& ptr)
{
	std::unique_lock<std::mutex> lock(mutex_);

	for(auto entry : submissions_)
	{
		auto it = std::find(entry.second.begin(), entry.second.end(), ptr);
		if(it != entry.second.end())
		{
			lock.unlock(); //the next submit call will lock it again.
			submit((*it)->queue);
			return;
		}
	}

	std::cerr << "vpp::SubmitManager::submit: could not find the given commandSubmission\n";
}

}
