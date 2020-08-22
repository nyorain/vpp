// Copyright (c) 2016-2020 Jan Kelling
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vpp/fwd.hpp>
#include <vpp/resource.hpp>

#include <mutex> // std::mutex
#include <shared_mutex> // std::shared_mutex

namespace vpp {

/// Represents a vulkan device queue.
/// Cannot be created or destroyed, must be received by the device class.
/// Provides synchronization mechanisms.
class VPP_API Queue : public Resource {
public:
	/// Return the queueFamily of this queue
	unsigned int family() const noexcept { return family_; }

	/// Returns the id of this queue which is unique under all queues with the
	/// same family. E.g. if there are two queues of family A and one queue of
	/// family B, the queues of family A wiill have the ids {0, 1} while the
	/// queue of family B will have the id 0.
	/// Gives every Queue object (for one device) a unique identification if used
	/// together with the family.
	unsigned int id() const noexcept { return id_; }

	/// Returns the properties of the queue family of this queue.
	const auto& properties() const noexcept { return *properties_; }

	/// The queue must be locked before performing any operations
	/// (such as presenting or sparse binding) on the queue.
	/// Note that locking the queues mutex is not enough for submitting
	/// command buffers to the queue, since while submitting, no operation
	/// on any other queue is allowed.
	/// Prefer to use the vpp::QueueLock class over using the plain mutex.
	std::mutex& mutex() const noexcept { return mutex_; }

	vk::Queue vkHandle() const noexcept { return queue_; }
	operator vk::Queue() const noexcept { return queue_; }

protected:
	Queue() = default;
	~Queue() = default;

	Queue(Queue&&) = delete;
	Queue& operator=(Queue&&) = delete;

	void init(const Device&, vk::Queue, unsigned int fam, unsigned int id);

private:
	vk::Queue queue_;
	const vk::QueueFamilyProperties* properties_;
	unsigned int family_;
	unsigned int id_;
	mutable std::mutex mutex_;
};

/// Acquires ownership over a single or all queues.
/// Used for queue synchronization across threads.
/// The constructor without a specific queue will lock all queues,
/// which is e.g. needed when submitting command buffes.
/// Otherwise just ownership over the given queue is claimed.
/// RAII lock class, i.e. the lock is bound the objects lifetime.
struct VPP_API QueueLock {
	QueueLock(const Device& dev);
	QueueLock(const Device& dev, const vpp::Queue& queue);
	~QueueLock();

	QueueLock(QueueLock&&) = delete;
	QueueLock& operator=(QueueLock&&) = delete;

private:
	std::mutex* queueMutex_ {};
	std::shared_mutex& sharedMutex_;
};

} // namespace vpp
