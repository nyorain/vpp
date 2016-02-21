#include <vpp/surface.hpp>
#include <vpp/call.hpp>
#include <vpp/procAddr.hpp>

namespace vpp
{

Surface::Surface(vk::Instance instance)
{
    init(instance);
}

Surface::Surface(vk::Instance instance, vk::SurfaceKHR surface) : Surface(instance)
{
    surface_ = surface;
}

void Surface::init(vk::Instance instance)
{
    instance_ = instance;
    loadProcs();
}

void Surface::destroy()
{
    if(vkSurface()) vk::destroySurfaceKHR(vkInstance(), vkSurface(), nullptr);
    surface_ = 0;
}

bool Surface::queueFamilySupported(vk::PhysicalDevice phdev, std::uint32_t qFamiliyIndex) const
{
    vk::Bool32 ret;
    VPP_CALL(fpGetPhysicalDeviceSurfaceSupportKHR(phdev, qFamiliyIndex, vkSurface(), &ret));
    return (ret == VK_TRUE);
}

std::vector<std::uint32_t> Surface::supportedQueueFamilies(vk::PhysicalDevice phdev) const
{
    std::uint32_t count;
	vk::getPhysicalDeviceQueueFamilyProperties(phdev, &count, nullptr);

    std::vector<std::uint32_t> ret;
	for(std::size_t i(0); i < count; ++i)
	{
        if(queueFamilySupported(phdev, i))
            ret.push_back(i);
	}

    return ret;
}

std::vector<std::uint32_t> Surface::supportedGraphicalQueueFamilies(vk::PhysicalDevice phdev) const
{
	auto queueProps = vk::getPhysicalDeviceQueueFamilyProperties(phdev);

    std::vector<std::uint32_t> ret;
	for(std::size_t i(0); i < queueProps.size(); ++i)
	{
        bool supported = queueFamilySupported(phdev, i);
		if((queueProps[i].queueFlags() & vk::QueueFlagBits::Graphics) && supported)
            ret.push_back(i);
	}

    return ret;
}

vk::SurfaceCapabilitiesKHR Surface::capabilities(vk::PhysicalDevice phdev) const
{
    vk::SurfaceCapabilitiesKHR surfCaps;
    VPP_CALL(fpGetPhysicalDeviceSurfaceCapabilitiesKHR(phdev, vkSurface(), &surfCaps.vkHandle()));
    return surfCaps;
}

std::vector<vk::SurfaceFormatKHR> Surface::formats(VkPhysicalDevice phdev) const
{
    uint32_t count;
    VPP_CALL(fpGetPhysicalDeviceSurfaceFormatsKHR(phdev, vkSurface(), &count, nullptr));

    std::vector<vk::SurfaceFormatKHR> ret(count);
    VPP_CALL(fpGetPhysicalDeviceSurfaceFormatsKHR(phdev, vkSurface(), &count,
		reinterpret_cast<VkSurfaceFormatKHR*>(ret.data())));

    return ret;
}

std::vector<vk::PresentModeKHR> Surface::presentModes(VkPhysicalDevice phdev) const
{
    uint32_t count;
    VPP_CALL(fpGetPhysicalDeviceSurfacePresentModesKHR(phdev, vkSurface(), &count, nullptr));

    std::vector<vk::PresentModeKHR> ret(count);
    VPP_CALL(fpGetPhysicalDeviceSurfacePresentModesKHR(phdev, vkSurface(), &count,
		reinterpret_cast<VkPresentModeKHR*>(ret.data())));

    return ret;
}

void Surface::loadProcs()
{
    VPP_LOAD_INSTANCE_PROC(vkInstance(), GetPhysicalDeviceSurfaceSupportKHR);
	VPP_LOAD_INSTANCE_PROC(vkInstance(), GetPhysicalDeviceSurfaceCapabilitiesKHR);
	VPP_LOAD_INSTANCE_PROC(vkInstance(), GetPhysicalDeviceSurfaceFormatsKHR);
	VPP_LOAD_INSTANCE_PROC(vkInstance(), GetPhysicalDeviceSurfacePresentModesKHR);
}

}
