// Copyright (c) 2016-2018 nyorain
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

// performc the specifid error action for DefaultSwapchainSettings
void onError(SwapchainPreferences::ErrorAction action, const char* field) {
	if(action == SwapchainPreferences::ErrorAction::output) {
		dlg_warnt(("swapchain"), "using different {}", field);
	} else if(action == SwapchainPreferences::ErrorAction::exception) {
		throw std::runtime_error("vpp::swapchainCreateInfo: "
			"could not use " + std::string(field));
	}
}

bool srgbFormat(vk::Format f) {
	switch(f) {
		case vk::Format::r8g8b8a8Srgb: return true;
		case vk::Format::b8g8r8a8Srgb: return true;
		case vk::Format::r8g8b8Srgb: return true;
		case vk::Format::b8g8r8Srgb: return true;
		default: return false;
	}
}

bool linearFormat(vk::Format f) {
	switch(f) {
		case vk::Format::r8g8b8a8Unorm: return true;
		case vk::Format::b8g8r8a8Unorm: return true;
		case vk::Format::r8g8b8Unorm: return true;
		case vk::Format::b8g8r8Unorm: return true;
		default: return false;
	}
}

} // anonymous util namespace

// CreatInfo
SwapchainPreferences::SwapchainPreferences() :
	format(preferSrgb ?
		vk::Format::b8g8r8a8Srgb :
		vk::Format::b8g8r8a8Unorm),
	presentMode(vk::PresentModeKHR::mailbox),
	alpha(vk::CompositeAlphaBitsKHR::opaque),
	transform(vk::SurfaceTransformBitsKHR::identity),
	usage(vk::ImageUsageBits::colorAttachment) {}

vk::SwapchainCreateInfoKHR swapchainCreateInfo(const vpp::Device& dev,
		vk::SurfaceKHR surface, const vk::Extent2D& size,
		const SwapchainPreferences& prefs) {

	// query information
	VPP_LOAD_PROC(dev.vkInstance(), GetPhysicalDeviceSurfacePresentModesKHR);
	VPP_LOAD_PROC(dev.vkInstance(), GetPhysicalDeviceSurfaceFormatsKHR);
	VPP_LOAD_PROC(dev.vkInstance(), GetPhysicalDeviceSurfaceCapabilitiesKHR);

	auto phdev = dev.vkPhysicalDevice();

	// query formats
	uint32_t count;
	VKPP_CALL(pfGetPhysicalDeviceSurfaceFormatsKHR(phdev,
		surface, &count, nullptr));
	if(!count) {
		throw std::runtime_error("vpp::swapchainCreateInfo: "
			"could not get any formats");
	}

	std::vector<vk::SurfaceFormatKHR> formats(count);
	VKPP_CALL(pfGetPhysicalDeviceSurfaceFormatsKHR(phdev,
		surface, &count, formats.data()));

	// present modes
	count = 0u;
	VKPP_CALL(pfGetPhysicalDeviceSurfacePresentModesKHR(phdev,
		surface, &count, nullptr));
	if(!count) {
		throw std::runtime_error("vpp::swapchainCreateInfo: "
			"could not get any present modes");
	}

	std::vector<vk::PresentModeKHR> modes(count);
	VKPP_CALL(pfGetPhysicalDeviceSurfacePresentModesKHR(phdev,
		surface, &count, modes.data()));

	// caps
	vk::SurfaceCapabilitiesKHR surfCaps;
	VKPP_CALL(pfGetPhysicalDeviceSurfaceCapabilitiesKHR(phdev,
		surface, &surfCaps));


	// parse createInfo
	vk::SwapchainCreateInfoKHR ret;

	// choose present mode
	ret.presentMode = modes[0];
	for(auto& mode : modes) {
		if(mode == prefs.presentMode) {
			ret.presentMode = mode;
			break;
		}
	}

	if(ret.presentMode != prefs.presentMode) {
		onError(prefs.errorAction, "presentMode");
	}

	// format
	ret.imageFormat = formats[0].format;
	ret.imageColorSpace = formats[0].colorSpace;

	if(formats.size() == 1 && formats[0].format == vk::Format::undefined) {
		ret.imageFormat = prefs.format;
	} else {
		for(auto& format : formats) {
			if(format.format == prefs.format) {
				ret.imageFormat = format.format;
				ret.imageColorSpace = format.colorSpace;
				break;
			} else if(linearFormat(format.format) && !prefs.preferSrgb) {
				ret.imageFormat = format.format;
				ret.imageColorSpace = format.colorSpace;
			} else if(srgbFormat(format.format) && prefs.preferSrgb) {
				ret.imageFormat = format.format;
				ret.imageColorSpace = format.colorSpace;
			}
		}

		if(ret.imageFormat != prefs.format) {
			onError(prefs.errorAction, "format");
		}
	}

	// size as in basic implementation
	auto& curr = surfCaps.currentExtent;
	bool choose = curr.width == 0xFFFFFFFF && curr.height == 0xFFFFFFFF;
	if(!prefs.preferCurrentExtent || choose) {
		const auto& min = surfCaps.minImageExtent;
		const auto& max = surfCaps.maxImageExtent;
		ret.imageExtent.width = std::clamp(size.width, min.width, max.width);
		ret.imageExtent.height = std::clamp(size.height, min.height, max.height);
	} else if(prefs.preferCurrentExtent) {
		ret.imageExtent = surfCaps.currentExtent;
	}

	// number of images
	count = std::max<unsigned int>(surfCaps.minImageCount, prefs.minImageCount);
	if(surfCaps.maxImageCount > 0) {
		count = std::min<unsigned int>(surfCaps.maxImageCount, count);
	}

	if(count != prefs.minImageCount) {
		onError(prefs.errorAction, "minImageCount");
	}

	ret.minImageCount = count;

	// transform
	ret.preTransform = surfCaps.currentTransform;
	auto supTrans = surfCaps.supportedTransforms;
	if(supTrans & prefs.transform) {
		ret.preTransform = prefs.transform;
	} else if(supTrans & vk::SurfaceTransformBitsKHR::identity) {
		ret.preTransform = vk::SurfaceTransformBitsKHR::identity;
	}

	if(ret.preTransform != prefs.transform) {
		onError(prefs.errorAction, "transform");
	}

	// alpha
	auto supAlpha = surfCaps.supportedCompositeAlpha;
	if(supAlpha & prefs.alpha) {
		ret.compositeAlpha = prefs.alpha;
	} else {
		for(auto i : {0, 3, 2, 1}) {
			if(supAlpha & (1u << i)) {
				ret.compositeAlpha =
					static_cast<vk::CompositeAlphaBitsKHR>(1u << i);
				break;
			}
		}
	}

	if(ret.compositeAlpha != prefs.alpha) {
		onError(prefs.errorAction, "alpha");
	}

	// usage
	auto usage = prefs.usage & surfCaps.supportedUsageFlags;
	if(prefs.usage != usage) {
		onError(prefs.errorAction, "imageUsage");
	}
	ret.imageUsage = usage;

	ret.imageArrayLayers = 1;
	ret.queueFamilyIndexCount = 0;
	ret.clipped = true;
	ret.surface = surface;

	return ret;
}


// Swapchain
Swapchain::Swapchain(const Device& dev, const vk::SwapchainCreateInfoKHR& info)
	: ResourceHandle(dev)
{
	VPP_LOAD_PROC(dev, CreateSwapchainKHR);
	VKPP_CALL(pfCreateSwapchainKHR(device(), &info, nullptr, &handle_));
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
	VKPP_CALL(pfGetSwapchainImagesKHR(device(), vkHandle(), &c, nullptr));

	std::vector<vk::Image> imgs(c);
	VKPP_CALL(pfGetSwapchainImagesKHR(device(), vkHandle(), &c, imgs.data()));

	return imgs;
}

void Swapchain::resize(const vk::Extent2D& size,
	vk::SwapchainCreateInfoKHR& info)
{
	dlg_assert(info.surface);

	VPP_LOAD_PROC(vkInstance(), GetPhysicalDeviceSurfaceCapabilitiesKHR);
	VPP_LOAD_PROC(device(), DestroySwapchainKHR);
	VPP_LOAD_PROC(device(), CreateSwapchainKHR);

	vk::SurfaceCapabilitiesKHR surfCaps;
	VKPP_CALL(pfGetPhysicalDeviceSurfaceCapabilitiesKHR(vkPhysicalDevice(),
		info.surface, &surfCaps));

	auto& curr = surfCaps.currentExtent;
	if(curr.width == 0xFFFFFFFF && curr.height == 0xFFFFFFFF) {
		const auto& min = surfCaps.minImageExtent;
		const auto& max = surfCaps.maxImageExtent;
		info.imageExtent.width = std::clamp(size.width, min.width, max.width);
		info.imageExtent.height = std::clamp(size.height, min.height, max.height);
	} else {
		info.imageExtent = surfCaps.currentExtent;
	}

	info.oldSwapchain = vkHandle();
	VKPP_CALL(pfCreateSwapchainKHR(device(), &info, nullptr, &handle_));

	if(info.oldSwapchain) {
		pfDestroySwapchainKHR(device(), info.oldSwapchain, nullptr);
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
