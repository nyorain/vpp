#pragma once

#define VK_USE_PLATFORM_WIN32_KHR

#include <vpp/surface.hpp>
#include <vpp/vk.hpp>

namespace vpp
{

class Win32Surface : public Surface
{
protected:
    void initSurface(void* hinstance, void* hwnd);

public:
    Win32Surface(VkInstance instance, void* hinstance, void* hwnd);
    virtual ~Win32Surface();
};

}
