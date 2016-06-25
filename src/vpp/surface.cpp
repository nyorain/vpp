#include <vpp/surface.hpp>
#include <vpp/vk.hpp>
#include <vpp/procAddr.hpp>

namespace vpp
{

Surface::Surface(vk::Instance instance, vk::SurfaceKHR surface)
	: instance_(instance), surface_(surface)
{
}

Surface::~Surface()
{
	if(vkInstance() && vkSurface())
		VPP_PROC(vkInstance(), DestroySurfaceKHR)(vkInstance(), vkSurface(), nullptr);

	surface_ = {};
	instance_ = {};
}

Surface::Surface(Surface&& other) noexcept
{
	swap(*this, other);
}

Surface& Surface::operator=(Surface other) noexcept
{
	swap(*this, other);
	return *this;
}

void swap(Surface& a, Surface& b) noexcept
{
	using std::swap;

	std::swap(a.instance_, b.instance_);
	std::swap(a.surface_, b.surface_);
}

bool Surface::queueFamilySupported(vk::PhysicalDevice phdev, std::uint32_t qFamiliyIndex) const
{
	VPP_LOAD_PROC(vkInstance(), GetPhysicalDeviceSurfaceSupportKHR);

	vk::Bool32 ret;
	VPP_CALL(pfGetPhysicalDeviceSurfaceSupportKHR(phdev, qFamiliyIndex, vkSurface(), &ret));
	return (ret == VK_TRUE);
}

std::vector<std::uint32_t> Surface::supportedQueueFamilies(vk::PhysicalDevice phdev) const
{
	std::uint32_t count;
	vk::getPhysicalDeviceQueueFamilyProperties(phdev, count, nullptr);

	std::vector<std::uint32_t> ret;
	for(std::size_t i(0); i < count; ++i)
	{
		if(queueFamilySupported(phdev, i))
			ret.push_back(i);
	}

	return ret;
}

vk::SurfaceCapabilitiesKHR Surface::capabilities(vk::PhysicalDevice phdev) const
{
	VPP_LOAD_PROC(vkInstance(), GetPhysicalDeviceSurfaceCapabilitiesKHR);

	vk::SurfaceCapabilitiesKHR surfCaps;
	VPP_CALL(pfGetPhysicalDeviceSurfaceCapabilitiesKHR(phdev, vkSurface(), &surfCaps));
	return surfCaps;
}

std::vector<vk::SurfaceFormatKHR> Surface::formats(vk::PhysicalDevice phdev) const
{
	VPP_LOAD_PROC(vkInstance(), GetPhysicalDeviceSurfaceFormatsKHR);

	uint32_t count;
	VPP_CALL(pfGetPhysicalDeviceSurfaceFormatsKHR(phdev, vkSurface(), &count, nullptr));

	std::vector<vk::SurfaceFormatKHR> ret(count);
	VPP_CALL(pfGetPhysicalDeviceSurfaceFormatsKHR(phdev, vkSurface(), &count, ret.data()));

	return ret;
}

std::vector<vk::PresentModeKHR> Surface::presentModes(vk::PhysicalDevice phdev) const
{
	VPP_LOAD_PROC(vkInstance(), GetPhysicalDeviceSurfacePresentModesKHR);

	uint32_t count;
	VPP_CALL(pfGetPhysicalDeviceSurfacePresentModesKHR(phdev, vkSurface(), &count, nullptr));

	std::vector<vk::PresentModeKHR> ret(count);
	VPP_CALL(pfGetPhysicalDeviceSurfacePresentModesKHR(phdev, vkSurface(), &count, ret.data()));

	return ret;
}

}
