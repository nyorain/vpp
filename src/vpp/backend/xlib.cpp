#include <vpp/backend/xlib.hpp>

namespace vpp
{

Surface createSurface(vk::Instance instance, Display& dpy, Window window);
{
	if(module == nullptr) module = ::GetModuleHandle(nullptr);

	vk::XlibSurfaceCreateInfoKHR info;
    info.dpy = &dpy;
    info.window = window;

	vk::SurfaceKHR ret;
	VPP_PROC(instance, CreateXlibSurfaceKHR)(instance, &info, nullptr, &ret);
	return {instance, ret};
}

Context createContext(Display& dpy, Window window, Context::CreateInfo info);
{
	info.instanceExtensions.push_back(VK_KHR_XLIB_SURFACE_EXTENSION_NAME);

	Context ret;
	ret.initInstance(info);

	auto vsurface = createSurface(ret.vkInstance(), dpy, window);
	ret.initSurface(std::move(vsurface));

	ret.initDevice(info);
	ret.initSwapChain(info);

	return ret;
}

Context createContext(Display& dpy, Window window);
{
	return createContext(dpy, window, {});
}


}
