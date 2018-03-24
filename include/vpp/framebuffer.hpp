// Copyright (c) 2016-2018 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vpp/fwd.hpp>
#include <vpp/resource.hpp>

namespace vpp {

/// Framebuffer wrapper. Stores no additional information.
class Framebuffer : public ResourceHandle<vk::Framebuffer> {
public:
	Framebuffer() = default;
	Framebuffer(const Device&, const vk::FramebufferCreateInfo& info);
	Framebuffer(const Device&, vk::Framebuffer);
	~Framebuffer();

	Framebuffer(Framebuffer&& rhs) noexcept { swap(*this, rhs); }
	Framebuffer& operator=(Framebuffer rhs) noexcept {
		swap(*this, rhs);
		return *this;
	}
};

} // namespace vpp
