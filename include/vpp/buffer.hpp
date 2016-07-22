#pragma once

#include <vpp/fwd.hpp>
#include <vpp/memoryResource.hpp>
#include <vpp/work.hpp>
#include <vpp/utility/range.hpp>

#include <memory>
#include <type_traits>
#include <cstring>
#include <cmath>

namespace vpp
{

///Representing a vulkan buffer on a device.
///Can be filled and read, and stores a handle to the memory location it is allocated on (or
///the allocator that will take care of its allocation).
///Does not store additional information such as buffer usage type or memory layout, this
///must be handled by the application for best performance.
class Buffer : public MemoryResource<vk::Buffer>
{
public:
	Buffer() = default;
	Buffer(const Device& dev, const vk::BufferCreateInfo& info, std::uint32_t memoryTypesBits);
	Buffer(const Device& dev, const vk::BufferCreateInfo& info,
		vk::MemoryPropertyFlags mflags = {});
	~Buffer();

	Buffer(Buffer&& other) noexcept = default;
	Buffer& operator=(Buffer&& other) noexcept = default;
};

///Vulkan shader data types.
///Defines all possible types that can be passed as buffer update paramter.
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
	matrix, //Vulkan type has additional "major" and "minor" and "transpose" members
	structure //Vulkan type has additional "member" tuples with member pointers
};

///Returns the alignment the given ShaderType requires.
///Returns 0 for special values such as none, buffer, structure or matrix.
constexpr unsigned int align(ShaderType type);

///Specifies the different buffer alignment methods.
///For the differences, read https://www.opengl.org/wiki/Interface_Block_(GLSL)#Memory_layout.
///Uniform buffer are by default std140 while storage buffers are by default std430.
///Both defaults can be explicitly changed in the shader files using the buffers.
enum class BufferAlign
{
	std140,
	std430
};

///This class can be used to update the contents of a buffer.
///It will automatically align the data as specified but can also be used to just upload/write
///raw data to the buffer. Can also be used to specify custom offsets or alignment for the data.
class BufferUpdate : public ResourceReference<BufferUpdate>
{
public:
	///The given align type will influence the applied alignments.
	///\param direct Specifies if direct updates should be preferred. Will be ignored
	///if the buffer is filled by mapping and in some cases, direct filling is not possible.
	///\exception std::runtime_error if the device has no queue that supports graphics/compute or
	///transfer operations and the buffer is not mappable.
	///\sa BufferAlign
	BufferUpdate(const Buffer& buffer, BufferAlign align, bool direct = false);
	~BufferUpdate();

	///Adds a single object to the buffer updates data. The type of the given object must have
	///a specialization for the VulkanType template struct that carriers information about
	///the corresponding shader variable type of the object to align it correctly.
	///The given object can also be a container/array of such types.
	///If one just wants to fill the buffer with raw data it can use to vpp::raw function for
	///a object which will wrap it into a temporary RawBuffer object that will just be copied
	///to the buffer without any alignment or offset.
	template<typename T> void addSingle(const T& obj);

	///Utility function that calls addSingle for all ob the given objects.
	template<typename... T> void add(const T&... obj);

	///Writes size bytes from ptr to the buffer.
	void write(const void* ptr, std::size_t size);

	///Offsets the current position on the buffer by size bytes. If update is true, it will
	///override the bytes with zero, otherwise they will not be changed.
	void offset(std::size_t size, bool update = true);

	///Assure that the current position on the buffer meets the given alignment requirements.
	void align(std::size_t align);

	void alignUniform();
	void alignStorage();
	void alignTexel();

	WorkPtr apply();

	///Returns the bufferOffset, i.e. the current position on the operating buffer.
	std::size_t bufferOffset() const { return bufferOffset_; }

	///Returns the internal offset, i.e. the position on the internal stored data.
	std::size_t internalOffset() const { return internalOffset_; }

	const Buffer& buffer() const { return *buffer_; }
	BufferAlign alignType() const { return align_; }

	bool std140() const { return align_ == BufferAlign::std140; }
	bool std430() const { return align_ == BufferAlign::std430; }

	const Buffer& resourceRef() const { return *buffer_; }

protected:
	void checkCopies();
	std::uint8_t& data();

protected:
	const Buffer* buffer_ {};
	WorkPtr work_ {};
	BufferAlign align_;

	MemoryMapView map_ {}; //for mapping (buffer/transfer)
	std::vector<std::uint8_t> data_; //for direct copying
	std::vector<vk::BufferCopy> copies_; //for transfer (direct/transfer)

	std::size_t bufferOffset_ {};
	std::size_t internalOffset_ {};

	bool direct_ = false;
};

#include <vpp/bits/vulkanTypes.inl>
#include <vpp/bits/buffer.inl>

///TODO: function for aligned buffer reading
//template<typename... T>
//void alignedRead(const std::uint8_t& data, T... args);


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
///can be used
///\sa BufferUpdate
///\sa ShaderType
///\sa fill140
///\sa fill430
template<typename... T>
WorkPtr fill(const Buffer& buf, BufferAlign align, const T&... args)
{
	BufferUpdate update(buf, align);
	update.add(args...);
	return update.apply();
}

///Utilty shortcut for filling the buffer with data using the std140 layout.
///\sa fill
///\sa BufferUpdate
template<typename... T> WorkPtr
fill140(const Buffer& buf, const T&... args){ return fill(buf, BufferAlign::std140, args...); }

///Utilty shortcut for filling the buffer with data using the std430 layout.
///\sa fill
///\sa BufferUpdate
template<typename... T> WorkPtr
fill430(const Buffer& buf, const T&... args){ return fill(buf, BufferAlign::std430, args...); }

///Retrives the data stored in the buffer.
///\param size The size of the range to retrive. If size is vk::wholeSize (default) the range
///from offset until the end of the buffer will be retrieved.
///\return A Work ptr that is able to retrive an array of std::uint8_t values storing the data.
DataWorkPtr retrieve(const Buffer& buf, vk::DeviceSize offset = 0,
	vk::DeviceSize size = vk::wholeSize);

};
