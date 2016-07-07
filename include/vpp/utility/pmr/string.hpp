#pragma once

#include <vpp/utility/memory_resource.hpp>
#include <string>

#if VPP_PMR
namespace std { namespace pmr {
	template <typename T>
	using basic_string = std::basic_string<T, std::char_traits<T>, polymorphic_allocator<T>>;
	using string = basic_string<char>;
}}
#endif

namespace vpp { namespace spm {
	template <typename T>
	using basic_string = std::basic_string<T, std::char_traits<T>, ScopedPolyAlloc<T>>;
	using string = basic_string<char>;
}}
