// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#include <vpp/work.hpp>
#include <iterator>
#include <algorithm>

namespace vpp {

WorkManager::~WorkManager()
{
	finish();
}

void WorkManager::add(std::unique_ptr<WorkBase>&& work)
{
	if(!work) return;
	todo_.push_back(std::move(work));
}

void WorkManager::add(std::vector<std::unique_ptr<WorkBase>>&& work)
{
	using std::make_move_iterator;

	std::remove(work.begin(), work.end(), nullptr);
	todo_.insert(todo_.end(), make_move_iterator(work.begin()), make_move_iterator(work.end()));
}

void WorkManager::add(WorkManager&& other)
{
	add(std::move(other.todo_));
}

void WorkManager::submit()
{
	for(auto& work : todo_)
		if(work && !work->submitted())
			work->submit();
}

void WorkManager::finish()
{
	for(auto& work : todo_)
		if(work && !work->finished())
			work->finish();

	todo_.clear();
}

} // namespace vpp
