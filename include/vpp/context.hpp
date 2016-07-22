#pragma once

#include <vpp/fwd.hpp>
#include <vpp/swapChain.hpp>
#include <vpp/surface.hpp>
#include <vpp/vulkan/enums.hpp>

#include <vector>
#include <string>
#include <memory>

namespace vpp
{

//TODO: correct graphics/compute/present queue support -- holy shit one bugfest atm
//TODO: sparse queue
//TODO: correct layer management
//TODO: do not public expose init functions. Needed for createContext funcionts.
//rather do some friend class Backend and then create backend implementations using the
//functionality.
// -- we are writing code for idiots --

#ifdef VPP_DEBUG
 const vk::DebugReportFlagsEXT contextDefaultDebugFlags = vk::DebugReportBitsEXT::warning |
	vk::DebugReportBitsEXT::error | vk::DebugReportBitsEXT::performanceWarning |
	vk::DebugReportBitsEXT::debug;

#else
 const vk::DebugReportFlagsEXT contextDefaultDebugFlags = {};

#endif

///A Vulkan Context. Can be used to easily create Device and Swapchain.
///The Context will automatically create a present queue for its surface as well as a graphics
///and compute queue (if possible just one queue for all needs).
///If more fine-grained control over device, queues and swapChain creation is needed, consider
///creating them manually.
class Context
{
public:
	struct CreateInfo
	{
		///These two members are ignored if the backend choses the size of the swapChain.
		unsigned int width = 0;
		unsigned int height = 0;
		vk::DebugReportFlagsEXT debugFlags = contextDefaultDebugFlags;
		std::vector<const char*> instanceExtensions;
		std::vector<const char*> deviceExtensions;
	};

public:
	Context();
	Context(vk::Instance ini, vk::SurfaceKHR surface, const CreateInfo& info);
	~Context();

	Context(Context&& other) noexcept;
	Context& operator=(Context other) noexcept;

	const Surface& surface() const { return surface_; }

	const Device& device() const { return *device_; }
	const SwapChain& swapChain() const { return swapChain_; }

	const Queue* graphicsComputeQueue() const { return presentQueue_; }
	const Queue& presentQueue() const { return *presentQueue_; }

	SwapChain& swapChain() { return swapChain_; }
	Device& device() { return *device_; }

	const vk::Instance& vkInstance() const { return instance_; }
	const vk::SurfaceKHR& vkSurface() const { return surface_.vkSurface(); }
	const vk::Device& vkDevice() const;
	const vk::SwapchainKHR& vkSwapChain() const { return swapChain().vkHandle(); }

	void initInstance(const CreateInfo& info);
	void initDevice(const CreateInfo& info);
	void initSurface(Surface&& surface) { surface_ = std::move(surface); }
	void initSwapChain(const CreateInfo& info);
	vk::PhysicalDevice choosePhysicalDevice(const std::vector<vk::PhysicalDevice>& devices) const;

	friend void swap(Context& a, Context& b) noexcept;

protected:
	///XXX: better use pImpl idion instead of 2 unique ptrs
	vk::Instance instance_ {};
	std::unique_ptr<Device> device_;
	Surface surface_;
	SwapChain swapChain_;

	const Queue* presentQueue_ = nullptr;
	const Queue* graphicsComputeQueue_ = nullptr;
	std::unique_ptr<DebugCallback> debugCallback_;
};

}
