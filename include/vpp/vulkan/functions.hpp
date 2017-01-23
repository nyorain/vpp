// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

// Automaitcally generated vulkan header file for the nyorain/vpp library.
// Do not edit manually, rather edit the codegen files.

#pragma once

#include "fwd.hpp"
#include "enums.hpp"
#include "structs.hpp"
#include "error.hpp"
#include "span.hpp"

#include <vector>
#include <vulkan/vulkan.h>

static_assert(VK_HEADER_VERSION ==  37, "Vulkan and vpp header incompatibility");

namespace vk {

inline Instance createInstance(const InstanceCreateInfo& pCreateInfo, const AllocationCallbacks* pAllocator = {})
{
	Instance ret = {};
	VPP_CALL(vkCreateInstance((const VkInstanceCreateInfo*)(&pCreateInfo), (const VkAllocationCallbacks*)(pAllocator), (VkInstance*)(&ret)));
	return ret;
}

inline void destroyInstance(Instance instance = {}, const AllocationCallbacks* pAllocator = {})
{
	return vkDestroyInstance((VkInstance)(instance), (const VkAllocationCallbacks*)(pAllocator));
}

inline Result enumeratePhysicalDevices(Instance instance, uint32_t& pPhysicalDeviceCount, PhysicalDevice* pPhysicalDevices = {})
{
	return static_cast<Result>(vkEnumeratePhysicalDevices((VkInstance)(instance), (uint32_t*)(&pPhysicalDeviceCount), (VkPhysicalDevice*)(pPhysicalDevices)));
}
inline std::vector<PhysicalDevice> enumeratePhysicalDevices(Instance instance)
{
	std::vector<PhysicalDevice> ret;
	uint32_t count = 0u;
	if(!error::success(VPP_CALL(vkEnumeratePhysicalDevices((VkInstance)(instance), &count, (VkPhysicalDevice*)(ret.data()))))) return ret;
	ret.resize(count);
	VPP_CALL(vkEnumeratePhysicalDevices((VkInstance)(instance), &count, (VkPhysicalDevice*)(ret.data())));
	return ret;
}

inline void getPhysicalDeviceFeatures(PhysicalDevice physicalDevice, PhysicalDeviceFeatures& pFeatures)
{
	return vkGetPhysicalDeviceFeatures((VkPhysicalDevice)(physicalDevice), (VkPhysicalDeviceFeatures*)(&pFeatures));
}

inline FormatProperties getPhysicalDeviceFormatProperties(PhysicalDevice physicalDevice, Format format)
{
	FormatProperties ret = {};
	vkGetPhysicalDeviceFormatProperties((VkPhysicalDevice)(physicalDevice), static_cast<VkFormat>(format), (VkFormatProperties*)(&ret));
	return ret;
}

inline ImageFormatProperties getPhysicalDeviceImageFormatProperties(PhysicalDevice physicalDevice, Format format, ImageType type, ImageTiling tiling, ImageUsageFlags usage, ImageCreateFlags flags = {})
{
	ImageFormatProperties ret = {};
	VPP_CALL(vkGetPhysicalDeviceImageFormatProperties((VkPhysicalDevice)(physicalDevice), static_cast<VkFormat>(format), static_cast<VkImageType>(type), static_cast<VkImageTiling>(tiling), static_cast<VkImageUsageFlags>(usage), static_cast<VkImageCreateFlags>(flags), (VkImageFormatProperties*)(&ret)));
	return ret;
}

inline PhysicalDeviceProperties getPhysicalDeviceProperties(PhysicalDevice physicalDevice)
{
	PhysicalDeviceProperties ret = {};
	vkGetPhysicalDeviceProperties((VkPhysicalDevice)(physicalDevice), (VkPhysicalDeviceProperties*)(&ret));
	return ret;
}

inline void getPhysicalDeviceQueueFamilyProperties(PhysicalDevice physicalDevice, uint32_t& pQueueFamilyPropertyCount, QueueFamilyProperties* pQueueFamilyProperties = {})
{
	return vkGetPhysicalDeviceQueueFamilyProperties((VkPhysicalDevice)(physicalDevice), (uint32_t*)(&pQueueFamilyPropertyCount), (VkQueueFamilyProperties*)(pQueueFamilyProperties));
}
inline std::vector<QueueFamilyProperties> getPhysicalDeviceQueueFamilyProperties(PhysicalDevice physicalDevice)
{
	std::vector<QueueFamilyProperties> ret;
	uint32_t count = 0u;
	vkGetPhysicalDeviceQueueFamilyProperties((VkPhysicalDevice)(physicalDevice), &count, (VkQueueFamilyProperties*)(ret.data()));
	ret.resize(count);
	vkGetPhysicalDeviceQueueFamilyProperties((VkPhysicalDevice)(physicalDevice), &count, (VkQueueFamilyProperties*)(ret.data()));
	return ret;
}

inline PhysicalDeviceMemoryProperties getPhysicalDeviceMemoryProperties(PhysicalDevice physicalDevice)
{
	PhysicalDeviceMemoryProperties ret = {};
	vkGetPhysicalDeviceMemoryProperties((VkPhysicalDevice)(physicalDevice), (VkPhysicalDeviceMemoryProperties*)(&ret));
	return ret;
}

inline PfnVoidFunction getInstanceProcAddr(Instance instance, const char* pName)
{
	return static_cast<PfnVoidFunction>(vkGetInstanceProcAddr((VkInstance)(instance), (const char*)(pName)));
}

inline PfnVoidFunction getDeviceProcAddr(Device device, const char* pName)
{
	return static_cast<PfnVoidFunction>(vkGetDeviceProcAddr((VkDevice)(device), (const char*)(pName)));
}

inline Device createDevice(PhysicalDevice physicalDevice, const DeviceCreateInfo& pCreateInfo, const AllocationCallbacks* pAllocator = {})
{
	Device ret = {};
	VPP_CALL(vkCreateDevice((VkPhysicalDevice)(physicalDevice), (const VkDeviceCreateInfo*)(&pCreateInfo), (const VkAllocationCallbacks*)(pAllocator), (VkDevice*)(&ret)));
	return ret;
}

inline void destroyDevice(Device device = {}, const AllocationCallbacks* pAllocator = {})
{
	return vkDestroyDevice((VkDevice)(device), (const VkAllocationCallbacks*)(pAllocator));
}

inline Result enumerateInstanceExtensionProperties(const char* pLayerName, uint32_t& pPropertyCount, ExtensionProperties* pProperties = {})
{
	return static_cast<Result>(vkEnumerateInstanceExtensionProperties((const char*)(pLayerName), (uint32_t*)(&pPropertyCount), (VkExtensionProperties*)(pProperties)));
}
inline std::vector<ExtensionProperties> enumerateInstanceExtensionProperties(const char* pLayerName)
{
	std::vector<ExtensionProperties> ret;
	uint32_t count = 0u;
	if(!error::success(VPP_CALL(vkEnumerateInstanceExtensionProperties((const char*)(pLayerName), &count, (VkExtensionProperties*)(ret.data()))))) return ret;
	ret.resize(count);
	VPP_CALL(vkEnumerateInstanceExtensionProperties((const char*)(pLayerName), &count, (VkExtensionProperties*)(ret.data())));
	return ret;
}

inline Result enumerateDeviceExtensionProperties(PhysicalDevice physicalDevice, const char* pLayerName, uint32_t& pPropertyCount, ExtensionProperties* pProperties = {})
{
	return static_cast<Result>(vkEnumerateDeviceExtensionProperties((VkPhysicalDevice)(physicalDevice), (const char*)(pLayerName), (uint32_t*)(&pPropertyCount), (VkExtensionProperties*)(pProperties)));
}
inline std::vector<ExtensionProperties> enumerateDeviceExtensionProperties(PhysicalDevice physicalDevice, const char* pLayerName)
{
	std::vector<ExtensionProperties> ret;
	uint32_t count = 0u;
	if(!error::success(VPP_CALL(vkEnumerateDeviceExtensionProperties((VkPhysicalDevice)(physicalDevice), (const char*)(pLayerName), &count, (VkExtensionProperties*)(ret.data()))))) return ret;
	ret.resize(count);
	VPP_CALL(vkEnumerateDeviceExtensionProperties((VkPhysicalDevice)(physicalDevice), (const char*)(pLayerName), &count, (VkExtensionProperties*)(ret.data())));
	return ret;
}

inline Result enumerateInstanceLayerProperties(uint32_t& pPropertyCount, LayerProperties* pProperties = {})
{
	return static_cast<Result>(vkEnumerateInstanceLayerProperties((uint32_t*)(&pPropertyCount), (VkLayerProperties*)(pProperties)));
}
inline std::vector<LayerProperties> enumerateInstanceLayerProperties()
{
	std::vector<LayerProperties> ret;
	uint32_t count = 0u;
	if(!error::success(VPP_CALL(vkEnumerateInstanceLayerProperties(&count, (VkLayerProperties*)(ret.data()))))) return ret;
	ret.resize(count);
	VPP_CALL(vkEnumerateInstanceLayerProperties(&count, (VkLayerProperties*)(ret.data())));
	return ret;
}

inline Result enumerateDeviceLayerProperties(PhysicalDevice physicalDevice, uint32_t& pPropertyCount, LayerProperties* pProperties = {})
{
	return static_cast<Result>(vkEnumerateDeviceLayerProperties((VkPhysicalDevice)(physicalDevice), (uint32_t*)(&pPropertyCount), (VkLayerProperties*)(pProperties)));
}
inline std::vector<LayerProperties> enumerateDeviceLayerProperties(PhysicalDevice physicalDevice)
{
	std::vector<LayerProperties> ret;
	uint32_t count = 0u;
	if(!error::success(VPP_CALL(vkEnumerateDeviceLayerProperties((VkPhysicalDevice)(physicalDevice), &count, (VkLayerProperties*)(ret.data()))))) return ret;
	ret.resize(count);
	VPP_CALL(vkEnumerateDeviceLayerProperties((VkPhysicalDevice)(physicalDevice), &count, (VkLayerProperties*)(ret.data())));
	return ret;
}

inline Queue getDeviceQueue(Device device, uint32_t queueFamilyIndex, uint32_t queueIndex)
{
	Queue ret = {};
	vkGetDeviceQueue((VkDevice)(device), queueFamilyIndex, queueIndex, (VkQueue*)(&ret));
	return ret;
}

inline Result queueSubmit(Queue queue, uint32_t submitCount, const SubmitInfo& pSubmits, Fence fence = {})
{
	return static_cast<Result>(vkQueueSubmit((VkQueue)(queue), submitCount, (const VkSubmitInfo*)(&pSubmits), (VkFence)(fence)));
}
inline Result queueSubmit(Queue queue, nytl::Span<const SubmitInfo> pSubmits, Fence fence = {})
{
	return VPP_CALL(vkQueueSubmit((VkQueue)(queue), pSubmits.size(), (const VkSubmitInfo*)(pSubmits.data()), (VkFence)(fence)));
}

inline Result queueWaitIdle(Queue queue)
{
	return static_cast<Result>(vkQueueWaitIdle((VkQueue)(queue)));
}

inline Result deviceWaitIdle(Device device)
{
	return static_cast<Result>(vkDeviceWaitIdle((VkDevice)(device)));
}

inline DeviceMemory allocateMemory(Device device, const MemoryAllocateInfo& pAllocateInfo, const AllocationCallbacks* pAllocator = {})
{
	DeviceMemory ret = {};
	VPP_CALL(vkAllocateMemory((VkDevice)(device), (const VkMemoryAllocateInfo*)(&pAllocateInfo), (const VkAllocationCallbacks*)(pAllocator), (VkDeviceMemory*)(&ret)));
	return ret;
}

inline void freeMemory(Device device, DeviceMemory memory = {}, const AllocationCallbacks* pAllocator = {})
{
	return vkFreeMemory((VkDevice)(device), (VkDeviceMemory)(memory), (const VkAllocationCallbacks*)(pAllocator));
}

inline void* mapMemory(Device device, DeviceMemory memory, DeviceSize offset, DeviceSize size, MemoryMapFlags flags = {})
{
	void* ret = {};
	VPP_CALL(vkMapMemory((VkDevice)(device), (VkDeviceMemory)(memory), offset, size, static_cast<VkMemoryMapFlags>(flags), (void**)(&ret)));
	return ret;
}

inline void unmapMemory(Device device, DeviceMemory memory)
{
	return vkUnmapMemory((VkDevice)(device), (VkDeviceMemory)(memory));
}

inline Result flushMappedMemoryRanges(Device device, uint32_t memoryRangeCount, const MappedMemoryRange& pMemoryRanges)
{
	return static_cast<Result>(vkFlushMappedMemoryRanges((VkDevice)(device), memoryRangeCount, (const VkMappedMemoryRange*)(&pMemoryRanges)));
}
inline Result flushMappedMemoryRanges(Device device, nytl::Span<const MappedMemoryRange> pMemoryRanges)
{
	return VPP_CALL(vkFlushMappedMemoryRanges((VkDevice)(device), pMemoryRanges.size(), (const VkMappedMemoryRange*)(pMemoryRanges.data())));
}

inline Result invalidateMappedMemoryRanges(Device device, uint32_t memoryRangeCount, const MappedMemoryRange& pMemoryRanges)
{
	return static_cast<Result>(vkInvalidateMappedMemoryRanges((VkDevice)(device), memoryRangeCount, (const VkMappedMemoryRange*)(&pMemoryRanges)));
}
inline Result invalidateMappedMemoryRanges(Device device, nytl::Span<const MappedMemoryRange> pMemoryRanges)
{
	return VPP_CALL(vkInvalidateMappedMemoryRanges((VkDevice)(device), pMemoryRanges.size(), (const VkMappedMemoryRange*)(pMemoryRanges.data())));
}

inline DeviceSize getDeviceMemoryCommitment(Device device, DeviceMemory memory)
{
	DeviceSize ret = {};
	vkGetDeviceMemoryCommitment((VkDevice)(device), (VkDeviceMemory)(memory), (VkDeviceSize*)(&ret));
	return ret;
}

inline Result bindBufferMemory(Device device, Buffer buffer, DeviceMemory memory, DeviceSize memoryOffset)
{
	return static_cast<Result>(vkBindBufferMemory((VkDevice)(device), (VkBuffer)(buffer), (VkDeviceMemory)(memory), memoryOffset));
}

inline Result bindImageMemory(Device device, Image image, DeviceMemory memory, DeviceSize memoryOffset)
{
	return static_cast<Result>(vkBindImageMemory((VkDevice)(device), (VkImage)(image), (VkDeviceMemory)(memory), memoryOffset));
}

inline MemoryRequirements getBufferMemoryRequirements(Device device, Buffer buffer)
{
	MemoryRequirements ret = {};
	vkGetBufferMemoryRequirements((VkDevice)(device), (VkBuffer)(buffer), (VkMemoryRequirements*)(&ret));
	return ret;
}

inline MemoryRequirements getImageMemoryRequirements(Device device, Image image)
{
	MemoryRequirements ret = {};
	vkGetImageMemoryRequirements((VkDevice)(device), (VkImage)(image), (VkMemoryRequirements*)(&ret));
	return ret;
}

inline void getImageSparseMemoryRequirements(Device device, Image image, uint32_t& pSparseMemoryRequirementCount, SparseImageMemoryRequirements* pSparseMemoryRequirements = {})
{
	return vkGetImageSparseMemoryRequirements((VkDevice)(device), (VkImage)(image), (uint32_t*)(&pSparseMemoryRequirementCount), (VkSparseImageMemoryRequirements*)(pSparseMemoryRequirements));
}
inline std::vector<SparseImageMemoryRequirements> getImageSparseMemoryRequirements(Device device, Image image)
{
	std::vector<SparseImageMemoryRequirements> ret;
	uint32_t count = 0u;
	vkGetImageSparseMemoryRequirements((VkDevice)(device), (VkImage)(image), &count, (VkSparseImageMemoryRequirements*)(ret.data()));
	ret.resize(count);
	vkGetImageSparseMemoryRequirements((VkDevice)(device), (VkImage)(image), &count, (VkSparseImageMemoryRequirements*)(ret.data()));
	return ret;
}

inline void getPhysicalDeviceSparseImageFormatProperties(PhysicalDevice physicalDevice, Format format, ImageType type, SampleCountBits samples, ImageUsageFlags usage, ImageTiling tiling, uint32_t& pPropertyCount, SparseImageFormatProperties* pProperties = {})
{
	return vkGetPhysicalDeviceSparseImageFormatProperties((VkPhysicalDevice)(physicalDevice), static_cast<VkFormat>(format), static_cast<VkImageType>(type), static_cast<VkSampleCountFlagBits>(samples), static_cast<VkImageUsageFlags>(usage), static_cast<VkImageTiling>(tiling), (uint32_t*)(&pPropertyCount), (VkSparseImageFormatProperties*)(pProperties));
}
inline std::vector<SparseImageFormatProperties> getPhysicalDeviceSparseImageFormatProperties(PhysicalDevice physicalDevice, Format format, ImageType type, SampleCountBits samples, ImageUsageFlags usage, ImageTiling tiling)
{
	std::vector<SparseImageFormatProperties> ret;
	uint32_t count = 0u;
	vkGetPhysicalDeviceSparseImageFormatProperties((VkPhysicalDevice)(physicalDevice), static_cast<VkFormat>(format), static_cast<VkImageType>(type), static_cast<VkSampleCountFlagBits>(samples), static_cast<VkImageUsageFlags>(usage), static_cast<VkImageTiling>(tiling), &count, (VkSparseImageFormatProperties*)(ret.data()));
	ret.resize(count);
	vkGetPhysicalDeviceSparseImageFormatProperties((VkPhysicalDevice)(physicalDevice), static_cast<VkFormat>(format), static_cast<VkImageType>(type), static_cast<VkSampleCountFlagBits>(samples), static_cast<VkImageUsageFlags>(usage), static_cast<VkImageTiling>(tiling), &count, (VkSparseImageFormatProperties*)(ret.data()));
	return ret;
}

inline Result queueBindSparse(Queue queue, uint32_t bindInfoCount, const BindSparseInfo& pBindInfo, Fence fence = {})
{
	return static_cast<Result>(vkQueueBindSparse((VkQueue)(queue), bindInfoCount, (const VkBindSparseInfo*)(&pBindInfo), (VkFence)(fence)));
}
inline Result queueBindSparse(Queue queue, nytl::Span<const BindSparseInfo> pBindInfo, Fence fence = {})
{
	return VPP_CALL(vkQueueBindSparse((VkQueue)(queue), pBindInfo.size(), (const VkBindSparseInfo*)(pBindInfo.data()), (VkFence)(fence)));
}

inline Fence createFence(Device device, const FenceCreateInfo& pCreateInfo, const AllocationCallbacks* pAllocator = {})
{
	Fence ret = {};
	VPP_CALL(vkCreateFence((VkDevice)(device), (const VkFenceCreateInfo*)(&pCreateInfo), (const VkAllocationCallbacks*)(pAllocator), (VkFence*)(&ret)));
	return ret;
}

inline void destroyFence(Device device, Fence fence = {}, const AllocationCallbacks* pAllocator = {})
{
	return vkDestroyFence((VkDevice)(device), (VkFence)(fence), (const VkAllocationCallbacks*)(pAllocator));
}

inline Result resetFences(Device device, uint32_t fenceCount, const Fence& pFences)
{
	return static_cast<Result>(vkResetFences((VkDevice)(device), fenceCount, (const VkFence*)(&pFences)));
}
inline Result resetFences(Device device, nytl::Span<const Fence> pFences)
{
	return VPP_CALL(vkResetFences((VkDevice)(device), pFences.size(), (const VkFence*)(pFences.data())));
}

inline Result getFenceStatus(Device device, Fence fence)
{
	return static_cast<Result>(vkGetFenceStatus((VkDevice)(device), (VkFence)(fence)));
}

inline Result waitForFences(Device device, uint32_t fenceCount, const Fence& pFences, Bool32 waitAll, uint64_t timeout)
{
	return static_cast<Result>(vkWaitForFences((VkDevice)(device), fenceCount, (const VkFence*)(&pFences), waitAll, timeout));
}
inline Result waitForFences(Device device, nytl::Span<const Fence> pFences, Bool32 waitAll, uint64_t timeout)
{
	return VPP_CALL(vkWaitForFences((VkDevice)(device), pFences.size(), (const VkFence*)(pFences.data()), waitAll, timeout));
}

inline Semaphore createSemaphore(Device device, const SemaphoreCreateInfo& pCreateInfo, const AllocationCallbacks* pAllocator = {})
{
	Semaphore ret = {};
	VPP_CALL(vkCreateSemaphore((VkDevice)(device), (const VkSemaphoreCreateInfo*)(&pCreateInfo), (const VkAllocationCallbacks*)(pAllocator), (VkSemaphore*)(&ret)));
	return ret;
}

inline void destroySemaphore(Device device, Semaphore semaphore = {}, const AllocationCallbacks* pAllocator = {})
{
	return vkDestroySemaphore((VkDevice)(device), (VkSemaphore)(semaphore), (const VkAllocationCallbacks*)(pAllocator));
}

inline Event createEvent(Device device, const EventCreateInfo& pCreateInfo, const AllocationCallbacks* pAllocator = {})
{
	Event ret = {};
	VPP_CALL(vkCreateEvent((VkDevice)(device), (const VkEventCreateInfo*)(&pCreateInfo), (const VkAllocationCallbacks*)(pAllocator), (VkEvent*)(&ret)));
	return ret;
}

inline void destroyEvent(Device device, Event event = {}, const AllocationCallbacks* pAllocator = {})
{
	return vkDestroyEvent((VkDevice)(device), (VkEvent)(event), (const VkAllocationCallbacks*)(pAllocator));
}

inline Result getEventStatus(Device device, Event event)
{
	return static_cast<Result>(vkGetEventStatus((VkDevice)(device), (VkEvent)(event)));
}

inline Result setEvent(Device device, Event event)
{
	return static_cast<Result>(vkSetEvent((VkDevice)(device), (VkEvent)(event)));
}

inline Result resetEvent(Device device, Event event)
{
	return static_cast<Result>(vkResetEvent((VkDevice)(device), (VkEvent)(event)));
}

inline QueryPool createQueryPool(Device device, const QueryPoolCreateInfo& pCreateInfo, const AllocationCallbacks* pAllocator = {})
{
	QueryPool ret = {};
	VPP_CALL(vkCreateQueryPool((VkDevice)(device), (const VkQueryPoolCreateInfo*)(&pCreateInfo), (const VkAllocationCallbacks*)(pAllocator), (VkQueryPool*)(&ret)));
	return ret;
}

inline void destroyQueryPool(Device device, QueryPool queryPool = {}, const AllocationCallbacks* pAllocator = {})
{
	return vkDestroyQueryPool((VkDevice)(device), (VkQueryPool)(queryPool), (const VkAllocationCallbacks*)(pAllocator));
}

inline Result getQueryPoolResults(Device device, QueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, size_t dataSize, void* pData, DeviceSize stride, QueryResultFlags flags = {})
{
	return static_cast<Result>(vkGetQueryPoolResults((VkDevice)(device), (VkQueryPool)(queryPool), firstQuery, queryCount, dataSize, (void*)(pData), stride, static_cast<VkQueryResultFlags>(flags)));
}
inline std::vector<uint8_t> getQueryPoolResults(Device device, QueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, size_t dataSize, DeviceSize stride, QueryResultFlags flags = {})
{
	std::vector<uint8_t> ret;
	ret.resize(dataSize);
	VPP_CALL(vkGetQueryPoolResults((VkDevice)(device), (VkQueryPool)(queryPool), firstQuery, queryCount, dataSize, (void*)(ret.data()), stride, static_cast<VkQueryResultFlags>(flags)));
	return ret;
}

inline Buffer createBuffer(Device device, const BufferCreateInfo& pCreateInfo, const AllocationCallbacks* pAllocator = {})
{
	Buffer ret = {};
	VPP_CALL(vkCreateBuffer((VkDevice)(device), (const VkBufferCreateInfo*)(&pCreateInfo), (const VkAllocationCallbacks*)(pAllocator), (VkBuffer*)(&ret)));
	return ret;
}

inline void destroyBuffer(Device device, Buffer buffer = {}, const AllocationCallbacks* pAllocator = {})
{
	return vkDestroyBuffer((VkDevice)(device), (VkBuffer)(buffer), (const VkAllocationCallbacks*)(pAllocator));
}

inline BufferView createBufferView(Device device, const BufferViewCreateInfo& pCreateInfo, const AllocationCallbacks* pAllocator = {})
{
	BufferView ret = {};
	VPP_CALL(vkCreateBufferView((VkDevice)(device), (const VkBufferViewCreateInfo*)(&pCreateInfo), (const VkAllocationCallbacks*)(pAllocator), (VkBufferView*)(&ret)));
	return ret;
}

inline void destroyBufferView(Device device, BufferView bufferView = {}, const AllocationCallbacks* pAllocator = {})
{
	return vkDestroyBufferView((VkDevice)(device), (VkBufferView)(bufferView), (const VkAllocationCallbacks*)(pAllocator));
}

inline Image createImage(Device device, const ImageCreateInfo& pCreateInfo, const AllocationCallbacks* pAllocator = {})
{
	Image ret = {};
	VPP_CALL(vkCreateImage((VkDevice)(device), (const VkImageCreateInfo*)(&pCreateInfo), (const VkAllocationCallbacks*)(pAllocator), (VkImage*)(&ret)));
	return ret;
}

inline void destroyImage(Device device, Image image = {}, const AllocationCallbacks* pAllocator = {})
{
	return vkDestroyImage((VkDevice)(device), (VkImage)(image), (const VkAllocationCallbacks*)(pAllocator));
}

inline SubresourceLayout getImageSubresourceLayout(Device device, Image image, const ImageSubresource& pSubresource)
{
	SubresourceLayout ret = {};
	vkGetImageSubresourceLayout((VkDevice)(device), (VkImage)(image), (const VkImageSubresource*)(&pSubresource), (VkSubresourceLayout*)(&ret));
	return ret;
}

inline ImageView createImageView(Device device, const ImageViewCreateInfo& pCreateInfo, const AllocationCallbacks* pAllocator = {})
{
	ImageView ret = {};
	VPP_CALL(vkCreateImageView((VkDevice)(device), (const VkImageViewCreateInfo*)(&pCreateInfo), (const VkAllocationCallbacks*)(pAllocator), (VkImageView*)(&ret)));
	return ret;
}

inline void destroyImageView(Device device, ImageView imageView = {}, const AllocationCallbacks* pAllocator = {})
{
	return vkDestroyImageView((VkDevice)(device), (VkImageView)(imageView), (const VkAllocationCallbacks*)(pAllocator));
}

inline ShaderModule createShaderModule(Device device, const ShaderModuleCreateInfo& pCreateInfo, const AllocationCallbacks* pAllocator = {})
{
	ShaderModule ret = {};
	VPP_CALL(vkCreateShaderModule((VkDevice)(device), (const VkShaderModuleCreateInfo*)(&pCreateInfo), (const VkAllocationCallbacks*)(pAllocator), (VkShaderModule*)(&ret)));
	return ret;
}

inline void destroyShaderModule(Device device, ShaderModule shaderModule = {}, const AllocationCallbacks* pAllocator = {})
{
	return vkDestroyShaderModule((VkDevice)(device), (VkShaderModule)(shaderModule), (const VkAllocationCallbacks*)(pAllocator));
}

inline PipelineCache createPipelineCache(Device device, const PipelineCacheCreateInfo& pCreateInfo, const AllocationCallbacks* pAllocator = {})
{
	PipelineCache ret = {};
	VPP_CALL(vkCreatePipelineCache((VkDevice)(device), (const VkPipelineCacheCreateInfo*)(&pCreateInfo), (const VkAllocationCallbacks*)(pAllocator), (VkPipelineCache*)(&ret)));
	return ret;
}

inline void destroyPipelineCache(Device device, PipelineCache pipelineCache = {}, const AllocationCallbacks* pAllocator = {})
{
	return vkDestroyPipelineCache((VkDevice)(device), (VkPipelineCache)(pipelineCache), (const VkAllocationCallbacks*)(pAllocator));
}

inline Result getPipelineCacheData(Device device, PipelineCache pipelineCache, size_t& pDataSize, void* pData = {})
{
	return static_cast<Result>(vkGetPipelineCacheData((VkDevice)(device), (VkPipelineCache)(pipelineCache), (size_t*)(&pDataSize), (void*)(pData)));
}
inline std::vector<uint8_t> getPipelineCacheData(Device device, PipelineCache pipelineCache)
{
	std::vector<uint8_t> ret;
	size_t count = 0u;
	if(!error::success(VPP_CALL(vkGetPipelineCacheData((VkDevice)(device), (VkPipelineCache)(pipelineCache), &count, (void*)(ret.data()))))) return ret;
	ret.resize(count);
	VPP_CALL(vkGetPipelineCacheData((VkDevice)(device), (VkPipelineCache)(pipelineCache), &count, (void*)(ret.data())));
	return ret;
}

inline Result mergePipelineCaches(Device device, PipelineCache dstCache, uint32_t srcCacheCount, const PipelineCache& pSrcCaches)
{
	return static_cast<Result>(vkMergePipelineCaches((VkDevice)(device), (VkPipelineCache)(dstCache), srcCacheCount, (const VkPipelineCache*)(&pSrcCaches)));
}
inline Result mergePipelineCaches(Device device, PipelineCache dstCache, nytl::Span<const PipelineCache> pSrcCaches)
{
	return VPP_CALL(vkMergePipelineCaches((VkDevice)(device), (VkPipelineCache)(dstCache), pSrcCaches.size(), (const VkPipelineCache*)(pSrcCaches.data())));
}

inline Result createGraphicsPipelines(Device device, PipelineCache pipelineCache, uint32_t createInfoCount, const GraphicsPipelineCreateInfo& pCreateInfos, const AllocationCallbacks* pAllocator, Pipeline& pPipelines)
{
	return static_cast<Result>(vkCreateGraphicsPipelines((VkDevice)(device), (VkPipelineCache)(pipelineCache), createInfoCount, (const VkGraphicsPipelineCreateInfo*)(&pCreateInfos), (const VkAllocationCallbacks*)(pAllocator), (VkPipeline*)(&pPipelines)));
}
inline std::vector<Pipeline> createGraphicsPipelines(Device device, PipelineCache pipelineCache, nytl::Span<const GraphicsPipelineCreateInfo> pCreateInfos, const AllocationCallbacks* pAllocator = {})
{
	std::vector<Pipeline> ret;
	ret.resize(pCreateInfos.size());
	VPP_CALL(vkCreateGraphicsPipelines((VkDevice)(device), (VkPipelineCache)(pipelineCache), pCreateInfos.size(), (const VkGraphicsPipelineCreateInfo*)(pCreateInfos.data()), (const VkAllocationCallbacks*)(pAllocator), (VkPipeline*)(ret.data())));
	return ret;
}

inline Result createComputePipelines(Device device, PipelineCache pipelineCache, uint32_t createInfoCount, const ComputePipelineCreateInfo& pCreateInfos, const AllocationCallbacks* pAllocator, Pipeline& pPipelines)
{
	return static_cast<Result>(vkCreateComputePipelines((VkDevice)(device), (VkPipelineCache)(pipelineCache), createInfoCount, (const VkComputePipelineCreateInfo*)(&pCreateInfos), (const VkAllocationCallbacks*)(pAllocator), (VkPipeline*)(&pPipelines)));
}
inline std::vector<Pipeline> createComputePipelines(Device device, PipelineCache pipelineCache, nytl::Span<const ComputePipelineCreateInfo> pCreateInfos, const AllocationCallbacks* pAllocator = {})
{
	std::vector<Pipeline> ret;
	ret.resize(pCreateInfos.size());
	VPP_CALL(vkCreateComputePipelines((VkDevice)(device), (VkPipelineCache)(pipelineCache), pCreateInfos.size(), (const VkComputePipelineCreateInfo*)(pCreateInfos.data()), (const VkAllocationCallbacks*)(pAllocator), (VkPipeline*)(ret.data())));
	return ret;
}

inline void destroyPipeline(Device device, Pipeline pipeline = {}, const AllocationCallbacks* pAllocator = {})
{
	return vkDestroyPipeline((VkDevice)(device), (VkPipeline)(pipeline), (const VkAllocationCallbacks*)(pAllocator));
}

inline PipelineLayout createPipelineLayout(Device device, const PipelineLayoutCreateInfo& pCreateInfo, const AllocationCallbacks* pAllocator = {})
{
	PipelineLayout ret = {};
	VPP_CALL(vkCreatePipelineLayout((VkDevice)(device), (const VkPipelineLayoutCreateInfo*)(&pCreateInfo), (const VkAllocationCallbacks*)(pAllocator), (VkPipelineLayout*)(&ret)));
	return ret;
}

inline void destroyPipelineLayout(Device device, PipelineLayout pipelineLayout = {}, const AllocationCallbacks* pAllocator = {})
{
	return vkDestroyPipelineLayout((VkDevice)(device), (VkPipelineLayout)(pipelineLayout), (const VkAllocationCallbacks*)(pAllocator));
}

inline Sampler createSampler(Device device, const SamplerCreateInfo& pCreateInfo, const AllocationCallbacks* pAllocator = {})
{
	Sampler ret = {};
	VPP_CALL(vkCreateSampler((VkDevice)(device), (const VkSamplerCreateInfo*)(&pCreateInfo), (const VkAllocationCallbacks*)(pAllocator), (VkSampler*)(&ret)));
	return ret;
}

inline void destroySampler(Device device, Sampler sampler = {}, const AllocationCallbacks* pAllocator = {})
{
	return vkDestroySampler((VkDevice)(device), (VkSampler)(sampler), (const VkAllocationCallbacks*)(pAllocator));
}

inline DescriptorSetLayout createDescriptorSetLayout(Device device, const DescriptorSetLayoutCreateInfo& pCreateInfo, const AllocationCallbacks* pAllocator = {})
{
	DescriptorSetLayout ret = {};
	VPP_CALL(vkCreateDescriptorSetLayout((VkDevice)(device), (const VkDescriptorSetLayoutCreateInfo*)(&pCreateInfo), (const VkAllocationCallbacks*)(pAllocator), (VkDescriptorSetLayout*)(&ret)));
	return ret;
}

inline void destroyDescriptorSetLayout(Device device, DescriptorSetLayout descriptorSetLayout = {}, const AllocationCallbacks* pAllocator = {})
{
	return vkDestroyDescriptorSetLayout((VkDevice)(device), (VkDescriptorSetLayout)(descriptorSetLayout), (const VkAllocationCallbacks*)(pAllocator));
}

inline DescriptorPool createDescriptorPool(Device device, const DescriptorPoolCreateInfo& pCreateInfo, const AllocationCallbacks* pAllocator = {})
{
	DescriptorPool ret = {};
	VPP_CALL(vkCreateDescriptorPool((VkDevice)(device), (const VkDescriptorPoolCreateInfo*)(&pCreateInfo), (const VkAllocationCallbacks*)(pAllocator), (VkDescriptorPool*)(&ret)));
	return ret;
}

inline void destroyDescriptorPool(Device device, DescriptorPool descriptorPool = {}, const AllocationCallbacks* pAllocator = {})
{
	return vkDestroyDescriptorPool((VkDevice)(device), (VkDescriptorPool)(descriptorPool), (const VkAllocationCallbacks*)(pAllocator));
}

inline Result resetDescriptorPool(Device device, DescriptorPool descriptorPool, DescriptorPoolResetFlags flags = {})
{
	return static_cast<Result>(vkResetDescriptorPool((VkDevice)(device), (VkDescriptorPool)(descriptorPool), static_cast<VkDescriptorPoolResetFlags>(flags)));
}

inline Result allocateDescriptorSets(Device device, const DescriptorSetAllocateInfo& pAllocateInfo, DescriptorSet& pDescriptorSets)
{
	return static_cast<Result>(vkAllocateDescriptorSets((VkDevice)(device), (const VkDescriptorSetAllocateInfo*)(&pAllocateInfo), (VkDescriptorSet*)(&pDescriptorSets)));
}
inline std::vector<DescriptorSet> allocateDescriptorSets(Device device, const DescriptorSetAllocateInfo& pAllocateInfo)
{
	std::vector<DescriptorSet> ret;
	ret.resize(pAllocateInfo.descriptorSetCount);
	VPP_CALL(vkAllocateDescriptorSets((VkDevice)(device), (const VkDescriptorSetAllocateInfo*)(&pAllocateInfo), (VkDescriptorSet*)(ret.data())));
	return ret;
}

inline Result freeDescriptorSets(Device device, DescriptorPool descriptorPool, uint32_t descriptorSetCount, const DescriptorSet& pDescriptorSets)
{
	return static_cast<Result>(vkFreeDescriptorSets((VkDevice)(device), (VkDescriptorPool)(descriptorPool), descriptorSetCount, (const VkDescriptorSet*)(&pDescriptorSets)));
}
inline Result freeDescriptorSets(Device device, DescriptorPool descriptorPool, nytl::Span<const DescriptorSet> pDescriptorSets)
{
	return VPP_CALL(vkFreeDescriptorSets((VkDevice)(device), (VkDescriptorPool)(descriptorPool), pDescriptorSets.size(), (const VkDescriptorSet*)(pDescriptorSets.data())));
}

inline void updateDescriptorSets(Device device, uint32_t descriptorWriteCount, const WriteDescriptorSet& pDescriptorWrites, uint32_t descriptorCopyCount, const CopyDescriptorSet& pDescriptorCopies)
{
	return vkUpdateDescriptorSets((VkDevice)(device), descriptorWriteCount, (const VkWriteDescriptorSet*)(&pDescriptorWrites), descriptorCopyCount, (const VkCopyDescriptorSet*)(&pDescriptorCopies));
}
inline void updateDescriptorSets(Device device, nytl::Span<const WriteDescriptorSet> pDescriptorWrites, nytl::Span<const CopyDescriptorSet> pDescriptorCopies)
{
	vkUpdateDescriptorSets((VkDevice)(device), pDescriptorWrites.size(), (const VkWriteDescriptorSet*)(pDescriptorWrites.data()), pDescriptorCopies.size(), (const VkCopyDescriptorSet*)(pDescriptorCopies.data()));
}

inline Framebuffer createFramebuffer(Device device, const FramebufferCreateInfo& pCreateInfo, const AllocationCallbacks* pAllocator = {})
{
	Framebuffer ret = {};
	VPP_CALL(vkCreateFramebuffer((VkDevice)(device), (const VkFramebufferCreateInfo*)(&pCreateInfo), (const VkAllocationCallbacks*)(pAllocator), (VkFramebuffer*)(&ret)));
	return ret;
}

inline void destroyFramebuffer(Device device, Framebuffer framebuffer = {}, const AllocationCallbacks* pAllocator = {})
{
	return vkDestroyFramebuffer((VkDevice)(device), (VkFramebuffer)(framebuffer), (const VkAllocationCallbacks*)(pAllocator));
}

inline RenderPass createRenderPass(Device device, const RenderPassCreateInfo& pCreateInfo, const AllocationCallbacks* pAllocator = {})
{
	RenderPass ret = {};
	VPP_CALL(vkCreateRenderPass((VkDevice)(device), (const VkRenderPassCreateInfo*)(&pCreateInfo), (const VkAllocationCallbacks*)(pAllocator), (VkRenderPass*)(&ret)));
	return ret;
}

inline void destroyRenderPass(Device device, RenderPass renderPass = {}, const AllocationCallbacks* pAllocator = {})
{
	return vkDestroyRenderPass((VkDevice)(device), (VkRenderPass)(renderPass), (const VkAllocationCallbacks*)(pAllocator));
}

inline void getRenderAreaGranularity(Device device, RenderPass renderPass, Extent2D& pGranularity)
{
	return vkGetRenderAreaGranularity((VkDevice)(device), (VkRenderPass)(renderPass), (VkExtent2D*)(&pGranularity));
}

inline CommandPool createCommandPool(Device device, const CommandPoolCreateInfo& pCreateInfo, const AllocationCallbacks* pAllocator = {})
{
	CommandPool ret = {};
	VPP_CALL(vkCreateCommandPool((VkDevice)(device), (const VkCommandPoolCreateInfo*)(&pCreateInfo), (const VkAllocationCallbacks*)(pAllocator), (VkCommandPool*)(&ret)));
	return ret;
}

inline void destroyCommandPool(Device device, CommandPool commandPool = {}, const AllocationCallbacks* pAllocator = {})
{
	return vkDestroyCommandPool((VkDevice)(device), (VkCommandPool)(commandPool), (const VkAllocationCallbacks*)(pAllocator));
}

inline Result resetCommandPool(Device device, CommandPool commandPool, CommandPoolResetFlags flags = {})
{
	return static_cast<Result>(vkResetCommandPool((VkDevice)(device), (VkCommandPool)(commandPool), static_cast<VkCommandPoolResetFlags>(flags)));
}

inline Result allocateCommandBuffers(Device device, const CommandBufferAllocateInfo& pAllocateInfo, CommandBuffer& pCommandBuffers)
{
	return static_cast<Result>(vkAllocateCommandBuffers((VkDevice)(device), (const VkCommandBufferAllocateInfo*)(&pAllocateInfo), (VkCommandBuffer*)(&pCommandBuffers)));
}
inline std::vector<CommandBuffer> allocateCommandBuffers(Device device, const CommandBufferAllocateInfo& pAllocateInfo)
{
	std::vector<CommandBuffer> ret;
	ret.resize(pAllocateInfo.commandBufferCount);
	VPP_CALL(vkAllocateCommandBuffers((VkDevice)(device), (const VkCommandBufferAllocateInfo*)(&pAllocateInfo), (VkCommandBuffer*)(ret.data())));
	return ret;
}

inline void freeCommandBuffers(Device device, CommandPool commandPool, uint32_t commandBufferCount, const CommandBuffer& pCommandBuffers)
{
	return vkFreeCommandBuffers((VkDevice)(device), (VkCommandPool)(commandPool), commandBufferCount, (const VkCommandBuffer*)(&pCommandBuffers));
}
inline void freeCommandBuffers(Device device, CommandPool commandPool, nytl::Span<const CommandBuffer> pCommandBuffers)
{
	vkFreeCommandBuffers((VkDevice)(device), (VkCommandPool)(commandPool), pCommandBuffers.size(), (const VkCommandBuffer*)(pCommandBuffers.data()));
}

inline Result beginCommandBuffer(CommandBuffer commandBuffer, const CommandBufferBeginInfo& pBeginInfo)
{
	return static_cast<Result>(vkBeginCommandBuffer((VkCommandBuffer)(commandBuffer), (const VkCommandBufferBeginInfo*)(&pBeginInfo)));
}

inline Result endCommandBuffer(CommandBuffer commandBuffer)
{
	return static_cast<Result>(vkEndCommandBuffer((VkCommandBuffer)(commandBuffer)));
}

inline Result resetCommandBuffer(CommandBuffer commandBuffer, CommandBufferResetFlags flags = {})
{
	return static_cast<Result>(vkResetCommandBuffer((VkCommandBuffer)(commandBuffer), static_cast<VkCommandBufferResetFlags>(flags)));
}

inline void cmdBindPipeline(CommandBuffer commandBuffer, PipelineBindPoint pipelineBindPoint, Pipeline pipeline)
{
	return vkCmdBindPipeline((VkCommandBuffer)(commandBuffer), static_cast<VkPipelineBindPoint>(pipelineBindPoint), (VkPipeline)(pipeline));
}

inline void cmdSetViewport(CommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const Viewport& pViewports)
{
	return vkCmdSetViewport((VkCommandBuffer)(commandBuffer), firstViewport, viewportCount, (const VkViewport*)(&pViewports));
}
inline void cmdSetViewport(CommandBuffer commandBuffer, uint32_t firstViewport, nytl::Span<const Viewport> pViewports)
{
	vkCmdSetViewport((VkCommandBuffer)(commandBuffer), firstViewport, pViewports.size(), (const VkViewport*)(pViewports.data()));
}

inline void cmdSetScissor(CommandBuffer commandBuffer, uint32_t firstScissor, uint32_t scissorCount, const Rect2D& pScissors)
{
	return vkCmdSetScissor((VkCommandBuffer)(commandBuffer), firstScissor, scissorCount, (const VkRect2D*)(&pScissors));
}
inline void cmdSetScissor(CommandBuffer commandBuffer, uint32_t firstScissor, nytl::Span<const Rect2D> pScissors)
{
	vkCmdSetScissor((VkCommandBuffer)(commandBuffer), firstScissor, pScissors.size(), (const VkRect2D*)(pScissors.data()));
}

inline void cmdSetLineWidth(CommandBuffer commandBuffer, float lineWidth)
{
	return vkCmdSetLineWidth((VkCommandBuffer)(commandBuffer), lineWidth);
}

inline void cmdSetDepthBias(CommandBuffer commandBuffer, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor)
{
	return vkCmdSetDepthBias((VkCommandBuffer)(commandBuffer), depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor);
}

inline void cmdSetBlendConstants(CommandBuffer commandBuffer, std::array<const float, 4> blendConstants)
{
	return vkCmdSetBlendConstants((VkCommandBuffer)(commandBuffer), blendConstants.data());
}

inline void cmdSetDepthBounds(CommandBuffer commandBuffer, float minDepthBounds, float maxDepthBounds)
{
	return vkCmdSetDepthBounds((VkCommandBuffer)(commandBuffer), minDepthBounds, maxDepthBounds);
}

inline void cmdSetStencilCompareMask(CommandBuffer commandBuffer, StencilFaceFlags faceMask, uint32_t compareMask)
{
	return vkCmdSetStencilCompareMask((VkCommandBuffer)(commandBuffer), static_cast<VkStencilFaceFlags>(faceMask), compareMask);
}

inline void cmdSetStencilWriteMask(CommandBuffer commandBuffer, StencilFaceFlags faceMask, uint32_t writeMask)
{
	return vkCmdSetStencilWriteMask((VkCommandBuffer)(commandBuffer), static_cast<VkStencilFaceFlags>(faceMask), writeMask);
}

inline void cmdSetStencilReference(CommandBuffer commandBuffer, StencilFaceFlags faceMask, uint32_t reference)
{
	return vkCmdSetStencilReference((VkCommandBuffer)(commandBuffer), static_cast<VkStencilFaceFlags>(faceMask), reference);
}

inline void cmdBindDescriptorSets(CommandBuffer commandBuffer, PipelineBindPoint pipelineBindPoint, PipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, const DescriptorSet& pDescriptorSets, uint32_t dynamicOffsetCount, const uint32_t& pDynamicOffsets)
{
	return vkCmdBindDescriptorSets((VkCommandBuffer)(commandBuffer), static_cast<VkPipelineBindPoint>(pipelineBindPoint), (VkPipelineLayout)(layout), firstSet, descriptorSetCount, (const VkDescriptorSet*)(&pDescriptorSets), dynamicOffsetCount, (const uint32_t*)(&pDynamicOffsets));
}
inline void cmdBindDescriptorSets(CommandBuffer commandBuffer, PipelineBindPoint pipelineBindPoint, PipelineLayout layout, uint32_t firstSet, nytl::Span<const DescriptorSet> pDescriptorSets, nytl::Span<const uint32_t> pDynamicOffsets)
{
	vkCmdBindDescriptorSets((VkCommandBuffer)(commandBuffer), static_cast<VkPipelineBindPoint>(pipelineBindPoint), (VkPipelineLayout)(layout), firstSet, pDescriptorSets.size(), (const VkDescriptorSet*)(pDescriptorSets.data()), pDynamicOffsets.size(), (const uint32_t*)(pDynamicOffsets.data()));
}

inline void cmdBindIndexBuffer(CommandBuffer commandBuffer, Buffer buffer, DeviceSize offset, IndexType indexType)
{
	return vkCmdBindIndexBuffer((VkCommandBuffer)(commandBuffer), (VkBuffer)(buffer), offset, static_cast<VkIndexType>(indexType));
}

inline void cmdBindVertexBuffers(CommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const Buffer& pBuffers, const DeviceSize& pOffsets)
{
	return vkCmdBindVertexBuffers((VkCommandBuffer)(commandBuffer), firstBinding, bindingCount, (const VkBuffer*)(&pBuffers), (const VkDeviceSize*)(&pOffsets));
}
inline void cmdBindVertexBuffers(CommandBuffer commandBuffer, uint32_t firstBinding, nytl::Span<const Buffer> pBuffers, nytl::Span<const DeviceSize> pOffsets)
{
	vkCmdBindVertexBuffers((VkCommandBuffer)(commandBuffer), firstBinding, pBuffers.size(), (const VkBuffer*)(pBuffers.data()), (const VkDeviceSize*)(pOffsets.data()));
}

inline void cmdDraw(CommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance)
{
	return vkCmdDraw((VkCommandBuffer)(commandBuffer), vertexCount, instanceCount, firstVertex, firstInstance);
}

inline void cmdDrawIndexed(CommandBuffer commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance)
{
	return vkCmdDrawIndexed((VkCommandBuffer)(commandBuffer), indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
}

inline void cmdDrawIndirect(CommandBuffer commandBuffer, Buffer buffer, DeviceSize offset, uint32_t drawCount, uint32_t stride)
{
	return vkCmdDrawIndirect((VkCommandBuffer)(commandBuffer), (VkBuffer)(buffer), offset, drawCount, stride);
}

inline void cmdDrawIndexedIndirect(CommandBuffer commandBuffer, Buffer buffer, DeviceSize offset, uint32_t drawCount, uint32_t stride)
{
	return vkCmdDrawIndexedIndirect((VkCommandBuffer)(commandBuffer), (VkBuffer)(buffer), offset, drawCount, stride);
}

inline void cmdDispatch(CommandBuffer commandBuffer, uint32_t x, uint32_t y, uint32_t z)
{
	return vkCmdDispatch((VkCommandBuffer)(commandBuffer), x, y, z);
}

inline void cmdDispatchIndirect(CommandBuffer commandBuffer, Buffer buffer, DeviceSize offset)
{
	return vkCmdDispatchIndirect((VkCommandBuffer)(commandBuffer), (VkBuffer)(buffer), offset);
}

inline void cmdCopyBuffer(CommandBuffer commandBuffer, Buffer srcBuffer, Buffer dstBuffer, uint32_t regionCount, const BufferCopy& pRegions)
{
	return vkCmdCopyBuffer((VkCommandBuffer)(commandBuffer), (VkBuffer)(srcBuffer), (VkBuffer)(dstBuffer), regionCount, (const VkBufferCopy*)(&pRegions));
}
inline void cmdCopyBuffer(CommandBuffer commandBuffer, Buffer srcBuffer, Buffer dstBuffer, nytl::Span<const BufferCopy> pRegions)
{
	vkCmdCopyBuffer((VkCommandBuffer)(commandBuffer), (VkBuffer)(srcBuffer), (VkBuffer)(dstBuffer), pRegions.size(), (const VkBufferCopy*)(pRegions.data()));
}

inline void cmdCopyImage(CommandBuffer commandBuffer, Image srcImage, ImageLayout srcImageLayout, Image dstImage, ImageLayout dstImageLayout, uint32_t regionCount, const ImageCopy& pRegions)
{
	return vkCmdCopyImage((VkCommandBuffer)(commandBuffer), (VkImage)(srcImage), static_cast<VkImageLayout>(srcImageLayout), (VkImage)(dstImage), static_cast<VkImageLayout>(dstImageLayout), regionCount, (const VkImageCopy*)(&pRegions));
}
inline void cmdCopyImage(CommandBuffer commandBuffer, Image srcImage, ImageLayout srcImageLayout, Image dstImage, ImageLayout dstImageLayout, nytl::Span<const ImageCopy> pRegions)
{
	vkCmdCopyImage((VkCommandBuffer)(commandBuffer), (VkImage)(srcImage), static_cast<VkImageLayout>(srcImageLayout), (VkImage)(dstImage), static_cast<VkImageLayout>(dstImageLayout), pRegions.size(), (const VkImageCopy*)(pRegions.data()));
}

inline void cmdBlitImage(CommandBuffer commandBuffer, Image srcImage, ImageLayout srcImageLayout, Image dstImage, ImageLayout dstImageLayout, uint32_t regionCount, const ImageBlit& pRegions, Filter filter)
{
	return vkCmdBlitImage((VkCommandBuffer)(commandBuffer), (VkImage)(srcImage), static_cast<VkImageLayout>(srcImageLayout), (VkImage)(dstImage), static_cast<VkImageLayout>(dstImageLayout), regionCount, (const VkImageBlit*)(&pRegions), static_cast<VkFilter>(filter));
}
inline void cmdBlitImage(CommandBuffer commandBuffer, Image srcImage, ImageLayout srcImageLayout, Image dstImage, ImageLayout dstImageLayout, nytl::Span<const ImageBlit> pRegions, Filter filter)
{
	vkCmdBlitImage((VkCommandBuffer)(commandBuffer), (VkImage)(srcImage), static_cast<VkImageLayout>(srcImageLayout), (VkImage)(dstImage), static_cast<VkImageLayout>(dstImageLayout), pRegions.size(), (const VkImageBlit*)(pRegions.data()), static_cast<VkFilter>(filter));
}

inline void cmdCopyBufferToImage(CommandBuffer commandBuffer, Buffer srcBuffer, Image dstImage, ImageLayout dstImageLayout, uint32_t regionCount, const BufferImageCopy& pRegions)
{
	return vkCmdCopyBufferToImage((VkCommandBuffer)(commandBuffer), (VkBuffer)(srcBuffer), (VkImage)(dstImage), static_cast<VkImageLayout>(dstImageLayout), regionCount, (const VkBufferImageCopy*)(&pRegions));
}
inline void cmdCopyBufferToImage(CommandBuffer commandBuffer, Buffer srcBuffer, Image dstImage, ImageLayout dstImageLayout, nytl::Span<const BufferImageCopy> pRegions)
{
	vkCmdCopyBufferToImage((VkCommandBuffer)(commandBuffer), (VkBuffer)(srcBuffer), (VkImage)(dstImage), static_cast<VkImageLayout>(dstImageLayout), pRegions.size(), (const VkBufferImageCopy*)(pRegions.data()));
}

inline void cmdCopyImageToBuffer(CommandBuffer commandBuffer, Image srcImage, ImageLayout srcImageLayout, Buffer dstBuffer, uint32_t regionCount, const BufferImageCopy& pRegions)
{
	return vkCmdCopyImageToBuffer((VkCommandBuffer)(commandBuffer), (VkImage)(srcImage), static_cast<VkImageLayout>(srcImageLayout), (VkBuffer)(dstBuffer), regionCount, (const VkBufferImageCopy*)(&pRegions));
}
inline void cmdCopyImageToBuffer(CommandBuffer commandBuffer, Image srcImage, ImageLayout srcImageLayout, Buffer dstBuffer, nytl::Span<const BufferImageCopy> pRegions)
{
	vkCmdCopyImageToBuffer((VkCommandBuffer)(commandBuffer), (VkImage)(srcImage), static_cast<VkImageLayout>(srcImageLayout), (VkBuffer)(dstBuffer), pRegions.size(), (const VkBufferImageCopy*)(pRegions.data()));
}

inline void cmdUpdateBuffer(CommandBuffer commandBuffer, Buffer dstBuffer, DeviceSize dstOffset, DeviceSize dataSize, const void* pData)
{
	return vkCmdUpdateBuffer((VkCommandBuffer)(commandBuffer), (VkBuffer)(dstBuffer), dstOffset, dataSize, (const void*)(pData));
}
inline void cmdUpdateBuffer(CommandBuffer commandBuffer, Buffer dstBuffer, DeviceSize dstOffset, nytl::Span<const uint8_t> pData)
{
	vkCmdUpdateBuffer((VkCommandBuffer)(commandBuffer), (VkBuffer)(dstBuffer), dstOffset, pData.size(), (const void*)(pData.data()));
}

inline void cmdFillBuffer(CommandBuffer commandBuffer, Buffer dstBuffer, DeviceSize dstOffset, DeviceSize size, uint32_t data)
{
	return vkCmdFillBuffer((VkCommandBuffer)(commandBuffer), (VkBuffer)(dstBuffer), dstOffset, size, data);
}

inline void cmdClearColorImage(CommandBuffer commandBuffer, Image image, ImageLayout imageLayout, const ClearColorValue& pColor, uint32_t rangeCount, const ImageSubresourceRange& pRanges)
{
	return vkCmdClearColorImage((VkCommandBuffer)(commandBuffer), (VkImage)(image), static_cast<VkImageLayout>(imageLayout), (const VkClearColorValue*)(&pColor), rangeCount, (const VkImageSubresourceRange*)(&pRanges));
}
inline void cmdClearColorImage(CommandBuffer commandBuffer, Image image, ImageLayout imageLayout, const ClearColorValue& pColor, nytl::Span<const ImageSubresourceRange> pRanges)
{
	vkCmdClearColorImage((VkCommandBuffer)(commandBuffer), (VkImage)(image), static_cast<VkImageLayout>(imageLayout), (const VkClearColorValue*)(&pColor), pRanges.size(), (const VkImageSubresourceRange*)(pRanges.data()));
}

inline void cmdClearDepthStencilImage(CommandBuffer commandBuffer, Image image, ImageLayout imageLayout, const ClearDepthStencilValue& pDepthStencil, uint32_t rangeCount, const ImageSubresourceRange& pRanges)
{
	return vkCmdClearDepthStencilImage((VkCommandBuffer)(commandBuffer), (VkImage)(image), static_cast<VkImageLayout>(imageLayout), (const VkClearDepthStencilValue*)(&pDepthStencil), rangeCount, (const VkImageSubresourceRange*)(&pRanges));
}
inline void cmdClearDepthStencilImage(CommandBuffer commandBuffer, Image image, ImageLayout imageLayout, const ClearDepthStencilValue& pDepthStencil, nytl::Span<const ImageSubresourceRange> pRanges)
{
	vkCmdClearDepthStencilImage((VkCommandBuffer)(commandBuffer), (VkImage)(image), static_cast<VkImageLayout>(imageLayout), (const VkClearDepthStencilValue*)(&pDepthStencil), pRanges.size(), (const VkImageSubresourceRange*)(pRanges.data()));
}

inline void cmdClearAttachments(CommandBuffer commandBuffer, uint32_t attachmentCount, const ClearAttachment& pAttachments, uint32_t rectCount, const ClearRect& pRects)
{
	return vkCmdClearAttachments((VkCommandBuffer)(commandBuffer), attachmentCount, (const VkClearAttachment*)(&pAttachments), rectCount, (const VkClearRect*)(&pRects));
}
inline void cmdClearAttachments(CommandBuffer commandBuffer, nytl::Span<const ClearAttachment> pAttachments, nytl::Span<const ClearRect> pRects)
{
	vkCmdClearAttachments((VkCommandBuffer)(commandBuffer), pAttachments.size(), (const VkClearAttachment*)(pAttachments.data()), pRects.size(), (const VkClearRect*)(pRects.data()));
}

inline void cmdResolveImage(CommandBuffer commandBuffer, Image srcImage, ImageLayout srcImageLayout, Image dstImage, ImageLayout dstImageLayout, uint32_t regionCount, const ImageResolve& pRegions)
{
	return vkCmdResolveImage((VkCommandBuffer)(commandBuffer), (VkImage)(srcImage), static_cast<VkImageLayout>(srcImageLayout), (VkImage)(dstImage), static_cast<VkImageLayout>(dstImageLayout), regionCount, (const VkImageResolve*)(&pRegions));
}
inline void cmdResolveImage(CommandBuffer commandBuffer, Image srcImage, ImageLayout srcImageLayout, Image dstImage, ImageLayout dstImageLayout, nytl::Span<const ImageResolve> pRegions)
{
	vkCmdResolveImage((VkCommandBuffer)(commandBuffer), (VkImage)(srcImage), static_cast<VkImageLayout>(srcImageLayout), (VkImage)(dstImage), static_cast<VkImageLayout>(dstImageLayout), pRegions.size(), (const VkImageResolve*)(pRegions.data()));
}

inline void cmdSetEvent(CommandBuffer commandBuffer, Event event, PipelineStageFlags stageMask)
{
	return vkCmdSetEvent((VkCommandBuffer)(commandBuffer), (VkEvent)(event), static_cast<VkPipelineStageFlags>(stageMask));
}

inline void cmdResetEvent(CommandBuffer commandBuffer, Event event, PipelineStageFlags stageMask)
{
	return vkCmdResetEvent((VkCommandBuffer)(commandBuffer), (VkEvent)(event), static_cast<VkPipelineStageFlags>(stageMask));
}

inline void cmdWaitEvents(CommandBuffer commandBuffer, uint32_t eventCount, const Event& pEvents, PipelineStageFlags srcStageMask, PipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, const MemoryBarrier& pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const BufferMemoryBarrier& pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const ImageMemoryBarrier& pImageMemoryBarriers)
{
	return vkCmdWaitEvents((VkCommandBuffer)(commandBuffer), eventCount, (const VkEvent*)(&pEvents), static_cast<VkPipelineStageFlags>(srcStageMask), static_cast<VkPipelineStageFlags>(dstStageMask), memoryBarrierCount, (const VkMemoryBarrier*)(&pMemoryBarriers), bufferMemoryBarrierCount, (const VkBufferMemoryBarrier*)(&pBufferMemoryBarriers), imageMemoryBarrierCount, (const VkImageMemoryBarrier*)(&pImageMemoryBarriers));
}
inline void cmdWaitEvents(CommandBuffer commandBuffer, nytl::Span<const Event> pEvents, PipelineStageFlags srcStageMask, PipelineStageFlags dstStageMask, nytl::Span<const MemoryBarrier> pMemoryBarriers, nytl::Span<const BufferMemoryBarrier> pBufferMemoryBarriers, nytl::Span<const ImageMemoryBarrier> pImageMemoryBarriers)
{
	vkCmdWaitEvents((VkCommandBuffer)(commandBuffer), pEvents.size(), (const VkEvent*)(pEvents.data()), static_cast<VkPipelineStageFlags>(srcStageMask), static_cast<VkPipelineStageFlags>(dstStageMask), pMemoryBarriers.size(), (const VkMemoryBarrier*)(pMemoryBarriers.data()), pBufferMemoryBarriers.size(), (const VkBufferMemoryBarrier*)(pBufferMemoryBarriers.data()), pImageMemoryBarriers.size(), (const VkImageMemoryBarrier*)(pImageMemoryBarriers.data()));
}

inline void cmdPipelineBarrier(CommandBuffer commandBuffer, PipelineStageFlags srcStageMask, PipelineStageFlags dstStageMask, DependencyFlags dependencyFlags, uint32_t memoryBarrierCount, const MemoryBarrier& pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const BufferMemoryBarrier& pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const ImageMemoryBarrier& pImageMemoryBarriers)
{
	return vkCmdPipelineBarrier((VkCommandBuffer)(commandBuffer), static_cast<VkPipelineStageFlags>(srcStageMask), static_cast<VkPipelineStageFlags>(dstStageMask), static_cast<VkDependencyFlags>(dependencyFlags), memoryBarrierCount, (const VkMemoryBarrier*)(&pMemoryBarriers), bufferMemoryBarrierCount, (const VkBufferMemoryBarrier*)(&pBufferMemoryBarriers), imageMemoryBarrierCount, (const VkImageMemoryBarrier*)(&pImageMemoryBarriers));
}
inline void cmdPipelineBarrier(CommandBuffer commandBuffer, PipelineStageFlags srcStageMask, PipelineStageFlags dstStageMask, DependencyFlags dependencyFlags, nytl::Span<const MemoryBarrier> pMemoryBarriers, nytl::Span<const BufferMemoryBarrier> pBufferMemoryBarriers, nytl::Span<const ImageMemoryBarrier> pImageMemoryBarriers)
{
	vkCmdPipelineBarrier((VkCommandBuffer)(commandBuffer), static_cast<VkPipelineStageFlags>(srcStageMask), static_cast<VkPipelineStageFlags>(dstStageMask), static_cast<VkDependencyFlags>(dependencyFlags), pMemoryBarriers.size(), (const VkMemoryBarrier*)(pMemoryBarriers.data()), pBufferMemoryBarriers.size(), (const VkBufferMemoryBarrier*)(pBufferMemoryBarriers.data()), pImageMemoryBarriers.size(), (const VkImageMemoryBarrier*)(pImageMemoryBarriers.data()));
}

inline void cmdBeginQuery(CommandBuffer commandBuffer, QueryPool queryPool, uint32_t query, QueryControlFlags flags = {})
{
	return vkCmdBeginQuery((VkCommandBuffer)(commandBuffer), (VkQueryPool)(queryPool), query, static_cast<VkQueryControlFlags>(flags));
}

inline void cmdEndQuery(CommandBuffer commandBuffer, QueryPool queryPool, uint32_t query)
{
	return vkCmdEndQuery((VkCommandBuffer)(commandBuffer), (VkQueryPool)(queryPool), query);
}

inline void cmdResetQueryPool(CommandBuffer commandBuffer, QueryPool queryPool, uint32_t firstQuery, uint32_t queryCount)
{
	return vkCmdResetQueryPool((VkCommandBuffer)(commandBuffer), (VkQueryPool)(queryPool), firstQuery, queryCount);
}

inline void cmdWriteTimestamp(CommandBuffer commandBuffer, PipelineStageBits pipelineStage, QueryPool queryPool, uint32_t query)
{
	return vkCmdWriteTimestamp((VkCommandBuffer)(commandBuffer), static_cast<VkPipelineStageFlagBits>(pipelineStage), (VkQueryPool)(queryPool), query);
}

inline void cmdCopyQueryPoolResults(CommandBuffer commandBuffer, QueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, Buffer dstBuffer, DeviceSize dstOffset, DeviceSize stride, QueryResultFlags flags = {})
{
	return vkCmdCopyQueryPoolResults((VkCommandBuffer)(commandBuffer), (VkQueryPool)(queryPool), firstQuery, queryCount, (VkBuffer)(dstBuffer), dstOffset, stride, static_cast<VkQueryResultFlags>(flags));
}

inline void cmdPushConstants(CommandBuffer commandBuffer, PipelineLayout layout, ShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void* pValues)
{
	return vkCmdPushConstants((VkCommandBuffer)(commandBuffer), (VkPipelineLayout)(layout), static_cast<VkShaderStageFlags>(stageFlags), offset, size, (const void*)(pValues));
}
inline void cmdPushConstants(CommandBuffer commandBuffer, PipelineLayout layout, ShaderStageFlags stageFlags, uint32_t offset, nytl::Span<const uint8_t> pValues)
{
	vkCmdPushConstants((VkCommandBuffer)(commandBuffer), (VkPipelineLayout)(layout), static_cast<VkShaderStageFlags>(stageFlags), offset, pValues.size(), (const void*)(pValues.data()));
}

inline void cmdBeginRenderPass(CommandBuffer commandBuffer, const RenderPassBeginInfo& pRenderPassBegin, SubpassContents contents)
{
	return vkCmdBeginRenderPass((VkCommandBuffer)(commandBuffer), (const VkRenderPassBeginInfo*)(&pRenderPassBegin), static_cast<VkSubpassContents>(contents));
}

inline void cmdNextSubpass(CommandBuffer commandBuffer, SubpassContents contents)
{
	return vkCmdNextSubpass((VkCommandBuffer)(commandBuffer), static_cast<VkSubpassContents>(contents));
}

inline void cmdEndRenderPass(CommandBuffer commandBuffer)
{
	return vkCmdEndRenderPass((VkCommandBuffer)(commandBuffer));
}

inline void cmdExecuteCommands(CommandBuffer commandBuffer, uint32_t commandBufferCount, const CommandBuffer& pCommandBuffers)
{
	return vkCmdExecuteCommands((VkCommandBuffer)(commandBuffer), commandBufferCount, (const VkCommandBuffer*)(&pCommandBuffers));
}
inline void cmdExecuteCommands(CommandBuffer commandBuffer, nytl::Span<const CommandBuffer> pCommandBuffers)
{
	vkCmdExecuteCommands((VkCommandBuffer)(commandBuffer), pCommandBuffers.size(), (const VkCommandBuffer*)(pCommandBuffers.data()));
}




} // namespace vk

// The specification (vk.xml) itself is protected by the following license:

// Copyright (c) 2015-2016 The Khronos Group Inc.
// 
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and/or associated documentation files (the
// "Materials"), to deal in the Materials without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Materials, and to
// permit persons to whom the Materials are furnished to do so, subject to
// the following conditions:
// 
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Materials.
// 
// THE MATERIALS ARE PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// MATERIALS OR THE USE OR OTHER DEALINGS IN THE MATERIALS.
// 
// ------------------------------------------------------------------------
// 
// This file, vk.xml, is the Vulkan API Registry. It is a critically important
// and normative part of the Vulkan Specification, including a canonical
// machine-readable definition of the API, parameter and member validation
// language incorporated into the Specification and reference pages, and other
// material which is registered by Khronos, such as tags used by extension and
// layer authors. The only authoritative version of vk.xml is the one
// maintained in the master branch of the Khronos Vulkan GitHub project.
    