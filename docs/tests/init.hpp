#define BUGGED_NO_MAIN
#include "bugged.hpp"
#include <vpp/vk.hpp>
#include <vpp/memory.hpp>
#include <vpp/device.hpp>
#include <vpp/instance.hpp>
#include <vpp/debug.hpp>
#include <vpp/physicalDevice.hpp>
#include <memory>
#include <dlg/dlg.hpp>

class CustomDebugCallback : public vpp::DebugCallback {
public:
	using vpp::DebugCallback::DebugCallback;

	bool call(const CallbackInfo& info) noexcept override {
		if(info.flags & vk::DebugReportBitsEXT::error) {
			++errors;
		}

		if(info.flags & vk::DebugReportBitsEXT::warning) {
			++warnings;
		}

		if(info.flags & vk::DebugReportBitsEXT::performanceWarning) {
			++performanceWarnings;
		}

		return vpp::DebugCallback::call(info);
	}

	unsigned int performanceWarnings {};
	unsigned int warnings {};
	unsigned int errors {};
};

struct Globals {
	vpp::Instance instance;
	std::unique_ptr<CustomDebugCallback> debugCallback;
	std::unique_ptr<vpp::Device> device;
};

static Globals globals;

void initGlobals() {
	constexpr const char* iniExtensions[] = {
		VK_KHR_SURFACE_EXTENSION_NAME,
		VK_EXT_DEBUG_REPORT_EXTENSION_NAME
	};

	constexpr auto layer = "VK_LAYER_LUNARG_standard_validation";

	vk::InstanceCreateInfo instanceInfo;
	instanceInfo.pApplicationInfo = nullptr;
	instanceInfo.enabledLayerCount = 1;
	instanceInfo.ppEnabledLayerNames = &layer;
	instanceInfo.enabledExtensionCount = sizeof(iniExtensions) / sizeof(iniExtensions[0]);
	instanceInfo.ppEnabledExtensionNames = iniExtensions;

	globals.instance = {instanceInfo};
	globals.debugCallback = std::make_unique<CustomDebugCallback>(globals.instance);
	globals.device = std::make_unique<vpp::Device>(globals.instance);

	dlg_info("Physical device info:\n\t{}",
		vpp::description(globals.device->vkPhysicalDevice(), "\n\t"));
}

unsigned dlgErrors = 0;
unsigned dlgWarnings = 0;
void dlgHandler(const struct dlg_origin* origin, const char* string, void* data) {
	if(origin->level == dlg_level_error) {
		++dlgErrors;
	} else if(origin->level == dlg_level_warn) {
		++dlgWarnings;
	}

	dlg_default_output(origin, string, data);
}

int main() {
	initGlobals();
	dlg_set_handler(dlgHandler, nullptr);

	auto ret = bugged::Testing::run();
	// ret += globals.debugCallback->performanceWarnings;
	ret += globals.debugCallback->errors + dlgErrors;
	ret += globals.debugCallback->warnings + dlgWarnings;

	globals.device = {};
	globals.debugCallback = {};
	globals.instance = {};

	return ret;
}
