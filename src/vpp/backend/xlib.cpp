#include <vpp/backend/xlib.hpp>
#include <vpp/procAddr.hpp>
#include <vpp/util/span.hpp>

namespace vpp
{

Surface createSurface(vk::Instance instance, Display& dpy, Window window)
{
	vk::XlibSurfaceCreateInfoKHR info;
	info.dpy = &dpy;
	info.window = window;

	vk::SurfaceKHR ret;
	VPP_PROC(instance, CreateXlibSurfaceKHR)(instance, &info, nullptr, &ret);
	return {instance, ret};
}

Context createContext(Display& dpy, Window window, Context::CreateInfo info)
{
	info.instanceExtensions.push_back(VK_KHR_XLIB_SURFACE_EXTENSION_NAME);

	Context ret;
	ret.initInstance(info.debugFlags, info.instanceExtensions,
		info.instanceLayers, info.reverseInstanceLayers);

	auto vsurface = createSurface(ret.vkInstance(), dpy, window);
	ret.initSurface(std::move(vsurface));

	ret.initDevice(info.deviceExtensions, info.deviceLayers, info.reverseDeviceLayers);
	ret.initSwapchain({info.width, info.height}, info.swapChainSettings);

	return ret;
}

Context createContext(Display& dpy, Window window)
{
	return createContext(dpy, window, {});
}


}
