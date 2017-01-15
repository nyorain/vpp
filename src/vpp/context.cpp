// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#include <vpp/context.hpp>
#include <vpp/procAddr.hpp>
#include <vpp/vk.hpp>
#include <vpp/debug.hpp>
#include <vpp/device.hpp>

#include <stdexcept>
#include <cstring>

namespace vpp {

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

	swap(a.instance_, b.instance_);
	swap(a.device_, b.device_);
	swap(a.surface_, b.surface_);
	swap(a.swapChain_, b.swapChain_);
	swap(a.presentQueue_, b.presentQueue_);
	swap(a.graphicsComputeQueue_, b.graphicsComputeQueue_);
	swap(a.debugCallback_, b.debugCallback_);
}

void Context::initInstance(vk::DebugReportFlagsEXT debugFlags,
	nytl::Span<const char* const> reqExtensions,
	nytl::Span<const char* const> reqLayers, bool reverseLayers)
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
	appInfo.apiVersion = VK_MAKE_VERSION(1, 0, 11);

	//iniinfo
	std::vector<const char*> extensions;
	auto enumeratedExtensions = vk::enumerateInstanceExtensionProperties(nullptr);
	extensions.reserve(enumeratedExtensions.size() + 1);
	for(auto& e : reqExtensions)
	{
		auto found = false;
		for(auto& ee : enumeratedExtensions)
		{
			if(std::strcmp(ee.extensionName.data(), e) == 0)
			{
				extensions.push_back(e);
				found = true;
				break;
			}
		}

		if(!found) std::cout << "vpp::Context: device extension " << e << " not available\n";
	}
	extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);

	std::vector<const char*> layers;
	auto enumeratedLayers = vk::enumerateInstanceLayerProperties();
	layers.reserve(enumeratedLayers.size());

	for(auto& l : reqLayers)
	{
		auto found = false;
		for(auto& el : enumeratedLayers)
		{
			if((std::strcmp(el.layerName.data(), l) == 0) ^ reverseLayers)
			{
				layers.push_back(l);
				found = true;
				break;
			}
		}

		if(!found && !reverseLayers)
			std::cout << "vpp::Context: instance layer " << l << " not available\n";
	}

	if(debugFlags != 0) extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);

	vk::InstanceCreateInfo iniinfo;
	iniinfo.enabledLayerCount = layers.size();
	iniinfo.ppEnabledLayerNames = layers.data();
	iniinfo.enabledExtensionCount = extensions.size();
	iniinfo.ppEnabledExtensionNames = extensions.data();
	iniinfo.pApplicationInfo = &appInfo;

	instance_ = vk::createInstance(iniinfo);
	if(debugFlags != 0) debugCallback_ = std::make_unique<DebugCallback>(vkInstance(), debugFlags);
}

vk::PhysicalDevice Context::choosePhysicalDevice(nytl::Span<const vk::PhysicalDevice> phdevs) const
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

void Context::initDevice(nytl::Span<const char* const> reqExtensions,
	nytl::Span<const char* const> reqLayers, bool reverseLayers)
{
	auto phdevs = vk::enumeratePhysicalDevices(vkInstance());
	auto phdev = choosePhysicalDevice(phdevs);

	initDevice(phdev, reqExtensions, reqLayers, reverseLayers);
}

void Context::initDevice(vk::PhysicalDevice phdev, nytl::Span<const char* const> reqExtensions,
	nytl::Span<const char* const> reqLayers, bool reverseLayers)
{
	//extensions & layers
	//atm: activate all layes - make this configurable maybe?
	//or at least query the layers and not use the hard coded names?
	std::vector<const char*> extensions;
	auto enumeratedExtensions = vk::enumerateDeviceExtensionProperties(phdev, nullptr);
	extensions.reserve(enumeratedExtensions.size() + 1);
	for(auto& e : reqExtensions)
	{
		auto found = false;
		for(auto& ee : enumeratedExtensions)
		{
			if(std::strcmp(ee.extensionName.data(), e) == 0)
			{
				extensions.push_back(e);
				found = true;
				break;
			}
		}

		if(!found) std::cout << "vpp::Context: device extension " << e << " not available\n";
	}
	extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

	std::vector<const char*> layers;
	auto enumeratedLayers = vk::enumerateDeviceLayerProperties(phdev);
	layers.reserve(enumeratedLayers.size() + 1);

	for(auto& l : reqLayers)
	{
		auto found = false;
		for(auto& el : enumeratedLayers)
		{
			if((std::strcmp(el.layerName.data(), l) == 0) ^ reverseLayers)
			{
				layers.push_back(l);
				found = true;
				break;
			}
		}

		if(!found && !reverseLayers)
			std::cout << "vpp::Context: device layer " << l << " not available\n";
	}

	//queues
	auto queues = surface().supportedQueueFamilies(phdev);
	auto queueProps = vk::getPhysicalDeviceQueueFamilyProperties(phdev);

	std::uint32_t presentQFam = -1;
	std::uint32_t graphicsComputeQFam = -1;

	static const auto bothFlags = vk::QueueBits::graphics | vk::QueueBits::compute;

	for(auto i = 0u; i < queueProps.size(); ++i)
	{
		const auto& qProp = queueProps[i];
		if(surface().queueFamilySupported(phdev, i))
		{
			presentQFam = i;
			if((qProp.queueFlags & bothFlags) == bothFlags)
			{
				graphicsComputeQFam = i;
				break;
			}
		}
		else if((qProp.queueFlags & bothFlags) == bothFlags)
		{
			graphicsComputeQFam = i;
		}
	}

	if(presentQFam == std::uint32_t(-1))
		throw std::runtime_error("vpp::Context: unable to get present queue");

	if(graphicsComputeQFam == std::uint32_t(-1))
		throw std::runtime_error("vpp::Context: unable to get gfx/comp queue");

	//create
	vk::DeviceCreateInfo devinfo{};
	devinfo.queueCreateInfoCount = 1;

	//queues
	float priorities[1] = {0.0};

	vk::DeviceQueueCreateInfo queueInfos[2];
	queueInfos[0].queueFamilyIndex = presentQFam;
	queueInfos[0].queueCount = 1;
	queueInfos[0].pQueuePriorities = priorities;

	if(graphicsComputeQFam != presentQFam)
	{
		devinfo.queueCreateInfoCount = 2;
		queueInfos[1].queueFamilyIndex = graphicsComputeQFam;
		queueInfos[1].queueCount = 1;
		queueInfos[1].pQueuePriorities = priorities;
	}

	devinfo.pQueueCreateInfos = queueInfos;
	devinfo.enabledLayerCount = layers.size();
	devinfo.ppEnabledLayerNames = layers.data();
	devinfo.enabledExtensionCount = extensions.size();
	devinfo.ppEnabledExtensionNames = extensions.data();

	// vk::getPhysicalDeviceQueueFamilyProperties(phdev);
	device_.reset(new Device(vkInstance(), phdev, devinfo));

	presentQueue_ = device().queue(presentQFam, 0);
	graphicsComputeQueue_ = device().queue(graphicsComputeQFam, 0);
}

void Context::initSwapChain(const vk::Extent2D& size, const SwapChainSettings& settings)
{
	swapChain_ = SwapChain(device(), surface(), size, settings);
}

vk::Device Context::vkDevice() const
{
	return device().vkDevice();
}

}
