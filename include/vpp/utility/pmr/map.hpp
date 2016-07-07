#pragma once

#include <vpp/utility/memory_resource.hpp>
#include <map>

#if VPP_PMR
namespace std { namespace pmr {
	template<typename K, typename T>
	using map = std::map<K, T, std::less<K>, polymorphic_allocator<std::pair<K, T>>>;
}}
#endif

namespace vpp { namespace spm {
	template<typename K, typename T>
	using map = std::map<K, T, std::less<K>, ScopedPolyAlloc<std::pair<K, T>>>;
}}
