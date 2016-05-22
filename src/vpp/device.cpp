#include <vpp/device.hpp>
#include <vpp/provider.hpp>
#include <vpp/commandBuffer.hpp>
#include <vpp/submit.hpp>
#include <vpp/transfer.hpp>

namespace vpp
{

Device::Device() = default;

Device::Device(vk::Instance ini, vk::PhysicalDevice phdev, const vk::DeviceCreateInfo& info)
	: instance_(ini), physicalDevice_(phdev)
{
	vk::getPhysicalDeviceProperties(vkPhysicalDevice(), &physicalDeviceProperties_);
	vk::getPhysicalDeviceMemoryProperties(vkPhysicalDevice(), &memoryProperties_);

	vk::createDevice(vkPhysicalDevice(), &info, nullptr, &device_);

	//retrieve/store requested queues
	auto qproperties = vk::getPhysicalDeviceQueueFamilyProperties(vkPhysicalDevice());
	std::map<std::uint32_t, unsigned int> familyIds;
	queues_.reserve(info.queueCreateInfoCount());
	vk::Queue queue;

	for(std::size_t i(0); i < info.queueCreateInfoCount(); ++i)
	{
		auto& queueInfo = info.pQueueCreateInfos()[i];
		auto idx = familyIds[queueInfo.queueFamilyIndex()]++;
		vk::getDeviceQueue(vkDevice(), queueInfo.queueFamilyIndex(), idx, &queue);

		queues_.push_back({queue, qproperties[i], queueInfo.queueFamilyIndex(), idx});
	}

	//setup provider and manager
	commandProvider_.reset(new CommandProvider(*this));
	memoryProvider_.reset(new DeviceMemoryProvider(*this));
	submitManager_.reset(new SubmitManager(*this));
	transferManager_.reset(new TransferManager(*this));
}

Device::~Device()
{
	transferManager_.reset();
	submitManager_.reset();
	memoryProvider_.reset();
	commandProvider_.reset();

	if(vkDevice()) vk::destroyDevice(device_, nullptr);
}

void Device::waitIdle() const
{
    vk::deviceWaitIdle(vkDevice());
}

const Device::Queue* Device::queue(std::uint32_t family) const
{
	for(auto& queue : queues())
		if(queue.family == family) return &queue;

	return nullptr;
}

const Device::Queue* Device::queue(std::uint32_t family, std::uint32_t id) const
{
	for(auto& queue : queues())
		if(queue.family == family && queue.id == id) return &queue;

	return nullptr;
}

const Device::Queue* Device::queue(vk::QueueFlags flags) const
{
	for(auto& queue : queues())
		if(queue.properties.queueFlags() & flags) return &queue;

	return nullptr;
}

int Device::memoryType(vk::MemoryPropertyFlags mflags, std::uint32_t typeBits) const
{
	for(std::uint32_t i = 0; i < memoryProperties().memoryTypeCount(); ++i)
	{
		if(typeBits & (1 << i)) //ith bit set to 1?
		{
			if((memoryProperties().memoryTypes()[i].propertyFlags() & mflags) == mflags)
				return i;
		}
	}

	return -1;
}

std::uint32_t Device::memoryTypeBits(vk::MemoryPropertyFlags mflags, std::uint32_t typeBits) const
{
	for(std::uint32_t i = 0; i < memoryProperties().memoryTypeCount(); ++i)
	{
		if(typeBits & (1 << i)) //ith bit set to 1?
		{
			if((memoryProperties().memoryTypes()[i].propertyFlags() & mflags) != mflags)
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
