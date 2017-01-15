// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#include <vpp/buffer.hpp>
#include <vpp/vk.hpp>
#include <utility> // std::move

namespace vpp {

Buffer::Buffer(const Device& dev, vk::Buffer buffer, vk::BufferUsageFlags usage,
	unsigned int memoryTypeBits)
{
	handle_ = buffer;
	auto reqs = vk::getBufferMemoryRequirements(dev, vkHandle());

	reqs.memoryTypeBits &= memoryTypeBits;
	dev.deviceAllocator().request(vkHandle(), reqs, usage, memoryEntry_);
}

Buffer::Buffer(const Device& dev, const vk::BufferCreateInfo& info, unsigned int memoryTypeBits)
{
	handle_ = vk::createBuffer(dev, info);
	auto reqs = vk::getBufferMemoryRequirements(dev, vkHandle());

	reqs.memoryTypeBits &= memoryTypeBits;
	dev.deviceAllocator().request(vkHandle(), reqs, info.usage, memoryEntry_);
}

Buffer::Buffer(vk::Buffer buffer, MemoryEntry&& entry)
{
	handle_ = buffer;
	memoryEntry_ = std::move(entry);
}

Buffer::~Buffer()
{
	if(vkHandle()) vk::destroyBuffer(device(), vkHandle());
}

} // namespace vpp
