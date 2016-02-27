#pragma once

#include <vpp/vk.hpp>
#include <vpp/fwd.hpp>

#include <vector>
#include <string>

namespace vpp
{

///Instance.
class Instance
{
public:
	struct CreateInfo
	{
		std::vector<const char*> extensions {};
		std::vector<const char*> layers {};
		std::string appName {""};
		unsigned int appVersion {0};
		std::string engineName {"vpp"};
		unsigned int engineVersion {0};
		unsigned int apiVersion {VK_MAKE_VERSION(1, 0, 2)};

		CreateInfo();
	};

protected:
	vk::Instance instance_ {};

public:
	Instance(const CreateInfo& createInfo = {});
	Instance(const vk::InstanceCreateInfo& createInfo);
	~Instance();

	vk::Instance vkInstance() const { return instance_; }
	std::vector<vk::PhysicalDevice> enumeratePhysicalDevices() const;
};

}
