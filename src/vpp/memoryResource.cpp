// Copyright (c) 2016-2018 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#include <vpp/memoryResource.hpp>

namespace vpp {

void MemoryResource::ensureMemory() {
	memoryEntry_.allocate();
}

bool MemoryResource::mappable() const {
	auto mem = memoryEntry_.memory();
	return mem && mem->mappable();
}

MemoryMapView MemoryResource::memoryMap(vk::DeviceSize offset,
		vk::DeviceSize size) const {
	return memoryEntry_.map(offset, size);
}

} // namespace vpp
