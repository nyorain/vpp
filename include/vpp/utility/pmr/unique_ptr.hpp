#pragma once

#include <vpp/utility/memory_resource.hpp>

#include <memory>
#include <type_traits>

namespace vpp { namespace spm {

template<typename T>
struct unique_ptr_deleter
{
	unique_ptr_deleter(std::pmr::memory_resource& xresource) : resource(xresource) {}
	void operator()(T* ptr) const { resource.deallocate(ptr, sizeof(T), alignof(T)); }

	std::pmr::memory_resource& resource;
};

template<typename T>
struct unique_ptr_deleter<T[]>
{
	unique_ptr_deleter(std::pmr::memory_resource& xresource, std::size_t xsize) : resource(xresource), size(xsize) {}
	void operator()(T* ptr) const { resource.deallocate(ptr, sizeof(T) * size, alignof(T)); }

	std::pmr::memory_resource& resource;
	std::size_t size;
};

template<typename T> using unique_ptr = std::unique_ptr<T, unique_ptr_deleter<T>>;
template<typename T> std::size_t size(const unique_ptr<T>& ptr) { return ptr.get_deleter().size; }

//custom make unqiue implememntation
namespace detail
{

template<class T> struct UniqueIf
{
    typedef unique_ptr<T> Single;
};

template<class T> struct UniqueIf<T[]>
{
    typedef unique_ptr<T[]> UnkownBound;
};

template<class T, std::size_t N> struct UniqueIf<T[N]>
{
    typedef void KnownBound;
};

}

template<class T, class... Args> typename detail::UniqueIf<T>::Single
make_unique(std::pmr::memory_resource& resource, Args&&... args)
{
	auto ptr = resource.allocate(sizeof(T), alignof(T));
	new(ptr) T(std::forward<Args>(args)...);
	return std::unique_ptr<T>(ptr, {resource});
}

template<class T> typename detail::UniqueIf<T>::UnkownBound
make_unique(std::pmr::memory_resource& resource, std::size_t n)
{
    typedef typename std::remove_extent<T>::type U;

	auto ptr = resource.allocate(sizeof(U) * n, alignof(U));
	new(n, ptr) T;
    return unique_ptr<T>(ptr, {resource, n});
}

template<class T, class... Args> typename detail::UniqueIf<T>::KnownBound
make_unique(std::pmr::memory_resource& resource, Args&&...) = delete;

}}
