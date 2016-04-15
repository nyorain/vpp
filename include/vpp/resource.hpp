#pragma once

#include <vpp/fwd.hpp>
#include <vpp/device.hpp>

namespace vpp
{

///The Resource class represents a vulkan resource associated to a vulkan device.
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

///Resource class that already owns another resource and does therefore not have to hold a second
///vulkan device reference.
///Classes inheriting from this template have to give theirselfs as template paramater (CRTP) and
///implement a <resourceRef() const> member function which returns a (const) reference to a Resource
///or another ResourceReference.
///This class exists just as an optimization (1 word less memory needed) for Resource classes.
///For an example look e.g. at the CommandBuffer class, which naturally holds a reference to the
///CommandPool it is allocated from and therefore does not have to hold a additional reference to
///its vulkan devcie, since this information can be retrieved from the CommandPool reference (done
///by this template class).
template <typename T>
class ResourceReference : public NonCopyable
{
public:
	const Device& device() const { return reinterpret_cast<const T&>(*this).resourceRef().device(); }

	VkInstance vkInstance() const { return device().vkInstance(); }
	VkPhysicalDevice vkPhysicalDevice() const { return device().vkPhysicalDevice(); }
	VkDevice vkDevice() const { return device().vkDevice(); }
};

}
