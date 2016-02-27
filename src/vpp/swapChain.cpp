#include <vpp/swapChain.hpp>
#include <vpp/procAddr.hpp>
#include <vpp/surface.hpp>
#include <vpp/vulkan/vulkan.hpp>

namespace vpp
{

//SwapChain
SwapChain::SwapChain(const Device& device, const Surface& surface, const vk::Extent2D& extent)
	: SwapChain(device, surface.vkSurface(), extent)
{
}

SwapChain::SwapChain(const Device& device, vk::SurfaceKHR surface, const vk::Extent2D& extent)
{
    create(device, surface, extent);
}

SwapChain::~SwapChain()
{
	destroy();
}

void SwapChain::create(const Device& device, vk::SurfaceKHR surface, const vk::Extent2D& extent)
{
    Resource::create(device);

    surface_ = surface;
	extent_ = extent;

	queryFormats();
	init();
}

void SwapChain::destroy()
{
	destroyBuffers();
	destroySwapchain();
}

void SwapChain::destroyBuffers()
{
	for(auto& buf : buffers_) vk::destroyImageView(vkDevice(), buf.imageView, nullptr);
    buffers_.clear();
}

void SwapChain::destroySwapchain()
{
	VPP_LOAD_DEVICE_PROC(vkDevice(), DestroySwapchainKHR);
    if(vkSwapChain()) fpDestroySwapchainKHR(vkDevice(), vkSwapChain(), nullptr);
}

void SwapChain::init()
{
	VPP_LOAD_DEVICE_PROC(vkDevice(), CreateSwapchainKHR);
	VPP_LOAD_DEVICE_PROC(vkDevice(), GetSwapchainImagesKHR);
	VPP_LOAD_DEVICE_PROC(vkDevice(), DestroySwapchainKHR);

	destroyBuffers(); //swapChain will be destroyed later, used for oldSwapchain

    //create swapChain
	auto oldSwapchain = vkSwapChain();

    auto createInfo = swapChainCreateInfo();
    fpCreateSwapchainKHR(vkDevice(), &createInfo, nullptr, &swapChain_);

	if(oldSwapchain)
	{
		fpDestroySwapchainKHR(vkDevice(), oldSwapchain, nullptr);
		oldSwapchain = 0;
	}

    //create buffers
    std::uint32_t count;
    fpGetSwapchainImagesKHR(vkDevice(), vkSwapChain(), &count, nullptr);

    std::vector<VkImage> imgs(count);
    fpGetSwapchainImagesKHR(vkDevice(), vkSwapChain(), &count, imgs.data());

    buffers_.reserve(count);
    for(auto& img : imgs) buffers_.push_back(createBuffer(img));
}

void SwapChain::resize(const vk::Extent2D& extent)
{
	extent_ = extent;
	init();
}

void SwapChain::queryFormats()
{
	VPP_LOAD_INSTANCE_PROC(vkInstance(), GetPhysicalDeviceSurfaceFormatsKHR);

	//get formats
    uint32_t count;
    fpGetPhysicalDeviceSurfaceFormatsKHR(vkPhysicalDevice(), vkSurface(), &count, nullptr);

	if(!count)
	{
		throw std::runtime_error("SwapChain::queryFormats: Surface has no valid formats.");
	}

    std::vector<vk::SurfaceFormatKHR> formats(count);
    fpGetPhysicalDeviceSurfaceFormatsKHR(vkPhysicalDevice(), vkSurface(), &count,
		reinterpret_cast<VkSurfaceFormatKHR*>(formats.data()));

	//choose best one
    if(formats.size() == 1 && formats[0].format() == vk::Format::Undefined)
	{
		format_ = vk::Format::B8G8R8A8Unorm; //no preferred format from surface
	}
	else
	{
		format_ = formats[0].format();
	}

	colorSpace_ = formats[0].colorSpace();
}

SwapChain::Buffer SwapChain::createBuffer(VkImage img) const
{
	vk::ComponentMapping components(
		vk::ComponentSwizzle::R,
		vk::ComponentSwizzle::G,
		vk::ComponentSwizzle::B,
		vk::ComponentSwizzle::A
	);

	vk::ImageSubresourceRange range{};
	range.aspectMask(vk::ImageAspectFlagBits::Color);
	range.levelCount(1);
	range.layerCount(1);

	vk::ImageViewCreateInfo info{};
	info.format(format());
	info.subresourceRange(range);
	info.viewType(vk::ImageViewType::e2D);
	info.components(components);
	info.image(img);

    VkImageView view;
    vk::createImageView(vkDevice(), &info, nullptr, &view);

    return {img, view};
}

VkSwapchainCreateInfoKHR SwapChain::swapChainCreateInfo()
{
	VPP_LOAD_INSTANCE_PROC(vkInstance(), GetPhysicalDeviceSurfacePresentModesKHR);
	VPP_LOAD_INSTANCE_PROC(vkInstance(), GetPhysicalDeviceSurfaceCapabilitiesKHR);

	//presentModes
    uint32_t count;
    fpGetPhysicalDeviceSurfacePresentModesKHR(vkPhysicalDevice(), vkSurface(), &count, nullptr);

    std::vector<vk::PresentModeKHR> presentModes(count);
    fpGetPhysicalDeviceSurfacePresentModesKHR(vkPhysicalDevice(), vkSurface(), &count,
		reinterpret_cast<VkPresentModeKHR*>(presentModes.data()));

    vk::PresentModeKHR presentMode = vk::PresentModeKHR::FifoKHR;
    for(auto& mode : presentModes)
    {
        if(mode == vk::PresentModeKHR::MailboxKHR)
        {
            presentMode = vk::PresentModeKHR::MailboxKHR;
            break;
        }
        else if(mode == vk::PresentModeKHR::ImmediateKHR)
        {
            presentMode = vk::PresentModeKHR::ImmediateKHR;
        }
    }

	//capabilities
	vk::SurfaceCapabilitiesKHR surfCaps;
	fpGetPhysicalDeviceSurfaceCapabilitiesKHR(vkPhysicalDevice(), vkSurface(), &surfCaps.vkHandle());

    //extents
    if(surfCaps.currentExtent().width() > 1)
        extent_ = surfCaps.currentExtent();

    //number of images
    std::uint32_t imagesCount = surfCaps.minImageCount() + 1;
    if((surfCaps.maxImageCount() > 0) && (imagesCount > surfCaps.maxImageCount()))
        imagesCount = surfCaps.maxImageCount();

    //transform
    auto preTransform = surfCaps.currentTransform();
    if (surfCaps.supportedTransforms() & vk::SurfaceTransformFlagBitsKHR::IdentityKHR)
        preTransform = vk::SurfaceTransformFlagBitsKHR::IdentityKHR;

	//createInfo
    vk::SwapchainCreateInfoKHR ret{};
    ret.surface(vkSurface());
    ret.minImageCount(imagesCount);
    ret.imageExtent(extent_);
    ret.imageUsage(vk::ImageUsageFlagBits::ColorAttachment);
    ret.preTransform(preTransform);
    ret.imageArrayLayers(1);
    ret.queueFamilyIndexCount(0);
    ret.pQueueFamilyIndices(nullptr);
    ret.clipped(true);
	ret.oldSwapchain(vkSwapChain());
    ret.compositeAlpha(vk::CompositeAlphaFlagBitsKHR::OpaqueKHR);
    ret.imageFormat(format());
    ret.imageColorSpace(colorSpace());
    ret.presentMode(presentMode);

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
    presentInfo.swapchainCount(1);
    presentInfo.pSwapchains(&swapChain_);
    presentInfo.pImageIndices(&currentBuffer);

    fpQueuePresentKHR(queue, &presentInfo.vkHandle());
}

}
