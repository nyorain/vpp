#include <vpp/backend/wayland.hpp>

namespace vpp
{

Surface createSurface(vk::Instance instance, struct wl_display& dislay, struct wl_surface& surface);
{
	if(module == nullptr) module = ::GetModuleHandle(nullptr);

	vk::WaylandSurfaceCreateInfoKHR info;
    info.display = &display;
    info.surface = &surface;

	vk::SurfaceKHR ret;
	VPP_PROC(instance, CreateWaylandSurfaceKHR)(instance, &info, nullptr, &ret);
	return {instance, ret};
}

Context createContext(struct wl_display& dpy, struct wl_surface& surface, Context::CreateInfo info);
{
	info.instanceExtensions.push_back(VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME);

	Context ret;
	ret.initInstance(info);

	auto vsurface = createSurface(ret.vkInstance(), cpy, surface);
	ret.initSurface(std::move(vsurface));

	ret.initDevice(info);
	ret.initSwapChain(info);

	return ret;
}

Context createContext(struct wl_display& display, struct wl_surface& surface);
{
	return createContext(display, surface, {});
}


}
