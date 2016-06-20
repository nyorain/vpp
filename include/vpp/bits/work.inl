#pragma once


template<typename R>
void CommandWork<R>::submit()
{
	std::cout << "sss\n";
	if(Work<R>::submitted()) return;

	std::cout << "queueing...\n";
	queue();
	std::cout << "sumitting...\n";
	executionState_.submit();
	state_ = WorkBase::State::submitted;
}

template<typename R>
void CommandWork<R>::wait()
{
	std::cout << "www\n";
	if(Work<R>::executed()) return;

	submit();
	std::cout << "wait2\n";
	executionState_.wait();
	state_ = WorkBase::State::executed;
}

template<typename R>
void CommandWork<R>::finish()
{
	std::cout << "fff\n";
	if(Work<R>::finished()) return;

	wait();
	cmdBuffer_ = {}; //free the commandBuffer it is no longer needed

	state_ = WorkBase::State::finished;
}

template<typename R>
void CommandWork<R>::queue()
{
	if(executionState_.submission()) return; //was already queued
	std::cout << "queueueueueueing...\n";

	//TODO: correct queues
	auto buf = cmdBuffer_.vkCommandBuffer();
	auto* queue = cmdBuffer_.device().queue(cmdBuffer_.commandPool().queueFamily());
	if(!queue) throw std::logic_error("dummy1");

	executionState_ = cmdBuffer_.device().submitManager().add(queue->vkQueue(), {buf});
	state_ = WorkBase::State::pending;
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
