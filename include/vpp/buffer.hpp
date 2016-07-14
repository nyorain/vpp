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
///Does return 0 for special values such as none, buffer, structure or matrix.
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

class BufferUpdate : public ResourceReference<BufferUpdate>
{
public:
	BufferUpdate(const Buffer& buffer, BufferAlign align = BufferAlign::std140,
		bool direct = false);
	~BufferUpdate();

	template<typename T> void addSingle(const T& obj);
	template<typename... T> void add(const T&... obj);

	void write(const void* ptr, std::size_t size);
	void offset(std::size_t size, bool update = true);
	void align(std::size_t align);

	void alignUniform();
	void alignStorage();
	void alignTexel();

	WorkPtr apply();

	const Buffer& buffer() const { return *buffer_; }
	std::size_t bufferOffset() const { return bufferOffset_; }
	std::size_t internalOffset() const { return internalOffset_; }
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
