#pragma once

namespace vpp
{

//e.g.: for(auto i : makeRange(5u, 10)) or for(auto i : makeRange(&values, 5)).

///Makes iteration over raw memory or an integer range really easy.
template<typename T>
class Range
{
public:
	T start;
	unsigned int size;

public:
	constexpr T begin() const { return start; }
	constexpr T end() const { return start + size; }
};

///Range constrcution helper.
template<typename T>
Range<T> makeRange(T value, unsigned int size)
{
	return Range<T>{value, size};
}

}
