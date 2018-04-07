// Copyright (c) 2016-2018 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#include <vpp/device.hpp>
#include <vpp/vk.hpp>
#include <vpp/queue.hpp>
#include <vpp/commandBuffer.hpp>
#include <vpp/submit.hpp>
#include <vpp/sharedBuffer.hpp>
#include <vpp/physicalDevice.hpp>
#include <vpp/trackedDescriptor.hpp>
#include <vpp/util/threadStorage.hpp>

#include <map> // std::map
#include <vector> // std::map
#include <utility> // std::pair

namespace vpp {

// order here is important
struct Device::Impl {
	vk::PhysicalDeviceProperties physicalDeviceProperties;
	vk::PhysicalDeviceMemoryProperties memoryProperties;

	std::vector<vk::QueueFamilyProperties> qFamilyProperties;
	std::vector<std::unique_ptr<Queue, Device::QueueDeleter>> queues;
	std::vector<const Queue*> queuesVec; // cache vector for queues() function
	std::shared_mutex sharedQueueMutex;

	DynamicThreadStorage tls;
	unsigned int tlsDeviceAllocatorID {};
	unsigned int tlsBufferAllocatorID {};
	unsigned int tlsQueueSubmitterID {};
	unsigned int tlsCommandAllocatorID {};
	unsigned int tlsDescriptorAllocatorID {};
};

// used so the Queue destructor can be made not public and Device a friend.
// The Device class is the only one that creates/destroys/owns queues.
struct Device::QueueDeleter {
	void operator()(Queue* q) { delete q; }
};

// Device
Device::Device(vk::Instance ini, vk::PhysicalDevice phdev, const vk::DeviceCreateInfo& info)
	: instance_(ini), physicalDevice_(phdev)
{
	// (void) vk::getPhysicalDeviceQueueFamilyProperties(phdev);
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

Device::Device(vk::Instance ini, vk::PhysicalDevice phdev,
	nytl::Span<const char* const> extensions) : instance_(ini), physicalDevice_(phdev)
{
	if(!phdev) {
		auto phdevs = vk::enumeratePhysicalDevices(ini);
		physicalDevice_ = choose(phdevs);
	}

	if(!physicalDevice_) {
		throw std::runtime_error("vpp::Device: could not find physical device");
	}

	// query gfx compute queue
	auto queueFlags = vk::QueueBits::compute | vk::QueueBits::graphics;
	int gfxCompQueueFam = findQueueFamily(vkPhysicalDevice(), queueFlags);
	if(gfxCompQueueFam == -1) {
		throw std::runtime_error("vpp::Device: unable to find gfx/comp queue family");
	}

	// init device and queue create info
	float priorities[1] = {0.0};

	vk::DeviceCreateInfo devInfo;
	vk::DeviceQueueCreateInfo queueInfo({}, gfxCompQueueFam, 1, priorities);

	devInfo.enabledExtensionCount = extensions.size();
	devInfo.ppEnabledExtensionNames = extensions.data();
	devInfo.pQueueCreateInfos = &queueInfo;
	devInfo.queueCreateInfoCount = 1;

	// create the device
	device_ = vk::createDevice(vkPhysicalDevice(), devInfo);
	if(!device_) {
		throw std::runtime_error("vpp::Device: device creation failed");
	}

	// retrieve the queues and init the device
	init({{vk::getDeviceQueue(vkDevice(), gfxCompQueueFam, 0), gfxCompQueueFam}});
}

Device::Device(vk::Instance ini, vk::SurfaceKHR surface, const Queue*& present,
	nytl::Span<const char* const> extensions) : instance_(ini)
{
	// find a physical device
	auto phdevs = vk::enumeratePhysicalDevices(vkInstance());
	auto phdev = choose(phdevs, vkInstance(), surface);
	if(!phdev) {
		throw std::runtime_error("vpp::Device: could not find a valid physical device");
	}

	physicalDevice_ = phdev;

	// find present, graphics and compute queue
	// if there is a queue family for all 3, create only one queue for the family
	// otherwise just find a compute/gfx queue family
	int presentQueueFam = -1;
	int gfxCompQueueFam = -1;

	auto gfxCompFlags = vk::QueueBits::graphics | vk::QueueBits::compute;
	auto allRound = findQueueFamily(phdev, vkInstance(), surface, gfxCompFlags);

	if(allRound != -1) {
		presentQueueFam = allRound;
		gfxCompQueueFam = allRound;
	} else {
		presentQueueFam = findQueueFamily(phdev, vkInstance(), surface);
		gfxCompQueueFam = findQueueFamily(phdev, gfxCompFlags);
	}

	if(presentQueueFam == -1 || gfxCompQueueFam == -1)
		throw std::runtime_error("vpp::Device: could not find matching queue families");

	// setup device create info
	vk::DeviceCreateInfo devInfo;
	devInfo.queueCreateInfoCount = 1;

	// setup queue create infos
	vk::DeviceQueueCreateInfo queueInfos[2];
	float priorities[1] = {0.0};

	queueInfos[0].queueFamilyIndex = presentQueueFam;
	queueInfos[0].queueCount = 1;
	queueInfos[0].pQueuePriorities = priorities;

	if(gfxCompQueueFam != presentQueueFam) {
		devInfo.queueCreateInfoCount = 2;
		queueInfos[1].queueFamilyIndex = gfxCompQueueFam;
		queueInfos[1].queueCount = 1;
		queueInfos[1].pQueuePriorities = priorities;
	}

	// automatically add the swapchain extension
	std::vector<const char*> exts;
	exts.reserve(extensions.size() + 1);

	exts.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
	for(auto ext : extensions) exts.push_back(ext);

	devInfo.enabledExtensionCount = exts.size();
	devInfo.ppEnabledExtensionNames = exts.data();

	devInfo.pQueueCreateInfos = queueInfos;

	device_ = vk::createDevice(vkPhysicalDevice(), devInfo);
	if(!device_) {
		throw std::runtime_error("vpp::Device: device creation failed");
	}

	// retrieve the queues and init the device
	std::vector<std::pair<vk::Queue, unsigned int>> queuePairs;
	auto presentQueue = vk::getDeviceQueue(vkDevice(), presentQueueFam, 0);

	queuePairs.emplace_back(presentQueue, presentQueueFam);
	if(devInfo.queueCreateInfoCount == 2) {
		auto gfxCompQueue = vk::getDeviceQueue(vkDevice(), presentQueueFam, 0);
		queuePairs.emplace_back(gfxCompQueue, presentQueueFam);
	}

	init(queuePairs);

	// set the present queue output parameter
	present = queue(presentQueueFam);
}

Device::~Device()
{
	// make sure there are no pending command buffers and stuff
	vk::deviceWaitIdle(*this);

	// the fact that we have to do this probably shows that
	// the whole thread-specific stuff isn't the best design but
	// anyways... make sure we have a sane destruction order.
	threadStorage().remove(impl_->tlsBufferAllocatorID);

	// it is important to first reset the stored objects that
	// depend on the vulkan device to be valid before actually destroying the
	// vulkan device.
	// When adding additional members that depend on the vulkan device, make
	// sure to destroy them here before calling vk::destroyDevice
	impl_.reset();

	if(vkDevice()) {
		vk::destroyDevice(device_, nullptr);
	}
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
		impl_->queues[i].reset(new Queue(*this, queues[i].first, id, queues[i].second));
		impl_->queuesVec[i] = impl_->queues[i].get();
	}

	// init thread local storage and providers
	impl_->tlsDeviceAllocatorID = impl_->tls.add();
	impl_->tlsBufferAllocatorID = impl_->tls.add();
	impl_->tlsQueueSubmitterID = impl_->tls.add();
	impl_->tlsCommandAllocatorID = impl_->tls.add();
	impl_->tlsDescriptorAllocatorID = impl_->tls.add();
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

const vk::QueueFamilyProperties& Device::queueFamilyProperties(unsigned int qFamily) const
{
	return impl_->qFamilyProperties[qFamily];
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

unsigned int Device::hostMemoryTypes(unsigned int typeBits) const {
	return memoryTypeBits(vk::MemoryPropertyBits::hostVisible, typeBits);
}

unsigned int Device::deviceMemoryTypes(unsigned int typeBits) const {
	return memoryTypeBits(vk::MemoryPropertyBits::deviceLocal, typeBits);
}

BufferAllocator& Device::bufferAllocator() const
{
	auto ptr = impl_->tls.get(impl_->tlsBufferAllocatorID); // DynamicStoragePtr*
	if(!ptr->get()) {
		auto storage = new ValueStorage<BufferAllocator>(*this);
		ptr->reset(storage);
		return storage->value;
	}

	return static_cast<ValueStorage<BufferAllocator>*>(ptr->get())->value;
}

DeviceMemoryAllocator& Device::deviceAllocator() const
{
	auto ptr = impl_->tls.get(impl_->tlsDeviceAllocatorID); // DynamicStoragePtr*
	if(!ptr->get()) {
		auto storage = new ValueStorage<DeviceMemoryAllocator>(*this);
		ptr->reset(storage);
		return storage->value;
	}

	return static_cast<ValueStorage<DeviceMemoryAllocator>*>(ptr->get())->value;
}

CommandAllocator& Device::commandAllocator() const
{
	auto ptr = impl_->tls.get(impl_->tlsCommandAllocatorID); // DynamicStoragePtr*
	if(!ptr->get()) {
		auto storage = new ValueStorage<CommandAllocator>(*this);
		ptr->reset(storage);
		return storage->value;
	}

	return static_cast<ValueStorage<CommandAllocator>*>(ptr->get())->value;
}

DescriptorAllocator& Device::descriptorAllocator() const
{
	auto ptr = impl_->tls.get(impl_->tlsDescriptorAllocatorID); // DynamicStoragePtr*
	if(!ptr->get()) {
		auto storage = new ValueStorage<DescriptorAllocator>(*this);
		ptr->reset(storage);
		return storage->value;
	}

	return static_cast<ValueStorage<DescriptorAllocator>*>(ptr->get())->value;
}

QueueSubmitter& Device::queueSubmitter() const
{
	auto ptr = impl_->tls.get(impl_->tlsQueueSubmitterID); // DynamicStoragePtr*
	if(!ptr->get()) {
		auto storage = new ValueStorage<QueueSubmitter>(*queue(vk::QueueBits::graphics));
		ptr->reset(storage);
		return storage->value;
	}

	return static_cast<ValueStorage<QueueSubmitter>*>(ptr->get())->value;
}

DynamicThreadStorage& Device::threadStorage() const
{
	return impl_->tls;
}

std::shared_mutex& Device::sharedQueueMutex() const
{
	return impl_->sharedQueueMutex;
}

} // namespace vpp
