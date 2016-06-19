#pragma once

#include <vpp/fwd.hpp>
#include <vector>
#include <cstdint>

namespace vpp
{

//flags
/*
vk::DebugReportFlagBitsEXT::ErrorEXT
vk::DebugReportFlagBitsEXT::WarningEXT
vk::DebugReportFlagBitsEXT::InformationEXT
vk::DebugReportFlagBitsEXT::DebugEXT
vk::DebugReportFlagBitsEXT::PerformanceWarningEXT
*/

///LayerNames
extern std::vector<const char*> validationLayerNames;

///Vulkan DebugCallback.
class DebugCallback
{
public:
	struct CallbackInfo
	{
		vk::DebugReportFlagsEXT flags;
		vk::DebugReportObjectTypeEXT objectType;
		std::uint64_t srcObject;
		std::size_t location;
		std::int32_t messageCode;
		const char* layer;
		const char* message;
	};

protected:
	vk::Instance instance_ {};
	vk::DebugReportCallbackEXT debugCallback_ {};

public:
	DebugCallback(vk::Instance instance, vk::DebugReportFlagsEXT flags);
	~DebugCallback();

	vk::Instance vkInstance() const { return instance_; }
	vk::DebugReportCallbackEXT vkCallback() const { return debugCallback_; }

	virtual bool call(const CallbackInfo& info);
};

};
