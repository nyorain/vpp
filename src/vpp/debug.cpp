#include <vpp/debug.hpp>
#include <vpp/procAddr.hpp>
#include <vpp/instance.hpp>
#include <iostream>

namespace vpp
{

//utility c free function callback
namespace
{

VkBool32 defaultMessageCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType,
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
DebugCallback::DebugCallback(Instance instance, vk::DebugReportFlagsEXT flags)
	: DebugCallback(instance.vkInstance(), flags)
{
}

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
	std::cout	<< enumString(info.flags) << ": " << info.message << "\n\t"
				<< "objType: " << enumString(info.objectType) << "\n\t"
				<< "srcObject: " << info.srcObject << "\n\t"
				<< "location: " << info.location << "\n\t"
				<< "code: " << info.messageCode << "\n\t"
				<< "layer: " << info.layer << std::endl;

	return false;
}

}
