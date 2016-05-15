#pragma once

void CommandWork::submit()
{
	if(static_cast<unsigned int>(state_) > static_cast<unsigned int>(WorkBase::State::pending))
		return;

	executionState_.submit();
	state_ = WorkBase::State::submitted;
}

void CommandWork::wait()
{
	if(static_cast<unsigned int>(state_) > static_cast<unsigned int>(WorkBase::State::submitted))
		return;

	submit();
	state_.wait();
	state_ = WorkBase::State::executed;
}

void CommandWork::finish()
{
	if(static_cast<unsigned int>(state_) > static_cast<unsigned int>(WorkBase::State::executed))
		return;

	wait();
	state_ = WorkBase::State::finished;
}

void CommandWork::queue()
{
	vk::SubmitInfo submitInfo;
	submitInfo.commandBufferCount(1);
	submitInfo.pCommandBuffers(&cmdBuffer_.vkCommandBuffer());

	auto queue = cmdBuffer_.device().queue(cmdBuffer_.commandPool().queueFamily());
	if(!queue) throw std::logic_error("dummy1");

	executionState_ = cmdBuffer_.device().submitManager().add(queue->queue, submitInfo);
	state_ = WorkBase::State::pending;
}
