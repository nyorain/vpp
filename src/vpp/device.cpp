#include <vpp/device.hpp>
#include <vpp/vk.hpp>
#include <vpp/provider.hpp>
#include <vpp/queue.hpp>
#include <vpp/commandBuffer.hpp>
#include <vpp/submit.hpp>
#include <vpp/transfer.hpp>

namespace vpp
{

//Device Impl
struct Device::Impl
{
	vk::PhysicalDeviceProperties physicalDeviceProperties;
	vk::PhysicalDeviceMemoryProperties memoryProperties;

	std::vector<Queue> queues;
	std::vector<vk::QueueFamilyProperties> qFamilyProperties;

	CommandProvider commandProvider;
	DeviceMemoryProvider memoryProvider;
	SubmitManager submitManager;
	TransferManager transferManager;

	Impl(const Device& dev) : commandProvider(dev), memoryProvider(dev), submitManager(dev), 
		transferManager(dev) {}
};


//Device
Device::Device() = default;

Device::Device(vk::Instance ini, vk::PhysicalDevice phdev, const vk::DeviceCreateInfo& info)
	: instance_(ini), physicalDevice_(phdev)
{
	impl_.reset(new Impl(*this));

	vk::getPhysicalDeviceProperties(vkPhysicalDevice(), &impl_->physicalDeviceProperties);
	vk::getPhysicalDeviceMemoryProperties(vkPhysicalDevice(), &impl_->memoryProperties);

	vk::createDevice(vkPhysicalDevice(), &info, nullptr, &device_);

	//retrieve/store requested queues
	std::uint32_t size;
	vk::getPhysicalDeviceQueueFamilyProperties(vkPhysicalDevice(), &size, nullptr);
	impl_->qFamilyProperties.resize(size);
	vk::getPhysicalDeviceQueueFamilyProperties(vkPhysicalDevice(), &size, impl_->qFamilyProperties.data());

	std::map<std::uint32_t, unsigned int> familyIds;
	impl_->queues.reserve(info.queueCreateInfoCount);
	vk::Queue queue;

	for(std::size_t i(0); i < info.queueCreateInfoCount; ++i)
	{
		auto& queueInfo = info.pQueueCreateInfos[i];
		auto idx = familyIds[queueInfo.queueFamilyIndex]++;
		vk::getDeviceQueue(vkDevice(), queueInfo.queueFamilyIndex, idx, &queue);

		impl_->queues.push_back({queue, impl_->qFamilyProperties[i], queueInfo.queueFamilyIndex, idx});
	}
}

Device::~Device()
{
	impl_.reset();
	if(vkDevice()) vk::destroyDevice(device_, nullptr);
}

void Device::waitIdle() const
{
    vk::deviceWaitIdle(vkDevice());
}

const std::vector<Queue>& Device::queues() const
{
	return impl_->queues;
}

const Queue* Device::queue(std::uint32_t family) const
{
	for(auto& queue : queues())
		if(queue.family() == family) return &queue;

	return nullptr;
}

const Queue* Device::queue(std::uint32_t family, std::uint32_t id) const
{
	for(auto& queue : queues())
		if(queue.family() == family && queue.id() == id) return &queue;

	return nullptr;
}

const Queue* Device::queue(vk::QueueFlags flags) const
{
	for(auto& queue : queues())
		if(queue.properties().queueFlags & flags) return &queue;

	return nullptr;
}

const vk::PhysicalDeviceMemoryProperties& Device::memoryProperties() const
{
	return impl_->memoryProperties;
}

const vk::PhysicalDeviceProperties& Device::properties() const
{
	return impl_->physicalDeviceProperties;
}

int Device::memoryType(vk::MemoryPropertyFlags mflags, std::uint32_t typeBits) const
{
	for(std::uint32_t i = 0; i < memoryProperties().memoryTypeCount; ++i)
	{
		if(typeBits & (1 << i)) //ith bit set to 1?
		{
			if((memoryProperties().memoryTypes[i].propertyFlags & mflags) == mflags)
				return i;
		}
	}

	return -1;
}

std::uint32_t Device::memoryTypeBits(vk::MemoryPropertyFlags mflags, std::uint32_t typeBits) const
{
	for(std::uint32_t i = 0; i < memoryProperties().memoryTypeCount; ++i)
	{
		if(typeBits & (1 << i)) //ith bit set to 1?
		{
			if((memoryProperties().memoryTypes[i].propertyFlags & mflags) != mflags)
				typeBits &= ~(1 << i); //unset ith bit
		}
	}

	return typeBits;
}

void Device::finishSetup() const
{
	submitManager().submit();
	waitIdle();
}

DeviceMemoryAllocator& Device::memoryAllocator() const
{
	return memoryProvider().get();
}

}
