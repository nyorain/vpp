// Copyright (c) 2016-2019 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#include <vpp/debug.hpp>
#include <vpp/vk.hpp>
#include <dlg/dlg.hpp>

namespace vpp {
namespace {

static VkBool32 messengerCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT severity,
		VkDebugUtilsMessageTypeFlagsEXT type,
		const VkDebugUtilsMessengerCallbackDataEXT* debugData,
		void* data) {
	if(!data || !debugData) {
		dlg_error("DebugCallback called with null data");
		return false;
	}

	auto messenger = static_cast<DebugMessenger*>(data);
	auto vseverity = static_cast<vk::DebugUtilsMessageSeverityBitsEXT>(severity);
	auto vtype = static_cast<vk::DebugUtilsMessageTypeBitsEXT>(type);

	// NOTE: gcc 8 warns here because of the memcpy and since
	// vdebugData isn't trivial (cannot be made trivial due to sType
	// initializer). But memcpy doesn't require that types are trivial,
	// only trivially copyable and vdebugData is that, so we cheat
	// the warning by casting to void*
	vk::DebugUtilsMessengerCallbackDataEXT vdebugData;
	std::memcpy((void*) &vdebugData, debugData, sizeof(vdebugData));
	messenger->call(vseverity, vtype, vdebugData);
	return false;
}

} // anon namespace

// DebugMessenger
vk::DebugUtilsMessageSeverityFlagsEXT DebugMessenger::defaultSeverity() {
	return MsgSeverity::error |
		MsgSeverity::warning;
}

vk::DebugUtilsMessageTypeFlagsEXT DebugMessenger::defaultTypes() {
	return MsgType::general |
		MsgType::performance |
		MsgType::validation;
}

DebugMessenger::DebugMessenger(vk::Instance instance, MsgSeverityFlags severity,
		MsgTypeFlags types) : instance_(instance) {
	if(!vk::dispatch.vkCreateDebugUtilsMessengerEXT) {
		auto msg = "vpp::DebugCallback: VK_EXT_debug_utils not enabled";
		throw std::runtime_error(msg);
	}

	vk::DebugUtilsMessengerCreateInfoEXT info;
	info.messageSeverity = severity;
	info.messageType = types;
	info.pfnUserCallback = &messengerCallback;
	info.pUserData = this;
	messenger_ = vk::createDebugUtilsMessengerEXT(instance, info);
}

DebugMessenger::~DebugMessenger() {
	if(vkMessenger()) {
		vk::destroyDebugUtilsMessengerEXT(vkInstance(), vkMessenger());
	}
}

void DebugMessenger::call(MsgSeverity severity, MsgTypeFlags,
		const Data& data) noexcept {
	auto level = dlg_level_trace;
	switch(severity) {
		case MsgSeverity::error:
			level = dlg_level_error;
			break;
		case MsgSeverity::warning:
			level = dlg_level_warn;
			break;
		case MsgSeverity::info:
			level = dlg_level_info;
			break;
		case MsgSeverity::verbose:
			level = dlg_level_debug;
			break;
	}

	dlg_tags("DebugCallback");
	dlg_log(level, "{} ({})", data.pMessage, data.messageIdNumber);

	for(auto i = 0u; i < data.objectCount; ++i) {
		auto& obj = data.pObjects[i];
		if(obj.pObjectName) {
			dlg_log(level, "    involving '{}'", obj.pObjectName);
		}
	}

	if(data.queueLabelCount > 0) {
		auto& lastLabel = data.pQueueLabels[0];
		dlg_log(level, "    last queue label '{}'", lastLabel.pLabelName);
	}

	if(data.cmdBufLabelCount > 0) {
		auto& lastLabel = data.pCmdBufLabels[0];
		dlg_log(level, "    last cmd label '{}'", lastLabel.pLabelName);
	}
}

// name/tag functions
vk::Result nameHandle(vk::Device dev, std::uint64_t handle,
		vk::ObjectType type, const char* name) {
	if(!vk::dispatch.vkSetDebugUtilsObjectNameEXT) {
		return vk::Result::errorExtensionNotPresent;
	}

	vk::DebugUtilsObjectNameInfoEXT info;
	info.objectHandle = handle;
	info.objectType = type;
	info.pObjectName = name;
	return vk::setDebugUtilsObjectNameEXT(dev, info);
}

vk::Result tagHandle(vk::Device dev, std::uint64_t handle,
		vk::ObjectType type, std::uint64_t name,
		nytl::Span<const std::byte> data) {
	if(!vk::dispatch.vkSetDebugUtilsObjectTagEXT) {
		return vk::Result::errorExtensionNotPresent;
	}

	vk::DebugUtilsObjectTagInfoEXT info;
	info.objectHandle = handle;
	info.objectType = type;
	info.tagName = name;
	info.pTag = data.data();
	info.tagSize = size_t(data.size());
	return vk::setDebugUtilsObjectTagEXT(dev, info);
}

bool beginDebugLabel(vk::CommandBuffer cb, const char* name,
		std::array<float, 4> col) {
	if(!vk::dispatch.vkCmdBeginDebugUtilsLabelEXT) {
		return false;
	}

	vk::DebugUtilsLabelEXT info;
	info.color = col;
	info.pLabelName = name;
	vk::cmdBeginDebugUtilsLabelEXT(cb, info);
	return true;
}

bool endDebugLabel(vk::CommandBuffer cb) {
	if(!vk::dispatch.vkCmdEndDebugUtilsLabelEXT) {
		return false;
	}

	vk::cmdEndDebugUtilsLabelEXT(cb);
	return true;
}

bool insertDebugLabel(vk::CommandBuffer cb, const char* name,
		std::array<float, 4> col) {
	if(!vk::dispatch.vkCmdInsertDebugUtilsLabelEXT) {
		return false;
	}

	vk::DebugUtilsLabelEXT info;
	info.color = col;
	info.pLabelName = name;
	vk::cmdInsertDebugUtilsLabelEXT(cb, info);
	return true;
}

// DebugLabel
DebugLabel::DebugLabel(vk::CommandBuffer cb, const char* name,
		std::array<float, 4> color) : cb_(cb) {
	beginDebugLabel(cb, name, color);
}

DebugLabel::~DebugLabel() {
	if(cb_) {
		endDebugLabel(cb_);
	}
}

// spezialization of Handletype
#define HandleSpec(handle, name) \
	template<> struct HandleType<handle> { \
		static constexpr auto value = vk::ObjectType::name; \
		static constexpr auto debugReportValue = \
			vk::DebugReportObjectTypeEXT::name; \
	}

HandleSpec(vk::Instance, instance);
HandleSpec(vk::PhysicalDevice, physicalDevice);
HandleSpec(vk::Device, device);
HandleSpec(vk::CommandBuffer, commandBuffer);
HandleSpec(vk::CommandPool, commandPool);
HandleSpec(vk::Queue, queue);
HandleSpec(vk::Image, image);
HandleSpec(vk::ImageView, imageView);
HandleSpec(vk::Buffer, buffer);
HandleSpec(vk::BufferView, bufferView);
HandleSpec(vk::Framebuffer, framebuffer);
HandleSpec(vk::Sampler, sampler);
HandleSpec(vk::DeviceMemory, deviceMemory);
HandleSpec(vk::DescriptorSetLayout, descriptorSetLayout);
HandleSpec(vk::DescriptorSet, descriptorSet);
HandleSpec(vk::DescriptorPool, descriptorPool);
HandleSpec(vk::PipelineLayout, pipelineLayout);
HandleSpec(vk::Pipeline, pipeline);
HandleSpec(vk::PipelineCache, pipelineCache);
HandleSpec(vk::RenderPass, renderPass);
HandleSpec(vk::Semaphore, semaphore);
HandleSpec(vk::Fence, fence);
HandleSpec(vk::Event, event);
HandleSpec(vk::QueryPool, queryPool);
HandleSpec(vk::ShaderModule, shaderModule);
HandleSpec(vk::SurfaceKHR, surfaceKHR);
HandleSpec(vk::SwapchainKHR, swapchainKHR);
HandleSpec(vk::DebugReportCallbackEXT, debugReportCallbackEXT);
HandleSpec(vk::DisplayKHR, displayKHR);
HandleSpec(vk::DisplayModeKHR, displayModeKHR);

#undef DebugHandleSpec

} // namespace vpp
