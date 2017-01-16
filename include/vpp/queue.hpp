// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vpp/fwd.hpp>
#include <vpp/util/nonCopyable.hpp> // nytl::NonCopyable

#include <mutex> // std::mutex

namespace vpp {

/// Represents a vulkan device queue.
/// Cannot be created or destroyed, must be received by the device class.
class Queue final : public nytl::NonMovable {
public:
	/// Return the queueFamily of this queue
	unsigned int family() const noexcept { return family_; }

	/// Returns the id of this queue which is unique under all queues with the same family.
	/// E.g. if there are two queues of family A and one queue of family B, the queues of family
	/// A wiill have the ids {0, 1} while the queue of family B will have the id 0.
	/// Gives every Queue object (for one device) a unique identification if used
	/// together with the family.
	unsigned int id() const noexcept { return id_; }

	/// Returns the properties of the queue family of this queue.
	const vk::QueueFamilyProperties& properties() const noexcept { return properties_; }

	/// The queue must be locked before performing any operations (such as presenting or sparse
	/// binding) on the queue.
	/// Note that locking the queues mutex is not enough for submitting command buffers
	/// to the queue, since while submitting, no operation on any other queue is allowed.
	/// To submit command buffers to a queue, use the SubmitManager class.
	/// \sa SubmitManager
	std::mutex& mutex() const noexcept { return mutex_; }

	[[deprecated("Use vkHandle instead")]]
	vk::Queue vkQueue() const noexcept { return queue_; }

	vk::Queue vkHandle() const noexcept { return queue_; }
	operator vk::Queue() const noexcept { return queue_; }

protected:
	friend class Device;
	Queue(vk::Queue, const vk::QueueFamilyProperties&, unsigned int fam, unsigned int id);
	~Queue() = default;

protected:
	const vk::Queue queue_;
	const vk::QueueFamilyProperties& properties_;
	const unsigned int family_;
	const unsigned int id_;
	mutable std::mutex mutex_;
};

} // namespace vpp
