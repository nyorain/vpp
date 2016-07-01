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

///Vulkan shader data types.
enum class ShaderType
{
	none, //used for things like e.g. containers
	buffer, //used for buffer that shall be plain copied
	scalar,
	scalar_64,
	vec2,
	vec3,
	vec4,
	vec2_64,
	vec3_64,
	vec4_64,
	mat4,
	mat3,
	mat2,
	structure
};

///Utility class for filling buffers with mulitple data segments.
class BufferData
{
public:
	const void* data = nullptr; //a pointer to the data
	std::size_t size = 0; //size of the given data
	std::size_t offset = 0; //additional relative (!) offset to the previous given data segment

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
			//type{VulkanType<T>::type, container.size()} {}
};

class BufferUpdate : public ResourceReference<BufferUpdate>
{
public:
	enum class Align
	{
		std140,
		std430
	};

public:
	BufferUpdate(const Buffer& buffer, Align align = Align::std140, bool direct = false);
	~BufferUpdate();

	template<typename T> void addSingle(const T& obj);
	template<typename... T> void add(const T&... obj);

	template<typename T, typename = std::enable_if_t<!std::is_pointer<T>::value>> void
	write(const T& obj, std::size_t count = 1, std::size_t align = 0);
	void write(const void* ptr, std::size_t size);

	void offset(std::size_t size, bool update = true);
	void align(std::size_t align);

	void alignUniform();
	void alignStorage();
	void alignTexel();

	WorkPtr apply();

	std::uint8_t& data();
	const Buffer& buffer() const { return *buffer_; }
	std::size_t bufferOffset() const { return bufferOffset_; }
	std::size_t internalOffset() const { return internalOffset_; }
	Align alignType() const { return align_; }

	bool std140() const { return align_ == Align::std140; }
	bool std430() const { return align_ == Align::std430; }

	const Buffer& resourceRef() const { return *buffer_; }

protected:
	const Buffer* buffer_ {};
	WorkPtr work_ {};
	Align align_;

	MemoryMapView map_ {}; //for mapping (buffer/transfer)
	std::vector<std::uint8_t> data_; //for direct copying
	std::vector<vk::BufferCopy> copies_; //for transfer (direct/transfer)

	std::size_t bufferOffset_ {};
	std::size_t internalOffset_ {};

	bool direct_ = false;
};

#include <vpp/bits/buffer.inl>

// class BufferRead
// {
// public:
// 	BufferRead(const Buffer& buffer);
// };
//
// void alignedWrite(std::uint8_t* ptr, const Range<BufferData>& datas);
// void alignedRead(const std::uint8_t* ptr, Range<BufferData>& datas);

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

	template<typename... T>
	WorkPtr fill2(BufferUpdate::Align align, const T&... args)
	{
		BufferUpdate update(*this, align);
		update.add(args...);
		return update.apply();
	}

	///Utilty shortcut for filling the buffer with data using the std140 layout.
	template<typename... T> WorkPtr
	fill140(const T&... args){ return fill2(BufferUpdate::Align::std140, args...); }

	///Utilty shortcut for filling the buffer with data using the std430 layout.
	template<typename... T> WorkPtr
	fill430(const T&... args){ return fill2(BufferUpdate::Align::std430, args...); }

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
