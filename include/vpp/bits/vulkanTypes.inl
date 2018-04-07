// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

using RawBufferData = nytl::Span<const std::byte>;

/// Vulkan shader data types.
/// Defines all possible types that can be passed as buffer update paramter.
/// See bits/vulkanTypes.inl for more information.
enum class ShaderType {
	none, // used for objects like containers
	buffer, // used for raw data buffers that shall be plain copied
	scalar, // scalar like float or int, VulkanType has "size64" bool member
	vec, // VulkanType has "dimension" "size64" members
	mat, // VulkanType has "major" "minor" "transpose" "size64" members
	structure, // VulkanType has "member" tuples with member pointers and a bool "align"
	custom // Vulkan type holds "impl" type that will be called instead. See BufferApplier
};

/// The VulkanType template can be specialized for custom types to make vpp::BufferUpdate
/// able to align them correctly.
template<typename T> struct VulkanType { static constexpr auto type = ShaderType::none; };
template<typename T> struct VulkanType<T&> : public VulkanType<T> {};
template<typename T> struct VulkanType<const T> : public VulkanType<T> {};
template<typename T> struct VulkanType<volatile T> : public VulkanType<T> {};

/// Some VulkanType utility implementations that can be derived from when specializing for
/// custom types.

/// Major and minor always specify the size of the host matrix.
/// If transpose is set to true the device matrix is transposed (size and components).
/// Note that glsl (and spirv) lets you specify the layout of matrix in memory.
/// Note that glsl matrix access is always column-major, only the memory layout can be changed.
///
/// Examples:
/// host matrix 4x3 row major transpose=true -> shader matrix 3x4 column major.
/// host matrix 2x3 row major transpose=false -> shader matrix 2x3 row major.
/// host matrix 3x4 column major transpose=false -> shader matrix 3x4 column major.
template<std::size_t MJ, std::size_t MN, bool TRP = false, bool S64 = false>
struct VulkanTypeMat {
	static constexpr auto type = ShaderType::mat;
	static constexpr auto major = MJ; //(transpose ? col : row) count of host mat
	static constexpr auto minor = MN; //(transpose ? row : col) count of host mat
	static constexpr auto transpose = TRP; //defines whether to transpose mat on device
	static constexpr auto size64 = S64; //double precision matrix componenets?
};

/// Default vector base
/// \tparam N The dimension of the vector must be one of {1, 2, 3, 4}.
/// \tparam S64 Whether the values of the vector have 64 bit size.
template<std::size_t N, bool S64 = false>
struct VulkanTypeVec {
	static constexpr auto type = ShaderType::vec;
	static constexpr auto size64 = S64; // double precision vector compoenents?
	static constexpr auto dimension = N; // size of the vector
};

/// Default scalar base
/// \tparam S64 Whether the value has 64 bit size.
template<bool S64 = false>
struct VulkanTypeScalar {
	static constexpr auto type = ShaderType::scalar;
	static constexpr auto size64 = S64;
};

using VulkanTypeScalar32 = VulkanTypeScalar<false>;
using VulkanTypeScalar64 = VulkanTypeScalar<true>;

/// Default struct base. One must declare the "members" member as a tuple of pointers to the
/// structs members in the order they should be transferred to the device.
/// The "align" member defines whether the structure is treated as a structure on the device or
/// if it is only a host structure that will be unpacked on the device.
template<bool Align = true>
struct VulkanTypeStruct {
	static constexpr auto type = ShaderType::structure;
	static constexpr auto align = Align; // defines whether it should be aligned like a structure

	// NOTE: The static constexpr "members" tuple must be custom specified.
	// The tuple will determine which members will be written/read in which order to/from the device
	// static constexpr auto members = std::make_tuple(&Type::member1, &Type::member2, ...);
};

/// Default raw buffer base.
struct VulkanTypeBuffer {
	static constexpr auto type = ShaderType::buffer;
};

///Some VulkanType specializations for default types.
template<> struct VulkanType<float> : public VulkanTypeScalar32 {};
template<> struct VulkanType<bool> : public VulkanTypeScalar32 {};
template<> struct VulkanType<uint32_t> : public VulkanTypeScalar32 {};
template<> struct VulkanType<int32_t> : public VulkanTypeScalar32 {};
template<> struct VulkanType<double> : public VulkanTypeScalar64 {};
template<> struct VulkanType<RawBufferData> : public VulkanTypeBuffer {};

/// Can be used to easily construct raw buffer wrappers around data to fill a buffer
/// \requires The given obj (or array of objects) must have standard layout, otherwise
/// they cannot be converted to raw memory.
template<typename T,
	typename = std::enable_if_t<!std::is_pointer_v<T>>,
	typename = std::enable_if_t<std::is_standard_layout_v<T>>>
constexpr RawBufferData raw(const T& obj, std::size_t count = 1) {
	auto ptr = reinterpret_cast<const std::byte*>(&obj);
	return {ptr, count * sizeof(T)};
}

/// Converts a given container of standard layout values into a raw data buffer.
template<typename C,
	typename D = decltype(*std::declval<C>().data()),
	typename = decltype(std::declval<C>().size()),
	typename = std::enable_if_t<std::is_standard_layout_v<std::remove_reference_t<D>>>>
constexpr RawBufferData rawSpan(const C& span) {
	auto ptr = reinterpret_cast<const std::byte*>(span.data());
	return {ptr, span.size() * sizeof(std::remove_reference_t<D>)};
}
