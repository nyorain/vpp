// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vpp/fwd.hpp>
#include <vpp/buffer.hpp>
#include <vpp/work.hpp>
#include <vpp/util/span.hpp>
#include <vpp/util/allocation.hpp>
#include <vpp/bits/apply.inl>

namespace vpp {

/// Vulkan shader data types.
/// Defines all possible types that can be passed as buffer update paramter.
/// See bits/vulkanTypes.inl for more information.
enum class ShaderType {
	none, // used for things like e.g. containers
	buffer, // used for raw data buffers that shall be plain copied
	scalar, // scalar like float or int, VulkanType has "size64" bool member
	vec, // VulkanType has "dimension" "size64" members
	mat, // VulkanType has "major" "minor" "transpose" "size64" members
	structure, // VulkanType has "member" tuples with member pointers and a bool "align"
	custom // Vulkan type holds "impl" type that will be called instead. See BufferApplier
};

/// Specifies the different buffer alignment methods.
/// For the differences, read https://www.opengl.org/wiki/Interface_Block_(GLSL)#Memory_layout.
/// Uniform buffer are by default std140 while storage buffers are by default std430.
/// Both defaults can be explicitly changed in the shader files using the buffers.
enum class BufferLayout {
	std140,
	std430
};

/// Base for classes that operator on a buffer such as BufferUpdate, BufferReader or BufferSizer.
/// Moves linearly over the buffer and somehow operates on it and the data it gets.
/// Uses the CRTP idiom.
template<typename B>
class BufferOperator {
public:
	constexpr BufferOperator(BufferLayout align) : align_(align) {}
	~BufferOperator() = default;

	/// Will operator on the given object. The type of the given object must have
	/// a specialization for the VulkanType template struct that carriers information about
	/// the corresponding shader variable type of the object to align it correctly.
	/// The given object can also be a container/array of such types.
	/// If one wants the operator to just use raw data it can use vpp::raw for
	/// an object which will wrap it into a temporary RawBuffer object that can also be operated
	/// on without any alignment or offsets.
	template<typename T>
	void addSingle(T&& obj);

	/// Utility function that calls addSingle for all ob the given objects.
	template<typename... T>
	void add(T&&... obj);

	/// Returns the current offset on the buffer.
	constexpr std::size_t offset() const { return offset_; }

	/// Sets the nextOffset value to which offset will be set before operating on the
	/// next data. Will not be applied if there is no further data to operate on.
	constexpr void nextOffset(std::size_t noffset) { nextOffset_ = noffset; }

	/// Sets the align that offset should have before operating on the next data.
	/// Will not be applied if there is no further data to operate on.
	constexpr void nextOffsetAlign(std::size_t align) { nextOffset_ = vpp::align(offset_, align); }

	constexpr BufferLayout alignType() const { return align_; }
	constexpr bool std140() const { return align_ == BufferLayout::std140; }
	constexpr bool std430()const { return align_ == BufferLayout::std430; }

protected:
	BufferLayout align_;
	std::size_t offset_ {};
	std::size_t nextOffset_ {}; // structs or arrays may have offset requirement for next member.
};

/// This class can be used to update/write the contents of a buffer.
/// It will automatically align the data as specified but can also be used to just upload/write
/// raw data to the buffer. Can also be used to specify custom offsets or alignment for the data.
class BufferUpdate : public BufferOperator<BufferUpdate>, public ResourceReference<BufferUpdate> {
public:
	/// The given align type will influence the applied alignments.
	/// \param direct Specifies if direct updates should be preferred. Will be ignored
	/// if the buffer is filled by mapping and in some cases, direct filling is not possible.
	/// \exception std::runtime_error if the device has no queue that supports graphics/compute or
	/// transfer operations and the buffer is not mappable.
	/// \sa BufferAlign
	BufferUpdate(const Buffer& buffer, BufferLayout align, bool direct = false);

	/// Calls apply and waits for the work to finish if apply was not called during
	/// the lifetime of this object.
	~BufferUpdate();

	/// Writes size bytes from ptr to the buffer.
	/// Undefined behaviour if ptr does not point to at least size bytes.
	void operate(const void* ptr, std::size_t size);

	/// Offsets the current position on the buffer by size bytes. If update is true, it will
	/// override the bytes with zero, otherwise they will not be changed.
	void offset(std::size_t size, bool update = true);

	/// Writes the stored data to the buffer.
	/// Returns a Work implemention that can be used to execute or defer the update.
	/// After this call, the BufferUpdate object should no longer be used in any way.
	WorkPtr apply();

	/// Assures that the current position on the buffer meets the given alignment requirements.
	/// If it does not, it will be changed to the next value fulfilling the requirement and
	/// not changing the buffer content in this section.
	void align(std::size_t align);

	/// Aligns the offset for a uniform buffer.
	void alignUniform();

	/// Aligns the offset for a storage buffer.
	void alignStorage();

	/// Aligns the offset for a texel buffer.
	void alignTexel();

	/// Returns the internal offset, i.e. the position on the internal stored data.
	/// This value is usually not from any interest, See BufferOperator::offset for
	/// the current offset on the buffer data.
	std::size_t internalOffset() const { return internalOffset_; }

	const Buffer& buffer() const { return *buffer_; }

	using BufferOperator::offset;
	using BufferOperator::alignType;
	using BufferOperator::std140;
	using BufferOperator::std430;

	const Buffer& resourceRef() const { return *buffer_; }

protected:
	void checkCopies();
	std::uint8_t& data();

protected:
	const Buffer* buffer_ {};
	WorkPtr work_ {};

	MemoryMapView map_ {}; //for mapping (buffer/transfer)
	std::vector<std::uint8_t> data_; //for direct copying
	std::vector<vk::BufferCopy> copies_; //for transfer (direct/transfer)
	std::size_t internalOffset_ {};

	bool direct_ = false;
};

///Can be used to calculate the size that would be needed to fit certain objects with certain
///alignments on a buffer.
///Alternative name: BufferSizeCalculator
class BufferSizer : public BufferOperator<BufferSizer>, public Resource
{
public:
	constexpr BufferSizer(BufferLayout layout) : BufferOperator(layout) {}
	BufferSizer(const Device& dev, BufferLayout align) : BufferOperator(align), Resource(dev) {}
	~BufferSizer() = default;

	using BufferOperator::add;
	template<typename... T> constexpr void add();

	constexpr void operate(const void*, std::size_t size);

	constexpr void offset(std::size_t size) { offset_ += size; }
	constexpr void align(std::size_t align) { offset_ = vpp::align(offset_, align); }

	using BufferOperator::offset;
	using BufferOperator::alignType;
	using BufferOperator::std140;
	using BufferOperator::std430;

	void alignUniform();
	void alignStorage();
	void alignTexel();
};

///Class that can be used to read raw data into objects using the coorect alignment.
class BufferReader : public BufferOperator<BufferReader>, public Resource
{
public:
	//XXX: better use owned data (a vector, data copy) here?
	///Constructs the BufferReader with the given data range.
	///Note that the range must stay valid until destruction.
	BufferReader(const Device& dev, BufferLayout align, nytl::Span<uint8_t> data);
	~BufferReader() = default;

	void operate(void* ptr, std::size_t size);

	void offset(std::size_t size) { align(0); offset_ += size; }
	void align(std::size_t align) { offset_ = vpp::align(offset_, align); }

	using BufferOperator::offset;
	using BufferOperator::alignType;
	using BufferOperator::std140;
	using BufferOperator::std430;

	void alignUniform();
	void alignStorage();
	void alignTexel();

protected:
	nytl::Span<std::uint8_t> data_;
};

///Fills the buffer with the given data.
///Does this either by memory mapping the buffer or by copying it via command buffer.
///Expects that buffer was created fillable, so either the buffer is memory mappable or
///it is allowed to copy data into it and the device was created with a matching queue.
///Note that this operation may be asnyc, therefore a work unique ptr is returned.
///If multiple (device local) buffers are to fill it brings usually huge performace gains
///to first call fill() on them all and then make sure that the needed work finishes.
///\param buf The Buffer to fill.
///\param align Specifies the align of the data to update (either std140 or std430).
///\param args The arguments to fill the buffer with. Notice that this function is just a
///utility wrapper around the BufferUpdate class (which may be used for more detailed updates)
///and therefore expects all given arguments to have a specialization for the VulkanType
///template which is used to specify their matching shader type.
///If raw (un- or custom-aligned) data should be written into the buffer, the vpp::raw() function
///can be used.
///The given arguments must only be valid until this function finishes, i.e. they can go out
///of scope before the returned work is finished.
///\sa BufferUpdate
///\sa ShaderType
///\sa fill140
///\sa fill430
template<typename... T>
WorkPtr fill(const Buffer& buf, BufferLayout align, const T&... args)
{
	BufferUpdate update(buf, align);
	update.add(args...);
	return update.apply();
}

///Utilty shortcut for filling the buffer with data using the std140 layout.
///\sa fill
///\sa BufferUpdate
template<typename... T> WorkPtr fill140(const Buffer& buf, const T&... args)
	{ return fill(buf, BufferLayout::std140, args...); }

///Utilty shortcut for filling the buffer with data using the std430 layout.
///\sa fill
///\sa BufferUpdate
template<typename... T> WorkPtr fill430(const Buffer& buf, const T&... args)
	{ return fill(buf, BufferLayout::std430, args...); }

///Retrives the data stored in the buffer.
///\param size The size of the range to retrive. If size is vk::wholeSize (default) the range
///from offset until the end of the buffer will be retrieved.
///\return A Work ptr that is able to retrive an array of std::uint8_t values storing the data.
DataWorkPtr retrieve(const Buffer& buf, vk::DeviceSize offset = 0,
	vk::DeviceSize size = vk::wholeSize);

///Reads the data stored in the given buffer aligned into the given objects.
///Note that the given objects MUST remain valid until the work finishes.
///You can basically pass all argument types that you can pass to the fill command.
///Internally just uses a combination of a retrieve work operation and the reads the
///retrieved data into the given arguments using the BufferReader class.
///\sa BufferReader
template<typename... T>
WorkPtr read(const Buffer& buf, BufferLayout align, T&... args);

///Reads the given buffer into the given objects using the std140 layout.
///\sa read
///\sa BufferReader
template<typename... T> WorkPtr read140(const Buffer& buf, T&... args)
	{ return read(buf, BufferLayout::std140, args...); }

///Reads the given buffer into the given objects using the std430 layout.
///\sa read
///\sa BufferReader
template<typename... T> WorkPtr read430(const Buffer& buf, T&... args)
	{ return read(buf, BufferLayout::std430, args...); }


//TODO: constexpr where possible. One does not have to really pass the objects in most cases
//(except raw buffers and containers.)

///Calculates the size a vulkan buffer must have to be able to store all the given objects.
///\sa BufferSizer
template<typename... T>
std::size_t neededBufferSize(BufferLayout align, const T&... args)
{
	BufferSizer sizer(align);
	sizer.add(args...);
	return sizer.offset();
}

template<typename... T>
constexpr std::size_t neededBufferSize(BufferLayout align)
{
	BufferSizer sizer(align);
	sizer.add<T...>();
	return sizer.offset();
}

///Calcualtes the size a vulkan buffer must have to be able to store all the given objects using
///the std140 layout.
///\sa neededBufferSize
///\sa BufferSizer
template<typename... T> std::size_t neededBufferSize140(const T&... args)
	{ return neededBufferSize(BufferLayout::std140, args...); }

template<typename... T> constexpr std::size_t neededBufferSize140()
	{ return neededBufferSize<T...>(BufferLayout::std140); }

///Calcualtes the size a vulkan buffer must have to be able to store all the given objects using
///the std430 layout.
///\sa neededBufferSize
///\sa BufferSizer
template<typename... T> std::size_t neededBufferSize430(const T&... args)
	{ return neededBufferSize(BufferLayout::std430, args...); }

template<typename... T> constexpr std::size_t neededBufferSize430()
	{ return neededBufferSize<T...>(BufferLayout::std430); }

///Implementation of buffer operations and vukan types
#include <vpp/bits/vulkanTypes.inl>
#include <vpp/bits/bufferOps.inl>

/*

//simple example for the size/fill/read api:
//does not use the features for which the more comlex OO-api is needed such as
//custom alignments and offsets.

//query the size needed for a buffer in layout std140 with the given objects.
//note that for all of these calls you have to specialize the VulkanType template correectly
//for the given types (except the rawBuffer if gotten from vpp::raw and the container).
auto size = vpp::neededBufferSize140(dev, myvec2, myvec3, mymat3, myvector, myrawdata);
auto buffer = createBuffer(dev, size);

//fill the data with the buffer
//Usually one would one finish this work in place but batch it together with other work
//can be done using a vpp::WorkManager.
vpp::fill140(dev, myvec2, myvec3, mymat3, myvector, myrawdata)->finish();

//some operations/shader access here that changes the data of the buffer

//read the data back in
//the first call retrieves the data the second one parses it/reads it into the given variables.
//Usually one would not finish the work in place to retrieve the data.
auto data = vpp::retrieve(buffer)->data();
vpp::read140(dev, data, myvec2, myvec3, mymat3, myvector, myrawdata);

*/

}
