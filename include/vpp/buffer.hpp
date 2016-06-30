#pragma once

#include <vpp/fwd.hpp>
#include <vpp/memoryResource.hpp>
#include <vpp/allocator.hpp>
#include <vpp/commandBuffer.hpp>
#include <vpp/work.hpp>
#include <vpp/vulkan/range.hpp>

#include <memory>
#include <type_traits>

namespace vpp
{

using vk::Range;

struct UnalignedTag {};
UnalignedTag unaligned;

///Vulkan shader data types.
enum ShaderType
{
	scalar,
	scalar_64,
	vec2,
	vec3,
	vec4,
	vec2_64,
	vec3_64,
	vec4_64,
	array
};

template<typename T> struct VulkanType;
template<> struct VulkanType<float> { static constexpr auto type = ShaderType::scalar; };
template<> struct VulkanType<bool> { static constexpr auto type = ShaderType::scalar; };
template<> struct VulkanType<std::uint32_t> { static constexpr auto type = ShaderType::scalar; };
template<> struct VulkanType<std::int32_t> { static constexpr auto type = ShaderType::scalar; };
template<> struct VulkanType<double> { static constexpr auto type = ShaderType::scalar_64; };

template<typename T> using VulkanTypeT = typename VulkanType<T>::type;

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
		: data(&obj), size(sizeof(T)), offset(xoff) {}

	///Constructs the buffer data for a given container, which must correctly implement
	///a size and a data member function and must have its value type as first template param.
	///The container must be contigous.
	///Can be e.g. be used to create a BufferData object diretly from a std::vector or std::string.
	template<template<typename, typename...> typename C, typename T, typename... A>
	BufferData(const C<T, A...>& container, std::size_t xoff = 0)
		: data(container.data()), size(container.size() * sizeof(T)), offset(xoff) {}

	///Constructs the buffer data for a given array.
	template<typename T, std::size_t S>
	BufferData(const T (&array)[S])
		: data(array), size(S * sizeof(T)), type{typename VulkanType<T>::type, S} {}
};


class BufferFill
{
public:
	BufferFill(const Buffer& buffer);
	~BufferFill();

	void add(DataType type, const BufferData& data);
	void alignUniform();
	void alignStorage();
	void alignTexel();

	WorkPtr apply();

protected:
	const Buffer* buffer_ {};
	WorkPtr work_ {};
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
