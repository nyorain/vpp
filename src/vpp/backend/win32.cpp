#include <vpp/backend/win32.hpp>
#include <vpp/call.hpp>
#include <windows.h>

namespace vpp
{

Win32Surface::Win32Surface(VkInstance instance, void* hinstance, void* hwnd) : Surface(instance)
{
    initSurface(hinstance, hwnd);
}

Win32Surface::~Win32Surface()
{
    Surface::destroy();
}

void Win32Surface::initSurface(void* hinstance, void* hwnd)
{
    VkWin32SurfaceCreateInfoKHR info {};
    info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    info.hinstance = static_cast<HINSTANCE>(hinstance);
    info.hwnd = static_cast<HWND>(hwnd);

    VPP_CALL(vkCreateWin32SurfaceKHR(vkInstance(), &info, nullptr, &surface_));
}

}
