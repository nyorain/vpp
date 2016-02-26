#pragma once

#include <vpp/fwd.hpp>
#include <vpp/vk.hpp>
#include <vpp/resource.hpp>

#include <vector>
#include <string>
#include <memory>

namespace vpp
{

///Represents Vulkan swap chain and associated images/frameBuffers.
class SwapChain : public Resource
{
public:
    struct Buffer
    {
        VkImage image;
        VkImageView imageView;
    };

protected:
    vk::SwapchainKHR swapChain_ {};
    vk::Format format_;
    vk::ColorSpaceKHR colorSpace_;
	vk::Extent2D extent_;

	vk::SurfaceKHR surface_ {};
	std::vector<Buffer> buffers_;

protected:
	SwapChain() = default;

    void create(const Device& context, vk::SurfaceKHR surface, const vk::Extent2D& extent);

	void init();
    void queryFormats();
    Buffer createBuffer(VkImage image) const;
    VkSwapchainCreateInfoKHR swapChainCreateInfo();

	void destroy();
	void destroyBuffers();
	void destroySwapchain();

public:
    SwapChain(const Device& device, const Surface& surface, const vk::Extent2D& extent = {});
	SwapChain(const Device& device, vk::SurfaceKHR surface, const vk::Extent2D& extent = {});
    ~SwapChain();

	void resize(const vk::Extent2D& extent);

    vk::SwapchainKHR vkSwapChain() const { return swapChain_; }
	vk::SurfaceKHR vkSurface() const { return surface_; }

	vk::Format format() const { return format_; }
	vk::ColorSpaceKHR colorSpace() const { return colorSpace_; }
	const vk::Extent2D& extent() const { return extent_; }
    const std::vector<Buffer>& buffers() const { return buffers_; }

    unsigned int acquireNextImage(vk::Semaphore presentComplete) const;
    void present(vk::Queue queue, unsigned int currentBuffer) const;
};

}
