#pragma once

#include <cstdlib>
#include <vector>

namespace vk
{

template<typename T>
class Range
{
public:
	using Value = std::remove_const_t<T>;

public:
	constexpr Range() = default;
	constexpr Range(T& value, std::size_t size = 1) : data_(&value), size_(size) {}
	constexpr Range(T* value, std::size_t size = 1) : data_(value), size_(size) {}
	constexpr Range(const std::vector<Value>& vec) : data_(vec.data()), size_(vec.size()) {}
	constexpr Range(const std::initializer_list<Value>& il) : data_(il.begin()), size_(il.size()) {}
	template<std::size_t N> constexpr Range(T (&value)[N]) : data_(value), size_(N) {}

	constexpr const T* data() const { return data_; }
	constexpr std::size_t size() const { return size_; }

protected:
	const T* data_ = nullptr;
	std::size_t size_ = 0;
};

}
