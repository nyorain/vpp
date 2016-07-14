#include <vpp/utility/allocator.hpp>

#include <memory>
#include <algorithm>
#include <iostream>

namespace vpp
{

//Buffer
void* Allocator::Buffer::alloc(Size size, Size alignment)
{
	const Allocation start = {reinterpret_cast<std::size_t>(buffer.data()), 0};

	const auto* old = &start;
	for(auto& alloc : allocs)
	{
		auto offset = align(old->end(), alignment);
		old = &alloc;
		if(offset + size > alloc.offset) continue;

		allocs.push_back({offset, size});
		return reinterpret_cast<void*>(offset);
	}

	//last block
	auto offset = align(old->end(), alignment);
	if(offset + size > reinterpret_cast<std::size_t>(buffer.data() + buffer.size())) return nullptr;

	allocs.push_back({offset, size});
	return reinterpret_cast<void*>(offset);
}

//Allocator
Allocator::~Allocator()
{
#ifndef NDEBUG
	if(numberAllocations() > 0) std::cerr << "vpp::~Allocator: allocations left\n.";
#endif //ndebug
}

void Allocator::addBuffer()
{
	buffers_.push_back({});

	buffers_.back().buffer.resize(settings.bufferSize);
	buffers_.back().allocs.reserve(settings.allocsSize);
}

void* Allocator::alloc(Size size, Size alignment)
{
	auto ptr = allocNothrow(size, alignment);
	if(!ptr) throw std::bad_alloc {};
	return ptr;
}

void* Allocator::allocNothrow(Size size, Size alignment)
{
	void* ret = nullptr;
	for(auto& buf : buffers_)
	{
		ret = buf.alloc(size, alignment);
		if(ret)
		{
			auto capacity = buf.allocs.capacity();
			if(buf.allocs.size() == capacity) buf.allocs.reserve(capacity + settings.allocsIncrease);
			return ret;
		}
	}

	if(size > settings.bufferSize) settings.bufferSize = size * 2;
	addBuffer();
	auto ptr = buffers_.back().alloc(size, alignment);
	return ptr;
}

void Allocator::free(const void* buffer)
{
	for(auto& buf : buffers_)
	{
		//check if in range
		if(buf.buffer.data() > buffer || buf.buffer.data() + buf.buffer.size() < buffer) continue;

		auto intBuffer = reinterpret_cast<std::size_t>(buffer);
		auto it = std::find_if(buf.allocs.cbegin(), buf.allocs.cend(),
			[=](const Allocation& alloc) { return alloc.offset == intBuffer; });

		if(it != buf.allocs.end())
		{
			auto a = *it;
			buf.allocs.erase(it);
			return;
		}
	}

#ifndef NDEBUG
	std::cerr << "vpp::Allocator::free: could not find " << buffer << "\n";
#endif
}

void* Allocator::realloc(const void* buffer, Size size, Size align)
{
	for(auto& buf : buffers_)
	{
		//check if in range
		if(buf.buffer.data() > buffer || buf.buffer.data() + buf.buffer.size() < buffer) continue;

		auto intBuffer = reinterpret_cast<std::size_t>(buffer);
		auto it = std::find_if(buf.allocs.begin(), buf.allocs.end(),
			[=](const Allocation& alloc) { return alloc.offset == intBuffer; });

		if(it != buf.allocs.end())
		{
			auto nextPos = reinterpret_cast<std::size_t>(buf.buffer.data() + buf.buffer.size());
			if((it + 1) != buf.allocs.end()) nextPos = (it + 1)->offset;

			//check if the space after the allocation is enough to extend it to the size
			if((nextPos - it->offset) >= size)
			{
				it->size = size;
				return reinterpret_cast<void*>(it->offset);
			}
			else
			{
				auto a = *it;
				buf.allocs.erase(it);
				return alloc(a.size, align);
			}
		}
	}

#ifndef NDEBUG
	std::cerr << "vpp::Allocator::realloc: could not find " << buffer << "\n";
#endif

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

std::size_t Allocator::numberAllocations() const
{
	std::size_t ret = 0;
	for(auto& buf : buffers_)
		ret += buf.allocs.size();

	return ret;
}

}
