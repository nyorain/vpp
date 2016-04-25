#pragma once

#include <vector>
#include <cstdint>

namespace vpp
{

///Custom host allocator class. Holds an internal allocated memory pool.
///Could be used for faste host memory allocations.
///For device memory allocation see DeviceMemory and DeviceMemoryAllocator.
class Allocator
{
public:
	using Size = std::size_t;
	using Value = std::uint8_t;
	using Pointer = Value*;

protected:
	struct Allocation
	{
		Pointer position {nullptr};
		Size size {0};
		Size alignment {0};

		Pointer end() const { return position + size - 1; }
	};

	struct Buffer
	{
		Allocation biggest; //biggest free block, just uses Allocation struct
		std::vector<Value> buffer;
		std::vector<Allocation> allocs;

		void recalcBiggest(const Allocation* freed = nullptr);
	};

protected:
	std::vector<Buffer> buffers_ {};

protected:
	Allocation& nextAllocation(Buffer& buf);
	void addBuffer();

public:
	Size bufferSize;
	Size allocsSize;
	Size allocsIncrease;

public:
	Allocator(Size pbufferSize = 20000, Size pallocsSize = 100, Size pallocsIncrease = 50);
	~Allocator();

	void* alloc(Size size, Size alignment);
	void free(void* buffer);
	void* realloc(void* buffer, Size size);

	Size size() const;
	Size allocated() const;
	Size biggestBlock() const;
	Size numberAllocations() const;
};

}
