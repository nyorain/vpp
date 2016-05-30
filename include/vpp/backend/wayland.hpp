#pragma once

#define VK_USE_PLATFORM_WAYLAND_KHR

#include <vpp/surface.hpp>
#include <vpp/context.hpp>
#include <vpp/vk.hpp>

#include <wayland-client-core.h>

namespace vpp
{

///Surface class for the wayland wsi backend.
class WaylandSurface : public Surface
{
protected:
    void initSurface(wl_display& display, wl_surface& surface);

public:
    WaylandSurface(vk::Instance instance, wl_display& display, wl_surface& surface);
    virtual ~WaylandSurface();
};

///Context class for the wayland wsi backend.
class WaylandContext : public Context
{
public:
	WaylandContext() = default;
	WaylandContext(const CreateInfo& info, wl_display& display, wl_surface& surface);
	virtual ~WaylandContext();

protected:
	WaylandSurface surface_;
};

}
