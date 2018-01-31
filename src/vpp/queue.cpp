// Copyright (c) 2016-2018 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#include <vpp/queue.hpp>
#include <vpp/device.hpp>
#include <dlg/dlg.hpp>

namespace vpp {

// Queue
Queue::Queue(const Device& dev, vk::Queue queue, unsigned int fam, unsigned int id)
	: Resource(dev), queue_(queue), family_(fam), id_(id)
{
	properties_ = &device().queueFamilyProperties(family_);
}

// QueueManager::Lock
// If constructed with queue, it only locks this specific queue and
// therefore only locks the shared queue mutex shared.
// If only the shared mutex is given, it is constructed as total lock and therefore
// locks the shared mutex totally
QueueLock::QueueLock(const Device& dev)
	: sharedMutex_(dev.sharedQueueMutex())
{
	sharedMutex_.lock();
}

QueueLock::QueueLock(const Device& dev, const vpp::Queue& queue)
	: queueMutex_(&queue.mutex()), sharedMutex_(dev.sharedQueueMutex())
{
	queueMutex_->lock();
	sharedMutex_.lock_shared();
	dlg_assertm(&queue.device() == &dev, "QueueLock: Invalid queue given");
}

QueueLock::~QueueLock()
{
	if(queueMutex_) {
		sharedMutex_.unlock_shared();
		queueMutex_->unlock();
	} else {
		sharedMutex_.unlock();
	}
}

} // namespace vpp
