#pragma once

#include <vpp/vk.hpp>
#include <vpp/fwd.hpp>
#include <vpp/device.hpp>
#include <vpp/swapChain.hpp>

#include <vector>
#include <string>
#include <memory>

namespace vpp
{

class DebugCallback;

//TODO: correct graphics/compute/present queue support
//Context
class Context
{
public:
	struct CreateInfo
	{
		vk::Extent2D size {800, 500};
		vk::DebugReportFlagsEXT debugFlags = vk::DebugReportFlagBitsEXT::ErrorEXT |
			vk::DebugReportFlagBitsEXT::WarningEXT;

		std::vector<const char*> instanceExtensions;
		std::vector<const char*> deviceExtensions;

		std::vector<vk::DeviceQueueCreateInfo> extraQueues;
		bool extraPresentQueue = 1;
	};

protected:
	vk::Instance instance_ {};
	Device device_;
	SwapChain swapChain_;

	const Device::Queue* presentQueue_ = nullptr;

	std::unique_ptr<DebugCallback> debugCallback_;

protected:
	Context();

	void initInstance(const CreateInfo& info);
	void initDevice(const CreateInfo& info);
	void initSwapChain(const CreateInfo& info);
	vk::PhysicalDevice choosePhysicalDevice(const std::vector<vk::PhysicalDevice>& devices) const;

public:
	virtual ~Context();

	virtual const Surface& surface() const = 0;

	const Device& device() const { return device_; }
	const SwapChain& swapChain() const { return swapChain_; }

	const Device::Queue* graphicsQueue() const { return presentQueue_; }
	const Device::Queue* computeQueue() const { return presentQueue_; }
	const Device::Queue& presentQueue() const { return *presentQueue_; }

	SwapChain& swapChain() { return swapChain_; }
	Device& device() { return device_; }

	const vk::Instance& vkInstance() const { return instance_; }
};

}
