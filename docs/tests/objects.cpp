#include "init.hpp"

#include <vpp/allocator.hpp>
#include <vpp/commandBuffer.hpp>
#include <vpp/memory.hpp>
#include <vpp/sharedBuffer.hpp>
#include <vpp/sync.hpp>
#include <vpp/formats.hpp>
#include <vpp/framebuffer.hpp>
#include <vpp/image.hpp>
#include <vpp/renderPass.hpp>
#include <vpp/vpp.hpp>

// test just instanciates all vpp objects
// also used as bootstrap for all classes that currently have no own test

TEST(mem) {
	auto& dev = *globals.device;

	auto rawMem = vk::allocateMemory(dev, {2048, 0u});
	auto memHandle1 = vpp::DeviceMemoryHandle(dev, {256u, 0u});
	auto memHandle2 = vpp::DeviceMemoryHandle(dev, rawMem);
	auto memHandle3 = std::move(memHandle2);

	auto rawMem2 = vk::allocateMemory(dev, {14u, 0u});
	auto mem1 = vpp::DeviceMemory(dev, {256u, 0u});
	auto mem2 = vpp::DeviceMemory(dev, rawMem2, 14u, 0u);

	auto allocator = vpp::DeviceMemoryAllocator(dev);
	auto memEntry = vpp::MemoryEntry(mem1, mem1.alloc(10u, 0u, vpp::AllocationType::linear));
	auto memEntry2 = std::move(memEntry);
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
	auto cmdBuf7 = std::move(cmdBuf5);
}

// returns the index of the first set bit of an uint
// returns -1 if no bit is set
int firstBitSet(uint32_t bits)
{
	for(auto i = 0u; i < 32; ++i) {
		if(bits & (1 << i)) {
			return i;
		}
	}

	return -1;
}

TEST(buf) {
	auto& dev = *globals.device;
	vpp::BufferHandle buf1(dev, {{}, 1024, vk::BufferUsageBits::uniformBuffer});

	auto rawBuf = vk::createBuffer(dev, {{}, 2341, vk::BufferUsageBits::vertexBuffer});
	auto rawBuf2 = vk::createBuffer(dev, {{}, 12, vk::BufferUsageBits::vertexBuffer});
	vpp::BufferHandle buf2(dev, rawBuf);

	// None of this should actually destroy we buffer, we use it below again
	vpp::NonOwned<vpp::BufferHandle> buf2b(dev, rawBuf2);
	vpp::NonOwned<vpp::BufferHandle> buf2c(std::move(buf2b));
	buf2c = {};

	auto hvBits = dev.memoryTypeBits(vk::MemoryPropertyBits::hostVisible);
	auto dlBits = dev.memoryTypeBits(vk::MemoryPropertyBits::deviceLocal);

	auto info = vk::BufferCreateInfo {{}, 123,
		vk::BufferUsageBits::storageBuffer |
		vk::BufferUsageBits::indexBuffer |
		vk::BufferUsageBits::transferDst |
		vk::BufferUsageBits::uniformTexelBuffer};
	vpp::Buffer buf3(dev, info);
	vpp::Buffer buf4(dev, info, hvBits);

	vpp::DeviceMemoryAllocator allocator(dev);
	vpp::Buffer buf5(vpp::defer, dev, info, dlBits, &allocator);
	auto rawBuf3b = vk::createBuffer(dev, info);
	vpp::Buffer buf5b(vpp::defer, dev, rawBuf3b, dlBits, &allocator);
	vpp::Buffer buf6(dev, info, dlBits, &allocator);
	buf5.init();
	EXPECT(allocator.memories().size(), 1u);

	vpp::Buffer buf7(dev, rawBuf2);

	auto rawBuf3 = vk::createBuffer(dev, {{}, 123, vk::BufferUsageBits::uniformBuffer});
	auto reqs = vk::getBufferMemoryRequirements(dev, rawBuf3);
	unsigned int type = firstBitSet(reqs.memoryTypeBits);
	vpp::DeviceMemory mem(dev, {12000, type});
	auto memAlloc = mem.alloc(reqs.size, reqs.alignment, vpp::AllocationType::linear);
	vk::bindBufferMemory(dev, rawBuf3, mem, memAlloc.offset);
	vpp::MemoryEntry entry(mem, memAlloc);
	vpp::Buffer buf8(dev, rawBuf3, std::move(entry));

	vpp::Buffer buf9(dev, info, mem);
	auto buf10 = std::move(buf9);
}

TEST(sync) {
	auto& dev = *globals.device;
	vpp::Fence fence(dev);
	vpp::Semaphore semaphore(dev);
	vpp::Event event(dev);
}

// TODO: add remaining objects
