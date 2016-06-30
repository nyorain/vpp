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
#include "range.hpp"

#include <vector>
#include <vulkan/vulkan.h>

namespace vk
{

inline Instance createInstance(const InstanceCreateInfo& pCreateInfo, const AllocationCallbacks* pAllocator = {})
{
	Instance ret = {};
	VPP_CALL(static_cast<Result>(vkCreateInstance(reinterpret_cast<const VkInstanceCreateInfo*>(&pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkInstance*>(&ret))));
	return ret;
}
inline void destroyInstance(Instance instance = {}, const AllocationCallbacks* pAllocator = {})
{
	return vkDestroyInstance(reinterpret_cast<VkInstance>(instance), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
inline Result enumeratePhysicalDevices(Instance instance, uint32_t& pPhysicalDeviceCount, PhysicalDevice* pPhysicalDevices = {})
{
	return static_cast<Result>(vkEnumeratePhysicalDevices(reinterpret_cast<VkInstance>(instance), reinterpret_cast<uint32_t*>(&pPhysicalDeviceCount), reinterpret_cast<VkPhysicalDevice*>(pPhysicalDevices)));
}
inline std::vector<PhysicalDevice> enumeratePhysicalDevices(Instance instance)
{
	std::vector<PhysicalDevice> ret;
	uint32_t count = 0u;
	if(!call::success(VPP_CALL(vkEnumeratePhysicalDevices(reinterpret_cast<VkInstance>(instance), &count, reinterpret_cast<VkPhysicalDevice*>(ret.data()))))) return ret;
	ret.resize(count);
	VPP_CALL(vkEnumeratePhysicalDevices(reinterpret_cast<VkInstance>(instance), &count, reinterpret_cast<VkPhysicalDevice*>(ret.data())));
	return ret;
}
inline void getPhysicalDeviceFeatures(PhysicalDevice physicalDevice, PhysicalDeviceFeatures& pFeatures)
{
	return vkGetPhysicalDeviceFeatures(reinterpret_cast<VkPhysicalDevice>(physicalDevice), reinterpret_cast<VkPhysicalDeviceFeatures*>(&pFeatures));
}
inline FormatProperties getPhysicalDeviceFormatProperties(PhysicalDevice physicalDevice, Format format)
{
	FormatProperties ret = {};
	vkGetPhysicalDeviceFormatProperties(reinterpret_cast<VkPhysicalDevice>(physicalDevice), static_cast<VkFormat>(format), reinterpret_cast<VkFormatProperties*>(&ret));
	return ret;
}
inline ImageFormatProperties getPhysicalDeviceImageFormatProperties(PhysicalDevice physicalDevice, Format format, ImageType type, ImageTiling tiling, ImageUsageFlags usage, ImageCreateFlags flags = {})
{
	ImageFormatProperties ret = {};
	VPP_CALL(static_cast<Result>(vkGetPhysicalDeviceImageFormatProperties(reinterpret_cast<VkPhysicalDevice>(physicalDevice), static_cast<VkFormat>(format), static_cast<VkImageType>(type), static_cast<VkImageTiling>(tiling), static_cast<VkImageUsageFlags>(usage), static_cast<VkImageCreateFlags>(flags), reinterpret_cast<VkImageFormatProperties*>(&ret))));
	return ret;
}
inline PhysicalDeviceProperties getPhysicalDeviceProperties(PhysicalDevice physicalDevice)
{
	PhysicalDeviceProperties ret = {};
	vkGetPhysicalDeviceProperties(reinterpret_cast<VkPhysicalDevice>(physicalDevice), reinterpret_cast<VkPhysicalDeviceProperties*>(&ret));
	return ret;
}
inline void getPhysicalDeviceQueueFamilyProperties(PhysicalDevice physicalDevice, uint32_t& pQueueFamilyPropertyCount, QueueFamilyProperties* pQueueFamilyProperties = {})
{
	return vkGetPhysicalDeviceQueueFamilyProperties(reinterpret_cast<VkPhysicalDevice>(physicalDevice), reinterpret_cast<uint32_t*>(&pQueueFamilyPropertyCount), reinterpret_cast<VkQueueFamilyProperties*>(pQueueFamilyProperties));
}
inline std::vector<QueueFamilyProperties> getPhysicalDeviceQueueFamilyProperties(PhysicalDevice physicalDevice)
{
	std::vector<QueueFamilyProperties> ret;
	uint32_t count = 0u;
	vkGetPhysicalDeviceQueueFamilyProperties(reinterpret_cast<VkPhysicalDevice>(physicalDevice), &count, reinterpret_cast<VkQueueFamilyProperties*>(ret.data()));
	ret.resize(count);
	vkGetPhysicalDeviceQueueFamilyProperties(reinterpret_cast<VkPhysicalDevice>(physicalDevice), &count, reinterpret_cast<VkQueueFamilyProperties*>(ret.data()));
	return ret;
}
inline PhysicalDeviceMemoryProperties getPhysicalDeviceMemoryProperties(PhysicalDevice physicalDevice)
{
	PhysicalDeviceMemoryProperties ret = {};
	vkGetPhysicalDeviceMemoryProperties(reinterpret_cast<VkPhysicalDevice>(physicalDevice), reinterpret_cast<VkPhysicalDeviceMemoryProperties*>(&ret));
	return ret;
}
inline PfnVoidFunction getInstanceProcAddr(Instance instance, const char* pName)
{
	return static_cast<PfnVoidFunction>(vkGetInstanceProcAddr(reinterpret_cast<VkInstance>(instance), reinterpret_cast<const char*>(pName)));
}
inline PfnVoidFunction getDeviceProcAddr(Device device, const char* pName)
{
	return static_cast<PfnVoidFunction>(vkGetDeviceProcAddr(reinterpret_cast<VkDevice>(device), reinterpret_cast<const char*>(pName)));
}
inline Device createDevice(PhysicalDevice physicalDevice, const DeviceCreateInfo& pCreateInfo, const AllocationCallbacks* pAllocator = {})
{
	Device ret = {};
	VPP_CALL(static_cast<Result>(vkCreateDevice(reinterpret_cast<VkPhysicalDevice>(physicalDevice), reinterpret_cast<const VkDeviceCreateInfo*>(&pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkDevice*>(&ret))));
	return ret;
}
inline void destroyDevice(Device device = {}, const AllocationCallbacks* pAllocator = {})
{
	return vkDestroyDevice(reinterpret_cast<VkDevice>(device), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
inline Result enumerateInstanceExtensionProperties(const char* pLayerName, uint32_t& pPropertyCount, ExtensionProperties* pProperties = {})
{
	return static_cast<Result>(vkEnumerateInstanceExtensionProperties(reinterpret_cast<const char*>(pLayerName), reinterpret_cast<uint32_t*>(&pPropertyCount), reinterpret_cast<VkExtensionProperties*>(pProperties)));
}
inline std::vector<ExtensionProperties> enumerateInstanceExtensionProperties(const char* pLayerName)
{
	std::vector<ExtensionProperties> ret;
	uint32_t count = 0u;
	if(!call::success(VPP_CALL(vkEnumerateInstanceExtensionProperties(reinterpret_cast<const char*>(pLayerName), &count, reinterpret_cast<VkExtensionProperties*>(ret.data()))))) return ret;
	ret.resize(count);
	VPP_CALL(vkEnumerateInstanceExtensionProperties(reinterpret_cast<const char*>(pLayerName), &count, reinterpret_cast<VkExtensionProperties*>(ret.data())));
	return ret;
}
inline Result enumerateDeviceExtensionProperties(PhysicalDevice physicalDevice, const char* pLayerName, uint32_t& pPropertyCount, ExtensionProperties* pProperties = {})
{
	return static_cast<Result>(vkEnumerateDeviceExtensionProperties(reinterpret_cast<VkPhysicalDevice>(physicalDevice), reinterpret_cast<const char*>(pLayerName), reinterpret_cast<uint32_t*>(&pPropertyCount), reinterpret_cast<VkExtensionProperties*>(pProperties)));
}
inline std::vector<ExtensionProperties> enumerateDeviceExtensionProperties(PhysicalDevice physicalDevice, const char* pLayerName)
{
	std::vector<ExtensionProperties> ret;
	uint32_t count = 0u;
	if(!call::success(VPP_CALL(vkEnumerateDeviceExtensionProperties(reinterpret_cast<VkPhysicalDevice>(physicalDevice), reinterpret_cast<const char*>(pLayerName), &count, reinterpret_cast<VkExtensionProperties*>(ret.data()))))) return ret;
	ret.resize(count);
	VPP_CALL(vkEnumerateDeviceExtensionProperties(reinterpret_cast<VkPhysicalDevice>(physicalDevice), reinterpret_cast<const char*>(pLayerName), &count, reinterpret_cast<VkExtensionProperties*>(ret.data())));
	return ret;
}
inline Result enumerateInstanceLayerProperties(uint32_t& pPropertyCount, LayerProperties* pProperties = {})
{
	return static_cast<Result>(vkEnumerateInstanceLayerProperties(reinterpret_cast<uint32_t*>(&pPropertyCount), reinterpret_cast<VkLayerProperties*>(pProperties)));
}
inline std::vector<LayerProperties> enumerateInstanceLayerProperties()
{
	std::vector<LayerProperties> ret;
	uint32_t count = 0u;
	if(!call::success(VPP_CALL(vkEnumerateInstanceLayerProperties(&count, reinterpret_cast<VkLayerProperties*>(ret.data()))))) return ret;
	ret.resize(count);
	VPP_CALL(vkEnumerateInstanceLayerProperties(&count, reinterpret_cast<VkLayerProperties*>(ret.data())));
	return ret;
}
inline Result enumerateDeviceLayerProperties(PhysicalDevice physicalDevice, uint32_t& pPropertyCount, LayerProperties* pProperties = {})
{
	return static_cast<Result>(vkEnumerateDeviceLayerProperties(reinterpret_cast<VkPhysicalDevice>(physicalDevice), reinterpret_cast<uint32_t*>(&pPropertyCount), reinterpret_cast<VkLayerProperties*>(pProperties)));
}
inline std::vector<LayerProperties> enumerateDeviceLayerProperties(PhysicalDevice physicalDevice)
{
	std::vector<LayerProperties> ret;
	uint32_t count = 0u;
	if(!call::success(VPP_CALL(vkEnumerateDeviceLayerProperties(reinterpret_cast<VkPhysicalDevice>(physicalDevice), &count, reinterpret_cast<VkLayerProperties*>(ret.data()))))) return ret;
	ret.resize(count);
	VPP_CALL(vkEnumerateDeviceLayerProperties(reinterpret_cast<VkPhysicalDevice>(physicalDevice), &count, reinterpret_cast<VkLayerProperties*>(ret.data())));
	return ret;
}
inline Queue getDeviceQueue(Device device, uint32_t queueFamilyIndex, uint32_t queueIndex)
{
	Queue ret = {};
	vkGetDeviceQueue(reinterpret_cast<VkDevice>(device), queueFamilyIndex, queueIndex, reinterpret_cast<VkQueue*>(&ret));
	return ret;
}
inline Result queueSubmit(Queue queue, uint32_t submitCount, const SubmitInfo& pSubmits, Fence fence = {})
{
	return static_cast<Result>(vkQueueSubmit(reinterpret_cast<VkQueue>(queue), submitCount, reinterpret_cast<const VkSubmitInfo*>(&pSubmits), reinterpret_cast<VkFence>(fence)));
}
inline Result queueSubmit(Queue queue, const Range<const SubmitInfo>& pSubmits, Fence fence = {})
{
	return VPP_CALL(vkQueueSubmit(reinterpret_cast<VkQueue>(queue), pSubmits.size(), reinterpret_cast<const VkSubmitInfo*>(pSubmits.data()), reinterpret_cast<VkFence>(fence)));
}
inline Result queueWaitIdle(Queue queue)
{
	return static_cast<Result>(vkQueueWaitIdle(reinterpret_cast<VkQueue>(queue)));
}
inline Result deviceWaitIdle(Device device)
{
	return static_cast<Result>(vkDeviceWaitIdle(reinterpret_cast<VkDevice>(device)));
}
inline DeviceMemory allocateMemory(Device device, const MemoryAllocateInfo& pAllocateInfo, const AllocationCallbacks* pAllocator = {})
{
	DeviceMemory ret = {};
	VPP_CALL(static_cast<Result>(vkAllocateMemory(reinterpret_cast<VkDevice>(device), reinterpret_cast<const VkMemoryAllocateInfo*>(&pAllocateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkDeviceMemory*>(&ret))));
	return ret;
}
inline void freeMemory(Device device, DeviceMemory memory = {}, const AllocationCallbacks* pAllocator = {})
{
	return vkFreeMemory(reinterpret_cast<VkDevice>(device), reinterpret_cast<VkDeviceMemory>(memory), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
inline void* mapMemory(Device device, DeviceMemory memory, DeviceSize offset, DeviceSize size, MemoryMapFlags flags = {})
{
	void* ret = {};
	VPP_CALL(static_cast<Result>(vkMapMemory(reinterpret_cast<VkDevice>(device), reinterpret_cast<VkDeviceMemory>(memory), offset, size, static_cast<VkMemoryMapFlags>(flags), reinterpret_cast<void**>(&ret))));
	return ret;
}
inline void unmapMemory(Device device, DeviceMemory memory)
{
	return vkUnmapMemory(reinterpret_cast<VkDevice>(device), reinterpret_cast<VkDeviceMemory>(memory));
}
inline Result flushMappedMemoryRanges(Device device, uint32_t memoryRangeCount, const MappedMemoryRange& pMemoryRanges)
{
	return static_cast<Result>(vkFlushMappedMemoryRanges(reinterpret_cast<VkDevice>(device), memoryRangeCount, reinterpret_cast<const VkMappedMemoryRange*>(&pMemoryRanges)));
}
inline Result flushMappedMemoryRanges(Device device, const Range<const MappedMemoryRange>& pMemoryRanges)
{
	return VPP_CALL(vkFlushMappedMemoryRanges(reinterpret_cast<VkDevice>(device), pMemoryRanges.size(), reinterpret_cast<const VkMappedMemoryRange*>(pMemoryRanges.data())));
}
inline Result invalidateMappedMemoryRanges(Device device, uint32_t memoryRangeCount, const MappedMemoryRange& pMemoryRanges)
{
	return static_cast<Result>(vkInvalidateMappedMemoryRanges(reinterpret_cast<VkDevice>(device), memoryRangeCount, reinterpret_cast<const VkMappedMemoryRange*>(&pMemoryRanges)));
}
inline Result invalidateMappedMemoryRanges(Device device, const Range<const MappedMemoryRange>& pMemoryRanges)
{
	return VPP_CALL(vkInvalidateMappedMemoryRanges(reinterpret_cast<VkDevice>(device), pMemoryRanges.size(), reinterpret_cast<const VkMappedMemoryRange*>(pMemoryRanges.data())));
}
inline DeviceSize getDeviceMemoryCommitment(Device device, DeviceMemory memory)
{
	DeviceSize ret = {};
	vkGetDeviceMemoryCommitment(reinterpret_cast<VkDevice>(device), reinterpret_cast<VkDeviceMemory>(memory), reinterpret_cast<VkDeviceSize*>(&ret));
	return ret;
}
inline Result bindBufferMemory(Device device, Buffer buffer, DeviceMemory memory, DeviceSize memoryOffset)
{
	return static_cast<Result>(vkBindBufferMemory(reinterpret_cast<VkDevice>(device), reinterpret_cast<VkBuffer>(buffer), reinterpret_cast<VkDeviceMemory>(memory), memoryOffset));
}
inline Result bindImageMemory(Device device, Image image, DeviceMemory memory, DeviceSize memoryOffset)
{
	return static_cast<Result>(vkBindImageMemory(reinterpret_cast<VkDevice>(device), reinterpret_cast<VkImage>(image), reinterpret_cast<VkDeviceMemory>(memory), memoryOffset));
}
inline MemoryRequirements getBufferMemoryRequirements(Device device, Buffer buffer)
{
	MemoryRequirements ret = {};
	vkGetBufferMemoryRequirements(reinterpret_cast<VkDevice>(device), reinterpret_cast<VkBuffer>(buffer), reinterpret_cast<VkMemoryRequirements*>(&ret));
	return ret;
}
inline MemoryRequirements getImageMemoryRequirements(Device device, Image image)
{
	MemoryRequirements ret = {};
	vkGetImageMemoryRequirements(reinterpret_cast<VkDevice>(device), reinterpret_cast<VkImage>(image), reinterpret_cast<VkMemoryRequirements*>(&ret));
	return ret;
}
inline void getImageSparseMemoryRequirements(Device device, Image image, uint32_t& pSparseMemoryRequirementCount, SparseImageMemoryRequirements* pSparseMemoryRequirements = {})
{
	return vkGetImageSparseMemoryRequirements(reinterpret_cast<VkDevice>(device), reinterpret_cast<VkImage>(image), reinterpret_cast<uint32_t*>(&pSparseMemoryRequirementCount), reinterpret_cast<VkSparseImageMemoryRequirements*>(pSparseMemoryRequirements));
}
inline std::vector<SparseImageMemoryRequirements> getImageSparseMemoryRequirements(Device device, Image image)
{
	std::vector<SparseImageMemoryRequirements> ret;
	uint32_t count = 0u;
	vkGetImageSparseMemoryRequirements(reinterpret_cast<VkDevice>(device), reinterpret_cast<VkImage>(image), &count, reinterpret_cast<VkSparseImageMemoryRequirements*>(ret.data()));
	ret.resize(count);
	vkGetImageSparseMemoryRequirements(reinterpret_cast<VkDevice>(device), reinterpret_cast<VkImage>(image), &count, reinterpret_cast<VkSparseImageMemoryRequirements*>(ret.data()));
	return ret;
}
inline void getPhysicalDeviceSparseImageFormatProperties(PhysicalDevice physicalDevice, Format format, ImageType type, SampleCountBits samples, ImageUsageFlags usage, ImageTiling tiling, uint32_t& pPropertyCount, SparseImageFormatProperties* pProperties = {})
{
	return vkGetPhysicalDeviceSparseImageFormatProperties(reinterpret_cast<VkPhysicalDevice>(physicalDevice), static_cast<VkFormat>(format), static_cast<VkImageType>(type), static_cast<VkSampleCountFlagBits>(samples), static_cast<VkImageUsageFlags>(usage), static_cast<VkImageTiling>(tiling), reinterpret_cast<uint32_t*>(&pPropertyCount), reinterpret_cast<VkSparseImageFormatProperties*>(pProperties));
}
inline std::vector<SparseImageFormatProperties> getPhysicalDeviceSparseImageFormatProperties(PhysicalDevice physicalDevice, Format format, ImageType type, SampleCountBits samples, ImageUsageFlags usage, ImageTiling tiling)
{
	std::vector<SparseImageFormatProperties> ret;
	uint32_t count = 0u;
	vkGetPhysicalDeviceSparseImageFormatProperties(reinterpret_cast<VkPhysicalDevice>(physicalDevice), static_cast<VkFormat>(format), static_cast<VkImageType>(type), static_cast<VkSampleCountFlagBits>(samples), static_cast<VkImageUsageFlags>(usage), static_cast<VkImageTiling>(tiling), &count, reinterpret_cast<VkSparseImageFormatProperties*>(ret.data()));
	ret.resize(count);
	vkGetPhysicalDeviceSparseImageFormatProperties(reinterpret_cast<VkPhysicalDevice>(physicalDevice), static_cast<VkFormat>(format), static_cast<VkImageType>(type), static_cast<VkSampleCountFlagBits>(samples), static_cast<VkImageUsageFlags>(usage), static_cast<VkImageTiling>(tiling), &count, reinterpret_cast<VkSparseImageFormatProperties*>(ret.data()));
	return ret;
}
inline Result queueBindSparse(Queue queue, uint32_t bindInfoCount, const BindSparseInfo& pBindInfo, Fence fence = {})
{
	return static_cast<Result>(vkQueueBindSparse(reinterpret_cast<VkQueue>(queue), bindInfoCount, reinterpret_cast<const VkBindSparseInfo*>(&pBindInfo), reinterpret_cast<VkFence>(fence)));
}
inline Result queueBindSparse(Queue queue, const Range<const BindSparseInfo>& pBindInfo, Fence fence = {})
{
	return VPP_CALL(vkQueueBindSparse(reinterpret_cast<VkQueue>(queue), pBindInfo.size(), reinterpret_cast<const VkBindSparseInfo*>(pBindInfo.data()), reinterpret_cast<VkFence>(fence)));
}
inline Fence createFence(Device device, const FenceCreateInfo& pCreateInfo, const AllocationCallbacks* pAllocator = {})
{
	Fence ret = {};
	VPP_CALL(static_cast<Result>(vkCreateFence(reinterpret_cast<VkDevice>(device), reinterpret_cast<const VkFenceCreateInfo*>(&pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkFence*>(&ret))));
	return ret;
}
inline void destroyFence(Device device, Fence fence = {}, const AllocationCallbacks* pAllocator = {})
{
	return vkDestroyFence(reinterpret_cast<VkDevice>(device), reinterpret_cast<VkFence>(fence), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
inline Result resetFences(Device device, uint32_t fenceCount, const Fence& pFences)
{
	return static_cast<Result>(vkResetFences(reinterpret_cast<VkDevice>(device), fenceCount, reinterpret_cast<const VkFence*>(&pFences)));
}
inline Result resetFences(Device device, const Range<const Fence>& pFences)
{
	return VPP_CALL(vkResetFences(reinterpret_cast<VkDevice>(device), pFences.size(), reinterpret_cast<const VkFence*>(pFences.data())));
}
inline Result getFenceStatus(Device device, Fence fence)
{
	return static_cast<Result>(vkGetFenceStatus(reinterpret_cast<VkDevice>(device), reinterpret_cast<VkFence>(fence)));
}
inline Result waitForFences(Device device, uint32_t fenceCount, const Fence& pFences, Bool32 waitAll, uint64_t timeout)
{
	return static_cast<Result>(vkWaitForFences(reinterpret_cast<VkDevice>(device), fenceCount, reinterpret_cast<const VkFence*>(&pFences), waitAll, timeout));
}
inline Result waitForFences(Device device, const Range<const Fence>& pFences, Bool32 waitAll, uint64_t timeout)
{
	return VPP_CALL(vkWaitForFences(reinterpret_cast<VkDevice>(device), pFences.size(), reinterpret_cast<const VkFence*>(pFences.data()), waitAll, timeout));
}
inline Semaphore createSemaphore(Device device, const SemaphoreCreateInfo& pCreateInfo, const AllocationCallbacks* pAllocator = {})
{
	Semaphore ret = {};
	VPP_CALL(static_cast<Result>(vkCreateSemaphore(reinterpret_cast<VkDevice>(device), reinterpret_cast<const VkSemaphoreCreateInfo*>(&pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkSemaphore*>(&ret))));
	return ret;
}
inline void destroySemaphore(Device device, Semaphore semaphore = {}, const AllocationCallbacks* pAllocator = {})
{
	return vkDestroySemaphore(reinterpret_cast<VkDevice>(device), reinterpret_cast<VkSemaphore>(semaphore), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
inline Event createEvent(Device device, const EventCreateInfo& pCreateInfo, const AllocationCallbacks* pAllocator = {})
{
	Event ret = {};
	VPP_CALL(static_cast<Result>(vkCreateEvent(reinterpret_cast<VkDevice>(device), reinterpret_cast<const VkEventCreateInfo*>(&pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkEvent*>(&ret))));
	return ret;
}
inline void destroyEvent(Device device, Event event = {}, const AllocationCallbacks* pAllocator = {})
{
	return vkDestroyEvent(reinterpret_cast<VkDevice>(device), reinterpret_cast<VkEvent>(event), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
inline Result getEventStatus(Device device, Event event)
{
	return static_cast<Result>(vkGetEventStatus(reinterpret_cast<VkDevice>(device), reinterpret_cast<VkEvent>(event)));
}
inline Result setEvent(Device device, Event event)
{
	return static_cast<Result>(vkSetEvent(reinterpret_cast<VkDevice>(device), reinterpret_cast<VkEvent>(event)));
}
inline Result resetEvent(Device device, Event event)
{
	return static_cast<Result>(vkResetEvent(reinterpret_cast<VkDevice>(device), reinterpret_cast<VkEvent>(event)));
}
inline QueryPool createQueryPool(Device device, const QueryPoolCreateInfo& pCreateInfo, const AllocationCallbacks* pAllocator = {})
{
	QueryPool ret = {};
	VPP_CALL(static_cast<Result>(vkCreateQueryPool(reinterpret_cast<VkDevice>(device), reinterpret_cast<const VkQueryPoolCreateInfo*>(&pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkQueryPool*>(&ret))));
	return ret;
}
inline void destroyQueryPool(Device device, QueryPool queryPool = {}, const AllocationCallbacks* pAllocator = {})
{
	return vkDestroyQueryPool(reinterpret_cast<VkDevice>(device), reinterpret_cast<VkQueryPool>(queryPool), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
inline Result getQueryPoolResults(Device device, QueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, size_t dataSize, void* pData, DeviceSize stride, QueryResultFlags flags = {})
{
	return static_cast<Result>(vkGetQueryPoolResults(reinterpret_cast<VkDevice>(device), reinterpret_cast<VkQueryPool>(queryPool), firstQuery, queryCount, dataSize, reinterpret_cast<void*>(pData), stride, static_cast<VkQueryResultFlags>(flags)));
}
inline std::vector<uint8_t> getQueryPoolResults(Device device, QueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, size_t dataSize, DeviceSize stride, QueryResultFlags flags = {})
{
	std::vector<uint8_t> ret;
	ret.resize(dataSize);
	VPP_CALL(vkGetQueryPoolResults(reinterpret_cast<VkDevice>(device), reinterpret_cast<VkQueryPool>(queryPool), firstQuery, queryCount, dataSize, reinterpret_cast<void*>(ret.data()), stride, static_cast<VkQueryResultFlags>(flags)));
	return ret;
}
inline Buffer createBuffer(Device device, const BufferCreateInfo& pCreateInfo, const AllocationCallbacks* pAllocator = {})
{
	Buffer ret = {};
	VPP_CALL(static_cast<Result>(vkCreateBuffer(reinterpret_cast<VkDevice>(device), reinterpret_cast<const VkBufferCreateInfo*>(&pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkBuffer*>(&ret))));
	return ret;
}
inline void destroyBuffer(Device device, Buffer buffer = {}, const AllocationCallbacks* pAllocator = {})
{
	return vkDestroyBuffer(reinterpret_cast<VkDevice>(device), reinterpret_cast<VkBuffer>(buffer), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
inline BufferView createBufferView(Device device, const BufferViewCreateInfo& pCreateInfo, const AllocationCallbacks* pAllocator = {})
{
	BufferView ret = {};
	VPP_CALL(static_cast<Result>(vkCreateBufferView(reinterpret_cast<VkDevice>(device), reinterpret_cast<const VkBufferViewCreateInfo*>(&pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkBufferView*>(&ret))));
	return ret;
}
inline void destroyBufferView(Device device, BufferView bufferView = {}, const AllocationCallbacks* pAllocator = {})
{
	return vkDestroyBufferView(reinterpret_cast<VkDevice>(device), reinterpret_cast<VkBufferView>(bufferView), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
inline Image createImage(Device device, const ImageCreateInfo& pCreateInfo, const AllocationCallbacks* pAllocator = {})
{
	Image ret = {};
	VPP_CALL(static_cast<Result>(vkCreateImage(reinterpret_cast<VkDevice>(device), reinterpret_cast<const VkImageCreateInfo*>(&pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkImage*>(&ret))));
	return ret;
}
inline void destroyImage(Device device, Image image = {}, const AllocationCallbacks* pAllocator = {})
{
	return vkDestroyImage(reinterpret_cast<VkDevice>(device), reinterpret_cast<VkImage>(image), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
inline SubresourceLayout getImageSubresourceLayout(Device device, Image image, const ImageSubresource& pSubresource)
{
	SubresourceLayout ret = {};
	vkGetImageSubresourceLayout(reinterpret_cast<VkDevice>(device), reinterpret_cast<VkImage>(image), reinterpret_cast<const VkImageSubresource*>(&pSubresource), reinterpret_cast<VkSubresourceLayout*>(&ret));
	return ret;
}
inline ImageView createImageView(Device device, const ImageViewCreateInfo& pCreateInfo, const AllocationCallbacks* pAllocator = {})
{
	ImageView ret = {};
	VPP_CALL(static_cast<Result>(vkCreateImageView(reinterpret_cast<VkDevice>(device), reinterpret_cast<const VkImageViewCreateInfo*>(&pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkImageView*>(&ret))));
	return ret;
}
inline void destroyImageView(Device device, ImageView imageView = {}, const AllocationCallbacks* pAllocator = {})
{
	return vkDestroyImageView(reinterpret_cast<VkDevice>(device), reinterpret_cast<VkImageView>(imageView), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
inline ShaderModule createShaderModule(Device device, const ShaderModuleCreateInfo& pCreateInfo, const AllocationCallbacks* pAllocator = {})
{
	ShaderModule ret = {};
	VPP_CALL(static_cast<Result>(vkCreateShaderModule(reinterpret_cast<VkDevice>(device), reinterpret_cast<const VkShaderModuleCreateInfo*>(&pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkShaderModule*>(&ret))));
	return ret;
}
inline void destroyShaderModule(Device device, ShaderModule shaderModule = {}, const AllocationCallbacks* pAllocator = {})
{
	return vkDestroyShaderModule(reinterpret_cast<VkDevice>(device), reinterpret_cast<VkShaderModule>(shaderModule), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
inline PipelineCache createPipelineCache(Device device, const PipelineCacheCreateInfo& pCreateInfo, const AllocationCallbacks* pAllocator = {})
{
	PipelineCache ret = {};
	VPP_CALL(static_cast<Result>(vkCreatePipelineCache(reinterpret_cast<VkDevice>(device), reinterpret_cast<const VkPipelineCacheCreateInfo*>(&pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkPipelineCache*>(&ret))));
	return ret;
}
inline void destroyPipelineCache(Device device, PipelineCache pipelineCache = {}, const AllocationCallbacks* pAllocator = {})
{
	return vkDestroyPipelineCache(reinterpret_cast<VkDevice>(device), reinterpret_cast<VkPipelineCache>(pipelineCache), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
inline Result getPipelineCacheData(Device device, PipelineCache pipelineCache, size_t& pDataSize, void* pData = {})
{
	return static_cast<Result>(vkGetPipelineCacheData(reinterpret_cast<VkDevice>(device), reinterpret_cast<VkPipelineCache>(pipelineCache), reinterpret_cast<size_t*>(&pDataSize), reinterpret_cast<void*>(pData)));
}
inline std::vector<uint8_t> getPipelineCacheData(Device device, PipelineCache pipelineCache)
{
	std::vector<uint8_t> ret;
	size_t count = 0u;
	if(!call::success(VPP_CALL(vkGetPipelineCacheData(reinterpret_cast<VkDevice>(device), reinterpret_cast<VkPipelineCache>(pipelineCache), &count, reinterpret_cast<void*>(ret.data()))))) return ret;
	ret.resize(count);
	VPP_CALL(vkGetPipelineCacheData(reinterpret_cast<VkDevice>(device), reinterpret_cast<VkPipelineCache>(pipelineCache), &count, reinterpret_cast<void*>(ret.data())));
	return ret;
}
inline Result mergePipelineCaches(Device device, PipelineCache dstCache, uint32_t srcCacheCount, const PipelineCache& pSrcCaches)
{
	return static_cast<Result>(vkMergePipelineCaches(reinterpret_cast<VkDevice>(device), reinterpret_cast<VkPipelineCache>(dstCache), srcCacheCount, reinterpret_cast<const VkPipelineCache*>(&pSrcCaches)));
}
inline Result mergePipelineCaches(Device device, PipelineCache dstCache, const Range<const PipelineCache>& pSrcCaches)
{
	return VPP_CALL(vkMergePipelineCaches(reinterpret_cast<VkDevice>(device), reinterpret_cast<VkPipelineCache>(dstCache), pSrcCaches.size(), reinterpret_cast<const VkPipelineCache*>(pSrcCaches.data())));
}
inline Result createGraphicsPipelines(Device device, PipelineCache pipelineCache, uint32_t createInfoCount, const GraphicsPipelineCreateInfo& pCreateInfos, const AllocationCallbacks* pAllocator, Pipeline& pPipelines)
{
	return static_cast<Result>(vkCreateGraphicsPipelines(reinterpret_cast<VkDevice>(device), reinterpret_cast<VkPipelineCache>(pipelineCache), createInfoCount, reinterpret_cast<const VkGraphicsPipelineCreateInfo*>(&pCreateInfos), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkPipeline*>(&pPipelines)));
}
inline std::vector<Pipeline> createGraphicsPipelines(Device device, PipelineCache pipelineCache, const Range<const GraphicsPipelineCreateInfo>& pCreateInfos, const AllocationCallbacks* pAllocator = {})
{
	std::vector<Pipeline> ret;
	ret.resize(pCreateInfos.size());
	VPP_CALL(vkCreateGraphicsPipelines(reinterpret_cast<VkDevice>(device), reinterpret_cast<VkPipelineCache>(pipelineCache), pCreateInfos.size(), reinterpret_cast<const VkGraphicsPipelineCreateInfo*>(pCreateInfos.data()), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkPipeline*>(ret.data())));
	return ret;
}
inline Result createComputePipelines(Device device, PipelineCache pipelineCache, uint32_t createInfoCount, const ComputePipelineCreateInfo& pCreateInfos, const AllocationCallbacks* pAllocator, Pipeline& pPipelines)
{
	return static_cast<Result>(vkCreateComputePipelines(reinterpret_cast<VkDevice>(device), reinterpret_cast<VkPipelineCache>(pipelineCache), createInfoCount, reinterpret_cast<const VkComputePipelineCreateInfo*>(&pCreateInfos), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkPipeline*>(&pPipelines)));
}
inline std::vector<Pipeline> createComputePipelines(Device device, PipelineCache pipelineCache, const Range<const ComputePipelineCreateInfo>& pCreateInfos, const AllocationCallbacks* pAllocator = {})
{
	std::vector<Pipeline> ret;
	ret.resize(pCreateInfos.size());
	VPP_CALL(vkCreateComputePipelines(reinterpret_cast<VkDevice>(device), reinterpret_cast<VkPipelineCache>(pipelineCache), pCreateInfos.size(), reinterpret_cast<const VkComputePipelineCreateInfo*>(pCreateInfos.data()), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkPipeline*>(ret.data())));
	return ret;
}
inline void destroyPipeline(Device device, Pipeline pipeline = {}, const AllocationCallbacks* pAllocator = {})
{
	return vkDestroyPipeline(reinterpret_cast<VkDevice>(device), reinterpret_cast<VkPipeline>(pipeline), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
inline PipelineLayout createPipelineLayout(Device device, const PipelineLayoutCreateInfo& pCreateInfo, const AllocationCallbacks* pAllocator = {})
{
	PipelineLayout ret = {};
	VPP_CALL(static_cast<Result>(vkCreatePipelineLayout(reinterpret_cast<VkDevice>(device), reinterpret_cast<const VkPipelineLayoutCreateInfo*>(&pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkPipelineLayout*>(&ret))));
	return ret;
}
inline void destroyPipelineLayout(Device device, PipelineLayout pipelineLayout = {}, const AllocationCallbacks* pAllocator = {})
{
	return vkDestroyPipelineLayout(reinterpret_cast<VkDevice>(device), reinterpret_cast<VkPipelineLayout>(pipelineLayout), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
inline Sampler createSampler(Device device, const SamplerCreateInfo& pCreateInfo, const AllocationCallbacks* pAllocator = {})
{
	Sampler ret = {};
	VPP_CALL(static_cast<Result>(vkCreateSampler(reinterpret_cast<VkDevice>(device), reinterpret_cast<const VkSamplerCreateInfo*>(&pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkSampler*>(&ret))));
	return ret;
}
inline void destroySampler(Device device, Sampler sampler = {}, const AllocationCallbacks* pAllocator = {})
{
	return vkDestroySampler(reinterpret_cast<VkDevice>(device), reinterpret_cast<VkSampler>(sampler), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
inline DescriptorSetLayout createDescriptorSetLayout(Device device, const DescriptorSetLayoutCreateInfo& pCreateInfo, const AllocationCallbacks* pAllocator = {})
{
	DescriptorSetLayout ret = {};
	VPP_CALL(static_cast<Result>(vkCreateDescriptorSetLayout(reinterpret_cast<VkDevice>(device), reinterpret_cast<const VkDescriptorSetLayoutCreateInfo*>(&pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkDescriptorSetLayout*>(&ret))));
	return ret;
}
inline void destroyDescriptorSetLayout(Device device, DescriptorSetLayout descriptorSetLayout = {}, const AllocationCallbacks* pAllocator = {})
{
	return vkDestroyDescriptorSetLayout(reinterpret_cast<VkDevice>(device), reinterpret_cast<VkDescriptorSetLayout>(descriptorSetLayout), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
inline DescriptorPool createDescriptorPool(Device device, const DescriptorPoolCreateInfo& pCreateInfo, const AllocationCallbacks* pAllocator = {})
{
	DescriptorPool ret = {};
	VPP_CALL(static_cast<Result>(vkCreateDescriptorPool(reinterpret_cast<VkDevice>(device), reinterpret_cast<const VkDescriptorPoolCreateInfo*>(&pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkDescriptorPool*>(&ret))));
	return ret;
}
inline void destroyDescriptorPool(Device device, DescriptorPool descriptorPool = {}, const AllocationCallbacks* pAllocator = {})
{
	return vkDestroyDescriptorPool(reinterpret_cast<VkDevice>(device), reinterpret_cast<VkDescriptorPool>(descriptorPool), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
inline Result resetDescriptorPool(Device device, DescriptorPool descriptorPool, DescriptorPoolResetFlags flags = {})
{
	return static_cast<Result>(vkResetDescriptorPool(reinterpret_cast<VkDevice>(device), reinterpret_cast<VkDescriptorPool>(descriptorPool), static_cast<VkDescriptorPoolResetFlags>(flags)));
}
inline Result allocateDescriptorSets(Device device, const DescriptorSetAllocateInfo& pAllocateInfo, DescriptorSet& pDescriptorSets)
{
	return static_cast<Result>(vkAllocateDescriptorSets(reinterpret_cast<VkDevice>(device), reinterpret_cast<const VkDescriptorSetAllocateInfo*>(&pAllocateInfo), reinterpret_cast<VkDescriptorSet*>(&pDescriptorSets)));
}
inline std::vector<DescriptorSet> allocateDescriptorSets(Device device, const DescriptorSetAllocateInfo& pAllocateInfo)
{
	std::vector<DescriptorSet> ret;
	ret.resize(pAllocateInfo.descriptorSetCount);
	VPP_CALL(vkAllocateDescriptorSets(reinterpret_cast<VkDevice>(device), reinterpret_cast<const VkDescriptorSetAllocateInfo*>(&pAllocateInfo), reinterpret_cast<VkDescriptorSet*>(ret.data())));
	return ret;
}
inline Result freeDescriptorSets(Device device, DescriptorPool descriptorPool, uint32_t descriptorSetCount, const DescriptorSet& pDescriptorSets)
{
	return static_cast<Result>(vkFreeDescriptorSets(reinterpret_cast<VkDevice>(device), reinterpret_cast<VkDescriptorPool>(descriptorPool), descriptorSetCount, reinterpret_cast<const VkDescriptorSet*>(&pDescriptorSets)));
}
inline Result freeDescriptorSets(Device device, DescriptorPool descriptorPool, const Range<const DescriptorSet>& pDescriptorSets)
{
	return VPP_CALL(vkFreeDescriptorSets(reinterpret_cast<VkDevice>(device), reinterpret_cast<VkDescriptorPool>(descriptorPool), pDescriptorSets.size(), reinterpret_cast<const VkDescriptorSet*>(pDescriptorSets.data())));
}
inline void updateDescriptorSets(Device device, uint32_t descriptorWriteCount, const WriteDescriptorSet& pDescriptorWrites, uint32_t descriptorCopyCount, const CopyDescriptorSet& pDescriptorCopies)
{
	return vkUpdateDescriptorSets(reinterpret_cast<VkDevice>(device), descriptorWriteCount, reinterpret_cast<const VkWriteDescriptorSet*>(&pDescriptorWrites), descriptorCopyCount, reinterpret_cast<const VkCopyDescriptorSet*>(&pDescriptorCopies));
}
inline void updateDescriptorSets(Device device, const Range<const WriteDescriptorSet>& pDescriptorWrites, const Range<const CopyDescriptorSet>& pDescriptorCopies)
{
	vkUpdateDescriptorSets(reinterpret_cast<VkDevice>(device), pDescriptorWrites.size(), reinterpret_cast<const VkWriteDescriptorSet*>(pDescriptorWrites.data()), pDescriptorCopies.size(), reinterpret_cast<const VkCopyDescriptorSet*>(pDescriptorCopies.data()));
}
inline Framebuffer createFramebuffer(Device device, const FramebufferCreateInfo& pCreateInfo, const AllocationCallbacks* pAllocator = {})
{
	Framebuffer ret = {};
	VPP_CALL(static_cast<Result>(vkCreateFramebuffer(reinterpret_cast<VkDevice>(device), reinterpret_cast<const VkFramebufferCreateInfo*>(&pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkFramebuffer*>(&ret))));
	return ret;
}
inline void destroyFramebuffer(Device device, Framebuffer framebuffer = {}, const AllocationCallbacks* pAllocator = {})
{
	return vkDestroyFramebuffer(reinterpret_cast<VkDevice>(device), reinterpret_cast<VkFramebuffer>(framebuffer), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
inline RenderPass createRenderPass(Device device, const RenderPassCreateInfo& pCreateInfo, const AllocationCallbacks* pAllocator = {})
{
	RenderPass ret = {};
	VPP_CALL(static_cast<Result>(vkCreateRenderPass(reinterpret_cast<VkDevice>(device), reinterpret_cast<const VkRenderPassCreateInfo*>(&pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkRenderPass*>(&ret))));
	return ret;
}
inline void destroyRenderPass(Device device, RenderPass renderPass = {}, const AllocationCallbacks* pAllocator = {})
{
	return vkDestroyRenderPass(reinterpret_cast<VkDevice>(device), reinterpret_cast<VkRenderPass>(renderPass), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
inline void getRenderAreaGranularity(Device device, RenderPass renderPass, Extent2D& pGranularity)
{
	return vkGetRenderAreaGranularity(reinterpret_cast<VkDevice>(device), reinterpret_cast<VkRenderPass>(renderPass), reinterpret_cast<VkExtent2D*>(&pGranularity));
}
inline CommandPool createCommandPool(Device device, const CommandPoolCreateInfo& pCreateInfo, const AllocationCallbacks* pAllocator = {})
{
	CommandPool ret = {};
	VPP_CALL(static_cast<Result>(vkCreateCommandPool(reinterpret_cast<VkDevice>(device), reinterpret_cast<const VkCommandPoolCreateInfo*>(&pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkCommandPool*>(&ret))));
	return ret;
}
inline void destroyCommandPool(Device device, CommandPool commandPool = {}, const AllocationCallbacks* pAllocator = {})
{
	return vkDestroyCommandPool(reinterpret_cast<VkDevice>(device), reinterpret_cast<VkCommandPool>(commandPool), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
inline Result resetCommandPool(Device device, CommandPool commandPool, CommandPoolResetFlags flags = {})
{
	return static_cast<Result>(vkResetCommandPool(reinterpret_cast<VkDevice>(device), reinterpret_cast<VkCommandPool>(commandPool), static_cast<VkCommandPoolResetFlags>(flags)));
}
inline Result allocateCommandBuffers(Device device, const CommandBufferAllocateInfo& pAllocateInfo, CommandBuffer& pCommandBuffers)
{
	return static_cast<Result>(vkAllocateCommandBuffers(reinterpret_cast<VkDevice>(device), reinterpret_cast<const VkCommandBufferAllocateInfo*>(&pAllocateInfo), reinterpret_cast<VkCommandBuffer*>(&pCommandBuffers)));
}
inline std::vector<CommandBuffer> allocateCommandBuffers(Device device, const CommandBufferAllocateInfo& pAllocateInfo)
{
	std::vector<CommandBuffer> ret;
	ret.resize(pAllocateInfo.commandBufferCount);
	VPP_CALL(vkAllocateCommandBuffers(reinterpret_cast<VkDevice>(device), reinterpret_cast<const VkCommandBufferAllocateInfo*>(&pAllocateInfo), reinterpret_cast<VkCommandBuffer*>(ret.data())));
	return ret;
}
inline void freeCommandBuffers(Device device, CommandPool commandPool, uint32_t commandBufferCount, const CommandBuffer& pCommandBuffers)
{
	return vkFreeCommandBuffers(reinterpret_cast<VkDevice>(device), reinterpret_cast<VkCommandPool>(commandPool), commandBufferCount, reinterpret_cast<const VkCommandBuffer*>(&pCommandBuffers));
}
inline void freeCommandBuffers(Device device, CommandPool commandPool, const Range<const CommandBuffer>& pCommandBuffers)
{
	vkFreeCommandBuffers(reinterpret_cast<VkDevice>(device), reinterpret_cast<VkCommandPool>(commandPool), pCommandBuffers.size(), reinterpret_cast<const VkCommandBuffer*>(pCommandBuffers.data()));
}
inline Result beginCommandBuffer(CommandBuffer commandBuffer, const CommandBufferBeginInfo& pBeginInfo)
{
	return static_cast<Result>(vkBeginCommandBuffer(reinterpret_cast<VkCommandBuffer>(commandBuffer), reinterpret_cast<const VkCommandBufferBeginInfo*>(&pBeginInfo)));
}
inline Result endCommandBuffer(CommandBuffer commandBuffer)
{
	return static_cast<Result>(vkEndCommandBuffer(reinterpret_cast<VkCommandBuffer>(commandBuffer)));
}
inline Result resetCommandBuffer(CommandBuffer commandBuffer, CommandBufferResetFlags flags = {})
{
	return static_cast<Result>(vkResetCommandBuffer(reinterpret_cast<VkCommandBuffer>(commandBuffer), static_cast<VkCommandBufferResetFlags>(flags)));
}
inline void cmdBindPipeline(CommandBuffer commandBuffer, PipelineBindPoint pipelineBindPoint, Pipeline pipeline)
{
	return vkCmdBindPipeline(reinterpret_cast<VkCommandBuffer>(commandBuffer), static_cast<VkPipelineBindPoint>(pipelineBindPoint), reinterpret_cast<VkPipeline>(pipeline));
}
inline void cmdSetViewport(CommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const Viewport& pViewports)
{
	return vkCmdSetViewport(reinterpret_cast<VkCommandBuffer>(commandBuffer), firstViewport, viewportCount, reinterpret_cast<const VkViewport*>(&pViewports));
}
inline void cmdSetViewport(CommandBuffer commandBuffer, uint32_t firstViewport, const Range<const Viewport>& pViewports)
{
	vkCmdSetViewport(reinterpret_cast<VkCommandBuffer>(commandBuffer), firstViewport, pViewports.size(), reinterpret_cast<const VkViewport*>(pViewports.data()));
}
inline void cmdSetScissor(CommandBuffer commandBuffer, uint32_t firstScissor, uint32_t scissorCount, const Rect2D& pScissors)
{
	return vkCmdSetScissor(reinterpret_cast<VkCommandBuffer>(commandBuffer), firstScissor, scissorCount, reinterpret_cast<const VkRect2D*>(&pScissors));
}
inline void cmdSetScissor(CommandBuffer commandBuffer, uint32_t firstScissor, const Range<const Rect2D>& pScissors)
{
	vkCmdSetScissor(reinterpret_cast<VkCommandBuffer>(commandBuffer), firstScissor, pScissors.size(), reinterpret_cast<const VkRect2D*>(pScissors.data()));
}
inline void cmdSetLineWidth(CommandBuffer commandBuffer, float lineWidth)
{
	return vkCmdSetLineWidth(reinterpret_cast<VkCommandBuffer>(commandBuffer), lineWidth);
}
inline void cmdSetDepthBias(CommandBuffer commandBuffer, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor)
{
	return vkCmdSetDepthBias(reinterpret_cast<VkCommandBuffer>(commandBuffer), depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor);
}
inline void cmdSetBlendConstants(CommandBuffer commandBuffer, std::array<const float, 4> blendConstants)
{
	return vkCmdSetBlendConstants(reinterpret_cast<VkCommandBuffer>(commandBuffer), blendConstants.data());
}
inline void cmdSetDepthBounds(CommandBuffer commandBuffer, float minDepthBounds, float maxDepthBounds)
{
	return vkCmdSetDepthBounds(reinterpret_cast<VkCommandBuffer>(commandBuffer), minDepthBounds, maxDepthBounds);
}
inline void cmdSetStencilCompareMask(CommandBuffer commandBuffer, StencilFaceFlags faceMask, uint32_t compareMask)
{
	return vkCmdSetStencilCompareMask(reinterpret_cast<VkCommandBuffer>(commandBuffer), static_cast<VkStencilFaceFlags>(faceMask), compareMask);
}
inline void cmdSetStencilWriteMask(CommandBuffer commandBuffer, StencilFaceFlags faceMask, uint32_t writeMask)
{
	return vkCmdSetStencilWriteMask(reinterpret_cast<VkCommandBuffer>(commandBuffer), static_cast<VkStencilFaceFlags>(faceMask), writeMask);
}
inline void cmdSetStencilReference(CommandBuffer commandBuffer, StencilFaceFlags faceMask, uint32_t reference)
{
	return vkCmdSetStencilReference(reinterpret_cast<VkCommandBuffer>(commandBuffer), static_cast<VkStencilFaceFlags>(faceMask), reference);
}
inline void cmdBindDescriptorSets(CommandBuffer commandBuffer, PipelineBindPoint pipelineBindPoint, PipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, const DescriptorSet& pDescriptorSets, uint32_t dynamicOffsetCount, const uint32_t& pDynamicOffsets)
{
	return vkCmdBindDescriptorSets(reinterpret_cast<VkCommandBuffer>(commandBuffer), static_cast<VkPipelineBindPoint>(pipelineBindPoint), reinterpret_cast<VkPipelineLayout>(layout), firstSet, descriptorSetCount, reinterpret_cast<const VkDescriptorSet*>(&pDescriptorSets), dynamicOffsetCount, reinterpret_cast<const uint32_t*>(&pDynamicOffsets));
}
inline void cmdBindDescriptorSets(CommandBuffer commandBuffer, PipelineBindPoint pipelineBindPoint, PipelineLayout layout, uint32_t firstSet, const Range<const DescriptorSet>& pDescriptorSets, const Range<const uint32_t>& pDynamicOffsets)
{
	vkCmdBindDescriptorSets(reinterpret_cast<VkCommandBuffer>(commandBuffer), static_cast<VkPipelineBindPoint>(pipelineBindPoint), reinterpret_cast<VkPipelineLayout>(layout), firstSet, pDescriptorSets.size(), reinterpret_cast<const VkDescriptorSet*>(pDescriptorSets.data()), pDynamicOffsets.size(), reinterpret_cast<const uint32_t*>(pDynamicOffsets.data()));
}
inline void cmdBindIndexBuffer(CommandBuffer commandBuffer, Buffer buffer, DeviceSize offset, IndexType indexType)
{
	return vkCmdBindIndexBuffer(reinterpret_cast<VkCommandBuffer>(commandBuffer), reinterpret_cast<VkBuffer>(buffer), offset, static_cast<VkIndexType>(indexType));
}
inline void cmdBindVertexBuffers(CommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const Buffer& pBuffers, const DeviceSize& pOffsets)
{
	return vkCmdBindVertexBuffers(reinterpret_cast<VkCommandBuffer>(commandBuffer), firstBinding, bindingCount, reinterpret_cast<const VkBuffer*>(&pBuffers), reinterpret_cast<const VkDeviceSize*>(&pOffsets));
}
inline void cmdBindVertexBuffers(CommandBuffer commandBuffer, uint32_t firstBinding, const Range<const Buffer>& pBuffers, const Range<const DeviceSize>& pOffsets)
{
	vkCmdBindVertexBuffers(reinterpret_cast<VkCommandBuffer>(commandBuffer), firstBinding, pBuffers.size(), reinterpret_cast<const VkBuffer*>(pBuffers.data()), reinterpret_cast<const VkDeviceSize*>(pOffsets.data()));
}
inline void cmdDraw(CommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance)
{
	return vkCmdDraw(reinterpret_cast<VkCommandBuffer>(commandBuffer), vertexCount, instanceCount, firstVertex, firstInstance);
}
inline void cmdDrawIndexed(CommandBuffer commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance)
{
	return vkCmdDrawIndexed(reinterpret_cast<VkCommandBuffer>(commandBuffer), indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
}
inline void cmdDrawIndirect(CommandBuffer commandBuffer, Buffer buffer, DeviceSize offset, uint32_t drawCount, uint32_t stride)
{
	return vkCmdDrawIndirect(reinterpret_cast<VkCommandBuffer>(commandBuffer), reinterpret_cast<VkBuffer>(buffer), offset, drawCount, stride);
}
inline void cmdDrawIndexedIndirect(CommandBuffer commandBuffer, Buffer buffer, DeviceSize offset, uint32_t drawCount, uint32_t stride)
{
	return vkCmdDrawIndexedIndirect(reinterpret_cast<VkCommandBuffer>(commandBuffer), reinterpret_cast<VkBuffer>(buffer), offset, drawCount, stride);
}
inline void cmdDispatch(CommandBuffer commandBuffer, uint32_t x, uint32_t y, uint32_t z)
{
	return vkCmdDispatch(reinterpret_cast<VkCommandBuffer>(commandBuffer), x, y, z);
}
inline void cmdDispatchIndirect(CommandBuffer commandBuffer, Buffer buffer, DeviceSize offset)
{
	return vkCmdDispatchIndirect(reinterpret_cast<VkCommandBuffer>(commandBuffer), reinterpret_cast<VkBuffer>(buffer), offset);
}
inline void cmdCopyBuffer(CommandBuffer commandBuffer, Buffer srcBuffer, Buffer dstBuffer, uint32_t regionCount, const BufferCopy& pRegions)
{
	return vkCmdCopyBuffer(reinterpret_cast<VkCommandBuffer>(commandBuffer), reinterpret_cast<VkBuffer>(srcBuffer), reinterpret_cast<VkBuffer>(dstBuffer), regionCount, reinterpret_cast<const VkBufferCopy*>(&pRegions));
}
inline void cmdCopyBuffer(CommandBuffer commandBuffer, Buffer srcBuffer, Buffer dstBuffer, const Range<const BufferCopy>& pRegions)
{
	vkCmdCopyBuffer(reinterpret_cast<VkCommandBuffer>(commandBuffer), reinterpret_cast<VkBuffer>(srcBuffer), reinterpret_cast<VkBuffer>(dstBuffer), pRegions.size(), reinterpret_cast<const VkBufferCopy*>(pRegions.data()));
}
inline void cmdCopyImage(CommandBuffer commandBuffer, Image srcImage, ImageLayout srcImageLayout, Image dstImage, ImageLayout dstImageLayout, uint32_t regionCount, const ImageCopy& pRegions)
{
	return vkCmdCopyImage(reinterpret_cast<VkCommandBuffer>(commandBuffer), reinterpret_cast<VkImage>(srcImage), static_cast<VkImageLayout>(srcImageLayout), reinterpret_cast<VkImage>(dstImage), static_cast<VkImageLayout>(dstImageLayout), regionCount, reinterpret_cast<const VkImageCopy*>(&pRegions));
}
inline void cmdCopyImage(CommandBuffer commandBuffer, Image srcImage, ImageLayout srcImageLayout, Image dstImage, ImageLayout dstImageLayout, const Range<const ImageCopy>& pRegions)
{
	vkCmdCopyImage(reinterpret_cast<VkCommandBuffer>(commandBuffer), reinterpret_cast<VkImage>(srcImage), static_cast<VkImageLayout>(srcImageLayout), reinterpret_cast<VkImage>(dstImage), static_cast<VkImageLayout>(dstImageLayout), pRegions.size(), reinterpret_cast<const VkImageCopy*>(pRegions.data()));
}
inline void cmdBlitImage(CommandBuffer commandBuffer, Image srcImage, ImageLayout srcImageLayout, Image dstImage, ImageLayout dstImageLayout, uint32_t regionCount, const ImageBlit& pRegions, Filter filter)
{
	return vkCmdBlitImage(reinterpret_cast<VkCommandBuffer>(commandBuffer), reinterpret_cast<VkImage>(srcImage), static_cast<VkImageLayout>(srcImageLayout), reinterpret_cast<VkImage>(dstImage), static_cast<VkImageLayout>(dstImageLayout), regionCount, reinterpret_cast<const VkImageBlit*>(&pRegions), static_cast<VkFilter>(filter));
}
inline void cmdBlitImage(CommandBuffer commandBuffer, Image srcImage, ImageLayout srcImageLayout, Image dstImage, ImageLayout dstImageLayout, const Range<const ImageBlit>& pRegions, Filter filter)
{
	vkCmdBlitImage(reinterpret_cast<VkCommandBuffer>(commandBuffer), reinterpret_cast<VkImage>(srcImage), static_cast<VkImageLayout>(srcImageLayout), reinterpret_cast<VkImage>(dstImage), static_cast<VkImageLayout>(dstImageLayout), pRegions.size(), reinterpret_cast<const VkImageBlit*>(pRegions.data()), static_cast<VkFilter>(filter));
}
inline void cmdCopyBufferToImage(CommandBuffer commandBuffer, Buffer srcBuffer, Image dstImage, ImageLayout dstImageLayout, uint32_t regionCount, const BufferImageCopy& pRegions)
{
	return vkCmdCopyBufferToImage(reinterpret_cast<VkCommandBuffer>(commandBuffer), reinterpret_cast<VkBuffer>(srcBuffer), reinterpret_cast<VkImage>(dstImage), static_cast<VkImageLayout>(dstImageLayout), regionCount, reinterpret_cast<const VkBufferImageCopy*>(&pRegions));
}
inline void cmdCopyBufferToImage(CommandBuffer commandBuffer, Buffer srcBuffer, Image dstImage, ImageLayout dstImageLayout, const Range<const BufferImageCopy>& pRegions)
{
	vkCmdCopyBufferToImage(reinterpret_cast<VkCommandBuffer>(commandBuffer), reinterpret_cast<VkBuffer>(srcBuffer), reinterpret_cast<VkImage>(dstImage), static_cast<VkImageLayout>(dstImageLayout), pRegions.size(), reinterpret_cast<const VkBufferImageCopy*>(pRegions.data()));
}
inline void cmdCopyImageToBuffer(CommandBuffer commandBuffer, Image srcImage, ImageLayout srcImageLayout, Buffer dstBuffer, uint32_t regionCount, const BufferImageCopy& pRegions)
{
	return vkCmdCopyImageToBuffer(reinterpret_cast<VkCommandBuffer>(commandBuffer), reinterpret_cast<VkImage>(srcImage), static_cast<VkImageLayout>(srcImageLayout), reinterpret_cast<VkBuffer>(dstBuffer), regionCount, reinterpret_cast<const VkBufferImageCopy*>(&pRegions));
}
inline void cmdCopyImageToBuffer(CommandBuffer commandBuffer, Image srcImage, ImageLayout srcImageLayout, Buffer dstBuffer, const Range<const BufferImageCopy>& pRegions)
{
	vkCmdCopyImageToBuffer(reinterpret_cast<VkCommandBuffer>(commandBuffer), reinterpret_cast<VkImage>(srcImage), static_cast<VkImageLayout>(srcImageLayout), reinterpret_cast<VkBuffer>(dstBuffer), pRegions.size(), reinterpret_cast<const VkBufferImageCopy*>(pRegions.data()));
}
inline void cmdUpdateBuffer(CommandBuffer commandBuffer, Buffer dstBuffer, DeviceSize dstOffset, DeviceSize dataSize, const uint32_t& pData)
{
	return vkCmdUpdateBuffer(reinterpret_cast<VkCommandBuffer>(commandBuffer), reinterpret_cast<VkBuffer>(dstBuffer), dstOffset, dataSize, reinterpret_cast<const uint32_t*>(&pData));
}
inline void cmdFillBuffer(CommandBuffer commandBuffer, Buffer dstBuffer, DeviceSize dstOffset, DeviceSize size, uint32_t data)
{
	return vkCmdFillBuffer(reinterpret_cast<VkCommandBuffer>(commandBuffer), reinterpret_cast<VkBuffer>(dstBuffer), dstOffset, size, data);
}
inline void cmdClearColorImage(CommandBuffer commandBuffer, Image image, ImageLayout imageLayout, const ClearColorValue& pColor, uint32_t rangeCount, const ImageSubresourceRange& pRanges)
{
	return vkCmdClearColorImage(reinterpret_cast<VkCommandBuffer>(commandBuffer), reinterpret_cast<VkImage>(image), static_cast<VkImageLayout>(imageLayout), reinterpret_cast<const VkClearColorValue*>(&pColor), rangeCount, reinterpret_cast<const VkImageSubresourceRange*>(&pRanges));
}
inline void cmdClearColorImage(CommandBuffer commandBuffer, Image image, ImageLayout imageLayout, const ClearColorValue& pColor, const Range<const ImageSubresourceRange>& pRanges)
{
	vkCmdClearColorImage(reinterpret_cast<VkCommandBuffer>(commandBuffer), reinterpret_cast<VkImage>(image), static_cast<VkImageLayout>(imageLayout), reinterpret_cast<const VkClearColorValue*>(&pColor), pRanges.size(), reinterpret_cast<const VkImageSubresourceRange*>(pRanges.data()));
}
inline void cmdClearDepthStencilImage(CommandBuffer commandBuffer, Image image, ImageLayout imageLayout, const ClearDepthStencilValue& pDepthStencil, uint32_t rangeCount, const ImageSubresourceRange& pRanges)
{
	return vkCmdClearDepthStencilImage(reinterpret_cast<VkCommandBuffer>(commandBuffer), reinterpret_cast<VkImage>(image), static_cast<VkImageLayout>(imageLayout), reinterpret_cast<const VkClearDepthStencilValue*>(&pDepthStencil), rangeCount, reinterpret_cast<const VkImageSubresourceRange*>(&pRanges));
}
inline void cmdClearDepthStencilImage(CommandBuffer commandBuffer, Image image, ImageLayout imageLayout, const ClearDepthStencilValue& pDepthStencil, const Range<const ImageSubresourceRange>& pRanges)
{
	vkCmdClearDepthStencilImage(reinterpret_cast<VkCommandBuffer>(commandBuffer), reinterpret_cast<VkImage>(image), static_cast<VkImageLayout>(imageLayout), reinterpret_cast<const VkClearDepthStencilValue*>(&pDepthStencil), pRanges.size(), reinterpret_cast<const VkImageSubresourceRange*>(pRanges.data()));
}
inline void cmdClearAttachments(CommandBuffer commandBuffer, uint32_t attachmentCount, const ClearAttachment& pAttachments, uint32_t rectCount, const ClearRect& pRects)
{
	return vkCmdClearAttachments(reinterpret_cast<VkCommandBuffer>(commandBuffer), attachmentCount, reinterpret_cast<const VkClearAttachment*>(&pAttachments), rectCount, reinterpret_cast<const VkClearRect*>(&pRects));
}
inline void cmdClearAttachments(CommandBuffer commandBuffer, const Range<const ClearAttachment>& pAttachments, const Range<const ClearRect>& pRects)
{
	vkCmdClearAttachments(reinterpret_cast<VkCommandBuffer>(commandBuffer), pAttachments.size(), reinterpret_cast<const VkClearAttachment*>(pAttachments.data()), pRects.size(), reinterpret_cast<const VkClearRect*>(pRects.data()));
}
inline void cmdResolveImage(CommandBuffer commandBuffer, Image srcImage, ImageLayout srcImageLayout, Image dstImage, ImageLayout dstImageLayout, uint32_t regionCount, const ImageResolve& pRegions)
{
	return vkCmdResolveImage(reinterpret_cast<VkCommandBuffer>(commandBuffer), reinterpret_cast<VkImage>(srcImage), static_cast<VkImageLayout>(srcImageLayout), reinterpret_cast<VkImage>(dstImage), static_cast<VkImageLayout>(dstImageLayout), regionCount, reinterpret_cast<const VkImageResolve*>(&pRegions));
}
inline void cmdResolveImage(CommandBuffer commandBuffer, Image srcImage, ImageLayout srcImageLayout, Image dstImage, ImageLayout dstImageLayout, const Range<const ImageResolve>& pRegions)
{
	vkCmdResolveImage(reinterpret_cast<VkCommandBuffer>(commandBuffer), reinterpret_cast<VkImage>(srcImage), static_cast<VkImageLayout>(srcImageLayout), reinterpret_cast<VkImage>(dstImage), static_cast<VkImageLayout>(dstImageLayout), pRegions.size(), reinterpret_cast<const VkImageResolve*>(pRegions.data()));
}
inline void cmdSetEvent(CommandBuffer commandBuffer, Event event, PipelineStageFlags stageMask)
{
	return vkCmdSetEvent(reinterpret_cast<VkCommandBuffer>(commandBuffer), reinterpret_cast<VkEvent>(event), static_cast<VkPipelineStageFlags>(stageMask));
}
inline void cmdResetEvent(CommandBuffer commandBuffer, Event event, PipelineStageFlags stageMask)
{
	return vkCmdResetEvent(reinterpret_cast<VkCommandBuffer>(commandBuffer), reinterpret_cast<VkEvent>(event), static_cast<VkPipelineStageFlags>(stageMask));
}
inline void cmdWaitEvents(CommandBuffer commandBuffer, uint32_t eventCount, const Event& pEvents, PipelineStageFlags srcStageMask, PipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, const MemoryBarrier& pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const BufferMemoryBarrier& pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const ImageMemoryBarrier& pImageMemoryBarriers)
{
	return vkCmdWaitEvents(reinterpret_cast<VkCommandBuffer>(commandBuffer), eventCount, reinterpret_cast<const VkEvent*>(&pEvents), static_cast<VkPipelineStageFlags>(srcStageMask), static_cast<VkPipelineStageFlags>(dstStageMask), memoryBarrierCount, reinterpret_cast<const VkMemoryBarrier*>(&pMemoryBarriers), bufferMemoryBarrierCount, reinterpret_cast<const VkBufferMemoryBarrier*>(&pBufferMemoryBarriers), imageMemoryBarrierCount, reinterpret_cast<const VkImageMemoryBarrier*>(&pImageMemoryBarriers));
}
inline void cmdWaitEvents(CommandBuffer commandBuffer, const Range<const Event>& pEvents, PipelineStageFlags srcStageMask, PipelineStageFlags dstStageMask, const Range<const MemoryBarrier>& pMemoryBarriers, const Range<const BufferMemoryBarrier>& pBufferMemoryBarriers, const Range<const ImageMemoryBarrier>& pImageMemoryBarriers)
{
	vkCmdWaitEvents(reinterpret_cast<VkCommandBuffer>(commandBuffer), pEvents.size(), reinterpret_cast<const VkEvent*>(pEvents.data()), static_cast<VkPipelineStageFlags>(srcStageMask), static_cast<VkPipelineStageFlags>(dstStageMask), pMemoryBarriers.size(), reinterpret_cast<const VkMemoryBarrier*>(pMemoryBarriers.data()), pBufferMemoryBarriers.size(), reinterpret_cast<const VkBufferMemoryBarrier*>(pBufferMemoryBarriers.data()), pImageMemoryBarriers.size(), reinterpret_cast<const VkImageMemoryBarrier*>(pImageMemoryBarriers.data()));
}
inline void cmdPipelineBarrier(CommandBuffer commandBuffer, PipelineStageFlags srcStageMask, PipelineStageFlags dstStageMask, DependencyFlags dependencyFlags, uint32_t memoryBarrierCount, const MemoryBarrier& pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const BufferMemoryBarrier& pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const ImageMemoryBarrier& pImageMemoryBarriers)
{
	return vkCmdPipelineBarrier(reinterpret_cast<VkCommandBuffer>(commandBuffer), static_cast<VkPipelineStageFlags>(srcStageMask), static_cast<VkPipelineStageFlags>(dstStageMask), static_cast<VkDependencyFlags>(dependencyFlags), memoryBarrierCount, reinterpret_cast<const VkMemoryBarrier*>(&pMemoryBarriers), bufferMemoryBarrierCount, reinterpret_cast<const VkBufferMemoryBarrier*>(&pBufferMemoryBarriers), imageMemoryBarrierCount, reinterpret_cast<const VkImageMemoryBarrier*>(&pImageMemoryBarriers));
}
inline void cmdPipelineBarrier(CommandBuffer commandBuffer, PipelineStageFlags srcStageMask, PipelineStageFlags dstStageMask, DependencyFlags dependencyFlags, const Range<const MemoryBarrier>& pMemoryBarriers, const Range<const BufferMemoryBarrier>& pBufferMemoryBarriers, const Range<const ImageMemoryBarrier>& pImageMemoryBarriers)
{
	vkCmdPipelineBarrier(reinterpret_cast<VkCommandBuffer>(commandBuffer), static_cast<VkPipelineStageFlags>(srcStageMask), static_cast<VkPipelineStageFlags>(dstStageMask), static_cast<VkDependencyFlags>(dependencyFlags), pMemoryBarriers.size(), reinterpret_cast<const VkMemoryBarrier*>(pMemoryBarriers.data()), pBufferMemoryBarriers.size(), reinterpret_cast<const VkBufferMemoryBarrier*>(pBufferMemoryBarriers.data()), pImageMemoryBarriers.size(), reinterpret_cast<const VkImageMemoryBarrier*>(pImageMemoryBarriers.data()));
}
inline void cmdBeginQuery(CommandBuffer commandBuffer, QueryPool queryPool, uint32_t query, QueryControlFlags flags = {})
{
	return vkCmdBeginQuery(reinterpret_cast<VkCommandBuffer>(commandBuffer), reinterpret_cast<VkQueryPool>(queryPool), query, static_cast<VkQueryControlFlags>(flags));
}
inline void cmdEndQuery(CommandBuffer commandBuffer, QueryPool queryPool, uint32_t query)
{
	return vkCmdEndQuery(reinterpret_cast<VkCommandBuffer>(commandBuffer), reinterpret_cast<VkQueryPool>(queryPool), query);
}
inline void cmdResetQueryPool(CommandBuffer commandBuffer, QueryPool queryPool, uint32_t firstQuery, uint32_t queryCount)
{
	return vkCmdResetQueryPool(reinterpret_cast<VkCommandBuffer>(commandBuffer), reinterpret_cast<VkQueryPool>(queryPool), firstQuery, queryCount);
}
inline void cmdWriteTimestamp(CommandBuffer commandBuffer, PipelineStageBits pipelineStage, QueryPool queryPool, uint32_t query)
{
	return vkCmdWriteTimestamp(reinterpret_cast<VkCommandBuffer>(commandBuffer), static_cast<VkPipelineStageFlagBits>(pipelineStage), reinterpret_cast<VkQueryPool>(queryPool), query);
}
inline void cmdCopyQueryPoolResults(CommandBuffer commandBuffer, QueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, Buffer dstBuffer, DeviceSize dstOffset, DeviceSize stride, QueryResultFlags flags = {})
{
	return vkCmdCopyQueryPoolResults(reinterpret_cast<VkCommandBuffer>(commandBuffer), reinterpret_cast<VkQueryPool>(queryPool), firstQuery, queryCount, reinterpret_cast<VkBuffer>(dstBuffer), dstOffset, stride, static_cast<VkQueryResultFlags>(flags));
}
inline void cmdPushConstants(CommandBuffer commandBuffer, PipelineLayout layout, ShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void* pValues)
{
	return vkCmdPushConstants(reinterpret_cast<VkCommandBuffer>(commandBuffer), reinterpret_cast<VkPipelineLayout>(layout), static_cast<VkShaderStageFlags>(stageFlags), offset, size, reinterpret_cast<const void*>(pValues));
}
inline void cmdPushConstants(CommandBuffer commandBuffer, PipelineLayout layout, ShaderStageFlags stageFlags, uint32_t offset, const Range<const uint8_t>& pValues)
{
	vkCmdPushConstants(reinterpret_cast<VkCommandBuffer>(commandBuffer), reinterpret_cast<VkPipelineLayout>(layout), static_cast<VkShaderStageFlags>(stageFlags), offset, pValues.size(), reinterpret_cast<const void*>(pValues.data()));
}
inline void cmdBeginRenderPass(CommandBuffer commandBuffer, const RenderPassBeginInfo& pRenderPassBegin, SubpassContents contents)
{
	return vkCmdBeginRenderPass(reinterpret_cast<VkCommandBuffer>(commandBuffer), reinterpret_cast<const VkRenderPassBeginInfo*>(&pRenderPassBegin), static_cast<VkSubpassContents>(contents));
}
inline void cmdNextSubpass(CommandBuffer commandBuffer, SubpassContents contents)
{
	return vkCmdNextSubpass(reinterpret_cast<VkCommandBuffer>(commandBuffer), static_cast<VkSubpassContents>(contents));
}
inline void cmdEndRenderPass(CommandBuffer commandBuffer)
{
	return vkCmdEndRenderPass(reinterpret_cast<VkCommandBuffer>(commandBuffer));
}
inline void cmdExecuteCommands(CommandBuffer commandBuffer, uint32_t commandBufferCount, const CommandBuffer& pCommandBuffers)
{
	return vkCmdExecuteCommands(reinterpret_cast<VkCommandBuffer>(commandBuffer), commandBufferCount, reinterpret_cast<const VkCommandBuffer*>(&pCommandBuffers));
}
inline void cmdExecuteCommands(CommandBuffer commandBuffer, const Range<const CommandBuffer>& pCommandBuffers)
{
	vkCmdExecuteCommands(reinterpret_cast<VkCommandBuffer>(commandBuffer), pCommandBuffers.size(), reinterpret_cast<const VkCommandBuffer*>(pCommandBuffers.data()));
}



} //namespace vk