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
		auto buffer = vpp::Buffer(vpp::defer, dev, bufInfo);
		vpp::Buffer buffer1(vpp::defer, dev, bufInfo);
		vpp::Buffer buffer2(vpp::defer, dev, bufInfo);
		vpp::Buffer buffer3(vpp::defer, dev, bufInfo);
		vpp::Buffer buffer4(vpp::defer, dev, bufInfo);
		EXPECT(alloc.memories().empty(), true);
		EXPECT(buffer1.memoryEntry().allocated(), false);
		EXPECT(buffer2.memoryEntry().allocator(), &alloc);

		buffer1.init();
		EXPECT(alloc.memories().size(), 1u);

		EXPECT(buffer2.memoryEntry().allocated(), true);
		EXPECT(buffer3.memoryEntry().allocated(), true);
		EXPECT(buffer3.memoryEntry().memory(), &alloc.memories()[0]);

		EXPECT(buffer1.memorySize(), 1024u);
		EXPECT(buffer2.memorySize(), 1024u);
		EXPECT(buffer3.memorySize(), 1024u);
		EXPECT(buffer4.memorySize(), 1024u);
	}

	EXPECT(alloc.memories().size(), 1u);

	// XXX: might fail due to alignment requirements etc (allocater 
	// allocated larger memory)
	EXPECT(alloc.memories()[0].totalFree(), 1024u * 5);

	// make sure same memory is used again
	{
		vk::BufferCreateInfo bufInfo;
		bufInfo.size = 1024;
		bufInfo.usage = vk::BufferUsageBits::storageBuffer;
		auto buffer = vpp::Buffer(dev, bufInfo);
		EXPECT(alloc.memories().size(), 1u);
		EXPECT(alloc.memories()[0].totalFree(), 1024u * 4);

		vpp::Buffer buffer1(vpp::defer, dev, bufInfo);
		EXPECT(alloc.memories()[0].totalFree(), 1024u * 4);

		buffer1.init();
		EXPECT(alloc.memories()[0].totalFree(), 1024u * 3);
	}
}


TEST(custom) {
	auto& dev = *globals.device;
	auto alloc = vpp::DeviceMemoryAllocator(dev);
	EXPECT(alloc.memories().empty(), true);

	// allocate some stuff
	{
		vk::BufferCreateInfo bufInfo;
		bufInfo.size = 1024;
		bufInfo.usage = vk::BufferUsageBits::storageBuffer;
		auto buffer = vpp::Buffer(vpp::defer, dev, bufInfo, ~0u, &alloc);
		vpp::Buffer buffer1(vpp::defer, dev, bufInfo, ~0u, &alloc);
		vpp::Buffer buffer2(vpp::defer, dev, bufInfo, ~0u, &alloc);
		vpp::Buffer buffer3(vpp::defer, dev, bufInfo, ~0u, &alloc);
		EXPECT(buffer3.memoryEntry().allocator(), &alloc);

		auto alloc2 = std::move(alloc);

		vpp::Buffer buffer4(vpp::defer, dev, bufInfo, ~0u, &alloc2);
		EXPECT(alloc2.memories().empty(), true);
		EXPECT(buffer1.memoryEntry().allocated(), false);
		EXPECT(buffer2.memoryEntry().allocator(), &alloc2);
		EXPECT(buffer3.memoryEntry().allocator(), &alloc2);

		alloc = std::move(alloc2);
		alloc.allocate();
		EXPECT(alloc.memories().size(), 1u);

		EXPECT(buffer2.memoryEntry().allocated(), true);
		EXPECT(buffer3.memoryEntry().allocated(), true);
		EXPECT(buffer3.memoryEntry().memory(), &alloc.memories()[0]);

		EXPECT(buffer1.memorySize(), 1024u);
		EXPECT(buffer2.memorySize(), 1024u);
		EXPECT(buffer3.memorySize(), 1024u);
		EXPECT(buffer4.memorySize(), 1024u);
	}

	EXPECT(alloc.memories().size(), 1u);

	// XXX: might fail due to alignment requirements etc (allocater 
	// allocated larger memory)
	EXPECT(alloc.memories()[0].totalFree(), 1024u * 5);

	// make sure same memory is used again
	{
		vk::BufferCreateInfo bufInfo;
		bufInfo.size = 1024;
		bufInfo.usage = vk::BufferUsageBits::storageBuffer;
		auto buffer = vpp::Buffer(dev, bufInfo, ~0u, &alloc);
		EXPECT(alloc.memories().size(), 1u);
		EXPECT(alloc.memories()[0].totalFree(), 1024u * 4);

		vpp::Buffer buffer1(vpp::defer, dev, bufInfo, ~0u, &alloc);
		EXPECT(alloc.memories()[0].totalFree(), 1024u * 4);

		auto alloc2 = std::move(alloc);
		buffer1.init();
		EXPECT(alloc2.memories()[0].totalFree(), 1024u * 3);

		alloc = std::move(alloc2);
	}
}