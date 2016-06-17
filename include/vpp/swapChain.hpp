#pragma once

#include <vpp/fwd.hpp>
#include <vpp/resource.hpp>

#include <vector>
#include <string>
#include <memory>

namespace vpp
{

///TODO: more settings, especially related to blocking, presentation mode (vsync) etc.
///Represents Vulkan swap chain and associated images/frameBuffers.
class SwapChain : public Resource
{
public:
    struct RenderBuffer
    {
        vk::Image image;
        vk::ImageView imageView;
    };

public:
	SwapChain() = default;
	///Constructs the swap chain for a given vulkan surface.
	///Note that the given size is only used if the backend does not provide
	///a surface size (e.g. wayland backend). Usually the swapChain will have the same size
	///as the underlaying native surface, then the size parameter is ignored.
	SwapChain(const Device& device, vk::SurfaceKHR surface, const vk::Extent2D& size);
    ~SwapChain();

	SwapChain(SwapChain&& other) noexcept;
	SwapChain& operator=(SwapChain&& other) noexcept;

	///Resizes the swapchain to the given size. Should be called if the native window of the
	///underlaying surface handle changes it size to make sure the swapchain fills the
	///whole window. May invalidate all images and imageViews retrived before the resize.
	///Note that the size paramter is only used when the backend does not provide a fixed
	///surface size. Otherwise the swapchain will simply be resized to the current surface
	///size and the given size paramter is ignored.
	void resize(const vk::Extent2D& size);

	///Acquires the next swapchain image (i.e. the next render buffer).
	///\return The id of the newly acquired image. This id should be passed
	///to present as currentBuffer parameter.
	///May block. TODO.
    unsigned int acquireNextImage(vk::Semaphore presentComplete) const;
    void present(vk::Queue queue, unsigned int currentBuffer) const;

    const vk::SwapchainKHR& vkSwapChain() const { return swapChain_; }
	const vk::SurfaceKHR& vkSurface() const { return surface_; }

	vk::Format format() const { return format_; }
	vk::ColorSpaceKHR colorSpace() const { return colorSpace_; }
	vk::Extent2D size() const;
    const std::vector<RenderBuffer>& renderBuffers() const { return buffers_; }

	friend void swap(SwapChain& a, SwapChain& b) noexcept;

protected:
	void initSwapChain();
    void queryFormats();
    vk::SwapchainCreateInfoKHR swapChainCreateInfo();
	void destroyBuffers();

protected:
    vk::SwapchainKHR swapChain_ {};
    vk::Format format_;
    vk::ColorSpaceKHR colorSpace_;

	unsigned int width_ = 0;
	unsigned int height_ = 0;

	vk::SurfaceKHR surface_ {};
	std::vector<RenderBuffer> buffers_;
};

}
