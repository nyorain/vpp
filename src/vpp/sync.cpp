// Copyright (c) 2016-2018 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#include <vpp/sync.hpp>
#include <vpp/vk.hpp>

namespace vpp {

// Fence
Fence::Fence(const Device& dev) : Fence(dev, vk::FenceCreateInfo {}) {}
Fence::Fence(const Device& dev, vk::Fence fence) : ResourceHandle(dev, fence) {}
Fence::Fence(const Device& dev, const vk::FenceCreateInfo& info) : ResourceHandle(dev)
{
	handle_ = vk::createFence(device(), info);
}

Fence::~Fence()
{
	if(vkHandle()) {
		vk::destroyFence(device(), vkHandle());
	}
}

// Semaphore
Semaphore::Semaphore(const Device& dev) : Semaphore(dev, vk::SemaphoreCreateInfo {}) {}
Semaphore::Semaphore(const Device& dev, vk::Semaphore sp) : ResourceHandle(dev, sp) {}
Semaphore::Semaphore(const Device& dev, const vk::SemaphoreCreateInfo& info) : ResourceHandle(dev)
{
	handle_ = vk::createSemaphore(device(), info);
}

Semaphore::~Semaphore()
{
	if(vkHandle()) {
		vk::destroySemaphore(device(), vkHandle());
	}
}

// Event
Event::Event(const Device& dev) : Event(dev, vk::EventCreateInfo {}) {}
Event::Event(const Device& dev, vk::Event event) : ResourceHandle(dev, event) {}
Event::Event(const Device& dev, const vk::EventCreateInfo& info) : ResourceHandle(dev)
{
	handle_ = vk::createEvent(device(), info);
}

Event::~Event()
{
	if(vkHandle()) {
		vk::destroyEvent(device(), vkHandle());
	}
}

} // namespace vpp
