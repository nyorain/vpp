#include <vpp/device.hpp>
#include <vpp/vk.hpp>
#include <vpp/provider.hpp>
#include <vpp/queue.hpp>
#include <vpp/commandBuffer.hpp>
#include <vpp/submit.hpp>
#include <vpp/transfer.hpp>

#include <vpp/utility/pmr/unique_ptr.hpp>

#include <cstdlib>

namespace vpp
{

//Device Impl
struct Device::Impl
{
	CommandProvider commandProvider;
	DeviceMemoryProvider deviceMemoryProvider;
	HostMemoryProvider hostMemoryProvider;

	SubmitManager submitManager;
	TransferManager transferManager;

	vk::PhysicalDeviceProperties physicalDeviceProperties;
	vk::PhysicalDeviceMemoryProperties memoryProperties;

	std::size_t queueCount;
	spm::unique_ptr<Queue[]> queues;

	std::vector<vk::QueueFamilyProperties> qFamilyProperties;


	Impl(const Device& dev, std::size_t i) : commandProvider(dev), deviceMemoryProvider(dev), hostMemoryProvider(),
		submitManager(dev), transferManager(dev), queues(nullptr, {hostMemoryProvider.get(), i}) {}
};

//Device
Device::Device() = default;

Device::Device(vk::Instance ini, vk::PhysicalDevice phdev, const vk::DeviceCreateInfo& info)
	: instance_(ini), physicalDevice_(phdev)
{
	device_ = vk::createDevice(vkPhysicalDevice(), info);

	impl_.reset(new Impl(*this, info.queueCreateInfoCount));
	impl_->physicalDeviceProperties = vk::getPhysicalDeviceProperties(vkPhysicalDevice());
	impl_->memoryProperties = vk::getPhysicalDeviceMemoryProperties(vkPhysicalDevice());
	impl_->qFamilyProperties = vk::getPhysicalDeviceQueueFamilyProperties(vkPhysicalDevice());

	std::map<std::uint32_t, unsigned int> familyIds;

	auto ptr = hostMemoryResource().allocate(sizeof(Queue) * info.queueCreateInfoCount, alignof(Queue));
	impl_->queues.reset(reinterpret_cast<Queue*>(ptr));
	impl_->queueCount = info.queueCreateInfoCount;

	for(std::size_t i(0); i < info.queueCreateInfoCount; ++i)
	{
		auto& queueInfo = info.pQueueCreateInfos[i];
		auto idx = familyIds[queueInfo.queueFamilyIndex]++;
		auto queue = vk::getDeviceQueue(vkDevice(), queueInfo.queueFamilyIndex, idx);

		auto* ptr = new(&impl_->queues[i]) Queue(queue, impl_->qFamilyProperties[i],
			queueInfo.queueFamilyIndex, idx);

		std::cout << queue << " vs " << impl_->queues[i].vkQueue() << "\n";
		std::cout << &impl_->queues[i] << "\n";
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

Range<Queue> Device::queues() const
{
	return {impl_->queues.get(), impl_->queueCount};
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

DeviceMemoryAllocator& Device::deviceAllocator() const
{
	return memoryProvider().get();
}

std::pmr::memory_resource& Device::hostMemoryResource() const
{
	return hostMemoryProvider().get();
}

HostMemoryProvider& Device::hostMemoryProvider() const
{
	return impl_->hostMemoryProvider;
}

DeviceMemoryProvider& Device::memoryProvider() const
{
	return impl_->deviceMemoryProvider;
}

CommandProvider& Device::commandProvider() const
{
	return impl_->commandProvider;
}

SubmitManager& Device::submitManager() const
{
	return impl_->submitManager;
}

TransferManager& Device::transferManager() const
{
	return impl_->transferManager;
}

}
