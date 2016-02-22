#pragma once

#include <vpp/vk.hpp>
#include <vector>
#include <string>

namespace vpp
{

///Stores information about the used backend/platform.
class Platform
{
public:
	bool wsi = 1;
	std::vector<std::string> instanceExtensions {};
	std::vector<std::string> deviceExtensions {};

public:
	inline Platform(bool use = 1);
};

//inline, will be created in using source
Platform::Platform(bool use)
{
	if(!use)
	{
		wsi = 0;
		return;
	}

	#if defined(VK_USE_PLATFORM_WIN32_KHR)
		instanceExtensions = {VK_KHR_SURFACE_EXTENSION_NAME, VK_KHR_WIN32_SURFACE_EXTENSION_NAME};

	#elif defined(VK_USE_PLATFORM_XLIB_KHR)
		instanceExtensions = {VK_KHR_SURFACE_EXTENSION_NAME, VK_KHR_XLIB_SURFACE_EXTENSION_NAME};

	#elif defined(VK_USE_PLATFORM_XCB_KHR)
		instanceExtensions = VK_KHR_SURFACE_EXTENSION_NAME, VK_KHR_XCB_SURFACE_EXTENSION_NAME};

	#elif defined(VK_USE_PLATFORM_WAYLAND_KHR)
		instanceExtensions = {VK_KHR_SURFACE_EXTENSION_NAME, VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME};

	#elif defined(VK_USE_PLATFORM_MIR_KHR)
		instanceExtensions = {VK_KHR_SURFACE_EXTENSION_NAME, VK_KHR_MIR_SURFACE_EXTENSION_NAME};

	#elif defined(VK_USE_PLATFORM_ANDROID_KHR)
		instanceExtensions = {VK_KHR_SURFACE_EXTENSION_NAME, VK_KHR_ANDROID_SURFACE_EXTENSION_NAME};

	#else
		wsi = 0;

	#endif

	if(wsi) deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
}

}
