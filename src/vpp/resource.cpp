#include <vpp/resource.hpp>

namespace vpp {

const vk::Instance& Device::vkInstance() const noexcept { return device().vkInstance(); }
const vk::Device& Device::vkDevice() const noexcept { return device().vkDevice(); }
const vk::PhysicalDevice& Device::vkPhysicalDevice() const noexcept
	{ return device().vkPhysicalDevice(); }

#ifndef VPP_ONE_DEVICE_OPTIMIZATION

Resource::Resource(Resource&& other) noexcept
{
	device_ = other.device_;
	other.device_ = {};
}

Resource& Resource::operator=(Resource&& other) noexcept
{
	device_ = other.device_;
	other.deivce_ = {};
}

#else // VPP_ONE_DEVICE_OPTIMIZATION

const Device* Resource::deviceInstance_;
void Resource::init(const Device& dev)
{
	if(deviceInstance_ == &dev) return; // most likely

	if(!deviceInstance_) deviceInstance_ = &dev;
	else throw std::logic_error("vpp::Resource: VPP_ONE_DEVICE_OPTIMIZATION invalid device");
}

#endif // VPP_ONE_DEVICE_OPTIMIZATION

} // namespace vpp
