// Copyright (c) 2016-2018 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#include <vpp/buffer.hpp>
#include <vpp/vk.hpp>
#include <dlg/dlg.hpp>

#include <utility> // std::move

namespace vpp {

// BufferHandle
BufferHandle::BufferHandle(const Device& dev, const vk::BufferCreateInfo& info) :
	ResourceHandle(dev, vk::createBuffer(dev, info))
{
}

BufferHandle::BufferHandle(const Device& dev, vk::Buffer buf) :
	ResourceHandle(dev, buf)	
{
}

BufferHandle::~BufferHandle()
{
	if(vkHandle()) {
		vk::destroyBuffer(device(), vkHandle());
	}
}

// Buffer
Buffer::Buffer(const Device& dev, const vk::BufferCreateInfo& info,
	unsigned int memBits, vpp::DeviceMemoryAllocator* alloc) :
		Buffer(dev, vk::createBuffer(dev, info), info.usage, memBits, alloc)
{
}

Buffer::Buffer(const Device& dev, vk::Buffer buffer, vk::BufferUsageFlags usage,
	unsigned int memBits, vpp::DeviceMemoryAllocator* alloc) :
		Buffer(defer, dev, buffer, usage, memBits, alloc)
{
	ensureMemory();
}

Buffer::Buffer(const Device& dev, const vk::BufferCreateInfo& info, 
	DeviceMemory& mem) :
		BufferHandle(dev, vk::createBuffer(dev, info))
{
	auto reqs = vk::getBufferMemoryRequirements(dev, vkHandle());
	dlg_assertm(reqs.memoryTypeBits & (1 << mem.type()), "Invalid memory type");
	auto align = std::max(reqs.alignment, usageAlignment(dev, info.usage));
	auto alloc = mem.alloc(reqs.size, align, AllocationType::linear);
	if(alloc.size == 0) {
		throw std::runtime_error("Failed to alloc from memory");
	}

	memoryEntry_ = {mem, alloc};
}

Buffer::Buffer(const Device& dev, vk::Buffer buffer, MemoryEntry&& entry) : 
		BufferHandle(dev, buffer), MemoryResource(std::move(entry))
{
	dlg_assert(memoryEntry().allocated());
}

Buffer::Buffer(DeferTag, const Device& dev, vk::Buffer buffer, 
	vk::BufferUsageFlags usage, unsigned int memBits, 
	vpp::DeviceMemoryAllocator* alloc) :
		BufferHandle(dev, buffer)
{
	dlg_assert(buffer);
	dlg_assert(memBits);

	auto reqs = vk::getBufferMemoryRequirements(dev, vkHandle());
	reqs.memoryTypeBits &= memBits;
	dlg_assertm(reqs.memoryTypeBits, "No memory type bits left");
	dlg_assert(reqs.size > 0);

	auto& allocr = alloc ? *alloc : dev.deviceAllocator();
	allocr.request(vkHandle(), reqs, usage, memoryEntry_);
}

Buffer::Buffer(DeferTag, const Device& dev, const vk::BufferCreateInfo& info,
	unsigned int memBits, vpp::DeviceMemoryAllocator* alloc) :
		Buffer(defer, dev, vk::createBuffer(dev, info), info.usage, memBits, alloc)
{
}

// BufferView
BufferView::BufferView(const Device& dev, const vk::BufferViewCreateInfo& info) :
	BufferView(dev, vk::createBufferView(dev, info))
{
}

BufferView::BufferView(const Device& dev, vk::BufferView view) :
	ResourceHandle(dev, view)
{
}

BufferView::~BufferView()
{
	if(vkHandle()) {
		vk::destroyBufferView(device(), vkHandle());
	}
}

// util
vk::DeviceSize usageAlignment(const Device& dev, vk::BufferUsageFlags usage)
{
	vk::DeviceSize ret = 0u;

	auto align = dev.properties().limits.minUniformBufferOffsetAlignment;
	if(usage & vk::BufferUsageBits::uniformBuffer && align > 0) {
		ret = std::max(ret, align);
	}

	align = dev.properties().limits.minTexelBufferOffsetAlignment;
	if(usage & vk::BufferUsageBits::uniformTexelBuffer && align > 0) {
		ret = std::max(ret, align);
	}

	align = dev.properties().limits.minStorageBufferOffsetAlignment;
	if(usage & vk::BufferUsageBits::storageBuffer && align > 0) {
		ret = std::max(ret, align);
	}

	return ret;
}

} // namespace vpp
