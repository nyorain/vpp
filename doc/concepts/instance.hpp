#pragma once

#include <vpp/fwd.hpp>

#include <vector>
#include <string>

namespace vpp
{

///Vulkan Instance.
class Instance
{
public:
	struct CreateInfo
	{
		std::vector<const char*> extensions {};
		std::vector<const char*> layers {};
		std::string appName {"unknown"};
		unsigned int appVersion {0};
		std::string engineName {"vpp"};
		unsigned int engineVersion {0};
		unsigned int apiVersion {VK_MAKE_VERSION(1, 0, 2)}; //todo

		CreateInfo(); //for some compiler bug in gcc
	};

public:
	Instance(const CreateInfo& createInfo = {});
	Instance(const vk::InstanceCreateInfo& createInfo);
	~Instance();

	const vk::Instance& vkInstance() const { return instance_; }
	std::vector<vk::PhysicalDevice> enumeratePhysicalDevices() const;

protected:
	vk::Instance instance_ {};
};

}
