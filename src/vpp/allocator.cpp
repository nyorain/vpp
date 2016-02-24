#include <vpp/allocation.hpp>
#include <memory>
#include <algorithm>

namespace vpp
{

//Buffer
void Allocator::Buffer::recalcBiggest(const Allocation* freed)
{
	if(freed && freed->position == biggest.end() + 1)
	{
		biggest.size += freed.size;
	}
	else if(freed && freed.end() == biggest->position - 1)
	{
		biggest.position = freed.position;
		biggest.size += freed.size;
	}
	else
	{
		void* oldEnd = nullptr;
		for(auto& alloc : allocs)
		{
			if(oldEnd != nullptr)
			{
				if(alloc.position - oldEnd > biggest.size)
				{
					biggest.size = alloc.position - oldEnd;
					biggest.position = oldEnd + 1;
				}
			}

			oldEnd = alloc.end();
		}
	}
}

//Allocator
Allocator::Allocator(Size pbufferSize, Size pallocsSize, Size pallocsIncrease)
	: bufferSize(pbufferSize), allocsSize(pallocsSize), allocsIncrease(pallocsIncrease)
{
	buffers_.resize(1);

	buffers_[0].buffer.resize(bufferSize);
	buffers_[0].allocs.resize(startAllocs);
}

Allocator::~Allocator()
{
	if(numberAllocations() > 0) std::cerr << "vpp::~Allocator: allocations left".
}

void Allocator::addBuffer()
{
	buffers_.push_back({});

	buffers_.back().buffer.resize(bufferSize);
	buffers_.back().allocs.resize(allocsSize);
	buffers_.back().biggest = bufferSize;
}

Allocation& Allocator::nextAllocation(Buffer& buf)
{
	for(auto& alloc : buf.allocs)
	{
		if(alloc.position == nullptr) return alloc;
	}

	buf.allocs.resize(buf.allocs.size() + allocsIncrease);
	return nextAllocation(buf); //try again!
}

void* Allocator::alloc(Size size, Size alignment)
{
	for(auto& buf : buffers_)
	{
		auto ptr = std::align(alignment, size, buf.biggest.position, buf.biggest.size);
		if(ptr)
		{
			auto al = nextAllocation(buf);
			al.position = ptr;
			al.size = size;

			buf.recalcBiggest();
			std::sort(buf.allocs.begin(), buf.allocs.end(),
				[](const Allocation& a, const Allocation& b) { return (a.position < b.position); });

			return ptr;
		}
	}

	addBuffer();
	return alloc(size, alignment); //try again!
}

void Allocator::free(void* buffer)
{
	for(auto& buf : buffers_)
	{
		auto it = std::find(buf.allocs.cbegin(), buf.allocs.cend(),
			[=](const Allocation& alloc) { return alloc.position == buffer; });

		if(it != buf.end())
		{
			auto a = *it;
			buf.allocs.erase(it);
			buf.recalcBiggest(a);
			return;
		}
	}

	std::cerr << "vpp::Allocator::free: could not find " << buffer << "\n";
}

void* Allocator::realloc(void* buffer, Size size)
{
	for(auto& buf : buffers_)
	{
		auto it = std::find(buf.allocs.cbegin(), buf.allocs.cend(),
			[=](const Allocation& alloc) { return alloc.position == buffer; });

		if(it != buf.end())
		{
			if(((it + 1)->position - it->position) > size)
			{
				bool recalc = (buf.biggest.position == it.end() + 1);
				it->size = size;
				if(recalc) buf.recalcBiggest();
				return buffer;
			}
			else
			{
				auto a = *it;
				buf.allocs.erase(it);
				buf.recalcBiggest(a);
				return;
			}
		}
	}

	std::cerr << "vpp::Allocator::realloc: could not find " << buffer << "\n";
}

std::size_t Allocator::size() const
{
	std::size_t ret = 0;
	for(auto& buf : buffers)
	{
		ret += buf.size();
	}

	return ret;
}

std::size_t Allocator::allocated() const
{
	std::size_t ret = 0;
	for(auto& buf : buffers)
	{
		for(auto& alloc : buf.allocs)
		{
			ret += alloc.size;
		}
	}

	return ret;
}

std::size_t Allocator::biggestBlock() const
{
	std::size_t ret = 0;
	for(auto& buf : buffers)
	{
		if(buf.biggestBlock > ret)
			ret = buf.biggest;
	}
}

std::size_t Allocator::numberAllocations() const
{
	std::size_t ret = 0;
	for(auto& buf : buffers)
	{
		for(auto& alloc : buf.allocs)
		{
			if(alloc.position != nullptr) ++ret;
		}
	}

	return ret;
}

}
