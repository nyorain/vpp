// Copyright (c) 2016-2018 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vpp/fwd.hpp>
#include <vpp/resource.hpp>

namespace vpp {

/// RAII Fence wrapper
class Fence : public ResourceHandle<vk::Fence> {
public:
	Fence() = default;
	Fence(const Device&);
	Fence(const Device&, vk::Fence);
	Fence(const Device&, const vk::FenceCreateInfo&);
	~Fence();

	Fence(Fence&& rhs) noexcept { swap(*this, rhs); }
	Fence& operator=(Fence rhs) noexcept {
		swap(*this, rhs);
		return *this;
	}
};

/// RAII semaphore wrapper
class Semaphore : public ResourceHandle<vk::Semaphore> {
public:
	Semaphore() = default;
	Semaphore(const Device&);
	Semaphore(const Device&, vk::Semaphore);
	Semaphore(const Device&, const vk::SemaphoreCreateInfo&);
	~Semaphore();

	Semaphore(Semaphore&& rhs) noexcept { swap(*this, rhs); }
	Semaphore& operator=(Semaphore rhs) noexcept {
		swap(*this, rhs);
		return *this;
	}
};

/// RAII event wrapper
class Event : public ResourceHandle<vk::Event> {
public:
	Event() = default;
	Event(const Device&);
	Event(const Device&, vk::Event);
	Event(const Device&, const vk::EventCreateInfo&);
	~Event();

	Event(Event&& rhs) noexcept { swap(*this, rhs); }
	Event& operator=(Event rhs) noexcept {
		swap(*this, rhs);
		return *this;
	}
};

/// Combines a semaphore and the stage in which it should be signaled/
/// should be waited upon.
struct StageSemaphore {
	vk::Semaphore semaphore;
	vk::PipelineStageFlags stage;
};

} // namespace vpp
