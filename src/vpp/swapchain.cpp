// Copyright (c) 2016-2019 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#include <vpp/swapchain.hpp>
#include <vpp/vk.hpp>
#include <vpp/queue.hpp>
#include <vpp/handles.hpp>
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

	auto phdev = dev.vkPhysicalDevice();

	auto formats = vk::getPhysicalDeviceSurfaceFormatsKHR(phdev, surface);
	auto modes = vk::getPhysicalDeviceSurfacePresentModesKHR(phdev, surface);
	if(formats.empty() || modes.empty()) {
		throw std::runtime_error("vpp::swapchainCreateInfo: "
			"could not get any formats or modes");
	}


	// caps
	auto surfCaps = vk::getPhysicalDeviceSurfaceCapabilitiesKHR(phdev, surface);

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

	// for(auto& format : formats) {
	// 	dlg_trace("swapchain format: {}", (int) format.format);
	// 	dlg_trace("swapchain color space: {}", (int) format.colorSpace);
	// }

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
	auto count = std::max<unsigned int>(surfCaps.minImageCount, prefs.minImageCount);
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
		: ResourceHandle(dev) {
	handle_ = vk::createSwapchainKHR(device(), info);
}

Swapchain::Swapchain(const Device& dev, vk::SwapchainKHR swapChain)
	: ResourceHandle(dev, swapChain) {
}

Swapchain::~Swapchain() {
	if(vkHandle()) {
		vk::destroySwapchainKHR(device(), vkHandle());
	}
}

std::vector<vk::Image> Swapchain::images() const {
	return vk::getSwapchainImagesKHR(device(), vkHandle());
}

void Swapchain::resize(const vk::Extent2D& size,
		vk::SwapchainCreateInfoKHR& info) {
	dlg_assert(info.surface);
	auto surfCaps = vk::getPhysicalDeviceSurfaceCapabilitiesKHR(
		vkPhysicalDevice(), info.surface);

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
	handle_ = vk::createSwapchainKHR(device(), info);

	if(info.oldSwapchain) {
		vk::destroySwapchainKHR(device(), info.oldSwapchain);
	}
}

// TODO: rework with vkpp error handling
// we don't want errors such as outOfDate or suboptimal to throw
// here since that is an expected case: e.g. when the surface is resizing
vk::Result Swapchain::acquire(unsigned int& id, vk::Semaphore sem,
		vk::Fence fence, std::uint64_t timeout) const {
	// TODO: suboptimal gets lost currently...
	try {
		id = vk::acquireNextImageKHR(device(), vkHandle(), timeout,
			sem, fence);
		return vk::Result::success;
	} catch(const vk::VulkanError& error) {
		return error.error;
	}
}

vk::Result Swapchain::present(const Queue& queue, std::uint32_t currentBuffer,
		vk::Semaphore wait) const {
	vk::PresentInfoKHR presentInfo {};
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = &vkHandle();
	presentInfo.pImageIndices = &currentBuffer;

	if(wait) {
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = &wait;
	}

	QueueLock(device(), queue);
	try {
		return vk::queuePresentKHR(queue, presentInfo);
	} catch(const vk::VulkanError& error) {
		return error.error;
	}
}

} // namespace vpp
