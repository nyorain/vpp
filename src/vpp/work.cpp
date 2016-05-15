#include <vpp/work.hpp>

namespace vpp
{

WorkManager::WorkManager(const Device& dev, std::size_t submitThreshold) : Resource(dev)
{
}

void WorkManager::add(std::unique_ptr<WorkBase> work)
{
	todo_.push_back(std::move(work));
	if(todo_.size() > submitThreshold_) submit();
}

void WorkManager::submit()
{
	for(auto& work : todo_) if(!work->submitted()) work->submit();
}

void WorkManager::finish()
{
	submit();
	for(auto& work : todo_) if(!work->finished()) work->finish();
	todo_.clear();
}

}
