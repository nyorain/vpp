// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#include <vpp/framebuffer.hpp>
#include <vpp/vk.hpp>

namespace vpp {

Framebuffer::Framebuffer(const Device& dev, vk::Framebuffer framebuffer)
	: ResourceHandle(dev, framebuffer)
{
}

Framebuffer::Framebuffer(const Device& dev, const vk::FramebufferCreateInfo& info)
	: ResourceHandle(dev, vk::createFramebuffer(dev, info))
{
}

Framebuffer::~Framebuffer()
{
	if(vkHandle()) {
		vk::destroyFramebuffer(vkDevice(), vkHandle(), nullptr);
	}
}

Framebuffer::Framebuffer(Framebuffer&& rhs) noexcept 
{ 
	swap(*this, rhs); 
}

Framebuffer& Framebuffer::operator=(Framebuffer rhs) noexcept 
{ 
	swap(*this, rhs); 
	return *this; 
}

} // namespace vpp
