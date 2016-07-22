#pragma once

#include <vpp/fwd.hpp>
#include <vpp/resource.hpp>
#include <vpp/vulkan/enums.hpp>
#include <vpp/vulkan/structs.hpp>

#include <vector>
#include <string>
#include <memory>

namespace vpp
{

///SwapChainSettings is a virtual base class that can be used to implement a settings
///chose mechanism for SwapChain creation. An extra virtual class is needed instead of a
///struct because there a various settings and their availibility must be queried.
///The default implementation just choses the values which are considered best from the
///available ones.
class SwapChainSettings
{
public:
	///This function will be called by the SwapChain this object was passed to.
	///The returned SwapChainCreateInfo will be used to create the vulkan swapchain.
	///\param caps The queried capabilities for the surface the swapChain will be created for.
	///\param modes The queried available present modes.
	///\param formats The queried available formats.
	///\param size The preferred size passed as parameter to the SwapChain.
	///On most platforms the size is chosen by the surface (from the native surface),
	///and this paramter can be ignored.
	virtual vk::SwapchainCreateInfoKHR parse(const vk::SurfaceCapabilitiesKHR& caps,
		const Range<vk::PresentModeKHR>& modes,
		const Range<vk::SurfaceFormatKHR>& formats,
		const vk::Extent2D& size) const;
};

///A SwapChainSettings implementation that allows to chose some preferred values for
///multiple fields. It can also be specified what should be done if one of those values is
///not available.
///After SwapChain creation the members will be set to the values that were chosen in the end
///for the SwapChain.
class DefaultSwapChainSettings final : public SwapChainSettings
{
public:
	///What should be done if a preferred setting is not available.
	enum class ErrorAction
	{
		none, ///Simply choses another setting.
		output, ///Choses another setting and prints a warning to cerr.
		throwException ///Throws a std::runtime_error. Does not change a setting.
	};

public:
	mutable vk::Format prefFormat = vk::Format::r8g8b8a8Unorm;
	mutable vk::PresentModeKHR prefPresentMode = vk::PresentModeKHR::mailbox;
	mutable vk::CompositeAlphaBitsKHR prefAlpha = vk::CompositeAlphaBitsKHR::opaque;
	mutable vk::SurfaceTransformBitsKHR prefTransform = vk::SurfaceTransformBitsKHR::identity;
	mutable vk::ImageUsageFlags prefUsage = {};

	ErrorAction errorAction;

public:
	vk::SwapchainCreateInfoKHR parse(const vk::SurfaceCapabilitiesKHR& caps,
		const Range<vk::PresentModeKHR>& modes,
		const Range<vk::SurfaceFormatKHR>& formats,
		const vk::Extent2D& size) const override;
};

///TODO: synchronization for acquire and present commands. Handle acquire -> out_of_date
///Represents Vulkan swap chain and associated images/frameBuffers.
class SwapChain : public ResourceHandle<vk::SwapchainKHR>
{
public:
    struct RenderBuffer
    {
        vk::Image image;
        vk::ImageView imageView;
    };

	struct CreateInfo
	{
		vk::Format preferredFormat;
		vk::PresentModeKHR preferredPresentMode;
	};

public:
	SwapChain() = default;

	///Constructs the swap chain for a given vulkan surface.
	///Note that the given size is only used if the backend does not provide
	///a surface size (e.g. wayland backend). Usually the swapChain will have the same size
	///as the underlaying native surface, then the size parameter is ignored.
	SwapChain(const Device& device, vk::SurfaceKHR surface,
		const vk::Extent2D& size = {}, const SwapChainSettings& = {});
    ~SwapChain();

	SwapChain(SwapChain&& other) noexcept = default;
	SwapChain& operator=(SwapChain&& other) noexcept = default;

	///Resizes the swapchain to the given size. Should be called if the native window of the
	///underlaying surface handle changes it size to make sure the swapchain fills the
	///whole window. May invalidate all images and imageViews retrived before the resize.
	///Note that the size paramter is only used when the backend does not provide a fixed
	///surface size. Otherwise the swapchain will simply be resized to the current surface
	///size and the given size paramter is ignored.
	void resize(const vk::Extent2D& size = {}, const SwapChainSettings& = {});

	///Acquires the next swapchain image (i.e. the next render buffer).
	///\param sem Semaphore to be signaled when acquiring is complete or nullHandle.
	///\param fence Fence to be signaled when acquiring is complete or nullHandle.
	///\param id The id of the newly acquired image.
	///\return The result returned by vkAcquireImageKHR. The caller has to handle
	///results like outOfDate or suboptimal and can decide if to recreate (resize()) the
	///swapChain. There will not be any check performed on the result.
    vk::Result acquire(unsigned int& id, vk::Semaphore sem = {}, vk::Fence fence = {}) const;

	///Queues commands to present the image with the given id on the given queue.
	///\param wait The semaphore to wait on before presenting (usually signaled at the end
	///of all rendering commands for this image). Can be nullHandle.
	///\return The result returned by vkQueuePresentKHR. The caller has to handle
	///results like outOfDate or suboptimal and can decide if to recreate (resize()) the
	///swapChain. There will not be any check performed on the result.
    vk::Result present(const Queue& queue, unsigned int image, vk::Semaphore wait = {}) const;

	const vk::SurfaceKHR& vkSurface() const { return surface_; }

	vk::Format format() const { return format_; }
	vk::Extent2D size() const;
    const std::vector<RenderBuffer>& renderBuffers() const { return buffers_; }

protected:
	void destroyBuffers();

protected:
	vk::SurfaceKHR surface_ {};
	std::vector<RenderBuffer> buffers_;

	//not needed to store these parameters. just done for convinience. reasonable?
	unsigned int width_ = 0;
	unsigned int height_ = 0;
    vk::Format format_;
};

}
