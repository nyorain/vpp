#include "init.hpp"

#include <vpp/allocator.hpp>
#include <vpp/commandBuffer.hpp>
#include <vpp/memory.hpp>
#include <vpp/sharedBuffer.hpp>
#include <vpp/sync.hpp>

// test just instanciates all vpp objects
// also used as bootstrap for all classes that currently have no own test

TEST(mem) {
	auto& dev = *globals.device;

	auto rawMem = vk::allocateMemory(dev, {2048, 0u});
	auto memHandle1 = vpp::DeviceMemoryHandle(dev, {256u, 0u});
	auto memHandle2 = vpp::DeviceMemoryHandle(dev, rawMem);

	auto rawMem2 = vk::allocateMemory(dev, {14u, 0u});
	auto mem1 = vpp::DeviceMemory(dev, {256u, 0u});
	auto mem2 = vpp::DeviceMemory(dev, rawMem2, 14u, 0u);

	auto allocator = vpp::DeviceMemoryAllocator(dev);
	auto memEntry = vpp::MemoryEntry(mem1, mem1.alloc(10u, 0u, vpp::AllocationType::linear));
}

TEST(cmd) {
	auto& dev = *globals.device;

	auto commandPool = vpp::CommandPool{dev, 0};
	auto cmdBuf1 = vpp::CommandBuffer(commandPool);
	auto cmdBuf2 = vpp::CommandBuffer(commandPool, vk::CommandBufferLevel::secondary);

	auto cmdBufsRaw = vk::allocateCommandBuffers(dev, {commandPool, 
		vk::CommandBufferLevel::primary, 2});
	auto cmdBuf3 = vpp::CommandBuffer(commandPool, cmdBufsRaw[0]);
	auto cmdBuf4 = vpp::CommandBuffer(dev, commandPool, cmdBufsRaw[1]);

	auto cmdAllocator = vpp::CommandAllocator(dev);
	auto cmdBuf5 = cmdAllocator.get(0);
	auto cmdBufs6 = cmdAllocator.get(0, 3);
}

TEST(sharedBuf) {
	auto& dev = *globals.device;
	vpp::SharedBuffer buf(dev, {{}, 1024u, vk::BufferUsageBits::uniformBuffer});
	auto alloc1 = buf.alloc(1000u);
	EXPECT(alloc1.offset, 0u);
	EXPECT(alloc1.size, 1000u);

	auto alloc2 = buf.alloc(3u);
	EXPECT(alloc2.size, 3u);

	auto alloc3 = buf.alloc(7u);
	EXPECT(alloc3.size, 7u);

	buf.free(alloc1);

	auto alloc4 = buf.alloc(231u);
	EXPECT(alloc4.size, 231u);

	buf.free(alloc2);
	buf.free(alloc4);

	auto alloc5 = buf.alloc(2100u);
	EXPECT(alloc5.size, 0u);
	buf.free(alloc3);

	vpp::BufferRange bufRange(buf, 100u);
	EXPECT(&bufRange.buffer(), &buf);
	EXPECT(bufRange.size(), 100u);
	EXPECT(bufRange.offset(), 0u);

	ERROR(vpp::BufferRange(buf, 1000u), std::runtime_error);

	// allocator
	vpp::BufferAllocator bufAlloc(dev);
	bufAlloc.reserve(1000u, vk::BufferUsageBits::uniformBuffer);
	bufAlloc.reserve(10000u, vk::BufferUsageBits::uniformBuffer);
	bufAlloc.reserve(7u, vk::BufferUsageBits::storageBuffer);
	bufAlloc.reserve(100u, vk::BufferUsageBits::indexBuffer);
	EXPECT(bufAlloc.buffers().size(), 0u);

	for(auto i = 0u; i < 100; ++i) {
		bufAlloc.alloc(11000u, vk::BufferUsageBits::uniformBuffer |
			vk::BufferUsageBits::storageBuffer);
		EXPECT(bufAlloc.buffers().size(), 1u);
	}
}

TEST(sync) {
	auto& dev = *globals.device;
	vpp::Fence fence(dev);
	vpp::Semaphore semaphore(dev);
	vpp::Event event(dev);
}

/*
TEST(other) {
	auto& dev = *globals.device;

	vk::FramebufferCreateInfo info;
	auto fb = Framebuffer(dev, info);
}
*/

// TODO: add remaining objects