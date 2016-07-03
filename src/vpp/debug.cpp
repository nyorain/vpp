#include <vpp/debug.hpp>
#include <vpp/procAddr.hpp>
#include <vpp/vk.hpp>
#include <iostream>

namespace vpp
{

//layer names
std::vector<const char*> validationLayerNames =
{
	//"VK_LAYER_LUNARG_api_dump",
	"VK_LAYER_LUNARG_core_validation",
	"VK_LAYER_LUNARG_device_limits",
	"VK_LAYER_LUNARG_image",
	"VK_LAYER_LUNARG_object_tracker",
	"VK_LAYER_LUNARG_parameter_validation",
	"VK_LAYER_LUNARG_swapchain",
	"VK_LAYER_GOOGLE_threading",
	"VK_LAYER_GOOGLE_unique_objects",
	"VK_LAYER_LUNARG_standard_validation",
};

//utility c free function callback
namespace
{

vk::Bool32 defaultMessageCallback(vk::DebugReportFlagsEXT flags, vk::DebugReportObjectTypeEXT objType,
	std::uint64_t srcObject, std::size_t location, std::int32_t msgCode, const char* pLayerPrefix,
	const char* pMsg, void* pUserData)
{
	auto callback = static_cast<DebugCallback*>(pUserData);
	return callback->call({flags, objType, srcObject, location, msgCode, pLayerPrefix, pMsg});
}

std::string to_string(vk::DebugReportFlagsEXT flags)
{
	std::string ret = "";
	if(flags & vk::DebugReportBitsEXT::information) ret += "information | ";
    if(flags & vk::DebugReportBitsEXT::warning) ret += "warning | ";
    if(flags & vk::DebugReportBitsEXT::performanceWarning) ret += "performanceWarning | ";
    if(flags & vk::DebugReportBitsEXT::error) ret += "error | ";
    if(flags & vk::DebugReportBitsEXT::debug) ret += "debug | ";
	return "{" + ret.substr(0, ret.size() > 0 ? ret.size() - 3 : 0) + "}";
}

std::string to_string(vk::DebugReportObjectTypeEXT type)
{
	switch(type)
	{
		case vk::DebugReportObjectTypeEXT::unknown: return "unknown";
		case vk::DebugReportObjectTypeEXT::instance: return "instance";
		case vk::DebugReportObjectTypeEXT::physicalDevice: return "physicalDevice";
		case vk::DebugReportObjectTypeEXT::device: return "device";
		case vk::DebugReportObjectTypeEXT::queue: return "queue";
		case vk::DebugReportObjectTypeEXT::semaphore: return "semaphore";
		case vk::DebugReportObjectTypeEXT::commandBuffer: return "commandBuffer";
		case vk::DebugReportObjectTypeEXT::fence: return "fence";
		case vk::DebugReportObjectTypeEXT::deviceMemory: return "deviceMemory";
		case vk::DebugReportObjectTypeEXT::buffer: return "buffer";
		case vk::DebugReportObjectTypeEXT::image: return "image";
		case vk::DebugReportObjectTypeEXT::event: return "event";
		case vk::DebugReportObjectTypeEXT::queryPool: return "queryPool";
		case vk::DebugReportObjectTypeEXT::bufferView: return "bufferView";
		case vk::DebugReportObjectTypeEXT::imageView: return "imageView";
		case vk::DebugReportObjectTypeEXT::shaderModule: return "shaderModule";
		case vk::DebugReportObjectTypeEXT::pipelineCache: return "pipelineCache";
		case vk::DebugReportObjectTypeEXT::pipelineLayout: return "pipelineLayout";
		case vk::DebugReportObjectTypeEXT::renderPass: return "renderPass";
		case vk::DebugReportObjectTypeEXT::pipeline: return "pipeline";
		case vk::DebugReportObjectTypeEXT::descriptorSetLayout: return "descriptorSetLayout";
		case vk::DebugReportObjectTypeEXT::sampler: return "sampler";
		case vk::DebugReportObjectTypeEXT::descriptorPool: return "descriptorPool";
		case vk::DebugReportObjectTypeEXT::descriptorSet: return "descriptorSet";
		case vk::DebugReportObjectTypeEXT::framebuffer: return "framebuffer";
		case vk::DebugReportObjectTypeEXT::commandPool: return "commandPool";
		case vk::DebugReportObjectTypeEXT::surfaceKHR: return "surfaceKHR";
		case vk::DebugReportObjectTypeEXT::swapchainKHR: return "swapchainKHR";
		case vk::DebugReportObjectTypeEXT::debugReport: return "debugReport";
		default: return "-";
	}
}

}

//DebugCallback
DebugCallback::DebugCallback(vk::Instance instance, vk::DebugReportFlagsEXT flags)
	: instance_(instance)
{
	VPP_LOAD_PROC(vkInstance(), CreateDebugReportCallbackEXT);
	vk::DebugReportCallbackCreateInfoEXT createInfo(flags, &defaultMessageCallback, this);
	VPP_CALL(pfCreateDebugReportCallbackEXT(vkInstance(), &createInfo, nullptr, &debugCallback_));
}

DebugCallback::~DebugCallback()
{
	if(vkCallback() && vkInstance())
		VPP_PROC(vkInstance(), DestroyDebugReportCallbackEXT)(vkInstance(), vkCallback(), nullptr);

	debugCallback_ = {};
}

bool DebugCallback::call(const CallbackInfo& info)
{
	std::cerr	<< to_string(info.flags) << ": " << info.message << "\n\t"
				<< "objType: " << to_string(info.objectType) << "\n\t"
				<< "srcObject: " << info.srcObject << "\n\t"
				<< "location: " << info.location << "\n\t"
				<< "code: " << info.messageCode << "\n\t"
				<< "layer: " << info.layer << std::endl;

	return false;
}

}
