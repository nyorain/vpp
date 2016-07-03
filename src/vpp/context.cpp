#include <vpp/context.hpp>
#include <vpp/procAddr.hpp>
#include <vpp/vk.hpp>
#include <vpp/debug.hpp>
#include <vpp/device.hpp>

#include <stdexcept>

namespace vpp
{

Context::Context() = default;
Context::~Context() = default;

Context::Context(Context&& other) noexcept
{
	swap(*this, other);
}

Context& Context::operator=(Context other) noexcept
{
	swap(*this, other);
	return *this;
}

void swap(Context& a, Context& b) noexcept
{
	using std::swap;

	swap(a.device_, b.device_);
	swap(a.instance_, b.instance_);
	swap(a.surface_, b.surface_);
	swap(a.swapChain_, b.swapChain_);
	swap(a.presentQueue_, b.presentQueue_);
	swap(a.debugCallback_, b.debugCallback_);
}

void Context::initInstance(const CreateInfo& info)
{
	std::vector<const char*> layers;
	auto vec = vk::enumerateInstanceLayerProperties();
	for(auto& layer : vec)
	{
		std::cout << "layer: " << layer.layerName.data() << "\n";
		//layers.push_back(layer.layerName.data());
	}

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
	appInfo.apiVersion = VK_MAKE_VERSION(1, 0, 11); //use header version when working

	//iniinfo
	std::vector<const char*> extensions = info.instanceExtensions;
	extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);

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
	std::cout << "ini: " << instance_ << "\n";

	if(info.debugFlags != 0)
	{
		auto flags = info.debugFlags;
		debugCallback_.reset(new DebugCallback(vkInstance(), flags));
	}
}

vk::PhysicalDevice Context::choosePhysicalDevice(const std::vector<vk::PhysicalDevice>& phdevs) const
{
	for(auto& phdev : phdevs)
	{
		auto queues = surface().supportedQueueFamilies(phdev);
		if(!queues.empty())
		{
			return phdev;
		}
	}
	
	throw std::runtime_error("vpp::Context: no valid physical devices");
}

void Context::initDevice(const CreateInfo& info)
{
	//phyiscal device
	auto phdevs = vk::enumeratePhysicalDevices(vkInstance());
	auto phdev = choosePhysicalDevice(phdevs);

	//extensions & layers
	//atm: activate all layes - make this configurable maybe?
	//or at least query the layers and not use the hard coded names?
	std::vector<const char*> extensions = info.deviceExtensions;
	extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

	std::vector<const char*> layers;
	if(info.debugFlags != 0) layers = validationLayerNames;

	//queues
	auto queueProps = vk::getPhysicalDeviceQueueFamilyProperties(phdev);

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
	swapChain_ = SwapChain(device(), surface(), {info.width, info.height});
}

const vk::Device& Context::vkDevice() const
{
	return device().vkDevice();
}

}
