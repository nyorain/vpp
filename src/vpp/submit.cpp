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
	std::cout << "start\n";
	if(completed()) return;
	submit();
	vk::waitForFences(vkDevice(), 1, submission_->fence, 0, timeout);
	std::cout << "end\n";
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
	Lock lock(mutex_);

	auto submission = std::make_shared<CommandSubmission>(device());
	submission->info = info;
	submission->queue = queue;
	submissions_[queue].push_back(submission);

	return {submission};
}

CommandExecutionState SubmitManager::add(vk::Queue queue, const std::vector<vk::CommandBuffer>& buffer)
{
	static std::vector<std::vector<vk::CommandBuffer>> buffers;
	if(buffers.size() > 100) buffers.erase(buffers.begin(), buffers.begin() + 50);

	buffers.push_back(buffer);

	vk::SubmitInfo info;
	info.commandBufferCount = buffer.size();
	info.pCommandBuffers = buffers.back().data();
	return add(queue, info);
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
