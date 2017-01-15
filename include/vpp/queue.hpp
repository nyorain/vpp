// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vpp/fwd.hpp>
#include <vpp/util/nonCopyable.hpp>

#include <mutex> // std::mutex

namespace vpp {

/// Represents a vulkan device queue.
/// Cannot be created or destroyed, must be received by the device class.
class Queue : public nytl::NonCopyable {
public:
	/// Return the queueFamily of this queue
	unsigned int family() const { return family_; }

	/// Returns the id of this queue which is unique under all queues with the same family.
	/// E.g. if there are two queues of family A and one queue of family B, the queues of family
	/// A wiill have the ids {0, 1} while the queue of family B will have the id 0.
	unsigned int id() const { return id_; }

	/// Returns the properties of the queue family of this queue.
	const vk::QueueFamilyProperties& properties() const { return properties_; }

	/// The queue must be locked before performing any operations (such as presenting or sparse
	/// binding) on the queue.
	/// Note that locking the queues mutex is not enough for submitting command buffers
	/// to the queue, since while submitting, no operation on any other queue is allowed.
	/// To submit command buffers to a queue, use the SubmitManager class.
	/// \sa SubmitManager
	std::mutex& mutex() const { return mutex_; }

	[[deprecated("Use vkHandle instead")]]
	vk::Queue vkQueue() const { return queue_; }

	vk::Queue vkHandle() const { return queue_; }
	operator vk::Queue() const { return queue_; }

protected:
	friend class Device;
	Queue(vk::Queue queue, const vk::QueueFamilyProperties& props, unsigned int fam, unsigned int id);
	~Queue() = default;

protected:
	vk::Queue queue_;
	const vk::QueueFamilyProperties& properties_;
	unsigned int family_;
	unsigned int id_;
	mutable std::mutex mutex_;
};

} // namespace vpp
