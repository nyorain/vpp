#pragma once

#include <vpp/vk.hpp>
#include <vpp/platform.hpp>

#include <vector>
#include <string>
#include <memory>

namespace vpp
{

class Context : public NonCopyable
{
protected:
	Instance instance_;
	Device device_;
	SwapChain swapChain_;

public:
	const Surface& surface() const = 0;
};

}
