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
	vpp::BufferHandle buf2b(dev, rawBuf2);
	buf2b.release();

	auto hvBits = dev.memoryTypeBits(vk::MemoryPropertyBits::hostVisible);
	auto dlBits = dev.memoryTypeBits(vk::MemoryPropertyBits::deviceLocal);

	auto info = vk::BufferCreateInfo {{}, 123, 
		vk::BufferUsageBits::storageBuffer |
		vk::BufferUsageBits::indexBuffer |
		vk::BufferUsageBits::uniformTexelBuffer};
	vpp::Buffer buf3(dev, info);
	vpp::Buffer buf4(dev, info, hvBits);

	vpp::DeviceMemoryAllocator allocator(dev);
	vpp::Buffer buf5(vpp::defer, dev, info, dlBits, &allocator);
	vpp::Buffer buf6(dev, info, dlBits, &allocator);
	buf5.init();
	EXPECT(allocator.memories().size(), 1u);

	vpp::Buffer buf7(dev, rawBuf2, vk::BufferUsageBits::vertexBuffer);

	auto rawBuf3 = vk::createBuffer(dev, {{}, 123, vk::BufferUsageBits::uniformBuffer});
	auto reqs = vk::getBufferMemoryRequirements(dev, rawBuf3);
	unsigned int type = firstBitSet(reqs.memoryTypeBits);
	vpp::DeviceMemory mem(dev, {1024, type});
	auto memAlloc = mem.alloc(reqs.size, reqs.alignment, vpp::AllocationType::linear);
	vk::bindBufferMemory(dev, rawBuf3, mem, memAlloc.offset);
	vpp::MemoryEntry entry(mem, memAlloc);
	vpp::Buffer buf8(dev, rawBuf3, std::move(entry));
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

// TODO: should get own test probably
TEST(framebuffer) {
	auto& dev = *globals.device;
	const auto size = vk::Extent3D {420, 693};

	using VICI = vpp::ViewableImageCreateInfo;
	auto colorUsage = vk::ImageUsageBits::colorAttachment;
	auto colorInfo = VICI::color(dev, size, colorUsage).value();
	auto depthInfo = VICI::depth(dev, size).value();
	vpp::ViewableImage color(dev, colorInfo);
	vpp::ViewableImage depth(dev, depthInfo);

	vk::AttachmentDescription attachments[2] {{{},
			colorInfo.img.format, vk::SampleCountBits::e1,
			vk::AttachmentLoadOp::dontCare,
			vk::AttachmentStoreOp::dontCare,
			vk::AttachmentLoadOp::dontCare,
			vk::AttachmentStoreOp::dontCare,
			vk::ImageLayout::undefined,
			vk::ImageLayout::colorAttachmentOptimal,
		}, {{},
			depthInfo.img.format, vk::SampleCountBits::e1,
			vk::AttachmentLoadOp::dontCare,
			vk::AttachmentStoreOp::dontCare,
			vk::AttachmentLoadOp::dontCare,
			vk::AttachmentStoreOp::dontCare,
			vk::ImageLayout::undefined,
			vk::ImageLayout::depthStencilAttachmentOptimal,
		}
	};

	vk::AttachmentReference colorRef {
		0, vk::ImageLayout::colorAttachmentOptimal
	};

	vk::AttachmentReference depthRef {
		1, vk::ImageLayout::depthStencilAttachmentOptimal
	};

	vk::SubpassDescription subpass {{},
		vk::PipelineBindPoint::graphics, 0, {},
		1, &colorRef, 0, &depthRef
	};

	vk::RenderPassCreateInfo rpInfo;
	vpp::RenderPass renderPass {dev, {{}, 
		2, attachments,
		1, &subpass,
	}};

	auto fbAttachments = {color.vkImageView(), depth.vkImageView()};
	vk::FramebufferCreateInfo info {
		{}, renderPass, 2, fbAttachments.begin(), size.width, size.height, 1
	};
	auto fb = vpp::Framebuffer(dev, info);
}

// TODO: add remaining objects