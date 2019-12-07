// Copyright (c) 2016-2019 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#include <vpp/debug.hpp>
#include <vpp/handles.hpp>
#include <vpp/vk.hpp>
#include <dlg/dlg.hpp>

namespace vpp {
namespace {

static VKAPI_PTR VkBool32 messengerCallback(
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
	// check if message is ignored
	auto ig = std::find(ignore.begin(), ignore.end(), data.pMessageIdName);
	if(ig != ignore.end()) {
		return;
	}

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
	dlg_log(level, "{} ({})", data.pMessage, data.pMessageIdName);

	for(auto i = 0u; i < data.objectCount; ++i) {
		auto& obj = data.pObjects[i];
		if(obj.pObjectName) {
			dlg_log(level, "    involving '{}'", obj.pObjectName);
		}
	}

	for(auto i = 0u; i < data.queueLabelCount; ++i) {
		auto& label = data.pQueueLabels[i];
		auto name = label.pLabelName ? label.pLabelName : "<unnamed>";
		dlg_log(level, "    queue label '{}'", name);
	}

	for(auto i = 0u; i < data.cmdBufLabelCount; ++i) {
		auto& label = data.pCmdBufLabels[i];
		auto name = label.pLabelName ? label.pLabelName : "<unnamed>";
		dlg_log(level, "    cmd label '{}'", name);
	}
}

// name/tag functions
inline namespace debug {

vk::Result nameHandle(const vpp::Device& dev, std::uint64_t handle,
		vk::ObjectType type, const char* name) {
	if(!dev.hasDebugUtils) {
		return vk::Result::errorExtensionNotPresent;
	}

	dlg_assert(vk::dispatch.vkSetDebugUtilsObjectNameEXT);
	vk::DebugUtilsObjectNameInfoEXT info;
	info.objectHandle = handle;
	info.objectType = type;
	info.pObjectName = name;
	return vk::setDebugUtilsObjectNameEXT(dev, info);
}

vk::Result tagHandle(const vpp::Device& dev, std::uint64_t handle,
		vk::ObjectType type, std::uint64_t name,
		nytl::Span<const std::byte> data) {
	if(!dev.hasDebugUtils) {
		return vk::Result::errorExtensionNotPresent;
	}

	dlg_assert(vk::dispatch.vkSetDebugUtilsObjectTagEXT);
	vk::DebugUtilsObjectTagInfoEXT info;
	info.objectHandle = handle;
	info.objectType = type;
	info.tagName = name;
	info.pTag = data.data();
	info.tagSize = size_t(data.size());
	return vk::setDebugUtilsObjectTagEXT(dev, info);
}

void beginDebugLabel(const vpp::Device& dev, vk::CommandBuffer cb,
		const char* name, std::array<float, 4> col) {
	if(!dev.hasDebugUtils) {
		return;
	}

	dlg_assert(vk::dispatch.vkCmdBeginDebugUtilsLabelEXT);
	vk::DebugUtilsLabelEXT info;
	info.color = col;
	info.pLabelName = name;
	vk::cmdBeginDebugUtilsLabelEXT(cb, info);
}

void endDebugLabel(const vpp::Device& dev, vk::CommandBuffer cb) {
	if(!dev.hasDebugUtils) {
		return;
	}

	dlg_assert(vk::dispatch.vkCmdEndDebugUtilsLabelEXT);
	vk::cmdEndDebugUtilsLabelEXT(cb);
	return;
}

void insertDebugLabel(const vpp::Device& dev, vk::CommandBuffer cb,
		const char* name, std::array<float, 4> col) {
	if(!dev.hasDebugUtils) {
		return;
	}

	dlg_assert(vk::dispatch.vkCmdInsertDebugUtilsLabelEXT);
	vk::DebugUtilsLabelEXT info;
	info.color = col;
	info.pLabelName = name;
	vk::cmdInsertDebugUtilsLabelEXT(cb, info);
}

void beginDebugLabel(const vpp::CommandBuffer& cb, const char* name,
		std::array<float, 4> col) {
	beginDebugLabel(cb.device(), cb, name, col);
}

void endDebugLabel(const vpp::CommandBuffer& cb) {
	endDebugLabel(cb.device(), cb);
}

void insertDebugLabel(const vpp::CommandBuffer& cb,
		const char* name, std::array<float, 4> col) {
	insertDebugLabel(cb.device(), cb, name, col);
}

// DebugLabel
DebugLabel::DebugLabel(const vpp::Device& dev, vk::CommandBuffer cb,
		const char* name, std::array<float, 4> color) {
	if(dev.hasDebugUtils) {
		beginDebugLabel(dev, cb, name, color);
		cb_ = cb;
		dev_ = &dev;
	}
}

DebugLabel::DebugLabel(const vpp::CommandBuffer& cb,
	const char* name, std::array<float, 4> color) :
		DebugLabel(cb.device(), cb.vkHandle(), name, color) {
}

DebugLabel::~DebugLabel() {
	if(dev_ && cb_) {
		endDebugLabel(*dev_, cb_);
	}
}

} // namespace debug

namespace nodebug {

const vk::Result resultExtensionNotPresent =
	vk::Result::errorExtensionNotPresent;

} // namespace nodebug
} // namespace vpp
