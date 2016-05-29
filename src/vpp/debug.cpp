#include <vpp/debug.hpp>
#include <vpp/procAddr.hpp>
#include <vpp/vk.hpp>
#include <iostream>

namespace vpp
{

//layer names
std::vector<const char*> validationLayerNames =
{
	"VK_LAYER_LUNARG_threading",
	"VK_LAYER_LUNARG_mem_tracker",
	"VK_LAYER_LUNARG_object_tracker",
	//"VK_LAYER_LUNARG_draw_state",
	"VK_LAYER_LUNARG_param_checker",
	"VK_LAYER_LUNARG_swapchain",
	"VK_LAYER_LUNARG_device_limits",
	"VK_LAYER_LUNARG_image",
	"VK_LAYER_GOOGLE_unique_objects"
	//"VK_LAYER_LUNARG_api_dump"
	//"VK_LAYER_LUNARG_screenshot",
	//"VK_LAYER_LUNARG_vktrace",
	//"VK_LAYER_RENDERDOC_Capture"

	/*,
	"VK_LAYER_GOOGLE_threading",
	"VK_LAYER_LUNARG_core_validation",
	"VK_LAYER_LUNARG_parameter_validation",
	"VK_LAYER_LUNARG_standard_validation",
	*/
};

//utility c free function callback
namespace
{

vk::Bool32 defaultMessageCallback(vk::DebugReportFlagsEXT flags, vk::DebugReportObjectTypeEXT objType,
	std::uint64_t srcObject, std::size_t location, std::int32_t msgCode, const char* pLayerPrefix,
	const char* pMsg, void* pUserData)
{
	auto callback = static_cast<DebugCallback*>(pUserData);

	auto vkFlags = reinterpret_cast<const vk::DebugReportFlagsEXT&>(flags);
	auto vkObjType = reinterpret_cast<const vk::DebugReportObjectTypeEXT&>(objType);

	return callback->call({vkFlags, vkObjType, srcObject, location, msgCode, pLayerPrefix, pMsg});
}

}

//DebugCallback
DebugCallback::DebugCallback(vk::Instance instance, vk::DebugReportFlagsEXT flags)
	: instance_(instance)
{
	VPP_LOAD_INSTANCE_PROC(vkInstance(), CreateDebugReportCallbackEXT);

	vk::DebugReportCallbackCreateInfoEXT dbgCreateInfo(flags, defaultMessageCallback, this);
	auto handle = &dbgCreateInfo.vkHandle();

	VPP_CALL(fpCreateDebugReportCallbackEXT(vkInstance(), handle, nullptr, &debugCallback_));
}

DebugCallback::~DebugCallback()
{
	VPP_LOAD_INSTANCE_PROC(vkInstance(), DestroyDebugReportCallbackEXT);

	if(vkCallback() && vkInstance()) fpDestroyDebugReportCallbackEXT(vkInstance(), vkCallback(), nullptr);
	debugCallback_ = {};
}

bool DebugCallback::call(const CallbackInfo& info)
{
	/*
	std::cout	<< enumString(info.flags) << ": " << info.message << "\n\t"
				<< "objType: " << enumString(info.objectType) << "\n\t"
				<< "srcObject: " << info.srcObject << "\n\t"
				<< "location: " << info.location << "\n\t"
				<< "code: " << info.messageCode << "\n\t"
				<< "layer: " << info.layer << std::endl;
	*/

	return false;
}

}
