#include <vpp/context.hpp>
#include <vpp/vulkan/vulkan.hpp>
#include <vpp/procAddr.hpp>

namespace vpp
{

/*
namespace
{

std::vector<const char*> strVecToCharVec(const std::vector<std::string>& vec)
{
	std::vector<const char*> ret;
	ret.reserve(vec.size());

	for(auto& str : vec)
		ret.push_back(str.c_str());

	return ret;
}

}

//
Context::Context(bool init, const Platform& platform) : platform_(platform)
{
    if(init)
    {
        initInstance();
        initPhysicalDevice();
        initDevice();
    }
}

Context::Context(vk::Instance instance, vk::PhysicalDevice phdev, const Platform& platform)
    : instance_(instance), physicalDevice_(phdev), platform_(platform)
{
    if(!instance_) initInstance();
	if(!physicalDevice_) initPhysicalDevice();

    initDevice();
}

Context::~Context()
{
    if(device_) vk::destroyDevice(device_, nullptr);
    if(instance_) vk::destroyInstance(instance_, nullptr);
}

void Context::initInstance()
{
    auto eName = engineName();
    auto aName = applicationName();

    vk::ApplicationInfo appInfo;
    appInfo.pApplicationName(aName.c_str());
    appInfo.applicationVersion(applicationVersion());
    appInfo.pEngineName(eName.c_str());
    appInfo.engineVersion(engineVersion());
    appInfo.apiVersion(VK_MAKE_VERSION(1, 0, 2)); //use header version when working

    auto layers = enabledInstanceLayers();
    auto extensions = enabledInstanceExtensions();

	auto extensionsChar = strVecToCharVec(extensions);
	auto layersChar = strVecToCharVec(layers);

    vk::InstanceCreateInfo info;
	info.enabledLayerCount(layers.size());
	info.ppEnabledLayerNames(layersChar.data());
	info.enabledExtensionCount(extensions.size());
	info.ppEnabledExtensionNames(extensionsChar.data());
    info.pApplicationInfo(&appInfo);

    VPP_CALL(vk::createInstance(&info, nullptr, &instance_));
}

void Context::initPhysicalDevice()
{
    if(!instance_) throw std::runtime_error("Context::initPhysicalDevice: invalid instance");

    std::vector<VkPhysicalDevice> phdevs{};
    VPP_CALL(vk::enumeratePhysicalDevices(vkInstance(), phdevs));
    physicalDevice_ = choosePhysicalDevice(phdevs);

	vk::getPhysicalDeviceMemoryProperties(physicalDevice_, &memoryProperties_);
}

void Context::initDevice()
{
    if(!physicalDevice_) throw std::runtime_error("Context::initDevice: invalid physicalDevice");

    auto layers = enabledDeviceLayers();
    auto extensions = enabledDeviceExtensions();
    auto queueInfos = deviceQueueCreateInfos();

	auto extensionsChar = strVecToCharVec(extensions);
	auto layersChar = strVecToCharVec(layers);

    vk::DeviceCreateInfo info{};
    info.queueCreateInfoCount(queueInfos.size());
	info.pQueueCreateInfos(queueInfos.data());
    info.enabledLayerCount(layers.size());
    info.ppEnabledLayerNames(layersChar.data());
	info.enabledExtensionCount(extensions.size());
	info.ppEnabledExtensionNames(extensionsChar.data());

    VPP_CALL(vk::createDevice(physicalDevice_, &info, nullptr, &device_));

	//retrieve/store requested queues
	std::map<unsigned int, unsigned int> familyIds;
	queues_.reserve(queueInfos.size());

	for(auto& info : queueInfos)
	{
		VkQueue queue;

		auto idx = familyIds[info.queueFamilyIndex()]++;
		vk::getDeviceQueue(vkDevice(), info.queueFamilyIndex(), idx, &queue);
		queues_.push_back({queue, info.queueFamilyIndex(), idx});
	}
}

std::vector<vk::DeviceQueueCreateInfo> Context::deviceQueueCreateInfos() const
{
    float priorities[1] = {0.0};
    unsigned int gfxqIndex = 0;

    vk::DeviceQueueCreateInfo info{};
    info.queueFamilyIndex(gfxqIndex);
    info.queueCount(1);
    info.pQueuePriorities(priorities);

    return {info};
}

std::vector<std::string> Context::enabledInstanceExtensions() const
{
	return platform_.instanceExtensions;
}

std::vector<std::string> Context::enabledDeviceExtensions() const
{
	return platform_.deviceExtensions;
}

std::vector<std::string> Context::enabledInstanceLayers() const
{
    return {};
}

std::vector<std::string> Context::enabledDeviceLayers() const
{
    return {};
}

VkPhysicalDevice Context::choosePhysicalDevice(const std::vector<vk::PhysicalDevice>& phdevs) const
{
    return phdevs[0];
}

void Context::waitDevice() const
{
    VPP_CALL(vk::deviceWaitIdle(vkDevice()));
}

int Context::memoryType(std::uint32_t typeBits, vk::MemoryPropertyFlags properties) const
{
	for(std::uint32_t i = 0; i < memoryProperties().memoryTypeCount(); ++i)
	{
		if(typeBits & (1 << i)) //ith bit set to 1
		{
			if((memoryProperties().memoryTypes()[i].propertyFlags() & properties) == properties)
				return 1;
		}
	}

	return -1;
}

PFN_vkVoidFunction Context::deviceProcAddr(const std::string& str) const
{
    if(device_) return deviceProc(vkDevice(), str);
    return nullptr;
}

PFN_vkVoidFunction Context::instanceProcAddr(const std::string& str) const
{
    if(instance_) return instanceProc(vkInstance(), str);
    return nullptr;
}
*/

}
