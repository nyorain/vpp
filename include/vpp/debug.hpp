#pragma once

#include <vpp/vk.hpp>
#include <vpp/fwd.hpp>

namespace vpp
{

///LeyerNames
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
