// Copyright(c) 2015-2016, NVIDIA CORPORATION. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of NVIDIA CORPORATION nor the names of its
//    contributors may be used to endorse or promote products derived
//    from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
// OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Forked and edited by nyorain. The given content is not equal to the original version.


#pragma once
#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <vector>
#include <vulkan/vulkan.h>

#include "types.hpp"
#include "enums.hpp"
#include "structs.hpp"
#include "call.hpp"

namespace vk
{
  inline Result createInstance( const InstanceCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, Instance* pInstance )
  {
    return static_cast<Result>( VPP_CALL(vkCreateInstance( reinterpret_cast<const VkInstanceCreateInfo*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), pInstance )) );
  }

  inline Result createInstance( const InstanceCreateInfo& createInfo, const AllocationCallbacks& allocator, Instance& instance )
  {
    return createInstance( &createInfo, &allocator, &instance );
  }

  inline void destroyInstance( Instance instance, const AllocationCallbacks* pAllocator )
  {
    vkDestroyInstance( instance, reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
  }

  inline void destroyInstance( Instance instance, const AllocationCallbacks& allocator )
  {
    destroyInstance( instance, &allocator );
  }

  inline Result enumeratePhysicalDevices( Instance instance, uint32_t* pPhysicalDeviceCount, PhysicalDevice* pPhysicalDevices )
  {
    return static_cast<Result>( VPP_CALL(vkEnumeratePhysicalDevices( instance, pPhysicalDeviceCount, pPhysicalDevices )) );
  }

  inline Result enumeratePhysicalDevices( Instance instance, std::vector<PhysicalDevice> & physicalDevices )
  {
    uint32_t pPhysicalDeviceCount = 0;
    Result result = enumeratePhysicalDevices( instance, &pPhysicalDeviceCount, nullptr );
    if ( result == Result::Success )
    {
      physicalDevices.resize( pPhysicalDeviceCount );
      result = enumeratePhysicalDevices( instance, &pPhysicalDeviceCount, physicalDevices.data() );
    }
	return result;
  }

  inline PFN_vkVoidFunction getDeviceProcAddr( Device device, const char* pName )
  {
    return vkGetDeviceProcAddr( device, pName );
  }

  inline PFN_vkVoidFunction getDeviceProcAddr( Device device, std::string const&  name )
  {
    return getDeviceProcAddr( device, name.data() );
  }

  inline PFN_vkVoidFunction getInstanceProcAddr( Instance instance, const char* pName )
  {
    return vkGetInstanceProcAddr( instance, pName );
  }

  inline PFN_vkVoidFunction getInstanceProcAddr( Instance instance, std::string const&  name )
  {
    return getInstanceProcAddr( instance, name.data() );
  }

  inline void getPhysicalDeviceProperties( PhysicalDevice physicalDevice, PhysicalDeviceProperties* pProperties )
  {
    vkGetPhysicalDeviceProperties( physicalDevice, reinterpret_cast<VkPhysicalDeviceProperties*>( pProperties ) );
  }

  inline void getPhysicalDeviceProperties( PhysicalDevice physicalDevice, PhysicalDeviceProperties& properties )
  {
    getPhysicalDeviceProperties( physicalDevice, &properties );
  }

  inline void getPhysicalDeviceQueueFamilyProperties( PhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, QueueFamilyProperties* pQueueFamilyProperties )
  {
    vkGetPhysicalDeviceQueueFamilyProperties( physicalDevice, pQueueFamilyPropertyCount, reinterpret_cast<VkQueueFamilyProperties*>( pQueueFamilyProperties ) );
  }

  inline std::vector<QueueFamilyProperties> getPhysicalDeviceQueueFamilyProperties( PhysicalDevice physicalDevice )
  {
    uint32_t pQueueFamilyPropertyCount = 0;
    getPhysicalDeviceQueueFamilyProperties( physicalDevice, &pQueueFamilyPropertyCount, nullptr );
    std::vector<QueueFamilyProperties> pQueueFamilyProperties( pQueueFamilyPropertyCount );
    getPhysicalDeviceQueueFamilyProperties( physicalDevice, &pQueueFamilyPropertyCount, pQueueFamilyProperties.data() );
    return pQueueFamilyProperties;
  }

  inline void getPhysicalDeviceMemoryProperties( PhysicalDevice physicalDevice, PhysicalDeviceMemoryProperties* pMemoryProperties )
  {
    vkGetPhysicalDeviceMemoryProperties( physicalDevice, reinterpret_cast<VkPhysicalDeviceMemoryProperties*>( pMemoryProperties ) );
  }

  inline void getPhysicalDeviceMemoryProperties( PhysicalDevice physicalDevice, PhysicalDeviceMemoryProperties& memoryProperties )
  {
    getPhysicalDeviceMemoryProperties( physicalDevice, &memoryProperties );
  }

  inline void getPhysicalDeviceFeatures( PhysicalDevice physicalDevice, PhysicalDeviceFeatures* pFeatures )
  {
    vkGetPhysicalDeviceFeatures( physicalDevice, reinterpret_cast<VkPhysicalDeviceFeatures*>( pFeatures ) );
  }

  inline void getPhysicalDeviceFeatures( PhysicalDevice physicalDevice, PhysicalDeviceFeatures& features )
  {
    getPhysicalDeviceFeatures( physicalDevice, &features );
  }

  inline void getPhysicalDeviceFormatProperties( PhysicalDevice physicalDevice, Format format, FormatProperties* pFormatProperties )
  {
    vkGetPhysicalDeviceFormatProperties( physicalDevice, static_cast<VkFormat>( format ), reinterpret_cast<VkFormatProperties*>( pFormatProperties ) );
  }

  inline void getPhysicalDeviceFormatProperties( PhysicalDevice physicalDevice, Format format, FormatProperties& formatProperties )
  {
    getPhysicalDeviceFormatProperties( physicalDevice, format, &formatProperties );
  }

  inline Result getPhysicalDeviceImageFormatProperties( PhysicalDevice physicalDevice, Format format, ImageType type, ImageTiling tiling, ImageUsageFlags usage, ImageCreateFlags flags, ImageFormatProperties* pImageFormatProperties )
  {
    return static_cast<Result>( VPP_CALL(vkGetPhysicalDeviceImageFormatProperties( physicalDevice, static_cast<VkFormat>( format ), static_cast<VkImageType>( type ), static_cast<VkImageTiling>( tiling ), static_cast<VkImageUsageFlags>( usage ), static_cast<VkImageCreateFlags>( flags ), reinterpret_cast<VkImageFormatProperties*>( pImageFormatProperties ) )) );
  }

  inline Result getPhysicalDeviceImageFormatProperties( PhysicalDevice physicalDevice, Format format, ImageType type, ImageTiling tiling, ImageUsageFlags usage, ImageCreateFlags flags, ImageFormatProperties& imageFormatProperties )
  {
    return getPhysicalDeviceImageFormatProperties( physicalDevice, format, type, tiling, usage, flags, &imageFormatProperties );
  }

  inline Result createDevice( PhysicalDevice physicalDevice, const DeviceCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, Device* pDevice )
  {
    return static_cast<Result>( VPP_CALL(vkCreateDevice( physicalDevice, reinterpret_cast<const VkDeviceCreateInfo*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), pDevice )) );
  }

  inline Result createDevice( PhysicalDevice physicalDevice, const DeviceCreateInfo& createInfo, const AllocationCallbacks& allocator, Device& device )
  {
    return createDevice( physicalDevice, &createInfo, &allocator, &device );
  }

  inline void destroyDevice( Device device, const AllocationCallbacks* pAllocator )
  {
    vkDestroyDevice( device, reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
  }

  inline void destroyDevice( Device device, const AllocationCallbacks& allocator )
  {
    destroyDevice( device, &allocator );
  }

  inline Result enumerateInstanceLayerProperties( uint32_t* pPropertyCount, LayerProperties* pProperties )
  {
    return static_cast<Result>( VPP_CALL(vkEnumerateInstanceLayerProperties( pPropertyCount, reinterpret_cast<VkLayerProperties*>( pProperties ) )) );
  }

  inline Result enumerateInstanceLayerProperties( std::vector<LayerProperties> & properties )
  {
    uint32_t pPropertyCount = 0;
    Result result = enumerateInstanceLayerProperties( &pPropertyCount, nullptr );
    if ( result == Result::Success )
    {
      properties.resize( pPropertyCount );
      result = enumerateInstanceLayerProperties( &pPropertyCount, properties.data() );
    }
	return result;
  }

  inline Result enumerateInstanceExtensionProperties( const char* pLayerName, uint32_t* pPropertyCount, ExtensionProperties* pProperties )
  {
    return static_cast<Result>( VPP_CALL(vkEnumerateInstanceExtensionProperties( pLayerName, pPropertyCount, reinterpret_cast<VkExtensionProperties*>( pProperties ) )) );
  }

  inline Result enumerateInstanceExtensionProperties( std::string const&  layerName, std::vector<ExtensionProperties> & properties )
  {
    uint32_t pPropertyCount = 0;
    Result result = enumerateInstanceExtensionProperties( layerName.data(), &pPropertyCount, nullptr );
    if ( result == Result::Success )
    {
      properties.resize( pPropertyCount );
      result = enumerateInstanceExtensionProperties( layerName.data(), &pPropertyCount, properties.data() );
    }
	return result;
  }

  inline Result enumerateDeviceLayerProperties( PhysicalDevice physicalDevice, uint32_t* pPropertyCount, LayerProperties* pProperties )
  {
    return static_cast<Result>( VPP_CALL(vkEnumerateDeviceLayerProperties( physicalDevice, pPropertyCount, reinterpret_cast<VkLayerProperties*>( pProperties ) )) );
  }

  inline Result enumerateDeviceLayerProperties( PhysicalDevice physicalDevice, std::vector<LayerProperties> & properties )
  {
    uint32_t pPropertyCount = 0;
    Result result = enumerateDeviceLayerProperties( physicalDevice, &pPropertyCount, nullptr );
    if ( result == Result::Success )
    {
      properties.resize( pPropertyCount );
      result = enumerateDeviceLayerProperties( physicalDevice, &pPropertyCount, properties.data() );
    }
	return result;
  }

  inline Result enumerateDeviceExtensionProperties( PhysicalDevice physicalDevice, const char* pLayerName, uint32_t* pPropertyCount, ExtensionProperties* pProperties )
  {
    return static_cast<Result>( VPP_CALL(vkEnumerateDeviceExtensionProperties( physicalDevice, pLayerName, pPropertyCount, reinterpret_cast<VkExtensionProperties*>( pProperties ) )) );
  }

  inline Result enumerateDeviceExtensionProperties( PhysicalDevice physicalDevice, std::string const&  layerName, std::vector<ExtensionProperties> & properties )
  {
    uint32_t pPropertyCount = 0;
    Result result = enumerateDeviceExtensionProperties( physicalDevice, layerName.data(), &pPropertyCount, nullptr );
    if ( result == Result::Success )
    {
      properties.resize( pPropertyCount );
      result = enumerateDeviceExtensionProperties( physicalDevice, layerName.data(), &pPropertyCount, properties.data() );
    }
	return result;
  }

  inline void getDeviceQueue( Device device, uint32_t queueFamilyIndex, uint32_t queueIndex, Queue* pQueue )
  {
    vkGetDeviceQueue( device, queueFamilyIndex, queueIndex, pQueue );
  }

  inline void getDeviceQueue( Device device, uint32_t queueFamilyIndex, uint32_t queueIndex, Queue& queue )
  {
    getDeviceQueue( device, queueFamilyIndex, queueIndex, &queue );
  }

  inline Result queueSubmit( Queue queue, uint32_t submitCount, const SubmitInfo* pSubmits, Fence fence )
  {
    return static_cast<Result>( VPP_CALL(vkQueueSubmit( queue, submitCount, reinterpret_cast<const VkSubmitInfo*>( pSubmits ), fence )) );
  }

  inline Result queueSubmit( Queue queue, std::vector<SubmitInfo> const& submits, Fence fence )
  {
    return queueSubmit( queue, static_cast<uint32_t>( submits.size() ), submits.data(), fence );
  }

  inline Result queueWaitIdle( Queue queue )
  {
    return static_cast<Result>( VPP_CALL(vkQueueWaitIdle( queue )) );
  }

  inline Result deviceWaitIdle( Device device )
  {
    return static_cast<Result>( VPP_CALL(vkDeviceWaitIdle( device )) );
  }

  inline Result allocateMemory( Device device, const MemoryAllocateInfo* pAllocateInfo, const AllocationCallbacks* pAllocator, DeviceMemory* pMemory )
  {
    return static_cast<Result>( VPP_CALL(vkAllocateMemory( device, reinterpret_cast<const VkMemoryAllocateInfo*>( pAllocateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), pMemory )) );
  }

  inline Result allocateMemory( Device device, const MemoryAllocateInfo& allocateInfo, const AllocationCallbacks& allocator, DeviceMemory& memory )
  {
    return allocateMemory( device, &allocateInfo, &allocator, &memory );
  }

  inline void freeMemory( Device device, DeviceMemory memory, const AllocationCallbacks* pAllocator )
  {
    vkFreeMemory( device, memory, reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
  }

  inline void freeMemory( Device device, DeviceMemory memory, const AllocationCallbacks& allocator )
  {
    freeMemory( device, memory, &allocator );
  }

  inline Result mapMemory( Device device, DeviceMemory memory, DeviceSize offset, DeviceSize size, MemoryMapFlags flags, void** ppData )
  {
    return static_cast<Result>( VPP_CALL(vkMapMemory( device, memory, offset, size, static_cast<VkMemoryMapFlags>( flags ), ppData )) );
  }

  inline void unmapMemory( Device device, DeviceMemory memory )
  {
    vkUnmapMemory( device, memory );
  }

  inline Result flushMappedMemoryRanges( Device device, uint32_t memoryRangeCount, const MappedMemoryRange* pMemoryRanges )
  {
    return static_cast<Result>( VPP_CALL(vkFlushMappedMemoryRanges( device, memoryRangeCount, reinterpret_cast<const VkMappedMemoryRange*>( pMemoryRanges ) )) );
  }

  inline Result flushMappedMemoryRanges( Device device, std::vector<MappedMemoryRange> const& memoryRanges )
  {
    return flushMappedMemoryRanges( device, static_cast<uint32_t>( memoryRanges.size() ), memoryRanges.data() );
  }

  inline Result invalidateMappedMemoryRanges( Device device, uint32_t memoryRangeCount, const MappedMemoryRange* pMemoryRanges )
  {
    return static_cast<Result>( VPP_CALL(vkInvalidateMappedMemoryRanges( device, memoryRangeCount, reinterpret_cast<const VkMappedMemoryRange*>( pMemoryRanges ) )) );
  }

  inline Result invalidateMappedMemoryRanges( Device device, std::vector<MappedMemoryRange> const& memoryRanges )
  {
    return invalidateMappedMemoryRanges( device, static_cast<uint32_t>( memoryRanges.size() ), memoryRanges.data() );
  }

  inline void getDeviceMemoryCommitment( Device device, DeviceMemory memory, DeviceSize* pCommittedMemoryInBytes )
  {
    vkGetDeviceMemoryCommitment( device, memory, pCommittedMemoryInBytes );
  }

  inline void getDeviceMemoryCommitment( Device device, DeviceMemory memory, DeviceSize& committedMemoryInBytes )
  {
    getDeviceMemoryCommitment( device, memory, &committedMemoryInBytes );
  }

  inline void getBufferMemoryRequirements( Device device, Buffer buffer, MemoryRequirements* pMemoryRequirements )
  {
    vkGetBufferMemoryRequirements( device, buffer, reinterpret_cast<VkMemoryRequirements*>( pMemoryRequirements ) );
  }

  inline void getBufferMemoryRequirements( Device device, Buffer buffer, MemoryRequirements& memoryRequirements )
  {
    getBufferMemoryRequirements( device, buffer, &memoryRequirements );
  }

  inline Result bindBufferMemory( Device device, Buffer buffer, DeviceMemory memory, DeviceSize memoryOffset )
  {
    return static_cast<Result>( VPP_CALL(vkBindBufferMemory( device, buffer, memory, memoryOffset )) );
  }

  inline void getImageMemoryRequirements( Device device, Image image, MemoryRequirements* pMemoryRequirements )
  {
    vkGetImageMemoryRequirements( device, image, reinterpret_cast<VkMemoryRequirements*>( pMemoryRequirements ) );
  }

  inline void getImageMemoryRequirements( Device device, Image image, MemoryRequirements& memoryRequirements )
  {
    getImageMemoryRequirements( device, image, &memoryRequirements );
  }

  inline Result bindImageMemory( Device device, Image image, DeviceMemory memory, DeviceSize memoryOffset )
  {
    return static_cast<Result>( VPP_CALL(vkBindImageMemory( device, image, memory, memoryOffset )) );
  }

  inline void getImageSparseMemoryRequirements( Device device, Image image, uint32_t* pSparseMemoryRequirementCount, SparseImageMemoryRequirements* pSparseMemoryRequirements )
  {
    vkGetImageSparseMemoryRequirements( device, image, pSparseMemoryRequirementCount, reinterpret_cast<VkSparseImageMemoryRequirements*>( pSparseMemoryRequirements ) );
  }

  inline std::vector<SparseImageMemoryRequirements> getImageSparseMemoryRequirements( Device device, Image image )
  {
    uint32_t pSparseMemoryRequirementCount = 0;
    getImageSparseMemoryRequirements( device, image, &pSparseMemoryRequirementCount, nullptr );
    std::vector<SparseImageMemoryRequirements> pSparseMemoryRequirements( pSparseMemoryRequirementCount );
    getImageSparseMemoryRequirements( device, image, &pSparseMemoryRequirementCount, pSparseMemoryRequirements.data() );
    return pSparseMemoryRequirements;
  }

  inline void getPhysicalDeviceSparseImageFormatProperties( PhysicalDevice physicalDevice, Format format, ImageType type, SampleCountFlagBits samples, ImageUsageFlags usage, ImageTiling tiling, uint32_t* pPropertyCount, SparseImageFormatProperties* pProperties )
  {
    vkGetPhysicalDeviceSparseImageFormatProperties( physicalDevice, static_cast<VkFormat>( format ), static_cast<VkImageType>( type ), static_cast<VkSampleCountFlagBits>( samples ), static_cast<VkImageUsageFlags>( usage ), static_cast<VkImageTiling>( tiling ), pPropertyCount, reinterpret_cast<VkSparseImageFormatProperties*>( pProperties ) );
  }

  inline std::vector<SparseImageFormatProperties> getPhysicalDeviceSparseImageFormatProperties( PhysicalDevice physicalDevice, Format format, ImageType type, SampleCountFlagBits samples, ImageUsageFlags usage, ImageTiling tiling )
  {
    uint32_t pPropertyCount = 0;
    getPhysicalDeviceSparseImageFormatProperties( physicalDevice, format, type, samples, usage, tiling, &pPropertyCount, nullptr );
    std::vector<SparseImageFormatProperties> pProperties( pPropertyCount );
    getPhysicalDeviceSparseImageFormatProperties( physicalDevice, format, type, samples, usage, tiling, &pPropertyCount, pProperties.data() );
    return pProperties;
  }

  inline Result queueBindSparse( Queue queue, uint32_t bindInfoCount, const BindSparseInfo* pBindInfo, Fence fence )
  {
    return static_cast<Result>( VPP_CALL(vkQueueBindSparse( queue, bindInfoCount, reinterpret_cast<const VkBindSparseInfo*>( pBindInfo ), fence )) );
  }

  inline Result queueBindSparse( Queue queue, std::vector<BindSparseInfo> const& bindInfo, Fence fence )
  {
    return queueBindSparse( queue, static_cast<uint32_t>( bindInfo.size() ), bindInfo.data(), fence );
  }

  inline Result createFence( Device device, const FenceCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, Fence* pFence )
  {
    return static_cast<Result>( VPP_CALL(vkCreateFence( device, reinterpret_cast<const VkFenceCreateInfo*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), pFence )) );
  }

  inline Result createFence( Device device, const FenceCreateInfo& createInfo, const AllocationCallbacks& allocator, Fence& fence )
  {
    return createFence( device, &createInfo, &allocator, &fence );
  }

  inline void destroyFence( Device device, Fence fence, const AllocationCallbacks* pAllocator )
  {
    vkDestroyFence( device, fence, reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
  }

  inline void destroyFence( Device device, Fence fence, const AllocationCallbacks& allocator )
  {
    destroyFence( device, fence, &allocator );
  }

  inline Result resetFences( Device device, uint32_t fenceCount, const Fence* pFences )
  {
    return static_cast<Result>( VPP_CALL(vkResetFences( device, fenceCount, pFences )) );
  }

  inline Result resetFences( Device device, std::vector<Fence> const& fences )
  {
    return resetFences( device, static_cast<uint32_t>( fences.size() ), fences.data() );
  }

  inline Result getFenceStatus( Device device, Fence fence )
  {
    return static_cast<Result>( VPP_CALL(vkGetFenceStatus( device, fence )) );
  }

  inline Result waitForFences( Device device, uint32_t fenceCount, const Fence* pFences, Bool32 waitAll, uint64_t timeout )
  {
    return static_cast<Result>( VPP_CALL(vkWaitForFences( device, fenceCount, pFences, waitAll, timeout )) );
  }

  inline Result waitForFences( Device device, std::vector<Fence> const& fences, Bool32 waitAll, uint64_t timeout )
  {
    return waitForFences( device, static_cast<uint32_t>( fences.size() ), fences.data(), waitAll, timeout );
  }

  inline Result createSemaphore( Device device, const SemaphoreCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, Semaphore* pSemaphore )
  {
    return static_cast<Result>( VPP_CALL(vkCreateSemaphore( device, reinterpret_cast<const VkSemaphoreCreateInfo*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), pSemaphore )) );
  }

  inline Result createSemaphore( Device device, const SemaphoreCreateInfo& createInfo, const AllocationCallbacks& allocator, Semaphore& semaphore )
  {
    return createSemaphore( device, &createInfo, &allocator, &semaphore );
  }

  inline void destroySemaphore( Device device, Semaphore semaphore, const AllocationCallbacks* pAllocator )
  {
    vkDestroySemaphore( device, semaphore, reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
  }

  inline void destroySemaphore( Device device, Semaphore semaphore, const AllocationCallbacks& allocator )
  {
    destroySemaphore( device, semaphore, &allocator );
  }

  inline Result createEvent( Device device, const EventCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, Event* pEvent )
  {
    return static_cast<Result>( VPP_CALL(vkCreateEvent( device, reinterpret_cast<const VkEventCreateInfo*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), pEvent )) );
  }

  inline Result createEvent( Device device, const EventCreateInfo& createInfo, const AllocationCallbacks& allocator, Event& event )
  {
    return createEvent( device, &createInfo, &allocator, &event );
  }

  inline void destroyEvent( Device device, Event event, const AllocationCallbacks* pAllocator )
  {
    vkDestroyEvent( device, event, reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
  }

  inline void destroyEvent( Device device, Event event, const AllocationCallbacks& allocator )
  {
    destroyEvent( device, event, &allocator );
  }

  inline Result getEventStatus( Device device, Event event )
  {
    return static_cast<Result>( VPP_CALL(vkGetEventStatus( device, event )) );
  }

  inline Result setEvent( Device device, Event event )
  {
    return static_cast<Result>( VPP_CALL(vkSetEvent( device, event )) );
  }

  inline Result resetEvent( Device device, Event event )
  {
    return static_cast<Result>( VPP_CALL(vkResetEvent( device, event )) );
  }

  inline Result createQueryPool( Device device, const QueryPoolCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, QueryPool* pQueryPool )
  {
    return static_cast<Result>( VPP_CALL(vkCreateQueryPool( device, reinterpret_cast<const VkQueryPoolCreateInfo*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), pQueryPool )) );
  }

  inline Result createQueryPool( Device device, const QueryPoolCreateInfo& createInfo, const AllocationCallbacks& allocator, QueryPool& queryPool )
  {
    return createQueryPool( device, &createInfo, &allocator, &queryPool );
  }

  inline void destroyQueryPool( Device device, QueryPool queryPool, const AllocationCallbacks* pAllocator )
  {
    vkDestroyQueryPool( device, queryPool, reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
  }

  inline void destroyQueryPool( Device device, QueryPool queryPool, const AllocationCallbacks& allocator )
  {
    destroyQueryPool( device, queryPool, &allocator );
  }

  inline Result getQueryPoolResults( Device device, QueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, size_t dataSize, void* pData, DeviceSize stride, QueryResultFlags flags )
  {
    return static_cast<Result>( VPP_CALL(vkGetQueryPoolResults( device, queryPool, firstQuery, queryCount, dataSize, pData, stride, static_cast<VkQueryResultFlags>( flags ) )) );
  }

  inline Result getQueryPoolResults( Device device, QueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, std::vector<uint8_t> & data, DeviceSize stride, QueryResultFlags flags )
  {
    return getQueryPoolResults( device, queryPool, firstQuery, queryCount, static_cast<size_t>( data.size() ), data.data(), stride, flags );
  }

  inline Result createBuffer( Device device, const BufferCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, Buffer* pBuffer )
  {
    return static_cast<Result>( VPP_CALL(vkCreateBuffer( device, reinterpret_cast<const VkBufferCreateInfo*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), pBuffer )) );
  }

  inline Result createBuffer( Device device, const BufferCreateInfo& createInfo, const AllocationCallbacks& allocator, Buffer& buffer )
  {
    return createBuffer( device, &createInfo, &allocator, &buffer );
  }

  inline void destroyBuffer( Device device, Buffer buffer, const AllocationCallbacks* pAllocator )
  {
    vkDestroyBuffer( device, buffer, reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
  }

  inline void destroyBuffer( Device device, Buffer buffer, const AllocationCallbacks& allocator )
  {
    destroyBuffer( device, buffer, &allocator );
  }

  inline Result createBufferView( Device device, const BufferViewCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, BufferView* pView )
  {
    return static_cast<Result>( VPP_CALL(vkCreateBufferView( device, reinterpret_cast<const VkBufferViewCreateInfo*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), pView )) );
  }

  inline Result createBufferView( Device device, const BufferViewCreateInfo& createInfo, const AllocationCallbacks& allocator, BufferView& view )
  {
    return createBufferView( device, &createInfo, &allocator, &view );
  }

  inline void destroyBufferView( Device device, BufferView bufferView, const AllocationCallbacks* pAllocator )
  {
    vkDestroyBufferView( device, bufferView, reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
  }

  inline void destroyBufferView( Device device, BufferView bufferView, const AllocationCallbacks& allocator )
  {
    destroyBufferView( device, bufferView, &allocator );
  }

  inline Result createImage( Device device, const ImageCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, Image* pImage )
  {
    return static_cast<Result>( VPP_CALL(vkCreateImage( device, reinterpret_cast<const VkImageCreateInfo*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), pImage )) );
  }

  inline Result createImage( Device device, const ImageCreateInfo& createInfo, const AllocationCallbacks& allocator, Image& image )
  {
    return createImage( device, &createInfo, &allocator, &image );
  }

  inline void destroyImage( Device device, Image image, const AllocationCallbacks* pAllocator )
  {
    vkDestroyImage( device, image, reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
  }

  inline void destroyImage( Device device, Image image, const AllocationCallbacks& allocator )
  {
    destroyImage( device, image, &allocator );
  }

  inline void getImageSubresourceLayout( Device device, Image image, const ImageSubresource* pSubresource, SubresourceLayout* pLayout )
  {
    vkGetImageSubresourceLayout( device, image, reinterpret_cast<const VkImageSubresource*>( pSubresource ), reinterpret_cast<VkSubresourceLayout*>( pLayout ) );
  }

  inline void getImageSubresourceLayout( Device device, Image image, const ImageSubresource& subresource, SubresourceLayout& layout )
  {
    getImageSubresourceLayout( device, image, &subresource, &layout );
  }

  inline Result createImageView( Device device, const ImageViewCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, ImageView* pView )
  {
    return static_cast<Result>( VPP_CALL(vkCreateImageView( device, reinterpret_cast<const VkImageViewCreateInfo*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), pView )) );
  }

  inline Result createImageView( Device device, const ImageViewCreateInfo& createInfo, const AllocationCallbacks& allocator, ImageView& view )
  {
    return createImageView( device, &createInfo, &allocator, &view );
  }

  inline void destroyImageView( Device device, ImageView imageView, const AllocationCallbacks* pAllocator )
  {
    vkDestroyImageView( device, imageView, reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
  }

  inline void destroyImageView( Device device, ImageView imageView, const AllocationCallbacks& allocator )
  {
    destroyImageView( device, imageView, &allocator );
  }

  inline Result createShaderModule( Device device, const ShaderModuleCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, ShaderModule* pShaderModule )
  {
    return static_cast<Result>( VPP_CALL(vkCreateShaderModule( device, reinterpret_cast<const VkShaderModuleCreateInfo*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), pShaderModule )) );
  }

  inline Result createShaderModule( Device device, const ShaderModuleCreateInfo& createInfo, const AllocationCallbacks& allocator, ShaderModule& shaderModule )
  {
    return createShaderModule( device, &createInfo, &allocator, &shaderModule );
  }

  inline void destroyShaderModule( Device device, ShaderModule shaderModule, const AllocationCallbacks* pAllocator )
  {
    vkDestroyShaderModule( device, shaderModule, reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
  }

  inline void destroyShaderModule( Device device, ShaderModule shaderModule, const AllocationCallbacks& allocator )
  {
    destroyShaderModule( device, shaderModule, &allocator );
  }

  inline Result createPipelineCache( Device device, const PipelineCacheCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, PipelineCache* pPipelineCache )
  {
    return static_cast<Result>( VPP_CALL(vkCreatePipelineCache( device, reinterpret_cast<const VkPipelineCacheCreateInfo*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), pPipelineCache )) );
  }

  inline Result createPipelineCache( Device device, const PipelineCacheCreateInfo& createInfo, const AllocationCallbacks& allocator, PipelineCache& pipelineCache )
  {
    return createPipelineCache( device, &createInfo, &allocator, &pipelineCache );
  }

  inline void destroyPipelineCache( Device device, PipelineCache pipelineCache, const AllocationCallbacks* pAllocator )
  {
    vkDestroyPipelineCache( device, pipelineCache, reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
  }

  inline void destroyPipelineCache( Device device, PipelineCache pipelineCache, const AllocationCallbacks& allocator )
  {
    destroyPipelineCache( device, pipelineCache, &allocator );
  }

  inline Result getPipelineCacheData( Device device, PipelineCache pipelineCache, size_t* pDataSize, void* pData )
  {
    return static_cast<Result>( VPP_CALL(vkGetPipelineCacheData( device, pipelineCache, pDataSize, pData )) );
  }

  inline Result getPipelineCacheData( Device device, PipelineCache pipelineCache, std::vector<uint8_t> & data )
  {
    size_t pDataSize = 0;
    Result result = getPipelineCacheData( device, pipelineCache, &pDataSize, nullptr );
    if ( result == Result::Success )
    {
      data.resize( pDataSize );
      result = getPipelineCacheData( device, pipelineCache, &pDataSize, data.data() );
    }
	return result;
  }

  inline Result mergePipelineCaches( Device device, PipelineCache dstCache, uint32_t srcCacheCount, const PipelineCache* pSrcCaches )
  {
    return static_cast<Result>( VPP_CALL(vkMergePipelineCaches( device, dstCache, srcCacheCount, pSrcCaches )) );
  }

  inline Result mergePipelineCaches( Device device, PipelineCache dstCache, std::vector<PipelineCache> const& srcCaches )
  {
    return mergePipelineCaches( device, dstCache, static_cast<uint32_t>( srcCaches.size() ), srcCaches.data() );
  }

  inline Result createGraphicsPipelines( Device device, PipelineCache pipelineCache, uint32_t createInfoCount, const GraphicsPipelineCreateInfo* pCreateInfos, const AllocationCallbacks* pAllocator, Pipeline* pPipelines )
  {
    return static_cast<Result>( VPP_CALL(vkCreateGraphicsPipelines( device, pipelineCache, createInfoCount, reinterpret_cast<const VkGraphicsPipelineCreateInfo*>( pCreateInfos ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), pPipelines )) );
  }

  inline Result createGraphicsPipelines( Device device, PipelineCache pipelineCache, std::vector<GraphicsPipelineCreateInfo> const& createInfos, const AllocationCallbacks& allocator, std::vector<Pipeline> & pipelines )
  {
    assert( createInfos.size() <= pipelines.size() );
    return createGraphicsPipelines( device, pipelineCache, static_cast<uint32_t>( createInfos.size() ), createInfos.data(), &allocator, pipelines.data() );
  }

  inline Result createComputePipelines( Device device, PipelineCache pipelineCache, uint32_t createInfoCount, const ComputePipelineCreateInfo* pCreateInfos, const AllocationCallbacks* pAllocator, Pipeline* pPipelines )
  {
    return static_cast<Result>( VPP_CALL(vkCreateComputePipelines( device, pipelineCache, createInfoCount, reinterpret_cast<const VkComputePipelineCreateInfo*>( pCreateInfos ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), pPipelines )) );
  }

  inline Result createComputePipelines( Device device, PipelineCache pipelineCache, std::vector<ComputePipelineCreateInfo> const& createInfos, const AllocationCallbacks& allocator, std::vector<Pipeline> & pipelines )
  {
    assert( createInfos.size() <= pipelines.size() );
    return createComputePipelines( device, pipelineCache, static_cast<uint32_t>( createInfos.size() ), createInfos.data(), &allocator, pipelines.data() );
  }

  inline void destroyPipeline( Device device, Pipeline pipeline, const AllocationCallbacks* pAllocator )
  {
    vkDestroyPipeline( device, pipeline, reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
  }

  inline void destroyPipeline( Device device, Pipeline pipeline, const AllocationCallbacks& allocator )
  {
    destroyPipeline( device, pipeline, &allocator );
  }

  inline Result createPipelineLayout( Device device, const PipelineLayoutCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, PipelineLayout* pPipelineLayout )
  {
    return static_cast<Result>( VPP_CALL(vkCreatePipelineLayout( device, reinterpret_cast<const VkPipelineLayoutCreateInfo*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), pPipelineLayout )) );
  }

  inline Result createPipelineLayout( Device device, const PipelineLayoutCreateInfo& createInfo, const AllocationCallbacks& allocator, PipelineLayout& pipelineLayout )
  {
    return createPipelineLayout( device, &createInfo, &allocator, &pipelineLayout );
  }

  inline void destroyPipelineLayout( Device device, PipelineLayout pipelineLayout, const AllocationCallbacks* pAllocator )
  {
    vkDestroyPipelineLayout( device, pipelineLayout, reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
  }

  inline void destroyPipelineLayout( Device device, PipelineLayout pipelineLayout, const AllocationCallbacks& allocator )
  {
    destroyPipelineLayout( device, pipelineLayout, &allocator );
  }

  inline Result createSampler( Device device, const SamplerCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, Sampler* pSampler )
  {
    return static_cast<Result>( VPP_CALL(vkCreateSampler( device, reinterpret_cast<const VkSamplerCreateInfo*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), pSampler )) );
  }

  inline Result createSampler( Device device, const SamplerCreateInfo& createInfo, const AllocationCallbacks& allocator, Sampler& sampler )
  {
    return createSampler( device, &createInfo, &allocator, &sampler );
  }

  inline void destroySampler( Device device, Sampler sampler, const AllocationCallbacks* pAllocator )
  {
    vkDestroySampler( device, sampler, reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
  }

  inline void destroySampler( Device device, Sampler sampler, const AllocationCallbacks& allocator )
  {
    destroySampler( device, sampler, &allocator );
  }

  inline Result createDescriptorSetLayout( Device device, const DescriptorSetLayoutCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, DescriptorSetLayout* pSetLayout )
  {
    return static_cast<Result>( VPP_CALL(vkCreateDescriptorSetLayout( device, reinterpret_cast<const VkDescriptorSetLayoutCreateInfo*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), pSetLayout )) );
  }

  inline Result createDescriptorSetLayout( Device device, const DescriptorSetLayoutCreateInfo& createInfo, const AllocationCallbacks& allocator, DescriptorSetLayout& setLayout )
  {
    return createDescriptorSetLayout( device, &createInfo, &allocator, &setLayout );
  }

  inline void destroyDescriptorSetLayout( Device device, DescriptorSetLayout descriptorSetLayout, const AllocationCallbacks* pAllocator )
  {
    vkDestroyDescriptorSetLayout( device, descriptorSetLayout, reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
  }

  inline void destroyDescriptorSetLayout( Device device, DescriptorSetLayout descriptorSetLayout, const AllocationCallbacks& allocator )
  {
    destroyDescriptorSetLayout( device, descriptorSetLayout, &allocator );
  }

  inline Result createDescriptorPool( Device device, const DescriptorPoolCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, DescriptorPool* pDescriptorPool )
  {
    return static_cast<Result>( VPP_CALL(vkCreateDescriptorPool( device, reinterpret_cast<const VkDescriptorPoolCreateInfo*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), pDescriptorPool )) );
  }

  inline Result createDescriptorPool( Device device, const DescriptorPoolCreateInfo& createInfo, const AllocationCallbacks& allocator, DescriptorPool& descriptorPool )
  {
    return createDescriptorPool( device, &createInfo, &allocator, &descriptorPool );
  }

  inline void destroyDescriptorPool( Device device, DescriptorPool descriptorPool, const AllocationCallbacks* pAllocator )
  {
    vkDestroyDescriptorPool( device, descriptorPool, reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
  }

  inline void destroyDescriptorPool( Device device, DescriptorPool descriptorPool, const AllocationCallbacks& allocator )
  {
    destroyDescriptorPool( device, descriptorPool, &allocator );
  }

  inline Result resetDescriptorPool( Device device, DescriptorPool descriptorPool, DescriptorPoolResetFlags flags )
  {
    return static_cast<Result>( VPP_CALL(vkResetDescriptorPool( device, descriptorPool, static_cast<VkDescriptorPoolResetFlags>( flags ) )) );
  }

  inline Result allocateDescriptorSets( Device device, const DescriptorSetAllocateInfo* pAllocateInfo, DescriptorSet* pDescriptorSets )
  {
    return static_cast<Result>( VPP_CALL(vkAllocateDescriptorSets( device, reinterpret_cast<const VkDescriptorSetAllocateInfo*>( pAllocateInfo ), pDescriptorSets )) );
  }

  inline Result allocateDescriptorSets( Device device, const DescriptorSetAllocateInfo& allocateInfo, std::vector<DescriptorSet> & descriptorSets )
  {
    return allocateDescriptorSets( device, &allocateInfo, descriptorSets.data() );
  }

  inline Result freeDescriptorSets( Device device, DescriptorPool descriptorPool, uint32_t descriptorSetCount, const DescriptorSet* pDescriptorSets )
  {
    return static_cast<Result>( VPP_CALL(vkFreeDescriptorSets( device, descriptorPool, descriptorSetCount, pDescriptorSets )) );
  }

  inline Result freeDescriptorSets( Device device, DescriptorPool descriptorPool, std::vector<DescriptorSet> const& descriptorSets )
  {
    return freeDescriptorSets( device, descriptorPool, static_cast<uint32_t>( descriptorSets.size() ), descriptorSets.data() );
  }

  inline void updateDescriptorSets( Device device, uint32_t descriptorWriteCount, const WriteDescriptorSet* pDescriptorWrites, uint32_t descriptorCopyCount, const CopyDescriptorSet* pDescriptorCopies )
  {
    vkUpdateDescriptorSets( device, descriptorWriteCount, reinterpret_cast<const VkWriteDescriptorSet*>( pDescriptorWrites ), descriptorCopyCount, reinterpret_cast<const VkCopyDescriptorSet*>( pDescriptorCopies ) );
  }

  inline void updateDescriptorSets( Device device, std::vector<WriteDescriptorSet> const& descriptorWrites, std::vector<CopyDescriptorSet> const& descriptorCopies )
  {
    updateDescriptorSets( device, static_cast<uint32_t>( descriptorWrites.size() ), descriptorWrites.data(), static_cast<uint32_t>( descriptorCopies.size() ), descriptorCopies.data() );
  }

  inline Result createFramebuffer( Device device, const FramebufferCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, Framebuffer* pFramebuffer )
  {
    return static_cast<Result>( VPP_CALL(vkCreateFramebuffer( device, reinterpret_cast<const VkFramebufferCreateInfo*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), pFramebuffer )) );
  }

  inline Result createFramebuffer( Device device, const FramebufferCreateInfo& createInfo, const AllocationCallbacks& allocator, Framebuffer& framebuffer )
  {
    return createFramebuffer( device, &createInfo, &allocator, &framebuffer );
  }

  inline void destroyFramebuffer( Device device, Framebuffer framebuffer, const AllocationCallbacks* pAllocator )
  {
    vkDestroyFramebuffer( device, framebuffer, reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
  }

  inline void destroyFramebuffer( Device device, Framebuffer framebuffer, const AllocationCallbacks& allocator )
  {
    destroyFramebuffer( device, framebuffer, &allocator );
  }

  inline Result createRenderPass( Device device, const RenderPassCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, RenderPass* pRenderPass )
  {
    return static_cast<Result>( VPP_CALL(vkCreateRenderPass( device, reinterpret_cast<const VkRenderPassCreateInfo*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), pRenderPass )) );
  }

  inline Result createRenderPass( Device device, const RenderPassCreateInfo& createInfo, const AllocationCallbacks& allocator, RenderPass& renderPass )
  {
    return createRenderPass( device, &createInfo, &allocator, &renderPass );
  }

  inline void destroyRenderPass( Device device, RenderPass renderPass, const AllocationCallbacks* pAllocator )
  {
    vkDestroyRenderPass( device, renderPass, reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
  }

  inline void destroyRenderPass( Device device, RenderPass renderPass, const AllocationCallbacks& allocator )
  {
    destroyRenderPass( device, renderPass, &allocator );
  }

  inline void getRenderAreaGranularity( Device device, RenderPass renderPass, Extent2D* pGranularity )
  {
    vkGetRenderAreaGranularity( device, renderPass, reinterpret_cast<VkExtent2D*>( pGranularity ) );
  }

  inline void getRenderAreaGranularity( Device device, RenderPass renderPass, Extent2D& granularity )
  {
    getRenderAreaGranularity( device, renderPass, &granularity );
  }

  inline Result createCommandPool( Device device, const CommandPoolCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, CommandPool* pCommandPool )
  {
    return static_cast<Result>( VPP_CALL(vkCreateCommandPool( device, reinterpret_cast<const VkCommandPoolCreateInfo*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), pCommandPool )) );
  }

  inline Result createCommandPool( Device device, const CommandPoolCreateInfo& createInfo, const AllocationCallbacks& allocator, CommandPool& commandPool )
  {
    return createCommandPool( device, &createInfo, &allocator, &commandPool );
  }

  inline void destroyCommandPool( Device device, CommandPool commandPool, const AllocationCallbacks* pAllocator )
  {
    vkDestroyCommandPool( device, commandPool, reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
  }

  inline void destroyCommandPool( Device device, CommandPool commandPool, const AllocationCallbacks& allocator )
  {
    destroyCommandPool( device, commandPool, &allocator );
  }

  inline Result resetCommandPool( Device device, CommandPool commandPool, CommandPoolResetFlags flags )
  {
    return static_cast<Result>( VPP_CALL(vkResetCommandPool( device, commandPool, static_cast<VkCommandPoolResetFlags>( flags ) )) );
  }

  inline Result allocateCommandBuffers( Device device, const CommandBufferAllocateInfo* pAllocateInfo, CommandBuffer* pCommandBuffers )
  {
    return static_cast<Result>( VPP_CALL(vkAllocateCommandBuffers( device, reinterpret_cast<const VkCommandBufferAllocateInfo*>( pAllocateInfo ), pCommandBuffers )) );
  }

  inline Result allocateCommandBuffers( Device device, const CommandBufferAllocateInfo& allocateInfo, std::vector<CommandBuffer> & commandBuffers )
  {
    return allocateCommandBuffers( device, &allocateInfo, commandBuffers.data() );
  }

  inline void freeCommandBuffers( Device device, CommandPool commandPool, uint32_t commandBufferCount, const CommandBuffer* pCommandBuffers )
  {
    vkFreeCommandBuffers( device, commandPool, commandBufferCount, pCommandBuffers );
  }

  inline void freeCommandBuffers( Device device, CommandPool commandPool, std::vector<CommandBuffer> const& commandBuffers )
  {
    freeCommandBuffers( device, commandPool, static_cast<uint32_t>( commandBuffers.size() ), commandBuffers.data() );
  }

  inline Result beginCommandBuffer( CommandBuffer commandBuffer, const CommandBufferBeginInfo* pBeginInfo )
  {
    return static_cast<Result>( VPP_CALL(vkBeginCommandBuffer( commandBuffer, reinterpret_cast<const VkCommandBufferBeginInfo*>( pBeginInfo ) )) );
  }

  inline Result beginCommandBuffer( CommandBuffer commandBuffer, const CommandBufferBeginInfo& beginInfo )
  {
    return beginCommandBuffer( commandBuffer, &beginInfo );
  }

  inline Result endCommandBuffer( CommandBuffer commandBuffer )
  {
    return static_cast<Result>( VPP_CALL(vkEndCommandBuffer( commandBuffer )) );
  }

  inline Result resetCommandBuffer( CommandBuffer commandBuffer, CommandBufferResetFlags flags )
  {
    return static_cast<Result>( VPP_CALL(vkResetCommandBuffer( commandBuffer, static_cast<VkCommandBufferResetFlags>( flags ) )) );
  }

  inline void cmdBindPipeline( CommandBuffer commandBuffer, PipelineBindPoint pipelineBindPoint, Pipeline pipeline )
  {
    vkCmdBindPipeline( commandBuffer, static_cast<VkPipelineBindPoint>( pipelineBindPoint ), pipeline );
  }

  inline void cmdSetViewport( CommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const Viewport* pViewports )
  {
    vkCmdSetViewport( commandBuffer, firstViewport, viewportCount, reinterpret_cast<const VkViewport*>( pViewports ) );
  }

  inline void cmdSetViewport( CommandBuffer commandBuffer, uint32_t firstViewport, std::vector<Viewport> const& viewports )
  {
    cmdSetViewport( commandBuffer, firstViewport, static_cast<uint32_t>( viewports.size() ), viewports.data() );
  }

  inline void cmdSetScissor( CommandBuffer commandBuffer, uint32_t firstScissor, uint32_t scissorCount, const Rect2D* pScissors )
  {
    vkCmdSetScissor( commandBuffer, firstScissor, scissorCount, reinterpret_cast<const VkRect2D*>( pScissors ) );
  }

  inline void cmdSetScissor( CommandBuffer commandBuffer, uint32_t firstScissor, std::vector<Rect2D> const& scissors )
  {
    cmdSetScissor( commandBuffer, firstScissor, static_cast<uint32_t>( scissors.size() ), scissors.data() );
  }

  inline void cmdSetLineWidth( CommandBuffer commandBuffer, float lineWidth )
  {
    vkCmdSetLineWidth( commandBuffer, lineWidth );
  }

  inline void cmdSetDepthBias( CommandBuffer commandBuffer, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor )
  {
    vkCmdSetDepthBias( commandBuffer, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor );
  }

  inline void cmdSetBlendConstants( CommandBuffer commandBuffer, const float blendConstants[4] )
  {
    vkCmdSetBlendConstants( commandBuffer, blendConstants );
  }

  inline void cmdSetDepthBounds( CommandBuffer commandBuffer, float minDepthBounds, float maxDepthBounds )
  {
    vkCmdSetDepthBounds( commandBuffer, minDepthBounds, maxDepthBounds );
  }

  inline void cmdSetStencilCompareMask( CommandBuffer commandBuffer, StencilFaceFlags faceMask, uint32_t compareMask )
  {
    vkCmdSetStencilCompareMask( commandBuffer, static_cast<VkStencilFaceFlags>( faceMask ), compareMask );
  }

  inline void cmdSetStencilWriteMask( CommandBuffer commandBuffer, StencilFaceFlags faceMask, uint32_t writeMask )
  {
    vkCmdSetStencilWriteMask( commandBuffer, static_cast<VkStencilFaceFlags>( faceMask ), writeMask );
  }

  inline void cmdSetStencilReference( CommandBuffer commandBuffer, StencilFaceFlags faceMask, uint32_t reference )
  {
    vkCmdSetStencilReference( commandBuffer, static_cast<VkStencilFaceFlags>( faceMask ), reference );
  }

  inline void cmdBindDescriptorSets( CommandBuffer commandBuffer, PipelineBindPoint pipelineBindPoint, PipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, const DescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, const uint32_t* pDynamicOffsets )
  {
    vkCmdBindDescriptorSets( commandBuffer, static_cast<VkPipelineBindPoint>( pipelineBindPoint ), layout, firstSet, descriptorSetCount, pDescriptorSets, dynamicOffsetCount, pDynamicOffsets );
  }

  inline void cmdBindDescriptorSets( CommandBuffer commandBuffer, PipelineBindPoint pipelineBindPoint, PipelineLayout layout, uint32_t firstSet, std::vector<DescriptorSet> const& descriptorSets, std::vector<uint32_t> const& dynamicOffsets )
  {
    cmdBindDescriptorSets( commandBuffer, pipelineBindPoint, layout, firstSet, static_cast<uint32_t>( descriptorSets.size() ), descriptorSets.data(), static_cast<uint32_t>( dynamicOffsets.size() ), dynamicOffsets.data() );
  }

  inline void cmdBindIndexBuffer( CommandBuffer commandBuffer, Buffer buffer, DeviceSize offset, IndexType indexType )
  {
    vkCmdBindIndexBuffer( commandBuffer, buffer, offset, static_cast<VkIndexType>( indexType ) );
  }

  inline void cmdBindVertexBuffers( CommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const Buffer* pBuffers, const DeviceSize* pOffsets )
  {
    vkCmdBindVertexBuffers( commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets );
  }

  inline void cmdBindVertexBuffers( CommandBuffer commandBuffer, uint32_t firstBinding, std::vector<Buffer> const& buffers, std::vector<DeviceSize> const& offsets )
  {
    assert( buffers.size() <= offsets.size() );
    cmdBindVertexBuffers( commandBuffer, firstBinding, static_cast<uint32_t>( buffers.size() ), buffers.data(), offsets.data() );
  }

  inline void cmdDraw( CommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance )
  {
    vkCmdDraw( commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance );
  }

  inline void cmdDrawIndexed( CommandBuffer commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance )
  {
    vkCmdDrawIndexed( commandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance );
  }

  inline void cmdDrawIndirect( CommandBuffer commandBuffer, Buffer buffer, DeviceSize offset, uint32_t drawCount, uint32_t stride )
  {
    vkCmdDrawIndirect( commandBuffer, buffer, offset, drawCount, stride );
  }

  inline void cmdDrawIndexedIndirect( CommandBuffer commandBuffer, Buffer buffer, DeviceSize offset, uint32_t drawCount, uint32_t stride )
  {
    vkCmdDrawIndexedIndirect( commandBuffer, buffer, offset, drawCount, stride );
  }

  inline void cmdDispatch( CommandBuffer commandBuffer, uint32_t x, uint32_t y, uint32_t z )
  {
    vkCmdDispatch( commandBuffer, x, y, z );
  }

  inline void cmdDispatchIndirect( CommandBuffer commandBuffer, Buffer buffer, DeviceSize offset )
  {
    vkCmdDispatchIndirect( commandBuffer, buffer, offset );
  }

  inline void cmdCopyBuffer( CommandBuffer commandBuffer, Buffer srcBuffer, Buffer dstBuffer, uint32_t regionCount, const BufferCopy* pRegions )
  {
    vkCmdCopyBuffer( commandBuffer, srcBuffer, dstBuffer, regionCount, reinterpret_cast<const VkBufferCopy*>( pRegions ) );
  }

  inline void cmdCopyBuffer( CommandBuffer commandBuffer, Buffer srcBuffer, Buffer dstBuffer, std::vector<BufferCopy> const& regions )
  {
    cmdCopyBuffer( commandBuffer, srcBuffer, dstBuffer, static_cast<uint32_t>( regions.size() ), regions.data() );
  }

  inline void cmdCopyImage( CommandBuffer commandBuffer, Image srcImage, ImageLayout srcImageLayout, Image dstImage, ImageLayout dstImageLayout, uint32_t regionCount, const ImageCopy* pRegions )
  {
    vkCmdCopyImage( commandBuffer, srcImage, static_cast<VkImageLayout>( srcImageLayout ), dstImage, static_cast<VkImageLayout>( dstImageLayout ), regionCount, reinterpret_cast<const VkImageCopy*>( pRegions ) );
  }

  inline void cmdCopyImage( CommandBuffer commandBuffer, Image srcImage, ImageLayout srcImageLayout, Image dstImage, ImageLayout dstImageLayout, std::vector<ImageCopy> const& regions )
  {
    cmdCopyImage( commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, static_cast<uint32_t>( regions.size() ), regions.data() );
  }

  inline void cmdBlitImage( CommandBuffer commandBuffer, Image srcImage, ImageLayout srcImageLayout, Image dstImage, ImageLayout dstImageLayout, uint32_t regionCount, const ImageBlit* pRegions, Filter filter )
  {
    vkCmdBlitImage( commandBuffer, srcImage, static_cast<VkImageLayout>( srcImageLayout ), dstImage, static_cast<VkImageLayout>( dstImageLayout ), regionCount, reinterpret_cast<const VkImageBlit*>( pRegions ), static_cast<VkFilter>( filter ) );
  }

  inline void cmdBlitImage( CommandBuffer commandBuffer, Image srcImage, ImageLayout srcImageLayout, Image dstImage, ImageLayout dstImageLayout, std::vector<ImageBlit> const& regions, Filter filter )
  {
    cmdBlitImage( commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, static_cast<uint32_t>( regions.size() ), regions.data(), filter );
  }

  inline void cmdCopyBufferToImage( CommandBuffer commandBuffer, Buffer srcBuffer, Image dstImage, ImageLayout dstImageLayout, uint32_t regionCount, const BufferImageCopy* pRegions )
  {
    vkCmdCopyBufferToImage( commandBuffer, srcBuffer, dstImage, static_cast<VkImageLayout>( dstImageLayout ), regionCount, reinterpret_cast<const VkBufferImageCopy*>( pRegions ) );
  }

  inline void cmdCopyBufferToImage( CommandBuffer commandBuffer, Buffer srcBuffer, Image dstImage, ImageLayout dstImageLayout, std::vector<BufferImageCopy> const& regions )
  {
    cmdCopyBufferToImage( commandBuffer, srcBuffer, dstImage, dstImageLayout, static_cast<uint32_t>( regions.size() ), regions.data() );
  }

  inline void cmdCopyImageToBuffer( CommandBuffer commandBuffer, Image srcImage, ImageLayout srcImageLayout, Buffer dstBuffer, uint32_t regionCount, const BufferImageCopy* pRegions )
  {
    vkCmdCopyImageToBuffer( commandBuffer, srcImage, static_cast<VkImageLayout>( srcImageLayout ), dstBuffer, regionCount, reinterpret_cast<const VkBufferImageCopy*>( pRegions ) );
  }

  inline void cmdCopyImageToBuffer( CommandBuffer commandBuffer, Image srcImage, ImageLayout srcImageLayout, Buffer dstBuffer, std::vector<BufferImageCopy> const& regions )
  {
    cmdCopyImageToBuffer( commandBuffer, srcImage, srcImageLayout, dstBuffer, static_cast<uint32_t>( regions.size() ), regions.data() );
  }

  inline void cmdUpdateBuffer( CommandBuffer commandBuffer, Buffer dstBuffer, DeviceSize dstOffset, DeviceSize dataSize, const uint32_t* pData )
  {
    vkCmdUpdateBuffer( commandBuffer, dstBuffer, dstOffset, dataSize, pData );
  }

  inline void cmdUpdateBuffer( CommandBuffer commandBuffer, Buffer dstBuffer, DeviceSize dstOffset, DeviceSize dataSize, std::vector<uint32_t> const& data )
  {
    cmdUpdateBuffer( commandBuffer, dstBuffer, dstOffset, dataSize, data.data() );
  }

  inline void cmdFillBuffer( CommandBuffer commandBuffer, Buffer dstBuffer, DeviceSize dstOffset, DeviceSize size, uint32_t data )
  {
    vkCmdFillBuffer( commandBuffer, dstBuffer, dstOffset, size, data );
  }

  inline void cmdClearColorImage( CommandBuffer commandBuffer, Image image, ImageLayout imageLayout, const ClearColorValue* pColor, uint32_t rangeCount, const ImageSubresourceRange* pRanges )
  {
    vkCmdClearColorImage( commandBuffer, image, static_cast<VkImageLayout>( imageLayout ), reinterpret_cast<const VkClearColorValue*>( pColor ), rangeCount, reinterpret_cast<const VkImageSubresourceRange*>( pRanges ) );
  }

  inline void cmdClearColorImage( CommandBuffer commandBuffer, Image image, ImageLayout imageLayout, const ClearColorValue& color, std::vector<ImageSubresourceRange> const& ranges )
  {
    cmdClearColorImage( commandBuffer, image, imageLayout, &color, static_cast<uint32_t>( ranges.size() ), ranges.data() );
  }

  inline void cmdClearDepthStencilImage( CommandBuffer commandBuffer, Image image, ImageLayout imageLayout, const ClearDepthStencilValue* pDepthStencil, uint32_t rangeCount, const ImageSubresourceRange* pRanges )
  {
    vkCmdClearDepthStencilImage( commandBuffer, image, static_cast<VkImageLayout>( imageLayout ), reinterpret_cast<const VkClearDepthStencilValue*>( pDepthStencil ), rangeCount, reinterpret_cast<const VkImageSubresourceRange*>( pRanges ) );
  }

  inline void cmdClearDepthStencilImage( CommandBuffer commandBuffer, Image image, ImageLayout imageLayout, const ClearDepthStencilValue& depthStencil, std::vector<ImageSubresourceRange> const& ranges )
  {
    cmdClearDepthStencilImage( commandBuffer, image, imageLayout, &depthStencil, static_cast<uint32_t>( ranges.size() ), ranges.data() );
  }

  inline void cmdClearAttachments( CommandBuffer commandBuffer, uint32_t attachmentCount, const ClearAttachment* pAttachments, uint32_t rectCount, const ClearRect* pRects )
  {
    vkCmdClearAttachments( commandBuffer, attachmentCount, reinterpret_cast<const VkClearAttachment*>( pAttachments ), rectCount, reinterpret_cast<const VkClearRect*>( pRects ) );
  }

  inline void cmdClearAttachments( CommandBuffer commandBuffer, std::vector<ClearAttachment> const& attachments, std::vector<ClearRect> const& rects )
  {
    cmdClearAttachments( commandBuffer, static_cast<uint32_t>( attachments.size() ), attachments.data(), static_cast<uint32_t>( rects.size() ), rects.data() );
  }

  inline void cmdResolveImage( CommandBuffer commandBuffer, Image srcImage, ImageLayout srcImageLayout, Image dstImage, ImageLayout dstImageLayout, uint32_t regionCount, const ImageResolve* pRegions )
  {
    vkCmdResolveImage( commandBuffer, srcImage, static_cast<VkImageLayout>( srcImageLayout ), dstImage, static_cast<VkImageLayout>( dstImageLayout ), regionCount, reinterpret_cast<const VkImageResolve*>( pRegions ) );
  }

  inline void cmdResolveImage( CommandBuffer commandBuffer, Image srcImage, ImageLayout srcImageLayout, Image dstImage, ImageLayout dstImageLayout, std::vector<ImageResolve> const& regions )
  {
    cmdResolveImage( commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, static_cast<uint32_t>( regions.size() ), regions.data() );
  }

  inline void cmdSetEvent( CommandBuffer commandBuffer, Event event, PipelineStageFlags stageMask )
  {
    vkCmdSetEvent( commandBuffer, event, static_cast<VkPipelineStageFlags>( stageMask ) );
  }

  inline void cmdResetEvent( CommandBuffer commandBuffer, Event event, PipelineStageFlags stageMask )
  {
    vkCmdResetEvent( commandBuffer, event, static_cast<VkPipelineStageFlags>( stageMask ) );
  }

  inline void cmdWaitEvents( CommandBuffer commandBuffer, uint32_t eventCount, const Event* pEvents, PipelineStageFlags srcStageMask, PipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, const MemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const BufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const ImageMemoryBarrier* pImageMemoryBarriers )
  {
    vkCmdWaitEvents( commandBuffer, eventCount, pEvents, static_cast<VkPipelineStageFlags>( srcStageMask ), static_cast<VkPipelineStageFlags>( dstStageMask ), memoryBarrierCount, reinterpret_cast<const VkMemoryBarrier*>( pMemoryBarriers ), bufferMemoryBarrierCount, reinterpret_cast<const VkBufferMemoryBarrier*>( pBufferMemoryBarriers ), imageMemoryBarrierCount, reinterpret_cast<const VkImageMemoryBarrier*>( pImageMemoryBarriers ) );
  }

  inline void cmdWaitEvents( CommandBuffer commandBuffer, std::vector<Event> const& events, PipelineStageFlags srcStageMask, PipelineStageFlags dstStageMask, std::vector<MemoryBarrier> const& memoryBarriers, std::vector<BufferMemoryBarrier> const& bufferMemoryBarriers, std::vector<ImageMemoryBarrier> const& imageMemoryBarriers )
  {
    cmdWaitEvents( commandBuffer, static_cast<uint32_t>( events.size() ), events.data(), srcStageMask, dstStageMask, static_cast<uint32_t>( memoryBarriers.size() ), memoryBarriers.data(), static_cast<uint32_t>( bufferMemoryBarriers.size() ), bufferMemoryBarriers.data(), static_cast<uint32_t>( imageMemoryBarriers.size() ), imageMemoryBarriers.data() );
  }

  inline void cmdPipelineBarrier( CommandBuffer commandBuffer, PipelineStageFlags srcStageMask, PipelineStageFlags dstStageMask, DependencyFlags dependencyFlags, uint32_t memoryBarrierCount, const MemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const BufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const ImageMemoryBarrier* pImageMemoryBarriers )
  {
    vkCmdPipelineBarrier( commandBuffer, static_cast<VkPipelineStageFlags>( srcStageMask ), static_cast<VkPipelineStageFlags>( dstStageMask ), static_cast<VkDependencyFlags>( dependencyFlags ), memoryBarrierCount, reinterpret_cast<const VkMemoryBarrier*>( pMemoryBarriers ), bufferMemoryBarrierCount, reinterpret_cast<const VkBufferMemoryBarrier*>( pBufferMemoryBarriers ), imageMemoryBarrierCount, reinterpret_cast<const VkImageMemoryBarrier*>( pImageMemoryBarriers ) );
  }

  inline void cmdPipelineBarrier( CommandBuffer commandBuffer, PipelineStageFlags srcStageMask, PipelineStageFlags dstStageMask, DependencyFlags dependencyFlags, std::vector<MemoryBarrier> const& memoryBarriers, std::vector<BufferMemoryBarrier> const& bufferMemoryBarriers, std::vector<ImageMemoryBarrier> const& imageMemoryBarriers )
  {
    cmdPipelineBarrier( commandBuffer, srcStageMask, dstStageMask, dependencyFlags, static_cast<uint32_t>( memoryBarriers.size() ), memoryBarriers.data(), static_cast<uint32_t>( bufferMemoryBarriers.size() ), bufferMemoryBarriers.data(), static_cast<uint32_t>( imageMemoryBarriers.size() ), imageMemoryBarriers.data() );
  }

  inline void cmdBeginQuery( CommandBuffer commandBuffer, QueryPool queryPool, uint32_t query, QueryControlFlags flags )
  {
    vkCmdBeginQuery( commandBuffer, queryPool, query, static_cast<VkQueryControlFlags>( flags ) );
  }

  inline void cmdEndQuery( CommandBuffer commandBuffer, QueryPool queryPool, uint32_t query )
  {
    vkCmdEndQuery( commandBuffer, queryPool, query );
  }

  inline void cmdResetQueryPool( CommandBuffer commandBuffer, QueryPool queryPool, uint32_t firstQuery, uint32_t queryCount )
  {
    vkCmdResetQueryPool( commandBuffer, queryPool, firstQuery, queryCount );
  }

  inline void cmdWriteTimestamp( CommandBuffer commandBuffer, PipelineStageFlagBits pipelineStage, QueryPool queryPool, uint32_t query )
  {
    vkCmdWriteTimestamp( commandBuffer, static_cast<VkPipelineStageFlagBits>( pipelineStage ), queryPool, query );
  }

  inline void cmdCopyQueryPoolResults( CommandBuffer commandBuffer, QueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, Buffer dstBuffer, DeviceSize dstOffset, DeviceSize stride, QueryResultFlags flags )
  {
    vkCmdCopyQueryPoolResults( commandBuffer, queryPool, firstQuery, queryCount, dstBuffer, dstOffset, stride, static_cast<VkQueryResultFlags>( flags ) );
  }

  inline void cmdPushConstants( CommandBuffer commandBuffer, PipelineLayout layout, ShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void* pValues )
  {
    vkCmdPushConstants( commandBuffer, layout, static_cast<VkShaderStageFlags>( stageFlags ), offset, size, pValues );
  }

  inline void cmdPushConstants( CommandBuffer commandBuffer, PipelineLayout layout, ShaderStageFlags stageFlags, uint32_t offset, std::vector<uint8_t> const& values )
  {
    cmdPushConstants( commandBuffer, layout, stageFlags, offset, static_cast<uint32_t>( values.size() ), values.data() );
  }

  inline void cmdBeginRenderPass( CommandBuffer commandBuffer, const RenderPassBeginInfo* pRenderPassBegin, SubpassContents contents )
  {
    vkCmdBeginRenderPass( commandBuffer, reinterpret_cast<const VkRenderPassBeginInfo*>( pRenderPassBegin ), static_cast<VkSubpassContents>( contents ) );
  }

  inline void cmdBeginRenderPass( CommandBuffer commandBuffer, const RenderPassBeginInfo& renderPassBegin, SubpassContents contents )
  {
    cmdBeginRenderPass( commandBuffer, &renderPassBegin, contents );
  }

  inline void cmdNextSubpass( CommandBuffer commandBuffer, SubpassContents contents )
  {
    vkCmdNextSubpass( commandBuffer, static_cast<VkSubpassContents>( contents ) );
  }

  inline void cmdEndRenderPass( CommandBuffer commandBuffer )
  {
    vkCmdEndRenderPass( commandBuffer );
  }

  inline void cmdExecuteCommands( CommandBuffer commandBuffer, uint32_t commandBufferCount, const CommandBuffer* pCommandBuffers )
  {
    vkCmdExecuteCommands( commandBuffer, commandBufferCount, pCommandBuffers );
  }

  inline void cmdExecuteCommands( CommandBuffer commandBuffer, std::vector<CommandBuffer> const& commandBuffers )
  {
    cmdExecuteCommands( commandBuffer, static_cast<uint32_t>( commandBuffers.size() ), commandBuffers.data() );
  }

  inline void destroySurfaceKHR( Instance instance, SurfaceKHR surface, const AllocationCallbacks* pAllocator )
  {
    vkDestroySurfaceKHR( instance, surface, reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
  }

  inline void destroySurfaceKHR( Instance instance, SurfaceKHR surface, const AllocationCallbacks& allocator )
  {
    destroySurfaceKHR( instance, surface, &allocator );
  }

  inline Result getPhysicalDeviceSurfaceSupportKHR( PhysicalDevice physicalDevice, uint32_t queueFamilyIndex, SurfaceKHR surface, Bool32* pSupported )
  {
    return static_cast<Result>( VPP_CALL(vkGetPhysicalDeviceSurfaceSupportKHR( physicalDevice, queueFamilyIndex, surface, pSupported )) );
  }

  inline Result getPhysicalDeviceSurfaceSupportKHR( PhysicalDevice physicalDevice, uint32_t queueFamilyIndex, SurfaceKHR surface, Bool32& supported )
  {
    return getPhysicalDeviceSurfaceSupportKHR( physicalDevice, queueFamilyIndex, surface, &supported );
  }

  inline Result getPhysicalDeviceSurfaceCapabilitiesKHR( PhysicalDevice physicalDevice, SurfaceKHR surface, SurfaceCapabilitiesKHR* pSurfaceCapabilities )
  {
    return static_cast<Result>( VPP_CALL(vkGetPhysicalDeviceSurfaceCapabilitiesKHR( physicalDevice, surface, reinterpret_cast<VkSurfaceCapabilitiesKHR*>( pSurfaceCapabilities ) )) );
  }

  inline Result getPhysicalDeviceSurfaceCapabilitiesKHR( PhysicalDevice physicalDevice, SurfaceKHR surface, SurfaceCapabilitiesKHR& surfaceCapabilities )
  {
    return getPhysicalDeviceSurfaceCapabilitiesKHR( physicalDevice, surface, &surfaceCapabilities );
  }

  inline Result getPhysicalDeviceSurfaceFormatsKHR( PhysicalDevice physicalDevice, SurfaceKHR surface, uint32_t* pSurfaceFormatCount, SurfaceFormatKHR* pSurfaceFormats )
  {
    return static_cast<Result>( VPP_CALL(vkGetPhysicalDeviceSurfaceFormatsKHR( physicalDevice, surface, pSurfaceFormatCount, reinterpret_cast<VkSurfaceFormatKHR*>( pSurfaceFormats ) )) );
  }

  inline Result getPhysicalDeviceSurfaceFormatsKHR( PhysicalDevice physicalDevice, SurfaceKHR surface, std::vector<SurfaceFormatKHR> & surfaceFormats )
  {
    uint32_t pSurfaceFormatCount = 0;
    Result result = getPhysicalDeviceSurfaceFormatsKHR( physicalDevice, surface, &pSurfaceFormatCount, nullptr );
    if ( result == Result::Success )
    {
      surfaceFormats.resize( pSurfaceFormatCount );
      result = getPhysicalDeviceSurfaceFormatsKHR( physicalDevice, surface, &pSurfaceFormatCount, surfaceFormats.data() );
    }
	return result;
  }

  inline Result getPhysicalDeviceSurfacePresentModesKHR( PhysicalDevice physicalDevice, SurfaceKHR surface, uint32_t* pPresentModeCount, PresentModeKHR* pPresentModes )
  {
    return static_cast<Result>( VPP_CALL(vkGetPhysicalDeviceSurfacePresentModesKHR( physicalDevice, surface, pPresentModeCount, reinterpret_cast<VkPresentModeKHR*>( pPresentModes ) )) );
  }

  inline Result getPhysicalDeviceSurfacePresentModesKHR( PhysicalDevice physicalDevice, SurfaceKHR surface, std::vector<PresentModeKHR> & presentModes )
  {
    uint32_t pPresentModeCount = 0;
    Result result = getPhysicalDeviceSurfacePresentModesKHR( physicalDevice, surface, &pPresentModeCount, nullptr );
    if ( result == Result::Success )
    {
      presentModes.resize( pPresentModeCount );
      result = getPhysicalDeviceSurfacePresentModesKHR( physicalDevice, surface, &pPresentModeCount, presentModes.data() );
    }
	return result;
  }

  inline Result createSwapchainKHR( Device device, const SwapchainCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, SwapchainKHR* pSwapchain )
  {
    return static_cast<Result>( VPP_CALL(vkCreateSwapchainKHR( device, reinterpret_cast<const VkSwapchainCreateInfoKHR*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), pSwapchain )) );
  }

  inline Result createSwapchainKHR( Device device, const SwapchainCreateInfoKHR& createInfo, const AllocationCallbacks& allocator, SwapchainKHR& swapchain )
  {
    return createSwapchainKHR( device, &createInfo, &allocator, &swapchain );
  }

  inline void destroySwapchainKHR( Device device, SwapchainKHR swapchain, const AllocationCallbacks* pAllocator )
  {
    vkDestroySwapchainKHR( device, swapchain, reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
  }

  inline void destroySwapchainKHR( Device device, SwapchainKHR swapchain, const AllocationCallbacks& allocator )
  {
    destroySwapchainKHR( device, swapchain, &allocator );
  }

  inline Result getSwapchainImagesKHR( Device device, SwapchainKHR swapchain, uint32_t* pSwapchainImageCount, Image* pSwapchainImages )
  {
    return static_cast<Result>( VPP_CALL(vkGetSwapchainImagesKHR( device, swapchain, pSwapchainImageCount, pSwapchainImages )) );
  }

  inline Result getSwapchainImagesKHR( Device device, SwapchainKHR swapchain, std::vector<Image> & swapchainImages )
  {
    uint32_t pSwapchainImageCount = 0;
    Result result = getSwapchainImagesKHR( device, swapchain, &pSwapchainImageCount, nullptr );
    if ( result == Result::Success )
    {
      swapchainImages.resize( pSwapchainImageCount );
      result = getSwapchainImagesKHR( device, swapchain, &pSwapchainImageCount, swapchainImages.data() );
    }
	return result;
  }

  inline Result acquireNextImageKHR( Device device, SwapchainKHR swapchain, uint64_t timeout, Semaphore semaphore, Fence fence, uint32_t* pImageIndex )
  {
    return static_cast<Result>( VPP_CALL(vkAcquireNextImageKHR( device, swapchain, timeout, semaphore, fence, pImageIndex )) );
  }

  inline Result acquireNextImageKHR( Device device, SwapchainKHR swapchain, uint64_t timeout, Semaphore semaphore, Fence fence, uint32_t& imageIndex )
  {
    return acquireNextImageKHR( device, swapchain, timeout, semaphore, fence, &imageIndex );
  }

  inline Result queuePresentKHR( Queue queue, const PresentInfoKHR* pPresentInfo )
  {
    return static_cast<Result>( VPP_CALL(vkQueuePresentKHR( queue, reinterpret_cast<const VkPresentInfoKHR*>( pPresentInfo ) )) );
  }

  inline Result queuePresentKHR( Queue queue, const PresentInfoKHR& presentInfo )
  {
    return queuePresentKHR( queue, &presentInfo );
  }

  inline Result getPhysicalDeviceDisplayPropertiesKHR( PhysicalDevice physicalDevice, uint32_t* pPropertyCount, DisplayPropertiesKHR* pProperties )
  {
    return static_cast<Result>( VPP_CALL(vkGetPhysicalDeviceDisplayPropertiesKHR( physicalDevice, pPropertyCount, reinterpret_cast<VkDisplayPropertiesKHR*>( pProperties ) )) );
  }

  inline Result getPhysicalDeviceDisplayPropertiesKHR( PhysicalDevice physicalDevice, std::vector<DisplayPropertiesKHR> & properties )
  {
    uint32_t pPropertyCount = 0;
    Result result = getPhysicalDeviceDisplayPropertiesKHR( physicalDevice, &pPropertyCount, nullptr );
    if ( result == Result::Success )
    {
      properties.resize( pPropertyCount );
      result = getPhysicalDeviceDisplayPropertiesKHR( physicalDevice, &pPropertyCount, properties.data() );
    }
	return result;
  }

  inline Result getPhysicalDeviceDisplayPlanePropertiesKHR( PhysicalDevice physicalDevice, uint32_t* pPropertyCount, DisplayPlanePropertiesKHR* pProperties )
  {
    return static_cast<Result>( VPP_CALL(vkGetPhysicalDeviceDisplayPlanePropertiesKHR( physicalDevice, pPropertyCount, reinterpret_cast<VkDisplayPlanePropertiesKHR*>( pProperties ) )) );
  }

  inline Result getPhysicalDeviceDisplayPlanePropertiesKHR( PhysicalDevice physicalDevice, std::vector<DisplayPlanePropertiesKHR> & properties )
  {
    uint32_t pPropertyCount = 0;
    Result result = getPhysicalDeviceDisplayPlanePropertiesKHR( physicalDevice, &pPropertyCount, nullptr );
    if ( result == Result::Success )
    {
      properties.resize( pPropertyCount );
      result = getPhysicalDeviceDisplayPlanePropertiesKHR( physicalDevice, &pPropertyCount, properties.data() );
    }
	return result;
  }

  inline Result getDisplayPlaneSupportedDisplaysKHR( PhysicalDevice physicalDevice, uint32_t planeIndex, uint32_t* pDisplayCount, DisplayKHR* pDisplays )
  {
    return static_cast<Result>( VPP_CALL(vkGetDisplayPlaneSupportedDisplaysKHR( physicalDevice, planeIndex, pDisplayCount, pDisplays )) );
  }

  inline Result getDisplayPlaneSupportedDisplaysKHR( PhysicalDevice physicalDevice, uint32_t planeIndex, std::vector<DisplayKHR> & displays )
  {
    uint32_t pDisplayCount = 0;
    Result result = getDisplayPlaneSupportedDisplaysKHR( physicalDevice, planeIndex, &pDisplayCount, nullptr );
    if ( result == Result::Success )
    {
      displays.resize( pDisplayCount );
      result = getDisplayPlaneSupportedDisplaysKHR( physicalDevice, planeIndex, &pDisplayCount, displays.data() );
    }
	return result;
  }

  inline Result getDisplayModePropertiesKHR( PhysicalDevice physicalDevice, DisplayKHR display, uint32_t* pPropertyCount, DisplayModePropertiesKHR* pProperties )
  {
    return static_cast<Result>( VPP_CALL(vkGetDisplayModePropertiesKHR( physicalDevice, display, pPropertyCount, reinterpret_cast<VkDisplayModePropertiesKHR*>( pProperties ) )) );
  }

  inline Result getDisplayModePropertiesKHR( PhysicalDevice physicalDevice, DisplayKHR display, std::vector<DisplayModePropertiesKHR> & properties )
  {
    uint32_t pPropertyCount = 0;
    Result result = getDisplayModePropertiesKHR( physicalDevice, display, &pPropertyCount, nullptr );
    if ( result == Result::Success )
    {
      properties.resize( pPropertyCount );
      result = getDisplayModePropertiesKHR( physicalDevice, display, &pPropertyCount, properties.data() );
    }
	return result;
  }

  inline Result createDisplayModeKHR( PhysicalDevice physicalDevice, DisplayKHR display, const DisplayModeCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, DisplayModeKHR* pMode )
  {
    return static_cast<Result>( VPP_CALL(vkCreateDisplayModeKHR( physicalDevice, display, reinterpret_cast<const VkDisplayModeCreateInfoKHR*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), pMode )) );
  }

  inline Result createDisplayModeKHR( PhysicalDevice physicalDevice, DisplayKHR display, const DisplayModeCreateInfoKHR& createInfo, const AllocationCallbacks& allocator, DisplayModeKHR& mode )
  {
    return createDisplayModeKHR( physicalDevice, display, &createInfo, &allocator, &mode );
  }

  inline Result getDisplayPlaneCapabilitiesKHR( PhysicalDevice physicalDevice, DisplayModeKHR mode, uint32_t planeIndex, DisplayPlaneCapabilitiesKHR* pCapabilities )
  {
    return static_cast<Result>( VPP_CALL(vkGetDisplayPlaneCapabilitiesKHR( physicalDevice, mode, planeIndex, reinterpret_cast<VkDisplayPlaneCapabilitiesKHR*>( pCapabilities ) )) );
  }

  inline Result getDisplayPlaneCapabilitiesKHR( PhysicalDevice physicalDevice, DisplayModeKHR mode, uint32_t planeIndex, DisplayPlaneCapabilitiesKHR& capabilities )
  {
    return getDisplayPlaneCapabilitiesKHR( physicalDevice, mode, planeIndex, &capabilities );
  }

  inline Result createDisplayPlaneSurfaceKHR( Instance instance, const DisplaySurfaceCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceKHR* pSurface )
  {
    return static_cast<Result>( VPP_CALL(vkCreateDisplayPlaneSurfaceKHR( instance, reinterpret_cast<const VkDisplaySurfaceCreateInfoKHR*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), pSurface )) );
  }

  inline Result createDisplayPlaneSurfaceKHR( Instance instance, const DisplaySurfaceCreateInfoKHR& createInfo, const AllocationCallbacks& allocator, SurfaceKHR& surface )
  {
    return createDisplayPlaneSurfaceKHR( instance, &createInfo, &allocator, &surface );
  }

  inline Result createSharedSwapchainsKHR( Device device, uint32_t swapchainCount, const SwapchainCreateInfoKHR* pCreateInfos, const AllocationCallbacks* pAllocator, SwapchainKHR* pSwapchains )
  {
    return static_cast<Result>( VPP_CALL(vkCreateSharedSwapchainsKHR( device, swapchainCount, reinterpret_cast<const VkSwapchainCreateInfoKHR*>( pCreateInfos ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), pSwapchains )) );
  }

  inline Result createSharedSwapchainsKHR( Device device, std::vector<SwapchainCreateInfoKHR> const& createInfos, const AllocationCallbacks& allocator, std::vector<SwapchainKHR> & swapchains )
  {
    assert( createInfos.size() <= swapchains.size() );
    return createSharedSwapchainsKHR( device, static_cast<uint32_t>( createInfos.size() ), createInfos.data(), &allocator, swapchains.data() );
  }

#ifdef VK_USE_PLATFORM_XLIB_KHR

  inline Result createXlibSurfaceKHR( Instance instance, const XlibSurfaceCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceKHR* pSurface )
  {
    return static_cast<Result>( VPP_CALL(vkCreateXlibSurfaceKHR( instance, reinterpret_cast<const VkXlibSurfaceCreateInfoKHR*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), pSurface )) );
  }

  inline Result createXlibSurfaceKHR( Instance instance, const XlibSurfaceCreateInfoKHR& createInfo, const AllocationCallbacks& allocator, SurfaceKHR& surface )
  {
    return createXlibSurfaceKHR( instance, &createInfo, &allocator, &surface );
  }

#endif /* VK_USE_PLATFORM_XLIB_KHR */

#ifdef VK_USE_PLATFORM_XLIB_KHR

  inline Bool32 getPhysicalDeviceXlibPresentationSupportKHR( PhysicalDevice physicalDevice, uint32_t queueFamilyIndex, Display* dpy, VisualID visualID )
  {
    return vkGetPhysicalDeviceXlibPresentationSupportKHR( physicalDevice, queueFamilyIndex, dpy, visualID );
  }

  inline Bool32 getPhysicalDeviceXlibPresentationSupportKHR( PhysicalDevice physicalDevice, uint32_t queueFamilyIndex, Display* dpy, VisualID visualID )
  {
    return getPhysicalDeviceXlibPresentationSupportKHR( physicalDevice, queueFamilyIndex, dpy, visualID );
  }

#endif /* VK_USE_PLATFORM_XLIB_KHR */

#ifdef VK_USE_PLATFORM_XCB_KHR

  inline Result createXcbSurfaceKHR( Instance instance, const XcbSurfaceCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceKHR* pSurface )
  {
    return static_cast<Result>( VPP_CALL(vkCreateXcbSurfaceKHR( instance, reinterpret_cast<const VkXcbSurfaceCreateInfoKHR*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), pSurface )) );
  }

  inline Result createXcbSurfaceKHR( Instance instance, const XcbSurfaceCreateInfoKHR& createInfo, const AllocationCallbacks& allocator, SurfaceKHR& surface )
  {
    return createXcbSurfaceKHR( instance, &createInfo, &allocator, &surface );
  }

#endif /* VK_USE_PLATFORM_XCB_KHR */

#ifdef VK_USE_PLATFORM_XCB_KHR

  inline Bool32 getPhysicalDeviceXcbPresentationSupportKHR( PhysicalDevice physicalDevice, uint32_t queueFamilyIndex, xcb_connection_t* connection, xcb_visualid_t visual_id )
  {
    return vkGetPhysicalDeviceXcbPresentationSupportKHR( physicalDevice, queueFamilyIndex, connection, visual_id );
  }

  inline Bool32 getPhysicalDeviceXcbPresentationSupportKHR( PhysicalDevice physicalDevice, uint32_t queueFamilyIndex, xcb_connection_t* connection, xcb_visualid_t visual_id )
  {
    return getPhysicalDeviceXcbPresentationSupportKHR( physicalDevice, queueFamilyIndex, connection, visual_id );
  }

#endif /* VK_USE_PLATFORM_XCB_KHR */

#ifdef VK_USE_PLATFORM_WAYLAND_KHR

  inline Result createWaylandSurfaceKHR( Instance instance, const WaylandSurfaceCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceKHR* pSurface )
  {
    return static_cast<Result>( VPP_CALL(vkCreateWaylandSurfaceKHR( instance, reinterpret_cast<const VkWaylandSurfaceCreateInfoKHR*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), pSurface )) );
  }

  inline Result createWaylandSurfaceKHR( Instance instance, const WaylandSurfaceCreateInfoKHR& createInfo, const AllocationCallbacks& allocator, SurfaceKHR& surface )
  {
    return createWaylandSurfaceKHR( instance, &createInfo, &allocator, &surface );
  }

#endif /* VK_USE_PLATFORM_WAYLAND_KHR */

#ifdef VK_USE_PLATFORM_WAYLAND_KHR

  inline Bool32 getPhysicalDeviceWaylandPresentationSupportKHR( PhysicalDevice physicalDevice, uint32_t queueFamilyIndex, struct wl_display* display )
  {
    return vkGetPhysicalDeviceWaylandPresentationSupportKHR( physicalDevice, queueFamilyIndex, display );
  }

  inline Bool32 getPhysicalDeviceWaylandPresentationSupportKHR( PhysicalDevice physicalDevice, uint32_t queueFamilyIndex, struct wl_display* display )
  {
    return getPhysicalDeviceWaylandPresentationSupportKHR( physicalDevice, queueFamilyIndex, display );
  }

#endif /* VK_USE_PLATFORM_WAYLAND_KHR */

#ifdef VK_USE_PLATFORM_MIR_KHR

  inline Result createMirSurfaceKHR( Instance instance, const MirSurfaceCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceKHR* pSurface )
  {
    return static_cast<Result>( VPP_CALL(vkCreateMirSurfaceKHR( instance, reinterpret_cast<const VkMirSurfaceCreateInfoKHR*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), pSurface )) );
  }

  inline Result createMirSurfaceKHR( Instance instance, const MirSurfaceCreateInfoKHR& createInfo, const AllocationCallbacks& allocator, SurfaceKHR& surface )
  {
    return createMirSurfaceKHR( instance, &createInfo, &allocator, &surface );
  }

#endif /* VK_USE_PLATFORM_MIR_KHR */

#ifdef VK_USE_PLATFORM_MIR_KHR

  inline Bool32 getPhysicalDeviceMirPresentationSupportKHR( PhysicalDevice physicalDevice, uint32_t queueFamilyIndex, MirConnection* connection )
  {
    return vkGetPhysicalDeviceMirPresentationSupportKHR( physicalDevice, queueFamilyIndex, connection );
  }

  inline Bool32 getPhysicalDeviceMirPresentationSupportKHR( PhysicalDevice physicalDevice, uint32_t queueFamilyIndex, MirConnection* connection )
  {
    return getPhysicalDeviceMirPresentationSupportKHR( physicalDevice, queueFamilyIndex, connection );
  }

#endif /* VK_USE_PLATFORM_MIR_KHR */

#ifdef VK_USE_PLATFORM_ANDROID_KHR

  inline Result createAndroidSurfaceKHR( Instance instance, const AndroidSurfaceCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceKHR* pSurface )
  {
    return static_cast<Result>( VPP_CALL(vkCreateAndroidSurfaceKHR( instance, reinterpret_cast<const VkAndroidSurfaceCreateInfoKHR*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), pSurface )) );
  }

  inline Result createAndroidSurfaceKHR( Instance instance, const AndroidSurfaceCreateInfoKHR& createInfo, const AllocationCallbacks& allocator, SurfaceKHR& surface )
  {
    return createAndroidSurfaceKHR( instance, &createInfo, &allocator, &surface );
  }

#endif /* VK_USE_PLATFORM_ANDROID_KHR */

#ifdef VK_USE_PLATFORM_WIN32_KHR

  inline Result createWin32SurfaceKHR( Instance instance, const Win32SurfaceCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceKHR* pSurface )
  {
    return static_cast<Result>( VPP_CALL(vkCreateWin32SurfaceKHR( instance, reinterpret_cast<const VkWin32SurfaceCreateInfoKHR*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), pSurface )) );
  }

  inline Result createWin32SurfaceKHR( Instance instance, const Win32SurfaceCreateInfoKHR& createInfo, const AllocationCallbacks& allocator, SurfaceKHR& surface )
  {
    return createWin32SurfaceKHR( instance, &createInfo, &allocator, &surface );
  }

#endif /* VK_USE_PLATFORM_WIN32_KHR */

#ifdef VK_USE_PLATFORM_WIN32_KHR

  inline Bool32 getPhysicalDeviceWin32PresentationSupportKHR( PhysicalDevice physicalDevice, uint32_t queueFamilyIndex )
  {
    return vkGetPhysicalDeviceWin32PresentationSupportKHR( physicalDevice, queueFamilyIndex );
  }

#endif /* VK_USE_PLATFORM_WIN32_KHR */

  inline Result createDebugReportCallbackEXT( Instance instance, const DebugReportCallbackCreateInfoEXT* pCreateInfo, const AllocationCallbacks* pAllocator, DebugReportCallbackEXT* pCallback )
  {
    return static_cast<Result>( VPP_CALL(vkCreateDebugReportCallbackEXT( instance, reinterpret_cast<const VkDebugReportCallbackCreateInfoEXT*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), pCallback )) );
  }

  inline Result createDebugReportCallbackEXT( Instance instance, const DebugReportCallbackCreateInfoEXT& createInfo, const AllocationCallbacks& allocator, DebugReportCallbackEXT& callback )
  {
    return createDebugReportCallbackEXT( instance, &createInfo, &allocator, &callback );
  }

  inline void destroyDebugReportCallbackEXT( Instance instance, DebugReportCallbackEXT callback, const AllocationCallbacks* pAllocator )
  {
    vkDestroyDebugReportCallbackEXT( instance, callback, reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
  }

  inline void destroyDebugReportCallbackEXT( Instance instance, DebugReportCallbackEXT callback, const AllocationCallbacks& allocator )
  {
    destroyDebugReportCallbackEXT( instance, callback, &allocator );
  }

  inline void debugReportMessageEXT( Instance instance, DebugReportFlagsEXT flags, DebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char* pLayerPrefix, const char* pMessage )
  {
    vkDebugReportMessageEXT( instance, static_cast<VkDebugReportFlagsEXT>( flags ), static_cast<VkDebugReportObjectTypeEXT>( objectType ), object, location, messageCode, pLayerPrefix, pMessage );
  }

  inline void debugReportMessageEXT( Instance instance, DebugReportFlagsEXT flags, DebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char& layerPrefix, const char& message )
  {
    debugReportMessageEXT( instance, flags, objectType, object, location, messageCode, &layerPrefix, &message );
  }

}
