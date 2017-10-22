// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#include <vpp/memoryMap.hpp>
#include <vpp/memory.hpp>
#include <vpp/vk.hpp>
#include <dlg/dlg.hpp>

namespace vpp {

// MemoryMap
MemoryMap::MemoryMap(const DeviceMemory& memory, const Allocation& alloc)
	: memory_(&memory), allocation_(alloc)
{
	dlg_assertm(memory.properties() & vk::MemoryPropertyBits::hostVisible,
		"Trying to map unmappable memory");
	ptr_ = vk::mapMemory(vkDevice(), vkMemory(), offset(), size(), {});
}

MemoryMap::~MemoryMap()
{
	try {
		unmap();
	} catch(const std::exception& error) {
		dlg_warnt(("~MemoryMap"), "unmap(): {}", error.what());
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

void swap(MemoryMap& a, MemoryMap& b) noexcept
{
	using std::swap;
	using RR = ResourceReference<MemoryMap>;

	swap(static_cast<RR&>(a), static_cast<RR&>(b));
	swap(a.memory_, b.memory_);
	swap(a.allocation_, b.allocation_);
	swap(a.ptr_, b.ptr_);
	swap(a.views_, b.views_);
}

vk::MappedMemoryRange MemoryMap::mappedMemoryRange() const noexcept
{
	return {vkMemory(), offset(), size()};
}

const vk::DeviceMemory& MemoryMap::vkMemory() const noexcept
{
	return memory_->vkHandle();
}

void MemoryMap::flush(const Allocation& range) const
{
	if(coherent()) {
		return;
	}

	auto o = range.offset + offset();
	auto s = range.size == vk::wholeSize ? size() : range.size;
	dlg_assert(s <= size());

	vk::flushMappedMemoryRanges(vkDevice(), 1, {vkMemory(), o, s});
}

void MemoryMap::invalidate(const Allocation& range) const
{
	if(coherent()) {
		return;
	}

	auto o = range.offset + offset();
	auto s = range.size == vk::wholeSize ? size() : range.size;
	dlg_assert(s <= size());

	vk::invalidateMappedMemoryRanges(vkDevice(), 1, {vkMemory(), o, s});
}

bool MemoryMap::coherent() const noexcept
{
	return memory().properties() & vk::MemoryPropertyBits::hostCoherent;
}

void MemoryMap::unmap()
{
	dlg_assertm(views_ == 0, "unmap: still views for this map");

	if(memory_ && vkMemory() && ptr() && size()) {
		vk::unmapMemory(memory().vkDevice(), vkMemory());
	}

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
	dlg_assertm(views_ > 0, "unref: refcount already zero");
	views_--;
	
	if(views_ == 0) {
		try {
			unmap();
		} catch(const std::exception& error) {
			dlg_warnt(("MemoryMap::unref"), "unmap: ", error.what());
		}
	}
}

// MemoryMapView
MemoryMapView::MemoryMapView(MemoryMap& map, const Allocation& allocation)
	: memoryMap_(&map), allocation_(allocation)
{
	dlg_assert(map.valid());
	dlg_assertm(allocation.size > 0, "MemoryMapView: invalid allocation");
	memoryMap_->ref();
}

MemoryMapView::~MemoryMapView()
{
	if(memoryMap_) {
		memoryMap_->unref();
	}
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
	if(coherent()) {
		return;
	}

	auto range = mappedMemoryRange();
	vk::flushMappedMemoryRanges(vkDevice(), 1, range);
}

void MemoryMapView::invalidate() const
{
	if(coherent()) {
		return;
	}
	
	auto range = mappedMemoryRange();
	vk::invalidateMappedMemoryRanges(vkDevice(), 1, range);
}

std::byte* MemoryMapView::ptr() const noexcept
{
	return memoryMap().ptr() + allocation().offset - memoryMap().offset();
}

bool MemoryMapView::coherent() const noexcept
{
	return memory().properties() & vk::MemoryPropertyBits::hostCoherent;
}

} // namespace vpp
