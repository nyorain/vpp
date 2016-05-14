#pragma once

#include <vpp/fwd.hpp>
#include <vpp/device.hpp>
#include <vpp/resource.hpp>

#include <map>
#include <vector>

namespace vpp
{

///Can be used to track the state of a queued command buffer or to submit it to the device.
class ExecutionState : public Resource
{
public:
	void submit();
	void wait();
	bool submitted() const { return submitted_; }

protected:
	bool submitted_ = false;
	vk::Queue queue_; //queue the command buffer is submitted to
};

///Class that manages all commands submitted to the gpu.
///In vulkan, submitting work to the device is a pretty heavy operation and must be synchronized
///(i.e. there should always only be one thread calling vkQueueSubmit no matter on which queue).
///This class threadsafely manages this submissions and also batches mulitple command buffers
///together which will increase performance.
///There is always only one SubmitManager for a vulkan device and if vkQueueSumit is called
///maually, it must be assured that no other thread calls this function or uses the submitManager
///for the same device at the same moment.
class SubmitManager : public Resource
{
public:
	///Submits all CommandBuffers in the submission queue.
	///To wait for their completion, one can simply wait for the device to become idle.
	void submit();

	///Submits all command buffers waiting for submission for the given queue.
	///Returns the associated vulkan fence.
	vk::Fence submit(vk::Queue queue);

	///Adds a given vulkan submit info for exection of a commandBuffer on the given queue.
	///Note that this function does NOT directly submits the given info. It will wait until there
	///are many submissions batched together or a submit member function is called.
	ExecutionState add(vk::Queue, vk::SubmitInfo info);

protected:
	std::size_t autoSubmitThreshold_;
	std::map<vk::Queue, std::vector<vk::SubmitInfo>> submissions_;
};
}
