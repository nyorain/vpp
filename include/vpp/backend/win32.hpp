#pragma once

#define VK_USE_PLATFORM_WIN32_KHR

#include <vpp/vk.hpp>
#include <vpp/surface.hpp>
#include <vpp/context.hpp>

#include <windows.h>

namespace vpp
{

//Surface
class Win32Surface : public Surface
{
protected:
    void initSurface(void* hinstance, void* hwnd);

public:
    Win32Surface(vk::Instance instance, void* hinstance, void* hwnd);
    virtual ~Win32Surface();
};

//Context
class Win32Context : public Context
{
protected:
	Win32Surface surface_;

public:
	virtual const Surface& surface() const override { return surface_; }
};

}
