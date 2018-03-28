// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#ifndef NYTL_INCLUDE_SPAN
#define NYTL_INCLUDE_SPAN

#include <cstdlib> // std::size_t
#include <stdexcept> // std::out_of_range
#include <array> // std::array

namespace nytl {
namespace detail {
	template<typename T, typename C, typename = void> struct ValidContainerT;
	template<typename T, typename C> using ValidContainer = typename ValidContainerT<T, C>::type;
} // namespace nytl::detail

// forward declaration with default parameter
#ifndef NYTL_INCLUDE_FWD_SPAN
#define NYTL_INCLUDE_FWD_SPAN
	template<typename T, std::size_t N = 0>
	class Span;
#endif // header guard

/// The underlaying storage type of spans that is specialized for dyanmic size.
template<typename T, std::size_t N> struct SpanStorage;

/// \brief Describes a contigous, non-owned range of elements of type 'T'.
/// \details Spans can be used to pass sequences of elements around in a lightweight manner.
/// Its main use are function parameters sine instead of a std::vector it will not allocate
/// any memory or copy any elements but instead just reference them.
/// \tparam T The type the range is defined over. Use const types for non-mutable ranges.
/// \tparam N The size of the range. If not known at compile
/// time, use nytl::constants::dynamicSize (also defaulted to dynamicSize).
///
/// Some examples below. Note that Spans must be used carefully outside of temporary
/// expressions since they are only valid as long the object they reference is valid.
/// ```cpp
/// void foo(nytl::Span<std::string> names); // takes dyanmic amount of strings, might modify it
/// void bar(nytl::Span<const std::string, 3> names); // takes exactly 3 const strings
/// void baz(nytl::Span<const std::string, 5> names); // takes exactly 5 const strings
///
/// int main()
/// {
///		std::array<std::string, 3> namesArray {"foo", "bar", "baz"};
///		foo(namesArray); // works
/// 	bar(namesArray); // works
///		baz(namesArray); // will throw std::logic_error since baz requires exactly 5 strings
///
///		std::vector<std::string> namesVector {"foo", "bar", "baz", "abz", "bla"};
///		foo(namesVector); // works
///		bar(namesVector); // will throw std::logic_error since bar requires exactly 3 strings
///		baz(namesVector); // works
///
/// 	// If we only want the first 3 elements from namesVector as range we can do it like this
///		bar({namesVector.data(), 3}); // works, takes the first 3 elements
///		foo({*namesVector.data(), 4}); // we can also use references
///
///		// careful when using span outside of temporary expressions
///		auto span = nytl::Span<int>(std::vector<int>{4, 1, 2, 0});
/// 	// std::cout << span[0] << "\n"; // undefined behaviour!
/// }
///
/// void foo(nytl::Span<std::string> names)
/// {
///		// Some usage possibilities for span.
////	// The main usage is iterating over it:
///		for(auto& name : names)
///			std::cout << name << "\n";
///
///		// We might alternatively do this with a traditional for loop
///		for(auto i = 0u; i < names.size(); ++i)
///			std::cout << name << "\n";
///
///		// In this case we have a span of std::string, not const std::string,
///		// therefore we might also change it. This will change the names in the actual
///		// sequence this span references. Usually nytl::Span<const T> is used when the
///		// span is only read
///		if(!names.empty()) {
///			names.front() = "first name";
///			names.back() = "last name";
///		}
///
///		// A span can additionally be sliced into a subspan. This can either be
///		// done with a size known at compile time (which will result in a fixed-size span)
///		// or dynamically for a dynamic-sized span.
///		if(names.size() <= 2) return;
///		for(auto& name : names.slice(2, names.size() - 2)) // output everything but the first two
///			std::cout << name << "\n";
///
///		for(auto& name : names.slice<2>(0)) // output only the first two names
///			std::cout << name << "\n";
/// }
/// ```
template<typename T, std::size_t N>
class Span : public SpanStorage<T, N> {
public:
	using Value = T;
	using Iterator = T*;
	using ReverseIterator = std::reverse_iterator<T*>;
	using Reference = T&;
	using Pointer = T*;
	using Difference = std::ptrdiff_t;
	using Size = std::size_t;

public:
	using SpanStorage<T, N>::SpanStorage;
	constexpr Span() noexcept = default;
	constexpr Span(std::nullptr_t) noexcept : Span(nullptr, 0) {}

	template<typename C, typename = detail::ValidContainer<T, C>>
	constexpr Span(C& c) : Span(c.data(), c.size()) {}

	template<typename C, typename = detail::ValidContainer<T, C>, std::size_t S = C::size()>
	constexpr Span(C& c) : Span(c.data()) {}

	constexpr Pointer data() const noexcept { return this->data_; }
	constexpr Size size() const noexcept { return this->size_; }
	constexpr bool empty() const noexcept { return size() == 0; }

	constexpr Iterator begin() const noexcept { return data(); }
	constexpr Iterator end() const noexcept { return data() + size(); }

	constexpr ReverseIterator rbegin() const noexcept { return {end()}; }
	constexpr ReverseIterator rend() const noexcept { return {begin()}; }

	constexpr Reference operator[](Size i) const noexcept { return *(data() + i); }
	constexpr Reference at(Size i) const { checkThrow(i); return data()[i]; }

	constexpr Reference front() const noexcept { return *data(); }
	constexpr Reference back() const noexcept { return *(data() + size() - 1); }

	constexpr Span<T> slice(Size pos, Size size) const { return {data() + pos, size}; }
	template<Size S> constexpr Span<T, S> slice(Size pos) const { return {data() + pos}; }

protected:
	void checkThrow(Size i) const { if(i >= size()) throw std::out_of_range("nytl::Span::at"); }
};

// Default SpanStorage implementation for compile-time size
template<typename T, std::size_t N>
struct SpanStorage {
	constexpr SpanStorage() noexcept = default;
	constexpr SpanStorage(T* pointer, std::size_t size = N) : data_(pointer)
	{
		if(size != N) throw std::logic_error("nytl::Span:: invalid size");
		if(!pointer && size != 0) throw std::logic_error("nytl::Span:: invalid data");
	}
	constexpr SpanStorage(T& ref, std::size_t size = N) : SpanStorage(&ref, size) {}
	constexpr SpanStorage(T (&arr)[N]) : SpanStorage(arr, N) {}

	T* data_ {};
	constexpr static auto size_ = N;
};

// SpanStorage specialization for runtime size. Stored an extra size value.
template<typename T>
struct SpanStorage<T, 0> {
	constexpr SpanStorage() noexcept = default;
	constexpr SpanStorage(T* pointer, std::size_t size) : data_(pointer), size_(size)
	{
		if(!pointer && size != 0) throw std::logic_error("nytl::Span:: invalid data");
	}
	constexpr SpanStorage(T& ref, std::size_t size) : SpanStorage(&ref, size) {}
	template<std::size_t S> constexpr SpanStorage(T (&arr)[S]) : SpanStorage(arr, S) {}

	T* data_ {};
	std::size_t size_ {};
};

// SpanStorage specialization for runtime size with const parameter.
// Allows constrsuction from initializer list.
template<typename T>
struct SpanStorage<const T, 0> {
	constexpr SpanStorage() noexcept = default;
	constexpr SpanStorage(const T* pointer, std::size_t size) : data_(pointer), size_(size)
	{
		if(!pointer && size != 0) throw std::logic_error("nytl::Span:: invalid data");
	}
	constexpr SpanStorage(const T& ref, std::size_t size) : SpanStorage(&ref, size) {}
	template<std::size_t S> constexpr SpanStorage(const T (&arr)[S]) : SpanStorage(arr, S) {}
	constexpr SpanStorage(const std::initializer_list<T>& l) : SpanStorage(l.begin(), l.size()) {}

	const T* data_ {};
	std::size_t size_ {};
};

namespace detail {

// Returns whether 'C' is a valid container to construct a Span<T> from
template<typename T, typename C>
struct ValidContainerT<T, C,
	typename std::enable_if<
		std::is_convertible<
			decltype(std::declval<C>().data()),
			T*
		>::value &&
		std::is_convertible<
			decltype(std::declval<C>().size()),
			std::size_t
		>::value
	>::type
> { using type = void; };

} // namespace detail
} // namespace nytl

#endif // header guard
