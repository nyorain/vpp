#include <vpp/transfer.hpp>
#include <algorithm>

namespace vpp
{

//TransferBuffer
TransferManager::TransferBuffer::TransferBuffer(const Device& dev, std::size_t size)
{
	vk::BufferCreateInfo info;
	info.size(size);
	info.usage(vk::BufferUsageFlagBits::TransferDst | vk::BufferUsageFlagBits::TransferSrc);

	buffer_ = Buffer(dev, info, vk::MemoryPropertyFlagBits::HostVisible);
}

TransferManager::TransferBuffer::~TransferBuffer()
{
	//todo: warn if allocation left?
}

Allocation TransferManager::TransferBuffer::use(std::size_t size)
{
	static const Allocation start = {0, 0};
	auto old = start;

	for(auto& alloc : ranges_)
	{
		if(alloc.offset - old.end() > size)
		{
			Allocation allocation = {old.end(), size};
			auto it = std::lower_bound(ranges_.begin(), ranges_.end(), allocation,
				[](auto& a, auto& b){ return a.offset < b.offset; });
			return allocation;
		}

		old = alloc;
	}

	return {};
}

bool TransferManager::TransferBuffer::release(const Allocation& alloc)
{
	for(auto it = ranges_.begin(); it < ranges_.end(); ++it)
	{
		if(it->offset == alloc.offset && it->size == alloc.size)
		{
			ranges_.erase(it);
			return true;
		}
	}

	return false;
}

//TransferManager
TransferManager::TransferManager(const Device& dev) : Resource(dev)
{
}

TransferRange TransferManager::transferBuffer(std::size_t size)
{
	for(auto& buffp : buffers_)
	{
		auto alloc = buffp->use(size);
		if(alloc.size > 0) return BufferRange(*buffp, alloc);
	}

	//Allocate new buffer
	buffers_.emplace_back(new TransferBuffer(device(), size));
	return BufferRange(*buffers_.back(), buffers_.back()->use(size));
}

std::size_t TransferManager::totalSize() const
{
	std::size_t ret {};
	for(auto& bufp : buffers_) ret += bufp->buffer().memoryEntry().size();
	return ret;
}

std::size_t TransferManager::activeRanges() const
{

	std::size_t ret {};
	for(auto& bufp : buffers_) ret += bufp->rangesCount();
	return ret;
}

void TransferManager::reserve(std::size_t size)
{
	buffers_.emplace_back(new TransferBuffer(device(), size));
}

void TransferManager::shrink()
{
	for(auto it = buffers_.begin(); it < buffers_.end();)
	{
		if((*it)->rangesCount() == 0) it = buffers_.erase(it);
		else ++it;
	}
}

void TransferManager::optimize()
{
	std::size_t size;
	for(auto it = buffers_.begin(); it < buffers_.end();)
	{
		if((*it)->rangesCount() == 0)
		{
			size += (*it)->buffer().memoryEntry().size();
			it = buffers_.erase(it);
		}
		else
		{
			++it;
		}
	}

	reserve(size);
}

}
