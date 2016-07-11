#include <vpp/queue.hpp>
#include <iostream>

namespace vpp
{

Queue::Queue(vk::Queue queue, const vk::QueueFamilyProperties& props, unsigned int fam, unsigned int id)
	: queue_(queue), properties_(props), family_(fam), id_(id)
{
}

}
