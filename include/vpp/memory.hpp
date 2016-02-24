#pragma once

#include <vpp/vk.hpp>
#include <vpp/fwd.hpp>
#include <vpp/resource.hpp>
#include <memory>

namespace vpp
{

using MemoryPtr = std::shared_ptr<Memory>;

///DeviceMemory class that keeps track of its allocated and freed areas.
///Makes it easy to resuse memory as well as bind multiple memoryRequestors to one allocation.
class Memory : public Resource
{
public:
	struct Allocation
	{
		std::size_t offset;
		std::size_t size;
	};

	class Entry : public NonCopyable
	{
	protected:
		MemoryPtr memory_;
		const Allocation* allocation_;

	public:
		Entry(MemoryPtr memory, const Allocation& alloc);
		~Entry();

		Memory& memory() const { return *memory_; };
		std::size_t offset() const { return allocation_->offset; };
		std::size_t size() const { return allocation_->size; }
	};

protected:
	std::vector<Allocation> entries_ {};
	vk::DeviceMemory memory_ {};
	std::size_t size_ {};

	std::size_t typeIndex_ {};
	vk::MemoryPropertyFlags flags_ {};

public:
	Memory(const Device& dev, const vk::MemoryAllocateInfo& info);
	Memory(const Device& dev, std::uint32_t size, std::uint32_t typeIndex);
	Memory(const Device& dev, std::uint32_t size, vk::MemoryPropertyFlags flags);
	~Memory();

	const Allocation* alloc(std::size_t size);
	void free(const Allocation& alloc);

	std::size_t biggestBlock() const; //the biggest (continuously) allocatable block.
	std::size_t free() const; //how much is free at all
	std::size_t size() const;

	vk::MemoryPropertyFlags propertyFlags() const { return flags_; }
};

//Makes it possible to allocate a few vk::DeviceMemory objects for many buffers/images.
class MemoryAllocator : public Resource
{
protected:
	struct BufferRequirement
	{
		vk::Buffer requestor;
		vk::MemoryRequirements requirements;
		Memory::Entry* entry {nullptr};
	};

	struct ImageRequirement
	{
		vk::Image requestor;
		vk::MemoryRequirements requirements;
		Memory::Entry* entry {nullptr};
	};

protected:
	std::vector<BufferRequirement> bufferRequirements_;
	std::vector<ImageRequirement> imageRequirements_;

public:
	MemoryAllocator(const Device& dev);
	~MemoryAllocator();

	void request(vk::Buffer requestor, const vk::MemoryRequirements& reqs,
		Memory::Entry* entry = nullptr);
	void request(vk::Image requestor,  const vk::MemoryRequirements& reqs,
		Memory::Entry* entry = nullptr);

	void allocate();
};

///MemoryMap
class MemoryMap : public NonCopyable
{
protected:
	const Memory* memory_;
	std::size_t offset_ {0};
	const std::size_t size_ {0};
	void* const ptr_ {nullptr};

public:
	MemoryMap(const Memory& memory, std::size_t offset, std::size_t size);
	~MemoryMap();

	vk::Memory vkMemory() const { return memory_; }
	vk::Device vkDevice() const { return device_; }

	std::size_t offset() const { return offset_; }
	std::size_t size() const { return size_; }
	void* ptr() const { return ptr_; }
	const Memory& memory() const { return memory_; }

	void flushRanges() const;
	void invalidateRanges() const;
	void unmap();
};

}
