#pragma once

#include <vpp/fwd.hpp>
#include <vpp/vk.hpp>

#include <vector>
#include <string>
#include <memory>

namespace vpp
{

///Represents Vulkan swap chain and associated images/frameBuffers.
class SwapChain
{
public:
    struct Buffer
    {
        VkImage image;
        VkImageView imageView;
    };

protected:
    PFN_vkCreateSwapchainKHR fpCreateSwapchainKHR;
    PFN_vkDestroySwapchainKHR fpDestroySwapchainKHR;
    PFN_vkGetSwapchainImagesKHR fpGetSwapchainImagesKHR;
    PFN_vkAcquireNextImageKHR fpAcquireNextImageKHR;
    PFN_vkQueuePresentKHR fpQueuePresentKHR;

    vk::SwapchainKHR swapChain_ {};
    vk::Format format_;
    vk::ColorSpaceKHR colorSpace_;
	vk::Extent2D extent_;

	const Context* context_ = nullptr;
	const Surface* surface_ = nullptr;
	std::vector<Buffer> buffers_;

protected:
	SwapChain() = default;

    void create(const Context& context, const Surface& surface, const vk::Extent2D& extent);

	void init();
    void loadProcs();
    void queryFormats();
    Buffer createBuffer(VkImage image) const;
    VkSwapchainCreateInfoKHR swapChainCreateInfo();

	void destroy();
	void destroyBuffers();
	void destroySwapchain();

public:
    SwapChain(const Context& context, const Surface& surface, const vk::Extent2D& extent);
    ~SwapChain();

	void resize(const vk::Extent2D& extent);

	const Surface& surface() const { return *surface_; }
	const Context& context() const { return *context_; }

    vk::Instance vkInstance() const;
    vk::PhysicalDevice vkPhysicalDevice() const;
    vk::Device vkDevice() const;

    vk::SwapchainKHR vkSwapChain() const { return swapChain_; }
	vk::SurfaceKHR vkSurface() const;
	vk::Format format() const { return format_; }
	vk::ColorSpaceKHR colorSpace() const { return colorSpace_; }
	const vk::Extent2D& extent() const { return extent_; }
    const std::vector<Buffer>& buffers() const { return buffers_; }

    unsigned int acquireNextImage(vk::Semaphore presentComplete) const;
    void present(vk::Queue queue, unsigned int currentBuffer) const;
};

}
