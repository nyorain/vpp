// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vpp/fwd.hpp>
#include <vpp/buffer.hpp>
#include <vpp/work.hpp>
#include <vpp/memoryMap.hpp>
#include <vpp/sharedBuffer.hpp>
#include <vpp/transferWork.hpp>
#include <vpp/util/span.hpp>
#include <vpp/util/allocation.hpp>
#include <vpp/util/tmp.hpp>
#include <vkpp/enums.hpp>
#include <cstring>

namespace vpp {

/// Specifies the different buffer alignment methods.
/// For the differences, read 
/// https://www.opengl.org/wiki/Interface_Block_(GLSL)#Memory_layout.
/// Uniform buffer are by default std140, storage buffers are by default std430.
/// Both defaults can be explicitly changed in the shader files using the buffers.
enum class BufferLayout {
	std140,
	std430
};

/// Base for classes that operator on a buffer such as Reader, Writer, Sizer.
/// Moves linearly over the buffer and somehow operates on it and the data it gets.
/// Uses the CRTP idiom.
template<typename B>
class BufferOperator {
public:
	constexpr BufferOperator(BufferLayout align) : layout_(align) {}
	~BufferOperator() = default;

	/// Utility function that calls addSingle for all ob the given objects.
	/// Usually this is all you need.
	template<typename... T>
	void add(T&&... obj);

	/// Will operator on the given object. 
	/// The type of the given object must have a specialization for the 
	/// VulkanType template struct that carriers information about
	/// the corresponding shader variable type of the object to align it correctly.
	/// The given object can also be a container/array of such types.
	/// If one wants the operator to just use raw data it can use vpp::raw for
	/// an object which will wrap it into a temporary RawBuffer object that 
	/// can also be operated on without any alignment or offsets.
	template<typename T>
	void addSingle(T&& obj);

	/// Returns the current offset on the buffer.
	constexpr auto offset() const noexcept { return offset_; }

	/// Sets the nextOffset value to which offset will be set before operating on the
	/// next data. Will not be applied if there is no further data to operate on.
	constexpr void nextOffset(vk::DeviceSize o) noexcept { nextOffset_ = o; }

	/// Sets the align that offset should have before operating on the next data.
	/// Will not be applied if there is no further data to operate on.
	constexpr void nextOffsetAlign(vk::DeviceSize algn) noexcept { 
		nextOffset_ = ::vpp::align(offset_, algn); 
	}

	/// Assures that the current position on the buffer meets the given 
	/// alignment requirements. If it does not, it will be changed to the next 
	/// value fulfilling the requirement.
	constexpr void align(vk::DeviceSize algn, bool update = true) { 
		auto delta = ::vpp::align(offset_, algn) - offset_;
		static_cast<B&>(*this).offset(delta, update);
	}

	constexpr BufferLayout layout() const noexcept { return layout_; }

	constexpr bool std140() const noexcept { 
		return layout_ == BufferLayout::std140; 
	}
	constexpr bool std430() const noexcept { 
		return layout_ == BufferLayout::std430; 
	}

	// Expected to exist in B:
	// void operate(const void* ptr, vk::DeviceSize size);
	// void offset(vk::DeviceSize, bool update = true);

protected:
	BufferLayout layout_;
	vk::DeviceSize offset_ {};

	// structs/arrays may have offset requirement for next written data.
	vk::DeviceSize nextOffset_ {}; 
};


// -- size api --
/// Can be used to calculate the size that would be needed to fit certain 
/// objects with certain alignments on a buffer.
/// Alternative name: BufferSizeCalculator
class BufferSizer : public BufferOperator<BufferSizer>, public Resource {
public:
	/// Token used to explicit construct a BufferSizer without device only
	/// for compile-time computation.
	struct CompileTimeTag {};
	static constexpr CompileTimeTag compileTime {};

public:
	/// Constructs a compile-time instance of BufferSizer.
	/// Cannot be used for dynamic alignment requirements (e.g. uniform offset).
	constexpr BufferSizer(CompileTimeTag, BufferLayout l) : 
		BufferOperator(l) {}

	/// Constructs a runtime instance of a BufferSizer.
	/// Can be used for dynamic alignment requirements like uniform, 
	/// storage or texel.
	BufferSizer(const Device&, BufferLayout);
	~BufferSizer() = default;

	using BufferOperator::add;
	template<typename... T> constexpr void add();

	constexpr void operate(const void*, vk::DeviceSize size);
	constexpr void offset(vk::DeviceSize size, bool = false) { offset_ += size; }

	using BufferOperator::offset;
	using BufferOperator::layout;
	using BufferOperator::std140;
	using BufferOperator::std430;

	void alignUniform() noexcept;
	void alignStorage() noexcept;
	void alignTexel() noexcept;
};

/// Calculates the size a vulkan buffer must have to be able to store all 
/// the given objects.
/// \sa BufferSizer
template<typename... T>
vk::DeviceSize neededBufferSize(BufferLayout align, const T&... args)
{
	BufferSizer sizer(BufferSizer::compileTime, align);
	sizer.add(args...);
	return sizer.offset();
}

/// Calculates the size a vulkan buffer must have to thold objects of the given types.
/// Can be computed at compile time. Note that this does not work for types which
/// does not have a constant size and alignment such as ShaderType::buffer or
/// ShaerType::custom with a non-constexpr size implementation.
/// \sa BufferSizer
template<typename... T>
constexpr vk::DeviceSize neededBufferSize(BufferLayout align)
{
	BufferSizer sizer(BufferSizer::compileTime, align);
	sizer.add<T...>();
	return sizer.offset();
}

template<typename... T> vk::DeviceSize neededBufferSize140(const T&... args) { 
	return neededBufferSize(BufferLayout::std140, args...); 
}

template<typename... T> constexpr vk::DeviceSize neededBufferSize140() { 
	return neededBufferSize<T...>(BufferLayout::std140); 
}

template<typename... T> vk::DeviceSize neededBufferSize430(const T&... args) { 
	return neededBufferSize(BufferLayout::std430, args...); 
}

template<typename... T> constexpr vk::DeviceSize neededBufferSize430() { 
	return neededBufferSize<T...>(BufferLayout::std430); 
}

// -- write api --
/// Can be used to write aligned data to a mapped buffer.
class MappedBufferWriter : 
	public BufferOperator<MappedBufferWriter>,
	public ResourceReference<MappedBufferWriter> {
public:
	MappedBufferWriter(MemoryMapView&& view, BufferLayout, 
		bool tight = false, vk::DeviceSize srcOffset = 0u);

	/// Writes size bytes from ptr to the buffer.
	/// Undefined behaviour if ptr does not point to at least size bytes
	/// or this write extends the range of the memory map.
	void operate(const void* ptr, vk::DeviceSize size);

	/// Offsets the current position on the buffer by size bytes. 
	/// If update is true, it will override the bytes with zero, otherwise they 
	/// will not be changed.
	void offset(vk::DeviceSize size, bool update = true);

	void alignUniform() noexcept;
	void alignStorage() noexcept;
	void alignTexel() noexcept;

	const auto& map() const noexcept { return view_; }
	auto& regions() noexcept { return regions_; }
	const auto& regions() const noexcept { return regions_; }

	const auto& resourceRef() const { return view_; }

protected:
	MemoryMapView view_;
	vk::DeviceSize viewOffset_ {};
	vk::DeviceSize srcOffset_ {};
	bool tight_ {};
	std::vector<vk::BufferCopy> regions_; // regions written
};

/// Can be used to write aligned data into raw memory, can
/// then be used to perform a direct update command.
class DirectBufferWriter : 
	public BufferOperator<DirectBufferWriter>,
	public ResourceReference<DirectBufferWriter> {
public:
	DirectBufferWriter(const Buffer& buf, BufferLayout);

	/// Writes size bytes from ptr to the buffer.
	/// Undefined behaviour if ptr does not point to at least size bytes
	/// or this write extends the range of the memory map.
	void operate(const void* ptr, vk::DeviceSize size);

	/// Offsets the current position on the buffer by size bytes. 
	/// If update is true, it will override the bytes with zero, otherwise they 
	/// will not be changed.
	void offset(vk::DeviceSize size, bool update = true);

	void alignUniform() noexcept;
	void alignStorage() noexcept;
	void alignTexel() noexcept;

	const auto& data() const noexcept { return data_; }
	const auto& copies() const noexcept { return copies_; }

	const auto& resourceRef() const { return buffer_; }

protected:
	const Buffer& buffer_;
	std::vector<std::byte> data_;
	std::vector<vk::BufferCopy> copies_; // regions to copy
};

namespace detail {

UploadWork apply(const Buffer&, BufferRange&& stage, 
	nytl::Span<const vk::BufferCopy>, QueueSubmitter&);
CommandWork<void> apply(const Buffer&, const DirectBufferWriter&, 
	QueueSubmitter&);
CommandBuffer copyCmdBuf(QueueSubmitter&, const Buffer& buffer,
	const BufferRange& stage, vk::DeviceSize offset, vk::DeviceSize size);

} // namespace detail

// TODO: assertions?

/// Uses a MappedBufferWriter to directly write the mappable buffer.
/// Undefined behavior if the buffer is not mappable.
/// The buffer must not be in use (you will probably need a pipeline barrier).
template<typename... T>
void writeMap(const Buffer& buf, BufferLayout layout, const T&... args) {
	MappedBufferWriter writer(buf.memoryMap(), layout);
	writer.map().flush();
	writer.add(args...);
}

template<typename... T>
void writeMap(const BufferRange& buf, BufferLayout layout, const T&... args) {
	MappedBufferWriter writer(buf.memoryMap(), layout);
	writer.map().flush();
	writer.add(args...);
}

template<typename B, typename... T>
void writeMap140(const B& buf, const T&... args) {
	writeMap(buf, BufferLayout::std140, args...);
}

template<typename B, typename... T>
void writeMap430(const B& buf, const T&... args) {
	writeMap(buf, BufferLayout::std430, args...);
}

/// Uses a staging buffer to write the given arguments with the
/// given layout into the given buffer. Will connect the uploadWork
/// to the given QueueSubmitter.
/// Note that the buffer must have been created with the transferDst
/// usage bit set. The buffer must not be in use (you will probably need 
/// a pipeline barrier).
/// The work must be finished before any resources are destroyed.
/// The passed arguments to write are not needed after the call returns.
template<typename... T>
UploadWork writeStaging(QueueSubmitter& qs, vk::DeviceSize offset, 
	const Buffer& buf, BufferLayout layout, const T&... args)
{
	auto size = neededBufferSize(layout, args...);
	auto stage = buf.device().bufferAllocator().alloc(true, size,
		vk::BufferUsageBits::transferSrc);
	MappedBufferWriter writer(stage.memoryMap(), layout, true, stage.offset());
	writer.offset(offset, false);
	writer.add(args...);
	writer.map().flush();
	return detail::apply(buf, std::move(stage), writer.regions(), qs);
}

template<typename... T>
auto writeStaging(QueueSubmitter& qs, const Buffer& buf, 
	BufferLayout layout, const T&... args) {
	return writeStaging(qs, 0u, buf, layout, args...);
}

template<typename... T>
auto writeStaging(QueueSubmitter& qs, const BufferRange& buf, 
	BufferLayout layout, const T&... args) 
{
	return writeStaging(qs, buf.offset(), buf.buffer(), layout, args...);
}

template<typename B, typename... T>
auto writeStaging(const B& buf, BufferLayout layout, const T&... args) {
	return writeStaging(buf.device().queueSubmitter(), buf, layout, args...);
}

template<typename B, typename... T>
auto writeStaging140(const B& buf, const T&... args) {
	return writeStaging(buf.device().queueSubmitter(), buf, 
		BufferLayout::std140, args...);
}

template<typename B, typename... T>
auto writeStaging430(const B& buf, const T&... args) {
	return writeStaging(buf.device().queueSubmitter(), buf, 
		BufferLayout::std430, args...);
}

/// Uses vk::cmdUpdateBuffer to update the given buffer directly with
/// the given arguments and layout. Should only be used (and only
/// works) for small updates, i.e. if the update size is smaller
/// than 2^16 bytes. The buffer must have been created with the transferDst
/// usage bit set. The buffer must not be in use (you will probably need 
/// a pipeline barrier).
/// The work must be finished before any resources are destroyed.
/// The passed arguments to write are not needed after the call returns.
template<typename... T>
CommandWork<void> writeDirect(QueueSubmitter& qs, vk::DeviceSize offset, 
	const Buffer& buf, BufferLayout layout, const T&... args)
{
	DirectBufferWriter writer(buf, layout);
	writer.offset(offset, false);
	writer.add(args...);
	return detail::apply(buf, writer, qs);
}

template<typename... T>
auto writeDirect(QueueSubmitter& qs, const Buffer& buf, 
	BufferLayout layout, const T&... args)
{
	return writeDirect(qs, 0u, buf, layout, args...);
}

template<typename... T>
auto writeDirect(QueueSubmitter& qs, const BufferRange& buf, 
	BufferLayout layout, const T&... args)
{
	return writeDirect(qs, buf.offset(), buf.buffer(), layout, args...);
}

template<typename B, typename... T>
auto writeDirect(const B& buf, BufferLayout layout, const T&... args) {
	return writeDirect(buf.device().queueSubmitter(), buf, layout, args...);
}

template<typename B, typename... T>
auto writeDirect140(const B& buf, const T&... args) {
	return writeDirect(buf.device().queueSubmitter(), buf, 
		BufferLayout::std140, args...);
}

template<typename B, typename... T>
auto writeDirect430(const B& buf, const T&... args) {
	return writeDirect(buf.device().queueSubmitter(), buf, 
		BufferLayout::std430, args...);
}

/// Class that can be used to read raw data into objects using the coorect alignment.
/// It is constructed with raw data and then can be used to read them into
/// the passed objects using the BufferOperator api.
class BufferReader : public BufferOperator<BufferReader>, public Resource {
public:
	/// Constructs the BuffeReader to read from the given data.
	/// \param data The raw data usually retrieved from the buffer to read.
	BufferReader(const Device&, BufferLayout, nytl::Span<const std::byte> data);
	~BufferReader() = default;

	void operate(void* ptr, vk::DeviceSize size);
	void offset(vk::DeviceSize size, bool = false) { offset_ += size; }

	using BufferOperator::offset;
	using BufferOperator::layout;
	using BufferOperator::std140;
	using BufferOperator::std430;

	void alignUniform() noexcept;
	void alignStorage() noexcept;
	void alignTexel() noexcept;

protected:
	nytl::Span<const std::byte> data_;
};

/// Reads the given buffer using the given layout into the given arguments
/// by mapping it. Undefined behvaior if the buffer is not mappable.
/// The buffer must not be in use (you will probably need a pipeline barrier).
/// If the given arguments don't match the data of the buffer you will
/// probably get garbage.
template<typename... T>
void readMap(const Buffer& buf, BufferLayout layout, T&... args) {
	auto map = buf.memoryMap();
	map.invalidate();
	BufferReader reader(buf.device(), layout, {map.ptr(), map.size()});
	reader.add(args...);
}

template<typename... T>
void readMap(const BufferRange& buf, BufferLayout layout, T&... args) {
	auto map = buf.memoryMap();
	map.invalidate();
	BufferReader reader(buf.device(), layout, {map.ptr(), map.size()});
	reader.add(args...);
}

template<typename B, typename... T>
void readMap140(const B& buf, T&... args) {
	return readMap(buf, BufferLayout::std140, args...);
}

template<typename B, typename... T>
void readMap430(const B& buf, T&... args) {
	return readMap(buf, BufferLayout::std430, args...);
}

/// Reads the given buffer using the given layout into the given arguments
/// by retrieving it from a temporary staging buffer.
/// The buffer must not be in use (you will probably need a pipeline barrier).
/// The buffer must have been created with the transferSrc usage bit set.
/// You have to assure that all resources (inclusive the passed argument
/// references) stay valid until the work is finished, then the data
/// was written into the referenced arguments.
/// If the given arguments don't match the data of the buffer you will
/// probably get garbage.
template<typename... T>
auto readStaging(QueueSubmitter& qs, vk::DeviceSize offset, 
	const Buffer& buf, BufferLayout layout, T&... args)
{
	auto size = neededBufferSize(layout, args...);
	auto stage = buf.device().bufferAllocator().alloc(true, size,
		vk::BufferUsageBits::transferDst);
	auto cmdBuf = detail::copyCmdBuf(qs, buf, stage, offset, size);

	class WorkImpl : public CommandWork<void> {
	public:
		WorkImpl(QueueSubmitter& qs, CommandBuffer&& cmdBuf, BufferRange&& stage, 
			BufferLayout layout, T&... xargs) :
				CommandWork(qs, std::move(cmdBuf)), stage_(std::move(stage)),
				layout_(layout), args_(xargs...)
		{
		}

		~WorkImpl() {
			tryFinish(*this, "readStaging");
		}

		void finish() override {
			CommandWork::finish();
			auto map = stage_.memoryMap();
			map.invalidate();
			BufferReader reader(stage_.device(), layout_, map.cspan());
			std::apply([&](auto&... args){ reader.add(args...); }, args_);
		}

		BufferRange stage_;
		BufferLayout layout_;
		std::tuple<T&...> args_;
	};

	return WorkImpl{qs, std::move(cmdBuf), std::move(stage),
		layout, args...};
}

template<typename... T>
auto readStaging(QueueSubmitter& qs, const Buffer& buf, 
	BufferLayout layout, T&... args) 
{
	return readStaging(qs, buf, 0u, layout, args...);
}

template<typename... T>
auto readStaging(QueueSubmitter& qs, const BufferRange& buf, 
	BufferLayout layout, T&... args) 
{
	return readStaging(qs, buf.offset(), buf.buffer(), layout, args...);
}

template<typename B, typename... T>
auto readStaging(const B& buf, BufferLayout layout, T&... args) {
	readStaging(buf.device().queueSubmitter(), buf, layout, args...);
}

template<typename B, typename... T>
auto readStaging140(const B& buf, T&... args) {
	return readStaging(buf.device().queueSubmitter(), buf, 
		BufferLayout::std140, args...);
}

template<typename B, typename... T>
auto readStaging430(const B& buf, T&... args) {
	return readStaging(buf.device().queueSubmitter(), buf, 
		BufferLayout::std430, args...);
}

/// Implementation of buffer operations and vukan types
#include <vpp/bits/vulkanTypes.inl>
#include <vpp/bits/bufferOps.inl>

} // namespace vpp
