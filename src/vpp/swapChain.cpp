#include <vpp/swapChain.hpp>
#include <vpp/vk.hpp>
#include <vpp/procAddr.hpp>
#include <vpp/queue.hpp>
#include <vpp/surface.hpp>
#include <vpp/image.hpp>
#include <vpp/vulkan/vulkan.hpp>

namespace vpp
{

SwapChain::SwapChain(const Device& device, vk::SurfaceKHR surface, const vk::Extent2D& size)
	: Resource(device), surface_(surface)
{
    surface_ = surface;

	width_ = size.width;
	height_ = size.height;

	queryFormats();
	initSwapChain();
}

SwapChain::~SwapChain()
{
	if(!vkSwapChain()) return;

	VPP_LOAD_PROC(vkDevice(), DestroySwapchainKHR);
    pfDestroySwapchainKHR(vkDevice(), vkSwapChain(), nullptr);
	swapChain_ = {};
}

SwapChain::SwapChain(SwapChain&& other) noexcept
{
	swap(*this, other);
}

SwapChain& SwapChain::operator=(SwapChain other) noexcept
{
	swap(*this, other);
	return *this;
}

void swap(SwapChain& a, SwapChain& b) noexcept
{
	using std::swap;

	swap(a.swapChain_, b.swapChain_);
	swap(a.buffers_, b.buffers_);
	swap(a.format_, b.format_);
	swap(a.colorSpace_, b.colorSpace_);
	swap(a.width_, b.width_);
	swap(a.height_, b.height_);
	swap(a.surface_, b.surface_);
	swap(a.device_, b.device_);
}

void SwapChain::destroyBuffers()
{
	for(auto& buf : buffers_) vk::destroyImageView(vkDevice(), buf.imageView, nullptr);
    buffers_.clear();
}

void SwapChain::initSwapChain()
{
	VPP_LOAD_PROC(vkDevice(), CreateSwapchainKHR);
	VPP_LOAD_PROC(vkDevice(), GetSwapchainImagesKHR);
	VPP_LOAD_PROC(vkDevice(), DestroySwapchainKHR);

	//destory just the buffers, not the swapchain (if there is any)
	destroyBuffers();

    //(re)create swapChain
	//if there is already a swapChain it will be passed as the old swapchain parameter
	auto oldSwapchain = vkSwapChain();

	//create info from seperate function since it requires a lot of querying
    auto createInfo = swapChainCreateInfo();
    VPP_CALL(pfCreateSwapchainKHR(vkDevice(), &createInfo, nullptr, &swapChain_));

	if(oldSwapchain)
	{
		pfDestroySwapchainKHR(vkDevice(), oldSwapchain, nullptr);
		oldSwapchain = {};
	}

    //new buffers
	//get swapchain images
    std::uint32_t count;
    VPP_CALL(pfGetSwapchainImagesKHR(vkDevice(), vkSwapChain(), &count, nullptr));

    std::vector<vk::Image> imgs(count);
    VPP_CALL(pfGetSwapchainImagesKHR(vkDevice(), vkSwapChain(), &count, imgs.data()));

	//create imageviews and insert buffers
    buffers_.reserve(count);
    for(auto& img : imgs)
	{
		vk::ComponentMapping components{
			vk::ComponentSwizzle::r,
			vk::ComponentSwizzle::g,
			vk::ComponentSwizzle::b,
			vk::ComponentSwizzle::a
		};

		vk::ImageSubresourceRange range{};
		range.aspectMask = vk::ImageAspectBits::color;
		range.levelCount = 1;
		range.layerCount = 1;

		vk::ImageViewCreateInfo info{};
		info.format = format();
		info.subresourceRange = range;
		info.viewType = vk::ImageViewType::e2d;
		info.components = components;
		info.image = img;

	    auto view = vk::createImageView(vkDevice(), info);

		buffers_.push_back({img, view});
	}
}

void SwapChain::resize(const vk::Extent2D& size)
{
	width_ = size.width;
	height_ = size.height;
	initSwapChain();
}

void SwapChain::queryFormats()
{
	VPP_LOAD_PROC(vkInstance(), GetPhysicalDeviceSurfaceFormatsKHR);

	//get formats
    uint32_t count;
    VPP_CALL(pfGetPhysicalDeviceSurfaceFormatsKHR(vkPhysicalDevice(), vkSurface(), &count, nullptr));

	if(!count) throw std::runtime_error("SwapChain::queryFormats: Surface has no valid formats.");

    std::vector<vk::SurfaceFormatKHR> formats(count);
    VPP_CALL(pfGetPhysicalDeviceSurfaceFormatsKHR(vkPhysicalDevice(), vkSurface(), &count,
		formats.data()));

    if(formats.size() == 1 && formats[0].format == vk::Format::undefined)
	{
		format_ = vk::Format::b8g8r8a8Unorm; //no preferred format from surface
	}
	else
	{
		//TODO: more advanced quering which format to choose
		format_ = formats[0].format;
	}

	colorSpace_ = formats[0].colorSpace;
}

vk::SwapchainCreateInfoKHR SwapChain::swapChainCreateInfo()
{
	VPP_LOAD_PROC(vkInstance(), GetPhysicalDeviceSurfacePresentModesKHR);
	VPP_LOAD_PROC(vkInstance(), GetPhysicalDeviceSurfaceCapabilitiesKHR);

	//presentModes
    uint32_t count;
    VPP_CALL(pfGetPhysicalDeviceSurfacePresentModesKHR(vkPhysicalDevice(), vkSurface(), &count,
		nullptr));

    std::vector<vk::PresentModeKHR> presentModes(count);
    VPP_CALL(pfGetPhysicalDeviceSurfacePresentModesKHR(vkPhysicalDevice(), vkSurface(), &count,
		presentModes.data()));

    auto presentMode = vk::PresentModeKHR::fifo;
    for(auto& mode : presentModes)
    {
        if(mode == vk::PresentModeKHR::mailbox)
        {
            presentMode = mode;
            break;
        }
        else if(mode == vk::PresentModeKHR::immediate)
        {
            presentMode = mode;
        }
    }

	//capabilities
	vk::SurfaceCapabilitiesKHR surfCaps;
	VPP_CALL(pfGetPhysicalDeviceSurfaceCapabilitiesKHR(vkPhysicalDevice(), vkSurface(), &surfCaps));

    //size
	//if the size is equal the zero the size has to be chosen by us (using the given size)
    if(surfCaps.currentExtent.width > 1)
	{
        width_ = surfCaps.currentExtent.width;
        height_ = surfCaps.currentExtent.height;
	}

    //number of images
    std::uint32_t imagesCount = surfCaps.minImageCount + 1;
    if((surfCaps.maxImageCount > 0) && (imagesCount > surfCaps.maxImageCount))
        imagesCount = surfCaps.maxImageCount;

    //transform
    auto preTransform = surfCaps.currentTransform;
    if (surfCaps.supportedTransforms & vk::SurfaceTransformBitsKHR::identity)
        preTransform = vk::SurfaceTransformBitsKHR::identity;

	//createInfo
    vk::SwapchainCreateInfoKHR ret;
    ret.surface = vkSurface();
    ret.minImageCount = imagesCount;
    ret.imageExtent = size();
    ret.imageUsage = vk::ImageUsageBits::colorAttachment;
    ret.preTransform = preTransform;
    ret.imageArrayLayers = 1;
    ret.queueFamilyIndexCount = 0;
    ret.pQueueFamilyIndices = nullptr;
    ret.clipped = true;
	ret.oldSwapchain = vkSwapChain();
    ret.compositeAlpha = vk::CompositeAlphaBitsKHR::opaque;
    ret.imageFormat = format();
    ret.imageColorSpace = colorSpace();
    ret.presentMode = presentMode;

    return ret;
}

unsigned int SwapChain::acquireNextImage(vk::Semaphore sem, vk::Fence fence) const
{
	//TODO: out of date, sync, timeout...
	VPP_LOAD_PROC(vkDevice(), AcquireNextImageKHR);

    std::uint32_t ret;
    VPP_CALL(pfAcquireNextImageKHR(vkDevice(), vkSwapChain(), UINT64_MAX, sem, fence, &ret));
    return ret;
}

void SwapChain::present(const Queue& queue, std::uint32_t currentBuffer, vk::Semaphore wait) const
{
	//TODO: sync!
	VPP_LOAD_PROC(vkDevice(), QueuePresentKHR);

    vk::PresentInfoKHR presentInfo {};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &swapChain_;
    presentInfo.pImageIndices = &currentBuffer;

	if(wait)
	{
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = &wait;
	}

    VPP_CALL(pfQueuePresentKHR(queue, &presentInfo));
}

vk::Extent2D SwapChain::size() const
{
	return {width_, height_};
}

}
