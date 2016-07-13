#pragma once

#include <vpp/fwd.hpp>
#include <vpp/resource.hpp>
#include <vpp/memory.hpp>
#include <vpp/utility/range.hpp>
#include <vpp/utility/allocation.hpp>

#include <vpp/utility/pmr/map.hpp>
#include <vpp/utility/pmr/vector.hpp>

#include <memory>
#include <vector>

namespace vpp
{

///Makes it possible to allocate a few vk::DeviceMemory objects for many buffers/images.
///Basically a memory pool. Can be used manually, but since the buffer and image (memoryResource)
///classes deal with it theirselfs, it is usually not required.
///The api is nontheless exposed publicly.
class DeviceMemoryAllocator : public Resource
{
public:
	DeviceMemoryAllocator() = default;
	DeviceMemoryAllocator(const Device& dev);
	~DeviceMemoryAllocator();

	DeviceMemoryAllocator(DeviceMemoryAllocator&& other) noexcept;
	DeviceMemoryAllocator& operator=(DeviceMemoryAllocator other) noexcept;

	///Requests memory for the given vulkan buffer and stores a (pending) reference to it into
	///the given entry.
	void request(vk::Buffer requestor, const vk::MemoryRequirements& reqs,
		vk::BufferUsageFlags usage, MemoryEntry& entry);

	///Requests memory for the given vulkan image and stores a (pending) reference to it into
	///the given entry. It additionally requires the tiling of the image to fulfill vulkans
	///granularity requiremens.
	void request(vk::Image requestor, const vk::MemoryRequirements& reqs, vk::ImageTiling tiling,
		MemoryEntry& entry);

	///Removes the (pending) request from this allocator. Returns false if the given entry could
	///not be found.
	bool removeRequest(const MemoryEntry& entry);

	///This function will be called when a stored entry is moved.
	///Will return false if the given entry is not found.
	bool moveEntry(const MemoryEntry& oldOne, MemoryEntry& newOne);

	///Allocates and associated device memory for all pending requests.
	void allocate();

	///Makes sure that the given entry has associated memory.
	///Will return false if the given entry cannot be found.
	bool allocate(const MemoryEntry& entry);

	///Returns all memories that this allocator manages.
	std::vector<DeviceMemory*> memories() const;

	friend void swap(DeviceMemoryAllocator& a, DeviceMemoryAllocator& b) noexcept;

protected:
	enum class RequirementType
	{
		linearImage,
		optimalImage,
		buffer
	};

	struct Requirement
	{
		RequirementType type {};
		vk::DeviceSize size {};
		vk::DeviceSize alignment {};
		std::uint32_t memoryTypes {};
		MemoryEntry* entry {};
		union { vk::Buffer buffer; vk::Image image; }; //type determines which is active
	};

	using Requirements = std::vector<Requirement>;

protected:
	static AllocationType toAllocType(RequirementType reqType);
	static bool supportsType(const Requirement& req, unsigned int type);
	static bool supportsType(std::uint32_t bits, unsigned int type);

	//utility allocation functions
	void allocate(unsigned int type);
	void allocate(unsigned int type, const Range<Requirement*>& requirements);
	DeviceMemory* findMem(Requirement& req);
	Requirements::iterator findReq(const MemoryEntry& entry);
	spm::map<unsigned int, std::pmr::vector<Requirement*>> queryTypes();
	unsigned int findBestType(std::uint32_t typeBits) const;

protected:
	Requirements requirements_;
	std::vector<std::unique_ptr<DeviceMemory>> memories_;
};

///Represents an entry on a vulkan device memory which will be dynamically, asynchronously
///allocated by a DeviceMemoryAllocator.
class MemoryEntry : public ResourceReference<MemoryEntry>
{
public:
	MemoryEntry() = default;
	MemoryEntry(DeviceMemory& memory, const Allocation& alloc);
	~MemoryEntry();

	MemoryEntry(MemoryEntry&& other) noexcept;
	MemoryEntry& operator=(MemoryEntry other) noexcept;

	///Will try to map the Memory and return a view to the location where this entry is placed.
	///Throws a std::logic_error if the DeviceMemory is not mappable.
	MemoryMapView map() const;

	///Returns whether memory on the device was allocated for this entry.
	bool allocated() const { return (allocation_.size > 0); }

	///Assures that there is memory allocated and associated with this entry.
	void allocate() const { if(!allocated()) allocator_->allocate(*this); }

	DeviceMemory* memory() const { return allocated() ? memory_ : nullptr; };
	DeviceMemoryAllocator* allocator() const { return allocated() ? nullptr : allocator_; };
	std::size_t offset() const { return allocation_.offset; };
	std::size_t size() const { return allocation_.size; }
	const Allocation& allocation() const { return allocation_; }

	Resource& resourceRef() const { if(allocated()) return *memory_; else return *allocator_; }
	friend void swap(MemoryEntry& a, MemoryEntry& b) noexcept;

protected:
	friend class DeviceMemoryAllocator;

	//if there is an allocation associated with this entry (the allocation size is > 0)
	//the memory member wil be active, otherwise the allocator.
	//so by default the allocator_ var will simple hold a nullptr and the allocation an empty
	//allocation {0, 0}. The allocation signals that is it not yet allocated and the
	//nullptr allocator var that it is invalid (i.e. not yet associated with an allocator).
	union
	{
		DeviceMemoryAllocator* allocator_ {};
		DeviceMemory* memory_;
	};

	Allocation allocation_ {};
};

}
