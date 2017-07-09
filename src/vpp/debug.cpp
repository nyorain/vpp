// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#include <vpp/debug.hpp>
#include <vpp/procAddr.hpp>
#include <vpp/vk.hpp>
#include <vpp/util/log.hpp>

#include <iostream> // std::cerr
#include <sstream> // std::stringstream

namespace vpp {
namespace {

VKAPI_PTR vk::Bool32 defaultMessageCallback(vk::DebugReportFlagsEXT flags,
	vk::DebugReportObjectTypeEXT objType,
	uint64_t srcObject,
	size_t location,
	int32_t msgCode,
	const char* pLayerPrefix,
	const char* pMsg,
	void* pUserData)
{
	if(!pUserData) {
		std::cerr << "vpp::debug: DebugCallback called without user data\n";
		return true;
	}

	auto callback = static_cast<DebugCallback*>(pUserData);
	return callback->call({flags, objType, srcObject, location, msgCode, pLayerPrefix, pMsg});
}

std::string to_string(vk::DebugReportFlagsEXT flags)
{
	std::string ret = "";
	unsigned int count = 0u;
	if(flags & vk::DebugReportBitsEXT::information) ret += "information, ", ++count;
	if(flags & vk::DebugReportBitsEXT::warning) ret += "warning, ", ++count;
	if(flags & vk::DebugReportBitsEXT::performanceWarning) ret += "performanceWarning, ", ++count;
	if(flags & vk::DebugReportBitsEXT::error) ret += "error, ", ++count;
	if(flags & vk::DebugReportBitsEXT::debug) ret += "debug, ", ++count;

	if(ret.empty()) ret = "<unknown>";
	else if(ret.size() > 3) ret = ret.substr(0, ret.size() - 2);
	if(count > 1) ret = "{" + ret + "}";
	return ret;
}

std::string to_string(vk::DebugReportObjectTypeEXT type)
{
	switch(type) {
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
		default: return "<unknown>";
	}
}

} // anonymous util namespace

// DebugCallback
vk::DebugReportFlagsEXT DebugCallback::defaultFlags()
{
	return vk::DebugReportBitsEXT::warning |
		vk::DebugReportBitsEXT::error |
		vk::DebugReportBitsEXT::performanceWarning;
}

DebugCallback::DebugCallback(vk::Instance instance, vk::DebugReportFlagsEXT flags)
	: instance_(instance)
{
	VPP_LOAD_PROC(vkInstance(), CreateDebugReportCallbackEXT);
	vk::DebugReportCallbackCreateInfoEXT createInfo(flags, &defaultMessageCallback, this);
	VPP_CALL(pfCreateDebugReportCallbackEXT(vkInstance(), &createInfo, nullptr, &debugCallback_));
}

DebugCallback::~DebugCallback()
{
	if(vkCallback())
		VPP_PROC(vkInstance(), DestroyDebugReportCallbackEXT)(vkInstance(), vkCallback(), nullptr);
}

bool DebugCallback::call(const CallbackInfo& info) const noexcept
{
	static constexpr bool verbose = false;
	// we use a stringstream here because the callback might be called from multiple threads
	// and debug messages should not interfer with each other

	std::stringstream message;
	message << info.message;

	if(verbose) {
		message << "\n\tflags: " << to_string(info.flags) << "\n\t"
			    << "objType: " << to_string(info.objectType) << "\n\t"
			    << "srcObject: " << info.srcObject << "\n\t"
			    << "location: " << info.location << "\n\t"
			    << "code: " << info.messageCode << "\n\t"
			    << "layer: " << info.layer;
	}

	dlg::SourceGuard sourceGuard("DebugCallback"_module);
	if(info.flags & vk::DebugReportBitsEXT::error) vpp_error(message.str());
	else if(info.flags & vk::DebugReportBitsEXT::warning) vpp_warn(message.str());
	else if(info.flags & vk::DebugReportBitsEXT::information) vpp_info(message.str());
	else if(info.flags & vk::DebugReportBitsEXT::performanceWarning) vpp_info(message.str());
	else if(info.flags & vk::DebugReportBitsEXT::debug) vpp_debug(message.str());

	return info.flags == vk::DebugReportBitsEXT::error;
}

} // namespace vpp
