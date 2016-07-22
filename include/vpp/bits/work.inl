#pragma once

template<typename R>
CommandWork<R>::CommandWork(CommandBuffer&& cmdBuf, vk::Queue queue) : cmdBuffer_(std::move(cmdBuf))
{
	cmdBuffer_.device().submitManager().add(queue, cmdBuffer_, &executionState_);
	state_ = WorkBase::State::pending;
}

template<typename R>
void CommandWork<R>::submit()
{
	if(Work<R>::submitted()) return;

	executionState_.submit();
	state_ = WorkBase::State::submitted;
}

template<typename R>
void CommandWork<R>::wait()
{
	if(Work<R>::executed()) return;

	submit();
	executionState_.wait();
	state_ = WorkBase::State::executed;
}

template<typename R>
void CommandWork<R>::finish()
{
	if(Work<R>::finished()) return;

	wait();
	cmdBuffer_ = {}; //free the commandBuffer it is no longer needed

	state_ = WorkBase::State::finished;
}

template<typename R>
WorkBase::State CommandWork<R>::state()
{
	//update state
	if(state_ == WorkBase::State::pending && executionState_.submitted())
		state_ = WorkBase::State::submitted;

	if(state_ == WorkBase::State::submitted && executionState_.completed())
		state_ = WorkBase::State::executed;

	return state_;
}
