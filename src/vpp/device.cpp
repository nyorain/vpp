#include <vpp/device.hpp>

namespace vpp
{

Device::Device(vk::Instance ini, vk::PhysicalDevice phdev, const vk::DeviceCreateInfo& info)
	: instance_(ini), physicalDevice_(phdev)
{
	//physicalDevice properties
	vk::getPhysicalDeviceProperties(vkPhysicalDevice(), &physicalDeviceProperties_);

	//physicalDevice memoryProperties
	vk::getPhysicalDeviceMemoryProperties(vkPhysicalDevice(), &memoryProperties_);

	//createDevice
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
}

Device::~Device()
{
	if(vkDevice()) vk::destroyDevice(device_, nullptr);
}

Device::Device(Device&& other) noexcept
{
	this->swap(other);
}

Device& Device::operator=(Device&& other) noexcept
{
	Device swapper(std::move(other));
	this->swap(swapper);
	return *this;
}

void Device::swap(Device& other) noexcept
{
	using std::swap;

	swap(device_, other.device_);
	swap(physicalDevice_, other.physicalDevice_);
	swap(instance_, other.instance_);
	swap(queues_, other.queues_);
	swap(physicalDeviceProperties_, other.physicalDeviceProperties_);
	swap(memoryProperties_, other.memoryProperties_);
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

int Device::memoryType(std::uint32_t typeBits, vk::MemoryPropertyFlags mflags) const
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

std::uint32_t Device::memoryTypeBits(std::uint32_t typeBits, vk::MemoryPropertyFlags mflags) const
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

}
