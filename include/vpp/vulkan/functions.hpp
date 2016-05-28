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

namespace vk
{

Result createInstance(const InstanceCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, Instance* pInstance)
{
	return VPP_CALL(static_cast<Result>(vkCreateInstance(reinterpret_cast<const VkInstanceCreateInfo*>(pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkInstance*>(pInstance))));
}
void destroyInstance(Instance instance, const AllocationCallbacks* pAllocator)
{
	vkDestroyInstance(instance, reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
Result enumeratePhysicalDevices(Instance instance, uint32_t* pPhysicalDeviceCount, PhysicalDevice* pPhysicalDevices)
{
	return VPP_CALL(static_cast<Result>(vkEnumeratePhysicalDevices(instance, reinterpret_cast<uint32_t*>(pPhysicalDeviceCount), reinterpret_cast<VkPhysicalDevice*>(pPhysicalDevices))));
}
void getPhysicalDeviceFeatures(PhysicalDevice physicalDevice, PhysicalDeviceFeatures* pFeatures)
{
	vkGetPhysicalDeviceFeatures(physicalDevice, reinterpret_cast<VkPhysicalDeviceFeatures*>(pFeatures));
}
void getPhysicalDeviceFormatProperties(PhysicalDevice physicalDevice, Format format, FormatProperties* pFormatProperties)
{
	vkGetPhysicalDeviceFormatProperties(physicalDevice, static_cast<VkFormat>(format), reinterpret_cast<VkFormatProperties*>(pFormatProperties));
}
Result getPhysicalDeviceImageFormatProperties(PhysicalDevice physicalDevice, Format format, ImageType type, ImageTiling tiling, ImageUsageFlags usage, ImageCreateFlags flags, ImageFormatProperties* pImageFormatProperties)
{
	return VPP_CALL(static_cast<Result>(vkGetPhysicalDeviceImageFormatProperties(physicalDevice, static_cast<VkFormat>(format), static_cast<VkImageType>(type), static_cast<VkImageTiling>(tiling), static_cast<VkImageUsageFlags>(usage), static_cast<VkImageCreateFlags>(flags), reinterpret_cast<VkImageFormatProperties*>(pImageFormatProperties))));
}
void getPhysicalDeviceProperties(PhysicalDevice physicalDevice, PhysicalDeviceProperties* pProperties)
{
	vkGetPhysicalDeviceProperties(physicalDevice, reinterpret_cast<VkPhysicalDeviceProperties*>(pProperties));
}
void getPhysicalDeviceQueueFamilyProperties(PhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, QueueFamilyProperties* pQueueFamilyProperties)
{
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, reinterpret_cast<uint32_t*>(pQueueFamilyPropertyCount), reinterpret_cast<VkQueueFamilyProperties*>(pQueueFamilyProperties));
}
void getPhysicalDeviceMemoryProperties(PhysicalDevice physicalDevice, PhysicalDeviceMemoryProperties* pMemoryProperties)
{
	vkGetPhysicalDeviceMemoryProperties(physicalDevice, reinterpret_cast<VkPhysicalDeviceMemoryProperties*>(pMemoryProperties));
}
PfnVoidFunction getInstanceProcAddr(Instance instance, const char* pName)
{
	return static_cast<PfnVoidFunction>(vkGetInstanceProcAddr(instance, reinterpret_cast<const char*>(pName)));
}
PfnVoidFunction getDeviceProcAddr(Device device, const char* pName)
{
	return static_cast<PfnVoidFunction>(vkGetDeviceProcAddr(device, reinterpret_cast<const char*>(pName)));
}
Result createDevice(PhysicalDevice physicalDevice, const DeviceCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, Device* pDevice)
{
	return VPP_CALL(static_cast<Result>(vkCreateDevice(physicalDevice, reinterpret_cast<const VkDeviceCreateInfo*>(pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkDevice*>(pDevice))));
}
void destroyDevice(Device device, const AllocationCallbacks* pAllocator)
{
	vkDestroyDevice(device, reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
Result enumerateInstanceExtensionProperties(const char* pLayerName, uint32_t* pPropertyCount, ExtensionProperties* pProperties)
{
	return VPP_CALL(static_cast<Result>(vkEnumerateInstanceExtensionProperties(reinterpret_cast<const char*>(pLayerName), reinterpret_cast<uint32_t*>(pPropertyCount), reinterpret_cast<VkExtensionProperties*>(pProperties))));
}
Result enumerateDeviceExtensionProperties(PhysicalDevice physicalDevice, const char* pLayerName, uint32_t* pPropertyCount, ExtensionProperties* pProperties)
{
	return VPP_CALL(static_cast<Result>(vkEnumerateDeviceExtensionProperties(physicalDevice, reinterpret_cast<const char*>(pLayerName), reinterpret_cast<uint32_t*>(pPropertyCount), reinterpret_cast<VkExtensionProperties*>(pProperties))));
}
Result enumerateInstanceLayerProperties(uint32_t* pPropertyCount, LayerProperties* pProperties)
{
	return VPP_CALL(static_cast<Result>(vkEnumerateInstanceLayerProperties(reinterpret_cast<uint32_t*>(pPropertyCount), reinterpret_cast<VkLayerProperties*>(pProperties))));
}
Result enumerateDeviceLayerProperties(PhysicalDevice physicalDevice, uint32_t* pPropertyCount, LayerProperties* pProperties)
{
	return VPP_CALL(static_cast<Result>(vkEnumerateDeviceLayerProperties(physicalDevice, reinterpret_cast<uint32_t*>(pPropertyCount), reinterpret_cast<VkLayerProperties*>(pProperties))));
}
void getDeviceQueue(Device device, uint32_t queueFamilyIndex, uint32_t queueIndex, Queue* pQueue)
{
	vkGetDeviceQueue(device, queueFamilyIndex, queueIndex, reinterpret_cast<VkQueue*>(pQueue));
}
Result queueSubmit(Queue queue, uint32_t submitCount, const SubmitInfo* pSubmits, Fence fence)
{
	return VPP_CALL(static_cast<Result>(vkQueueSubmit(queue, submitCount, reinterpret_cast<const VkSubmitInfo*>(pSubmits), fence)));
}
Result queueWaitIdle(Queue queue)
{
	return VPP_CALL(static_cast<Result>(vkQueueWaitIdle(queue)));
}
Result deviceWaitIdle(Device device)
{
	return VPP_CALL(static_cast<Result>(vkDeviceWaitIdle(device)));
}
Result allocateMemory(Device device, const MemoryAllocateInfo* pAllocateInfo, const AllocationCallbacks* pAllocator, DeviceMemory* pMemory)
{
	return VPP_CALL(static_cast<Result>(vkAllocateMemory(device, reinterpret_cast<const VkMemoryAllocateInfo*>(pAllocateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkDeviceMemory*>(pMemory))));
}
void freeMemory(Device device, DeviceMemory memory, const AllocationCallbacks* pAllocator)
{
	vkFreeMemory(device, memory, reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
Result mapMemory(Device device, DeviceMemory memory, DeviceSize offset, DeviceSize size, MemoryMapFlags flags, void** ppData)
{
	return VPP_CALL(static_cast<Result>(vkMapMemory(device, memory, offset, size, static_cast<VkMemoryMapFlags>(flags), reinterpret_cast<void**>(ppData))));
}
void unmapMemory(Device device, DeviceMemory memory)
{
	vkUnmapMemory(device, memory);
}
Result flushMappedMemoryRanges(Device device, uint32_t memoryRangeCount, const MappedMemoryRange* pMemoryRanges)
{
	return VPP_CALL(static_cast<Result>(vkFlushMappedMemoryRanges(device, memoryRangeCount, reinterpret_cast<const VkMappedMemoryRange*>(pMemoryRanges))));
}
Result invalidateMappedMemoryRanges(Device device, uint32_t memoryRangeCount, const MappedMemoryRange* pMemoryRanges)
{
	return VPP_CALL(static_cast<Result>(vkInvalidateMappedMemoryRanges(device, memoryRangeCount, reinterpret_cast<const VkMappedMemoryRange*>(pMemoryRanges))));
}
void getDeviceMemoryCommitment(Device device, DeviceMemory memory, DeviceSize* pCommittedMemoryInBytes)
{
	vkGetDeviceMemoryCommitment(device, memory, reinterpret_cast<VkDeviceSize*>(pCommittedMemoryInBytes));
}
Result bindBufferMemory(Device device, Buffer buffer, DeviceMemory memory, DeviceSize memoryOffset)
{
	return VPP_CALL(static_cast<Result>(vkBindBufferMemory(device, buffer, memory, memoryOffset)));
}
Result bindImageMemory(Device device, Image image, DeviceMemory memory, DeviceSize memoryOffset)
{
	return VPP_CALL(static_cast<Result>(vkBindImageMemory(device, image, memory, memoryOffset)));
}
void getBufferMemoryRequirements(Device device, Buffer buffer, MemoryRequirements* pMemoryRequirements)
{
	vkGetBufferMemoryRequirements(device, buffer, reinterpret_cast<VkMemoryRequirements*>(pMemoryRequirements));
}
void getImageMemoryRequirements(Device device, Image image, MemoryRequirements* pMemoryRequirements)
{
	vkGetImageMemoryRequirements(device, image, reinterpret_cast<VkMemoryRequirements*>(pMemoryRequirements));
}
void getImageSparseMemoryRequirements(Device device, Image image, uint32_t* pSparseMemoryRequirementCount, SparseImageMemoryRequirements* pSparseMemoryRequirements)
{
	vkGetImageSparseMemoryRequirements(device, image, reinterpret_cast<uint32_t*>(pSparseMemoryRequirementCount), reinterpret_cast<VkSparseImageMemoryRequirements*>(pSparseMemoryRequirements));
}
void getPhysicalDeviceSparseImageFormatProperties(PhysicalDevice physicalDevice, Format format, ImageType type, SampleCountBits samples, ImageUsageFlags usage, ImageTiling tiling, uint32_t* pPropertyCount, SparseImageFormatProperties* pProperties)
{
	vkGetPhysicalDeviceSparseImageFormatProperties(physicalDevice, static_cast<VkFormat>(format), static_cast<VkImageType>(type), static_cast<VkSampleCountFlagBits>(samples), static_cast<VkImageUsageFlags>(usage), static_cast<VkImageTiling>(tiling), reinterpret_cast<uint32_t*>(pPropertyCount), reinterpret_cast<VkSparseImageFormatProperties*>(pProperties));
}
Result queueBindSparse(Queue queue, uint32_t bindInfoCount, const BindSparseInfo* pBindInfo, Fence fence)
{
	return VPP_CALL(static_cast<Result>(vkQueueBindSparse(queue, bindInfoCount, reinterpret_cast<const VkBindSparseInfo*>(pBindInfo), fence)));
}
Result createFence(Device device, const FenceCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, Fence* pFence)
{
	return VPP_CALL(static_cast<Result>(vkCreateFence(device, reinterpret_cast<const VkFenceCreateInfo*>(pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkFence*>(pFence))));
}
void destroyFence(Device device, Fence fence, const AllocationCallbacks* pAllocator)
{
	vkDestroyFence(device, fence, reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
Result resetFences(Device device, uint32_t fenceCount, const Fence* pFences)
{
	return VPP_CALL(static_cast<Result>(vkResetFences(device, fenceCount, reinterpret_cast<const VkFence*>(pFences))));
}
Result getFenceStatus(Device device, Fence fence)
{
	return VPP_CALL(static_cast<Result>(vkGetFenceStatus(device, fence)));
}
Result waitForFences(Device device, uint32_t fenceCount, const Fence* pFences, Bool32 waitAll, uint64_t timeout)
{
	return VPP_CALL(static_cast<Result>(vkWaitForFences(device, fenceCount, reinterpret_cast<const VkFence*>(pFences), waitAll, timeout)));
}
Result createSemaphore(Device device, const SemaphoreCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, Semaphore* pSemaphore)
{
	return VPP_CALL(static_cast<Result>(vkCreateSemaphore(device, reinterpret_cast<const VkSemaphoreCreateInfo*>(pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkSemaphore*>(pSemaphore))));
}
void destroySemaphore(Device device, Semaphore semaphore, const AllocationCallbacks* pAllocator)
{
	vkDestroySemaphore(device, semaphore, reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
Result createEvent(Device device, const EventCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, Event* pEvent)
{
	return VPP_CALL(static_cast<Result>(vkCreateEvent(device, reinterpret_cast<const VkEventCreateInfo*>(pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkEvent*>(pEvent))));
}
void destroyEvent(Device device, Event event, const AllocationCallbacks* pAllocator)
{
	vkDestroyEvent(device, event, reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
Result getEventStatus(Device device, Event event)
{
	return VPP_CALL(static_cast<Result>(vkGetEventStatus(device, event)));
}
Result setEvent(Device device, Event event)
{
	return VPP_CALL(static_cast<Result>(vkSetEvent(device, event)));
}
Result resetEvent(Device device, Event event)
{
	return VPP_CALL(static_cast<Result>(vkResetEvent(device, event)));
}
Result createQueryPool(Device device, const QueryPoolCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, QueryPool* pQueryPool)
{
	return VPP_CALL(static_cast<Result>(vkCreateQueryPool(device, reinterpret_cast<const VkQueryPoolCreateInfo*>(pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkQueryPool*>(pQueryPool))));
}
void destroyQueryPool(Device device, QueryPool queryPool, const AllocationCallbacks* pAllocator)
{
	vkDestroyQueryPool(device, queryPool, reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
Result getQueryPoolResults(Device device, QueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, size_t dataSize, void* pData, DeviceSize stride, QueryResultFlags flags)
{
	return VPP_CALL(static_cast<Result>(vkGetQueryPoolResults(device, queryPool, firstQuery, queryCount, dataSize, reinterpret_cast<void*>(pData), stride, static_cast<VkQueryResultFlags>(flags))));
}
Result createBuffer(Device device, const BufferCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, Buffer* pBuffer)
{
	return VPP_CALL(static_cast<Result>(vkCreateBuffer(device, reinterpret_cast<const VkBufferCreateInfo*>(pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkBuffer*>(pBuffer))));
}
void destroyBuffer(Device device, Buffer buffer, const AllocationCallbacks* pAllocator)
{
	vkDestroyBuffer(device, buffer, reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
Result createBufferView(Device device, const BufferViewCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, BufferView* pView)
{
	return VPP_CALL(static_cast<Result>(vkCreateBufferView(device, reinterpret_cast<const VkBufferViewCreateInfo*>(pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkBufferView*>(pView))));
}
void destroyBufferView(Device device, BufferView bufferView, const AllocationCallbacks* pAllocator)
{
	vkDestroyBufferView(device, bufferView, reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
Result createImage(Device device, const ImageCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, Image* pImage)
{
	return VPP_CALL(static_cast<Result>(vkCreateImage(device, reinterpret_cast<const VkImageCreateInfo*>(pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkImage*>(pImage))));
}
void destroyImage(Device device, Image image, const AllocationCallbacks* pAllocator)
{
	vkDestroyImage(device, image, reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
void getImageSubresourceLayout(Device device, Image image, const ImageSubresource* pSubresource, SubresourceLayout* pLayout)
{
	vkGetImageSubresourceLayout(device, image, reinterpret_cast<const VkImageSubresource*>(pSubresource), reinterpret_cast<VkSubresourceLayout*>(pLayout));
}
Result createImageView(Device device, const ImageViewCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, ImageView* pView)
{
	return VPP_CALL(static_cast<Result>(vkCreateImageView(device, reinterpret_cast<const VkImageViewCreateInfo*>(pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkImageView*>(pView))));
}
void destroyImageView(Device device, ImageView imageView, const AllocationCallbacks* pAllocator)
{
	vkDestroyImageView(device, imageView, reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
Result createShaderModule(Device device, const ShaderModuleCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, ShaderModule* pShaderModule)
{
	return VPP_CALL(static_cast<Result>(vkCreateShaderModule(device, reinterpret_cast<const VkShaderModuleCreateInfo*>(pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkShaderModule*>(pShaderModule))));
}
void destroyShaderModule(Device device, ShaderModule shaderModule, const AllocationCallbacks* pAllocator)
{
	vkDestroyShaderModule(device, shaderModule, reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
Result createPipelineCache(Device device, const PipelineCacheCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, PipelineCache* pPipelineCache)
{
	return VPP_CALL(static_cast<Result>(vkCreatePipelineCache(device, reinterpret_cast<const VkPipelineCacheCreateInfo*>(pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkPipelineCache*>(pPipelineCache))));
}
void destroyPipelineCache(Device device, PipelineCache pipelineCache, const AllocationCallbacks* pAllocator)
{
	vkDestroyPipelineCache(device, pipelineCache, reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
Result getPipelineCacheData(Device device, PipelineCache pipelineCache, size_t* pDataSize, void* pData)
{
	return VPP_CALL(static_cast<Result>(vkGetPipelineCacheData(device, pipelineCache, reinterpret_cast<size_t*>(pDataSize), reinterpret_cast<void*>(pData))));
}
Result mergePipelineCaches(Device device, PipelineCache dstCache, uint32_t srcCacheCount, const PipelineCache* pSrcCaches)
{
	return VPP_CALL(static_cast<Result>(vkMergePipelineCaches(device, dstCache, srcCacheCount, reinterpret_cast<const VkPipelineCache*>(pSrcCaches))));
}
Result createGraphicsPipelines(Device device, PipelineCache pipelineCache, uint32_t createInfoCount, const GraphicsPipelineCreateInfo* pCreateInfos, const AllocationCallbacks* pAllocator, Pipeline* pPipelines)
{
	return VPP_CALL(static_cast<Result>(vkCreateGraphicsPipelines(device, pipelineCache, createInfoCount, reinterpret_cast<const VkGraphicsPipelineCreateInfo*>(pCreateInfos), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkPipeline*>(pPipelines))));
}
Result createComputePipelines(Device device, PipelineCache pipelineCache, uint32_t createInfoCount, const ComputePipelineCreateInfo* pCreateInfos, const AllocationCallbacks* pAllocator, Pipeline* pPipelines)
{
	return VPP_CALL(static_cast<Result>(vkCreateComputePipelines(device, pipelineCache, createInfoCount, reinterpret_cast<const VkComputePipelineCreateInfo*>(pCreateInfos), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkPipeline*>(pPipelines))));
}
void destroyPipeline(Device device, Pipeline pipeline, const AllocationCallbacks* pAllocator)
{
	vkDestroyPipeline(device, pipeline, reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
Result createPipelineLayout(Device device, const PipelineLayoutCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, PipelineLayout* pPipelineLayout)
{
	return VPP_CALL(static_cast<Result>(vkCreatePipelineLayout(device, reinterpret_cast<const VkPipelineLayoutCreateInfo*>(pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkPipelineLayout*>(pPipelineLayout))));
}
void destroyPipelineLayout(Device device, PipelineLayout pipelineLayout, const AllocationCallbacks* pAllocator)
{
	vkDestroyPipelineLayout(device, pipelineLayout, reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
Result createSampler(Device device, const SamplerCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, Sampler* pSampler)
{
	return VPP_CALL(static_cast<Result>(vkCreateSampler(device, reinterpret_cast<const VkSamplerCreateInfo*>(pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkSampler*>(pSampler))));
}
void destroySampler(Device device, Sampler sampler, const AllocationCallbacks* pAllocator)
{
	vkDestroySampler(device, sampler, reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
Result createDescriptorSetLayout(Device device, const DescriptorSetLayoutCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, DescriptorSetLayout* pSetLayout)
{
	return VPP_CALL(static_cast<Result>(vkCreateDescriptorSetLayout(device, reinterpret_cast<const VkDescriptorSetLayoutCreateInfo*>(pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkDescriptorSetLayout*>(pSetLayout))));
}
void destroyDescriptorSetLayout(Device device, DescriptorSetLayout descriptorSetLayout, const AllocationCallbacks* pAllocator)
{
	vkDestroyDescriptorSetLayout(device, descriptorSetLayout, reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
Result createDescriptorPool(Device device, const DescriptorPoolCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, DescriptorPool* pDescriptorPool)
{
	return VPP_CALL(static_cast<Result>(vkCreateDescriptorPool(device, reinterpret_cast<const VkDescriptorPoolCreateInfo*>(pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkDescriptorPool*>(pDescriptorPool))));
}
void destroyDescriptorPool(Device device, DescriptorPool descriptorPool, const AllocationCallbacks* pAllocator)
{
	vkDestroyDescriptorPool(device, descriptorPool, reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
Result resetDescriptorPool(Device device, DescriptorPool descriptorPool, DescriptorPoolResetFlags flags)
{
	return VPP_CALL(static_cast<Result>(vkResetDescriptorPool(device, descriptorPool, static_cast<VkDescriptorPoolResetFlags>(flags))));
}
Result allocateDescriptorSets(Device device, const DescriptorSetAllocateInfo* pAllocateInfo, DescriptorSet* pDescriptorSets)
{
	return VPP_CALL(static_cast<Result>(vkAllocateDescriptorSets(device, reinterpret_cast<const VkDescriptorSetAllocateInfo*>(pAllocateInfo), reinterpret_cast<VkDescriptorSet*>(pDescriptorSets))));
}
Result freeDescriptorSets(Device device, DescriptorPool descriptorPool, uint32_t descriptorSetCount, const DescriptorSet* pDescriptorSets)
{
	return VPP_CALL(static_cast<Result>(vkFreeDescriptorSets(device, descriptorPool, descriptorSetCount, reinterpret_cast<const VkDescriptorSet*>(pDescriptorSets))));
}
void updateDescriptorSets(Device device, uint32_t descriptorWriteCount, const WriteDescriptorSet* pDescriptorWrites, uint32_t descriptorCopyCount, const CopyDescriptorSet* pDescriptorCopies)
{
	vkUpdateDescriptorSets(device, descriptorWriteCount, reinterpret_cast<const VkWriteDescriptorSet*>(pDescriptorWrites), descriptorCopyCount, reinterpret_cast<const VkCopyDescriptorSet*>(pDescriptorCopies));
}
Result createFramebuffer(Device device, const FramebufferCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, Framebuffer* pFramebuffer)
{
	return VPP_CALL(static_cast<Result>(vkCreateFramebuffer(device, reinterpret_cast<const VkFramebufferCreateInfo*>(pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkFramebuffer*>(pFramebuffer))));
}
void destroyFramebuffer(Device device, Framebuffer framebuffer, const AllocationCallbacks* pAllocator)
{
	vkDestroyFramebuffer(device, framebuffer, reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
Result createRenderPass(Device device, const RenderPassCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, RenderPass* pRenderPass)
{
	return VPP_CALL(static_cast<Result>(vkCreateRenderPass(device, reinterpret_cast<const VkRenderPassCreateInfo*>(pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkRenderPass*>(pRenderPass))));
}
void destroyRenderPass(Device device, RenderPass renderPass, const AllocationCallbacks* pAllocator)
{
	vkDestroyRenderPass(device, renderPass, reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
void getRenderAreaGranularity(Device device, RenderPass renderPass, Extent2D* pGranularity)
{
	vkGetRenderAreaGranularity(device, renderPass, reinterpret_cast<VkExtent2D*>(pGranularity));
}
Result createCommandPool(Device device, const CommandPoolCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, CommandPool* pCommandPool)
{
	return VPP_CALL(static_cast<Result>(vkCreateCommandPool(device, reinterpret_cast<const VkCommandPoolCreateInfo*>(pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkCommandPool*>(pCommandPool))));
}
void destroyCommandPool(Device device, CommandPool commandPool, const AllocationCallbacks* pAllocator)
{
	vkDestroyCommandPool(device, commandPool, reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
Result resetCommandPool(Device device, CommandPool commandPool, CommandPoolResetFlags flags)
{
	return VPP_CALL(static_cast<Result>(vkResetCommandPool(device, commandPool, static_cast<VkCommandPoolResetFlags>(flags))));
}
Result allocateCommandBuffers(Device device, const CommandBufferAllocateInfo* pAllocateInfo, CommandBuffer* pCommandBuffers)
{
	return VPP_CALL(static_cast<Result>(vkAllocateCommandBuffers(device, reinterpret_cast<const VkCommandBufferAllocateInfo*>(pAllocateInfo), reinterpret_cast<VkCommandBuffer*>(pCommandBuffers))));
}
void freeCommandBuffers(Device device, CommandPool commandPool, uint32_t commandBufferCount, const CommandBuffer* pCommandBuffers)
{
	vkFreeCommandBuffers(device, commandPool, commandBufferCount, reinterpret_cast<const VkCommandBuffer*>(pCommandBuffers));
}
Result beginCommandBuffer(CommandBuffer commandBuffer, const CommandBufferBeginInfo* pBeginInfo)
{
	return VPP_CALL(static_cast<Result>(vkBeginCommandBuffer(commandBuffer, reinterpret_cast<const VkCommandBufferBeginInfo*>(pBeginInfo))));
}
Result endCommandBuffer(CommandBuffer commandBuffer)
{
	return VPP_CALL(static_cast<Result>(vkEndCommandBuffer(commandBuffer)));
}
Result resetCommandBuffer(CommandBuffer commandBuffer, CommandBufferResetFlags flags)
{
	return VPP_CALL(static_cast<Result>(vkResetCommandBuffer(commandBuffer, static_cast<VkCommandBufferResetFlags>(flags))));
}
void cmdBindPipeline(CommandBuffer commandBuffer, PipelineBindPoint pipelineBindPoint, Pipeline pipeline)
{
	vkCmdBindPipeline(commandBuffer, static_cast<VkPipelineBindPoint>(pipelineBindPoint), pipeline);
}
void cmdSetViewport(CommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const Viewport* pViewports)
{
	vkCmdSetViewport(commandBuffer, firstViewport, viewportCount, reinterpret_cast<const VkViewport*>(pViewports));
}
void cmdSetScissor(CommandBuffer commandBuffer, uint32_t firstScissor, uint32_t scissorCount, const Rect2D* pScissors)
{
	vkCmdSetScissor(commandBuffer, firstScissor, scissorCount, reinterpret_cast<const VkRect2D*>(pScissors));
}
void cmdSetLineWidth(CommandBuffer commandBuffer, float lineWidth)
{
	vkCmdSetLineWidth(commandBuffer, lineWidth);
}
void cmdSetDepthBias(CommandBuffer commandBuffer, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor)
{
	vkCmdSetDepthBias(commandBuffer, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor);
}
void cmdSetBlendConstants(CommandBuffer commandBuffer, const float blendConstants[4])
{
	vkCmdSetBlendConstants(commandBuffer, blendConstants);
}
void cmdSetDepthBounds(CommandBuffer commandBuffer, float minDepthBounds, float maxDepthBounds)
{
	vkCmdSetDepthBounds(commandBuffer, minDepthBounds, maxDepthBounds);
}
void cmdSetStencilCompareMask(CommandBuffer commandBuffer, StencilFaceFlags faceMask, uint32_t compareMask)
{
	vkCmdSetStencilCompareMask(commandBuffer, static_cast<VkStencilFaceFlags>(faceMask), compareMask);
}
void cmdSetStencilWriteMask(CommandBuffer commandBuffer, StencilFaceFlags faceMask, uint32_t writeMask)
{
	vkCmdSetStencilWriteMask(commandBuffer, static_cast<VkStencilFaceFlags>(faceMask), writeMask);
}
void cmdSetStencilReference(CommandBuffer commandBuffer, StencilFaceFlags faceMask, uint32_t reference)
{
	vkCmdSetStencilReference(commandBuffer, static_cast<VkStencilFaceFlags>(faceMask), reference);
}
void cmdBindDescriptorSets(CommandBuffer commandBuffer, PipelineBindPoint pipelineBindPoint, PipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, const DescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, const uint32_t* pDynamicOffsets)
{
	vkCmdBindDescriptorSets(commandBuffer, static_cast<VkPipelineBindPoint>(pipelineBindPoint), layout, firstSet, descriptorSetCount, reinterpret_cast<const VkDescriptorSet*>(pDescriptorSets), dynamicOffsetCount, reinterpret_cast<const uint32_t*>(pDynamicOffsets));
}
void cmdBindIndexBuffer(CommandBuffer commandBuffer, Buffer buffer, DeviceSize offset, IndexType indexType)
{
	vkCmdBindIndexBuffer(commandBuffer, buffer, offset, static_cast<VkIndexType>(indexType));
}
void cmdBindVertexBuffers(CommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const Buffer* pBuffers, const DeviceSize* pOffsets)
{
	vkCmdBindVertexBuffers(commandBuffer, firstBinding, bindingCount, reinterpret_cast<const VkBuffer*>(pBuffers), reinterpret_cast<const VkDeviceSize*>(pOffsets));
}
void cmdDraw(CommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance)
{
	vkCmdDraw(commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
}
void cmdDrawIndexed(CommandBuffer commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance)
{
	vkCmdDrawIndexed(commandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
}
void cmdDrawIndirect(CommandBuffer commandBuffer, Buffer buffer, DeviceSize offset, uint32_t drawCount, uint32_t stride)
{
	vkCmdDrawIndirect(commandBuffer, buffer, offset, drawCount, stride);
}
void cmdDrawIndexedIndirect(CommandBuffer commandBuffer, Buffer buffer, DeviceSize offset, uint32_t drawCount, uint32_t stride)
{
	vkCmdDrawIndexedIndirect(commandBuffer, buffer, offset, drawCount, stride);
}
void cmdDispatch(CommandBuffer commandBuffer, uint32_t x, uint32_t y, uint32_t z)
{
	vkCmdDispatch(commandBuffer, x, y, z);
}
void cmdDispatchIndirect(CommandBuffer commandBuffer, Buffer buffer, DeviceSize offset)
{
	vkCmdDispatchIndirect(commandBuffer, buffer, offset);
}
void cmdCopyBuffer(CommandBuffer commandBuffer, Buffer srcBuffer, Buffer dstBuffer, uint32_t regionCount, const BufferCopy* pRegions)
{
	vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, regionCount, reinterpret_cast<const VkBufferCopy*>(pRegions));
}
void cmdCopyImage(CommandBuffer commandBuffer, Image srcImage, ImageLayout srcImageLayout, Image dstImage, ImageLayout dstImageLayout, uint32_t regionCount, const ImageCopy* pRegions)
{
	vkCmdCopyImage(commandBuffer, srcImage, static_cast<VkImageLayout>(srcImageLayout), dstImage, static_cast<VkImageLayout>(dstImageLayout), regionCount, reinterpret_cast<const VkImageCopy*>(pRegions));
}
void cmdBlitImage(CommandBuffer commandBuffer, Image srcImage, ImageLayout srcImageLayout, Image dstImage, ImageLayout dstImageLayout, uint32_t regionCount, const ImageBlit* pRegions, Filter filter)
{
	vkCmdBlitImage(commandBuffer, srcImage, static_cast<VkImageLayout>(srcImageLayout), dstImage, static_cast<VkImageLayout>(dstImageLayout), regionCount, reinterpret_cast<const VkImageBlit*>(pRegions), static_cast<VkFilter>(filter));
}
void cmdCopyBufferToImage(CommandBuffer commandBuffer, Buffer srcBuffer, Image dstImage, ImageLayout dstImageLayout, uint32_t regionCount, const BufferImageCopy* pRegions)
{
	vkCmdCopyBufferToImage(commandBuffer, srcBuffer, dstImage, static_cast<VkImageLayout>(dstImageLayout), regionCount, reinterpret_cast<const VkBufferImageCopy*>(pRegions));
}
void cmdCopyImageToBuffer(CommandBuffer commandBuffer, Image srcImage, ImageLayout srcImageLayout, Buffer dstBuffer, uint32_t regionCount, const BufferImageCopy* pRegions)
{
	vkCmdCopyImageToBuffer(commandBuffer, srcImage, static_cast<VkImageLayout>(srcImageLayout), dstBuffer, regionCount, reinterpret_cast<const VkBufferImageCopy*>(pRegions));
}
void cmdUpdateBuffer(CommandBuffer commandBuffer, Buffer dstBuffer, DeviceSize dstOffset, DeviceSize dataSize, const uint32_t* pData)
{
	vkCmdUpdateBuffer(commandBuffer, dstBuffer, dstOffset, dataSize, reinterpret_cast<const uint32_t*>(pData));
}
void cmdFillBuffer(CommandBuffer commandBuffer, Buffer dstBuffer, DeviceSize dstOffset, DeviceSize size, uint32_t data)
{
	vkCmdFillBuffer(commandBuffer, dstBuffer, dstOffset, size, data);
}
void cmdClearColorImage(CommandBuffer commandBuffer, Image image, ImageLayout imageLayout, const ClearColorValue* pColor, uint32_t rangeCount, const ImageSubresourceRange* pRanges)
{
	vkCmdClearColorImage(commandBuffer, image, static_cast<VkImageLayout>(imageLayout), reinterpret_cast<const VkClearColorValue*>(pColor), rangeCount, reinterpret_cast<const VkImageSubresourceRange*>(pRanges));
}
void cmdClearDepthStencilImage(CommandBuffer commandBuffer, Image image, ImageLayout imageLayout, const ClearDepthStencilValue* pDepthStencil, uint32_t rangeCount, const ImageSubresourceRange* pRanges)
{
	vkCmdClearDepthStencilImage(commandBuffer, image, static_cast<VkImageLayout>(imageLayout), reinterpret_cast<const VkClearDepthStencilValue*>(pDepthStencil), rangeCount, reinterpret_cast<const VkImageSubresourceRange*>(pRanges));
}
void cmdClearAttachments(CommandBuffer commandBuffer, uint32_t attachmentCount, const ClearAttachment* pAttachments, uint32_t rectCount, const ClearRect* pRects)
{
	vkCmdClearAttachments(commandBuffer, attachmentCount, reinterpret_cast<const VkClearAttachment*>(pAttachments), rectCount, reinterpret_cast<const VkClearRect*>(pRects));
}
void cmdResolveImage(CommandBuffer commandBuffer, Image srcImage, ImageLayout srcImageLayout, Image dstImage, ImageLayout dstImageLayout, uint32_t regionCount, const ImageResolve* pRegions)
{
	vkCmdResolveImage(commandBuffer, srcImage, static_cast<VkImageLayout>(srcImageLayout), dstImage, static_cast<VkImageLayout>(dstImageLayout), regionCount, reinterpret_cast<const VkImageResolve*>(pRegions));
}
void cmdSetEvent(CommandBuffer commandBuffer, Event event, PipelineStageFlags stageMask)
{
	vkCmdSetEvent(commandBuffer, event, static_cast<VkPipelineStageFlags>(stageMask));
}
void cmdResetEvent(CommandBuffer commandBuffer, Event event, PipelineStageFlags stageMask)
{
	vkCmdResetEvent(commandBuffer, event, static_cast<VkPipelineStageFlags>(stageMask));
}
void cmdWaitEvents(CommandBuffer commandBuffer, uint32_t eventCount, const Event* pEvents, PipelineStageFlags srcStageMask, PipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, const MemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const BufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const ImageMemoryBarrier* pImageMemoryBarriers)
{
	vkCmdWaitEvents(commandBuffer, eventCount, reinterpret_cast<const VkEvent*>(pEvents), static_cast<VkPipelineStageFlags>(srcStageMask), static_cast<VkPipelineStageFlags>(dstStageMask), memoryBarrierCount, reinterpret_cast<const VkMemoryBarrier*>(pMemoryBarriers), bufferMemoryBarrierCount, reinterpret_cast<const VkBufferMemoryBarrier*>(pBufferMemoryBarriers), imageMemoryBarrierCount, reinterpret_cast<const VkImageMemoryBarrier*>(pImageMemoryBarriers));
}
void cmdPipelineBarrier(CommandBuffer commandBuffer, PipelineStageFlags srcStageMask, PipelineStageFlags dstStageMask, DependencyFlags dependencyFlags, uint32_t memoryBarrierCount, const MemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const BufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const ImageMemoryBarrier* pImageMemoryBarriers)
{
	vkCmdPipelineBarrier(commandBuffer, static_cast<VkPipelineStageFlags>(srcStageMask), static_cast<VkPipelineStageFlags>(dstStageMask), static_cast<VkDependencyFlags>(dependencyFlags), memoryBarrierCount, reinterpret_cast<const VkMemoryBarrier*>(pMemoryBarriers), bufferMemoryBarrierCount, reinterpret_cast<const VkBufferMemoryBarrier*>(pBufferMemoryBarriers), imageMemoryBarrierCount, reinterpret_cast<const VkImageMemoryBarrier*>(pImageMemoryBarriers));
}
void cmdBeginQuery(CommandBuffer commandBuffer, QueryPool queryPool, uint32_t query, QueryControlFlags flags)
{
	vkCmdBeginQuery(commandBuffer, queryPool, query, static_cast<VkQueryControlFlags>(flags));
}
void cmdEndQuery(CommandBuffer commandBuffer, QueryPool queryPool, uint32_t query)
{
	vkCmdEndQuery(commandBuffer, queryPool, query);
}
void cmdResetQueryPool(CommandBuffer commandBuffer, QueryPool queryPool, uint32_t firstQuery, uint32_t queryCount)
{
	vkCmdResetQueryPool(commandBuffer, queryPool, firstQuery, queryCount);
}
void cmdWriteTimestamp(CommandBuffer commandBuffer, PipelineStageBits pipelineStage, QueryPool queryPool, uint32_t query)
{
	vkCmdWriteTimestamp(commandBuffer, static_cast<VkPipelineStageFlagBits>(pipelineStage), queryPool, query);
}
void cmdCopyQueryPoolResults(CommandBuffer commandBuffer, QueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, Buffer dstBuffer, DeviceSize dstOffset, DeviceSize stride, QueryResultFlags flags)
{
	vkCmdCopyQueryPoolResults(commandBuffer, queryPool, firstQuery, queryCount, dstBuffer, dstOffset, stride, static_cast<VkQueryResultFlags>(flags));
}
void cmdPushConstants(CommandBuffer commandBuffer, PipelineLayout layout, ShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void* pValues)
{
	vkCmdPushConstants(commandBuffer, layout, static_cast<VkShaderStageFlags>(stageFlags), offset, size, reinterpret_cast<const void*>(pValues));
}
void cmdBeginRenderPass(CommandBuffer commandBuffer, const RenderPassBeginInfo* pRenderPassBegin, SubpassContents contents)
{
	vkCmdBeginRenderPass(commandBuffer, reinterpret_cast<const VkRenderPassBeginInfo*>(pRenderPassBegin), static_cast<VkSubpassContents>(contents));
}
void cmdNextSubpass(CommandBuffer commandBuffer, SubpassContents contents)
{
	vkCmdNextSubpass(commandBuffer, static_cast<VkSubpassContents>(contents));
}
void cmdEndRenderPass(CommandBuffer commandBuffer)
{
	vkCmdEndRenderPass(commandBuffer);
}
void cmdExecuteCommands(CommandBuffer commandBuffer, uint32_t commandBufferCount, const CommandBuffer* pCommandBuffers)
{
	vkCmdExecuteCommands(commandBuffer, commandBufferCount, reinterpret_cast<const VkCommandBuffer*>(pCommandBuffers));
}

void destroySurfaceKHR(Instance instance, SurfaceKHR surface, const AllocationCallbacks* pAllocator)
{
	vkDestroySurfaceKHR(instance, surface, reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
Result getPhysicalDeviceSurfaceSupportKHR(PhysicalDevice physicalDevice, uint32_t queueFamilyIndex, SurfaceKHR surface, Bool32* pSupported)
{
	return VPP_CALL(static_cast<Result>(vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, queueFamilyIndex, surface, reinterpret_cast<VkBool32*>(pSupported))));
}
Result getPhysicalDeviceSurfaceCapabilitiesKHR(PhysicalDevice physicalDevice, SurfaceKHR surface, SurfaceCapabilitiesKHR* pSurfaceCapabilities)
{
	return VPP_CALL(static_cast<Result>(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, reinterpret_cast<VkSurfaceCapabilitiesKHR*>(pSurfaceCapabilities))));
}
Result getPhysicalDeviceSurfaceFormatsKHR(PhysicalDevice physicalDevice, SurfaceKHR surface, uint32_t* pSurfaceFormatCount, SurfaceFormatKHR* pSurfaceFormats)
{
	return VPP_CALL(static_cast<Result>(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, reinterpret_cast<uint32_t*>(pSurfaceFormatCount), reinterpret_cast<VkSurfaceFormatKHR*>(pSurfaceFormats))));
}
Result getPhysicalDeviceSurfacePresentModesKHR(PhysicalDevice physicalDevice, SurfaceKHR surface, uint32_t* pPresentModeCount, PresentModeKHR* pPresentModes)
{
	return VPP_CALL(static_cast<Result>(vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, reinterpret_cast<uint32_t*>(pPresentModeCount), reinterpret_cast<VkPresentModeKHR*>(pPresentModes))));
}

Result createSwapchainKHR(Device device, const SwapchainCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, SwapchainKHR* pSwapchain)
{
	return VPP_CALL(static_cast<Result>(vkCreateSwapchainKHR(device, reinterpret_cast<const VkSwapchainCreateInfoKHR*>(pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkSwapchainKHR*>(pSwapchain))));
}
void destroySwapchainKHR(Device device, SwapchainKHR swapchain, const AllocationCallbacks* pAllocator)
{
	vkDestroySwapchainKHR(device, swapchain, reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
Result getSwapchainImagesKHR(Device device, SwapchainKHR swapchain, uint32_t* pSwapchainImageCount, Image* pSwapchainImages)
{
	return VPP_CALL(static_cast<Result>(vkGetSwapchainImagesKHR(device, swapchain, reinterpret_cast<uint32_t*>(pSwapchainImageCount), reinterpret_cast<VkImage*>(pSwapchainImages))));
}
Result acquireNextImageKHR(Device device, SwapchainKHR swapchain, uint64_t timeout, Semaphore semaphore, Fence fence, uint32_t* pImageIndex)
{
	return VPP_CALL(static_cast<Result>(vkAcquireNextImageKHR(device, swapchain, timeout, semaphore, fence, reinterpret_cast<uint32_t*>(pImageIndex))));
}
Result queuePresentKHR(Queue queue, const PresentInfoKHR* pPresentInfo)
{
	return VPP_CALL(static_cast<Result>(vkQueuePresentKHR(queue, reinterpret_cast<const VkPresentInfoKHR*>(pPresentInfo))));
}

Result getPhysicalDeviceDisplayPropertiesKHR(PhysicalDevice physicalDevice, uint32_t* pPropertyCount, DisplayPropertiesKHR* pProperties)
{
	return VPP_CALL(static_cast<Result>(vkGetPhysicalDeviceDisplayPropertiesKHR(physicalDevice, reinterpret_cast<uint32_t*>(pPropertyCount), reinterpret_cast<VkDisplayPropertiesKHR*>(pProperties))));
}
Result getPhysicalDeviceDisplayPlanePropertiesKHR(PhysicalDevice physicalDevice, uint32_t* pPropertyCount, DisplayPlanePropertiesKHR* pProperties)
{
	return VPP_CALL(static_cast<Result>(vkGetPhysicalDeviceDisplayPlanePropertiesKHR(physicalDevice, reinterpret_cast<uint32_t*>(pPropertyCount), reinterpret_cast<VkDisplayPlanePropertiesKHR*>(pProperties))));
}
Result getDisplayPlaneSupportedDisplaysKHR(PhysicalDevice physicalDevice, uint32_t planeIndex, uint32_t* pDisplayCount, DisplayKHR* pDisplays)
{
	return VPP_CALL(static_cast<Result>(vkGetDisplayPlaneSupportedDisplaysKHR(physicalDevice, planeIndex, reinterpret_cast<uint32_t*>(pDisplayCount), reinterpret_cast<VkDisplayKHR*>(pDisplays))));
}
Result getDisplayModePropertiesKHR(PhysicalDevice physicalDevice, DisplayKHR display, uint32_t* pPropertyCount, DisplayModePropertiesKHR* pProperties)
{
	return VPP_CALL(static_cast<Result>(vkGetDisplayModePropertiesKHR(physicalDevice, display, reinterpret_cast<uint32_t*>(pPropertyCount), reinterpret_cast<VkDisplayModePropertiesKHR*>(pProperties))));
}
Result createDisplayModeKHR(PhysicalDevice physicalDevice, DisplayKHR display, const DisplayModeCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, DisplayModeKHR* pMode)
{
	return VPP_CALL(static_cast<Result>(vkCreateDisplayModeKHR(physicalDevice, display, reinterpret_cast<const VkDisplayModeCreateInfoKHR*>(pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkDisplayModeKHR*>(pMode))));
}
Result getDisplayPlaneCapabilitiesKHR(PhysicalDevice physicalDevice, DisplayModeKHR mode, uint32_t planeIndex, DisplayPlaneCapabilitiesKHR* pCapabilities)
{
	return VPP_CALL(static_cast<Result>(vkGetDisplayPlaneCapabilitiesKHR(physicalDevice, mode, planeIndex, reinterpret_cast<VkDisplayPlaneCapabilitiesKHR*>(pCapabilities))));
}
Result createDisplayPlaneSurfaceKHR(Instance instance, const DisplaySurfaceCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceKHR* pSurface)
{
	return VPP_CALL(static_cast<Result>(vkCreateDisplayPlaneSurfaceKHR(instance, reinterpret_cast<const VkDisplaySurfaceCreateInfoKHR*>(pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkSurfaceKHR*>(pSurface))));
}

Result createSharedSwapchainsKHR(Device device, uint32_t swapchainCount, const SwapchainCreateInfoKHR* pCreateInfos, const AllocationCallbacks* pAllocator, SwapchainKHR* pSwapchains)
{
	return VPP_CALL(static_cast<Result>(vkCreateSharedSwapchainsKHR(device, swapchainCount, reinterpret_cast<const VkSwapchainCreateInfoKHR*>(pCreateInfos), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkSwapchainKHR*>(pSwapchains))));
}

#ifdef VK_USE_PLATFORM_XLIB_KHR

Result createXlibSurfaceKHR(Instance instance, const XlibSurfaceCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceKHR* pSurface)
{
	return VPP_CALL(static_cast<Result>(vkCreateXlibSurfaceKHR(instance, reinterpret_cast<const VkXlibSurfaceCreateInfoKHR*>(pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkSurfaceKHR*>(pSurface))));
}
Bool32 getPhysicalDeviceXlibPresentationSupportKHR(PhysicalDevice physicalDevice, uint32_t queueFamilyIndex, Display* dpy, VisualID visualID)
{
	return static_cast<Bool32>(vkGetPhysicalDeviceXlibPresentationSupportKHR(physicalDevice, queueFamilyIndex, reinterpret_cast<Display*>(dpy), visualID));
}

#endif //VK_USE_PLATFORM_XLIB_KHR

#ifdef VK_USE_PLATFORM_XCB_KHR

Result createXcbSurfaceKHR(Instance instance, const XcbSurfaceCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceKHR* pSurface)
{
	return VPP_CALL(static_cast<Result>(vkCreateXcbSurfaceKHR(instance, reinterpret_cast<const VkXcbSurfaceCreateInfoKHR*>(pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkSurfaceKHR*>(pSurface))));
}
Bool32 getPhysicalDeviceXcbPresentationSupportKHR(PhysicalDevice physicalDevice, uint32_t queueFamilyIndex, xcb_connection_t* connection, xcb_visualid_t visual_id)
{
	return static_cast<Bool32>(vkGetPhysicalDeviceXcbPresentationSupportKHR(physicalDevice, queueFamilyIndex, reinterpret_cast<xcb_connection_t*>(connection), visual_id));
}

#endif //VK_USE_PLATFORM_XCB_KHR

#ifdef VK_USE_PLATFORM_WAYLAND_KHR

Result createWaylandSurfaceKHR(Instance instance, const WaylandSurfaceCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceKHR* pSurface)
{
	return VPP_CALL(static_cast<Result>(vkCreateWaylandSurfaceKHR(instance, reinterpret_cast<const VkWaylandSurfaceCreateInfoKHR*>(pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkSurfaceKHR*>(pSurface))));
}
Bool32 getPhysicalDeviceWaylandPresentationSupportKHR(PhysicalDevice physicalDevice, uint32_t queueFamilyIndex, wl_display* display)
{
	return static_cast<Bool32>(vkGetPhysicalDeviceWaylandPresentationSupportKHR(physicalDevice, queueFamilyIndex, reinterpret_cast<wl_display*>(display)));
}

#endif //VK_USE_PLATFORM_WAYLAND_KHR

#ifdef VK_USE_PLATFORM_MIR_KHR

Result createMirSurfaceKHR(Instance instance, const MirSurfaceCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceKHR* pSurface)
{
	return VPP_CALL(static_cast<Result>(vkCreateMirSurfaceKHR(instance, reinterpret_cast<const VkMirSurfaceCreateInfoKHR*>(pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkSurfaceKHR*>(pSurface))));
}
Bool32 getPhysicalDeviceMirPresentationSupportKHR(PhysicalDevice physicalDevice, uint32_t queueFamilyIndex, MirConnection* connection)
{
	return static_cast<Bool32>(vkGetPhysicalDeviceMirPresentationSupportKHR(physicalDevice, queueFamilyIndex, reinterpret_cast<MirConnection*>(connection)));
}

#endif //VK_USE_PLATFORM_MIR_KHR

#ifdef VK_USE_PLATFORM_ANDROID_KHR

Result createAndroidSurfaceKHR(Instance instance, const AndroidSurfaceCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceKHR* pSurface)
{
	return VPP_CALL(static_cast<Result>(vkCreateAndroidSurfaceKHR(instance, reinterpret_cast<const VkAndroidSurfaceCreateInfoKHR*>(pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkSurfaceKHR*>(pSurface))));
}

#endif //VK_USE_PLATFORM_ANDROID_KHR

#ifdef VK_USE_PLATFORM_WIN32_KHR

Result createWin32SurfaceKHR(Instance instance, const Win32SurfaceCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceKHR* pSurface)
{
	return VPP_CALL(static_cast<Result>(vkCreateWin32SurfaceKHR(instance, reinterpret_cast<const VkWin32SurfaceCreateInfoKHR*>(pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkSurfaceKHR*>(pSurface))));
}
Bool32 getPhysicalDeviceWin32PresentationSupportKHR(PhysicalDevice physicalDevice, uint32_t queueFamilyIndex)
{
	return static_cast<Bool32>(vkGetPhysicalDeviceWin32PresentationSupportKHR(physicalDevice, queueFamilyIndex));
}

#endif //VK_USE_PLATFORM_WIN32_KHR

Result createDebugReportCallbackEXT(Instance instance, const DebugReportCallbackCreateInfoEXT* pCreateInfo, const AllocationCallbacks* pAllocator, DebugReportCallbackEXT* pCallback)
{
	return VPP_CALL(static_cast<Result>(vkCreateDebugReportCallbackEXT(instance, reinterpret_cast<const VkDebugReportCallbackCreateInfoEXT*>(pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkDebugReportCallbackEXT*>(pCallback))));
}
void destroyDebugReportCallbackEXT(Instance instance, DebugReportCallbackEXT callback, const AllocationCallbacks* pAllocator)
{
	vkDestroyDebugReportCallbackEXT(instance, callback, reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
void debugReportMessageEXT(Instance instance, DebugReportFlagsEXT flags, DebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char* pLayerPrefix, const char* pMessage)
{
	vkDebugReportMessageEXT(instance, static_cast<VkDebugReportFlagsEXT>(flags), static_cast<VkDebugReportObjectTypeEXT>(objectType), object, location, messageCode, reinterpret_cast<const char*>(pLayerPrefix), reinterpret_cast<const char*>(pMessage));
}

Result debugMarkerSetObjectTagEXT(Device device, DebugMarkerObjectTagInfoEXT* pTagInfo)
{
	return VPP_CALL(static_cast<Result>(vkDebugMarkerSetObjectTagEXT(device, reinterpret_cast<VkDebugMarkerObjectTagInfoEXT*>(pTagInfo))));
}
Result debugMarkerSetObjectNameEXT(Device device, DebugMarkerObjectNameInfoEXT* pNameInfo)
{
	return VPP_CALL(static_cast<Result>(vkDebugMarkerSetObjectNameEXT(device, reinterpret_cast<VkDebugMarkerObjectNameInfoEXT*>(pNameInfo))));
}
void cmdDebugMarkerBeginEXT(CommandBuffer commandBuffer, DebugMarkerMarkerInfoEXT* pMarkerInfo)
{
	vkCmdDebugMarkerBeginEXT(commandBuffer, reinterpret_cast<VkDebugMarkerMarkerInfoEXT*>(pMarkerInfo));
}
void cmdDebugMarkerEndEXT(CommandBuffer commandBuffer)
{
	vkCmdDebugMarkerEndEXT(commandBuffer);
}
void cmdDebugMarkerInsertEXT(CommandBuffer commandBuffer, DebugMarkerMarkerInfoEXT* pMarkerInfo)
{
	vkCmdDebugMarkerInsertEXT(commandBuffer, reinterpret_cast<VkDebugMarkerMarkerInfoEXT*>(pMarkerInfo));
}



} //namespace vk