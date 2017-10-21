// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

template<typename R>
CommandWork<R>::CommandWork(QueueSubmitter& submitter, CommandBuffer&& cmd) :
	cmdBuffer_(std::move(cmd))
{
	init(submitter, {{}, {}, {}, 1, &cmdBuffer_.vkHandle(), {}, {}});
}

template<typename R>
CommandWork<R>::CommandWork(QueueSubmitter& submitter, 
	const vk::SubmitInfo& info, CommandBuffer&& cmd) :
		cmdBuffer_(std::move(cmd))
{
	init(submitter, info);
}

template<typename R>
CommandWork<R>::~CommandWork()
{
	tryFinish(*this, "~CommandWork");
}

template<typename R>
void CommandWork<R>::init(QueueSubmitter& submitter, const vk::SubmitInfo& info)
{
	submitter_ = &submitter;
	submitID_ = submitter.add(info);
	state_ = WorkBase::State::pending;
}

template<typename R>
void CommandWork<R>::submit()
{
	// dlg_assert(state_ != WorkBase::State::none);
	// dlg_assert(submitter_ && submitID_);

	submitter_->submit(submitID_);
	state_ = WorkBase::State::submitted;
}

template<typename R>
void CommandWork<R>::wait()
{
	// dlg_assert(state_ != WorkBase::State::none);
	// dlg_assert(submitter_ && submitID_);

	submitter_->wait(submitID_); // will automatically submit if needed
	cmdBuffer_ = {}; // free the commandBuffer it is no longer needed
	state_ = WorkBase::State::executed;
}

template<typename R>
WorkBase::State CommandWork<R>::state()
{
	if(state_ == WorkBase::State::none) {
		return state_;
	}

	// query state
	// dlg_assert(submitID_);
	// dlg_assert(submitter_);

	if(state_ == WorkBase::State::pending && submitter_->submitted(submitID_)) {
		state_ = WorkBase::State::submitted;
	}

	if(state_ == WorkBase::State::submitted && submitter_->completed(submitID_)) {
		state_ = WorkBase::State::executed;
	}

	return state_;
}
