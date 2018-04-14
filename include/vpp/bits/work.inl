// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

void commandWork_assert(bool, const char* msg);
void commandWork_updateMoved(QueueSubmitter*, uint64_t, unsigned int,
	const vk::CommandBuffer&);

template<typename R>
CommandWork<R>::CommandWork(QueueSubmitter& submitter, CommandBuffer&& cmd) :
	cmdBuffer_(std::move(cmd))
{
	commandWork_assert(cmdBuffer_, "invalid commandBuffer passed");
	init(submitter, {{}, {}, {}, 1, &cmdBuffer_.vkHandle(), {}, {}});
}

template<typename R>
CommandWork<R>::CommandWork(QueueSubmitter& submitter, const vk::SubmitInfo& info)
{
	init(submitter, info);
}

template<typename R>
CommandWork<R>::~CommandWork()
{
	if(submitter_) {
		tryFinish(*this, "~CommandWork");
	}
}

template<typename R>
void CommandWork<R>::init(QueueSubmitter& submitter, const vk::SubmitInfo& info)
{
	commandWork_assert(state_ == WorkBase::State::none,
		"already initialized");

	submitter_ = &submitter;
	submitID_ = submitter.add(info, &infoID_);
	state_ = WorkBase::State::pending;
}

template<typename R>
void CommandWork<R>::submit()
{
	commandWork_assert(state_ != WorkBase::State::none && submitter_ && submitID_,
		"submit: invalid");

	if(this->pending()) {
		submitter_->submit(submitID_);
		state_ = WorkBase::State::submitted;
	}
}

template<typename R>
void CommandWork<R>::wait()
{
	commandWork_assert(state_ != WorkBase::State::none && submitter_ && submitID_,
		"wait: invalid");

	if(!this->executed()) {
		submitter_->wait(submitID_); // will automatically submit if needed
	}

	cmdBuffer_ = {}; // free the commandBuffer it is no longer needed
	state_ = WorkBase::State::finished;
}

template<typename R>
void CommandWork<R>::finish()
{
	wait();
}

template<typename R>
WorkBase::State CommandWork<R>::state()
{
	if(state_ == WorkBase::State::none) {
		return state_;
	}

	// query state
	if(state_ == WorkBase::State::pending && submitter_->submitted(submitID_)) {
		state_ = WorkBase::State::submitted;
	}

	if(state_ == WorkBase::State::submitted && submitter_->completed(submitID_)) {
		state_ = WorkBase::State::finished;
	}

	return state_;
}

template<typename R>
CommandWork<R>::CommandWork(CommandWork&& rhs) noexcept :
	cmdBuffer_(std::move(rhs.cmdBuffer_)), submitter_(rhs.submitter_),
	submitID_(rhs.submitID_), infoID_(rhs.infoID_), state_(rhs.state_)
{
	rhs.submitter_ = {};
	rhs.submitID_ = {};
	rhs.state_ = WorkBase::State::none;
	rhs.infoID_ = {};

	commandWork_updateMoved(submitter_, submitID_, infoID_, cmdBuffer_.vkHandle());
}

template<typename R>
CommandWork<R>& CommandWork<R>::operator=(CommandWork&& rhs) noexcept
{
	if(submitter_) {
		tryFinish(*this, "CommandWork::operator=");
	}

	cmdBuffer_ = std::move(rhs.cmdBuffer_);
	submitter_ = rhs.submitter_;
	submitID_ = rhs.submitID_;
	infoID_ = rhs.infoID_;
	state_ = rhs.state_;

	rhs.submitter_ = {};
	rhs.submitID_ = {};
	rhs.infoID_ = {};
	rhs.state_ = WorkBase::State::none;

	commandWork_updateMoved(submitter_, submitID_, infoID_, cmdBuffer_.vkHandle());

	return *this;
}
