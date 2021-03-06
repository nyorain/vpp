#include "init.hpp"
#include <vpp/sharedBuffer.hpp>
#include <ostream>

template<typename T>
std::ostream& operator<<(std::ostream& os, const vpp::BasicAllocation<T>& alloc)
{
	os << "(" << alloc.offset << ", " << alloc.size << ")";
	return os;
}

TEST(sharedBuf) {
	auto& dev = *globals.device;
	vpp::SharedBuffer buf(dev.devMemAllocator(),
		{{}, 1024u, vk::BufferUsageBits::uniformBuffer});
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

	vpp::SubBuffer bufRange(buf, buf.alloc(100u));
	EXPECT(&bufRange.buffer(), &buf);
	EXPECT(bufRange.size(), 100u);
	EXPECT(bufRange.offset(), 0u);

	auto bufRange2 = std::move(bufRange);
	EXPECT(&bufRange2.buffer(), &buf);
	EXPECT(bufRange2.size(), 100u);
	EXPECT(bufRange2.offset(), 0u);

	// allocator
	vpp::BufferAllocator bufAlloc(dev);
	auto id0 = bufAlloc.reserve(10000u, vk::BufferUsageBits::uniformBuffer);
	auto id1 = bufAlloc.reserve(7u, vk::BufferUsageBits::storageBuffer);
	auto id2 = bufAlloc.reserve(100u, vk::BufferUsageBits::indexBuffer);
	EXPECT(bufAlloc.buffers().size(), 0u);

	auto usage = vk::BufferUsageBits::uniformBuffer |
			vk::BufferUsageBits::storageBuffer;
	for(auto i = 0u; i < 100; ++i) {
		vpp::SubBuffer tmp(bufAlloc, 11000u, usage);
		EXPECT(bufAlloc.buffers().size(), 1u);
	}

	bufAlloc.cancel(id0);
	bufAlloc.cancel(id1);
	bufAlloc.cancel(id2);
}

TEST(alignment) {
	using Alloc = vpp::SharedBuffer::Allocation;
	auto& dev = *globals.device;
	vpp::SharedBuffer buf(dev.devMemAllocator(),
		{{}, 2048u, vk::BufferUsageBits::uniformBuffer});

	auto alloc1 = buf.alloc(230u, 64u);
	EXPECT(alloc1, (Alloc{0, 230u}));

	auto alloc2 = buf.alloc(200u, 128u);
	EXPECT(alloc2, (Alloc{256u, 200u}));

	auto alloc3 = buf.alloc(44u, 2u);
	EXPECT(alloc3, (Alloc{456u, 44u}));

	auto alloc4 = buf.alloc(2048, 0u);
	EXPECT(alloc4.size, 0u);

	auto alloc5 = buf.alloc(100u, 0u);
	EXPECT(alloc5, (Alloc{500u, 100u}));

	auto alloc6 = buf.alloc(100u, 1u);
	EXPECT(alloc6, (Alloc{600u, 100u}));

	auto alloc7 = buf.alloc(100u, 2048u);
	EXPECT(alloc7.size, 0u);

	buf.free(alloc1);
	auto alloc8 = buf.alloc(100u, 2048u);
	EXPECT(alloc8, (Alloc{0u, 100u}));

	auto alloc9 = buf.alloc(99, 0u);
	EXPECT(alloc9, (Alloc{100u, 99u}));

	auto alloc10 = buf.alloc(1, 1);
	EXPECT(alloc10, (Alloc{199u, 1u}));

	buf.free(alloc2);
	buf.free(alloc3);
	buf.free(alloc5);
	buf.free(alloc6);
	buf.free(alloc8);
	buf.free(alloc9);
	buf.free(alloc10);
}

TEST(nonCoherentAtomAlign) {
	using Alloc = vpp::SharedBuffer::Allocation;
	auto& dev = *globals.device;
	auto coherentBits = dev.memoryTypeBits(vk::MemoryPropertyBits::hostCoherent);
	auto atomAlign = dev.properties().limits.nonCoherentAtomSize;
	auto hostNonCoherent = dev.hostMemoryTypes() & ~coherentBits;

	// not guaranteed to exist
	if(!hostNonCoherent) {
		return;
	}

	vpp::SharedBuffer buf(dev.devMemAllocator(),
		{{}, 2048u, vk::BufferUsageBits::uniformBuffer}, hostNonCoherent);

	vpp::SubBuffer range1(buf, buf.alloc(10u));
	EXPECT(range1.allocation(), (Alloc{0u, 10u}));

	vpp::SubBuffer range2(buf, buf.alloc(10u));
	auto offset = vpp::align<vk::DeviceSize>(10u, atomAlign);
	EXPECT(range2.allocation(), (Alloc{offset, 10u}));

	vpp::SubBuffer range3(buf, buf.alloc(100u));
	offset = vpp::align<vk::DeviceSize>(end(range2.allocation()), atomAlign);
	EXPECT(range3.allocation(), (Alloc{offset, 100u}));

	auto range4 = std::move(range3);
}

TEST(mappable) {
	auto& dev = *globals.device;
	auto& allocator = dev.bufferAllocator();
	auto usage = vk::BufferUsageBits::vertexBuffer;
	auto coherentBits = dev.memoryTypeBits(vk::MemoryPropertyBits::hostCoherent);
	auto atomAlign = dev.properties().limits.nonCoherentAtomSize;
	auto hostBits = dev.hostMemoryTypes();

	// just allocate a mappable buffer
	auto buf1 = vpp::SubBuffer(allocator, 100u, usage, hostBits);
	EXPECT(buf1.allocation().size, 100u);
	EXPECT(buf1.buffer().mappable(), true);
	auto props = buf1.buffer().memory().properties();
	auto coherent = (props & vk::MemoryPropertyBits::hostCoherent);
	EXPECT(buf1.offset() % 128u, 0u);
	if(!coherent) {
		EXPECT(buf1.offset() % atomAlign, 0u);
	}

	// allocate mappable buffer on coherent memory
	auto buf2 = vpp::SubBuffer{allocator, 1000u, usage, coherentBits};
	EXPECT(buf2.allocation().size, 1000u);
	EXPECT(buf2.buffer().mappable(), true);
	auto type = buf2.buffer().memory().type();
	EXPECT((coherentBits & (1 << type)) != 0, true);

	auto mapView1 = buf2.memoryMap();
	auto mapView2 = buf2.memoryMap();
	auto mapView3 = buf2.memoryMap();

	// allocate mappable buffer on non-coherent memory
	// there might be vulkan implementations where are all hostVisible
	// types are also hostCoherent, we cannot do this test there
	auto hostNonCoherent = hostBits & ~coherentBits;
	if(hostNonCoherent) {
		auto buf3 = vpp::SubBuffer(allocator, 511u, usage, hostNonCoherent);
		EXPECT(buf3.allocation().size, 511u);
		EXPECT(buf3.buffer().mappable(), true);
		type = buf3.buffer().memory().type();
		EXPECT((~coherentBits & (1 << type)) != 0, true);
		EXPECT(buf3.offset() % atomAlign, 0u);
	}
}

TEST(defer) {
	auto& dev = *globals.device;
	auto& allocator = dev.bufferAllocator();
	auto usage = vk::BufferUsageBits::uniformBuffer |
		vk::BufferUsageBits::vertexBuffer;
	auto hostBits = dev.hostMemoryTypes();
	auto devBits = dev.deviceMemoryTypes();

	std::array<vpp::SubBuffer::InitData, 6> data;
	auto buf1 = vpp::SubBuffer(data[0], allocator, 3251u, usage, hostBits);
	usage |= vk::BufferUsageBits::transferSrc;
	auto buf2 = vpp::SubBuffer(data[1], allocator, 6431u, usage, devBits);
	usage = vk::BufferUsageBits::transferDst;
	auto buf3 = vpp::SubBuffer(data[2], allocator, 234u, usage, devBits, 32u);
	usage |= vk::BufferUsageBits::storageBuffer;
	auto buf4 = vpp::SubBuffer(data[3], allocator, 54u, usage, hostBits);
	usage = vk::BufferUsageBits::storageTexelBuffer;
	auto buf5 = vpp::SubBuffer(data[4], allocator, 53221u, usage, devBits);

	buf1.init(data[0]);
	buf2.init(data[1]);
	buf3.init(data[2]);

	EXPECT(buf1.size(), 3251u);
	auto b1m = (1u << buf1.buffer().memory().type());
	EXPECT(((b1m & hostBits) != 0), true);
	EXPECT(buf2.size(), 6431u);
	auto b2m = (1u << buf2.buffer().memory().type());
	EXPECT(((b2m & devBits) != 0), true);
	EXPECT(buf3.size(), 234u);

	auto buf6 = vpp::SubBuffer(data[5], allocator, 2143u, usage, devBits, 2u);
	buf4.init(data[3]);
	buf5.init(data[4]);
	buf6.init(data[5]);

	EXPECT(buf4.size(), 54u);
	EXPECT(buf5.size(), 53221u);
	EXPECT(buf6.size(), 2143u);
}
