#pragma once

#define VK_USE_PLATFORM_WAYLAND_KHR

#include <vpp/fwd.hpp>
#include <vpp/surface.hpp>
#include <vpp/context.hpp>

#include <wayland-client-core.h>

namespace vpp
{

///Creates a wayland surface for the given vulkan instance, wayland surface and display.
///Does require the needed extensions to be enabled for the given instance.
Surface createSurface(vk::Instance instance, struct wl_display& dislay, struct wl_surface& surface);

///Creates a vulkan context for the given wayland surface and display.
Context createContext(struct wl_display& dpy, struct wl_surface& srf, Context::CreateInfo info);
Context createContext(struct wl_display& dpy, struct wl_surface& srf);

}
