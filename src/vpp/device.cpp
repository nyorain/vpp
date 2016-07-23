#include <vpp/device.hpp>
#include <vpp/vk.hpp>
#include <vpp/provider.hpp>
#include <vpp/queue.hpp>
#include <vpp/commandBuffer.hpp>
#include <vpp/submit.hpp>
#include <vpp/transfer.hpp>

#include <cstdlib>
#include <thread>
#include <mutex>

namespace vpp
{

struct Device::TLStorage
{
	std::vector<CommandPool> commandPools;
	std::pmr::unsynchronized_pool_resource memoryResource;
	DeviceMemoryAllocator deviceAllocator;
	// VulkanAllocator vulkanAllocator;

	// TLStorage(const Device& dev) : deviceAllocator(dev), vulkanAllocator(memoryResource) {}
	TLStorage(const Device& dev) : deviceAllocator(dev) {}
};

//Device Impl
//XXX: care for order in this structure since some of the vars depend on each other.
//i.e. tlStorage (with deviceAllocator and memoryResource) should not be placed after
//submitManager or transferManger or commandProvider.
struct Device::Impl
{
	std::map<std::thread::id, TLStorage> tlStorage;
	std::mutex storageMutex; //use a shared_mutex here with c++17.

	CommandProvider commandProvider;
	SubmitManager submitManager;
	TransferManager transferManager;

	vk::PhysicalDeviceProperties physicalDeviceProperties;
	vk::PhysicalDeviceMemoryProperties memoryProperties;

	std::vector<vk::QueueFamilyProperties> qFamilyProperties;
	std::vector<std::unique_ptr<Queue>> queues;

	Impl(const Device& dev) : commandProvider(dev), submitManager(dev), transferManager(dev) {}
};

//Device
Device::Device() = default;

Device::Device(vk::Instance ini, vk::PhysicalDevice phdev, const vk::DeviceCreateInfo& info)
	: instance_(ini), physicalDevice_(phdev)
{
	auto qProps = vk::getPhysicalDeviceQueueFamilyProperties(vkPhysicalDevice());
	device_ = vk::createDevice(vkPhysicalDevice(), info);

	impl_.reset(new Impl(*this));
	impl_->physicalDeviceProperties = vk::getPhysicalDeviceProperties(vkPhysicalDevice());
	impl_->memoryProperties = vk::getPhysicalDeviceMemoryProperties(vkPhysicalDevice());
	impl_->qFamilyProperties = qProps;

	std::map<std::uint32_t, unsigned int> familyIds; //for counting (and passing) the correct ids
	impl_->queues.resize(info.queueCreateInfoCount);

	for(std::size_t i(0); i < info.queueCreateInfoCount; ++i)
	{
		auto& queueInfo = info.pQueueCreateInfos[i];
		auto idx = familyIds[queueInfo.queueFamilyIndex]++;
		auto queue = vk::getDeviceQueue(vkDevice(), queueInfo.queueFamilyIndex, idx);

		impl_->queues[i].reset(new Queue(queue, impl_->qFamilyProperties[queueInfo.queueFamilyIndex],
			queueInfo.queueFamilyIndex, idx));
	}

	tlStorage(); //automatically provide storage for this thread
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

Range<std::unique_ptr<Queue>> Device::queues() const
{
	return {impl_->queues};
}

const Queue* Device::queue(std::uint32_t family) const
{
	for(auto& queue : queues())
		if(queue->family() == family) return queue.get();

	return nullptr;
}

const Queue* Device::queue(std::uint32_t family, std::uint32_t id) const
{
	for(auto& queue : queues())
		if(queue->family() == family && queue->id() == id) return queue.get();

	return nullptr;
}

const Queue* Device::queue(vk::QueueFlags flags) const
{
	for(auto& queue : queues())
		if(queue->properties().queueFlags & flags) return queue.get();

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

DeviceMemoryAllocator& Device::deviceAllocator() const
{
	auto& storage = tlStorage();
	return storage.deviceAllocator;
}

std::pmr::memory_resource& Device::hostMemoryResource() const
{
	auto& storage = tlStorage();
	return storage.memoryResource;
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

Device::TLStorage& Device::tlStorage() const
{
	auto threadid = std::this_thread::get_id();
	std::lock_guard<std::mutex>(impl_->storageMutex); //only lock shared mutex in c++17

	auto it = impl_->tlStorage.find(threadid);
	if(it == impl_->tlStorage.end())
	{
		it = impl_->tlStorage.emplace(std::piecewise_construct,
			std::forward_as_tuple(threadid), std::forward_as_tuple(*this)).first;
	}

	return it->second;
}

std::vector<CommandPool>& Device::tlCommandPools() const
{
	auto& storage = tlStorage();
	return storage.commandPools;
}

}
