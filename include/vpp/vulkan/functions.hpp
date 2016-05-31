// Copyright © 2016 nyorain
//
// Permission is hereby granted, free of charge, to any person
// obtaining a copy of this software and associated documentation
// files (the “Software”), to deal in the Software without
// restriction, including without limitation the rights to use,
// copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following
// conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.

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
// maintained in the master branch of the Khronos Vulkan Github project.

// Automaitcally generated vulkan header file for the nyorain/vpp library.
// Do not edit manually, rather edit the codegen files.

#pragma once

#include "fwd.hpp"
#include "enums.hpp"
#include "structs.hpp"
#include "call.hpp"

#include <vector>

namespace vk
{

inline Result createInstance(const InstanceCreateInfo& pCreateInfo, const AllocationCallbacks* pAllocator, Instance& pInstance)
{
	return VPP_CALL(static_cast<Result>(vkCreateInstance(reinterpret_cast<const VkInstanceCreateInfo*>(&pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkInstance*>(&pInstance))));
}
inline void destroyInstance(Instance instance, const AllocationCallbacks* pAllocator)
{
	vkDestroyInstance(instance, reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
inline Result enumeratePhysicalDevices(Instance instance, uint32_t& pPhysicalDeviceCount, PhysicalDevice* pPhysicalDevices)
{
	return VPP_CALL(static_cast<Result>(vkEnumeratePhysicalDevices(instance, reinterpret_cast<uint32_t*>(&pPhysicalDeviceCount), reinterpret_cast<VkPhysicalDevice*>(pPhysicalDevices))));
}
inline std::vector<PhysicalDevice> enumeratePhysicalDevices(Instance instance)
{
	std::vector<PhysicalDevice> ret;
	std::uint32_t count = 0u;
	if(!call::success(VPP_CALL(vkEnumeratePhysicalDevices(instance, &count, reinterpret_cast<VkPhysicalDevice*>(ret.data()))))) return ret;
	ret.resize(count);
	VPP_CALL(vkEnumeratePhysicalDevices(instance, &count, reinterpret_cast<VkPhysicalDevice*>(ret.data())));
	return ret;
}
inline void getPhysicalDeviceFeatures(PhysicalDevice physicalDevice, PhysicalDeviceFeatures& pFeatures)
{
	vkGetPhysicalDeviceFeatures(physicalDevice, reinterpret_cast<VkPhysicalDeviceFeatures*>(&pFeatures));
}
inline void getPhysicalDeviceFormatProperties(PhysicalDevice physicalDevice, Format format, FormatProperties& pFormatProperties)
{
	vkGetPhysicalDeviceFormatProperties(physicalDevice, static_cast<VkFormat>(format), reinterpret_cast<VkFormatProperties*>(&pFormatProperties));
}
inline Result getPhysicalDeviceImageFormatProperties(PhysicalDevice physicalDevice, Format format, ImageType type, ImageTiling tiling, ImageUsageFlags usage, ImageCreateFlags flags, ImageFormatProperties& pImageFormatProperties)
{
	return VPP_CALL(static_cast<Result>(vkGetPhysicalDeviceImageFormatProperties(physicalDevice, static_cast<VkFormat>(format), static_cast<VkImageType>(type), static_cast<VkImageTiling>(tiling), static_cast<VkImageUsageFlags>(usage), static_cast<VkImageCreateFlags>(flags), reinterpret_cast<VkImageFormatProperties*>(&pImageFormatProperties))));
}
inline void getPhysicalDeviceProperties(PhysicalDevice physicalDevice, PhysicalDeviceProperties& pProperties)
{
	vkGetPhysicalDeviceProperties(physicalDevice, reinterpret_cast<VkPhysicalDeviceProperties*>(&pProperties));
}
inline void getPhysicalDeviceQueueFamilyProperties(PhysicalDevice physicalDevice, uint32_t& pQueueFamilyPropertyCount, QueueFamilyProperties* pQueueFamilyProperties)
{
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, reinterpret_cast<uint32_t*>(&pQueueFamilyPropertyCount), reinterpret_cast<VkQueueFamilyProperties*>(pQueueFamilyProperties));
}
inline std::vector<QueueFamilyProperties> getPhysicalDeviceQueueFamilyProperties(PhysicalDevice physicalDevice)
{
	std::vector<QueueFamilyProperties> ret;
	std::uint32_t count = 0u;
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &count, reinterpret_cast<VkQueueFamilyProperties*>(ret.data()));
	ret.resize(count);
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &count, reinterpret_cast<VkQueueFamilyProperties*>(ret.data()));
	return ret;
}
inline void getPhysicalDeviceMemoryProperties(PhysicalDevice physicalDevice, PhysicalDeviceMemoryProperties& pMemoryProperties)
{
	vkGetPhysicalDeviceMemoryProperties(physicalDevice, reinterpret_cast<VkPhysicalDeviceMemoryProperties*>(&pMemoryProperties));
}
inline PfnVoidFunction getInstanceProcAddr(Instance instance, const char& pName)
{
	return static_cast<PfnVoidFunction>(vkGetInstanceProcAddr(instance, reinterpret_cast<const char*>(&pName)));
}
inline PfnVoidFunction getDeviceProcAddr(Device device, const char& pName)
{
	return static_cast<PfnVoidFunction>(vkGetDeviceProcAddr(device, reinterpret_cast<const char*>(&pName)));
}
inline Result createDevice(PhysicalDevice physicalDevice, const DeviceCreateInfo& pCreateInfo, const AllocationCallbacks* pAllocator, Device& pDevice)
{
	return VPP_CALL(static_cast<Result>(vkCreateDevice(physicalDevice, reinterpret_cast<const VkDeviceCreateInfo*>(&pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkDevice*>(&pDevice))));
}
inline void destroyDevice(Device device, const AllocationCallbacks* pAllocator)
{
	vkDestroyDevice(device, reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
inline Result enumerateInstanceExtensionProperties(const char* pLayerName, uint32_t& pPropertyCount, ExtensionProperties* pProperties)
{
	return VPP_CALL(static_cast<Result>(vkEnumerateInstanceExtensionProperties(reinterpret_cast<const char*>(pLayerName), reinterpret_cast<uint32_t*>(&pPropertyCount), reinterpret_cast<VkExtensionProperties*>(pProperties))));
}
inline std::vector<ExtensionProperties> enumerateInstanceExtensionProperties(const char* pLayerName)
{
	std::vector<ExtensionProperties> ret;
	std::uint32_t count = 0u;
	if(!call::success(VPP_CALL(vkEnumerateInstanceExtensionProperties(reinterpret_cast<const char*>(pLayerName), &count, reinterpret_cast<VkExtensionProperties*>(ret.data()))))) return ret;
	ret.resize(count);
	VPP_CALL(vkEnumerateInstanceExtensionProperties(reinterpret_cast<const char*>(pLayerName), &count, reinterpret_cast<VkExtensionProperties*>(ret.data())));
	return ret;
}
inline Result enumerateDeviceExtensionProperties(PhysicalDevice physicalDevice, const char* pLayerName, uint32_t& pPropertyCount, ExtensionProperties* pProperties)
{
	return VPP_CALL(static_cast<Result>(vkEnumerateDeviceExtensionProperties(physicalDevice, reinterpret_cast<const char*>(pLayerName), reinterpret_cast<uint32_t*>(&pPropertyCount), reinterpret_cast<VkExtensionProperties*>(pProperties))));
}
inline std::vector<ExtensionProperties> enumerateDeviceExtensionProperties(PhysicalDevice physicalDevice, const char* pLayerName)
{
	std::vector<ExtensionProperties> ret;
	std::uint32_t count = 0u;
	if(!call::success(VPP_CALL(vkEnumerateDeviceExtensionProperties(physicalDevice, reinterpret_cast<const char*>(pLayerName), &count, reinterpret_cast<VkExtensionProperties*>(ret.data()))))) return ret;
	ret.resize(count);
	VPP_CALL(vkEnumerateDeviceExtensionProperties(physicalDevice, reinterpret_cast<const char*>(pLayerName), &count, reinterpret_cast<VkExtensionProperties*>(ret.data())));
	return ret;
}
inline Result enumerateInstanceLayerProperties(uint32_t& pPropertyCount, LayerProperties* pProperties)
{
	return VPP_CALL(static_cast<Result>(vkEnumerateInstanceLayerProperties(reinterpret_cast<uint32_t*>(&pPropertyCount), reinterpret_cast<VkLayerProperties*>(pProperties))));
}
inline std::vector<LayerProperties> enumerateInstanceLayerProperties()
{
	std::vector<LayerProperties> ret;
	std::uint32_t count = 0u;
	if(!call::success(VPP_CALL(vkEnumerateInstanceLayerProperties(&count, reinterpret_cast<VkLayerProperties*>(ret.data()))))) return ret;
	ret.resize(count);
	VPP_CALL(vkEnumerateInstanceLayerProperties(&count, reinterpret_cast<VkLayerProperties*>(ret.data())));
	return ret;
}
inline Result enumerateDeviceLayerProperties(PhysicalDevice physicalDevice, uint32_t& pPropertyCount, LayerProperties* pProperties)
{
	return VPP_CALL(static_cast<Result>(vkEnumerateDeviceLayerProperties(physicalDevice, reinterpret_cast<uint32_t*>(&pPropertyCount), reinterpret_cast<VkLayerProperties*>(pProperties))));
}
inline std::vector<LayerProperties> enumerateDeviceLayerProperties(PhysicalDevice physicalDevice)
{
	std::vector<LayerProperties> ret;
	std::uint32_t count = 0u;
	if(!call::success(VPP_CALL(vkEnumerateDeviceLayerProperties(physicalDevice, &count, reinterpret_cast<VkLayerProperties*>(ret.data()))))) return ret;
	ret.resize(count);
	VPP_CALL(vkEnumerateDeviceLayerProperties(physicalDevice, &count, reinterpret_cast<VkLayerProperties*>(ret.data())));
	return ret;
}
inline void getDeviceQueue(Device device, uint32_t queueFamilyIndex, uint32_t queueIndex, Queue& pQueue)
{
	vkGetDeviceQueue(device, queueFamilyIndex, queueIndex, reinterpret_cast<VkQueue*>(&pQueue));
}
inline Result queueSubmit(Queue queue, uint32_t submitCount, const SubmitInfo& pSubmits, Fence fence)
{
	return VPP_CALL(static_cast<Result>(vkQueueSubmit(queue, submitCount, reinterpret_cast<const VkSubmitInfo*>(&pSubmits), fence)));
}
inline Result queueSubmit(Queue queue, const std::vector<SubmitInfo>& pSubmits, Fence fence)
{
	return VPP_CALL(vkQueueSubmit(queue, pSubmits.size(), reinterpret_cast<const VkSubmitInfo*>(pSubmits.data()), fence));
}
inline Result queueWaitIdle(Queue queue)
{
	return VPP_CALL(static_cast<Result>(vkQueueWaitIdle(queue)));
}
inline Result deviceWaitIdle(Device device)
{
	return VPP_CALL(static_cast<Result>(vkDeviceWaitIdle(device)));
}
inline Result allocateMemory(Device device, const MemoryAllocateInfo& pAllocateInfo, const AllocationCallbacks* pAllocator, DeviceMemory& pMemory)
{
	return VPP_CALL(static_cast<Result>(vkAllocateMemory(device, reinterpret_cast<const VkMemoryAllocateInfo*>(&pAllocateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkDeviceMemory*>(&pMemory))));
}
inline void freeMemory(Device device, DeviceMemory memory, const AllocationCallbacks* pAllocator)
{
	vkFreeMemory(device, memory, reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
inline Result mapMemory(Device device, DeviceMemory memory, DeviceSize offset, DeviceSize size, MemoryMapFlags flags, void** ppData)
{
	return VPP_CALL(static_cast<Result>(vkMapMemory(device, memory, offset, size, static_cast<VkMemoryMapFlags>(flags), reinterpret_cast<void**>(ppData))));
}
inline void unmapMemory(Device device, DeviceMemory memory)
{
	vkUnmapMemory(device, memory);
}
inline Result flushMappedMemoryRanges(Device device, uint32_t memoryRangeCount, const MappedMemoryRange& pMemoryRanges)
{
	return VPP_CALL(static_cast<Result>(vkFlushMappedMemoryRanges(device, memoryRangeCount, reinterpret_cast<const VkMappedMemoryRange*>(&pMemoryRanges))));
}
inline Result flushMappedMemoryRanges(Device device, const std::vector<MappedMemoryRange>& pMemoryRanges)
{
	return VPP_CALL(vkFlushMappedMemoryRanges(device, pMemoryRanges.size(), reinterpret_cast<const VkMappedMemoryRange*>(pMemoryRanges.data())));
}
inline Result invalidateMappedMemoryRanges(Device device, uint32_t memoryRangeCount, const MappedMemoryRange& pMemoryRanges)
{
	return VPP_CALL(static_cast<Result>(vkInvalidateMappedMemoryRanges(device, memoryRangeCount, reinterpret_cast<const VkMappedMemoryRange*>(&pMemoryRanges))));
}
inline Result invalidateMappedMemoryRanges(Device device, const std::vector<MappedMemoryRange>& pMemoryRanges)
{
	return VPP_CALL(vkInvalidateMappedMemoryRanges(device, pMemoryRanges.size(), reinterpret_cast<const VkMappedMemoryRange*>(pMemoryRanges.data())));
}
inline void getDeviceMemoryCommitment(Device device, DeviceMemory memory, DeviceSize& pCommittedMemoryInBytes)
{
	vkGetDeviceMemoryCommitment(device, memory, reinterpret_cast<VkDeviceSize*>(&pCommittedMemoryInBytes));
}
inline Result bindBufferMemory(Device device, Buffer buffer, DeviceMemory memory, DeviceSize memoryOffset)
{
	return VPP_CALL(static_cast<Result>(vkBindBufferMemory(device, buffer, memory, memoryOffset)));
}
inline Result bindImageMemory(Device device, Image image, DeviceMemory memory, DeviceSize memoryOffset)
{
	return VPP_CALL(static_cast<Result>(vkBindImageMemory(device, image, memory, memoryOffset)));
}
inline void getBufferMemoryRequirements(Device device, Buffer buffer, MemoryRequirements& pMemoryRequirements)
{
	vkGetBufferMemoryRequirements(device, buffer, reinterpret_cast<VkMemoryRequirements*>(&pMemoryRequirements));
}
inline void getImageMemoryRequirements(Device device, Image image, MemoryRequirements& pMemoryRequirements)
{
	vkGetImageMemoryRequirements(device, image, reinterpret_cast<VkMemoryRequirements*>(&pMemoryRequirements));
}
inline void getImageSparseMemoryRequirements(Device device, Image image, uint32_t& pSparseMemoryRequirementCount, SparseImageMemoryRequirements* pSparseMemoryRequirements)
{
	vkGetImageSparseMemoryRequirements(device, image, reinterpret_cast<uint32_t*>(&pSparseMemoryRequirementCount), reinterpret_cast<VkSparseImageMemoryRequirements*>(pSparseMemoryRequirements));
}
inline std::vector<SparseImageMemoryRequirements> getImageSparseMemoryRequirements(Device device, Image image)
{
	std::vector<SparseImageMemoryRequirements> ret;
	std::uint32_t count = 0u;
	vkGetImageSparseMemoryRequirements(device, image, &count, reinterpret_cast<VkSparseImageMemoryRequirements*>(ret.data()));
	ret.resize(count);
	vkGetImageSparseMemoryRequirements(device, image, &count, reinterpret_cast<VkSparseImageMemoryRequirements*>(ret.data()));
	return ret;
}
inline void getPhysicalDeviceSparseImageFormatProperties(PhysicalDevice physicalDevice, Format format, ImageType type, SampleCountBits samples, ImageUsageFlags usage, ImageTiling tiling, uint32_t& pPropertyCount, SparseImageFormatProperties* pProperties)
{
	vkGetPhysicalDeviceSparseImageFormatProperties(physicalDevice, static_cast<VkFormat>(format), static_cast<VkImageType>(type), static_cast<VkSampleCountFlagBits>(samples), static_cast<VkImageUsageFlags>(usage), static_cast<VkImageTiling>(tiling), reinterpret_cast<uint32_t*>(&pPropertyCount), reinterpret_cast<VkSparseImageFormatProperties*>(pProperties));
}
inline std::vector<SparseImageFormatProperties> getPhysicalDeviceSparseImageFormatProperties(PhysicalDevice physicalDevice, Format format, ImageType type, SampleCountBits samples, ImageUsageFlags usage, ImageTiling tiling)
{
	std::vector<SparseImageFormatProperties> ret;
	std::uint32_t count = 0u;
	vkGetPhysicalDeviceSparseImageFormatProperties(physicalDevice, static_cast<VkFormat>(format), static_cast<VkImageType>(type), static_cast<VkSampleCountFlagBits>(samples), static_cast<VkImageUsageFlags>(usage), static_cast<VkImageTiling>(tiling), &count, reinterpret_cast<VkSparseImageFormatProperties*>(ret.data()));
	ret.resize(count);
	vkGetPhysicalDeviceSparseImageFormatProperties(physicalDevice, static_cast<VkFormat>(format), static_cast<VkImageType>(type), static_cast<VkSampleCountFlagBits>(samples), static_cast<VkImageUsageFlags>(usage), static_cast<VkImageTiling>(tiling), &count, reinterpret_cast<VkSparseImageFormatProperties*>(ret.data()));
	return ret;
}
inline Result queueBindSparse(Queue queue, uint32_t bindInfoCount, const BindSparseInfo& pBindInfo, Fence fence)
{
	return VPP_CALL(static_cast<Result>(vkQueueBindSparse(queue, bindInfoCount, reinterpret_cast<const VkBindSparseInfo*>(&pBindInfo), fence)));
}
inline Result queueBindSparse(Queue queue, const std::vector<BindSparseInfo>& pBindInfo, Fence fence)
{
	return VPP_CALL(vkQueueBindSparse(queue, pBindInfo.size(), reinterpret_cast<const VkBindSparseInfo*>(pBindInfo.data()), fence));
}
inline Result createFence(Device device, const FenceCreateInfo& pCreateInfo, const AllocationCallbacks* pAllocator, Fence& pFence)
{
	return VPP_CALL(static_cast<Result>(vkCreateFence(device, reinterpret_cast<const VkFenceCreateInfo*>(&pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkFence*>(&pFence))));
}
inline void destroyFence(Device device, Fence fence, const AllocationCallbacks* pAllocator)
{
	vkDestroyFence(device, fence, reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
inline Result resetFences(Device device, uint32_t fenceCount, const Fence& pFences)
{
	return VPP_CALL(static_cast<Result>(vkResetFences(device, fenceCount, reinterpret_cast<const VkFence*>(&pFences))));
}
inline Result resetFences(Device device, const std::vector<Fence>& pFences)
{
	return VPP_CALL(vkResetFences(device, pFences.size(), reinterpret_cast<const VkFence*>(pFences.data())));
}
inline Result getFenceStatus(Device device, Fence fence)
{
	return VPP_CALL(static_cast<Result>(vkGetFenceStatus(device, fence)));
}
inline Result waitForFences(Device device, uint32_t fenceCount, const Fence& pFences, Bool32 waitAll, uint64_t timeout)
{
	return VPP_CALL(static_cast<Result>(vkWaitForFences(device, fenceCount, reinterpret_cast<const VkFence*>(&pFences), waitAll, timeout)));
}
inline Result waitForFences(Device device, const std::vector<Fence>& pFences, Bool32 waitAll, uint64_t timeout)
{
	return VPP_CALL(vkWaitForFences(device, pFences.size(), reinterpret_cast<const VkFence*>(pFences.data()), waitAll, timeout));
}
inline Result createSemaphore(Device device, const SemaphoreCreateInfo& pCreateInfo, const AllocationCallbacks* pAllocator, Semaphore& pSemaphore)
{
	return VPP_CALL(static_cast<Result>(vkCreateSemaphore(device, reinterpret_cast<const VkSemaphoreCreateInfo*>(&pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkSemaphore*>(&pSemaphore))));
}
inline void destroySemaphore(Device device, Semaphore semaphore, const AllocationCallbacks* pAllocator)
{
	vkDestroySemaphore(device, semaphore, reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
inline Result createEvent(Device device, const EventCreateInfo& pCreateInfo, const AllocationCallbacks* pAllocator, Event& pEvent)
{
	return VPP_CALL(static_cast<Result>(vkCreateEvent(device, reinterpret_cast<const VkEventCreateInfo*>(&pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkEvent*>(&pEvent))));
}
inline void destroyEvent(Device device, Event event, const AllocationCallbacks* pAllocator)
{
	vkDestroyEvent(device, event, reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
inline Result getEventStatus(Device device, Event event)
{
	return VPP_CALL(static_cast<Result>(vkGetEventStatus(device, event)));
}
inline Result setEvent(Device device, Event event)
{
	return VPP_CALL(static_cast<Result>(vkSetEvent(device, event)));
}
inline Result resetEvent(Device device, Event event)
{
	return VPP_CALL(static_cast<Result>(vkResetEvent(device, event)));
}
inline Result createQueryPool(Device device, const QueryPoolCreateInfo& pCreateInfo, const AllocationCallbacks* pAllocator, QueryPool& pQueryPool)
{
	return VPP_CALL(static_cast<Result>(vkCreateQueryPool(device, reinterpret_cast<const VkQueryPoolCreateInfo*>(&pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkQueryPool*>(&pQueryPool))));
}
inline void destroyQueryPool(Device device, QueryPool queryPool, const AllocationCallbacks* pAllocator)
{
	vkDestroyQueryPool(device, queryPool, reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
inline Result getQueryPoolResults(Device device, QueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, size_t dataSize, void* pData, DeviceSize stride, QueryResultFlags flags)
{
	return VPP_CALL(static_cast<Result>(vkGetQueryPoolResults(device, queryPool, firstQuery, queryCount, dataSize, reinterpret_cast<void*>(pData), stride, static_cast<VkQueryResultFlags>(flags))));
}
inline Result createBuffer(Device device, const BufferCreateInfo& pCreateInfo, const AllocationCallbacks* pAllocator, Buffer& pBuffer)
{
	return VPP_CALL(static_cast<Result>(vkCreateBuffer(device, reinterpret_cast<const VkBufferCreateInfo*>(&pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkBuffer*>(&pBuffer))));
}
inline void destroyBuffer(Device device, Buffer buffer, const AllocationCallbacks* pAllocator)
{
	vkDestroyBuffer(device, buffer, reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
inline Result createBufferView(Device device, const BufferViewCreateInfo& pCreateInfo, const AllocationCallbacks* pAllocator, BufferView& pView)
{
	return VPP_CALL(static_cast<Result>(vkCreateBufferView(device, reinterpret_cast<const VkBufferViewCreateInfo*>(&pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkBufferView*>(&pView))));
}
inline void destroyBufferView(Device device, BufferView bufferView, const AllocationCallbacks* pAllocator)
{
	vkDestroyBufferView(device, bufferView, reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
inline Result createImage(Device device, const ImageCreateInfo& pCreateInfo, const AllocationCallbacks* pAllocator, Image& pImage)
{
	return VPP_CALL(static_cast<Result>(vkCreateImage(device, reinterpret_cast<const VkImageCreateInfo*>(&pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkImage*>(&pImage))));
}
inline void destroyImage(Device device, Image image, const AllocationCallbacks* pAllocator)
{
	vkDestroyImage(device, image, reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
inline void getImageSubresourceLayout(Device device, Image image, const ImageSubresource& pSubresource, SubresourceLayout& pLayout)
{
	vkGetImageSubresourceLayout(device, image, reinterpret_cast<const VkImageSubresource*>(&pSubresource), reinterpret_cast<VkSubresourceLayout*>(&pLayout));
}
inline Result createImageView(Device device, const ImageViewCreateInfo& pCreateInfo, const AllocationCallbacks* pAllocator, ImageView& pView)
{
	return VPP_CALL(static_cast<Result>(vkCreateImageView(device, reinterpret_cast<const VkImageViewCreateInfo*>(&pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkImageView*>(&pView))));
}
inline void destroyImageView(Device device, ImageView imageView, const AllocationCallbacks* pAllocator)
{
	vkDestroyImageView(device, imageView, reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
inline Result createShaderModule(Device device, const ShaderModuleCreateInfo& pCreateInfo, const AllocationCallbacks* pAllocator, ShaderModule& pShaderModule)
{
	return VPP_CALL(static_cast<Result>(vkCreateShaderModule(device, reinterpret_cast<const VkShaderModuleCreateInfo*>(&pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkShaderModule*>(&pShaderModule))));
}
inline void destroyShaderModule(Device device, ShaderModule shaderModule, const AllocationCallbacks* pAllocator)
{
	vkDestroyShaderModule(device, shaderModule, reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
inline Result createPipelineCache(Device device, const PipelineCacheCreateInfo& pCreateInfo, const AllocationCallbacks* pAllocator, PipelineCache& pPipelineCache)
{
	return VPP_CALL(static_cast<Result>(vkCreatePipelineCache(device, reinterpret_cast<const VkPipelineCacheCreateInfo*>(&pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkPipelineCache*>(&pPipelineCache))));
}
inline void destroyPipelineCache(Device device, PipelineCache pipelineCache, const AllocationCallbacks* pAllocator)
{
	vkDestroyPipelineCache(device, pipelineCache, reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
inline Result getPipelineCacheData(Device device, PipelineCache pipelineCache, size_t& pDataSize, void* pData)
{
	return VPP_CALL(static_cast<Result>(vkGetPipelineCacheData(device, pipelineCache, reinterpret_cast<size_t*>(&pDataSize), reinterpret_cast<void*>(pData))));
}
inline Result mergePipelineCaches(Device device, PipelineCache dstCache, uint32_t srcCacheCount, const PipelineCache& pSrcCaches)
{
	return VPP_CALL(static_cast<Result>(vkMergePipelineCaches(device, dstCache, srcCacheCount, reinterpret_cast<const VkPipelineCache*>(&pSrcCaches))));
}
inline Result mergePipelineCaches(Device device, PipelineCache dstCache, const std::vector<PipelineCache>& pSrcCaches)
{
	return VPP_CALL(vkMergePipelineCaches(device, dstCache, pSrcCaches.size(), reinterpret_cast<const VkPipelineCache*>(pSrcCaches.data())));
}
inline Result createGraphicsPipelines(Device device, PipelineCache pipelineCache, uint32_t createInfoCount, const GraphicsPipelineCreateInfo& pCreateInfos, const AllocationCallbacks* pAllocator, Pipeline& pPipelines)
{
	return VPP_CALL(static_cast<Result>(vkCreateGraphicsPipelines(device, pipelineCache, createInfoCount, reinterpret_cast<const VkGraphicsPipelineCreateInfo*>(&pCreateInfos), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkPipeline*>(&pPipelines))));
}
inline Result createGraphicsPipelines(Device device, PipelineCache pipelineCache, const GraphicsPipelineCreateInfo& pCreateInfos, const AllocationCallbacks* pAllocator, std::vector<Pipeline>& pPipelines)
{
	return VPP_CALL(vkCreateGraphicsPipelines(device, pipelineCache, pPipelines.size(), reinterpret_cast<const VkGraphicsPipelineCreateInfo*>(&pCreateInfos), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkPipeline*>(pPipelines.data())));
}
inline Result createComputePipelines(Device device, PipelineCache pipelineCache, uint32_t createInfoCount, const ComputePipelineCreateInfo& pCreateInfos, const AllocationCallbacks* pAllocator, Pipeline& pPipelines)
{
	return VPP_CALL(static_cast<Result>(vkCreateComputePipelines(device, pipelineCache, createInfoCount, reinterpret_cast<const VkComputePipelineCreateInfo*>(&pCreateInfos), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkPipeline*>(&pPipelines))));
}
inline Result createComputePipelines(Device device, PipelineCache pipelineCache, const ComputePipelineCreateInfo& pCreateInfos, const AllocationCallbacks* pAllocator, std::vector<Pipeline>& pPipelines)
{
	return VPP_CALL(vkCreateComputePipelines(device, pipelineCache, pPipelines.size(), reinterpret_cast<const VkComputePipelineCreateInfo*>(&pCreateInfos), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkPipeline*>(pPipelines.data())));
}
inline void destroyPipeline(Device device, Pipeline pipeline, const AllocationCallbacks* pAllocator)
{
	vkDestroyPipeline(device, pipeline, reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
inline Result createPipelineLayout(Device device, const PipelineLayoutCreateInfo& pCreateInfo, const AllocationCallbacks* pAllocator, PipelineLayout& pPipelineLayout)
{
	return VPP_CALL(static_cast<Result>(vkCreatePipelineLayout(device, reinterpret_cast<const VkPipelineLayoutCreateInfo*>(&pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkPipelineLayout*>(&pPipelineLayout))));
}
inline void destroyPipelineLayout(Device device, PipelineLayout pipelineLayout, const AllocationCallbacks* pAllocator)
{
	vkDestroyPipelineLayout(device, pipelineLayout, reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
inline Result createSampler(Device device, const SamplerCreateInfo& pCreateInfo, const AllocationCallbacks* pAllocator, Sampler& pSampler)
{
	return VPP_CALL(static_cast<Result>(vkCreateSampler(device, reinterpret_cast<const VkSamplerCreateInfo*>(&pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkSampler*>(&pSampler))));
}
inline void destroySampler(Device device, Sampler sampler, const AllocationCallbacks* pAllocator)
{
	vkDestroySampler(device, sampler, reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
inline Result createDescriptorSetLayout(Device device, const DescriptorSetLayoutCreateInfo& pCreateInfo, const AllocationCallbacks* pAllocator, DescriptorSetLayout& pSetLayout)
{
	return VPP_CALL(static_cast<Result>(vkCreateDescriptorSetLayout(device, reinterpret_cast<const VkDescriptorSetLayoutCreateInfo*>(&pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkDescriptorSetLayout*>(&pSetLayout))));
}
inline void destroyDescriptorSetLayout(Device device, DescriptorSetLayout descriptorSetLayout, const AllocationCallbacks* pAllocator)
{
	vkDestroyDescriptorSetLayout(device, descriptorSetLayout, reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
inline Result createDescriptorPool(Device device, const DescriptorPoolCreateInfo& pCreateInfo, const AllocationCallbacks* pAllocator, DescriptorPool& pDescriptorPool)
{
	return VPP_CALL(static_cast<Result>(vkCreateDescriptorPool(device, reinterpret_cast<const VkDescriptorPoolCreateInfo*>(&pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkDescriptorPool*>(&pDescriptorPool))));
}
inline void destroyDescriptorPool(Device device, DescriptorPool descriptorPool, const AllocationCallbacks* pAllocator)
{
	vkDestroyDescriptorPool(device, descriptorPool, reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
inline Result resetDescriptorPool(Device device, DescriptorPool descriptorPool, DescriptorPoolResetFlags flags)
{
	return VPP_CALL(static_cast<Result>(vkResetDescriptorPool(device, descriptorPool, static_cast<VkDescriptorPoolResetFlags>(flags))));
}
inline Result allocateDescriptorSets(Device device, const DescriptorSetAllocateInfo& pAllocateInfo, DescriptorSet& pDescriptorSets)
{
	return VPP_CALL(static_cast<Result>(vkAllocateDescriptorSets(device, reinterpret_cast<const VkDescriptorSetAllocateInfo*>(&pAllocateInfo), reinterpret_cast<VkDescriptorSet*>(&pDescriptorSets))));
}
inline Result freeDescriptorSets(Device device, DescriptorPool descriptorPool, uint32_t descriptorSetCount, const DescriptorSet& pDescriptorSets)
{
	return VPP_CALL(static_cast<Result>(vkFreeDescriptorSets(device, descriptorPool, descriptorSetCount, reinterpret_cast<const VkDescriptorSet*>(&pDescriptorSets))));
}
inline Result freeDescriptorSets(Device device, DescriptorPool descriptorPool, const std::vector<DescriptorSet>& pDescriptorSets)
{
	return VPP_CALL(vkFreeDescriptorSets(device, descriptorPool, pDescriptorSets.size(), reinterpret_cast<const VkDescriptorSet*>(pDescriptorSets.data())));
}
inline void updateDescriptorSets(Device device, uint32_t descriptorWriteCount, const WriteDescriptorSet& pDescriptorWrites, uint32_t descriptorCopyCount, const CopyDescriptorSet& pDescriptorCopies)
{
	vkUpdateDescriptorSets(device, descriptorWriteCount, reinterpret_cast<const VkWriteDescriptorSet*>(&pDescriptorWrites), descriptorCopyCount, reinterpret_cast<const VkCopyDescriptorSet*>(&pDescriptorCopies));
}
inline void updateDescriptorSets(Device device, const std::vector<WriteDescriptorSet>& pDescriptorWrites, const std::vector<CopyDescriptorSet>& pDescriptorCopies)
{
	vkUpdateDescriptorSets(device, pDescriptorWrites.size(), reinterpret_cast<const VkWriteDescriptorSet*>(pDescriptorWrites.data()), pDescriptorCopies.size(), reinterpret_cast<const VkCopyDescriptorSet*>(pDescriptorCopies.data()));
}
inline Result createFramebuffer(Device device, const FramebufferCreateInfo& pCreateInfo, const AllocationCallbacks* pAllocator, Framebuffer& pFramebuffer)
{
	return VPP_CALL(static_cast<Result>(vkCreateFramebuffer(device, reinterpret_cast<const VkFramebufferCreateInfo*>(&pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkFramebuffer*>(&pFramebuffer))));
}
inline void destroyFramebuffer(Device device, Framebuffer framebuffer, const AllocationCallbacks* pAllocator)
{
	vkDestroyFramebuffer(device, framebuffer, reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
inline Result createRenderPass(Device device, const RenderPassCreateInfo& pCreateInfo, const AllocationCallbacks* pAllocator, RenderPass& pRenderPass)
{
	return VPP_CALL(static_cast<Result>(vkCreateRenderPass(device, reinterpret_cast<const VkRenderPassCreateInfo*>(&pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkRenderPass*>(&pRenderPass))));
}
inline void destroyRenderPass(Device device, RenderPass renderPass, const AllocationCallbacks* pAllocator)
{
	vkDestroyRenderPass(device, renderPass, reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
inline void getRenderAreaGranularity(Device device, RenderPass renderPass, Extent2D& pGranularity)
{
	vkGetRenderAreaGranularity(device, renderPass, reinterpret_cast<VkExtent2D*>(&pGranularity));
}
inline Result createCommandPool(Device device, const CommandPoolCreateInfo& pCreateInfo, const AllocationCallbacks* pAllocator, CommandPool& pCommandPool)
{
	return VPP_CALL(static_cast<Result>(vkCreateCommandPool(device, reinterpret_cast<const VkCommandPoolCreateInfo*>(&pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkCommandPool*>(&pCommandPool))));
}
inline void destroyCommandPool(Device device, CommandPool commandPool, const AllocationCallbacks* pAllocator)
{
	vkDestroyCommandPool(device, commandPool, reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
inline Result resetCommandPool(Device device, CommandPool commandPool, CommandPoolResetFlags flags)
{
	return VPP_CALL(static_cast<Result>(vkResetCommandPool(device, commandPool, static_cast<VkCommandPoolResetFlags>(flags))));
}
inline Result allocateCommandBuffers(Device device, const CommandBufferAllocateInfo& pAllocateInfo, CommandBuffer& pCommandBuffers)
{
	return VPP_CALL(static_cast<Result>(vkAllocateCommandBuffers(device, reinterpret_cast<const VkCommandBufferAllocateInfo*>(&pAllocateInfo), reinterpret_cast<VkCommandBuffer*>(&pCommandBuffers))));
}
inline void freeCommandBuffers(Device device, CommandPool commandPool, uint32_t commandBufferCount, const CommandBuffer& pCommandBuffers)
{
	vkFreeCommandBuffers(device, commandPool, commandBufferCount, reinterpret_cast<const VkCommandBuffer*>(&pCommandBuffers));
}
inline void freeCommandBuffers(Device device, CommandPool commandPool, const std::vector<CommandBuffer>& pCommandBuffers)
{
	vkFreeCommandBuffers(device, commandPool, pCommandBuffers.size(), reinterpret_cast<const VkCommandBuffer*>(pCommandBuffers.data()));
}
inline Result beginCommandBuffer(CommandBuffer commandBuffer, const CommandBufferBeginInfo& pBeginInfo)
{
	return VPP_CALL(static_cast<Result>(vkBeginCommandBuffer(commandBuffer, reinterpret_cast<const VkCommandBufferBeginInfo*>(&pBeginInfo))));
}
inline Result endCommandBuffer(CommandBuffer commandBuffer)
{
	return VPP_CALL(static_cast<Result>(vkEndCommandBuffer(commandBuffer)));
}
inline Result resetCommandBuffer(CommandBuffer commandBuffer, CommandBufferResetFlags flags)
{
	return VPP_CALL(static_cast<Result>(vkResetCommandBuffer(commandBuffer, static_cast<VkCommandBufferResetFlags>(flags))));
}
inline void cmdBindPipeline(CommandBuffer commandBuffer, PipelineBindPoint pipelineBindPoint, Pipeline pipeline)
{
	vkCmdBindPipeline(commandBuffer, static_cast<VkPipelineBindPoint>(pipelineBindPoint), pipeline);
}
inline void cmdSetViewport(CommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const Viewport& pViewports)
{
	vkCmdSetViewport(commandBuffer, firstViewport, viewportCount, reinterpret_cast<const VkViewport*>(&pViewports));
}
inline void cmdSetViewport(CommandBuffer commandBuffer, uint32_t firstViewport, const std::vector<Viewport>& pViewports)
{
	vkCmdSetViewport(commandBuffer, firstViewport, pViewports.size(), reinterpret_cast<const VkViewport*>(pViewports.data()));
}
inline void cmdSetScissor(CommandBuffer commandBuffer, uint32_t firstScissor, uint32_t scissorCount, const Rect2D& pScissors)
{
	vkCmdSetScissor(commandBuffer, firstScissor, scissorCount, reinterpret_cast<const VkRect2D*>(&pScissors));
}
inline void cmdSetScissor(CommandBuffer commandBuffer, uint32_t firstScissor, const std::vector<Rect2D>& pScissors)
{
	vkCmdSetScissor(commandBuffer, firstScissor, pScissors.size(), reinterpret_cast<const VkRect2D*>(pScissors.data()));
}
inline void cmdSetLineWidth(CommandBuffer commandBuffer, float lineWidth)
{
	vkCmdSetLineWidth(commandBuffer, lineWidth);
}
inline void cmdSetDepthBias(CommandBuffer commandBuffer, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor)
{
	vkCmdSetDepthBias(commandBuffer, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor);
}
inline void cmdSetBlendConstants(CommandBuffer commandBuffer, std::array<const float, 4> blendConstants)
{
	vkCmdSetBlendConstants(commandBuffer, blendConstants.data());
}
inline void cmdSetDepthBounds(CommandBuffer commandBuffer, float minDepthBounds, float maxDepthBounds)
{
	vkCmdSetDepthBounds(commandBuffer, minDepthBounds, maxDepthBounds);
}
inline void cmdSetStencilCompareMask(CommandBuffer commandBuffer, StencilFaceFlags faceMask, uint32_t compareMask)
{
	vkCmdSetStencilCompareMask(commandBuffer, static_cast<VkStencilFaceFlags>(faceMask), compareMask);
}
inline void cmdSetStencilWriteMask(CommandBuffer commandBuffer, StencilFaceFlags faceMask, uint32_t writeMask)
{
	vkCmdSetStencilWriteMask(commandBuffer, static_cast<VkStencilFaceFlags>(faceMask), writeMask);
}
inline void cmdSetStencilReference(CommandBuffer commandBuffer, StencilFaceFlags faceMask, uint32_t reference)
{
	vkCmdSetStencilReference(commandBuffer, static_cast<VkStencilFaceFlags>(faceMask), reference);
}
inline void cmdBindDescriptorSets(CommandBuffer commandBuffer, PipelineBindPoint pipelineBindPoint, PipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, const DescriptorSet& pDescriptorSets, uint32_t dynamicOffsetCount, const uint32_t& pDynamicOffsets)
{
	vkCmdBindDescriptorSets(commandBuffer, static_cast<VkPipelineBindPoint>(pipelineBindPoint), layout, firstSet, descriptorSetCount, reinterpret_cast<const VkDescriptorSet*>(&pDescriptorSets), dynamicOffsetCount, reinterpret_cast<const uint32_t*>(&pDynamicOffsets));
}
inline void cmdBindDescriptorSets(CommandBuffer commandBuffer, PipelineBindPoint pipelineBindPoint, PipelineLayout layout, uint32_t firstSet, const std::vector<DescriptorSet>& pDescriptorSets, const std::vector<uint32_t>& pDynamicOffsets)
{
	vkCmdBindDescriptorSets(commandBuffer, static_cast<VkPipelineBindPoint>(pipelineBindPoint), layout, firstSet, pDescriptorSets.size(), reinterpret_cast<const VkDescriptorSet*>(pDescriptorSets.data()), pDynamicOffsets.size(), reinterpret_cast<const uint32_t*>(pDynamicOffsets.data()));
}
inline void cmdBindIndexBuffer(CommandBuffer commandBuffer, Buffer buffer, DeviceSize offset, IndexType indexType)
{
	vkCmdBindIndexBuffer(commandBuffer, buffer, offset, static_cast<VkIndexType>(indexType));
}
inline void cmdBindVertexBuffers(CommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const Buffer& pBuffers, const DeviceSize& pOffsets)
{
	vkCmdBindVertexBuffers(commandBuffer, firstBinding, bindingCount, reinterpret_cast<const VkBuffer*>(&pBuffers), reinterpret_cast<const VkDeviceSize*>(&pOffsets));
}
inline void cmdBindVertexBuffers(CommandBuffer commandBuffer, uint32_t firstBinding, const Buffer& pBuffers, const std::vector<DeviceSize>& pOffsets)
{
	vkCmdBindVertexBuffers(commandBuffer, firstBinding, pOffsets.size(), reinterpret_cast<const VkBuffer*>(&pBuffers), reinterpret_cast<const VkDeviceSize*>(pOffsets.data()));
}
inline void cmdDraw(CommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance)
{
	vkCmdDraw(commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
}
inline void cmdDrawIndexed(CommandBuffer commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance)
{
	vkCmdDrawIndexed(commandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
}
inline void cmdDrawIndirect(CommandBuffer commandBuffer, Buffer buffer, DeviceSize offset, uint32_t drawCount, uint32_t stride)
{
	vkCmdDrawIndirect(commandBuffer, buffer, offset, drawCount, stride);
}
inline void cmdDrawIndexedIndirect(CommandBuffer commandBuffer, Buffer buffer, DeviceSize offset, uint32_t drawCount, uint32_t stride)
{
	vkCmdDrawIndexedIndirect(commandBuffer, buffer, offset, drawCount, stride);
}
inline void cmdDispatch(CommandBuffer commandBuffer, uint32_t x, uint32_t y, uint32_t z)
{
	vkCmdDispatch(commandBuffer, x, y, z);
}
inline void cmdDispatchIndirect(CommandBuffer commandBuffer, Buffer buffer, DeviceSize offset)
{
	vkCmdDispatchIndirect(commandBuffer, buffer, offset);
}
inline void cmdCopyBuffer(CommandBuffer commandBuffer, Buffer srcBuffer, Buffer dstBuffer, uint32_t regionCount, const BufferCopy& pRegions)
{
	vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, regionCount, reinterpret_cast<const VkBufferCopy*>(&pRegions));
}
inline void cmdCopyBuffer(CommandBuffer commandBuffer, Buffer srcBuffer, Buffer dstBuffer, const std::vector<BufferCopy>& pRegions)
{
	vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, pRegions.size(), reinterpret_cast<const VkBufferCopy*>(pRegions.data()));
}
inline void cmdCopyImage(CommandBuffer commandBuffer, Image srcImage, ImageLayout srcImageLayout, Image dstImage, ImageLayout dstImageLayout, uint32_t regionCount, const ImageCopy& pRegions)
{
	vkCmdCopyImage(commandBuffer, srcImage, static_cast<VkImageLayout>(srcImageLayout), dstImage, static_cast<VkImageLayout>(dstImageLayout), regionCount, reinterpret_cast<const VkImageCopy*>(&pRegions));
}
inline void cmdCopyImage(CommandBuffer commandBuffer, Image srcImage, ImageLayout srcImageLayout, Image dstImage, ImageLayout dstImageLayout, const std::vector<ImageCopy>& pRegions)
{
	vkCmdCopyImage(commandBuffer, srcImage, static_cast<VkImageLayout>(srcImageLayout), dstImage, static_cast<VkImageLayout>(dstImageLayout), pRegions.size(), reinterpret_cast<const VkImageCopy*>(pRegions.data()));
}
inline void cmdBlitImage(CommandBuffer commandBuffer, Image srcImage, ImageLayout srcImageLayout, Image dstImage, ImageLayout dstImageLayout, uint32_t regionCount, const ImageBlit& pRegions, Filter filter)
{
	vkCmdBlitImage(commandBuffer, srcImage, static_cast<VkImageLayout>(srcImageLayout), dstImage, static_cast<VkImageLayout>(dstImageLayout), regionCount, reinterpret_cast<const VkImageBlit*>(&pRegions), static_cast<VkFilter>(filter));
}
inline void cmdBlitImage(CommandBuffer commandBuffer, Image srcImage, ImageLayout srcImageLayout, Image dstImage, ImageLayout dstImageLayout, const std::vector<ImageBlit>& pRegions, Filter filter)
{
	vkCmdBlitImage(commandBuffer, srcImage, static_cast<VkImageLayout>(srcImageLayout), dstImage, static_cast<VkImageLayout>(dstImageLayout), pRegions.size(), reinterpret_cast<const VkImageBlit*>(pRegions.data()), static_cast<VkFilter>(filter));
}
inline void cmdCopyBufferToImage(CommandBuffer commandBuffer, Buffer srcBuffer, Image dstImage, ImageLayout dstImageLayout, uint32_t regionCount, const BufferImageCopy& pRegions)
{
	vkCmdCopyBufferToImage(commandBuffer, srcBuffer, dstImage, static_cast<VkImageLayout>(dstImageLayout), regionCount, reinterpret_cast<const VkBufferImageCopy*>(&pRegions));
}
inline void cmdCopyBufferToImage(CommandBuffer commandBuffer, Buffer srcBuffer, Image dstImage, ImageLayout dstImageLayout, const std::vector<BufferImageCopy>& pRegions)
{
	vkCmdCopyBufferToImage(commandBuffer, srcBuffer, dstImage, static_cast<VkImageLayout>(dstImageLayout), pRegions.size(), reinterpret_cast<const VkBufferImageCopy*>(pRegions.data()));
}
inline void cmdCopyImageToBuffer(CommandBuffer commandBuffer, Image srcImage, ImageLayout srcImageLayout, Buffer dstBuffer, uint32_t regionCount, const BufferImageCopy& pRegions)
{
	vkCmdCopyImageToBuffer(commandBuffer, srcImage, static_cast<VkImageLayout>(srcImageLayout), dstBuffer, regionCount, reinterpret_cast<const VkBufferImageCopy*>(&pRegions));
}
inline void cmdCopyImageToBuffer(CommandBuffer commandBuffer, Image srcImage, ImageLayout srcImageLayout, Buffer dstBuffer, const std::vector<BufferImageCopy>& pRegions)
{
	vkCmdCopyImageToBuffer(commandBuffer, srcImage, static_cast<VkImageLayout>(srcImageLayout), dstBuffer, pRegions.size(), reinterpret_cast<const VkBufferImageCopy*>(pRegions.data()));
}
inline void cmdUpdateBuffer(CommandBuffer commandBuffer, Buffer dstBuffer, DeviceSize dstOffset, DeviceSize dataSize, const uint32_t& pData)
{
	vkCmdUpdateBuffer(commandBuffer, dstBuffer, dstOffset, dataSize, reinterpret_cast<const uint32_t*>(&pData));
}
inline void cmdFillBuffer(CommandBuffer commandBuffer, Buffer dstBuffer, DeviceSize dstOffset, DeviceSize size, uint32_t data)
{
	vkCmdFillBuffer(commandBuffer, dstBuffer, dstOffset, size, data);
}
inline void cmdClearColorImage(CommandBuffer commandBuffer, Image image, ImageLayout imageLayout, const ClearColorValue& pColor, uint32_t rangeCount, const ImageSubresourceRange& pRanges)
{
	vkCmdClearColorImage(commandBuffer, image, static_cast<VkImageLayout>(imageLayout), reinterpret_cast<const VkClearColorValue*>(&pColor), rangeCount, reinterpret_cast<const VkImageSubresourceRange*>(&pRanges));
}
inline void cmdClearColorImage(CommandBuffer commandBuffer, Image image, ImageLayout imageLayout, const ClearColorValue& pColor, const std::vector<ImageSubresourceRange>& pRanges)
{
	vkCmdClearColorImage(commandBuffer, image, static_cast<VkImageLayout>(imageLayout), reinterpret_cast<const VkClearColorValue*>(&pColor), pRanges.size(), reinterpret_cast<const VkImageSubresourceRange*>(pRanges.data()));
}
inline void cmdClearDepthStencilImage(CommandBuffer commandBuffer, Image image, ImageLayout imageLayout, const ClearDepthStencilValue& pDepthStencil, uint32_t rangeCount, const ImageSubresourceRange& pRanges)
{
	vkCmdClearDepthStencilImage(commandBuffer, image, static_cast<VkImageLayout>(imageLayout), reinterpret_cast<const VkClearDepthStencilValue*>(&pDepthStencil), rangeCount, reinterpret_cast<const VkImageSubresourceRange*>(&pRanges));
}
inline void cmdClearDepthStencilImage(CommandBuffer commandBuffer, Image image, ImageLayout imageLayout, const ClearDepthStencilValue& pDepthStencil, const std::vector<ImageSubresourceRange>& pRanges)
{
	vkCmdClearDepthStencilImage(commandBuffer, image, static_cast<VkImageLayout>(imageLayout), reinterpret_cast<const VkClearDepthStencilValue*>(&pDepthStencil), pRanges.size(), reinterpret_cast<const VkImageSubresourceRange*>(pRanges.data()));
}
inline void cmdClearAttachments(CommandBuffer commandBuffer, uint32_t attachmentCount, const ClearAttachment& pAttachments, uint32_t rectCount, const ClearRect& pRects)
{
	vkCmdClearAttachments(commandBuffer, attachmentCount, reinterpret_cast<const VkClearAttachment*>(&pAttachments), rectCount, reinterpret_cast<const VkClearRect*>(&pRects));
}
inline void cmdClearAttachments(CommandBuffer commandBuffer, const std::vector<ClearAttachment>& pAttachments, const std::vector<ClearRect>& pRects)
{
	vkCmdClearAttachments(commandBuffer, pAttachments.size(), reinterpret_cast<const VkClearAttachment*>(pAttachments.data()), pRects.size(), reinterpret_cast<const VkClearRect*>(pRects.data()));
}
inline void cmdResolveImage(CommandBuffer commandBuffer, Image srcImage, ImageLayout srcImageLayout, Image dstImage, ImageLayout dstImageLayout, uint32_t regionCount, const ImageResolve& pRegions)
{
	vkCmdResolveImage(commandBuffer, srcImage, static_cast<VkImageLayout>(srcImageLayout), dstImage, static_cast<VkImageLayout>(dstImageLayout), regionCount, reinterpret_cast<const VkImageResolve*>(&pRegions));
}
inline void cmdResolveImage(CommandBuffer commandBuffer, Image srcImage, ImageLayout srcImageLayout, Image dstImage, ImageLayout dstImageLayout, const std::vector<ImageResolve>& pRegions)
{
	vkCmdResolveImage(commandBuffer, srcImage, static_cast<VkImageLayout>(srcImageLayout), dstImage, static_cast<VkImageLayout>(dstImageLayout), pRegions.size(), reinterpret_cast<const VkImageResolve*>(pRegions.data()));
}
inline void cmdSetEvent(CommandBuffer commandBuffer, Event event, PipelineStageFlags stageMask)
{
	vkCmdSetEvent(commandBuffer, event, static_cast<VkPipelineStageFlags>(stageMask));
}
inline void cmdResetEvent(CommandBuffer commandBuffer, Event event, PipelineStageFlags stageMask)
{
	vkCmdResetEvent(commandBuffer, event, static_cast<VkPipelineStageFlags>(stageMask));
}
inline void cmdWaitEvents(CommandBuffer commandBuffer, uint32_t eventCount, const Event& pEvents, PipelineStageFlags srcStageMask, PipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, const MemoryBarrier& pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const BufferMemoryBarrier& pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const ImageMemoryBarrier& pImageMemoryBarriers)
{
	vkCmdWaitEvents(commandBuffer, eventCount, reinterpret_cast<const VkEvent*>(&pEvents), static_cast<VkPipelineStageFlags>(srcStageMask), static_cast<VkPipelineStageFlags>(dstStageMask), memoryBarrierCount, reinterpret_cast<const VkMemoryBarrier*>(&pMemoryBarriers), bufferMemoryBarrierCount, reinterpret_cast<const VkBufferMemoryBarrier*>(&pBufferMemoryBarriers), imageMemoryBarrierCount, reinterpret_cast<const VkImageMemoryBarrier*>(&pImageMemoryBarriers));
}
inline void cmdWaitEvents(CommandBuffer commandBuffer, const std::vector<Event>& pEvents, PipelineStageFlags srcStageMask, PipelineStageFlags dstStageMask, const std::vector<MemoryBarrier>& pMemoryBarriers, const std::vector<BufferMemoryBarrier>& pBufferMemoryBarriers, const std::vector<ImageMemoryBarrier>& pImageMemoryBarriers)
{
	vkCmdWaitEvents(commandBuffer, pEvents.size(), reinterpret_cast<const VkEvent*>(pEvents.data()), static_cast<VkPipelineStageFlags>(srcStageMask), static_cast<VkPipelineStageFlags>(dstStageMask), pMemoryBarriers.size(), reinterpret_cast<const VkMemoryBarrier*>(pMemoryBarriers.data()), pBufferMemoryBarriers.size(), reinterpret_cast<const VkBufferMemoryBarrier*>(pBufferMemoryBarriers.data()), pImageMemoryBarriers.size(), reinterpret_cast<const VkImageMemoryBarrier*>(pImageMemoryBarriers.data()));
}
inline void cmdPipelineBarrier(CommandBuffer commandBuffer, PipelineStageFlags srcStageMask, PipelineStageFlags dstStageMask, DependencyFlags dependencyFlags, uint32_t memoryBarrierCount, const MemoryBarrier& pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const BufferMemoryBarrier& pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const ImageMemoryBarrier& pImageMemoryBarriers)
{
	vkCmdPipelineBarrier(commandBuffer, static_cast<VkPipelineStageFlags>(srcStageMask), static_cast<VkPipelineStageFlags>(dstStageMask), static_cast<VkDependencyFlags>(dependencyFlags), memoryBarrierCount, reinterpret_cast<const VkMemoryBarrier*>(&pMemoryBarriers), bufferMemoryBarrierCount, reinterpret_cast<const VkBufferMemoryBarrier*>(&pBufferMemoryBarriers), imageMemoryBarrierCount, reinterpret_cast<const VkImageMemoryBarrier*>(&pImageMemoryBarriers));
}
inline void cmdPipelineBarrier(CommandBuffer commandBuffer, PipelineStageFlags srcStageMask, PipelineStageFlags dstStageMask, DependencyFlags dependencyFlags, const std::vector<MemoryBarrier>& pMemoryBarriers, const std::vector<BufferMemoryBarrier>& pBufferMemoryBarriers, const std::vector<ImageMemoryBarrier>& pImageMemoryBarriers)
{
	vkCmdPipelineBarrier(commandBuffer, static_cast<VkPipelineStageFlags>(srcStageMask), static_cast<VkPipelineStageFlags>(dstStageMask), static_cast<VkDependencyFlags>(dependencyFlags), pMemoryBarriers.size(), reinterpret_cast<const VkMemoryBarrier*>(pMemoryBarriers.data()), pBufferMemoryBarriers.size(), reinterpret_cast<const VkBufferMemoryBarrier*>(pBufferMemoryBarriers.data()), pImageMemoryBarriers.size(), reinterpret_cast<const VkImageMemoryBarrier*>(pImageMemoryBarriers.data()));
}
inline void cmdBeginQuery(CommandBuffer commandBuffer, QueryPool queryPool, uint32_t query, QueryControlFlags flags)
{
	vkCmdBeginQuery(commandBuffer, queryPool, query, static_cast<VkQueryControlFlags>(flags));
}
inline void cmdEndQuery(CommandBuffer commandBuffer, QueryPool queryPool, uint32_t query)
{
	vkCmdEndQuery(commandBuffer, queryPool, query);
}
inline void cmdResetQueryPool(CommandBuffer commandBuffer, QueryPool queryPool, uint32_t firstQuery, uint32_t queryCount)
{
	vkCmdResetQueryPool(commandBuffer, queryPool, firstQuery, queryCount);
}
inline void cmdWriteTimestamp(CommandBuffer commandBuffer, PipelineStageBits pipelineStage, QueryPool queryPool, uint32_t query)
{
	vkCmdWriteTimestamp(commandBuffer, static_cast<VkPipelineStageFlagBits>(pipelineStage), queryPool, query);
}
inline void cmdCopyQueryPoolResults(CommandBuffer commandBuffer, QueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, Buffer dstBuffer, DeviceSize dstOffset, DeviceSize stride, QueryResultFlags flags)
{
	vkCmdCopyQueryPoolResults(commandBuffer, queryPool, firstQuery, queryCount, dstBuffer, dstOffset, stride, static_cast<VkQueryResultFlags>(flags));
}
inline void cmdPushConstants(CommandBuffer commandBuffer, PipelineLayout layout, ShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void* pValues)
{
	vkCmdPushConstants(commandBuffer, layout, static_cast<VkShaderStageFlags>(stageFlags), offset, size, reinterpret_cast<const void*>(pValues));
}
inline void cmdBeginRenderPass(CommandBuffer commandBuffer, const RenderPassBeginInfo& pRenderPassBegin, SubpassContents contents)
{
	vkCmdBeginRenderPass(commandBuffer, reinterpret_cast<const VkRenderPassBeginInfo*>(&pRenderPassBegin), static_cast<VkSubpassContents>(contents));
}
inline void cmdNextSubpass(CommandBuffer commandBuffer, SubpassContents contents)
{
	vkCmdNextSubpass(commandBuffer, static_cast<VkSubpassContents>(contents));
}
inline void cmdEndRenderPass(CommandBuffer commandBuffer)
{
	vkCmdEndRenderPass(commandBuffer);
}
inline void cmdExecuteCommands(CommandBuffer commandBuffer, uint32_t commandBufferCount, const CommandBuffer& pCommandBuffers)
{
	vkCmdExecuteCommands(commandBuffer, commandBufferCount, reinterpret_cast<const VkCommandBuffer*>(&pCommandBuffers));
}
inline void cmdExecuteCommands(CommandBuffer commandBuffer, const std::vector<CommandBuffer>& pCommandBuffers)
{
	vkCmdExecuteCommands(commandBuffer, pCommandBuffers.size(), reinterpret_cast<const VkCommandBuffer*>(pCommandBuffers.data()));
}

inline void destroySurfaceKHR(Instance instance, SurfaceKHR surface, const AllocationCallbacks* pAllocator)
{
	vkDestroySurfaceKHR(instance, surface, reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
inline Result getPhysicalDeviceSurfaceSupportKHR(PhysicalDevice physicalDevice, uint32_t queueFamilyIndex, SurfaceKHR surface, Bool32& pSupported)
{
	return VPP_CALL(static_cast<Result>(vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, queueFamilyIndex, surface, reinterpret_cast<VkBool32*>(&pSupported))));
}
inline Result getPhysicalDeviceSurfaceCapabilitiesKHR(PhysicalDevice physicalDevice, SurfaceKHR surface, SurfaceCapabilitiesKHR& pSurfaceCapabilities)
{
	return VPP_CALL(static_cast<Result>(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, reinterpret_cast<VkSurfaceCapabilitiesKHR*>(&pSurfaceCapabilities))));
}
inline Result getPhysicalDeviceSurfaceFormatsKHR(PhysicalDevice physicalDevice, SurfaceKHR surface, uint32_t& pSurfaceFormatCount, SurfaceFormatKHR* pSurfaceFormats)
{
	return VPP_CALL(static_cast<Result>(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, reinterpret_cast<uint32_t*>(&pSurfaceFormatCount), reinterpret_cast<VkSurfaceFormatKHR*>(pSurfaceFormats))));
}
inline std::vector<SurfaceFormatKHR> getPhysicalDeviceSurfaceFormatsKHR(PhysicalDevice physicalDevice, SurfaceKHR surface)
{
	std::vector<SurfaceFormatKHR> ret;
	std::uint32_t count = 0u;
	if(!call::success(VPP_CALL(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &count, reinterpret_cast<VkSurfaceFormatKHR*>(ret.data()))))) return ret;
	ret.resize(count);
	VPP_CALL(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &count, reinterpret_cast<VkSurfaceFormatKHR*>(ret.data())));
	return ret;
}
inline Result getPhysicalDeviceSurfacePresentModesKHR(PhysicalDevice physicalDevice, SurfaceKHR surface, uint32_t& pPresentModeCount, PresentModeKHR* pPresentModes)
{
	return VPP_CALL(static_cast<Result>(vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, reinterpret_cast<uint32_t*>(&pPresentModeCount), reinterpret_cast<VkPresentModeKHR*>(pPresentModes))));
}
inline std::vector<PresentModeKHR> getPhysicalDeviceSurfacePresentModesKHR(PhysicalDevice physicalDevice, SurfaceKHR surface)
{
	std::vector<PresentModeKHR> ret;
	std::uint32_t count = 0u;
	if(!call::success(VPP_CALL(vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &count, reinterpret_cast<VkPresentModeKHR*>(ret.data()))))) return ret;
	ret.resize(count);
	VPP_CALL(vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &count, reinterpret_cast<VkPresentModeKHR*>(ret.data())));
	return ret;
}

inline Result createSwapchainKHR(Device device, const SwapchainCreateInfoKHR& pCreateInfo, const AllocationCallbacks* pAllocator, SwapchainKHR& pSwapchain)
{
	return VPP_CALL(static_cast<Result>(vkCreateSwapchainKHR(device, reinterpret_cast<const VkSwapchainCreateInfoKHR*>(&pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkSwapchainKHR*>(&pSwapchain))));
}
inline void destroySwapchainKHR(Device device, SwapchainKHR swapchain, const AllocationCallbacks* pAllocator)
{
	vkDestroySwapchainKHR(device, swapchain, reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
inline Result getSwapchainImagesKHR(Device device, SwapchainKHR swapchain, uint32_t& pSwapchainImageCount, Image* pSwapchainImages)
{
	return VPP_CALL(static_cast<Result>(vkGetSwapchainImagesKHR(device, swapchain, reinterpret_cast<uint32_t*>(&pSwapchainImageCount), reinterpret_cast<VkImage*>(pSwapchainImages))));
}
inline std::vector<Image> getSwapchainImagesKHR(Device device, SwapchainKHR swapchain)
{
	std::vector<Image> ret;
	std::uint32_t count = 0u;
	if(!call::success(VPP_CALL(vkGetSwapchainImagesKHR(device, swapchain, &count, reinterpret_cast<VkImage*>(ret.data()))))) return ret;
	ret.resize(count);
	VPP_CALL(vkGetSwapchainImagesKHR(device, swapchain, &count, reinterpret_cast<VkImage*>(ret.data())));
	return ret;
}
inline Result acquireNextImageKHR(Device device, SwapchainKHR swapchain, uint64_t timeout, Semaphore semaphore, Fence fence, uint32_t& pImageIndex)
{
	return VPP_CALL(static_cast<Result>(vkAcquireNextImageKHR(device, swapchain, timeout, semaphore, fence, reinterpret_cast<uint32_t*>(&pImageIndex))));
}
inline Result queuePresentKHR(Queue queue, const PresentInfoKHR& pPresentInfo)
{
	return VPP_CALL(static_cast<Result>(vkQueuePresentKHR(queue, reinterpret_cast<const VkPresentInfoKHR*>(&pPresentInfo))));
}

inline Result getPhysicalDeviceDisplayPropertiesKHR(PhysicalDevice physicalDevice, uint32_t& pPropertyCount, DisplayPropertiesKHR* pProperties)
{
	return VPP_CALL(static_cast<Result>(vkGetPhysicalDeviceDisplayPropertiesKHR(physicalDevice, reinterpret_cast<uint32_t*>(&pPropertyCount), reinterpret_cast<VkDisplayPropertiesKHR*>(pProperties))));
}
inline std::vector<DisplayPropertiesKHR> getPhysicalDeviceDisplayPropertiesKHR(PhysicalDevice physicalDevice)
{
	std::vector<DisplayPropertiesKHR> ret;
	std::uint32_t count = 0u;
	if(!call::success(VPP_CALL(vkGetPhysicalDeviceDisplayPropertiesKHR(physicalDevice, &count, reinterpret_cast<VkDisplayPropertiesKHR*>(ret.data()))))) return ret;
	ret.resize(count);
	VPP_CALL(vkGetPhysicalDeviceDisplayPropertiesKHR(physicalDevice, &count, reinterpret_cast<VkDisplayPropertiesKHR*>(ret.data())));
	return ret;
}
inline Result getPhysicalDeviceDisplayPlanePropertiesKHR(PhysicalDevice physicalDevice, uint32_t& pPropertyCount, DisplayPlanePropertiesKHR* pProperties)
{
	return VPP_CALL(static_cast<Result>(vkGetPhysicalDeviceDisplayPlanePropertiesKHR(physicalDevice, reinterpret_cast<uint32_t*>(&pPropertyCount), reinterpret_cast<VkDisplayPlanePropertiesKHR*>(pProperties))));
}
inline std::vector<DisplayPlanePropertiesKHR> getPhysicalDeviceDisplayPlanePropertiesKHR(PhysicalDevice physicalDevice)
{
	std::vector<DisplayPlanePropertiesKHR> ret;
	std::uint32_t count = 0u;
	if(!call::success(VPP_CALL(vkGetPhysicalDeviceDisplayPlanePropertiesKHR(physicalDevice, &count, reinterpret_cast<VkDisplayPlanePropertiesKHR*>(ret.data()))))) return ret;
	ret.resize(count);
	VPP_CALL(vkGetPhysicalDeviceDisplayPlanePropertiesKHR(physicalDevice, &count, reinterpret_cast<VkDisplayPlanePropertiesKHR*>(ret.data())));
	return ret;
}
inline Result getDisplayPlaneSupportedDisplaysKHR(PhysicalDevice physicalDevice, uint32_t planeIndex, uint32_t& pDisplayCount, DisplayKHR* pDisplays)
{
	return VPP_CALL(static_cast<Result>(vkGetDisplayPlaneSupportedDisplaysKHR(physicalDevice, planeIndex, reinterpret_cast<uint32_t*>(&pDisplayCount), reinterpret_cast<VkDisplayKHR*>(pDisplays))));
}
inline std::vector<DisplayKHR> getDisplayPlaneSupportedDisplaysKHR(PhysicalDevice physicalDevice, uint32_t planeIndex)
{
	std::vector<DisplayKHR> ret;
	std::uint32_t count = 0u;
	if(!call::success(VPP_CALL(vkGetDisplayPlaneSupportedDisplaysKHR(physicalDevice, planeIndex, &count, reinterpret_cast<VkDisplayKHR*>(ret.data()))))) return ret;
	ret.resize(count);
	VPP_CALL(vkGetDisplayPlaneSupportedDisplaysKHR(physicalDevice, planeIndex, &count, reinterpret_cast<VkDisplayKHR*>(ret.data())));
	return ret;
}
inline Result getDisplayModePropertiesKHR(PhysicalDevice physicalDevice, DisplayKHR display, uint32_t& pPropertyCount, DisplayModePropertiesKHR* pProperties)
{
	return VPP_CALL(static_cast<Result>(vkGetDisplayModePropertiesKHR(physicalDevice, display, reinterpret_cast<uint32_t*>(&pPropertyCount), reinterpret_cast<VkDisplayModePropertiesKHR*>(pProperties))));
}
inline std::vector<DisplayModePropertiesKHR> getDisplayModePropertiesKHR(PhysicalDevice physicalDevice, DisplayKHR display)
{
	std::vector<DisplayModePropertiesKHR> ret;
	std::uint32_t count = 0u;
	if(!call::success(VPP_CALL(vkGetDisplayModePropertiesKHR(physicalDevice, display, &count, reinterpret_cast<VkDisplayModePropertiesKHR*>(ret.data()))))) return ret;
	ret.resize(count);
	VPP_CALL(vkGetDisplayModePropertiesKHR(physicalDevice, display, &count, reinterpret_cast<VkDisplayModePropertiesKHR*>(ret.data())));
	return ret;
}
inline Result createDisplayModeKHR(PhysicalDevice physicalDevice, DisplayKHR display, const DisplayModeCreateInfoKHR& pCreateInfo, const AllocationCallbacks* pAllocator, DisplayModeKHR& pMode)
{
	return VPP_CALL(static_cast<Result>(vkCreateDisplayModeKHR(physicalDevice, display, reinterpret_cast<const VkDisplayModeCreateInfoKHR*>(&pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkDisplayModeKHR*>(&pMode))));
}
inline Result getDisplayPlaneCapabilitiesKHR(PhysicalDevice physicalDevice, DisplayModeKHR mode, uint32_t planeIndex, DisplayPlaneCapabilitiesKHR& pCapabilities)
{
	return VPP_CALL(static_cast<Result>(vkGetDisplayPlaneCapabilitiesKHR(physicalDevice, mode, planeIndex, reinterpret_cast<VkDisplayPlaneCapabilitiesKHR*>(&pCapabilities))));
}
inline Result createDisplayPlaneSurfaceKHR(Instance instance, const DisplaySurfaceCreateInfoKHR& pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceKHR& pSurface)
{
	return VPP_CALL(static_cast<Result>(vkCreateDisplayPlaneSurfaceKHR(instance, reinterpret_cast<const VkDisplaySurfaceCreateInfoKHR*>(&pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkSurfaceKHR*>(&pSurface))));
}

inline Result createSharedSwapchainsKHR(Device device, uint32_t swapchainCount, const SwapchainCreateInfoKHR& pCreateInfos, const AllocationCallbacks* pAllocator, SwapchainKHR& pSwapchains)
{
	return VPP_CALL(static_cast<Result>(vkCreateSharedSwapchainsKHR(device, swapchainCount, reinterpret_cast<const VkSwapchainCreateInfoKHR*>(&pCreateInfos), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkSwapchainKHR*>(&pSwapchains))));
}
inline Result createSharedSwapchainsKHR(Device device, const SwapchainCreateInfoKHR& pCreateInfos, const AllocationCallbacks* pAllocator, std::vector<SwapchainKHR>& pSwapchains)
{
	return VPP_CALL(vkCreateSharedSwapchainsKHR(device, pSwapchains.size(), reinterpret_cast<const VkSwapchainCreateInfoKHR*>(&pCreateInfos), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkSwapchainKHR*>(pSwapchains.data())));
}

#ifdef VK_USE_PLATFORM_XLIB_KHR

inline Result createXlibSurfaceKHR(Instance instance, const XlibSurfaceCreateInfoKHR& pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceKHR& pSurface)
{
	return VPP_CALL(static_cast<Result>(vkCreateXlibSurfaceKHR(instance, reinterpret_cast<const VkXlibSurfaceCreateInfoKHR*>(&pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkSurfaceKHR*>(&pSurface))));
}
inline Bool32 getPhysicalDeviceXlibPresentationSupportKHR(PhysicalDevice physicalDevice, uint32_t queueFamilyIndex, Display& dpy, VisualID visualID)
{
	return static_cast<Bool32>(vkGetPhysicalDeviceXlibPresentationSupportKHR(physicalDevice, queueFamilyIndex, reinterpret_cast<Display*>(&dpy), visualID));
}

#endif //VK_USE_PLATFORM_XLIB_KHR

#ifdef VK_USE_PLATFORM_XCB_KHR

inline Result createXcbSurfaceKHR(Instance instance, const XcbSurfaceCreateInfoKHR& pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceKHR& pSurface)
{
	return VPP_CALL(static_cast<Result>(vkCreateXcbSurfaceKHR(instance, reinterpret_cast<const VkXcbSurfaceCreateInfoKHR*>(&pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkSurfaceKHR*>(&pSurface))));
}
inline Bool32 getPhysicalDeviceXcbPresentationSupportKHR(PhysicalDevice physicalDevice, uint32_t queueFamilyIndex, xcb_connection_t& connection, xcb_visualid_t visual_id)
{
	return static_cast<Bool32>(vkGetPhysicalDeviceXcbPresentationSupportKHR(physicalDevice, queueFamilyIndex, reinterpret_cast<xcb_connection_t*>(&connection), visual_id));
}

#endif //VK_USE_PLATFORM_XCB_KHR

#ifdef VK_USE_PLATFORM_WAYLAND_KHR

inline Result createWaylandSurfaceKHR(Instance instance, const WaylandSurfaceCreateInfoKHR& pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceKHR& pSurface)
{
	return VPP_CALL(static_cast<Result>(vkCreateWaylandSurfaceKHR(instance, reinterpret_cast<const VkWaylandSurfaceCreateInfoKHR*>(&pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkSurfaceKHR*>(&pSurface))));
}
inline Bool32 getPhysicalDeviceWaylandPresentationSupportKHR(PhysicalDevice physicalDevice, uint32_t queueFamilyIndex, wl_display& display)
{
	return static_cast<Bool32>(vkGetPhysicalDeviceWaylandPresentationSupportKHR(physicalDevice, queueFamilyIndex, reinterpret_cast<wl_display*>(&display)));
}

#endif //VK_USE_PLATFORM_WAYLAND_KHR

#ifdef VK_USE_PLATFORM_MIR_KHR

inline Result createMirSurfaceKHR(Instance instance, const MirSurfaceCreateInfoKHR& pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceKHR& pSurface)
{
	return VPP_CALL(static_cast<Result>(vkCreateMirSurfaceKHR(instance, reinterpret_cast<const VkMirSurfaceCreateInfoKHR*>(&pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkSurfaceKHR*>(&pSurface))));
}
inline Bool32 getPhysicalDeviceMirPresentationSupportKHR(PhysicalDevice physicalDevice, uint32_t queueFamilyIndex, MirConnection& connection)
{
	return static_cast<Bool32>(vkGetPhysicalDeviceMirPresentationSupportKHR(physicalDevice, queueFamilyIndex, reinterpret_cast<MirConnection*>(&connection)));
}

#endif //VK_USE_PLATFORM_MIR_KHR

#ifdef VK_USE_PLATFORM_ANDROID_KHR

inline Result createAndroidSurfaceKHR(Instance instance, const AndroidSurfaceCreateInfoKHR& pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceKHR& pSurface)
{
	return VPP_CALL(static_cast<Result>(vkCreateAndroidSurfaceKHR(instance, reinterpret_cast<const VkAndroidSurfaceCreateInfoKHR*>(&pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkSurfaceKHR*>(&pSurface))));
}

#endif //VK_USE_PLATFORM_ANDROID_KHR

#ifdef VK_USE_PLATFORM_WIN32_KHR

inline Result createWin32SurfaceKHR(Instance instance, const Win32SurfaceCreateInfoKHR& pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceKHR& pSurface)
{
	return VPP_CALL(static_cast<Result>(vkCreateWin32SurfaceKHR(instance, reinterpret_cast<const VkWin32SurfaceCreateInfoKHR*>(&pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkSurfaceKHR*>(&pSurface))));
}
inline Bool32 getPhysicalDeviceWin32PresentationSupportKHR(PhysicalDevice physicalDevice, uint32_t queueFamilyIndex)
{
	return static_cast<Bool32>(vkGetPhysicalDeviceWin32PresentationSupportKHR(physicalDevice, queueFamilyIndex));
}

#endif //VK_USE_PLATFORM_WIN32_KHR

inline Result createDebugReportCallbackEXT(Instance instance, const DebugReportCallbackCreateInfoEXT& pCreateInfo, const AllocationCallbacks* pAllocator, DebugReportCallbackEXT& pCallback)
{
	return VPP_CALL(static_cast<Result>(vkCreateDebugReportCallbackEXT(instance, reinterpret_cast<const VkDebugReportCallbackCreateInfoEXT*>(&pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkDebugReportCallbackEXT*>(&pCallback))));
}
inline void destroyDebugReportCallbackEXT(Instance instance, DebugReportCallbackEXT callback, const AllocationCallbacks* pAllocator)
{
	vkDestroyDebugReportCallbackEXT(instance, callback, reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
inline void debugReportMessageEXT(Instance instance, DebugReportFlagsEXT flags, DebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char& pLayerPrefix, const char& pMessage)
{
	vkDebugReportMessageEXT(instance, static_cast<VkDebugReportFlagsEXT>(flags), static_cast<VkDebugReportObjectTypeEXT>(objectType), object, location, messageCode, reinterpret_cast<const char*>(&pLayerPrefix), reinterpret_cast<const char*>(&pMessage));
}

inline Result debugMarkerSetObjectTagEXT(Device device, DebugMarkerObjectTagInfoEXT& pTagInfo)
{
	return VPP_CALL(static_cast<Result>(vkDebugMarkerSetObjectTagEXT(device, reinterpret_cast<VkDebugMarkerObjectTagInfoEXT*>(&pTagInfo))));
}
inline Result debugMarkerSetObjectNameEXT(Device device, DebugMarkerObjectNameInfoEXT& pNameInfo)
{
	return VPP_CALL(static_cast<Result>(vkDebugMarkerSetObjectNameEXT(device, reinterpret_cast<VkDebugMarkerObjectNameInfoEXT*>(&pNameInfo))));
}
inline void cmdDebugMarkerBeginEXT(CommandBuffer commandBuffer, DebugMarkerMarkerInfoEXT& pMarkerInfo)
{
	vkCmdDebugMarkerBeginEXT(commandBuffer, reinterpret_cast<VkDebugMarkerMarkerInfoEXT*>(&pMarkerInfo));
}
inline void cmdDebugMarkerEndEXT(CommandBuffer commandBuffer)
{
	vkCmdDebugMarkerEndEXT(commandBuffer);
}
inline void cmdDebugMarkerInsertEXT(CommandBuffer commandBuffer, DebugMarkerMarkerInfoEXT& pMarkerInfo)
{
	vkCmdDebugMarkerInsertEXT(commandBuffer, reinterpret_cast<VkDebugMarkerMarkerInfoEXT*>(&pMarkerInfo));
}



} //namespace vk
