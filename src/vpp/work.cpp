#include <vpp/work.hpp>

namespace vpp
{

WorkManager::~WorkManager()
{
	finish();
}

void WorkManager::add(std::unique_ptr<WorkBase> work)
{
	todo_.push_back(std::move(work));
}

void WorkManager::submit()
{
	for(auto& work : todo_) if(!work->submitted()) work->submit();
}

void WorkManager::finish()
{
	for(auto& work : todo_) if(!work->finished()) work->finish();
	todo_.clear();
}

}
