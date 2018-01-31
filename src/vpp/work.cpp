// Copyright (c) 2016-2018 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#include <vpp/work.hpp>
#include <vkpp/structs.hpp>
#include <dlg/dlg.hpp>

#include <iterator>
#include <algorithm>

namespace vpp {

WorkManager::~WorkManager()
{
	finish();
}

void WorkManager::addImpl(std::unique_ptr<WorkBase>&& work)
{
	dlg_assert(work);
	todo_.push_back(std::move(work));
}

void WorkManager::add(std::vector<std::unique_ptr<WorkBase>>&& work)
{
	dlg_check({
		for(auto& w : work) {
			dlg_assertm(w, "WorkManager::add: nullptr work");
		}
	});

	using std::make_move_iterator;
	todo_.insert(todo_.end(), 
		make_move_iterator(work.begin()), 
		make_move_iterator(work.end()));
}

void WorkManager::add(WorkManager&& other)
{
	add(std::move(other.todo_));
}

void WorkManager::submit()
{
	for(auto& work : todo_) {
		if(work && !work->submitted()) {
			work->submit();
		}
	}
}

void WorkManager::finish()
{
	for(auto& work : todo_)
		if(work && !work->finished())
			work->finish();

	todo_.clear();
}

// Utility
bool tryFinish(WorkBase& work, const char* msg) noexcept
{
	try {
		work.finish();
	} catch(const std::exception& err) {
		dlg_error("{} tryFinish(work): {}", msg, err.what());
		return false;
	} catch(...) {
		dlg_error("{} tryFinish(work): <no std::exception>", msg);
		return false;
	}

	return true;
}

void commandWork_assert(bool expr, const char* msg)
{
	dlg_assertm(expr, "CommandWork: {}", msg);
}

void commandWork_updateMoved(QueueSubmitter* submitter, uint64_t submitID, 
	unsigned int infoID, const vk::CommandBuffer& cmdBuf)
{
	if(submitter && !submitter->submitted(submitID) && cmdBuf) {
		dlg_assert(submitter->pendingInfos().size() > infoID);
		dlg_assert(submitter->pendingInfos()[infoID].commandBufferCount == 1);
		submitter->pendingInfos()[infoID].pCommandBuffers = &cmdBuf;
	}
}

} // namespace vpp
