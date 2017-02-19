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

	constexpr auto layer = "VK_LAYER_LUNARG_standard_validation";
	vk::ApplicationInfo appInfo ("vpp-test", 1, "vpp", 1, VK_API_VERSION_1_0);

	vk::InstanceCreateInfo instanceInfo;
	instanceInfo.pApplicationInfo = &appInfo;
	instanceInfo.enabledLayerCount = 1;
	instanceInfo.ppEnabledLayerNames = &layer;

	ret.instance = {instanceInfo};
	ret.debugCallback = std::make_unique<vpp::DebugCallback>(ret.instance);
	ret.device = std::make_unique<vpp::Device>(ret.instance);

	return ret;
}
