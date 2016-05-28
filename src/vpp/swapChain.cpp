#include <vpp/swapChain.hpp>
#include <vpp/vk.hpp>
#include <vpp/procAddr.hpp>
#include <vpp/surface.hpp>
#include <vpp/vulkan/vulkan.hpp>

namespace vpp
{

//SwapChain
SwapChain::SwapChain(const Device& device, const Surface& surface, const vk::Extent2D& size)
	: SwapChain(device, surface.vkSurface(), size)
{
}

SwapChain::SwapChain(const Device& device, vk::SurfaceKHR surface, const vk::Extent2D& size)
	: Resource(device), surface_(surface), size_(size)
{
    surface_ = surface;
	size_ = size;

	queryFormats();
	initSwapChain();
}

SwapChain::~SwapChain()
{

	if(!vkSwapChain()) return;

	VPP_LOAD_DEVICE_PROC(vkDevice(), DestroySwapchainKHR);
    fpDestroySwapchainKHR(vkDevice(), vkSwapChain(), nullptr);
	swapChain_ = {};
}

SwapChain::SwapChain(SwapChain&& other) noexcept
{
	swap(*this, other);
}

SwapChain& SwapChain::operator=(SwapChain&& other) noexcept
{
	this->~SwapChain();
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
	swap(a.size_, b.size_);
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
	VPP_LOAD_DEVICE_PROC(vkDevice(), CreateSwapchainKHR);
	VPP_LOAD_DEVICE_PROC(vkDevice(), GetSwapchainImagesKHR);
	VPP_LOAD_DEVICE_PROC(vkDevice(), DestroySwapchainKHR);

	//destory just the buffers, not the swapchain (if there is any)
	destroyBuffers();

    //(re)create swapChain
	//if there is already a swapChain it will be passed as the old swapchain parameter
	auto oldSwapchain = vkSwapChain();

	//create info from seperate function since it requires a lot of querying
    auto createInfo = swapChainCreateInfo();
    fpCreateSwapchainKHR(vkDevice(), &createInfo.vkHandle(), nullptr, &swapChain_);

	if(oldSwapchain)
	{
		fpDestroySwapchainKHR(vkDevice(), oldSwapchain, nullptr);
		oldSwapchain = 0;
	}

    //new buffers
	//get swapchain images
    std::uint32_t count;
    fpGetSwapchainImagesKHR(vkDevice(), vkSwapChain(), &count, nullptr);

    std::vector<VkImage> imgs(count);
    fpGetSwapchainImagesKHR(vkDevice(), vkSwapChain(), &count, imgs.data());

	//create imageviews and insert buffers
    buffers_.reserve(count);
    for(auto& img : imgs)
	{
		vk::ComponentMapping components(
			vk::ComponentSwizzle::r,
			vk::ComponentSwizzle::g,
			vk::ComponentSwizzle::b,
			vk::ComponentSwizzle::a
		);

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

	    VkImageView view;
	    vk::createImageView(vkDevice(), &info, nullptr, &view);

		buffers_.push_back({img, view});
	}
}

void SwapChain::resize(const vk::Extent2D& size)
{
	size_ = size;
	initSwapChain();
}

void SwapChain::queryFormats()
{
	VPP_LOAD_INSTANCE_PROC(vkInstance(), GetPhysicalDeviceSurfaceFormatsKHR);

	//get formats
    uint32_t count;
    fpGetPhysicalDeviceSurfaceFormatsKHR(vkPhysicalDevice(), vkSurface(), &count, nullptr);

	if(!count) throw std::runtime_error("SwapChain::queryFormats: Surface has no valid formats.");

    std::vector<vk::SurfaceFormatKHR> formats(count);
    fpGetPhysicalDeviceSurfaceFormatsKHR(vkPhysicalDevice(), vkSurface(), &count,
		reinterpret_cast<VkSurfaceFormatKHR*>(formats.data()));

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
	VPP_LOAD_INSTANCE_PROC(vkInstance(), GetPhysicalDeviceSurfacePresentModesKHR);
	VPP_LOAD_INSTANCE_PROC(vkInstance(), GetPhysicalDeviceSurfaceCapabilitiesKHR);

	//presentModes
    uint32_t count;
    fpGetPhysicalDeviceSurfacePresentModesKHR(vkPhysicalDevice(), vkSurface(), &count, nullptr);

    std::vector<vk::PresentModeKHR> presentModes(count);
    fpGetPhysicalDeviceSurfacePresentModesKHR(vkPhysicalDevice(), vkSurface(), &count,
		reinterpret_cast<VkPresentModeKHR*>(presentModes.data()));

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
	fpGetPhysicalDeviceSurfaceCapabilitiesKHR(vkPhysicalDevice(), vkSurface(), &surfCaps.vkHandle());

    //size
    if(surfCaps.currentExtent.width > 1)
        size_ = surfCaps.currentExtent;

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
    ret.imageExtent = size_;
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

unsigned int SwapChain::acquireNextImage(vk::Semaphore presentComplete) const
{
	VPP_LOAD_DEVICE_PROC(vkDevice(), AcquireNextImageKHR);

    std::uint32_t ret;
    fpAcquireNextImageKHR(vkDevice(), vkSwapChain(), UINT64_MAX, presentComplete, 0, &ret);
    return ret;
}

void SwapChain::present(vk::Queue queue, std::uint32_t currentBuffer) const
{
	VPP_LOAD_DEVICE_PROC(vkDevice(), QueuePresentKHR);

    vk::PresentInfoKHR presentInfo {};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &swapChain_;
    presentInfo.pImageIndices = &currentBuffer;

    fpQueuePresentKHR(queue, &presentInfo.vkHandle());
}

}
