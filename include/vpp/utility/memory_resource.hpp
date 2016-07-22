#pragma once

//needed until c++17
#define VPP_PMR 1

#include <scoped_allocator>

#ifdef VPP_PMR
 #include <boost/container/pmr/memory_resource.hpp>
 #include <boost/container/pmr/polymorphic_allocator.hpp>
 #include <boost/container/pmr/synchronized_pool_resource.hpp>
 #include <boost/container/pmr/unsynchronized_pool_resource.hpp>

 namespace std { namespace pmr {
	template<typename T> using polymorphic_allocator = boost::container::pmr::polymorphic_allocator<T>;

	using synchronized_pool_resource = boost::container::pmr::synchronized_pool_resource;
	using unsynchronized_pool_resource = boost::container::pmr::unsynchronized_pool_resource;
 }}
#endif

//spm for scoped polymorphic memory (allocator/resource)
namespace vpp { namespace spm {
	template<typename T>
	using ScopedPolyAlloc = std::scoped_allocator_adaptor<std::pmr::polymorphic_allocator<T>>;
}}
