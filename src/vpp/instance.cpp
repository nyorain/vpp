#include <vpp/instance.hpp>
#include <vpp/call.hpp>

namespace vpp
{

Instance::CreateInfo::CreateInfo(){}

//non-static
Instance::Instance(const CreateInfo& createInfo)
{
	vk::ApplicationInfo appInfo;
	appInfo.pApplicationName(createInfo.appName.c_str());
	appInfo.applicationVersion(createInfo.appVersion);
	appInfo.pEngineName(createInfo.engineName.c_str());
	appInfo.engineVersion(createInfo.engineVersion);
	appInfo.apiVersion(createInfo.apiVersion);

	vk::InstanceCreateInfo instanceInfo;
	instanceInfo.pApplicationInfo(&appInfo);
	instanceInfo.enabledLayerCount(createInfo.layers.size());
	instanceInfo.ppEnabledLayerNames(createInfo.layers.data());
	instanceInfo.enabledExtensionCount(createInfo.extensions.size());
	instanceInfo.ppEnabledExtensionNames(createInfo.extensions.data());

	vk::createInstance(&instanceInfo, nullptr, &instance_);
}

Instance::Instance(const vk::InstanceCreateInfo& createInfo)
{
	vk::createInstance(&createInfo, nullptr, &instance_);
}

Instance::~Instance()
{
	if(vkInstance()) vk::destroyInstance(instance_, nullptr);
}

std::vector<vk::PhysicalDevice> Instance::enumeratePhysicalDevices() const
{
	std::vector<VkPhysicalDevice> phdevs{};
    VPP_CALL(vk::enumeratePhysicalDevices(vkInstance(), phdevs));
    return phdevs;
}

}
