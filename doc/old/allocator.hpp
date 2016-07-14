#pragma once

#include <vpp/utility/allocation.hpp>
#include <vpp/utility/nonCopyable.hpp>
#include <vector>
#include <cstdint>

namespace vpp
{

///Custom host allocator class. Holds an internal allocated memory pool.
///Can be used for fast host memory allocations.
///For device memory allocation see DeviceMemory and DeviceMemoryAllocator.
///\sa TypeAllocator
class Allocator : public NonCopyable
{
public:
	using Size = std::size_t;
	using Value = std::uint8_t;
	using Pointer = Value*;

	static constexpr auto defaultBufferSize = 20000;
	static constexpr auto defaultAllocsSize = 100;
	static constexpr auto defaultAllocsIncrease = 50;

	struct Settings
	{
		Size bufferSize = defaultBufferSize;
		Size allocsSize = defaultAllocsSize;
		Size allocsIncrease = defaultAllocsIncrease;
	};

public:
	Settings settings;

public:
	Allocator() { addBuffer(); }
	Allocator(const Settings& xsettings) : settings(xsettings) { addBuffer(); }
	~Allocator();

	//Move functions should only be used when there is no TypeAllocator referencing this allocator
	Allocator(Allocator&& other) noexcept = default;
	Allocator& operator=(Allocator&& other) noexcept = default;

	///\exception std::bad_alloc If the allocation fails.
	///\return Always a valid pointer to size bytes, aligned with the given alignment.
	void* alloc(Size size, Size alignment);

	///\return nullptr on failure, a valid ptr otherwise.
	void* allocNothrow(Size size, Size alignment);

	void* realloc(const void* buffer, Size size, Size alignment);
	void free(const void* buffer);

	Size size() const;
	Size allocated() const;
	Size numberAllocations() const;
	void addBuffer();

protected:
	struct Buffer
	{
		std::vector<Value> buffer;
		std::vector<Allocation> allocs;

		void* alloc(Size size, Size alignment);
	};

protected:
	std::vector<Buffer> buffers_ {};
};

///Template allocator class
///This class can be used e.g. as custom allocators for stl classes such as vector or string.
template<typename T>
class TypeAllocator
{
public:
	using Size = std::size_t;
	using Value = T;
	using Pointer = Value*;

	//std
	using size_type = Size;
	using value_type = Value;
	using pointer = Pointer;
	using const_pointer = const Value*;
	using void_pointer = void*;
	using difference_type = std::ptrdiff_t;
	using is_always_equal = std::true_type;

	template<typename O> struct rebind { using other = TypeAllocator<O>; };

public:
	TypeAllocator(Allocator& allocator) : allocator_(&allocator) {}
	~TypeAllocator() = default;

	template<typename O>
	TypeAllocator(const TypeAllocator<O>& other) noexcept : allocator_(&other.allocator()) {}

	template<typename O> TypeAllocator&
	operator=(const TypeAllocator<O>& other) noexcept { allocator_ = &other.allocator(); }

	Pointer allocate(Size n) const
		{ return reinterpret_cast<Pointer>(allocator_->alloc(n * sizeof(T), alignof(T))); }
	void deallocate(Pointer ptr, Size) const noexcept { allocator_->free(ptr); }

	Allocator& allocator() const { return *allocator_; }

protected:
	Allocator* allocator_;
};

template<typename A, typename B>
bool operator==(const TypeAllocator<A>& a, const TypeAllocator<B>& b)
{
	return (&a.allocator() == &b.allocator());
}

template<typename A, typename B>
bool operator!=(const TypeAllocator<A>& a, const TypeAllocator<B>& b)
{
	return (&a.allocator() != &b.allocator());
}

}
