#include "init.hpp"
#include "test.hpp"

TEST_METHOD("[memory-1]") {
	auto size = 1024u;
	vpp::DeviceMemory memory(*globals.device, size, vk::MemoryPropertyBits::hostVisible);

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

	EXPECT_ERROR(memory.alloc(allocSize, 32, vpp::AllocationType::linear), std::runtime_error);
	EXPECT_ERROR(memory.alloc(100, 1024, vpp::AllocationType::linear), std::runtime_error);
	EXPECT(memory.allocatable(5, 512, vpp::AllocationType::optimal).size, 0u);
	EXPECT(memory.allocatable(500, 0, vpp::AllocationType::optimal).size, 0u);

	allocSize = 424u;
	auto alloc2 = memory.allocatable(allocSize, 4, vpp::AllocationType::linear);
	EXPECT(alloc2.offset, 600u);
	EXPECT(alloc2.size, allocSize);

	auto check2 = memory.allocSpecified(alloc2.offset, alloc2.size, vpp::AllocationType::linear);
	EXPECT(check2.offset, alloc2.offset);
	EXPECT(check2.size, alloc2.size);

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
