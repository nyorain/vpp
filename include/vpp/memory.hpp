#pragma once

#include <vpp/vk.hpp>
#include <vpp/fwd.hpp>
#include <vpp/resource.hpp>

#include <memory>
#include <map>
#include <vector>

namespace vpp
{

using DeviceMemoryPtr = std::shared_ptr<DeviceMemory>;

///DeviceMemory class that keeps track of its allocated and freed areas.
// /Makes it easy to resuse memory as well as bind multiple memoryRequestors to one allocation.
class DeviceMemory : public Resource
{
public:
	struct Allocation
	{
		std::size_t offset {0};
		std::size_t size {0};
	};

	class Entry : public NonCopyable
	{
	protected:
		DeviceMemoryPtr memory_ {nullptr};
		Allocation allocation_ {};

	protected:
		void free();

	public:
		Entry() = default;
		Entry(DeviceMemoryPtr memory, const Allocation& alloc);
		~Entry();

		Entry(Entry&& other) noexcept;
		Entry& operator=(Entry&& other) noexcept;

		DeviceMemory& memory() const { return *memory_; };
		std::size_t offset() const { return allocation_.offset; };
		std::size_t size() const { return allocation_.size; }
	};

protected:
	std::vector<Allocation> allocations_ {}; //use sorted container!
	vk::DeviceMemory memory_ {};
	std::size_t size_ {};

	std::size_t typeIndex_ {};
	vk::MemoryPropertyFlags flags_ {};

public:
	DeviceMemory(const Device& dev, const vk::MemoryAllocateInfo& info);
	DeviceMemory(const Device& dev, std::uint32_t size, std::uint32_t typeIndex);
	DeviceMemory(const Device& dev, std::uint32_t size, vk::MemoryPropertyFlags flgs);
	~DeviceMemory();

	Allocation alloc(std::size_t size, std::size_t aligment);
	Allocation allocSpecified(std::size_t offset, std::size_t size);
	void free(const Allocation& alloc);

	std::size_t biggestBlock() const; //the biggest (continuously) allocatable block.
	std::size_t free() const; //how much is free at all
	std::size_t size() const;

	vk::DeviceMemory vkDeviceMemory() const { return memory_; }
	vk::MemoryPropertyFlags propertyFlags() const { return flags_; }
};

//Makes it possible to allocate a few vk::DeviceMemory objects for many buffers/images.
class DeviceMemoryAllocator : public Resource
{
protected:
	struct BufferRequirement
	{
		vk::Buffer requestor;
		vk::MemoryRequirements requirements;
		DeviceMemory::Entry* entry {nullptr};
		std::size_t offset {0}; //internal use in alloc
	};

	struct ImageRequirement
	{
		vk::Image requestor;
		vk::MemoryRequirements requirements;
		vk::ImageTiling tiling;
		DeviceMemory::Entry* entry {nullptr};
		std::size_t offset {0}; //internal use in alloc
	};

protected:
	std::map<unsigned int, std::vector<BufferRequirement>> bufferRequirements_;
	std::map<unsigned int, std::vector<ImageRequirement>> imageRequirements_;

public:
	DeviceMemoryAllocator(const Device& dev);
	~DeviceMemoryAllocator();

	void request(vk::Buffer requestor, const vk::MemoryRequirements& reqs,
		DeviceMemory::Entry& entry);
	void request(vk::Image requestor,  const vk::MemoryRequirements& reqs, vk::ImageTiling tiling,
		DeviceMemory::Entry& entry);

	void allocate();
};

///MemoryMap
class MemoryMap : public NonCopyable
{
protected:
	const DeviceMemory* memory_ {nullptr};
	std::size_t offset_ {0};
	std::size_t size_ {0};
	void* ptr_ {nullptr};

public:
	MemoryMap(const DeviceMemory& memory, std::size_t offset, std::size_t size);
	MemoryMap(const DeviceMemory& memory, const DeviceMemory::Allocation& alloc);
	MemoryMap(const DeviceMemory::Entry& entry);
	~MemoryMap();

	MemoryMap(MemoryMap&& other);
	MemoryMap& operator=(MemoryMap&& other);

	vk::DeviceMemory vkMemory() const { return memory_->vkDeviceMemory(); }

	std::size_t offset() const { return offset_; }
	std::size_t size() const { return size_; }
	void* ptr() const { return ptr_; }
	const DeviceMemory& memory() const { return *memory_; }

	void flushRanges() const;
	void invalidateRanges() const;
	void unmap();
};

///Memory Resource initializer.
template<typename T> class MemoryResourceInitializer
{
protected:
	bool valid_ {1};
	T resource_;

public:
	template<typename... Args>
	MemoryResourceInializer(Args&&... args) : resource_()
	{
		resource_.initMemoryLess(std::forward<Args>(args)...);
	};

	template<typename... Args>
	T init(Args&&... args)
	{
		if(!valid_) throw std::logic_error("Called MemoryResourceInitializer::init 2 times");

		valid_ = 0;
		resource_.initMemoryResource(std::forward<Args>(args)...);
		return std::move(resource_);
	}
};

}
