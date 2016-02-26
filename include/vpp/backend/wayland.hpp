#pragma once

#define VK_USE_PLATFORM_WAYLAND_KHR

#include <vpp/surface.hpp>
#include <vpp/vk.hpp>

#include <wayland-client-core.h>

namespace vpp
{

class WaylandSurface : public Surface
{
protected:
    void initSurface(wl_display& display, wl_surface& surface);

public:
    WaylandSurface(vk::Instance instance, wl_display& display, wl_surface& surface);
    virtual ~WaylandSurface();
};

}
