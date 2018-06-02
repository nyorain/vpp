// Copyright (c) 2016-2018 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#include <vpp/debug.hpp>
#include <vpp/procAddr.hpp>
#include <vpp/vk.hpp>
#include <dlg/dlg.hpp>

namespace vpp {
namespace {

VKAPI_PTR vk::Bool32 defaultMessageCallback(vk::DebugReportFlagsEXT flags,
		vk::DebugReportObjectTypeEXT objType,
		uint64_t srcObject,
		size_t location,
		int32_t msgCode,
		const char* pLayerPrefix,
		const char* pMsg,
		void* pUserData) {

	if(!pUserData) {
		dlg_error("DebugCallback called with nullptr user data");
		return true;
	}

	auto callback = static_cast<DebugCallback*>(pUserData);
	return callback->call({flags, objType, srcObject, location, msgCode, pLayerPrefix, pMsg});
}

std::string to_string(vk::DebugReportFlagsEXT flags) {
	static constexpr struct {
		vk::DebugReportBitsEXT bit;
		const char* name;
	} names[] = {
		{vk::DebugReportBitsEXT::information, "information"},
		{vk::DebugReportBitsEXT::warning, "warning"},
		{vk::DebugReportBitsEXT::performanceWarning, "performanceWarning"},
		{vk::DebugReportBitsEXT::error, "error"},
		{vk::DebugReportBitsEXT::debug, "debug"},
	};

	std::string ret;
	unsigned int count = 0u;
	for(auto name : names) {
		if(flags & name.bit) {
			++count;
			ret += name.name;
			ret += ", ";
		}
	}

	if(ret.empty()) {
		ret = "<unknown>";
	} else if(ret.size() > 3) {
		ret = ret.substr(0, ret.size() - 2);
	}

	if(count > 1) {
		ret = "{" + ret + "}";
	}

	return ret;
}

std::string to_string(vk::DebugReportObjectTypeEXT type) {
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
		default: return "<unknown>";
	}
}

} // anonymous util namespace

// DebugCallback
vk::DebugReportFlagsEXT DebugCallback::defaultFlags() {
	return vk::DebugReportBitsEXT::warning |
		vk::DebugReportBitsEXT::error |
		vk::DebugReportBitsEXT::performanceWarning;
}

vk::DebugReportFlagsEXT DebugCallback::defaultErrorFlags() {
	return vk::DebugReportBitsEXT::error;
}
vk::DebugReportFlagsEXT DebugCallback::allBits() {
	return vk::DebugReportBitsEXT::warning |
		vk::DebugReportBitsEXT::error |
		vk::DebugReportBitsEXT::performanceWarning |
		vk::DebugReportBitsEXT::debug |
		vk::DebugReportBitsEXT::information;
}

DebugCallback::DebugCallback(vk::Instance instance, vk::DebugReportFlagsEXT flags,
		bool verbose, vk::DebugReportFlagsEXT error) :
		instance_(instance), errorFlags_(error), verbose_(verbose) {

	VPP_LOAD_PROC(vkInstance(), CreateDebugReportCallbackEXT);
	vk::DebugReportCallbackCreateInfoEXT createInfo(flags, &defaultMessageCallback, this);
	VKPP_CALL(pfCreateDebugReportCallbackEXT(vkInstance(), &createInfo, nullptr, &debugCallback_));
}

DebugCallback::~DebugCallback() {
	if(vkCallback()) {
		VPP_PROC(vkInstance(), DestroyDebugReportCallbackEXT)(vkInstance(), vkCallback(), nullptr);
	}
}

bool DebugCallback::call(const CallbackInfo& info) noexcept {
	std::string verbose;
	if(verbose_) {
		verbose = "\n\tflags: " + to_string(info.flags) + "\n\t";
		verbose += "objType: " + to_string(info.objectType) + "\n\t";
		verbose += "srcObject: " + std::to_string(info.srcObject) + "\n\t";
		verbose += "location: " + std::to_string(info.location) + "\n\t";
		verbose += "code: " + std::to_string(info.messageCode) + "\n\t";
		verbose += "layer: ";
		verbose += info.layer;
	}

	dlg_tags("DebugCallback");
	if(info.flags & vk::DebugReportBitsEXT::error) {
		dlg_error("{}{}", info.message, verbose);
	} else if(info.flags & vk::DebugReportBitsEXT::warning) {
		dlg_warn("{}{}", info.message, verbose);
	} else if(info.flags & vk::DebugReportBitsEXT::information) {
		dlg_info("{}{}", info.message, verbose);
	} else if(info.flags & vk::DebugReportBitsEXT::performanceWarning) {
		dlg_info("{}{}", info.message, verbose);
	} else if(info.flags & vk::DebugReportBitsEXT::debug) {
		dlg_debug("{}{}", info.message, verbose);
	}

	return (info.flags & errorFlags_);
}

// debug report
vk::Result nameHandle(vk::Device dev, std::uint64_t handle,
		vk::DebugReportObjectTypeEXT type, const char* name) {

	VPP_LOAD_PROC_NOTHROW(dev, DebugMarkerSetObjectNameEXT);
	if(!pfDebugMarkerSetObjectNameEXT) {
		return vk::Result::errorExtensionNotPresent;
	}

	vk::DebugMarkerObjectNameInfoEXT info;
	info.object = handle;
	info.objectType = type;
	info.pObjectName = name;
	return pfDebugMarkerSetObjectNameEXT(dev, &info);
}

vk::Result tagHandle(vk::Device dev, std::uint64_t handle,
		vk::DebugReportObjectTypeEXT type, std::uint64_t name,
		nytl::Span<const std::byte> data) {

	VPP_LOAD_PROC_NOTHROW(dev, DebugMarkerSetObjectTagEXT);
	if(!pfDebugMarkerSetObjectTagEXT) {
		return vk::Result::errorExtensionNotPresent;
	}

	vk::DebugMarkerObjectTagInfoEXT info;
	info.object = handle;
	info.objectType = type;
	info.pTag = data.data();
	info.tagSize = data.size();
	info.tagName = name;
	return pfDebugMarkerSetObjectTagEXT(dev, &info);
}

bool beginDebugRegion(vk::Device dev, vk::CommandBuffer cmdBuf,
		const char* name, std::array<float, 4> col) {

	VPP_LOAD_PROC_NOTHROW(dev, CmdDebugMarkerBeginEXT);
	if(!pfCmdDebugMarkerBeginEXT) {
		return false;
	}

	vk::DebugMarkerMarkerInfoEXT markerInfo;
	markerInfo.color = col;
	markerInfo.pMarkerName = name;
	pfCmdDebugMarkerBeginEXT(cmdBuf, &markerInfo);
	return true;
}

bool endDebugRegion(vk::Device dev, vk::CommandBuffer cmdBuf) {
	VPP_LOAD_PROC_NOTHROW(dev, CmdDebugMarkerEndEXT);
	if(!pfCmdDebugMarkerEndEXT) {
		return false;
	}

	pfCmdDebugMarkerEndEXT(cmdBuf);
	return true;
}

bool insertDebugMarker(vk::Device dev, vk::CommandBuffer cmdBuf,
		const char* name, std::array<float, 4> col) {

	VPP_LOAD_PROC_NOTHROW(dev, CmdDebugMarkerInsertEXT);
	if(!pfCmdDebugMarkerInsertEXT) {
		return false;
	}

	vk::DebugMarkerMarkerInfoEXT markerInfo;
	markerInfo.color = col;
	markerInfo.pMarkerName = name;
	pfCmdDebugMarkerInsertEXT(cmdBuf, &markerInfo);
	return true;
}

} // namespace vpp
