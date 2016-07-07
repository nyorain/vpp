#include <vpp/swapChain.hpp>
#include <vpp/vk.hpp>
#include <vpp/procAddr.hpp>
#include <vpp/queue.hpp>
#include <vpp/surface.hpp>
#include <vpp/image.hpp>
#include <vpp/utility/debug.hpp>

namespace vpp
{

//SwapChainSettings
vk::SwapchainCreateInfoKHR SwapChainSettings::parse(const vk::SurfaceCapabilitiesKHR& caps,
	const Range<vk::PresentModeKHR>& modes,
	const Range<vk::SurfaceFormatKHR>& formats,
	const vk::Extent2D& size) const
{
	vk::SwapchainCreateInfoKHR ret;

	//choose present mode
	//mailbox: no tearing, blocks
	//fifo: no tearing, no blocking (using a queue)
	//fifo-relaxed: tearing, no blocking (tearing only if fps > monitor fps)
	//immediate-relaxed: tearing, no blocking (tearing only if fps < monitor fps)
	//mailbox > fifo > fifo_relaxed > immediate
	ret.presentMode = modes[0];
	auto best = 0u;
	for(auto& mode : modes)
	{
		if(mode == vk::PresentModeKHR::mailbox)
		{
			ret.presentMode = mode;
			break;
		}
		else if(mode == vk::PresentModeKHR::fifo && best < 3)
		{
			best = 3u;
			ret.presentMode = mode;
		}
		else if(mode == vk::PresentModeKHR::fifoRelaxed && best < 2)
		{
			best = 2u;
			ret.presentMode = mode;
		}
		else if(mode == vk::PresentModeKHR::immediate && best < 1)
		{
			best = 1u;
			ret.presentMode = mode;
		}
	}

	//format
	ret.imageFormat = formats[0].format;
	ret.imageColorSpace = formats[0].colorSpace;
	if(formats.size() == 1 && formats[0].format == vk::Format::undefined)
		ret.imageFormat = vk::Format::r8g8b8a8Unorm; //no preferred format from surface

	//size
	//if the size is equal the zero the size has to be chosen by manually (using the given size)
	if(caps.currentExtent.width == 0xFFFFFFFF && caps.currentExtent.height == 0xFFFFFFFF)
	{
		ret.imageExtent = size;
		VPP_DEBUG_CHECK(vpp::SwapChainSettings,
		{
			if(!size.width || !size.height) VPP_DEBUG_OUTPUT("Invalid size will be set.");
		});
	}
	else
	{
		ret.imageExtent = caps.currentExtent;
	}

	//number of images
	ret.minImageCount = caps.minImageCount + 1;
	if((caps.maxImageCount > 0) && (ret.minImageCount > caps.maxImageCount))
		ret.minImageCount = caps.maxImageCount;

	//transform
	ret.preTransform = caps.currentTransform;
	if(caps.supportedTransforms & vk::SurfaceTransformBitsKHR::identity)
		ret.preTransform = vk::SurfaceTransformBitsKHR::identity;

	//alpha
	ret.compositeAlpha = vk::CompositeAlphaBitsKHR::opaque;
	if(caps.supportedCompositeAlpha & vk::CompositeAlphaBitsKHR::inherit)
		ret.compositeAlpha = vk::CompositeAlphaBitsKHR::inherit;
	else if(caps.supportedCompositeAlpha & vk::CompositeAlphaBitsKHR::postMultiplied)
		ret.compositeAlpha = vk::CompositeAlphaBitsKHR::postMultiplied;
	else if(caps.supportedCompositeAlpha & vk::CompositeAlphaBitsKHR::preMultiplied)
		ret.compositeAlpha = vk::CompositeAlphaBitsKHR::preMultiplied;

	//createInfo
	ret.imageUsage = vk::ImageUsageBits::colorAttachment;
	ret.imageArrayLayers = 1;
	ret.queueFamilyIndexCount = 0;
	ret.clipped = true;
	return ret;
}

//SwapChain
SwapChain::SwapChain(const Device& device, vk::SurfaceKHR surface, const vk::Extent2D& size,
	const SwapChainSettings& settings) : Resource(device), surface_(surface)
{
	resize(size, settings);
}

SwapChain::~SwapChain()
{
	if(!vkSwapChain()) return;
	destroyBuffers();

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

void SwapChain::resize(const vk::Extent2D& size, const SwapChainSettings& settings)
{
	VPP_LOAD_PROC(vkDevice(), CreateSwapchainKHR);
	VPP_LOAD_PROC(vkDevice(), GetSwapchainImagesKHR);
	VPP_LOAD_PROC(vkDevice(), DestroySwapchainKHR);
	VPP_LOAD_PROC(vkInstance(), GetPhysicalDeviceSurfacePresentModesKHR);
	VPP_LOAD_PROC(vkInstance(), GetPhysicalDeviceSurfaceFormatsKHR);
	VPP_LOAD_PROC(vkInstance(), GetPhysicalDeviceSurfaceCapabilitiesKHR);

	//destory just the buffers, not the swapchain (if there is any)
	destroyBuffers();

	//(re)create swapChain
	//if there is already a swapChain it will be passed as the old swapchain parameter
	auto oldSwapchain = vkSwapChain();

	//formats
	uint32_t count;
	VPP_CALL(pfGetPhysicalDeviceSurfaceFormatsKHR(vkPhysicalDevice(), vkSurface(), &count, nullptr));
	if(!count) throw std::runtime_error("vpp::SwapChain::queryFormats: Surface has no valid formats");

	std::vector<vk::SurfaceFormatKHR> formats(count);
	VPP_CALL(pfGetPhysicalDeviceSurfaceFormatsKHR(vkPhysicalDevice(), vkSurface(), &count,
		formats.data()));

	//present modes
	VPP_CALL(pfGetPhysicalDeviceSurfacePresentModesKHR(vkPhysicalDevice(), vkSurface(), &count,
		nullptr));
	if(!count) throw std::runtime_error("vpp::SwapChain::queryFormats: Surface has no valid modes");

	std::vector<vk::PresentModeKHR> presentModes(count);
	VPP_CALL(pfGetPhysicalDeviceSurfacePresentModesKHR(vkPhysicalDevice(), vkSurface(), &count,
		presentModes.data()));

	//caps
	vk::SurfaceCapabilitiesKHR surfCaps;
	VPP_CALL(pfGetPhysicalDeviceSurfaceCapabilitiesKHR(vkPhysicalDevice(), vkSurface(), &surfCaps));

	//create info from seperate function since it requires a lot of querying
	auto createInfo = settings.parse(surfCaps, presentModes, formats, size);
	createInfo.surface = vkSurface();
	createInfo.oldSwapchain = vkSwapChain();
	VPP_CALL(pfCreateSwapchainKHR(vkDevice(), &createInfo, nullptr, &swapChain_));

	format_ = createInfo.imageFormat;
	width_ = createInfo.imageExtent.width;
	height_ = createInfo.imageExtent.height;

	if(oldSwapchain)
	{
		pfDestroySwapchainKHR(vkDevice(), oldSwapchain, nullptr);
		oldSwapchain = {};
	}

	//new buffers
	//get swapchain images
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
		info.format = createInfo.imageFormat;
		info.subresourceRange = range;
		info.viewType = vk::ImageViewType::e2d;
		info.components = components;
		info.image = img;

		auto view = vk::createImageView(vkDevice(), info);
		buffers_.push_back({img, view});
	}
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

	std::lock_guard<std::mutex> guard(queue.mutex());
	VPP_CALL(pfQueuePresentKHR(queue, &presentInfo));
}

vk::Extent2D SwapChain::size() const
{
	return {width_, height_};
}

}
