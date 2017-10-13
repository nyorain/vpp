// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#include <vpp/swapchain.hpp>
#include <vpp/vk.hpp>
#include <vpp/procAddr.hpp>
#include <vpp/queue.hpp>
#include <vpp/surface.hpp>
#include <vpp/image.hpp>
#include <dlg/dlg.hpp>

namespace vpp {
namespace {

// might not be the best rating
constexpr struct PresentModeRating {
	vk::PresentModeKHR mode;
	unsigned int rating;
} presentModeRatings[] = {
	{vk::PresentModeKHR::immediate, 1},
	{vk::PresentModeKHR::fifoRelaxed, 2},
	{vk::PresentModeKHR::fifo, 3},
	{vk::PresentModeKHR::mailbox, 4}
};

// performc the specifid error action for DefaultSwapchainSettings
void onError(DefaultSwapchainSettings::ErrorAction action, const char* field)
{
	if(action == DefaultSwapchainSettings::ErrorAction::output)
		dlg_warnt(("DefaultSwapchainSettings"), "using different {}", field);

	static const std::string errorMsg = "vpp::DefaultSwapchainSettings: using different ";
	if(action == DefaultSwapchainSettings::ErrorAction::throwException)
		throw std::runtime_error(errorMsg + field);
}

} // anonymous util namespace


// SwapchainSettings
DefaultSwapchainSettings::DefaultSwapchainSettings() :
	prefFormat(vk::Format::r8g8b8a8Unorm),
	prefPresentMode(vk::PresentModeKHR::mailbox),
	prefAlpha(vk::CompositeAlphaBitsKHR::opaque),
	prefTransform(vk::SurfaceTransformBitsKHR::identity),
	prefUsage() {}

const SwapchainSettings& SwapchainSettings::instance()
{
	static SwapchainSettings ret;
	return ret;
}

vk::SwapchainCreateInfoKHR SwapchainSettings::parse(const vk::SurfaceCapabilitiesKHR& caps,
	nytl::Span<const vk::PresentModeKHR> modes,
	nytl::Span<const vk::SurfaceFormatKHR> formats,
	const vk::Extent2D& size) const
{
	vk::SwapchainCreateInfoKHR ret;

	// choose present mode
	ret.presentMode = modes[0];
	auto best = 0u;
	for(auto& mode : modes) {
		for(auto rating : presentModeRatings) {
			if(mode == rating.mode && rating.rating > best) {
				ret.presentMode = mode;
				best = rating.rating;
			}
		}
	}

	// format
	// if there is no preferred format from surface, we use rgba32
	ret.imageFormat = formats[0].format;
	ret.imageColorSpace = formats[0].colorSpace;
	if(formats.size() == 1 && formats[0].format == vk::Format::undefined) {
		ret.imageFormat = vk::Format::r8g8b8a8Unorm;
	}

	// size
	// if the size is equal the zero the size has to be chosen
	// manually (using the given size)
	auto& curr = caps.currentExtent;
	if(curr.width == 0xFFFFFFFF && curr.height == 0xFFFFFFFF) {
		const auto& min = caps.minImageExtent;
		const auto& max = caps.minImageExtent;
		ret.imageExtent.width = std::clamp(size.width, min.width, max.width);
		ret.imageExtent.height = std::clamp(size.height, min.height, max.height);
	} else {
		ret.imageExtent = caps.currentExtent;
	}

	// number of images to create
	ret.minImageCount = caps.minImageCount + 1;
	if((caps.maxImageCount > 0) && (ret.minImageCount > caps.maxImageCount))
		ret.minImageCount = caps.maxImageCount;

	// transform, prefer identity
	ret.preTransform = caps.currentTransform;
	if(caps.supportedTransforms & vk::SurfaceTransformBitsKHR::identity)
		ret.preTransform = vk::SurfaceTransformBitsKHR::identity;

	// alpha, prefer inherit
	auto supAlpha = caps.supportedCompositeAlpha;
	ret.compositeAlpha = vk::CompositeAlphaBitsKHR::opaque;
	if(supAlpha & vk::CompositeAlphaBitsKHR::inherit) {
		ret.compositeAlpha = vk::CompositeAlphaBitsKHR::inherit;
	} else if(supAlpha & vk::CompositeAlphaBitsKHR::postMultiplied) {
		ret.compositeAlpha = vk::CompositeAlphaBitsKHR::postMultiplied;
	} else if(supAlpha & vk::CompositeAlphaBitsKHR::preMultiplied) {
		ret.compositeAlpha = vk::CompositeAlphaBitsKHR::preMultiplied;
	}

	// createInfo
	ret.imageUsage = vk::ImageUsageBits::colorAttachment;
	ret.imageArrayLayers = 1;
	ret.queueFamilyIndexCount = 0;
	ret.clipped = true;
	return ret;
}

// DefaultSwapchainSettings
vk::SwapchainCreateInfoKHR DefaultSwapchainSettings::parse(const vk::SurfaceCapabilitiesKHR& caps,
	nytl::Span<const vk::PresentModeKHR> modes,
	nytl::Span<const vk::SurfaceFormatKHR> formats,
	const vk::Extent2D& size) const
{
	vk::SwapchainCreateInfoKHR ret;

	// choose present mode
	ret.presentMode = modes[0];
	auto best = 0u;
	for(auto& mode : modes) {
		if(mode == prefPresentMode) {
			ret.presentMode = mode;
			break;
		}

		for(auto rating : presentModeRatings) {
			if(mode == rating.mode && rating.rating > best) {
				ret.presentMode = mode;
				best = rating.rating;
			}
		}
	}

	if(ret.presentMode != prefPresentMode) onError(errorAction, "presentMode");
	prefPresentMode = ret.presentMode;

	// format
	ret.imageFormat = formats[0].format;
	ret.imageColorSpace = formats[0].colorSpace;

	if(formats.size() == 1 && formats[0].format == vk::Format::undefined) {
		ret.imageFormat = prefFormat;
	} else {
		for(auto& format : formats) {
			if(format.format == prefFormat)
			{
				ret.imageFormat = format.format;
				ret.imageColorSpace = format.colorSpace;
				break;
			} else if(format.format == vk::Format::r8g8b8a8Unorm) {
				ret.imageFormat = format.format;
				ret.imageColorSpace = format.colorSpace;
			}
		}

		if(ret.imageFormat != prefFormat) onError(errorAction, "format");
		prefFormat = ret.imageFormat;
	}

	// size as in basic implementation
	auto& curr = caps.currentExtent;
	if(curr.width == 0xFFFFFFFF && curr.height == 0xFFFFFFFF) {
		const auto& min = caps.minImageExtent;
		const auto& max = caps.minImageExtent;
		ret.imageExtent.width = std::clamp(size.width, min.width, max.width);
		ret.imageExtent.height = std::clamp(size.height, min.height, max.height);
	} else {
		ret.imageExtent = caps.currentExtent;
	}

	// number of images
	ret.minImageCount = caps.minImageCount + 1;
	if((caps.maxImageCount > 0) && (ret.minImageCount > caps.maxImageCount))
		ret.minImageCount = caps.maxImageCount;

	// transform
	ret.preTransform = caps.currentTransform;
	if(caps.supportedTransforms & prefTransform)
		ret.preTransform = prefTransform;
	else if(caps.supportedTransforms & vk::SurfaceTransformBitsKHR::identity)
		ret.preTransform = vk::SurfaceTransformBitsKHR::identity;

	if(ret.preTransform != prefTransform) onError(errorAction, "transform");
	prefTransform = ret.preTransform;

	// alpha
	ret.compositeAlpha = vk::CompositeAlphaBitsKHR::opaque;
	if(caps.supportedCompositeAlpha & prefAlpha) {
		ret.compositeAlpha = prefAlpha;
	} else if(caps.supportedCompositeAlpha & vk::CompositeAlphaBitsKHR::inherit) {
		ret.compositeAlpha = vk::CompositeAlphaBitsKHR::inherit;
	}

	if(ret.compositeAlpha != prefAlpha) {
		onError(errorAction, "alpha");
	}
	
	prefAlpha = ret.compositeAlpha;

	// createInfo
	ret.imageUsage = vk::ImageUsageBits::colorAttachment | prefUsage;
	ret.imageArrayLayers = 1;
	ret.queueFamilyIndexCount = 0;
	ret.clipped = true;

	return ret;
}


// Swapchain
Swapchain::Swapchain(const Device& dev, vk::SurfaceKHR surface, 
	vk::Extent2D& size, const SwapchainSettings& settings,
	std::vector<std::pair<vk::Image, ImageView>>* views)
		: ResourceHandle(dev)
{
	resize(surface, size, settings, views);
}

Swapchain::Swapchain(const Device& dev, vk::SwapchainKHR swapChain) 
	: ResourceHandle(dev, swapChain)
{
}

Swapchain::~Swapchain()
{
	if(vkHandle()) {
		VPP_LOAD_PROC(device(), DestroySwapchainKHR);
		pfDestroySwapchainKHR(device(), vkHandle(), nullptr);
	}
}

std::vector<vk::Image> Swapchain::images() const
{
	VPP_LOAD_PROC(vkDevice(), GetSwapchainImagesKHR);

	std::uint32_t c;
	VPP_CALL(pfGetSwapchainImagesKHR(device(), vkHandle(), &c, nullptr));

	std::vector<vk::Image> imgs(c);
	VPP_CALL(pfGetSwapchainImagesKHR(device(), vkHandle(), &c, imgs.data()));

	return imgs;
}

/*
void Swapchain::createBuffers()
{
	VPP_LOAD_PROC(vkDevice(), GetSwapchainImagesKHR);

	// get swapchain images
	std::uint32_t cnt;
	VPP_CALL(pfGetSwapchainImagesKHR(device(), vkHandle(), &cnt, nullptr));

	std::vector<vk::Image> imgs(cnt);
	VPP_CALL(pfGetSwapchainImagesKHR(device(), vkHandle(), &cnt, imgs.data()));

	// create imageviews and insert buffers
	buffers_.reserve(cnt);
	for(auto& img : imgs) {
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
		info.format = format_;
		info.subresourceRange = range;
		info.viewType = vk::ImageViewType::e2d;
		info.components = components;
		info.image = img;

		auto view = vk::createImageView(device(), info);
		buffers_.push_back({img, view});
	}
}
*/

void Swapchain::resize(vk::SurfaceKHR surface, vk::Extent2D& size, 
	const SwapchainSettings& settings,
	std::vector<std::pair<vk::Image, ImageView>>* views)
{
	dlg_assert(surface);
	dlg_assert(size.width > 0 && size.height > 0);

	VPP_LOAD_PROC(vkDevice(), CreateSwapchainKHR);
	VPP_LOAD_PROC(vkDevice(), DestroySwapchainKHR);
	VPP_LOAD_PROC(vkInstance(), GetPhysicalDeviceSurfacePresentModesKHR);
	VPP_LOAD_PROC(vkInstance(), GetPhysicalDeviceSurfaceFormatsKHR);
	VPP_LOAD_PROC(vkInstance(), GetPhysicalDeviceSurfaceCapabilitiesKHR);

	// (re)create swapChain
	// use the oldSwapchain in the createInfo for resource reusing
	auto oldSwapchain = vkHandle();

	// query formats
	uint32_t count;
	VPP_CALL(pfGetPhysicalDeviceSurfaceFormatsKHR(vkPhysicalDevice(),
		surface, &count, nullptr));
	if(!count) {
		throw std::runtime_error("vpp::Swapchain::resize: "
			"could not get any formats");
	}

	std::vector<vk::SurfaceFormatKHR> formats(count);
	VPP_CALL(pfGetPhysicalDeviceSurfaceFormatsKHR(vkPhysicalDevice(),
		surface, &count, formats.data()));

	// present modes
	count = 0u;
	VPP_CALL(pfGetPhysicalDeviceSurfacePresentModesKHR(vkPhysicalDevice(), 
		surface, &count, nullptr));
	if(!count) {
		throw std::runtime_error("vpp::Swapchain::resize: "
			"could not get any present modes");
	}

	std::vector<vk::PresentModeKHR> presentModes(count);
	VPP_CALL(pfGetPhysicalDeviceSurfacePresentModesKHR(vkPhysicalDevice(),
		surface, &count, presentModes.data()));

	// caps
	vk::SurfaceCapabilitiesKHR surfCaps;
	VPP_CALL(pfGetPhysicalDeviceSurfaceCapabilitiesKHR(vkPhysicalDevice(), 
		surface, &surfCaps));

	auto createInfo = settings.parse(surfCaps, presentModes, formats, size);
	createInfo.surface = surface;
	createInfo.oldSwapchain = vkHandle();
	VPP_CALL(pfCreateSwapchainKHR(device(), &createInfo, nullptr, &handle_));

	size.width = createInfo.imageExtent.width;
	size.height = createInfo.imageExtent.height;

	if(oldSwapchain) {
		pfDestroySwapchainKHR(device(), oldSwapchain, nullptr);
		oldSwapchain = {};
	}

	if(views) {
		auto imgs = images();
		views->reserve(views->size() + imgs.size());

		static const vk::ComponentMapping components{
			vk::ComponentSwizzle::r,
			vk::ComponentSwizzle::g,
			vk::ComponentSwizzle::b,
			vk::ComponentSwizzle::a
		};

		static const vk::ImageSubresourceRange range {
			vk::ImageAspectBits::color, 1, 1
		};

		vk::ImageViewCreateInfo viewInfo {};
		viewInfo.format = createInfo.imageFormat;
		viewInfo.subresourceRange = range;
		viewInfo.viewType = vk::ImageViewType::e2d;
		viewInfo.components = components;

		for(auto& img : imgs) {
			viewInfo.image = img;
			views->push_back({img, ImageView {device(), viewInfo}});
		}
	}
}

vk::Result Swapchain::acquire(unsigned int& id, vk::Semaphore sem, 
	vk::Fence fence, std::uint64_t timeout) const
{
	VPP_LOAD_PROC(vkDevice(), AcquireNextImageKHR);
	auto ret = pfAcquireNextImageKHR(device(), vkHandle(), timeout, 
		sem, fence, &id);

	return ret;
}

vk::Result Swapchain::present(const Queue& queue, std::uint32_t currentBuffer,
	vk::Semaphore wait) const
{
	VPP_LOAD_PROC(vkDevice(), QueuePresentKHR);

	vk::PresentInfoKHR presentInfo {};
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = &vkHandle();
	presentInfo.pImageIndices = &currentBuffer;

	if(wait) {
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = &wait;
	}

	QueueLock(device(), queue);
	auto ret = pfQueuePresentKHR(queue, &presentInfo);
	return ret;
}

} // namespace vpp
