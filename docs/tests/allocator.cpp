#include "init.hpp"
#include "bugged.hpp"

#include <vpp/buffer.hpp>

TEST(memory) {
	auto& dev = *globals.device;
	auto& alloc = dev.deviceAllocator();
	EXPECT(alloc.memories().empty(), true);

	// allocate some stuff
	{
		vk::BufferCreateInfo bufInfo;
		bufInfo.size = 1024;
		bufInfo.usage = vk::BufferUsageBits::storageBuffer;
		auto buffer = vpp::Buffer(dev, bufInfo);
		vpp::Buffer buffer1(dev, bufInfo);
		vpp::Buffer buffer2(dev, bufInfo);
		vpp::Buffer buffer3(dev, bufInfo);
		vpp::Buffer buffer4(dev, bufInfo);
		EXPECT(alloc.memories().empty(), true);
		EXPECT(buffer1.memoryEntry().allocated(), false);
		EXPECT(buffer2.memoryEntry().allocator(), &alloc);

		buffer1.assureMemory();
		EXPECT(alloc.memories().size(), 1u);
		EXPECT(alloc.memories()[0] == nullptr, false);

		EXPECT(buffer2.memoryEntry().allocated(), true);
		EXPECT(buffer3.memoryEntry().allocated(), true);
		EXPECT(buffer3.memoryEntry().memory(), alloc.memories()[0]);

		EXPECT(buffer1.memorySize(), 1024u);
		EXPECT(buffer2.memorySize(), 1024u);
		EXPECT(buffer3.memorySize(), 1024u);
		EXPECT(buffer4.memorySize(), 1024u);
	}

	EXPECT(alloc.memories().size(), 1u);

	// XXX: might fail due to alignment requirements etc (allocater allocated larger memory)
	EXPECT(alloc.memories()[0]->totalFree(), 1024u * 5);

	// make sure same memory is used again
	{
		vk::BufferCreateInfo bufInfo;
		bufInfo.size = 1024;
		bufInfo.usage = vk::BufferUsageBits::storageBuffer;
		auto buffer = vpp::Buffer(dev, bufInfo);
		vpp::Buffer buffer1(dev, bufInfo);
		buffer1.memoryEntry().allocate();

		EXPECT(alloc.memories().size(), 1u);
		EXPECT(alloc.memories()[0]->totalFree(), 1024u * 4);
	}
}
