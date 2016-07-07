#pragma once

#include <vpp/utility/memory_resource.hpp>
#include <deque>

#if VPP_PMR
namespace std { namespace pmr {
	template <typename T> using deque = std::deque<T, polymorphic_allocator<T>>;
}}
#endif

namespace vpp { namespace spm {
	template <typename T> using deque = std::deque<T, ScopedPolyAlloc<T>>;
}}
