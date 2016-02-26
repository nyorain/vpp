#pragma once

#define VK_USE_PLATFORM_WIN32_KHR

#include <vpp/surface.hpp>
#include <vpp/vk.hpp>

#include <windows.h>

namespace vpp
{

class Win32Surface : public Surface
{
protected:
    void initSurface(void* hinstance, void* hwnd);

public:
    Win32Surface(vk::Instance instance, void* hinstance, void* hwnd);
    virtual ~Win32Surface();
};

}
