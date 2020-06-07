// Copyright (c) 2016-2020 Jan Kelling
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#include <vpp/buffer.hpp>
#include <vpp/sharedBuffer.hpp>
#include <vpp/vk.hpp>
#include <dlg/dlg.hpp>

#include <utility> // std::move

namespace vpp {

// Buffer
Buffer::Buffer(DeviceMemoryAllocator& alloc, const vk::BufferCreateInfo& info,
	unsigned int memBits) :
		Buffer(alloc, vk::createBuffer(alloc.device(), info), memBits) {
}

Buffer::Buffer(DeviceMemoryAllocator& alloc, vk::Buffer buffer,
		unsigned int memBits) {
	InitData data;
	*this = {data, alloc, buffer, memBits};
	init(data);
}

Buffer::Buffer(DeviceMemory& mem, const vk::BufferCreateInfo& info) :
		BufferHandle(mem.device(), vk::createBuffer(mem.device(), info)) {
	auto reqs = vk::getBufferMemoryRequirements(mem.device(), vkHandle());
	dlg_assertm(reqs.memoryTypeBits & (1 << mem.type()), "Invalid memory type");
	auto alloc = mem.alloc(reqs.size, reqs.alignment, AllocationType::linear);
	if(alloc.size == 0) {
		throw std::runtime_error("Failed to alloc from memory");
	}

	MemoryResource::memory_ = &mem;
	MemoryResource::offset_ = alloc.offset;
}

Buffer::Buffer(DeviceMemory& mem, vk::Buffer buffer, vk::DeviceSize memOffset) :
		BufferHandle(mem.device(), buffer),
		MemoryResource(mem, memOffset) {
}

Buffer::Buffer(InitData& data, DeviceMemoryAllocator& alloc, vk::Buffer buffer,
		unsigned int memBits) : BufferHandle(alloc.device(), buffer) {
	dlg_assert(buffer);
	dlg_assert(memBits);

	auto reqs = vk::getBufferMemoryRequirements(alloc.device(), vkHandle());
	reqs.memoryTypeBits &= memBits;
	dlg_assertm(reqs.memoryTypeBits, "No memory type bits left");
	dlg_assert(reqs.size > 0);

	data.allocator = &alloc;
	data.reservation = data.allocator->reserve(AllocationType::linear, reqs);
}

Buffer::Buffer(InitData& data, DeviceMemoryAllocator& alloc,
	const vk::BufferCreateInfo& info, unsigned int memBits) :
		Buffer(data, alloc, vk::createBuffer(alloc.device(), info), memBits) {
}

void Buffer::init(InitData& data) {
	MemoryResource::init(data);
	vk::bindBufferMemory(device(), vkHandle(), memory(), memoryOffset());
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
