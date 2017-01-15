// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#include <vpp/device.hpp>
#include <vpp/vk.hpp>
#include <vpp/provider.hpp>
#include <vpp/queue.hpp>
#include <vpp/commandBuffer.hpp>
#include <vpp/submit.hpp>
#include <vpp/transfer.hpp>
#include <cstdlib>

namespace vpp {

struct Device::Impl {
	DynamicThreadLocalStorage tls;
	unsigned int tlsDeviceAllocatorID {};

	vk::PhysicalDeviceProperties physicalDeviceProperties;
	vk::PhysicalDeviceMemoryProperties memoryProperties;

	std::vector<vk::QueueFamilyProperties> qFamilyProperties;
	std::vector<std::unique_ptr<Queue, Device::QueueDeleter>> queues;
	std::vector<const Queue*> queuesVec;

};

struct Device::Provider {
	CommandProvider command;
	SubmitManager submit;
	TransferManager transfer;

	Provider(const Device& dev) : command(dev), submit(dev), transfer(dev) {}
};

// NOTE: used so the Queue destructor can be made not public and Device a friend.
// The Device class is the only one that creates/destroys/owns queues.
struct Device::QueueDeleter {
	void operator()(Queue* q) { delete q; }
};

// Device
Device::Device(vk::Instance ini, vk::PhysicalDevice phdev, const vk::DeviceCreateInfo& info)
	: instance_(ini), physicalDevice_(phdev)
{
	device_ = vk::createDevice(vkPhysicalDevice(), info);

	// we can assume that info.pQueueCreateInfo contains for every
	// family only one create info entry
	std::vector<std::pair<vk::Queue, unsigned int>> queuePairs;
	queuePairs.reserve(info.queueCreateInfoCount);

	for(auto q = 0u; q < info.queueCreateInfoCount; ++q) {
		auto fam = info.pQueueCreateInfos[q].queueFamilyIndex;
		for(auto i = 0u; i < info.pQueueCreateInfos[q].queueCount; ++i)
			queuePairs.push_back({vk::getDeviceQueue(vkDevice(), fam, i), fam});
	}

	init(queuePairs);
}

Device::Device(vk::Instance ini, vk::PhysicalDevice phdev, vk::Device device,
	nytl::Span<const std::pair<unsigned int, unsigned int>> queues)
		: instance_(ini), physicalDevice_(phdev), device_(device)
{
	std::vector<std::pair<vk::Queue, unsigned int>> queuePairs;
	queuePairs.reserve(queues.size());
	for(auto& q : queues) {
		auto queue = vk::getDeviceQueue(vkDevice(), q.first, q.second);
		queuePairs.push_back({queue, q.first});
	}

	init(queuePairs);
}

Device::Device(vk::Instance ini, vk::PhysicalDevice phdev, vk::Device device,
	nytl::Span<const std::pair<vk::Queue, unsigned int>> queues)
		: instance_(ini), physicalDevice_(phdev), device_(device)
{
	init(queues);
}

Device::~Device()
{
	provider_.reset();
	impl_.reset();

	if(vkDevice()) vk::destroyDevice(device_, nullptr);
}

void Device::init(nytl::Span<const std::pair<vk::Queue, unsigned int>> queues)
{
	// init impl and properties
	impl_ = std::make_unique<Impl>();
	impl_->physicalDeviceProperties = vk::getPhysicalDeviceProperties(vkPhysicalDevice());
	impl_->memoryProperties = vk::getPhysicalDeviceMemoryProperties(vkPhysicalDevice());
	impl_->qFamilyProperties = vk::getPhysicalDeviceQueueFamilyProperties(vkPhysicalDevice());

	// init queues
	impl_->queues.resize(queues.size());
	impl_->queuesVec.resize(queues.size());

	std::map<unsigned int, unsigned int> queueIds;
	for(std::size_t i(0); i < queues.size(); ++i) {
		auto id = queueIds[queues[i].second]++;
		const auto& props = impl_->qFamilyProperties[queues[i].second];

		impl_->queues[i].reset(new Queue(queues[i].first, props, id, queues[i].second));
		impl_->queuesVec[i] = impl_->queues[i].get();
	}

	// init thread local storage and providers
	impl_->tlsDeviceAllocatorID = impl_->tls.add();
	provider_ = std::make_unique<Provider>(*this);
}

void Device::release()
{
	impl_.reset();
	device_ = {};
}

nytl::Span<const Queue*> Device::queues() const
{
	return {impl_->queuesVec};
}

const Queue* Device::queue(unsigned int family) const
{
	for(auto& queue : queues())
		if(queue->family() == family) return queue;

	return nullptr;
}

const Queue* Device::queue(unsigned int family, unsigned int id) const
{
	for(auto& queue : queues())
		if(queue->family() == family && queue->id() == id) return queue;

	return nullptr;
}

const Queue* Device::queue(vk::QueueFlags flags) const
{
	for(auto& queue : queues())
		if(queue->properties().queueFlags & flags) return queue;

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

int Device::memoryType(vk::MemoryPropertyFlags mflags, unsigned int typeBits) const
{
	for(auto i = 0u; i < memoryProperties().memoryTypeCount; ++i) {
		if(typeBits & (1 << i)) {
			if((memoryProperties().memoryTypes[i].propertyFlags & mflags) == mflags)
				return i;
		}
	}

	return -1;
}

unsigned int Device::memoryTypeBits(vk::MemoryPropertyFlags mflags, unsigned int typeBits) const
{
	for(auto i = 0u; i < memoryProperties().memoryTypeCount; ++i) {
		if(typeBits & (1 << i)) {
			if((memoryProperties().memoryTypes[i].propertyFlags & mflags) != mflags)
				typeBits &= ~(1 << i);
		}
	}

	return typeBits;
}

DeviceMemoryAllocator& Device::deviceAllocator() const
{
	auto ptr = impl_->tls.get(impl_->tlsDeviceAllocatorID); // DynamicStoragePtr*
	if(!ptr->get()) {
		auto storage = new ValueStorage<DeviceMemoryAllocator>();
		storage->value = {*this};
		ptr->reset(storage);
		return storage->value;
	}

	return static_cast<ValueStorage<DeviceMemoryAllocator>*>(ptr->get())->value;
}

DynamicThreadLocalStorage& Device::threadLocalStorage() const
{
	return impl_->tls;
}

CommandProvider& Device::commandProvider() const
{
	return provider_->command;
}

SubmitManager& Device::submitManager() const
{
	return provider_->submit;
}

TransferManager& Device::transferManager() const
{
	return provider_->transfer;
}

} // namespace vpp
