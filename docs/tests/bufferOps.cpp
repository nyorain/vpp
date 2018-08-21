#include "init.hpp"
#include "bugged.hpp"
#include <vpp/buffer.hpp>
#include <vpp/bufferOps.hpp>
#include <vector>
#include <cstdint>

// some custom dummy shader types
struct Vec2f : public std::array<float, 2> {};
struct Vec3f : public std::array<float, 3> {};
struct Vec4f : public std::array<float, 4> {};

struct Vec2d : public std::array<double, 2> {};
struct Vec3d : public std::array<double, 3> {};
struct Vec4d : public std::array<double, 4> {};

struct Mat2f : public std::array<Vec2f, 2> {};
struct Mat3f : public std::array<Vec3f, 3> {};
struct Mat4f : public std::array<Vec4f, 4> {};

struct Mat2d : public std::array<Vec2d, 2> {};
struct Mat3d : public std::array<Vec3d, 3> {};
struct Mat4d : public std::array<Vec4d, 4> {};

template<typename T, std::size_t I>
std::ostream& operator<<(std::ostream& os, const std::array<T, I>& arr)
{
	os << "(";
	for(auto& val : arr) {
		os << val << ",";
	}
	os << ")";
	return os;
}

struct SomePOD {
	float value;
	Vec3f vec;
};

// specialize vulkan handling of our dummy types
namespace vpp {

template<> struct VulkanType<Vec2f> : VulkanTypeVec<2> {};
template<> struct VulkanType<Vec3f> : VulkanTypeVec<3> {};
template<> struct VulkanType<Vec4f> : VulkanTypeVec<4> {};

template<> struct VulkanType<Vec2d> : VulkanTypeVec<2, true> {};
template<> struct VulkanType<Vec3d> : VulkanTypeVec<3, true> {};
template<> struct VulkanType<Vec4d> : VulkanTypeVec<4, true> {};

template<> struct VulkanType<Mat2f> : VulkanTypeMat<2, 2> {};
template<> struct VulkanType<Mat3f> : VulkanTypeMat<3, 3> {};
template<> struct VulkanType<Mat4f> : VulkanTypeMat<4, 4> {};

template<> struct VulkanType<Mat2d> : VulkanTypeMat<2, 2, false, true> {};
template<> struct VulkanType<Mat3d> : VulkanTypeMat<3, 3, false, true> {};
template<> struct VulkanType<Mat4d> : VulkanTypeMat<4, 4, false, true> {};

template<> struct VulkanType<SomePOD> : VulkanTypeStruct<> {
	static constexpr auto members = std::make_tuple(&SomePOD::value, &SomePOD::vec);
};

}

template <int i> struct D;

// tests
TEST(constexpr_size) {
	// basic tests
	static_assert(vpp::neededBufferSize140<float>() == 4);
	static_assert(vpp::neededBufferSize140<double>() == 8);
	static_assert(vpp::neededBufferSize140<int>() == 4);
	static_assert(vpp::neededBufferSize140<unsigned int>() == 4);
	static_assert(vpp::neededBufferSize140<Vec2f>() == 8);
	static_assert(vpp::neededBufferSize140<Vec3f>() == 12);
	static_assert(vpp::neededBufferSize140<Vec4f>() == 16);
	static_assert(vpp::neededBufferSize140<Vec2d>() == 8 * 2);
	static_assert(vpp::neededBufferSize140<Vec3d>() == 12 * 2);
	static_assert(vpp::neededBufferSize140<Vec4d>() == 16 * 2);
	static_assert(vpp::neededBufferSize140<Mat2f>() == 2 * 4 * 4);
	static_assert(vpp::neededBufferSize140<Mat3f>() == 3 * 4 * 4);
	static_assert(vpp::neededBufferSize140<Mat4f>() == 4 * 4 * 4);
	static_assert(vpp::neededBufferSize140<SomePOD>() == 4 + 12 + 3 * 4);

	static_assert(vpp::neededBufferSize430<Vec2f>() == 8);
	static_assert(vpp::neededBufferSize430<Vec3f>() == 12);
	static_assert(vpp::neededBufferSize430<Vec4f>() == 16);
	static_assert(vpp::neededBufferSize430<Vec2d>() == 8 * 2);
	static_assert(vpp::neededBufferSize430<Vec3d>() == 12 * 2);
	static_assert(vpp::neededBufferSize430<Vec4d>() == 16 * 2);
	static_assert(vpp::neededBufferSize430<Mat2f>() == 2 * 2 * 4);
	static_assert(vpp::neededBufferSize430<Mat3f>() == 3 * 4 * 4);
	static_assert(vpp::neededBufferSize430<Mat4f>() == 4 * 4 * 4);
	static_assert(vpp::neededBufferSize430<SomePOD>() == 4 + 12 + 3 * 4);


	// float, align, vec, align, mat
	constexpr auto a140 = vpp::neededBufferSize140<float, Vec3f, Mat4f>();
	static_assert(a140 == 4 + 12 + (4 * 3) + 4 + (4 * 4 * 4));

	constexpr auto a430 = vpp::neededBufferSize430<float, Vec3f, Mat4f>();
	static_assert(a430 == a430);

	// float, align, vec, align, mat
	// second align, mat stride differs for std430
	constexpr auto b140 = vpp::neededBufferSize140<float, Vec3f, Mat2f>();
	static_assert(b140 == 4 + 12 + (4 * 3) + 4 + (2 * 4 * 4));

	constexpr auto b430 = vpp::neededBufferSize430<float, Vec3f, Mat2f>();
	static_assert(b430 == 4 + 12 + (4 * 3) + 4 + (2 * 2 * 4));

	// double, align, mat, mat, vec
	constexpr auto c140 = vpp::neededBufferSize140<double, Mat2f, Mat3f, Vec2f>();
	static_assert(c140 == 8 + 8 + (2 * 4 * 4) + (3 * 4 * 4) + (2 * 4));

	constexpr auto c430 = vpp::neededBufferSize430<double, Mat2f, Mat3f, Vec2f>();
	static_assert(c430 == 8 + (2 * 2 * 4) + 8 + (3 * 4 * 4) + (2 * 4));
}

TEST(runtime_size) {
	vpp::BufferSizer sizer140(*globals.device, vpp::BufferLayout::std140);
	vpp::BufferSizer sizer430(*globals.device, vpp::BufferLayout::std430);

	// interpreted as array
	std::vector<float> fvec(20);
	sizer140.add(fvec);
	sizer430.add(fvec);

	EXPECT(sizer140.offset(), 19u * 16u + 4u);
	EXPECT(sizer430.offset(), 20u * 4u);

	// interpreted as raw buffer
	sizer140.add(vpp::rawSpan(fvec));
	sizer430.add(vpp::rawSpan(fvec));

	EXPECT(sizer140.offset(), 20u * 16u + 20u * 4u);
	EXPECT(sizer430.offset(), 40u * 4u);

	// interpreted as array
	std::uint32_t iarray[5] {};
	sizer140.add(iarray);
	sizer430.add(iarray);

	EXPECT(sizer140.offset(), 20u * 16u + 20u * 4u + 4u * 16u + 4u);
	EXPECT(sizer430.offset(), 40u * 4u + 5u * 4u);

	// TODO: MyPOD with vpp::raw
}

TEST(write_read) {
	// create a mappable buffer
	constexpr auto bufSize = 1024;
	vk::BufferCreateInfo bufInfo;
	bufInfo.size = bufSize;
	bufInfo.usage = vk::BufferUsageBits::uniformBuffer |
		vk::BufferUsageBits::transferSrc;
	auto bits = globals.device->hostMemoryTypes();
	vpp::Buffer buf(*globals.device, bufInfo, bits);

	// write to it
	vpp::MappedBufferWriter writer(buf.memoryMap(), vpp::BufferLayout::std140);
	writer.add(1.f);
	writer.add(Vec2f{2.f, 3.f});
	writer.add(4.);
	writer.add(Vec3f{5.f, 6.f, 7.f});
	writer.add(8);

	// validate contents manually
	{
		auto map = buf.memoryMap();
		auto ptr = map.ptr();

		EXPECT(*reinterpret_cast<float*>(ptr), 1.f);
		ptr += 4; // float
		ptr += 4; // alignment
		EXPECT(*reinterpret_cast<Vec2f*>(ptr), (Vec2f{2.f, 3.f}));
		ptr += 8; // vec2
		EXPECT(*reinterpret_cast<double*>(ptr), 4.);
		ptr += 8; // double
		ptr += 8; // alignment
		EXPECT(*reinterpret_cast<Vec3f*>(ptr), (Vec3f{5.f, 6.f, 7.f}));
		ptr += 12; // vec3
		EXPECT(*reinterpret_cast<int*>(ptr), 8);
	}

	// read from it
	float r1;
	Vec2f r23;
	double r4;
	Vec3f r567;
	int r8;

	vpp::readMap140({buf, bufSize}, r1, r23, r4, r567, r8);

	EXPECT(r1, 1.f);
	EXPECT(r23, (Vec2f{2.f, 3.f}));
	EXPECT(r4, 4.);
	EXPECT(r567, (Vec3f{5.f, 6.f, 7.f}));
	EXPECT(r8, 8);

	r1 = {}; r23 = {}; r4 = {}; r567 = {}; r8 = {};
	vpp::readStaging140({buf, bufSize}, r1, r23, r4, r567, r8);

	EXPECT(r1, 1.f);
	EXPECT(r23, (Vec2f{2.f, 3.f}));
	EXPECT(r4, 4.);
	EXPECT(r567, (Vec3f{5.f, 6.f, 7.f}));
	EXPECT(r8, 8);
}

vpp::SubBuffer readWrite(bool mappable, vk::BufferUsageFlags usage,
	bool direct = false)
{
	auto& dev = *globals.device;
	auto bits = mappable ? dev.hostMemoryTypes() : ~0u;
	auto size = vpp::neededBufferSize140<float, Vec3f, std::int32_t>();
	auto buf = vpp::SubBuffer(dev.bufferAllocator(), size, usage, bits);

	float a {};
	Vec3f b {};
	std::int32_t c {};

	if(!direct && !mappable) {
		auto work = vpp::writeStaging140(buf, 42.f, Vec3f {1.f, 2.f, 3.f},
			(std::int32_t) -420);
		auto work2 = std::move(work);
		work2.finish();

		vpp::readStaging140(buf, a, b, c);
	} else if(!direct && mappable) {
		vpp::writeMap140(buf, 42.f, Vec3f {1.f, 2.f, 3.f}, (std::int32_t) -420);
		vpp::readMap140(buf, a, b, c);
	} else {
		auto work = vpp::writeDirect140(buf, 42.f, Vec3f {1.f, 2.f, 3.f},
			(std::int32_t) -420);
		auto work2 = std::move(work);
		work2.finish();

		vpp::readStaging140(buf, a, b, c);
	}

	EXPECT(a, 42.f);
	EXPECT(b, (Vec3f {1.f, 2.f, 3.f}));
	EXPECT(c, -420);

	return buf;
}

TEST(buffer_range) {
	using BUB = vk::BufferUsageBits;;

	auto& dev = *globals.device;
	auto usage = BUB::storageBuffer | BUB::transferSrc | BUB::transferDst;
	dev.bufferAllocator().reserve(500, usage, dev.hostMemoryTypes());
	dev.bufferAllocator().reserve(500, usage, dev.hostMemoryTypes());

	auto buf1 = readWrite(true, usage);
	auto buf2 = readWrite(true, usage);
	auto buf3 = readWrite(false, usage);
	auto buf4 = readWrite(false, usage);
	auto buf5 = readWrite(true, usage, true);
	auto buf6 = readWrite(false, usage, true);

	for(auto i = 0u; i < 100u; ++i) {
		auto range = readWrite(true, usage);
		auto range2 = std::move(range);

		readWrite(false, usage);
		readWrite(true, usage, true);
		readWrite(false, usage, true);
	}

	EXPECT(dev.bufferAllocator().buffers().size() <= 2, true);
}

// NOTE: this test should output a failed dlg_assert
TEST(bufferOps_overflow) {
	using BUB = vk::BufferUsageBits;
	auto& dev = *globals.device;

	auto bits = dev.memoryTypeBits(vk::MemoryPropertyBits::hostVisible);
	auto usage = BUB::uniformBuffer | BUB::transferDst;
	vpp::SharedBuffer sbuf(dev, {{}, 1024, usage}, bits);
	auto range1 = vpp::SubBuffer(sbuf, sbuf.alloc(64u));
	auto range2 = vpp::SubBuffer(sbuf, sbuf.alloc(64u));
	auto range3 = vpp::SubBuffer(sbuf, sbuf.alloc(64u));

	// all three of them should trigger a failed assertion
	// std::byte data[65];
	// std::byte data2[60];
	// vpp::writeMap140(range1, vpp::raw(data));
	// vpp::writeStaging430(range2, vpp::raw(data));
	// vpp::writeStaging140(range3, 1.f, 2.f, 3.f, vpp::raw(data2));
}
