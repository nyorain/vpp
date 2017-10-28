#include "init.hpp"

TEST(memory) {
	auto size = 1024u;
	auto& dev = *globals.device;
	uint32_t type = dev.memoryType(vk::MemoryPropertyBits::hostVisible);
	vpp::DeviceMemory memory(dev, {size, type});

	EXPECT(memory.mappable(), true);
	EXPECT(memory.mapped(), nullptr);

	EXPECT(memory.size(), size);
	EXPECT(memory.largestFreeSegment(), size);
	EXPECT(memory.totalFree(), size);
	EXPECT(memory.allocations().empty(), true);

	auto allocSize = 600u;
	auto alloc1 = memory.alloc(allocSize, 16, vpp::AllocationType::linear);
	EXPECT(alloc1.offset, 0u);
	EXPECT(alloc1.size, allocSize);

	EXPECT(memory.largestFreeSegment(), size - allocSize);
	EXPECT(memory.totalFree(), size - allocSize);
	EXPECT(memory.allocations().size(), 1u);

	EXPECT(memory.alloc(allocSize, 32, vpp::AllocationType::linear).size, 0u);
	EXPECT(memory.alloc(100, 1024, vpp::AllocationType::linear).size, 0u);
	EXPECT(memory.allocatable(5, 512, vpp::AllocationType::optimal).size, 0u);
	EXPECT(memory.allocatable(500, 0, vpp::AllocationType::optimal).size, 0u);

	allocSize = 424u;
	auto alloc2 = memory.allocatable(allocSize, 4, vpp::AllocationType::linear);
	EXPECT(alloc2.offset, 600u);
	EXPECT(alloc2.size, allocSize);

	memory.allocSpecified({alloc2.offset, alloc2.size}, 
		vpp::AllocationType::linear);

	EXPECT(memory.largestFreeSegment(), 0u);
	EXPECT(memory.totalFree(), 0u);
	EXPECT(memory.allocations().size(), 2u);

	memory.free(alloc1);

	EXPECT(memory.largestFreeSegment(), size - allocSize);
	EXPECT(memory.totalFree(), size - allocSize);
	EXPECT(memory.allocations().size(), 1u);

	memory.free(alloc2);

	EXPECT(memory.largestFreeSegment(), size);
	EXPECT(memory.totalFree(), size);
	EXPECT(memory.allocations().empty(), true);
}

TEST(map) {
	auto size = 1024u;
	auto& dev = *globals.device;
	uint32_t type = dev.memoryType(vk::MemoryPropertyBits::hostVisible);
	vpp::DeviceMemory memory(dev, {size, type});
	
	EXPECT(memory.mappable(), true);
	auto map = memory.map({0u, 1024u});
	auto map2 = std::move(map);
	map = std::move(map2);

	EXPECT(map.offset(), 0u);
	EXPECT(map.size(), 1024u);
	EXPECT(&map.memory(), &memory);
	EXPECT(map.ptr() != nullptr, true);
	EXPECT(memory.mapped() != nullptr, true);

	// this will have no effect, just return another view
	map2 = memory.map({0, 256});
	EXPECT(map2.offset(), 0u);
	EXPECT(map2.size(), 256u);
	EXPECT(map2.ptr(), map.ptr());
	EXPECT(map2.memoryMap().valid(), true);

	// just another view
	auto map3 = memory.map({256, 256});
	EXPECT(map3.ptr(), map.ptr() + 256);

	map = std::move(map3);
}

TEST(remap) {
	auto size = 1024u;
	auto& dev = *globals.device;
	uint32_t type = dev.memoryType(vk::MemoryPropertyBits::hostVisible);
	vpp::DeviceMemory memory(dev, {size, type});
	
	EXPECT(memory.mappable(), true);
	auto map = memory.map({100u, 200});
	EXPECT(map.offset(), 100u);
	EXPECT(memory.mapped() != nullptr, true);
	EXPECT(memory.mapped()->valid(), true);
	EXPECT(memory.mapped()->offset(), 100u);
	EXPECT(memory.mapped()->size(), 200u);
	EXPECT(map.size(), 200u);
	EXPECT(&map.memory(), &memory);
	EXPECT(map.ptr() != nullptr, true);

	// remap the memory
	{
		auto map2 = memory.map({0u, 16u});
		EXPECT(map2.offset(), 0u);
		EXPECT(map2.size(), 16u);
		EXPECT(map2.memoryMap().valid(), true);
		EXPECT(memory.mapped(), &map2.memoryMap());
		EXPECT(memory.mapped()->valid(), true);
		EXPECT(memory.mapped()->offset(), 0u);
		EXPECT(memory.mapped()->size(), 300u);
		EXPECT(map2.ptr(), memory.mapped()->ptr());
		EXPECT(map.ptr(), memory.mapped()->ptr() + 100u);
	}

	EXPECT(map.ptr() != nullptr, true);
}