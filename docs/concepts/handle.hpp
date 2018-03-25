// Idea: provide typesafe handle wrapper (for vkpp)
// Then, vk::nullHandle would finally be possible.
// Or name it vk::nullhdl?


// #1
// Not yet working completely

#pragma once

#include <cstdint>


namespace vk {

//Handle
template<typename T>
class Handle {
private:
	std::uint64_t data_ {};

public:
	Handle() = default;
	Handle(const T& n) : data_((std::uint64_t)n) {}

	operator T() const { return (T)data_; }
	operator bool() const { return data_ != 0; }

	T native() const { return (T)data_; }
	std::uint64_t data() cosnt { return data_; }
};

template<typename T>
bool operator==(const Handle<T>& a, const Handle<T>& b) { return a.data() == b.data(); }

template<typename T>
bool operator!=(const Handle<T>& a, const Handle<T>& b) { return a.data() != b.data(); }

///NullHandle
struct NullHandle {
	template<typename T> operator Handle<T>() const { return {}; }
} nullHandle;

}


// #2
// Not yet working completely

namespace vk {

//Handle
template<typename T>
struct Handle {
	std::uint64_t data_ {}; // bad idea to always use 64 bits
	operator bool() const { return data_; }
	operator std::uint64_t() const { return data_; }
	template<typename O> explicit operator O*() const { return (O*)data_; }
};


template<typename T>
bool operator==(const Handle<T>& a, const Handle<T>& b) { return a.data() == b.data(); }

template<typename T>
bool operator!=(const Handle<T>& a, const Handle<T>& b) { return a.data() != b.data(); }

///NullHandle
struct NullHandle {
	template<typename T> operator Handle<T>() const { return {}; }
} nullHandle;

}

#define VPP_DEFINE_HANDLE(name)\
	struct name##_T; \
	using name = vk::Handle<name##_T>;
