// Copyright (c) 2016-2019 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#include <vpp/memoryResource.hpp>
#include <dlg/dlg.hpp>

namespace vpp {

void MemoryResource::init(InitData& data) {
	dlg_assert(data.allocator && data.reservation);
	dlg_assert(!memory_);

	auto [mem, alloc] = data.allocator->alloc(data.reservation);
	memory_ = &mem;
	offset_ = alloc.offset;
	data.allocator = nullptr;
}

MemoryResource::~MemoryResource() {
	if(memory_) {
		memory_->free(offset_);
	}
}

MemoryResource::MemoryResource(DeviceMemory& mem, vk::DeviceSize offset) :
	memory_(&mem), offset_(offset) {
}

bool MemoryResource::mappable() const {
	return memory_ && memory_->mappable();
}

MemoryMapView MemoryResource::memoryMap(vk::DeviceSize offset,
		vk::DeviceSize size) const {
	dlg_assert(size);
	dlg_assert(memory_);
	return memory_->map({offset_ + offset, size});
}

void swap(MemoryResource& a, MemoryResource& b) {
	using std::swap;
	swap(a.memory_, b.memory_);
	swap(a.offset_, b.offset_);
}

// InitData
MemoryResource::InitData::InitData(InitData&& rhs) noexcept {
	allocator = rhs.allocator;
	reservation = rhs.reservation;
	rhs.allocator = {};
	rhs.reservation = {};
}

MemoryResource::InitData& MemoryResource::InitData::operator=(
		InitData&& rhs) noexcept {
	this->~InitData();

	allocator = rhs.allocator;
	reservation = rhs.reservation;
	rhs.allocator = {};
	rhs.reservation = {};
	return *this;
}

MemoryResource::InitData::~InitData() {
	if(allocator) {
		dlg_assert(reservation);
		allocator->cancel(reservation);
	}
}

} // namespace vpp
