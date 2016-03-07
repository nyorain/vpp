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

//Context
class Context
{
public:
	struct CreateInfo
	{
		bool debug = 0;
		vk::Extent2D size {800, 500};

		std::vector<const char*> instanceExtensions;
		std::vector<const char*> deviceExtensions;
	};

protected:
	vk::Instance instance_ {};
	Device device_;
	const Device::Queue* queue_ = nullptr;
	SwapChain swapChain_;

	std::unique_ptr<DebugCallback> debugCallback_;

protected:
	Context();

	void initInstance(const CreateInfo& info);
	void initDevice(const CreateInfo& info);
	void initSwapChain(const CreateInfo& info);

public:
	virtual ~Context();

	virtual const Surface& surface() const = 0;

	const Device& device() const { return device_; }
	const SwapChain& swapChain() const { return swapChain_; }
	const Device::Queue& queue() const { return *queue_; }

	SwapChain& swapChain() { return swapChain_; }
	Device& device() { return device_; }

	const vk::Instance& vkInstance() const { return instance_; }
};

}
