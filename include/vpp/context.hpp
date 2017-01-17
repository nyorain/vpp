// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vpp/fwd.hpp>
#include <vpp/swapChain.hpp>
#include <vpp/surface.hpp>
#include <vpp/debug.hpp>
#include <vpp/vulkan/enums.hpp>
#include <vpp/util/span.hpp>

#include <vector>
#include <string>
#include <memory>

// TODO: sparse queues

namespace vpp {
namespace {
#ifdef VPP_DEBUG
	const auto& contextDefaultInstanceLayers = defaultLayerNames();
 	const std::vector<const char*> contextDefaultDeviceLayers = {};
 	const vk::DebugReportFlagsEXT contextDefaultDebugFlags = DebugCallback::defaultFlags();
#else
 	const std::vector<const char*> contextDefaultInstanceLayers = {};
 	const std::vector<const char*> contextDefaultDeviceLayers = {};
 	const vk::DebugReportFlagsEXT contextDefaultDebugFlags = {};
#endif
}

/// A Vulkan Context. Can be used to easily create Device, SwapChain and set everything
/// up for basic use.
/// The Context will automatically request a present queue for its surface as well as a graphics
/// and compute queue (if possible just one queue for all needs).
/// If more fine-grained control over device, queues and swapChain creation is needed, consider
/// creating them manually.
class Context {
public:
	struct CreateInfo {
		/// These two members are ignored if the backend choses the size of the swapChain.
		unsigned int width {};
		unsigned int height {};

		/// Specifies in which cases the debug callback should output information.
		/// If this is set to an empty value, no callback will be created at all
		vk::DebugReportFlagsEXT debugFlags = contextDefaultDebugFlags;

		/// The needed extensions.
		/// If one of them is not available, an warning will be output and it will
		/// simply not be enabled.
		std::vector<const char*> instanceExtensions {};
		std::vector<const char*> instanceLayers = contextDefaultInstanceLayers;

		/// If this is set to true, all available instance layers except the given ones
		/// will be enabled.
		bool reverseInstanceLayers {};

		/// The needed device extensions.
		/// If one of them is not available, an warning will be output and it will
		/// simply not be enabled.
		std::vector<const char*> deviceExtensions {};
		std::vector<const char*> deviceLayers = contextDefaultDeviceLayers;

		/// If this is set to true, all available device layers except the given ones
		/// will be enabled.
		bool reverseDeviceLayers {};

		/// A SwapChainSettings implementation to tweak swapchain creation.
		const SwapChainSettings& swapChainSettings = SwapChainSettings::instance();
	};

public:
	/// \exception std::runtime_error If graphics queues are not supported.
	/// \exception std::runtime_error If there are no physical devices
	/// \exception vpp::VulkanError If a vulkan functions fails, i.e. if the instance
	/// cannot be created due to an invalid driver.
	Context(vk::Instance ini, vk::SurfaceKHR surface, const CreateInfo& info);
	Context(); // = default
	~Context(); // = default

	Context(Context&& other) noexcept;
	Context& operator=(Context other) noexcept;

	const Surface& surface() const { return surface_; }

	const Device& device() const { return *device_; }
	const SwapChain& swapChain() const { return swapChain_; }

	/// The queue that can be used for graphics and compute operations.
	const Queue& graphicsComputeQueue() const { return *graphicsComputeQueue_; }

	///  The queue that can be used for present operations.
	/// Might be the same as graphicsComputeQueue().
	const Queue& presentQueue() const { return *presentQueue_; }

	SwapChain& swapChain() { return swapChain_; }
	Device& device() { return *device_; }

	vk::Instance vkInstance() const { return instance_; }
	vk::SurfaceKHR vkSurface() const { return surface_.vkSurface(); }
	vk::Device vkDevice() const;
	vk::SwapchainKHR vkSwapChain() const { return swapChain().vkHandle(); }

	/// Following functions can be used to init the context step by step.
	/// Note that these functions should not be called if the associated initialization has
	/// already taken place. Useful to customly tweek some default settings.
	void initInstance(vk::DebugReportFlagsEXT, nytl::Span<const char* const> ext,
		nytl::Span<const char* const> layers, bool reverseLayers);
	void initDevice(vk::PhysicalDevice, nytl::Span<const char* const> ext,
		nytl::Span<const char* const> layers, bool reverseLayers);
	void initDevice(nytl::Span<const char* const> ext, nytl::Span<const char* const> layers,
		bool reverseLayers);
	void initSurface(Surface&& surface) { surface_ = std::move(surface); }
	void initSwapChain(const vk::Extent2D& size, const SwapChainSettings& settings);
	vk::PhysicalDevice choosePhysicalDevice(nytl::Span<const vk::PhysicalDevice> devices) const;

	friend void swap(Context& a, Context& b) noexcept;

protected:
	vk::Instance instance_ {};
	std::unique_ptr<DebugCallback> debugCallback_; //nonMovable
	Surface surface_;

	std::unique_ptr<Device> device_; //nonMovable
	SwapChain swapChain_;

	const Queue* presentQueue_ = nullptr;
	const Queue* graphicsComputeQueue_ = nullptr;
};

} // namespace vpp
