#pragma once

#include <vpp/vk.hpp>

#include <vector>
#include <string>
#include <memory>

namespace vpp
{

class Surface
{
protected:
    vk::Instance instance_ {};
    vk::SurfaceKHR surface_ {};

protected:
	Surface() = default;
    Surface(vk::Instance instance);

    void init(vk::Instance instance);
    void destroy();

public:
    Surface(VkInstance instance, VkSurfaceKHR surface);
    virtual ~Surface();

	Surface(Surface&& other) noexcept;
	Surface& operator=(Surface&& other) noexcept;

    vk::Instance vkInstance() const { return instance_; }
    vk::SurfaceKHR vkSurface() const { return surface_; }

    bool queueFamilySupported(vk::PhysicalDevice phdev, std::uint32_t qFamiliyIndex) const;
    std::vector<std::uint32_t> supportedQueueFamilies(vk::PhysicalDevice phdev) const;

    vk::SurfaceCapabilitiesKHR capabilities(vk::PhysicalDevice phdev) const;
    std::vector<vk::SurfaceFormatKHR> formats(vk::PhysicalDevice phdev) const;
    std::vector<vk::PresentModeKHR> presentModes(vk::PhysicalDevice phdev) const;

	void swap(Surface& other) noexcept;
};

}
