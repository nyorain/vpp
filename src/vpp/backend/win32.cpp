#include <vpp/backend/win32.hpp>
#include <vpp/call.hpp>

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
	vk::Win32SurfaceCreateInfoKHR info;
    info.hinstance(static_cast<HINSTANCE>(hinstance));
    info.hwnd(static_cast<HWND>(hwnd));

	vk::createWin32SurfaceKHR(vkInstance(), &info, nullptr, &surface_);
}

}
