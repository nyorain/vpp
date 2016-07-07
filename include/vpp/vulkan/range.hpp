#pragma once

#include <cstdlib>
#include <vector>

namespace vk
{

namespace range
{

namespace detail
{
	template<typename T, typename C> using ValidContainer =
		typename std::enable_if<
			std::is_same<
				std::decay_t<std::remove_cv_t<T>>,
				std::decay_t<std::remove_cv_t<
					decltype(*(std::declval<C>().data() + std::declval<C>().size()))
				>>
			>::value
		>::type;
}

///TODO: this can be done better. Use some open source header only file to replace it.
//e.g. https://github.com/rhysd/array_view
template<typename T>
class Range
{
public:
	using Value = std::remove_const_t<T>;

public:
	constexpr Range() = default;
	constexpr Range(const T& value, std::size_t size = 1) : data_(&value), size_(size) {}
	constexpr Range(const T* value, std::size_t size = 1) : data_(value), size_(size) {}
	constexpr Range(const std::initializer_list<Value>& il) : data_(il.begin()), size_(il.size()) {}
	template<std::size_t N> constexpr Range(T (&value)[N]) : data_(value), size_(N) {}

	template<typename C, typename = detail::ValidContainer<T, C>>
	Range(const C& con) : data_(con.data()), size_(con.size()) {}

	constexpr const T* data() const { return data_; }
	constexpr std::size_t size() const { return size_; }

	constexpr const T* begin() { return data_; }
	constexpr const T* end() { return data_ + size_; }

	constexpr const T* begin() const { return data_; }
	constexpr const T* end() const { return data_ + size_; }

	constexpr const T* cbegin() const { return data_; }
	constexpr const T* cend() const { return data_ + size_; }

	const T& operator[](std::size_t i) const { return *(data_ + i); }

protected:
	const T* data_ = nullptr;
	std::size_t size_ = 0;
};

template<typename T> Range<T> makeRange(T& value, std::size_t size){ return Range<T>(value, size); }
template<typename T> Range<T> makeRange(T& value){ return Range<T>(value); }

}

using namespace range;

}
