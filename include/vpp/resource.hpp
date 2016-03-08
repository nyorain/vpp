#pragma once

#include <vpp/fwd.hpp>
#include <vpp/device.hpp>

namespace vpp
{

///Resource
class Resource : public NonCopyable
{
protected:
	const Device* device_ {nullptr};

protected:
	Resource() = default;

	void init(const Device& device) { device_ = &device; };
	void destroy(){ device_ = {}; }

public:
	Resource(const Device& device) : device_(&device) {}
	~Resource() = default;

	const Device& device() const { return *device_; }

	VkInstance vkInstance() const { return device().vkInstance(); }
	VkPhysicalDevice vkPhysicalDevice() const { return device().vkPhysicalDevice(); }
	VkDevice vkDevice() const { return device().vkDevice(); }
};

//swap template
template<typename T>
auto swap(T& a, T& b) -> decltype(a.swap(b))
{
	a.swap(b);
}

}
