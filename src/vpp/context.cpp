#include <vpp/context.hpp>
#include <vpp/procAddr.hpp>
#include <vpp/debug.hpp>
#include <vpp/surface.hpp>
#include <vpp/swapChain.hpp>

#include <stdexcept>

namespace vpp
{

Context::Context() = default;
Context::~Context() = default;

void Context::initInstance(const CreateInfo& info)
{
	//appinfo
    auto eName = "vpp";
    auto aName = "unknown";

	auto aVersion = 0;
	auto eVersion = 0;

    vk::ApplicationInfo appInfo;
    appInfo.pApplicationName(aName);
    appInfo.applicationVersion(aVersion);
    appInfo.pEngineName(eName);
    appInfo.engineVersion(eVersion);
    appInfo.apiVersion(VK_MAKE_VERSION(1, 0, 2)); //use header version when working

	//iniinfo
	std::vector<const char*> extensions = info.instanceExtensions;
	extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);

	std::vector<const char*> layers;

	if(info.debug)
	{
		layers = validationLayerNames;
		extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
	}

    vk::InstanceCreateInfo iniinfo;
	iniinfo.enabledLayerCount(layers.size());
	iniinfo.ppEnabledLayerNames(layers.data());
	iniinfo.enabledExtensionCount(extensions.size());
	iniinfo.ppEnabledExtensionNames(extensions.data());
    iniinfo.pApplicationInfo(&appInfo);

    vk::createInstance(&iniinfo, nullptr, &instance_);

	if(info.debug)
	{
		auto flags =
			vk::DebugReportFlagBitsEXT::ErrorEXT |
			vk::DebugReportFlagBitsEXT::WarningEXT |
			vk::DebugReportFlagBitsEXT::PerformanceWarningEXT;

		debugCallback_.reset(new DebugCallback(vkInstance(), flags));
	}
}

void Context::initDevice(const CreateInfo& info)
{
	//phyiscal device - TODO

	std::vector<vk::PhysicalDevice> phdevs;
    vk::enumeratePhysicalDevices(vkInstance(), phdevs);
/*
auto size = 0u;
vk::enumeratePhysicalDevices(vkInstance(), &size, nullptr);

std::vector<vk::PhysicalDevice> phdevs(size);
vk::enumeratePhysicalDevices(vkInstance(), &size, phdevs.data());
*/

	if(phdevs.empty())
	{
		throw std::runtime_error("no physical devices");
	}

	auto phdev = phdevs[0];

	//extensions & layers
	std::vector<const char*> extensions = info.deviceExtensions;
	extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

	std::vector<const char*> layers;

	if(info.debug)
	{
		layers = validationLayerNames;
	}

	//queues
	auto queues = surface().supportedQueueFamilies(phdev);
	auto queueProps = vk::getPhysicalDeviceQueueFamilyProperties(phdev);

	std::uint32_t queueFamily = 99;
	for(auto queue : queues)
	{
		if(queueProps[queue].queueFlags() & vk::QueueFlagBits::Graphics)
		{
			queueFamily = queue;
			break;
		}
	}

	if(queueFamily == 99)
	{
		throw std::runtime_error("unable to get present & graphical queue");
	}

	float priorities[1] = {0.0};
	std::vector<vk::DeviceQueueCreateInfo> queueInfos(1);
	queueInfos[0] = vk::DeviceQueueCreateInfo({}, queueFamily, 1, priorities);

	//create
    vk::DeviceCreateInfo devinfo{};
    devinfo.queueCreateInfoCount(queueInfos.size());
	devinfo.pQueueCreateInfos(queueInfos.data());
    devinfo.enabledLayerCount(layers.size());
    devinfo.ppEnabledLayerNames(layers.data());
	devinfo.enabledExtensionCount(extensions.size());
	devinfo.ppEnabledExtensionNames(extensions.data());

    device_ = Device(vkInstance(), phdev, devinfo);

	queue_ = device_.queue(queueFamily);
	if(!queue_)
	{
		throw std::runtime_error("failed to create queue");
	}
}

void Context::initSwapChain(const CreateInfo& info)
{
	swapChain_ = SwapChain(device(), surface(), info.size);
}

}
