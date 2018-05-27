// Copyright (c) 2016-2018 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vpp/fwd.hpp>
#include <vpp/resource.hpp>
#include <vpp/util/span.hpp>

#include <vector>
#include <string>
#include <memory>

namespace vpp {

/// Preferences for creating a swapchain.
/// Can be used to query a valid SwapchainCreateInfo.
struct SwapchainPreferences {
	enum class ErrorAction {
		none, /// Simply chooses another setting.
		output, /// Choses another setting and prints a warning to cerr.
		exception /// Throws a std::runtime_error
	};

	SwapchainPreferences();

	ErrorAction errorAction {};
	bool preferCurrentExtent {true}; // ignore the size parameter if possible
	bool preferSrgb {true}; // choose srgb over unorm formats
	unsigned minImageCount {2};

	vk::Format format; // = vk::Format::b8g8r8a8, depends on preferSrgb.
	vk::PresentModeKHR presentMode; // = vk::PresentModeKHR::mailbox;
	vk::CompositeAlphaBitsKHR alpha; // = vk::CompositeAlphaBitsKHR::opaque;
	vk::SurfaceTransformBitsKHR transform; // = vk::SurfaceTransformBitsKHR::identity;
	vk::ImageUsageFlags usage; // = vk::ImageUsageBits::colorAttachment;
};

/// Parses the given SwapchainPreferences for the given device and surface
/// into a valid SwapchainCreateInfo. This has to be done only once,
/// the returned info can be reused when a swapchain is resized.
vk::SwapchainCreateInfoKHR swapchainCreateInfo(const vpp::Device&,
	vk::SurfaceKHR, const vk::Extent2D& size,
	const SwapchainPreferences& prefs = {});

/// Swapchain wrapper that (together with SwapchainSettings and its
/// implementations) manages surface property querying and swapchain setup.
/// Does not remember/store any additional information about itself.
class Swapchain : public ResourceHandle<vk::SwapchainKHR> {
public:
	Swapchain() = default;
	Swapchain(const Device& device, const vk::SwapchainCreateInfoKHR&);
	Swapchain(const Device& dev, vk::SwapchainKHR swapchain);
	~Swapchain();

	Swapchain(Swapchain&& rhs) noexcept { swap(*this, rhs); }
	Swapchain& operator=(Swapchain rhs) noexcept {
		swap(*this, rhs);
		return *this;
	}

	/// Resizes the swapchain to the given size.
	/// Should be called if the native window of the underlying surface
	/// handle changes it size to make sure the swapchain fills the
	/// whole window.
	/// Will automatically update the given size (or currentExtent if valid
	/// and prefCurrentExtent true) and the oldSwapchain member of the
	/// given info.
	/// Will return the really used size.
	void resize(const vk::Extent2D& size, vk::SwapchainCreateInfoKHR&);

	/// Wrapper for vkAcquireNextImageKHR, will simply forward the result.
	vk::Result acquire(std::uint32_t& id, vk::Semaphore sem,
		vk::Fence fence = {}, std::uint64_t timeout = UINT64_MAX) const;

	/// Wrapper for vkQueuePresentKHR, will simply forward the result.
	vk::Result present(const Queue& queue, unsigned int image,
		vk::Semaphore wait = {}) const;

	/// Wrapper around vkGetSwapchainImagesKHR
	std::vector<vk::Image> images() const;
};

} // namespace vpp
