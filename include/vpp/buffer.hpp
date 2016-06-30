#pragma once

#include <vpp/fwd.hpp>
#include <vpp/memoryResource.hpp>
#include <vpp/allocator.hpp>
#include <vpp/commandBuffer.hpp>
#include <vpp/work.hpp>
#include <vpp/vulkan/range.hpp>

#include <memory>
#include <type_traits>
#include <cstring>
#include <cmath>

namespace vpp
{

using vk::Range;

struct UnalignedTag {};
constexpr UnalignedTag unaligned;

///Vulkan shader data types.
enum class ShaderType
{
	scalar,
	scalar_64,
	vec2,
	vec3,
	vec4,
	vec2_64,
	vec3_64,
	vec4_64,
	structure
};

inline unsigned int align(ShaderType type)
{
	switch(type)
	{
		case ShaderType::scalar: return 4;
		case ShaderType::scalar_64:
		case ShaderType::vec2: return 8;
		case ShaderType::vec3:
		case ShaderType::vec4:
		case ShaderType::vec2_64: return 16;
		case ShaderType::vec3_64:
		case ShaderType::vec4_64: return 32;
		default: return 0;
	}
}
inline unsigned int align(const Range<ShaderType> types)
{
	auto max = 0u;
	for(auto& t : types)
		if(align(t) > max) max = align(t);
	return std::max(max, 16u);
}

template<typename T> struct VulkanType;
template<typename T> struct VulkanType<T&> : public VulkanType<T> {};
template<typename T> struct VulkanType<const T> : public VulkanType<T> {};
template<typename T> struct VulkanType<volatile T> : public VulkanType<T> {};
template<typename T> struct VulkanType<const volatile T> : public VulkanType<T> {};
template<> struct VulkanType<float> { static constexpr auto type = ShaderType::scalar; };
template<> struct VulkanType<bool> { static constexpr auto type = ShaderType::scalar; };
template<> struct VulkanType<std::uint32_t> { static constexpr auto type = ShaderType::scalar; };
template<> struct VulkanType<std::int32_t> { static constexpr auto type = ShaderType::scalar; };
template<> struct VulkanType<double> { static constexpr auto type = ShaderType::scalar_64; };

struct Vec2 { float x,y; };
struct MyStruct { float a; Vec2 b; };
template<> struct VulkanType<Vec2> { static constexpr auto type = ShaderType::vec2; };
template<> struct VulkanType<MyStruct>
{
	static constexpr auto members = std::make_tuple(&MyStruct::a, &MyStruct::b);
	static constexpr auto type = ShaderType::structure;
};

template<typename T> auto constexpr VulkanTypeV = VulkanType<T>::type;

///Utilty classes for filling a buffer with aligned data.
struct DataType
{
	ShaderType type;
	unsigned int count = 1;
};

///Utility class for filling buffers with mulitple data segments.
class BufferData
{
public:
	const void* data = nullptr; //a pointer to the data
	std::size_t size = 0; //size of the given data
	std::size_t offset = 0; //additional relative (!) offset to the previous given data segment

	DataType type;

public:
	BufferData() = default;

	///Constructs the buffer data by directly storing an object.
	template<typename T, typename = std::enable_if_t<std::is_standard_layout<T>::value>>
	BufferData(const T& obj, std::size_t xoff = 0)
		: data(&obj), size(sizeof(T)), offset(xoff), type{VulkanType<T>::type} {}

	///Constructs the buffer data for a given container, which must correctly implement
	///a size and a data member function and must have its value type as first template param.
	///The container must be contigous.
	///Can be e.g. be used to create a BufferData object diretly from a std::vector or std::string.
	template<template<typename, typename...> typename C, typename T, typename... A>
	BufferData(const C<T, A...>& container, std::size_t xoff = 0)
		: data(container.data()), size(container.size() * sizeof(T)), offset(xoff) {}
			//type{VulkanType<T>::type, container.size()} {}

	///Constructs the buffer data for a given array.
	template<typename T, std::size_t S>
	BufferData(const T (&array)[S], std::size_t xoff = 0)
		: data(array), size(S * sizeof(T)), offset(xoff), type{VulkanType<T>::type, S} {}
};

template<typename T, ShaderType V>
struct BufferWriter
{
	void operator()(std::uint8_t& data, std::size_t& offset, const T& obj, bool std140)
	{
		offset = std::ceil(offset / double(align(V))) * align(V);
		std::memcpy(&data + offset, &obj, sizeof(obj));
		offset += sizeof(obj);
	}
};

template<typename T, std::size_t S, ShaderType V>
struct BufferWriter<T[S], V>
{
	void operator()(std::uint8_t& data, std::size_t& offset, const T (&array)[S], bool std140)
	{
		auto sa = align(V);
		if(std140) sa = std::max(sa, 16u);
		offset = std::ceil(offset / double(sa)) * sa;
		for(auto a : array)
		{
			if(std140) offset = std::ceil(offset / double(16)) * 16;
			bufferWrite(data, offset, a, std140);
		}

		if(std140) offset = std::ceil(offset / double(sa)) * sa;
	}
};

template<ShaderType V, typename T>
void bufferWrite(std::uint8_t& data, std::size_t& offset, const T& obj, bool std140)
{
	BufferWriter<T, V> writer;
	writer(data, offset, obj, std140);
}

template<typename T, ShaderType V = VulkanType<T>::type>
void bufferWrite(std::uint8_t& data, std::size_t& offset, const T& obj, bool std140)
{
	BufferWriter<T, V> writer;
	writer(data, offset, obj, std140);
}

template<typename T, typename = decltype(&T::begin), typename = decltype(&T::end)>
void bufferWrite(std::uint8_t& data, std::size_t& offset, const T& obj, bool std140)
{
	static constexpr auto V = VulkanType<decltype(*obj.begin())>::type;

	auto sa = align(V);
	if(std140) sa = std::max(sa, 16u);
	offset = std::ceil(offset / double(sa)) * sa;
	for(auto& a : obj)
	{
		if(std140) offset = std::ceil(offset / double(16)) * 16;
		bufferWrite(data, offset, a, std140);
	}

	if(std140) offset = std::ceil(offset / double(sa)) * sa;
}

template<typename T> struct PrintMembers;

template<std::size_t... I>
struct PrintMembers<std::index_sequence<I...>>
{
	template<typename T>
	void operator()(std::uint8_t& data, std::size_t& offset, const T& obj, bool std140)
	{
		auto constexpr tup = VulkanType<T>::members;
		auto sa = 0u;
		if(std140) sa = 16u;
		int expand1[] = {( sa = std::max(sa,
			align(VulkanType<decltype(obj.*(std::get<I>(tup)))>::type)), 0)...};
		offset = std::ceil(offset / sa) * sa;
		int expand2[] = {( bufferWrite(data, offset, obj.*(std::get<I>(tup)), std140), 0) ...};
	}
};

template<typename T>
struct BufferWriter<T, ShaderType::structure>
{
	void operator()(std::uint8_t& data, std::size_t& offset, const T& obj, bool std140)
	{
		using V = VulkanType<T>;
		PrintMembers<std::make_index_sequence<std::tuple_size<decltype(V::members)>::value>> printer;
		printer(data, offset, obj, std140);
	}
};

struct RandomStruct
{
	float a;
	int b;
	int c[2];
};

inline int someFunc()
{
	MyStruct a;
	a.a = 100;
	a.b = {23, 420};
	std::uint8_t data[100] {0};

	auto list = {4, 6, 3};

	RandomStruct b;

	std::size_t offset = 0u;
	bufferWrite(*data, offset, a, true);
	bufferWrite(*data, offset, list, true);
	bufferWrite(*data, offset, list, true);
	//bufferWrite(*(data + offset), offset, b);

	std::cout << (float&) data[0] << "\n";
	std::cout << (float&) data[8] << "\n";
	std::cout << (float&) data[12] << "\n";
	std::cout << (int&) data[16] << "\n";
	std::cout << (int&) data[20] << "\n";

	std::cout << "offset: " << offset << "\n";

	offset = 0u;
	bufferWrite(*data, offset, a, false);
	bufferWrite(*data, offset, list, false);
	bufferWrite(*data, offset, list, false);

	std::cout << "offset: " << offset << "\n";
}


class BufferFill
{
public:
	enum class Align
	{
		std140,
		std430
	};

public:
	BufferFill(const Buffer& buffer, Align align);
	~BufferFill();

	template<typename T>
	void add(const T& obj){ offset_ += bufferWrite(data(), offset, obj, true); }

	// template<typename T>
	// void add(const T& obj){ offset_ += structWrite(data(), obj); }

	void offset(std::size_t size){ offset_ += size; }

	void alignUniform();
	void alignStorage();
	void alignTexel();
	void alignStruct();

	WorkPtr apply();

	void write(const std::uint8_t& data, const std::size_t size);
	std::uint8_t& data();

protected:
	const Buffer* buffer_ {};
	WorkPtr work_ {};
	std::size_t offset_ {};
};

class BufferRead
{
public:
	BufferRead(const Buffer& buffer);
};

void alignedWrite(std::uint8_t* ptr, const Range<BufferData>& datas);
void alignedRead(const std::uint8_t* ptr, Range<BufferData>& datas);

///Representing a vulkan buffer on a device.
///Can be filled and read, and stores a handle to the memory location it is allocated on (or
///the allocator that will take care of its allocation).
///Does not store additional information such as buffer usage type or memory layout, this
///must be handled by the application for best performance.
class Buffer : public MemoryResource
{
public:
	Buffer() = default;
	Buffer(const Device& dev, const vk::BufferCreateInfo& info, vk::MemoryPropertyFlags mflags = {});
	~Buffer();

	Buffer(Buffer&& other) noexcept;
	Buffer& operator=(Buffer other) noexcept;

	///Fills the buffer with the given data.
	///Does this either by memory mapping the buffer or by copying it via command buffer.
	///Expects that buffer was created fillable, so either the buffer is memory mappable or
	///it is allowed to copy data into it and the device was created with a matching queue.
	///Note that this operation may be asnyc, therefore a work unique ptr is returned.
	///If multiple (device local) buffers are to fill it brings usually huge performace gains
	///to first call fill() on them all and then make sure that the needed work finishes.
	///\param data The data the buffer should be filled with (may be multiple data segments)
	///\param direct Specifies whether direct transfer via cmdUpdateBuffer should be preferred over a
	///uploadBuffer copy if possible. By default it is preferred. Will only be taken into account if
	///the buffer is device local and the given data makes direct transfer possible.
	WorkPtr fill(const std::vector<BufferData>& data, bool direct = true) const;

	//TODO: retrieve only specific range
	///Retrives the data stored in the buffer.
	///\return A Work ptr that is able to retrive an array of std::uint8_t values storing the data.
	DataWorkPtr retrieve() const;

	const vk::Buffer& vkBuffer() const { return buffer_; }

	operator vk::Buffer() const { return vkBuffer(); }
	friend void swap(Buffer& a, Buffer& b) noexcept;

protected:
	vk::Buffer buffer_ {};
};

};
