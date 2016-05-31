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
    appInfo.pApplicationName = aName;
    appInfo.applicationVersion = aVersion;
    appInfo.pEngineName = eName;
    appInfo.engineVersion = eVersion;
    appInfo.apiVersion = VK_MAKE_VERSION(1, 0, 2); //use header version when working

	//iniinfo
	std::vector<const char*> extensions = info.instanceExtensions;
	extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);

	std::vector<const char*> layers;

	if(info.debugFlags != 0)
	{
		layers = validationLayerNames;
		extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
	}

    vk::InstanceCreateInfo iniinfo;
	iniinfo.enabledLayerCount = layers.size();
	iniinfo.ppEnabledLayerNames = layers.data();
	iniinfo.enabledExtensionCount = extensions.size();
	iniinfo.ppEnabledExtensionNames = extensions.data();
    iniinfo.pApplicationInfo = &appInfo;

    instance_ = vk::createInstance(iniinfo);

	if(info.debugFlags != 0)
	{
		auto flags = info.debugFlags;
		debugCallback_.reset(new DebugCallback(vkInstance(), flags));
	}
}

vk::PhysicalDevice Context::choosePhysicalDevice(const std::vector<vk::PhysicalDevice>& phdevs) const
{
	if(phdevs.empty())
	{
		throw std::runtime_error("Context::choosePhysicalDevice: no physical devices");
	}

	//TODO - algorithm for choosing, presetnation support
	return phdevs[0];
}

void Context::initDevice(const CreateInfo& info)
{
	//phyiscal device
	std::vector<vk::PhysicalDevice> phdevs;
	auto size = 0u;
    vk::enumeratePhysicalDevices(vkInstance(), size, nullptr);
	phdevs.reserve(size);
    vk::enumeratePhysicalDevices(vkInstance(), size, phdevs.data());
	auto phdev = choosePhysicalDevice(phdevs);

	//extensions & layers
	std::vector<const char*> extensions = info.deviceExtensions;
	extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

	std::vector<const char*> layers;

	if(info.debugFlags != 0)
	{
		layers = validationLayerNames;
	}

	//queues
	std::vector<vk::QueueFamilyProperties> queueProps;
	size = 0u;
	vk::getPhysicalDeviceQueueFamilyProperties(phdev, size, nullptr);
	queueProps.reserve(size);
	vk::getPhysicalDeviceQueueFamilyProperties(phdev, size, queueProps.data());

	//present queue
	auto queues = surface().supportedQueueFamilies(phdev);
	std::uint32_t presentQueueFamily = -1; //only queueFamilies < 32 are valid
	std::uint32_t presentQueueId = 0;

	for(auto queue : queues)
	{
		if(queueProps[queue].queueFlags & vk::QueueBits::graphics)
		{
			presentQueueFamily = queue;
			break;
		}
	}

	if(presentQueueFamily == std::uint32_t(-1))
	{
		throw std::runtime_error("unable to get present & graphical queue");
	}

	float priorities[1] = {0.0};
	std::vector<vk::DeviceQueueCreateInfo> queueInfos = info.extraQueues;

	bool found = 0;
	for(auto& queueinfo : queueInfos)
	{
		if(queueinfo.queueFamilyIndex == presentQueueFamily)
		{
			if(info.extraPresentQueue)
			{
				presentQueueId = queueinfo.queueCount;
				queueinfo.queueCount = queueinfo.queueCount + 1;
			}

			found = 1;
			break;
		}
	}

	if(!found)
	{
		queueInfos.push_back({{}, presentQueueFamily, 1, priorities});
	}

	//create
    vk::DeviceCreateInfo devinfo{};
    devinfo.queueCreateInfoCount = queueInfos.size();
	devinfo.pQueueCreateInfos = queueInfos.data();
    devinfo.enabledLayerCount = layers.size();
    devinfo.ppEnabledLayerNames = layers.data();
	devinfo.enabledExtensionCount = extensions.size();
	devinfo.ppEnabledExtensionNames = extensions.data();

    device_.reset(new Device(vkInstance(), phdev, devinfo));

	presentQueue_ = device().queue(presentQueueFamily, presentQueueId);
	if(!presentQueue_)
	{
		throw std::runtime_error("failed to create queue");
	}
}

void Context::initSwapChain(const CreateInfo& info)
{
	swapChain_ = SwapChain(device(), surface(), info.size);
}

}
