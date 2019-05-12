#include "init.hpp"
#include "bugged.hpp"

#include <vpp/buffer.hpp>

TEST(memory) {
	auto& dev = *globals.device;
	auto& alloc = dev.deviceAllocator();
	EXPECT(alloc.memories().empty(), true);

	// allocate some stuff
	{
		std::array<vpp::Buffer::InitData, 5> initData;

		vk::BufferCreateInfo bufInfo;
		bufInfo.size = 1024;
		bufInfo.usage = vk::BufferUsageBits::storageBuffer;
		auto buffer = vpp::Buffer(initData[0], dev, bufInfo);
		vpp::Buffer buffer1(initData[1], dev, bufInfo);
		vpp::Buffer buffer2(initData[2], dev, bufInfo);
		vpp::Buffer buffer3(initData[3], dev, bufInfo);
		vpp::Buffer buffer4(initData[4], dev, bufInfo);
		EXPECT(alloc.memories().empty(), true);

		buffer.init(initData[0]);
		buffer1.init(initData[1]);
		buffer2.init(initData[2]);
		buffer3.init(initData[3]);
		buffer4.init(initData[4]);

		EXPECT(alloc.memories().size(), 1u);

		EXPECT(&buffer1.memory(), &alloc.memories()[0]);
		EXPECT(&buffer2.memory(), &alloc.memories()[0]);
		EXPECT(&buffer3.memory(), &alloc.memories()[0]);
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

		vpp::Buffer::InitData initData;
		vpp::Buffer buffer1(initData, dev, bufInfo);
		EXPECT(alloc.memories()[0].totalFree(), 1024u * 4);

		buffer1.init(initData);
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

		std::array<vpp::Buffer::InitData, 5u> initData;
		auto buffer = vpp::Buffer(initData[0], dev, bufInfo, ~0u, &alloc);
		vpp::Buffer buffer1(initData[1], dev, bufInfo, ~0u, &alloc);
		vpp::Buffer buffer2(initData[2], dev, bufInfo, ~0u, &alloc);
		vpp::Buffer buffer3(initData[3], dev, bufInfo, ~0u, &alloc);
		EXPECT(initData[3].allocator, &alloc);

		vpp::Buffer buffer4(initData[4], dev, bufInfo, ~0u, &alloc);
		EXPECT(alloc.memories().empty(), true);
		EXPECT(initData[4].allocator, &alloc);

		alloc.alloc();
		buffer.init(initData[0]);
		buffer1.init(initData[1]);
		buffer2.init(initData[2]);
		buffer3.init(initData[3]);
		buffer4.init(initData[4]);

		EXPECT(alloc.memories().size(), 1u);
		EXPECT(&buffer.memory(), &alloc.memories()[0]);
		EXPECT(&buffer1.memory(), &alloc.memories()[0]);
		EXPECT(&buffer2.memory(), &alloc.memories()[0]);
		EXPECT(&buffer3.memory(), &alloc.memories()[0]);
		EXPECT(&buffer4.memory(), &alloc.memories()[0]);
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

		vpp::Buffer::InitData data;
		vpp::Buffer buffer1(data, dev, bufInfo, ~0u, &alloc);
		EXPECT(alloc.memories()[0].totalFree(), 1024u * 4);

		buffer1.init(data);
		EXPECT(alloc.memories()[0].totalFree(), 1024u * 3);
	}
}

TEST(cancel) {
	auto& dev = *globals.device;
	vk::BufferCreateInfo bufInfo;
	bufInfo.size = 1024;
	bufInfo.usage = vk::BufferUsageBits::storageBuffer |
		vk::BufferUsageBits::uniformBuffer;

	vpp::Buffer::InitData data;
	vpp::Buffer buffer(data, dev, bufInfo);
	buffer = {}; // destroy, never finish initialization
}
