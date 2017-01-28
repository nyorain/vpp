// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#include <vpp/memoryMap.hpp>
#include <vpp/memory.hpp>
#include <vpp/vk.hpp>
#include <vpp/util/debug.hpp>

namespace vpp {

// MemoryMap
MemoryMap::MemoryMap(const DeviceMemory& memory, const Allocation& alloc)
	: memory_(&memory), allocation_(alloc)
{
	VPP_DEBUG_CHECK("vpp::MemoryMap", {
		if(!(memory.properties() & vk::MemoryPropertyBits::hostVisible))
			VPP_CHECK_THROW("trying to map unmappable memory");
	})

	ptr_ = vk::mapMemory(vkDevice(), vkMemory(), offset(), size(), {});
}

MemoryMap::~MemoryMap()
{
	try {
		unmap();
	} catch(const std::exception& error) {
		warn("vpp::~MemoryMap: unmap(): ", error.what());
	}
}

void MemoryMap::remap(const Allocation& allocation)
{
	// new allocation extent
	auto nbeg = std::min(allocation.offset, allocation_.offset);
	auto nsize = std::max(allocation.end(), allocation_.end()) - nbeg;

	// if new extent lay inside old do nothing
	if(offset() <= nbeg && offset() + size() >= nbeg + nsize) return;

	// else remap the memory
	vk::unmapMemory(vkDevice(), vkMemory());
	allocation_ = {nbeg, nsize};

	ptr_ = vk::mapMemory(vkDevice(), vkMemory(), offset(), size(), {});
}

void MemoryMap::swap(MemoryMap& lhs) noexcept
{
	using std::swap;

	swap(resourceBase(), lhs.resourceBase());
	swap(memory_, lhs.memory_);
	swap(allocation_, lhs.allocation_);
	swap(ptr_, lhs.ptr_);
	swap(views_, lhs.views_);
}

vk::MappedMemoryRange MemoryMap::mappedMemoryRange() const noexcept
{
	return {vkMemory(), offset(), size()};
}

const vk::DeviceMemory& MemoryMap::vkMemory() const noexcept
{
	return memory_->vkHandle();
}

void MemoryMap::flush() const
{
	VPP_DEBUG_CHECK("vpp::MemoryMap::flush", {
		if(coherent()) VPP_CHECK_WARN("Called on coherent memory. Not needed.");
	})

	auto range = mappedMemoryRange();
	vk::flushMappedMemoryRanges(vkDevice(), 1, range);
}

void MemoryMap::reload() const
{
	VPP_DEBUG_CHECK("vpp::MemoryMap::reload", {
		if(coherent()) VPP_CHECK_WARN("Called on coherent memory. Not needed.");
	})

	auto range = mappedMemoryRange();
	vk::invalidateMappedMemoryRanges(vkDevice(), 1, range);
}

bool MemoryMap::coherent() const noexcept
{
	return memory().properties() & vk::MemoryPropertyBits::hostCoherent;
}

void MemoryMap::unmap()
{
	VPP_DEBUG_CHECK("vpp::MemoryMap::ummap", {
		if(views_ > 0) VPP_CHECK_THROW("there are still views for this map");
	})

	if(memory_ && vkMemory() && ptr() && size())
		vk::unmapMemory(memory().vkDevice(), vkMemory());

	memory_ = nullptr;
	allocation_ = {};
	ptr_ = nullptr;
	views_ = 0;
}

void MemoryMap::ref() noexcept
{
	views_++;
}

void MemoryMap::unref() noexcept
{
	VPP_DEBUG_CHECK("vpp::MemoryMap::unref", {
		if(views_ == 0) VPP_CHECK_WARN("refcount already zero");
	})

	views_--;
	if(views_ == 0) {
		try {
			unmap();
		} catch(const std::exception& error) {
			warn("vpp::MemoryMap::unref: unmap: ", error.what());
		}
	}
}

// MemoryMapView
MemoryMapView::MemoryMapView(MemoryMap& map, const Allocation& allocation)
	: memoryMap_(&map), allocation_(allocation)
{
	VPP_DEBUG_CHECK("vpp::MemoryMap", {
		if(allocation.size == 0) VPP_CHECK_THROW("invalid allocatoin");
	})

	memoryMap_->ref();
}

MemoryMapView::~MemoryMapView()
{
	if(memoryMap_)
		memoryMap_->unref();
}

void MemoryMapView::swap(MemoryMapView& lhs) noexcept
{
	using std::swap;

	swap(memoryMap_, lhs.memoryMap_);
	swap(allocation_, lhs.allocation_);
}

vk::MappedMemoryRange MemoryMapView::mappedMemoryRange() const noexcept
{
	return {vkMemory(), offset(), size()};
}

void MemoryMapView::flush() const
{
	VPP_DEBUG_CHECK("vpp::MemoryMapView::flush", {
		if(coherent()) VPP_CHECK_WARN("Called on coherent memory. Not needed.");
	})

	auto range = mappedMemoryRange();
	vk::flushMappedMemoryRanges(vkDevice(), 1, range);
}

void MemoryMapView::reload() const
{
	VPP_DEBUG_CHECK("vpp::MemoryMapView::reload", {
		if(coherent()) VPP_CHECK_WARN("Called on coherent memory. Not needed.");
	})

	auto range = mappedMemoryRange();
	vk::invalidateMappedMemoryRanges(vkDevice(), 1, range);
}

uint8_t* MemoryMapView::ptr() const noexcept
{
	return memoryMap().ptr() + allocation().offset - memoryMap().offset();
}

bool MemoryMapView::coherent() const noexcept
{
	return memory().properties() & vk::MemoryPropertyBits::hostCoherent;
}

} // namespace vpp
