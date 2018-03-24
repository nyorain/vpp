// Copyright (c) 2016-2018 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#include <vpp/framebuffer.hpp>
#include <vpp/vk.hpp>

namespace vpp {

Framebuffer::Framebuffer(const Device& dev, vk::Framebuffer framebuffer)
		: ResourceHandle(dev, framebuffer) {
}

Framebuffer::Framebuffer(const Device& dev, const vk::FramebufferCreateInfo& i)
		: ResourceHandle(dev, vk::createFramebuffer(dev, i)) {
}

Framebuffer::~Framebuffer() {
	if(vkHandle()) {
		vk::destroyFramebuffer(vkDevice(), vkHandle(), nullptr);
	}
}

} // namespace vpp
