#include <vpp/backend/win32.hpp>

namespace vpp
{

//Surface
Win32Surface::Win32Surface(VkInstance instance, HINSTANCE hinstance, HWND hwnd) : Surface(instance)
{
    initSurface(hinstance, hwnd);
}

Win32Surface::Win32Surface(VkInstance instance, HWND hwnd) : Surface(instance)
{
    initSurface(GetModuleHandle(nullptr), hwnd);
}

void Win32Surface::initSurface(HINSTANCE hinstance, HWND hwnd)
{
	vk::Win32SurfaceCreateInfoKHR info;
    info.hinstance(hinstance);
    info.hwnd(hwnd);

	vk::createWin32SurfaceKHR(vkInstance(), &info, nullptr, &surface_);
}

//Context
Win32Context::Win32Context(const CreateInfo& info, HINSTANCE hinstance, HWND hwnd)
{
	auto wininfo = info;
	wininfo.instanceExtensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);

	Context::initInstance(wininfo);
	surface_ = Win32Surface(vkInstance(), hinstance, hwnd);

	Context::initDevice(wininfo);
	Context::initSwapChain(wininfo);
}

Win32Context::Win32Context(const CreateInfo& info, HWND hwnd)
	: Win32Context(info, GetModuleHandle(nullptr), hwnd)
{
}

}
