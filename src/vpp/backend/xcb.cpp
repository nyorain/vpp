#include <vpp/backend/xcb.hpp>

namespace vpp
{

Surface createSurface(vk::Instance instance, xcb_connection_t& con, xcb_window_t window);
{
	if(module == nullptr) module = ::GetModuleHandle(nullptr);

	vk::XcbSurfaceCreateInfoKHR info;
    info.connection = &con;
    info.window = window;

	vk::SurfaceKHR ret;
	VPP_PROC(instance, CreateXcbSurfaceKHR)(instance, &info, nullptr, &ret);
	return {instance, ret};
}

Context createContext(xcb_connection_t& con, xcb_window_t window, Context::CreateInfo info);
{
	info.instanceExtensions.push_back(VK_KHR_XCB_SURFACE_EXTENSION_NAME);

	Context ret;
	ret.initInstance(info);

	auto vsurface = createSurface(ret.vkInstance(), con, window);
	ret.initSurface(std::move(vsurface));

	ret.initDevice(info);
	ret.initSwapChain(info);

	return ret;
}

Context createContext(xcb_connection_t& con, xcb_window_t window);
{
	return createContext(con, window, {});
}


}
