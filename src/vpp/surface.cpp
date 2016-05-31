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
    if(vkInstance() && vkSurface()) vk::destroySurfaceKHR(vkInstance(), vkSurface(), nullptr);
    surface_ = {};
}

Surface::Surface(Surface&& other) noexcept
{
	swap(*this, other);
}

Surface& Surface::operator=(Surface&& other) noexcept
{
	this->~Surface();
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
	VPP_LOAD_INSTANCE_PROC(vkInstance(), GetPhysicalDeviceSurfaceSupportKHR);

    vk::Bool32 ret;
    fpGetPhysicalDeviceSurfaceSupportKHR(phdev, qFamiliyIndex, vkSurface(), &ret);
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
	VPP_LOAD_INSTANCE_PROC(vkInstance(), GetPhysicalDeviceSurfaceCapabilitiesKHR);

    vk::SurfaceCapabilitiesKHR surfCaps;
    fpGetPhysicalDeviceSurfaceCapabilitiesKHR(phdev, vkSurface(), &surfCaps.vkHandle());
    return surfCaps;
}

std::vector<vk::SurfaceFormatKHR> Surface::formats(VkPhysicalDevice phdev) const
{
	VPP_LOAD_INSTANCE_PROC(vkInstance(), GetPhysicalDeviceSurfaceFormatsKHR);

    uint32_t count;
    fpGetPhysicalDeviceSurfaceFormatsKHR(phdev, vkSurface(), &count, nullptr);

    std::vector<vk::SurfaceFormatKHR> ret(count);
    fpGetPhysicalDeviceSurfaceFormatsKHR(phdev, vkSurface(), &count,
		reinterpret_cast<VkSurfaceFormatKHR*>(ret.data()));

    return ret;
}

std::vector<vk::PresentModeKHR> Surface::presentModes(VkPhysicalDevice phdev) const
{
	VPP_LOAD_INSTANCE_PROC(vkInstance(), GetPhysicalDeviceSurfacePresentModesKHR);

    uint32_t count;
    fpGetPhysicalDeviceSurfacePresentModesKHR(phdev, vkSurface(), &count, nullptr);

    std::vector<vk::PresentModeKHR> ret(count);
    fpGetPhysicalDeviceSurfacePresentModesKHR(phdev, vkSurface(), &count,
		reinterpret_cast<VkPresentModeKHR*>(ret.data()));

    return ret;
}

}
