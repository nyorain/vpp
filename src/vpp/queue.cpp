// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#include <vpp/queue.hpp>

namespace vpp {

Queue::Queue(vk::Queue queue, const vk::QueueFamilyProperties& props,
	unsigned int fam, unsigned int id)
		: queue_(queue), properties_(props), family_(fam), id_(id) {}

} // namespace vpp
