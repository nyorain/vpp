#pragma once

#include <vpp/fwd.hpp>

namespace vpp
{

///Represents a vulkan device queue.
///Cannot be created or destroyed, must be received by the device class.
class Queue
{
public:
	///Return the queueFamily of this queue
	unsigned int family() const { return family_; }

	///Returns the id of this queue which is unique under all queues with the same family.
	///E.g. if there are two queues of family A and one queue of family B, the queues of family
	///A wiill have the ids {0, 1} while the queue of family B will have the id 0.
	unsigned int id() const { return id_; }

	///Returns the properties of the queue family of this queue.
	const vk::QueueFamilyProperties& properties() const { return properties_; }

	///Returns the vulkan queue handle.
	vk::Queue vkQueue() const { return queue_; }

protected:
	friend Device;
	Queue(vk::Queue queue, const vk::QueueFamilyProperties& prop, unsigned int family, unsigned int id);
	~Queue() = default;

protected:
	vk::Queue queue_;
	const vk::QueueFamilyProperties& properties_;
	unsigned int family_;
	unsigned int id_;
};

}
