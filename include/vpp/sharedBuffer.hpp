#pragma once

#include <vpp/buffer.hpp>
#include <vpp/resource.hpp>
#include <deque>

namespace vpp {

// NOTE: it might be useful to go even one step further and add a new
//  SharedBuffer class that makes use of sparse memory bindings and
//  only binds those regions that are currently used by a BufferRange.
//  Will have different costs than this concept (and this is fine
//  in most cases, also sparse stuff is only sparingly (i'm sorry) supported)
//  so keep this separate.

// NOTE: also implement synchronized versions of these classes?
//  might be useful since often there are multiple threads e.g. 
//  uploading or initializing stuff. But again, implement as 
//  a separate version, only pay for what you use.

// can buffer offsets (like, ever) need alignments?
// neither vkCmdBindVertexBuffers nor VkDescriptorBufferInfo
// mention anything (as of 1.0.62) so i guess not.


/// Like vpp::Buffer but keeps track of sub-allocated BufferRanges.
/// Can be used to share a buffer.
/// See also BufferAllocator.
class SharedBuffer : public Buffer {
public:
	using Allocation = BasicAllocation<vk::DeviceSize>;

public:
	using Buffer::Buffer;
	~SharedBuffer();

	/// Tries to allocate a range with the given size.
	/// Returns an empty (i.e. size == 0) allocation if there
	/// is not enough free space left.
	Allocation alloc(vk::DeviceSize size);

	/// Frees the given allocation. Undefined behvaiour if the
	/// allocation is invalid (i.e. not allocated on this SharedBuffer).
	void free(const Allocation&);
	const auto& allocations() const { return allocations_; }

protected:
	// sorted by position for efficient allocation/release
	// NOTE: we might want to use set/list for more efficient insert/erase,
	//  worse iteration though. Would allow to only use a poiner in
	//  bufferRange intead of the full allocation object
	std::vector<Allocation> allocations_;
};

/// Sub-allocated range of a buffer.
/// See also SharedBuffer.
class BufferRange : public ResourceReference<BufferRange> {
public:
	using Allocation = SharedBuffer::Allocation;

public:
	BufferRange() = default;
	BufferRange(SharedBuffer&, vk::DeviceSize size);
	BufferRange(SharedBuffer&, const Allocation& allocation);
	~BufferRange();

	BufferRange(BufferRange&& rhs) noexcept { swap(*this, rhs); }
	BufferRange& operator=(BufferRange rhs) noexcept {
		swap(*this, rhs);
		return *this;
	}

	const SharedBuffer& buffer() const { return *shared_; }
	const Allocation& allocation() const { return allocation_; }
	vk::DeviceSize offset() const { return allocation().offset; }
	vk::DeviceSize size() const { return allocation().size; }

	bool mappable() const noexcept { return buffer().mappable(); }
	MemoryMapView memoryMap() const;

	const SharedBuffer& resourceRef() const { return *shared_; }
	friend void swap(BufferRange&, BufferRange&) noexcept;

protected:
	SharedBuffer* shared_ {};
	Allocation allocation_;
};

// movable? 

/// Allocates BufferRanges on owned SharedBuffers.
/// Useful to reuse (espeically short-lived) buffers like staging
/// transfer buffers or small buffers like ubos.
class BufferAllocator : public vpp::Resource {
public:
	BufferAllocator(const Device& dev);

	/// Reserves the given requirements.
	/// The next time alloc is called, these are initialized additionally.
	/// Can be used to group alloc groups together in a 
	/// deferred-initialization-like manner.
	/// This does not automatically reserve anything, only
	/// causes the next buffer that has to be created and matches
	/// the given requirements to be larger (using the size parameter).
	void reserve(vk::DeviceSize size, vk::BufferUsageFlags, 
		unsigned int memBits = ~0u);
	void reserve(vk::DeviceSize size, vk::BufferUsageFlags, 
		vk::MemoryPropertyFlags memProps);

	/// Allocates a buffer range with the given requirements.
	BufferRange alloc(vk::DeviceSize size, vk::BufferUsageFlags, 
		unsigned int memBits = ~0u);
	BufferRange alloc(vk::DeviceSize size, vk::BufferUsageFlags, 
		vk::MemoryPropertyFlags memProps);

	/// Optimizes the buffer allocations. Will recreate all unused buffers
	/// as one big buffer. Makes sense to call when no/few buffer rangers
	/// are allocated.
	void optimize();

	/// Releases all currently unused shared buffers.
	void shrink();

protected:
	struct Requirement {
		std::size_t size {};
		vk::BufferUsageFlags usage {};
		unsigned int memBits {};
	};

	struct Buffer {
		SharedBuffer buffer;
		vk::BufferUsageFlags usage;
	};

	std::deque<Buffer> buffers_;
	std::vector<Requirement> reqs_;
};

} // namespace vpp