#include <vpp/backend/wayland.hpp>

namespace vpp
{

WaylandSurface::WaylandSurface(vk::Instance instance, wl_display& display, wl_surface& surface) 
{
	instance_ = instance;
    initSurface(display, surface);
}

WaylandSurface::~WaylandSurface()
{
	instance_ = {};
}

void WaylandSurface::initSurface(wl_display& display, wl_surface& surface)
{
	vk::WaylandSurfaceCreateInfoKHR info;
    info.display = &display;
    info.surface = &surface;

	vk::createWaylandSurfaceKHR(vkInstance(), &info, nullptr, &surface_);
}

}
