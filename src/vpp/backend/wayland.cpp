#include <vpp/backend/wayland.hpp>
#include <vpp/procAddr.hpp>

namespace vpp
{

Surface createSurface(vk::Instance instance, struct wl_display& dpy, struct wl_surface& surface)
{
	vk::WaylandSurfaceCreateInfoKHR info;
    info.display = &dpy;
    info.surface = &surface;

	vk::SurfaceKHR ret;
	VPP_PROC(instance, CreateWaylandSurfaceKHR)(instance, &info, nullptr, &ret);
	return {instance, ret};
}

Context createContext(struct wl_display& dpy, struct wl_surface& surface, Context::CreateInfo info)
{
	info.instanceExtensions.push_back(VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME);

	Context ret;
	ret.initInstance(info);

	auto vsurface = createSurface(ret.vkInstance(), dpy, surface);
	ret.initSurface(std::move(vsurface));

	ret.initDevice(info);
	ret.initSwapChain(info);

	return ret;
}

Context createContext(struct wl_display& display, struct wl_surface& surface)
{
	return createContext(display, surface, {});
}


}
