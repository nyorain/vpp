#pragma once

#include <vpp/fwd.hpp>
#include <vpp/swapChain.hpp>
#include <vpp/surface.hpp>

#include <vector>
#include <string>
#include <memory>

namespace vpp
{

namespace fwd { extern const vk::DebugReportFlagsEXT defaultDebugFlags; }

//TODO: correct graphics/compute/present queue support -- holy shit one bugfest atm
//TODO: correct layer management
//TODO: do not public expose init functions. Needed for createContext funcionts.
//rather do some friend class Backend and then create backend implementations using the
//functionality.
// -- we are writing code for idiots --

///A Vulkan Context. Can be used to easily create Device and Swapchain.
class Context
{
public:
	struct CreateInfo
	{
		///These two members are ignored if the backend choses the size (i.e. the size of the window)
		unsigned int width = 0;
		unsigned int height = 0;
		vk::DebugReportFlagsEXT debugFlags = fwd::defaultDebugFlags;

		std::vector<const char*> instanceExtensions;
		std::vector<const char*> deviceExtensions;

		std::vector<vk::DeviceQueueCreateInfo> extraQueues;
		bool extraPresentQueue = 1;
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

	const Queue* graphicsQueue() const { return presentQueue_; }
	const Queue* computeQueue() const { return presentQueue_; }
	const Queue& presentQueue() const { return *presentQueue_; }

	SwapChain& swapChain() { return swapChain_; }
	Device& device() { return *device_; }

	const vk::Instance& vkInstance() const { return instance_; }
	const vk::SurfaceKHR& vkSurface() const { return surface_.vkSurface(); }
	const vk::Device& vkDevice() const;
	const vk::SwapchainKHR& vkSwapChain() const { return swapChain_.vkSwapChain(); }

	void initInstance(const CreateInfo& info);
	void initDevice(const CreateInfo& info);
	void initSurface(Surface&& surface) { surface_ = std::move(surface); }
	void initSwapChain(const CreateInfo& info);
	vk::PhysicalDevice choosePhysicalDevice(const std::vector<vk::PhysicalDevice>& devices) const;

	friend void swap(Context& a, Context& b) noexcept;

protected:
	vk::Instance instance_ {};
	std::unique_ptr<Device> device_;
	Surface surface_;
	SwapChain swapChain_;

	const Queue* presentQueue_ = nullptr;
	std::unique_ptr<DebugCallback> debugCallback_;
};

}
