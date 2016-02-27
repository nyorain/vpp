#include <vpp/device.hpp>
#include <vpp/call.hpp>

namespace vpp
{

Device::Device(vk::Instance ini, vk::PhysicalDevice phdev, const vk::DeviceCreateInfo& info)
	: instance_(ini), physicalDevice_(phdev)
{
	//physicalDevice properties
	vk::getPhysicalDeviceMemoryProperties(vkPhysicalDevice(), &properties_);

	//physicalDevice memoryProperties
	vk::getPhysicalDeviceMemoryProperties(vkPhysicalDevice(), &memoryProperties_);

	//createDevice
	VPP_CALL(vk::createDevice(vkPhysicalDevice(), &info, nullptr, &device_));

	//retrieve/store requested queues
	auto qproperties = vk::getPhysicalDeviceQueueFamilyProperties(vkPhysicalDevice());
	std::map<std::uint32_t, unsigned int> familyIds;
	queues_.reserve(info.queueCreateInfoCount());
	vk::queue queue;

	for(std::size_t i(0); i < info.queueCreateInfoCount(); ++i)
	{
		auto& queueInfo = info.pQueueCreateInfos()[i];
		auto idx = familyIds[queueInfo.queueFamilyIndex()]++;
		vk::getDeviceQueue(vkDevice(), queueInfo.queueFamilyIndex(), idx, &queue);

		queues_.push_back({queue, qproperties[i], queueInfo.queueFamilyIndex(), idx});
	}
}

Device::~Device()
{
	if(vkDevice()) vk::destroyDevice(device_, nullptr);
}

void Device::waitIdle() const
{
    VPP_CALL(vk::deviceWaitIdle(vkDevice()));
}

vk::Queue Device::queue(std::uint32_t family) const
{
	for(auto& queue : queues())
		if(queue.family == family) return queue.queue;

	return 0;
}

vk::Queue Device::queue(std::uint32_t family, std::uint32_t id) const
{
	for(auto& queue : queues())
		if(queue.family == family && queue.id == id) return queue.queue;

	return 0;
}

int Context::memoryType(std::uint32_t typeBits, vk::MemoryPropertyFlags mflags) const
{
	for(std::uint32_t i = 0; i < memoryProperties().memoryTypeCount(); ++i)
	{
		if(typeBits & (1 << i)) //ith bit set to 1
		{
			if((memoryProperties().memoryTypes()[i].propertyFlags() & mflags) == mflags)
				return i;
		}
	}

	return -1;
}

std::uint32_t Context::memoryTypeBits(std::uint32_t typeBits, vk::MemoryPropertyFlags mflags) const
{
	for(std::uint32_t i = 0; i < memoryProperties().memoryTypeCount(); ++i)
	{
		if(typeBits & (1 << i)) //ith bit set to 1
		{
			if((memoryProperties().memoryTypes()[i].propertyFlags() & mflags) != mflags)
			{
				typeBits &= ~(1 << i); //unset ith bit
			}
		}
	}

	return typeBits;
}

}
