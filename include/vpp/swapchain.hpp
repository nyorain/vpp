// Copyright (c) 2017 nyorain
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

/// SwapchainSettings is a virtual base class that can be used to implement a settings
/// chose mechanism for Swapchain creation. An extra virtual class is needed instead of a
/// struct because there a various settings and their availability must be queried.
/// The default implementation just choses the values which are considered best from the
/// available ones.
class SwapchainSettings {
public:
	/// Returns a static instance of the default implementation.
	/// Since the default implementation is stateless the returned object can
	/// also safely be used from multiple threads.
	static const SwapchainSettings& instance();

public:
	virtual ~SwapchainSettings() = default;
	
	/// This function will be called by the Swapchain this object was passed to.
	/// The returned SwapchainCreateInfo will be used to create the vulkan swapchain.
	/// \param caps The queried capabilities for the surface the swapChain will be created for.
	/// \param modes The queried available present modes.
	/// \param formats The queried available formats.
	/// \param size The preferred size passed as parameter to the Swapchain.
	/// On most platforms the size is chosen by the surface (from the native surface),
	/// and this paramter can be ignored.
	virtual vk::SwapchainCreateInfoKHR parse(const vk::SurfaceCapabilitiesKHR& caps,
		nytl::Span<const vk::PresentModeKHR> modes,
		nytl::Span<const vk::SurfaceFormatKHR> formats,
		const vk::Extent2D& size) const;
};

/// A SwapchainSettings implementation that allows to chose some preferred values for
/// multiple fields. It can also be specified what should be done if one of those values is
/// not available.
/// After Swapchain creation the members will be set to the values that were chosen in the end
/// for the Swapchain.
class DefaultSwapchainSettings final : public SwapchainSettings {
public:
	/// What should be done if a preferred setting is not available.
	enum class ErrorAction {
		none, /// Simply choses another setting.
		output, /// Choses another setting and prints a warning to cerr.
		throwException /// Throws a std::runtime_error. Does not change a setting.
	};

public:
	ErrorAction errorAction {};

	mutable vk::Format prefFormat; // = vk::Format::r8g8b8a8Unorm;
	mutable vk::PresentModeKHR prefPresentMode; // = vk::PresentModeKHR::mailbox;
	mutable vk::CompositeAlphaBitsKHR prefAlpha; // = vk::CompositeAlphaBitsKHR::opaque;
	mutable vk::SurfaceTransformBitsKHR prefTransform; // = vk::SurfaceTransformBitsKHR::identity;
	mutable vk::ImageUsageFlags prefUsage; // = {};

public:
	DefaultSwapchainSettings();
	vk::SwapchainCreateInfoKHR parse(const vk::SurfaceCapabilitiesKHR& caps,
		nytl::Span<const vk::PresentModeKHR> modes,
		nytl::Span<const vk::SurfaceFormatKHR> formats,
		const vk::Extent2D& size) const override;
};

/// Swapchain wrapper that (together with SwapchainSettings and its
/// implementations) manages surface property querying and swapchain setup.
/// Does not remember/store any additional information about itself.
class Swapchain : public ResourceHandle<vk::SwapchainKHR> {
public:
	Swapchain() = default;

	/// Constructs the swap chain for a given vulkan surface.
	/// Note that the given size is only used if the backend does not provide
	/// a surface size (e.g. wayland backend).
	/// The real size of the swapchain will be returned in size, might
	/// differ from the passed one (but a valid value must be passed).
	/// If the given views vector pointer is not nullptr, will retrieve
	/// the new swapchains images, create imageViews and push them into it.
	Swapchain(const Device& device, vk::SurfaceKHR surface,
		vk::Extent2D& size, const SwapchainSettings& = {},
		std::vector<std::pair<vk::Image, ImageView>>* views = {});

	/// Transfers ownership of the given swapChain handle to the created object.
	Swapchain(const Device& dev, vk::SwapchainKHR swapchain);
	~Swapchain();

	Swapchain(Swapchain&& rhs) noexcept { swap(*this, rhs); }
	Swapchain& operator=(Swapchain&& rhs) noexcept { swap(*this, rhs); return *this; }

	/// Resizes the swapchain to the given size. 
	/// Should be called if the native window of the underlying surface 
	/// handle changes it size to make sure the swapchain fills the
	/// whole window.
	/// Will destroy the previous vk::SwapchainKHR handle.
	/// The real size of the swapchain will be returned in size, might
	/// differ from the passed one (but a valid value must be passed).
	/// If the given views vector pointer is not nullptr, will retrieve
	/// the new swapchains images, create imageViews and push them into it.
	/// Can be called on an invalid (empty, default-constructed or moved-from)
	/// swapchain handle to initialize it.
	void resize(vk::SurfaceKHR, vk::Extent2D& size, 
		const SwapchainSettings& = {},
		std::vector<std::pair<vk::Image, ImageView>>* views = {});

	/// Wrapper for vkAcquireNextImageKHR, will simply forward the result.
	vk::Result acquire(std::uint32_t& id, vk::Semaphore sem,
		vk::Fence fence = {}, std::uint64_t timeout = ~0u) const;

	/// Wrapper for vkQueuePresentKHR, will simply forward the result.
	vk::Result present(const Queue& queue, unsigned int image, 
		vk::Semaphore wait = {}) const;

	/// Wrapper around vkGetSwapchainImagesKHR
	std::vector<vk::Image> images() const;
};

} // namespace vpp
