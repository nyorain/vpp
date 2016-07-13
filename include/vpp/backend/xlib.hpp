#pragma once

#define VK_USE_PLATFORM_XLIB_KHR

#include <vpp/fwd.hpp>
#include <vpp/surface.hpp>
#include <vpp/context.hpp>

#include <X11/Xlib.h>

namespace vpp
{

///Creates a xcb surface for the given vulkan instance, xlib window and display.
///Does require the needed extensions to be enabled for the given instance.
Surface createSurface(vk::Instance instance, Display& dpy, Window window);

///Creates a vulkan context for the given xlib window and display.
Context createContext(Display& con, Window window, Context::CreateInfo info);
Context createContext(Display& con, Window window);

}
