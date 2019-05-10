// Copyright (c) 2016-2019 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#include <vpp/buffer.hpp>
#include <vpp/sharedBuffer.hpp>
#include <vpp/vk.hpp>
#include <dlg/dlg.hpp>

#include <utility> // std::move

namespace vpp {

// Buffer
Buffer::Buffer(const Device& dev, const vk::BufferCreateInfo& info,
	unsigned int memBits, vpp::DeviceMemoryAllocator* alloc) :
		Buffer(dev, vk::createBuffer(dev, info), memBits, alloc) {
}

Buffer::Buffer(const Device& dev, vk::Buffer buffer,
	unsigned int memBits, vpp::DeviceMemoryAllocator* alloc) :
		Buffer(defer, dev, buffer, memBits, alloc) {
	ensureMemory();
}

Buffer::Buffer(const Device& dev, const vk::BufferCreateInfo& info,
		DeviceMemory& mem) : BufferHandle(dev, vk::createBuffer(dev, info)) {
	auto reqs = vk::getBufferMemoryRequirements(dev, vkHandle());
	dlg_assertm(reqs.memoryTypeBits & (1 << mem.type()), "Invalid memory type");
	auto alloc = mem.alloc(reqs.size, reqs.alignment, AllocationType::linear);
	if(alloc.size == 0) {
		throw std::runtime_error("Failed to alloc from memory");
	}

	memoryEntry_ = {mem, alloc};
}

Buffer::Buffer(const Device& dev, vk::Buffer buffer, MemoryEntry&& entry) :
		BufferHandle(dev, buffer), MemoryResource(std::move(entry)) {
	dlg_assert(memoryEntry().allocated());
}

Buffer::Buffer(DeferTag, const Device& dev, vk::Buffer buffer,
	unsigned int memBits, vpp::DeviceMemoryAllocator* alloc) :
		BufferHandle(dev, buffer) {
	dlg_assert(buffer);
	dlg_assert(memBits);

	auto reqs = vk::getBufferMemoryRequirements(dev, vkHandle());
	reqs.memoryTypeBits &= memBits;
	dlg_assertm(reqs.memoryTypeBits, "No memory type bits left");
	dlg_assert(reqs.size > 0);

	auto& allocr = alloc ? *alloc : dev.deviceAllocator();
	allocr.request(vkHandle(), reqs, memoryEntry_);
}

Buffer::Buffer(DeferTag, const Device& dev, const vk::BufferCreateInfo& info,
	unsigned int memBits, vpp::DeviceMemoryAllocator* alloc) :
		Buffer(defer, dev, vk::createBuffer(dev, info), memBits, alloc) {
}

// BufferSpan
BufferSpan::BufferSpan(const SubBuffer& b) : allocation_(b.allocation()) {
	if(size()) {
		buffer_ = &b.buffer();
	}
}

BufferSpan::BufferSpan(const Buffer& b, vk::DeviceSize size,
		vk::DeviceSize offset) : buffer_(&b), allocation_{offset, size} {
	dlg_assert(size == 0 || b);
}

MemoryMapView BufferSpan::memoryMap() const {
	return buffer().memoryMap(offset(), size());
}

} // namespace vpp
