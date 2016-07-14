#pragma once

///The RawBufferData is an explicit wrapper around custom aligned data to fill a buffer
///with.
struct RawBufferData
{
public:
	const std::uint8_t& data;
	std::size_t size;

public:
	template<typename T> RawBufferData(const T& obj, std::size_t count = 1)
		: data(reinterpret_cast<const std::uint8_t&>(obj)), size(sizeof(T) * count) {}
};

///\{
///Can be used to easily construct raw buffer wrappers around data to fill a buffer
template<typename T, typename = std::enable_if_t<!std::is_pointer<T>::value>>
constexpr RawBufferData raw(const T& obj, std::size_t count = 1) { return {obj, count}; }

template<typename T> constexpr RawBufferData
raw(const T* obj, std::size_t count = 1) { return {*obj, count}; }

template<template<class...> typename C, typename T, typename... TA> constexpr RawBufferData
raw(const C<T, TA...>& container) { return {*container.begin(), container.size()}; }
///\}

///The VulkanType template can be specialized for custom types to make vpp::BufferUpdate
///able to align them correctly.
template<typename T> struct VulkanType { static constexpr auto type = ShaderType::none; };

///\{
///Some VulkanType specializations for default types.
template<typename T> struct VulkanType<T&> : public VulkanType<T> {};
template<typename T> struct VulkanType<const T> : public VulkanType<T> {};
template<typename T> struct VulkanType<volatile T> : public VulkanType<T> {};
template<typename T> struct VulkanType<const volatile T> : public VulkanType<T> {};
template<> struct VulkanType<float> { static constexpr auto type = ShaderType::scalar; };
template<> struct VulkanType<bool> { static constexpr auto type = ShaderType::scalar; };
template<> struct VulkanType<std::uint32_t> { static constexpr auto type = ShaderType::scalar; };
template<> struct VulkanType<std::int32_t> { static constexpr auto type = ShaderType::scalar; };
template<> struct VulkanType<double> { static constexpr auto type = ShaderType::scalar_64; };
template<> struct VulkanType<RawBufferData> { static constexpr auto type = ShaderType::buffer; };
///\}

///\{
///Some VulkanType utility implementations that can be derived from when specializing for
///custom types.
template<std::size_t MJ, std::size_t MN, bool TRP = false> struct VulkanTypeMatrix
{
	static constexpr auto type = vpp::ShaderType::matrix;
	static constexpr auto major = MJ;
	static constexpr auto minor = MN;
	static constexpr auto transpose = TRP;
};

template<std::size_t N, std::size_t S> struct VulkanTypeVec;
template<> struct VulkanTypeVec<2, 4> { static constexpr auto type = ShaderType::vec2; };
template<> struct VulkanTypeVec<3, 4> { static constexpr auto type = ShaderType::vec3; };
template<> struct VulkanTypeVec<4, 4> { static constexpr auto type = ShaderType::vec4; };
template<> struct VulkanTypeVec<2, 8> { static constexpr auto type = ShaderType::vec2_64; };
template<> struct VulkanTypeVec<3, 8> { static constexpr auto type = ShaderType::vec3_64; };
template<> struct VulkanTypeVec<4, 8> { static constexpr auto type = ShaderType::vec4_64; };

struct VulkanTypeVec2 { static constexpr auto type = vpp::ShaderType::vec2; };
struct VulkanTypeVec3 { static constexpr auto type = vpp::ShaderType::vec3; };
struct VulkanTypeVec4 { static constexpr auto type = vpp::ShaderType::vec4; };
///\}

template<typename T> auto constexpr VulkanTypeV = VulkanType<T>::type;
