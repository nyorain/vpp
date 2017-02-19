#define VK_USE_PLATFORM_WIN32_KHR

#include <vpp/vk.hpp>
#include <vpp/memory.hpp>
#include <vpp/device.hpp>
#include <vpp/instance.hpp>
#include <vpp/debug.hpp>

struct Globals {
	vpp::Instance instance;
	std::unique_ptr<vpp::DebugCallback> debugCallback;
	std::unique_ptr<vpp::Device> device;
};

Globals initGlobals();
Globals globals = initGlobals();

Globals initGlobals()
{
	Globals ret;

	constexpr const char* iniExtensions[] = {
		VK_KHR_SURFACE_EXTENSION_NAME,
		VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
		VK_EXT_DEBUG_REPORT_EXTENSION_NAME
	};

	constexpr auto layer = "VK_LAYER_LUNARG_standard_validation";
	vk::ApplicationInfo appInfo ("vpp-test", 1, "vpp", 1, VK_API_VERSION_1_0);

	vk::InstanceCreateInfo instanceInfo;
	instanceInfo.pApplicationInfo = &appInfo;
	instanceInfo.enabledLayerCount = 1;
	instanceInfo.ppEnabledLayerNames = &layer;
	instanceInfo.enabledExtensionCount = sizeof(iniExtensions) / sizeof(iniExtensions[0]);
	instanceInfo.ppEnabledExtensionNames = iniExtensions;

	ret.instance = {instanceInfo};
	ret.debugCallback = std::make_unique<vpp::DebugCallback>(ret.instance);
	ret.device = std::make_unique<vpp::Device>(ret.instance);

	return ret;
}
