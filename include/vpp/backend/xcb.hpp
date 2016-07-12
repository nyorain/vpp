#pragma once

#define VK_USE_PLATFORM_XCB_KHR

#include <vpp/fwd.hpp>
#include <vpp/surface.hpp>
#include <vpp/context.hpp>

#include <xcb/xcb.h>

namespace vpp
{

///Creates a xcb surface for the given vulkan instance, xcb window and connection.
///Does require the needed extensions to be enabled for the given instance.
Surface createSurface(vk::Instance instance, xcb_connection_t& con, xcb_window_t window);

///Creates a vulkan context for the given xcb window and connection.
Context createContext(xcb_connection_t& con, xcb_window_t window, Context::CreateInfo info);
Context createContext(xcb_connection_t& con, xcb_window_t window);

}
