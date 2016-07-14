#include <vpp/backend/win32.hpp>
#include <vpp/procAddr.hpp>

namespace vpp
{

Surface createSurface(vk::Instance instance, HWND window, HINSTANCE module)
{
	if(module == nullptr) module = ::GetModuleHandle(nullptr);

	vk::Win32SurfaceCreateInfoKHR info;
    info.hinstance = module;
    info.hwnd = window;

	vk::SurfaceKHR ret;
	VPP_PROC(instance, CreateWin32SurfaceKHR)(instance, &info, nullptr, &ret);
	return {instance, ret};
}

Context createContext(HWND window, Context::CreateInfo info, HINSTANCE module)
{
	info.instanceExtensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);

	Context ret;
	ret.initInstance(info);

	auto vsurface = createSurface(ret.vkInstance(), window, module);
	ret.initSurface(std::move(vsurface));

	ret.initDevice(info);
	ret.initSwapChain(info);

	return ret;
}

Context createContext(HWND window, HINSTANCE module)
{
	return createContext(window, {}, module);
}

}
