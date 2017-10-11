// to be inserted into device.cpp
// probably not worth it though, the driver can do this better than we can
// and the host-side allocator is mainly for debugging (as stated by the spec)

struct VulkanAllocator
{
	std::pmr::memory_resource& memoryResource;
	std::vector<Allocation> vulkanAllocations;
	vk::AllocationCallbacks allocationCallbacks;

	VulkanAllocator(std::pmr::memory_resource& memres) : memoryResource(memres)
	{
		allocationCallbacks.pUserData = this;
		allocationCallbacks.pfnAllocation = (vk::PfnAllocationFunction) &VulkanAllocator::alloc;
	}

	void* alloc(std::size_t size, std::size_t align, vk::SystemAllocationScope scope);
	void free(void* ptr);
	void* realloc(void* ptr, std::size_t size, std::size_t align, vk::SystemAllocationScope scp);
	void internalAlloc(size_t size, vk::InternalAllocationType type, vk::SystemAllocationScope scp);
	void internalFree(size_t size, vk::InternalAllocationType type, vk::SystemAllocationScope scp);

	static void* alloc(void* data, std::size_t size, std::size_t align,
		vk::SystemAllocationScope scope)
		{ ((VulkanAllocator*)data)->alloc(size, align, scope); }

	static void free(void* data, void* ptr)
		{ ((VulkanAllocator*)data)->free(ptr); }

	static void* realloc(void* data, void* original, std::size_t size, std::size_t align,
		vk::SystemAllocationScope scope)
		{ ((VulkanAllocator*)data)->realloc(original, size, align, scope); }

	static void internalAlloc(void* data, size_t size, vk::InternalAllocationType type,
		vk::SystemAllocationScope scope)
		{ ((VulkanAllocator*)data)->internalAlloc(size, type, scope); }

	static void internalFree(void* data, size_t size, vk::InternalAllocationType type,
		vk::SystemAllocationScope scope)
		{ ((VulkanAllocator*)data)->internalFree(size, type, scope); }
};
