#pragma once

#include <vpp/utility/memory_resource.hpp>
#include <vector>

#if VPP_PMR
namespace std { namespace pmr {
	template<typename T> using vector = std::vector<T, polymorphic_allocator<T>>;
}}
#endif

namespace vpp { namespace spm {
	template<typename T> using vector = std::vector<T, ScopedPolyAlloc<T>>;
}}
