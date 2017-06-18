#include "bugged.hpp"
#include "init.hpp"
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
	sizer140.add(vpp::raw(fvec));
	sizer430.add(vpp::raw(fvec));

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

// TODO: fill, read
