#include <vpp/memoryResource.hpp>
#include <stdexcept>

namespace vpp
{

void MemoryResource::init() const
{
	memoryEntry().allocate();
}

void MemoryResource::assureMemory() const
{
	if(!memoryEntry().allocated()) memoryEntry().allocate();
}

MemoryMapView MemoryResource::memoryMap() const
{
	if(!mappable()) throw std::logic_error("vpp::MemoryResource::memoryMap: not mappable");

	assureMemory();
	return memoryEntry().map();
}

}
