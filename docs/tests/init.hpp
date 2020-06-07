#define BUGGED_NO_MAIN
#include "bugged.hpp"
#include <vpp/vk.hpp>
#include <vpp/memory.hpp>
#include <vpp/handles.hpp>
#include <vpp/device.hpp>
#include <vpp/debug.hpp>
#include <vpp/physicalDevice.hpp>
#include <memory>
#include <dlg/dlg.hpp>

class CustomDebugCallback : public vpp::DebugMessenger {
public:
	using vpp::DebugMessenger::DebugMessenger;

	void call(MsgSeverity severity, MsgTypeFlags types,
			const Data& data) noexcept override {
		if(severity == MsgSeverity::error) {
			++errors;
		}

		if(severity == MsgSeverity::warning) {
			++warnings;
		}

		return vpp::DebugMessenger::call(severity, types, data);
	}

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
		VK_EXT_DEBUG_UTILS_EXTENSION_NAME
	};

	constexpr auto layer = "VK_LAYER_KHRONOS_validation";

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
	globals.device = {};

	ret += globals.debugCallback->errors + dlgErrors;
	ret += globals.debugCallback->warnings + dlgWarnings;

	globals.debugCallback = {};
	globals.instance = {};

	return ret;
}
