#include <vpp/utility/allocator.hpp>
#include <memory>
#include <algorithm>
#include <iostream>

namespace vpp
{

//Buffer
void Allocator::Buffer::recalcBiggest(const Allocation* freed)
{
	if(freed && freed->position == biggest.end() + 1)
	{
		biggest.size += freed->size;
	}
	else if(freed && freed->end() == biggest.position - 1)
	{
		biggest.position = freed->position;
		biggest.size += freed->size;
	}
	else
	{
		Pointer oldEnd = nullptr;
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
	addBuffer();
}

Allocator::~Allocator()
{
	if(numberAllocations() > 0) std::cerr << "vpp::~Allocator: allocations left\n.";
}

void Allocator::addBuffer()
{
	buffers_.push_back({});

	buffers_.back().buffer.resize(bufferSize);
	buffers_.back().allocs.resize(allocsSize);
	buffers_.back().biggest = {buffers_.back().buffer.data(), bufferSize};
}

Allocator::Allocation& Allocator::nextAllocation(Buffer& buf)
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
		void* pos = buf.biggest.position;
		pos = std::align(alignment, size, pos, buf.biggest.size);
		auto ptr = static_cast<Pointer>(pos);

		if(ptr)
		{
			auto& al = nextAllocation(buf);
			al.position = ptr;
			al.size = size;
			al.alignment = alignment;

			buf.recalcBiggest();
			std::sort(buf.allocs.begin(), buf.allocs.end(),
				[](const Allocation& a, const Allocation& b) { return (a.position < b.position); });

			return ptr;
		}
	}

	if(size > bufferSize)
	{
		bufferSize = size * 2;
	}

	addBuffer();
	return this->alloc(size, alignment); //try again!
}

void Allocator::free(void* buffer)
{
	for(auto& buf : buffers_)
	{
		auto it = std::find_if(buf.allocs.cbegin(), buf.allocs.cend(),
			[=](const Allocation& alloc) { return alloc.position == buffer; });

		if(it != buf.allocs.end())
		{
			auto a = *it;
			buf.allocs.erase(it);
			buf.recalcBiggest(&a);
			return;
		}
	}

	std::cerr << "vpp::Allocator::free: could not find " << buffer << "\n";
}

void* Allocator::realloc(void* buffer, Size size)
{
	for(auto& buf : buffers_)
	{
		auto it = std::find_if(buf.allocs.begin(), buf.allocs.end(),
			[=](const Allocation& alloc) { return alloc.position == buffer; });

		if(it != buf.allocs.end())
		{
			auto nextPos = buf.buffer.data() + buf.buffer.size();
			if((it + 1) != buf.allocs.end()) nextPos = (it + 1)->position;

			if((nextPos - it->position) > size)
			{
				bool recalc = (buf.biggest.position == it->end() + 1);
				it->size = size;
				if(recalc) buf.recalcBiggest();
				return buffer;
			}
			else
			{
				auto a = *it;
				buf.allocs.erase(it);
				buf.recalcBiggest(&a);
				return alloc(a.size, a.alignment);
			}
		}
	}

	std::cerr << "vpp::Allocator::realloc: could not find " << buffer << "\n";
	return nullptr;
}

std::size_t Allocator::size() const
{
	std::size_t ret = 0;
	for(auto& buf : buffers_)
	{
		ret += buf.buffer.size();
	}

	return ret;
}

std::size_t Allocator::allocated() const
{
	std::size_t ret = 0;
	for(auto& buf : buffers_)
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
	for(auto& buf : buffers_)
	{
		if(buf.biggest.size > ret)
			ret = buf.biggest.size;
	}
}

std::size_t Allocator::numberAllocations() const
{
	std::size_t ret = 0;
	for(auto& buf : buffers_)
	{
		for(auto& alloc : buf.allocs)
		{
			if(alloc.position != nullptr) ++ret;
		}
	}

	return ret;
}

}
