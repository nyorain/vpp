// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#include <vpp/backend/win32.hpp>
#include <vpp/procAddr.hpp>

namespace vpp
{

Surface createSurface(vk::Instance instance, HWND window, HINSTANCE module)
{
	if(module == nullptr) module = ::GetModuleHandle(nullptr);

	vk::Win32SurfaceCreateInfoKHR info;
	info.hinstance = module;
	info.hwnd = window;

	vk::SurfaceKHR ret;
	VPP_PROC(instance, CreateWin32SurfaceKHR)(instance, &info, nullptr, &ret);
	return {instance, ret};
}

Context createContext(HWND window, Context::CreateInfo info, HINSTANCE module)
{
	info.instanceExtensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);

	Context ret;
	ret.initInstance(info.debugFlags, info.instanceExtensions,
		info.instanceLayers, info.reverseInstanceLayers);

	auto vsurface = createSurface(ret.vkInstance(), window, module);
	ret.initSurface(std::move(vsurface));

	ret.initDevice(info.deviceExtensions, info.deviceLayers, info.reverseDeviceLayers);
	ret.initSwapChain({info.width, info.height}, info.swapChainSettings);

	return ret;
}

Context createContext(HWND window, HINSTANCE module)
{
	return createContext(window, {}, module);
}

} // namespace vpp
