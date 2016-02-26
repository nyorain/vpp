#pragma once

#include <vector>

namespace vpp
{

//Custom allocator class
class Allocator
{
public:
	using Size = std::size_t;
	using Value = std::uint8_t;
	using Pointer = void*;
	using Buffer = std::vector<std::uint8_t>;

protected:
	struct Allocation
	{
		Pointer position {nullptr};
		Size size {0};

		Pointer end() const { return position + size - 1; }
	};

	struct Buffer
	{
		Allocation biggest; //biggest free block, just uses Allocation struct
		std::vector<std::uint8_t> buffer;
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

	Pointer alloc(Size size, Size alignment);
	void free(Pointer buffer);
	Pointer realloc(Pointer buffer, Size size);

	Size size() const;
	Size allocated() const;
	Size biggestBlock() const;
	Size numberAllocations() const;
};

}
