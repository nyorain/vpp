#pragma once

#include <vpp/utility/memory_resource.hpp>
#include <list>

#if VPP_PMR
namespace std { namespace pmr {
	template <typename T> using list = std::list<T, polymorphic_allocator<T>>;
}}
#endif

namespace vpp { namespace spm {
	template <typename T> using list = std::list<T, ScopedPolyAlloc<T>>;
}}
