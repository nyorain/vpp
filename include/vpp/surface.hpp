#pragma once

#include <vpp/fwd.hpp>

#include <vector>
#include <string>
#include <memory>

namespace vpp
{

///Base class for vulkan surfaces.
///There is only a vulkan instance needed to create a Surface, so it is not considered a
///Resource class.
///The different backends provide functions for create surfaces from native window handles.
class Surface
{
public:
	Surface() = default;
    Surface(vk::Instance instance, vk::SurfaceKHR surface);
    virtual ~Surface();

	Surface(Surface&& other) noexcept;
	Surface& operator=(Surface other) noexcept;

    const vk::Instance& vkInstance() const { return instance_; }
    const vk::SurfaceKHR& vkSurface() const { return surface_; }

	///Returns whether the surface suppports the given queue family.
    bool queueFamilySupported(vk::PhysicalDevice phdev, std::uint32_t qFamiliyIndex) const;

	///Returns all supported queue families.
    std::vector<std::uint32_t> supportedQueueFamilies(vk::PhysicalDevice phdev) const;

    vk::SurfaceCapabilitiesKHR capabilities(vk::PhysicalDevice phdev) const;
    std::vector<vk::SurfaceFormatKHR> formats(vk::PhysicalDevice phdev) const;
    std::vector<vk::PresentModeKHR> presentModes(vk::PhysicalDevice phdev) const;

	operator vk::SurfaceKHR() const { return vkSurface(); }
	friend void swap(Surface& a, Surface& b) noexcept;

protected:
    vk::Instance instance_ {};
    vk::SurfaceKHR surface_ {};
};

}
