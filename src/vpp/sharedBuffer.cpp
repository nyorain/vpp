#include <vpp/sharedBuffer.hpp>
#include <dlg/dlg.hpp>
#include <algorithm>

namespace vpp {

// BufferRange
BufferRange::BufferRange(SharedBuffer& buf, vk::DeviceSize size)
{
	dlg_assert(buf.vkHandle());

	auto alloc = buf.alloc(size);
	if(!alloc.size) {
		throw std::runtime_error("BufferRange: not enough space on shared buffer");
	}

	shared_ = &buf;
	allocation_ = alloc;
}

BufferRange::BufferRange(SharedBuffer& buf, const Allocation& alloc) :
	shared_(&buf), allocation_(alloc)
{
	dlg_assert(buf.vkHandle());
	dlg_assert(alloc.size != 0);
}

BufferRange::~BufferRange()
{
	if(shared_) {
		shared_->free(allocation_);
	}
}

MemoryMapView BufferRange::memoryMap() const
{
	// buffer().memoryMap will already warn if buffer is mappable
	return buffer().memoryMap(offset(), size());
}

void swap(BufferRange& a, BufferRange& b) noexcept
{
	using RR = ResourceReference<BufferRange>;
	using std::swap;
	swap(static_cast<RR&>(a), static_cast<RR&>(b));
	swap(a.shared_, b.shared_);
	swap(a.allocation_, b.allocation_);
}

// SharedBuffer
SharedBuffer::~SharedBuffer()
{
	dlg_assertm(allocations_.empty(), "~SharedBuffer: allocations left");
}

SharedBuffer::Allocation SharedBuffer::alloc(vk::DeviceSize size)
{
	// TODO: can be improved
	Allocation old = {0, 0};

	for(auto it = allocations_.begin(); it != allocations_.end(); ++it) {
		if(it->offset - old.end() >= size) {
			Allocation range = {old.end(), size};

			// inserts the tested range before the higher range, if there is any
			allocations_.insert(it, range);
			return range;
		}

		old = *it;
	}

	// last
	if(memorySize() - old.end() >= size) {
		Allocation range = {old.end(), size};
		allocations_.push_back(range);
		return range;
	}

	return {};
}

void SharedBuffer::free(const Allocation& alloc)
{
	auto it = std::find(allocations_.begin(), allocations_.end(), alloc);
	dlg_assertm(it != allocations_.end(), "free: invalid allocation");
	allocations_.erase(it);
}

// BufferAllocator
BufferAllocator::BufferAllocator(const Device& dev) :
	Resource(dev)
{
}

void BufferAllocator::reserve(vk::DeviceSize size, vk::BufferUsageFlags usage, 
	unsigned int memBits)
{
	auto& back = reqs_.emplace_back();	
	back.size = size;
	back.usage = usage;
	back.memBits = memBits;
}

void BufferAllocator::reserve(vk::DeviceSize size, vk::BufferUsageFlags usage, 
	vk::MemoryPropertyFlags memProps)
{
	reserve(size, usage, device().memoryTypeBits(memProps));
}

BufferRange BufferAllocator::alloc(vk::DeviceSize size, 
	vk::BufferUsageFlags usage, unsigned int memBits)
{
	for(auto& buf : buffers_) {
		auto* mem = buf.buffer.memoryEntry().memory();
		dlg_assert(mem);
		if(buf.usage & usage != usage || !(memBits & mem->type())) {
			continue;
		}

		auto alloc = buf.buffer.alloc(size);
		if(alloc.size != 0) {
			return BufferRange(buf.buffer, alloc);
		}
	}

	// allocate a new buffer
	// gather all matching buffer information
	TODO
	vk::BufferCreateInfo createInfo;

	/*
	buffers_.emplace_back(new TransferBuffer(device(), size, mutex_));
	auto alloc = buffers_.back()->use(size);
	dlg_assert(alloc.size == size);
	return BufferRange(*buffers_.back(), alloc);
	*/
}

BufferRange BufferAllocator::alloc(vk::DeviceSize size, 
	vk::BufferUsageFlags usage, vk::MemoryPropertyFlags memProps)
{
	alloc(size, usage, device().memoryTypeBits(memProps));
}

void BufferAllocator::optimize()
{

}

void BufferAllocator::shrink()
{

}

} // namespace vpp