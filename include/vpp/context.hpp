#pragma once

#include <vpp/vk.hpp>
#include <vpp/platform.hpp>

#include <vector>
#include <string>
#include <memory>

namespace vpp
{

///A Vulkan context (instance, physicalDevice and device).
class Context
{
public:
	struct Queue
	{
		vk::Queue queue;
		unsigned int queueFamily;
		unsigned int id;
	};

protected:
    vk::Instance instance_ {};
    vk::PhysicalDevice physicalDevice_ {};
    vk::Device device_ {};

	Platform platform_;
	std::vector<Queue> queues_;

protected:
    virtual std::string applicationName() const { return "VulkanTest"; }
    virtual std::string engineName() const { return "None"; }
    virtual unsigned int applicationVersion() const { return 0; }
    virtual unsigned int engineVersion() const { return 0; }

    virtual std::vector<std::string> enabledInstanceExtensions() const;
    virtual std::vector<std::string> enabledDeviceExtensions() const;

    virtual std::vector<std::string> enabledInstanceLayers() const;
    virtual std::vector<std::string> enabledDeviceLayers() const;

    virtual std::vector<vk::DeviceQueueCreateInfo> deviceQueueCreateInfos() const;
    virtual vk::PhysicalDevice choosePhysicalDevice(const std::vector<vk::PhysicalDevice>&) const;

public:
    Context(bool init = 1, const Platform& platform = {});
    Context(VkInstance, VkPhysicalDevice phdev = 0, const Platform& platform = {});
    virtual ~Context();

	virtual void initInstance();
	virtual void initPhysicalDevice();
	virtual void initDevice();

    VkInstance vkInstance() const { return instance_; }
    VkPhysicalDevice vkPhysicalDevice() const { return physicalDevice_; }
    VkDevice vkDevice() const { return device_; }

	const Platform& platform() const { return platform_; }

    PFN_vkVoidFunction deviceProcAddr(const std::string& name) const;
    PFN_vkVoidFunction instanceProcAddr(const std::string& name) const;

    void waitDevice() const;
	const std::vector<Queue>& queues() const { return queues_; }
};

}
