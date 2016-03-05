#pragma once

#include <vpp/vk.hpp>
#include <vpp/platform.hpp>
#include <vpp/fwd.hpp>
#include <vpp/device.hpp>
#include <vpp/instance.hpp>
#include <vpp/swapChain.hpp>

#include <vector>
#include <string>
#include <memory>

namespace vpp
{

class Context
{
protected:
	Instance instance_;
	Device device_;
	SwapChain swapChain_;

public:
	virtual const Surface& surface() const = 0;

	const Device& device() const { return device_; }
	const Instance& instance() const { return instance_; }
	const SwapChain& swapChain() const { return swapChain_; }
};

}
