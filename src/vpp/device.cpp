#include <vpp/device.hpp>
#include <vpp/provider.hpp>
#include <vpp/commandBuffer.hpp>

namespace vpp
{

Device::Device() = default;

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

	cbProvider_.reset(new CommandBufferProvider(*this));
	dmProvider_.reset(new DeviceMemoryProvider(*this));
	commandManager_.reset(new CommandManager(*this));
}

Device::~Device()
{
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

CommandBufferProvider& Device::commandBufferProvider() const
{
	return *cbProvider_;
}

SetupCommandBuffer Device::setupCommandBuffer() const
{
	//todo: querygood qFamily instead of using 0
	return std::move(SetupCommandBuffer(std::move(commandBufferProvider().allocate(0))));
}

void Device::finishSetup() const
{
	commandManager_->wait();
}

DeviceMemoryProvider& Device::deviceMemoryProvider() const
{
	return *dmProvider_;
}

DeviceMemoryAllocator& Device::deviceMemoryAllocator() const
{
	return deviceMemoryProvider().get();
}

}
