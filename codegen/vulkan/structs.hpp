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

namespace vk
{
  class Offset2D
  {
  public:
    Offset2D() : Offset2D(0, 0) {}
    Offset2D(int32_t x, int32_t y) {m_offset2D.x = x; m_offset2D.y = y; }
    Offset2D(const VkOffset2D& other) : m_offset2D{other} {}
    Offset2D& operator=(const VkOffset2D& other){ m_offset2D = other; return *this; }

    const int32_t& x() const { return m_offset2D.x; }
    Offset2D& x(int32_t x){ m_offset2D.x = x; return *this; }
    const int32_t& y() const { return m_offset2D.y; }
    Offset2D& y(int32_t y){ m_offset2D.y = y; return *this; }
    operator VkOffset2D&() { return m_offset2D; }
    operator const VkOffset2D&() const { return m_offset2D; }
    VkOffset2D& vkHandle() { return m_offset2D; }
    const VkOffset2D& vkHandle() const { return m_offset2D; }

  private:
    VkOffset2D m_offset2D;
  };

  class Offset3D
  {
  public:
    Offset3D() : Offset3D(0, 0, 0) {}
    Offset3D(int32_t x, int32_t y, int32_t z) {m_offset3D.x = x; m_offset3D.y = y; m_offset3D.z = z; }
    Offset3D(const VkOffset3D& other) : m_offset3D{other} {}
    Offset3D& operator=(const VkOffset3D& other){ m_offset3D = other; return *this; }

    const int32_t& x() const { return m_offset3D.x; }
    Offset3D& x(int32_t x){ m_offset3D.x = x; return *this; }
    const int32_t& y() const { return m_offset3D.y; }
    Offset3D& y(int32_t y){ m_offset3D.y = y; return *this; }
    const int32_t& z() const { return m_offset3D.z; }
    Offset3D& z(int32_t z){ m_offset3D.z = z; return *this; }
    operator VkOffset3D&() { return m_offset3D; }
    operator const VkOffset3D&() const { return m_offset3D; }
    VkOffset3D& vkHandle() { return m_offset3D; }
    const VkOffset3D& vkHandle() const { return m_offset3D; }

  private:
    VkOffset3D m_offset3D;
  };

  class Extent2D
  {
  public:
    Extent2D() : Extent2D(0, 0) {}
    Extent2D(uint32_t width, uint32_t height) {m_extent2D.width = width; m_extent2D.height = height; }
    Extent2D(const VkExtent2D& other) : m_extent2D{other} {}
    Extent2D& operator=(const VkExtent2D& other){ m_extent2D = other; return *this; }

    const uint32_t& width() const { return m_extent2D.width; }
    Extent2D& width(uint32_t width){ m_extent2D.width = width; return *this; }
    const uint32_t& height() const { return m_extent2D.height; }
    Extent2D& height(uint32_t height){ m_extent2D.height = height; return *this; }
    operator VkExtent2D&() { return m_extent2D; }
    operator const VkExtent2D&() const { return m_extent2D; }
    VkExtent2D& vkHandle() { return m_extent2D; }
    const VkExtent2D& vkHandle() const { return m_extent2D; }

  private:
    VkExtent2D m_extent2D;
  };

  class Extent3D
  {
  public:
    Extent3D() : Extent3D(0, 0, 0) {}
    Extent3D(uint32_t width, uint32_t height, uint32_t depth) {m_extent3D.width = width; m_extent3D.height = height; m_extent3D.depth = depth; }
    Extent3D(const VkExtent3D& other) : m_extent3D{other} {}
    Extent3D& operator=(const VkExtent3D& other){ m_extent3D = other; return *this; }

    const uint32_t& width() const { return m_extent3D.width; }
    Extent3D& width(uint32_t width){ m_extent3D.width = width; return *this; }
    const uint32_t& height() const { return m_extent3D.height; }
    Extent3D& height(uint32_t height){ m_extent3D.height = height; return *this; }
    const uint32_t& depth() const { return m_extent3D.depth; }
    Extent3D& depth(uint32_t depth){ m_extent3D.depth = depth; return *this; }
    operator VkExtent3D&() { return m_extent3D; }
    operator const VkExtent3D&() const { return m_extent3D; }
    VkExtent3D& vkHandle() { return m_extent3D; }
    const VkExtent3D& vkHandle() const { return m_extent3D; }

  private:
    VkExtent3D m_extent3D;
  };

  class Viewport
  {
  public:
    Viewport() : Viewport(0, 0, 0, 0, 0, 0) {}
    Viewport(float x, float y, float width, float height, float minDepth, float maxDepth) {m_viewport.x = x; m_viewport.y = y; m_viewport.width = width; m_viewport.height = height; m_viewport.minDepth = minDepth; m_viewport.maxDepth = maxDepth; }
    Viewport(const VkViewport& other) : m_viewport{other} {}
    Viewport& operator=(const VkViewport& other){ m_viewport = other; return *this; }

    const float& x() const { return m_viewport.x; }
    Viewport& x(float x){ m_viewport.x = x; return *this; }
    const float& y() const { return m_viewport.y; }
    Viewport& y(float y){ m_viewport.y = y; return *this; }
    const float& width() const { return m_viewport.width; }
    Viewport& width(float width){ m_viewport.width = width; return *this; }
    const float& height() const { return m_viewport.height; }
    Viewport& height(float height){ m_viewport.height = height; return *this; }
    const float& minDepth() const { return m_viewport.minDepth; }
    Viewport& minDepth(float minDepth){ m_viewport.minDepth = minDepth; return *this; }
    const float& maxDepth() const { return m_viewport.maxDepth; }
    Viewport& maxDepth(float maxDepth){ m_viewport.maxDepth = maxDepth; return *this; }
    operator VkViewport&() { return m_viewport; }
    operator const VkViewport&() const { return m_viewport; }
    VkViewport& vkHandle() { return m_viewport; }
    const VkViewport& vkHandle() const { return m_viewport; }

  private:
    VkViewport m_viewport;
  };

  class Rect2D
  {
  public:
    Rect2D() : Rect2D(Offset2D(), Extent2D()) {}
    Rect2D(Offset2D offset, Extent2D extent) {m_rect2D.offset = static_cast<VkOffset2D>( offset ); m_rect2D.extent = static_cast<VkExtent2D>( extent ); }
    Rect2D(const VkRect2D& other) : m_rect2D{other} {}
    Rect2D& operator=(const VkRect2D& other){ m_rect2D = other; return *this; }

    const Offset2D& offset() const { return reinterpret_cast<const Offset2D&>( m_rect2D.offset ); }
    Rect2D& offset(Offset2D offset){ m_rect2D.offset = static_cast<VkOffset2D>( offset ); return *this; }
    const Extent2D& extent() const { return reinterpret_cast<const Extent2D&>( m_rect2D.extent ); }
    Rect2D& extent(Extent2D extent){ m_rect2D.extent = static_cast<VkExtent2D>( extent ); return *this; }
    operator VkRect2D&() { return m_rect2D; }
    operator const VkRect2D&() const { return m_rect2D; }
    VkRect2D& vkHandle() { return m_rect2D; }
    const VkRect2D& vkHandle() const { return m_rect2D; }

  private:
    VkRect2D m_rect2D;
  };

  class ClearRect
  {
  public:
    ClearRect() : ClearRect(Rect2D(), 0, 0) {}
    ClearRect(Rect2D rect, uint32_t baseArrayLayer, uint32_t layerCount) {m_clearRect.rect = static_cast<VkRect2D>( rect ); m_clearRect.baseArrayLayer = baseArrayLayer; m_clearRect.layerCount = layerCount; }
    ClearRect(const VkClearRect& other) : m_clearRect{other} {}
    ClearRect& operator=(const VkClearRect& other){ m_clearRect = other; return *this; }

    const Rect2D& rect() const { return reinterpret_cast<const Rect2D&>( m_clearRect.rect ); }
    ClearRect& rect(Rect2D rect){ m_clearRect.rect = static_cast<VkRect2D>( rect ); return *this; }
    const uint32_t& baseArrayLayer() const { return m_clearRect.baseArrayLayer; }
    ClearRect& baseArrayLayer(uint32_t baseArrayLayer){ m_clearRect.baseArrayLayer = baseArrayLayer; return *this; }
    const uint32_t& layerCount() const { return m_clearRect.layerCount; }
    ClearRect& layerCount(uint32_t layerCount){ m_clearRect.layerCount = layerCount; return *this; }
    operator VkClearRect&() { return m_clearRect; }
    operator const VkClearRect&() const { return m_clearRect; }
    VkClearRect& vkHandle() { return m_clearRect; }
    const VkClearRect& vkHandle() const { return m_clearRect; }

  private:
    VkClearRect m_clearRect;
  };

  class ExtensionProperties
  {
  public:
    const char* extensionName() const { return reinterpret_cast<const char*>( m_extensionProperties.extensionName ); }
    const uint32_t& specVersion() const { return m_extensionProperties.specVersion; }
    operator VkExtensionProperties&() { return m_extensionProperties; }
    operator const VkExtensionProperties&() const { return m_extensionProperties; }
    VkExtensionProperties& vkHandle() { return m_extensionProperties; }
    const VkExtensionProperties& vkHandle() const { return m_extensionProperties; }

  private:
    VkExtensionProperties m_extensionProperties;
  };

  class LayerProperties
  {
  public:
    const char* layerName() const { return reinterpret_cast<const char*>( m_layerProperties.layerName ); }
    const uint32_t& specVersion() const { return m_layerProperties.specVersion; }
    const uint32_t& implementationVersion() const { return m_layerProperties.implementationVersion; }
    const char* description() const { return reinterpret_cast<const char*>( m_layerProperties.description ); }
    operator VkLayerProperties&() { return m_layerProperties; }
    operator const VkLayerProperties&() const { return m_layerProperties; }
    VkLayerProperties& vkHandle() { return m_layerProperties; }
    const VkLayerProperties& vkHandle() const { return m_layerProperties; }

  private:
    VkLayerProperties m_layerProperties;
  };

  class AllocationCallbacks
  {
  public:
    AllocationCallbacks(void* pUserData, PFN_vkAllocationFunction pfnAllocation, PFN_vkReallocationFunction pfnReallocation, PFN_vkFreeFunction pfnFree, PFN_vkInternalAllocationNotification pfnInternalAllocation, PFN_vkInternalFreeNotification pfnInternalFree) {m_allocationCallbacks.pUserData = pUserData; m_allocationCallbacks.pfnAllocation = pfnAllocation; m_allocationCallbacks.pfnReallocation = pfnReallocation; m_allocationCallbacks.pfnFree = pfnFree; m_allocationCallbacks.pfnInternalAllocation = pfnInternalAllocation; m_allocationCallbacks.pfnInternalFree = pfnInternalFree; }
    AllocationCallbacks(const VkAllocationCallbacks& other) : m_allocationCallbacks{other} {}
    AllocationCallbacks& operator=(const VkAllocationCallbacks& other){ m_allocationCallbacks = other; return *this; }

    const void* pUserData() const { return reinterpret_cast<void*>( m_allocationCallbacks.pUserData ); }
    AllocationCallbacks& pUserData(void* pUserData){ m_allocationCallbacks.pUserData = pUserData; return *this; }
    const PFN_vkAllocationFunction& pfnAllocation() const { return m_allocationCallbacks.pfnAllocation; }
    AllocationCallbacks& pfnAllocation(PFN_vkAllocationFunction pfnAllocation){ m_allocationCallbacks.pfnAllocation = pfnAllocation; return *this; }
    const PFN_vkReallocationFunction& pfnReallocation() const { return m_allocationCallbacks.pfnReallocation; }
    AllocationCallbacks& pfnReallocation(PFN_vkReallocationFunction pfnReallocation){ m_allocationCallbacks.pfnReallocation = pfnReallocation; return *this; }
    const PFN_vkFreeFunction& pfnFree() const { return m_allocationCallbacks.pfnFree; }
    AllocationCallbacks& pfnFree(PFN_vkFreeFunction pfnFree){ m_allocationCallbacks.pfnFree = pfnFree; return *this; }
    const PFN_vkInternalAllocationNotification& pfnInternalAllocation() const { return m_allocationCallbacks.pfnInternalAllocation; }
    AllocationCallbacks& pfnInternalAllocation(PFN_vkInternalAllocationNotification pfnInternalAllocation){ m_allocationCallbacks.pfnInternalAllocation = pfnInternalAllocation; return *this; }
    const PFN_vkInternalFreeNotification& pfnInternalFree() const { return m_allocationCallbacks.pfnInternalFree; }
    AllocationCallbacks& pfnInternalFree(PFN_vkInternalFreeNotification pfnInternalFree){ m_allocationCallbacks.pfnInternalFree = pfnInternalFree; return *this; }
    operator VkAllocationCallbacks&() { return m_allocationCallbacks; }
    operator const VkAllocationCallbacks&() const { return m_allocationCallbacks; }
    VkAllocationCallbacks& vkHandle() { return m_allocationCallbacks; }
    const VkAllocationCallbacks& vkHandle() const { return m_allocationCallbacks; }

  private:
    VkAllocationCallbacks m_allocationCallbacks;
  };

  class MemoryRequirements
  {
  public:
    const DeviceSize& size() const { return m_memoryRequirements.size; }
    const DeviceSize& alignment() const { return m_memoryRequirements.alignment; }
    const uint32_t& memoryTypeBits() const { return m_memoryRequirements.memoryTypeBits; }
    operator VkMemoryRequirements&() { return m_memoryRequirements; }
    operator const VkMemoryRequirements&() const { return m_memoryRequirements; }
    VkMemoryRequirements& vkHandle() { return m_memoryRequirements; }
    const VkMemoryRequirements& vkHandle() const { return m_memoryRequirements; }

  private:
    VkMemoryRequirements m_memoryRequirements;
  };

  class DescriptorBufferInfo
  {
  public:
    DescriptorBufferInfo() : DescriptorBufferInfo(Buffer(), 0, 0) {}
    DescriptorBufferInfo(Buffer buffer, DeviceSize offset, DeviceSize range) {m_descriptorBufferInfo.buffer = buffer; m_descriptorBufferInfo.offset = offset; m_descriptorBufferInfo.range = range; }
    DescriptorBufferInfo(const VkDescriptorBufferInfo& other) : m_descriptorBufferInfo{other} {}
    DescriptorBufferInfo& operator=(const VkDescriptorBufferInfo& other){ m_descriptorBufferInfo = other; return *this; }

    const Buffer& buffer() const { return m_descriptorBufferInfo.buffer; }
    DescriptorBufferInfo& buffer(Buffer buffer){ m_descriptorBufferInfo.buffer = buffer; return *this; }
    const DeviceSize& offset() const { return m_descriptorBufferInfo.offset; }
    DescriptorBufferInfo& offset(DeviceSize offset){ m_descriptorBufferInfo.offset = offset; return *this; }
    const DeviceSize& range() const { return m_descriptorBufferInfo.range; }
    DescriptorBufferInfo& range(DeviceSize range){ m_descriptorBufferInfo.range = range; return *this; }
    operator VkDescriptorBufferInfo&() { return m_descriptorBufferInfo; }
    operator const VkDescriptorBufferInfo&() const { return m_descriptorBufferInfo; }
    VkDescriptorBufferInfo& vkHandle() { return m_descriptorBufferInfo; }
    const VkDescriptorBufferInfo& vkHandle() const { return m_descriptorBufferInfo; }

  private:
    VkDescriptorBufferInfo m_descriptorBufferInfo;
  };

  class SubresourceLayout
  {
  public:
    SubresourceLayout() : SubresourceLayout(0, 0, 0, 0, 0) {}
    SubresourceLayout(DeviceSize offset, DeviceSize size, DeviceSize rowPitch, DeviceSize arrayPitch, DeviceSize depthPitch) {m_subresourceLayout.offset = offset; m_subresourceLayout.size = size; m_subresourceLayout.rowPitch = rowPitch; m_subresourceLayout.arrayPitch = arrayPitch; m_subresourceLayout.depthPitch = depthPitch; }
    SubresourceLayout(const VkSubresourceLayout& other) : m_subresourceLayout{other} {}
    SubresourceLayout& operator=(const VkSubresourceLayout& other){ m_subresourceLayout = other; return *this; }

    const DeviceSize& offset() const { return m_subresourceLayout.offset; }
    SubresourceLayout& offset(DeviceSize offset){ m_subresourceLayout.offset = offset; return *this; }
    const DeviceSize& size() const { return m_subresourceLayout.size; }
    SubresourceLayout& size(DeviceSize size){ m_subresourceLayout.size = size; return *this; }
    const DeviceSize& rowPitch() const { return m_subresourceLayout.rowPitch; }
    SubresourceLayout& rowPitch(DeviceSize rowPitch){ m_subresourceLayout.rowPitch = rowPitch; return *this; }
    const DeviceSize& arrayPitch() const { return m_subresourceLayout.arrayPitch; }
    SubresourceLayout& arrayPitch(DeviceSize arrayPitch){ m_subresourceLayout.arrayPitch = arrayPitch; return *this; }
    const DeviceSize& depthPitch() const { return m_subresourceLayout.depthPitch; }
    SubresourceLayout& depthPitch(DeviceSize depthPitch){ m_subresourceLayout.depthPitch = depthPitch; return *this; }
    operator VkSubresourceLayout&() { return m_subresourceLayout; }
    operator const VkSubresourceLayout&() const { return m_subresourceLayout; }
    VkSubresourceLayout& vkHandle() { return m_subresourceLayout; }
    const VkSubresourceLayout& vkHandle() const { return m_subresourceLayout; }

  private:
    VkSubresourceLayout m_subresourceLayout;
  };

  class BufferCopy
  {
  public:
    BufferCopy() : BufferCopy(0, 0, 0) {}
    BufferCopy(DeviceSize srcOffset, DeviceSize dstOffset, DeviceSize size) {m_bufferCopy.srcOffset = srcOffset; m_bufferCopy.dstOffset = dstOffset; m_bufferCopy.size = size; }
    BufferCopy(const VkBufferCopy& other) : m_bufferCopy{other} {}
    BufferCopy& operator=(const VkBufferCopy& other){ m_bufferCopy = other; return *this; }

    const DeviceSize& srcOffset() const { return m_bufferCopy.srcOffset; }
    BufferCopy& srcOffset(DeviceSize srcOffset){ m_bufferCopy.srcOffset = srcOffset; return *this; }
    const DeviceSize& dstOffset() const { return m_bufferCopy.dstOffset; }
    BufferCopy& dstOffset(DeviceSize dstOffset){ m_bufferCopy.dstOffset = dstOffset; return *this; }
    const DeviceSize& size() const { return m_bufferCopy.size; }
    BufferCopy& size(DeviceSize size){ m_bufferCopy.size = size; return *this; }
    operator VkBufferCopy&() { return m_bufferCopy; }
    operator const VkBufferCopy&() const { return m_bufferCopy; }
    VkBufferCopy& vkHandle() { return m_bufferCopy; }
    const VkBufferCopy& vkHandle() const { return m_bufferCopy; }

  private:
    VkBufferCopy m_bufferCopy;
  };

  class SpecializationMapEntry
  {
  public:
    SpecializationMapEntry() : SpecializationMapEntry(0, 0, 0) {}
    SpecializationMapEntry(uint32_t constantID, uint32_t offset, size_t size) {m_specializationMapEntry.constantID = constantID; m_specializationMapEntry.offset = offset; m_specializationMapEntry.size = size; }
    SpecializationMapEntry(const VkSpecializationMapEntry& other) : m_specializationMapEntry{other} {}
    SpecializationMapEntry& operator=(const VkSpecializationMapEntry& other){ m_specializationMapEntry = other; return *this; }

    const uint32_t& constantID() const { return m_specializationMapEntry.constantID; }
    SpecializationMapEntry& constantID(uint32_t constantID){ m_specializationMapEntry.constantID = constantID; return *this; }
    const uint32_t& offset() const { return m_specializationMapEntry.offset; }
    SpecializationMapEntry& offset(uint32_t offset){ m_specializationMapEntry.offset = offset; return *this; }
    const size_t& size() const { return m_specializationMapEntry.size; }
    SpecializationMapEntry& size(size_t size){ m_specializationMapEntry.size = size; return *this; }
    operator VkSpecializationMapEntry&() { return m_specializationMapEntry; }
    operator const VkSpecializationMapEntry&() const { return m_specializationMapEntry; }
    VkSpecializationMapEntry& vkHandle() { return m_specializationMapEntry; }
    const VkSpecializationMapEntry& vkHandle() const { return m_specializationMapEntry; }

  private:
    VkSpecializationMapEntry m_specializationMapEntry;
  };

  class SpecializationInfo
  {
  public:
    SpecializationInfo() : SpecializationInfo(0, nullptr, 0, nullptr) {}
    SpecializationInfo(uint32_t mapEntryCount, const SpecializationMapEntry* pMapEntries, size_t dataSize, const void* pData) {m_specializationInfo.mapEntryCount = mapEntryCount; m_specializationInfo.pMapEntries = reinterpret_cast<const VkSpecializationMapEntry*>( pMapEntries ); m_specializationInfo.dataSize = dataSize; m_specializationInfo.pData = pData; }
    SpecializationInfo(const VkSpecializationInfo& other) : m_specializationInfo{other} {}
    SpecializationInfo& operator=(const VkSpecializationInfo& other){ m_specializationInfo = other; return *this; }

    const uint32_t& mapEntryCount() const { return m_specializationInfo.mapEntryCount; }
    SpecializationInfo& mapEntryCount(uint32_t mapEntryCount){ m_specializationInfo.mapEntryCount = mapEntryCount; return *this; }
    const SpecializationMapEntry* pMapEntries() const { return reinterpret_cast<const SpecializationMapEntry*>( m_specializationInfo.pMapEntries ); }
    SpecializationInfo& pMapEntries(const SpecializationMapEntry* pMapEntries){ m_specializationInfo.pMapEntries = reinterpret_cast<const VkSpecializationMapEntry*>( pMapEntries ); return *this; }
    const size_t& dataSize() const { return m_specializationInfo.dataSize; }
    SpecializationInfo& dataSize(size_t dataSize){ m_specializationInfo.dataSize = dataSize; return *this; }
    const void* pData() const { return reinterpret_cast<const void*>( m_specializationInfo.pData ); }
    SpecializationInfo& pData(const void* pData){ m_specializationInfo.pData = pData; return *this; }
    operator VkSpecializationInfo&() { return m_specializationInfo; }
    operator const VkSpecializationInfo&() const { return m_specializationInfo; }
    VkSpecializationInfo& vkHandle() { return m_specializationInfo; }
    const VkSpecializationInfo& vkHandle() const { return m_specializationInfo; }

  private:
    VkSpecializationInfo m_specializationInfo;
  };

  class ClearColorValue
  {
  public:
    ClearColorValue( const std::array<float,4>& float32 = { 0 }){ memcpy(&m_clearColorValue.float32, float32.data(), 4 * sizeof( float)); }
    const float* float32() const { return reinterpret_cast<const float*>( m_clearColorValue.float32 ); }
    ClearColorValue& float32(std::array<float,4> float32){ memcpy(&m_clearColorValue.float32, float32.data(), 4 * sizeof(float)); return *this; }
    ClearColorValue( const std::array<int32_t,4>& int32){ memcpy(&m_clearColorValue.int32, int32.data(), 4 * sizeof( int32_t)); }
    const int32_t* int32() const { return reinterpret_cast<const int32_t*>( m_clearColorValue.int32 ); }
    ClearColorValue& int32(std::array<int32_t,4> int32){ memcpy(&m_clearColorValue.int32, int32.data(), 4 * sizeof(int32_t)); return *this; }
    ClearColorValue( const std::array<uint32_t,4>& uint32){ memcpy(&m_clearColorValue.uint32, uint32.data(), 4 * sizeof( uint32_t)); }
    const uint32_t* uint32() const { return reinterpret_cast<const uint32_t*>( m_clearColorValue.uint32 ); }
    ClearColorValue& uint32(std::array<uint32_t,4> uint32){ memcpy(&m_clearColorValue.uint32, uint32.data(), 4 * sizeof(uint32_t)); return *this; }
    operator VkClearColorValue&() { return m_clearColorValue; }
    operator const VkClearColorValue&() const { return m_clearColorValue; }
    VkClearColorValue& vkHandle() { return m_clearColorValue; }
    const VkClearColorValue& vkHandle() const { return m_clearColorValue; }

  private:
    VkClearColorValue m_clearColorValue;
  };

  class ClearDepthStencilValue
  {
  public:
    ClearDepthStencilValue() : ClearDepthStencilValue(0, 0) {}
    ClearDepthStencilValue(float depth, uint32_t stencil) {m_clearDepthStencilValue.depth = depth; m_clearDepthStencilValue.stencil = stencil; }
    ClearDepthStencilValue(const VkClearDepthStencilValue& other) : m_clearDepthStencilValue{other} {}
    ClearDepthStencilValue& operator=(const VkClearDepthStencilValue& other){ m_clearDepthStencilValue = other; return *this; }

    const float& depth() const { return m_clearDepthStencilValue.depth; }
    ClearDepthStencilValue& depth(float depth){ m_clearDepthStencilValue.depth = depth; return *this; }
    const uint32_t& stencil() const { return m_clearDepthStencilValue.stencil; }
    ClearDepthStencilValue& stencil(uint32_t stencil){ m_clearDepthStencilValue.stencil = stencil; return *this; }
    operator VkClearDepthStencilValue&() { return m_clearDepthStencilValue; }
    operator const VkClearDepthStencilValue&() const { return m_clearDepthStencilValue; }
    VkClearDepthStencilValue& vkHandle() { return m_clearDepthStencilValue; }
    const VkClearDepthStencilValue& vkHandle() const { return m_clearDepthStencilValue; }

  private:
    VkClearDepthStencilValue m_clearDepthStencilValue;
  };

  class ClearValue
  {
  public:
    ClearValue( ClearColorValue color = ClearColorValue()){ m_clearValue.color = color; }
    const ClearColorValue& color() const { return reinterpret_cast<const ClearColorValue&>( m_clearValue.color ); }
    ClearValue& color(ClearColorValue color){ m_clearValue.color = static_cast<VkClearColorValue>( color ); return *this; }
    ClearValue( ClearDepthStencilValue depthStencil){ m_clearValue.depthStencil = depthStencil; }
    const ClearDepthStencilValue& depthStencil() const { return reinterpret_cast<const ClearDepthStencilValue&>( m_clearValue.depthStencil ); }
    ClearValue& depthStencil(ClearDepthStencilValue depthStencil){ m_clearValue.depthStencil = static_cast<VkClearDepthStencilValue>( depthStencil ); return *this; }
    operator VkClearValue&() { return m_clearValue; }
    operator const VkClearValue&() const { return m_clearValue; }
    VkClearValue& vkHandle() { return m_clearValue; }
    const VkClearValue& vkHandle() const { return m_clearValue; }

  private:
    VkClearValue m_clearValue;
  };

  class PhysicalDeviceFeatures
  {
  public:
    PhysicalDeviceFeatures() : PhysicalDeviceFeatures(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0) {}
    PhysicalDeviceFeatures(Bool32 robustBufferAccess, Bool32 fullDrawIndexUint32, Bool32 imageCubeArray, Bool32 independentBlend, Bool32 geometryShader, Bool32 tessellationShader, Bool32 sampleRateShading, Bool32 dualSrcBlend, Bool32 logicOp, Bool32 multiDrawIndirect, Bool32 drawIndirectFirstInstance, Bool32 depthClamp, Bool32 depthBiasClamp, Bool32 fillModeNonSolid, Bool32 depthBounds, Bool32 wideLines, Bool32 largePoints, Bool32 alphaToOne, Bool32 multiViewport, Bool32 samplerAnisotropy, Bool32 textureCompressionETC2, Bool32 textureCompressionASTC_LDR, Bool32 textureCompressionBC, Bool32 occlusionQueryPrecise, Bool32 pipelineStatisticsQuery, Bool32 vertexPipelineStoresAndAtomics, Bool32 fragmentStoresAndAtomics, Bool32 shaderTessellationAndGeometryPointSize, Bool32 shaderImageGatherExtended, Bool32 shaderStorageImageExtendedFormats, Bool32 shaderStorageImageMultisample, Bool32 shaderStorageImageReadWithoutFormat, Bool32 shaderStorageImageWriteWithoutFormat, Bool32 shaderUniformBufferArrayDynamicIndexing, Bool32 shaderSampledImageArrayDynamicIndexing, Bool32 shaderStorageBufferArrayDynamicIndexing, Bool32 shaderStorageImageArrayDynamicIndexing, Bool32 shaderClipDistance, Bool32 shaderCullDistance, Bool32 shaderFloat64, Bool32 shaderInt64, Bool32 shaderInt16, Bool32 shaderResourceResidency, Bool32 shaderResourceMinLod, Bool32 sparseBinding, Bool32 sparseResidencyBuffer, Bool32 sparseResidencyImage2D, Bool32 sparseResidencyImage3D, Bool32 sparseResidency2Samples, Bool32 sparseResidency4Samples, Bool32 sparseResidency8Samples, Bool32 sparseResidency16Samples, Bool32 sparseResidencyAliased, Bool32 variableMultisampleRate, Bool32 inheritedQueries) {m_physicalDeviceFeatures.robustBufferAccess = robustBufferAccess; m_physicalDeviceFeatures.fullDrawIndexUint32 = fullDrawIndexUint32; m_physicalDeviceFeatures.imageCubeArray = imageCubeArray; m_physicalDeviceFeatures.independentBlend = independentBlend; m_physicalDeviceFeatures.geometryShader = geometryShader; m_physicalDeviceFeatures.tessellationShader = tessellationShader; m_physicalDeviceFeatures.sampleRateShading = sampleRateShading; m_physicalDeviceFeatures.dualSrcBlend = dualSrcBlend; m_physicalDeviceFeatures.logicOp = logicOp; m_physicalDeviceFeatures.multiDrawIndirect = multiDrawIndirect; m_physicalDeviceFeatures.drawIndirectFirstInstance = drawIndirectFirstInstance; m_physicalDeviceFeatures.depthClamp = depthClamp; m_physicalDeviceFeatures.depthBiasClamp = depthBiasClamp; m_physicalDeviceFeatures.fillModeNonSolid = fillModeNonSolid; m_physicalDeviceFeatures.depthBounds = depthBounds; m_physicalDeviceFeatures.wideLines = wideLines; m_physicalDeviceFeatures.largePoints = largePoints; m_physicalDeviceFeatures.alphaToOne = alphaToOne; m_physicalDeviceFeatures.multiViewport = multiViewport; m_physicalDeviceFeatures.samplerAnisotropy = samplerAnisotropy; m_physicalDeviceFeatures.textureCompressionETC2 = textureCompressionETC2; m_physicalDeviceFeatures.textureCompressionASTC_LDR = textureCompressionASTC_LDR; m_physicalDeviceFeatures.textureCompressionBC = textureCompressionBC; m_physicalDeviceFeatures.occlusionQueryPrecise = occlusionQueryPrecise; m_physicalDeviceFeatures.pipelineStatisticsQuery = pipelineStatisticsQuery; m_physicalDeviceFeatures.vertexPipelineStoresAndAtomics = vertexPipelineStoresAndAtomics; m_physicalDeviceFeatures.fragmentStoresAndAtomics = fragmentStoresAndAtomics; m_physicalDeviceFeatures.shaderTessellationAndGeometryPointSize = shaderTessellationAndGeometryPointSize; m_physicalDeviceFeatures.shaderImageGatherExtended = shaderImageGatherExtended; m_physicalDeviceFeatures.shaderStorageImageExtendedFormats = shaderStorageImageExtendedFormats; m_physicalDeviceFeatures.shaderStorageImageMultisample = shaderStorageImageMultisample; m_physicalDeviceFeatures.shaderStorageImageReadWithoutFormat = shaderStorageImageReadWithoutFormat; m_physicalDeviceFeatures.shaderStorageImageWriteWithoutFormat = shaderStorageImageWriteWithoutFormat; m_physicalDeviceFeatures.shaderUniformBufferArrayDynamicIndexing = shaderUniformBufferArrayDynamicIndexing; m_physicalDeviceFeatures.shaderSampledImageArrayDynamicIndexing = shaderSampledImageArrayDynamicIndexing; m_physicalDeviceFeatures.shaderStorageBufferArrayDynamicIndexing = shaderStorageBufferArrayDynamicIndexing; m_physicalDeviceFeatures.shaderStorageImageArrayDynamicIndexing = shaderStorageImageArrayDynamicIndexing; m_physicalDeviceFeatures.shaderClipDistance = shaderClipDistance; m_physicalDeviceFeatures.shaderCullDistance = shaderCullDistance; m_physicalDeviceFeatures.shaderFloat64 = shaderFloat64; m_physicalDeviceFeatures.shaderInt64 = shaderInt64; m_physicalDeviceFeatures.shaderInt16 = shaderInt16; m_physicalDeviceFeatures.shaderResourceResidency = shaderResourceResidency; m_physicalDeviceFeatures.shaderResourceMinLod = shaderResourceMinLod; m_physicalDeviceFeatures.sparseBinding = sparseBinding; m_physicalDeviceFeatures.sparseResidencyBuffer = sparseResidencyBuffer; m_physicalDeviceFeatures.sparseResidencyImage2D = sparseResidencyImage2D; m_physicalDeviceFeatures.sparseResidencyImage3D = sparseResidencyImage3D; m_physicalDeviceFeatures.sparseResidency2Samples = sparseResidency2Samples; m_physicalDeviceFeatures.sparseResidency4Samples = sparseResidency4Samples; m_physicalDeviceFeatures.sparseResidency8Samples = sparseResidency8Samples; m_physicalDeviceFeatures.sparseResidency16Samples = sparseResidency16Samples; m_physicalDeviceFeatures.sparseResidencyAliased = sparseResidencyAliased; m_physicalDeviceFeatures.variableMultisampleRate = variableMultisampleRate; m_physicalDeviceFeatures.inheritedQueries = inheritedQueries; }
    PhysicalDeviceFeatures(const VkPhysicalDeviceFeatures& other) : m_physicalDeviceFeatures{other} {}
    PhysicalDeviceFeatures& operator=(const VkPhysicalDeviceFeatures& other){ m_physicalDeviceFeatures = other; return *this; }

    const Bool32& robustBufferAccess() const { return m_physicalDeviceFeatures.robustBufferAccess; }
    PhysicalDeviceFeatures& robustBufferAccess(Bool32 robustBufferAccess){ m_physicalDeviceFeatures.robustBufferAccess = robustBufferAccess; return *this; }
    const Bool32& fullDrawIndexUint32() const { return m_physicalDeviceFeatures.fullDrawIndexUint32; }
    PhysicalDeviceFeatures& fullDrawIndexUint32(Bool32 fullDrawIndexUint32){ m_physicalDeviceFeatures.fullDrawIndexUint32 = fullDrawIndexUint32; return *this; }
    const Bool32& imageCubeArray() const { return m_physicalDeviceFeatures.imageCubeArray; }
    PhysicalDeviceFeatures& imageCubeArray(Bool32 imageCubeArray){ m_physicalDeviceFeatures.imageCubeArray = imageCubeArray; return *this; }
    const Bool32& independentBlend() const { return m_physicalDeviceFeatures.independentBlend; }
    PhysicalDeviceFeatures& independentBlend(Bool32 independentBlend){ m_physicalDeviceFeatures.independentBlend = independentBlend; return *this; }
    const Bool32& geometryShader() const { return m_physicalDeviceFeatures.geometryShader; }
    PhysicalDeviceFeatures& geometryShader(Bool32 geometryShader){ m_physicalDeviceFeatures.geometryShader = geometryShader; return *this; }
    const Bool32& tessellationShader() const { return m_physicalDeviceFeatures.tessellationShader; }
    PhysicalDeviceFeatures& tessellationShader(Bool32 tessellationShader){ m_physicalDeviceFeatures.tessellationShader = tessellationShader; return *this; }
    const Bool32& sampleRateShading() const { return m_physicalDeviceFeatures.sampleRateShading; }
    PhysicalDeviceFeatures& sampleRateShading(Bool32 sampleRateShading){ m_physicalDeviceFeatures.sampleRateShading = sampleRateShading; return *this; }
    const Bool32& dualSrcBlend() const { return m_physicalDeviceFeatures.dualSrcBlend; }
    PhysicalDeviceFeatures& dualSrcBlend(Bool32 dualSrcBlend){ m_physicalDeviceFeatures.dualSrcBlend = dualSrcBlend; return *this; }
    const Bool32& logicOp() const { return m_physicalDeviceFeatures.logicOp; }
    PhysicalDeviceFeatures& logicOp(Bool32 logicOp){ m_physicalDeviceFeatures.logicOp = logicOp; return *this; }
    const Bool32& multiDrawIndirect() const { return m_physicalDeviceFeatures.multiDrawIndirect; }
    PhysicalDeviceFeatures& multiDrawIndirect(Bool32 multiDrawIndirect){ m_physicalDeviceFeatures.multiDrawIndirect = multiDrawIndirect; return *this; }
    const Bool32& drawIndirectFirstInstance() const { return m_physicalDeviceFeatures.drawIndirectFirstInstance; }
    PhysicalDeviceFeatures& drawIndirectFirstInstance(Bool32 drawIndirectFirstInstance){ m_physicalDeviceFeatures.drawIndirectFirstInstance = drawIndirectFirstInstance; return *this; }
    const Bool32& depthClamp() const { return m_physicalDeviceFeatures.depthClamp; }
    PhysicalDeviceFeatures& depthClamp(Bool32 depthClamp){ m_physicalDeviceFeatures.depthClamp = depthClamp; return *this; }
    const Bool32& depthBiasClamp() const { return m_physicalDeviceFeatures.depthBiasClamp; }
    PhysicalDeviceFeatures& depthBiasClamp(Bool32 depthBiasClamp){ m_physicalDeviceFeatures.depthBiasClamp = depthBiasClamp; return *this; }
    const Bool32& fillModeNonSolid() const { return m_physicalDeviceFeatures.fillModeNonSolid; }
    PhysicalDeviceFeatures& fillModeNonSolid(Bool32 fillModeNonSolid){ m_physicalDeviceFeatures.fillModeNonSolid = fillModeNonSolid; return *this; }
    const Bool32& depthBounds() const { return m_physicalDeviceFeatures.depthBounds; }
    PhysicalDeviceFeatures& depthBounds(Bool32 depthBounds){ m_physicalDeviceFeatures.depthBounds = depthBounds; return *this; }
    const Bool32& wideLines() const { return m_physicalDeviceFeatures.wideLines; }
    PhysicalDeviceFeatures& wideLines(Bool32 wideLines){ m_physicalDeviceFeatures.wideLines = wideLines; return *this; }
    const Bool32& largePoints() const { return m_physicalDeviceFeatures.largePoints; }
    PhysicalDeviceFeatures& largePoints(Bool32 largePoints){ m_physicalDeviceFeatures.largePoints = largePoints; return *this; }
    const Bool32& alphaToOne() const { return m_physicalDeviceFeatures.alphaToOne; }
    PhysicalDeviceFeatures& alphaToOne(Bool32 alphaToOne){ m_physicalDeviceFeatures.alphaToOne = alphaToOne; return *this; }
    const Bool32& multiViewport() const { return m_physicalDeviceFeatures.multiViewport; }
    PhysicalDeviceFeatures& multiViewport(Bool32 multiViewport){ m_physicalDeviceFeatures.multiViewport = multiViewport; return *this; }
    const Bool32& samplerAnisotropy() const { return m_physicalDeviceFeatures.samplerAnisotropy; }
    PhysicalDeviceFeatures& samplerAnisotropy(Bool32 samplerAnisotropy){ m_physicalDeviceFeatures.samplerAnisotropy = samplerAnisotropy; return *this; }
    const Bool32& textureCompressionETC2() const { return m_physicalDeviceFeatures.textureCompressionETC2; }
    PhysicalDeviceFeatures& textureCompressionETC2(Bool32 textureCompressionETC2){ m_physicalDeviceFeatures.textureCompressionETC2 = textureCompressionETC2; return *this; }
    const Bool32& textureCompressionASTC_LDR() const { return m_physicalDeviceFeatures.textureCompressionASTC_LDR; }
    PhysicalDeviceFeatures& textureCompressionASTC_LDR(Bool32 textureCompressionASTC_LDR){ m_physicalDeviceFeatures.textureCompressionASTC_LDR = textureCompressionASTC_LDR; return *this; }
    const Bool32& textureCompressionBC() const { return m_physicalDeviceFeatures.textureCompressionBC; }
    PhysicalDeviceFeatures& textureCompressionBC(Bool32 textureCompressionBC){ m_physicalDeviceFeatures.textureCompressionBC = textureCompressionBC; return *this; }
    const Bool32& occlusionQueryPrecise() const { return m_physicalDeviceFeatures.occlusionQueryPrecise; }
    PhysicalDeviceFeatures& occlusionQueryPrecise(Bool32 occlusionQueryPrecise){ m_physicalDeviceFeatures.occlusionQueryPrecise = occlusionQueryPrecise; return *this; }
    const Bool32& pipelineStatisticsQuery() const { return m_physicalDeviceFeatures.pipelineStatisticsQuery; }
    PhysicalDeviceFeatures& pipelineStatisticsQuery(Bool32 pipelineStatisticsQuery){ m_physicalDeviceFeatures.pipelineStatisticsQuery = pipelineStatisticsQuery; return *this; }
    const Bool32& vertexPipelineStoresAndAtomics() const { return m_physicalDeviceFeatures.vertexPipelineStoresAndAtomics; }
    PhysicalDeviceFeatures& vertexPipelineStoresAndAtomics(Bool32 vertexPipelineStoresAndAtomics){ m_physicalDeviceFeatures.vertexPipelineStoresAndAtomics = vertexPipelineStoresAndAtomics; return *this; }
    const Bool32& fragmentStoresAndAtomics() const { return m_physicalDeviceFeatures.fragmentStoresAndAtomics; }
    PhysicalDeviceFeatures& fragmentStoresAndAtomics(Bool32 fragmentStoresAndAtomics){ m_physicalDeviceFeatures.fragmentStoresAndAtomics = fragmentStoresAndAtomics; return *this; }
    const Bool32& shaderTessellationAndGeometryPointSize() const { return m_physicalDeviceFeatures.shaderTessellationAndGeometryPointSize; }
    PhysicalDeviceFeatures& shaderTessellationAndGeometryPointSize(Bool32 shaderTessellationAndGeometryPointSize){ m_physicalDeviceFeatures.shaderTessellationAndGeometryPointSize = shaderTessellationAndGeometryPointSize; return *this; }
    const Bool32& shaderImageGatherExtended() const { return m_physicalDeviceFeatures.shaderImageGatherExtended; }
    PhysicalDeviceFeatures& shaderImageGatherExtended(Bool32 shaderImageGatherExtended){ m_physicalDeviceFeatures.shaderImageGatherExtended = shaderImageGatherExtended; return *this; }
    const Bool32& shaderStorageImageExtendedFormats() const { return m_physicalDeviceFeatures.shaderStorageImageExtendedFormats; }
    PhysicalDeviceFeatures& shaderStorageImageExtendedFormats(Bool32 shaderStorageImageExtendedFormats){ m_physicalDeviceFeatures.shaderStorageImageExtendedFormats = shaderStorageImageExtendedFormats; return *this; }
    const Bool32& shaderStorageImageMultisample() const { return m_physicalDeviceFeatures.shaderStorageImageMultisample; }
    PhysicalDeviceFeatures& shaderStorageImageMultisample(Bool32 shaderStorageImageMultisample){ m_physicalDeviceFeatures.shaderStorageImageMultisample = shaderStorageImageMultisample; return *this; }
    const Bool32& shaderStorageImageReadWithoutFormat() const { return m_physicalDeviceFeatures.shaderStorageImageReadWithoutFormat; }
    PhysicalDeviceFeatures& shaderStorageImageReadWithoutFormat(Bool32 shaderStorageImageReadWithoutFormat){ m_physicalDeviceFeatures.shaderStorageImageReadWithoutFormat = shaderStorageImageReadWithoutFormat; return *this; }
    const Bool32& shaderStorageImageWriteWithoutFormat() const { return m_physicalDeviceFeatures.shaderStorageImageWriteWithoutFormat; }
    PhysicalDeviceFeatures& shaderStorageImageWriteWithoutFormat(Bool32 shaderStorageImageWriteWithoutFormat){ m_physicalDeviceFeatures.shaderStorageImageWriteWithoutFormat = shaderStorageImageWriteWithoutFormat; return *this; }
    const Bool32& shaderUniformBufferArrayDynamicIndexing() const { return m_physicalDeviceFeatures.shaderUniformBufferArrayDynamicIndexing; }
    PhysicalDeviceFeatures& shaderUniformBufferArrayDynamicIndexing(Bool32 shaderUniformBufferArrayDynamicIndexing){ m_physicalDeviceFeatures.shaderUniformBufferArrayDynamicIndexing = shaderUniformBufferArrayDynamicIndexing; return *this; }
    const Bool32& shaderSampledImageArrayDynamicIndexing() const { return m_physicalDeviceFeatures.shaderSampledImageArrayDynamicIndexing; }
    PhysicalDeviceFeatures& shaderSampledImageArrayDynamicIndexing(Bool32 shaderSampledImageArrayDynamicIndexing){ m_physicalDeviceFeatures.shaderSampledImageArrayDynamicIndexing = shaderSampledImageArrayDynamicIndexing; return *this; }
    const Bool32& shaderStorageBufferArrayDynamicIndexing() const { return m_physicalDeviceFeatures.shaderStorageBufferArrayDynamicIndexing; }
    PhysicalDeviceFeatures& shaderStorageBufferArrayDynamicIndexing(Bool32 shaderStorageBufferArrayDynamicIndexing){ m_physicalDeviceFeatures.shaderStorageBufferArrayDynamicIndexing = shaderStorageBufferArrayDynamicIndexing; return *this; }
    const Bool32& shaderStorageImageArrayDynamicIndexing() const { return m_physicalDeviceFeatures.shaderStorageImageArrayDynamicIndexing; }
    PhysicalDeviceFeatures& shaderStorageImageArrayDynamicIndexing(Bool32 shaderStorageImageArrayDynamicIndexing){ m_physicalDeviceFeatures.shaderStorageImageArrayDynamicIndexing = shaderStorageImageArrayDynamicIndexing; return *this; }
    const Bool32& shaderClipDistance() const { return m_physicalDeviceFeatures.shaderClipDistance; }
    PhysicalDeviceFeatures& shaderClipDistance(Bool32 shaderClipDistance){ m_physicalDeviceFeatures.shaderClipDistance = shaderClipDistance; return *this; }
    const Bool32& shaderCullDistance() const { return m_physicalDeviceFeatures.shaderCullDistance; }
    PhysicalDeviceFeatures& shaderCullDistance(Bool32 shaderCullDistance){ m_physicalDeviceFeatures.shaderCullDistance = shaderCullDistance; return *this; }
    const Bool32& shaderFloat64() const { return m_physicalDeviceFeatures.shaderFloat64; }
    PhysicalDeviceFeatures& shaderFloat64(Bool32 shaderFloat64){ m_physicalDeviceFeatures.shaderFloat64 = shaderFloat64; return *this; }
    const Bool32& shaderInt64() const { return m_physicalDeviceFeatures.shaderInt64; }
    PhysicalDeviceFeatures& shaderInt64(Bool32 shaderInt64){ m_physicalDeviceFeatures.shaderInt64 = shaderInt64; return *this; }
    const Bool32& shaderInt16() const { return m_physicalDeviceFeatures.shaderInt16; }
    PhysicalDeviceFeatures& shaderInt16(Bool32 shaderInt16){ m_physicalDeviceFeatures.shaderInt16 = shaderInt16; return *this; }
    const Bool32& shaderResourceResidency() const { return m_physicalDeviceFeatures.shaderResourceResidency; }
    PhysicalDeviceFeatures& shaderResourceResidency(Bool32 shaderResourceResidency){ m_physicalDeviceFeatures.shaderResourceResidency = shaderResourceResidency; return *this; }
    const Bool32& shaderResourceMinLod() const { return m_physicalDeviceFeatures.shaderResourceMinLod; }
    PhysicalDeviceFeatures& shaderResourceMinLod(Bool32 shaderResourceMinLod){ m_physicalDeviceFeatures.shaderResourceMinLod = shaderResourceMinLod; return *this; }
    const Bool32& sparseBinding() const { return m_physicalDeviceFeatures.sparseBinding; }
    PhysicalDeviceFeatures& sparseBinding(Bool32 sparseBinding){ m_physicalDeviceFeatures.sparseBinding = sparseBinding; return *this; }
    const Bool32& sparseResidencyBuffer() const { return m_physicalDeviceFeatures.sparseResidencyBuffer; }
    PhysicalDeviceFeatures& sparseResidencyBuffer(Bool32 sparseResidencyBuffer){ m_physicalDeviceFeatures.sparseResidencyBuffer = sparseResidencyBuffer; return *this; }
    const Bool32& sparseResidencyImage2D() const { return m_physicalDeviceFeatures.sparseResidencyImage2D; }
    PhysicalDeviceFeatures& sparseResidencyImage2D(Bool32 sparseResidencyImage2D){ m_physicalDeviceFeatures.sparseResidencyImage2D = sparseResidencyImage2D; return *this; }
    const Bool32& sparseResidencyImage3D() const { return m_physicalDeviceFeatures.sparseResidencyImage3D; }
    PhysicalDeviceFeatures& sparseResidencyImage3D(Bool32 sparseResidencyImage3D){ m_physicalDeviceFeatures.sparseResidencyImage3D = sparseResidencyImage3D; return *this; }
    const Bool32& sparseResidency2Samples() const { return m_physicalDeviceFeatures.sparseResidency2Samples; }
    PhysicalDeviceFeatures& sparseResidency2Samples(Bool32 sparseResidency2Samples){ m_physicalDeviceFeatures.sparseResidency2Samples = sparseResidency2Samples; return *this; }
    const Bool32& sparseResidency4Samples() const { return m_physicalDeviceFeatures.sparseResidency4Samples; }
    PhysicalDeviceFeatures& sparseResidency4Samples(Bool32 sparseResidency4Samples){ m_physicalDeviceFeatures.sparseResidency4Samples = sparseResidency4Samples; return *this; }
    const Bool32& sparseResidency8Samples() const { return m_physicalDeviceFeatures.sparseResidency8Samples; }
    PhysicalDeviceFeatures& sparseResidency8Samples(Bool32 sparseResidency8Samples){ m_physicalDeviceFeatures.sparseResidency8Samples = sparseResidency8Samples; return *this; }
    const Bool32& sparseResidency16Samples() const { return m_physicalDeviceFeatures.sparseResidency16Samples; }
    PhysicalDeviceFeatures& sparseResidency16Samples(Bool32 sparseResidency16Samples){ m_physicalDeviceFeatures.sparseResidency16Samples = sparseResidency16Samples; return *this; }
    const Bool32& sparseResidencyAliased() const { return m_physicalDeviceFeatures.sparseResidencyAliased; }
    PhysicalDeviceFeatures& sparseResidencyAliased(Bool32 sparseResidencyAliased){ m_physicalDeviceFeatures.sparseResidencyAliased = sparseResidencyAliased; return *this; }
    const Bool32& variableMultisampleRate() const { return m_physicalDeviceFeatures.variableMultisampleRate; }
    PhysicalDeviceFeatures& variableMultisampleRate(Bool32 variableMultisampleRate){ m_physicalDeviceFeatures.variableMultisampleRate = variableMultisampleRate; return *this; }
    const Bool32& inheritedQueries() const { return m_physicalDeviceFeatures.inheritedQueries; }
    PhysicalDeviceFeatures& inheritedQueries(Bool32 inheritedQueries){ m_physicalDeviceFeatures.inheritedQueries = inheritedQueries; return *this; }
    operator VkPhysicalDeviceFeatures&() { return m_physicalDeviceFeatures; }
    operator const VkPhysicalDeviceFeatures&() const { return m_physicalDeviceFeatures; }
    VkPhysicalDeviceFeatures& vkHandle() { return m_physicalDeviceFeatures; }
    const VkPhysicalDeviceFeatures& vkHandle() const { return m_physicalDeviceFeatures; }

  private:
    VkPhysicalDeviceFeatures m_physicalDeviceFeatures;
  };

  class PhysicalDeviceSparseProperties
  {
  public:
    const Bool32& residencyStandard2DBlockShape() const { return m_physicalDeviceSparseProperties.residencyStandard2DBlockShape; }
    const Bool32& residencyStandard2DMultisampleBlockShape() const { return m_physicalDeviceSparseProperties.residencyStandard2DMultisampleBlockShape; }
    const Bool32& residencyStandard3DBlockShape() const { return m_physicalDeviceSparseProperties.residencyStandard3DBlockShape; }
    const Bool32& residencyAlignedMipSize() const { return m_physicalDeviceSparseProperties.residencyAlignedMipSize; }
    const Bool32& residencyNonResidentStrict() const { return m_physicalDeviceSparseProperties.residencyNonResidentStrict; }
    operator VkPhysicalDeviceSparseProperties&() { return m_physicalDeviceSparseProperties; }
    operator const VkPhysicalDeviceSparseProperties&() const { return m_physicalDeviceSparseProperties; }
    VkPhysicalDeviceSparseProperties& vkHandle() { return m_physicalDeviceSparseProperties; }
    const VkPhysicalDeviceSparseProperties& vkHandle() const { return m_physicalDeviceSparseProperties; }

  private:
    VkPhysicalDeviceSparseProperties m_physicalDeviceSparseProperties;
  };

  class DrawIndirectCommand
  {
  public:
    DrawIndirectCommand() : DrawIndirectCommand(0, 0, 0, 0) {}
    DrawIndirectCommand(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) {m_drawIndirectCommand.vertexCount = vertexCount; m_drawIndirectCommand.instanceCount = instanceCount; m_drawIndirectCommand.firstVertex = firstVertex; m_drawIndirectCommand.firstInstance = firstInstance; }
    DrawIndirectCommand(const VkDrawIndirectCommand& other) : m_drawIndirectCommand{other} {}
    DrawIndirectCommand& operator=(const VkDrawIndirectCommand& other){ m_drawIndirectCommand = other; return *this; }

    const uint32_t& vertexCount() const { return m_drawIndirectCommand.vertexCount; }
    DrawIndirectCommand& vertexCount(uint32_t vertexCount){ m_drawIndirectCommand.vertexCount = vertexCount; return *this; }
    const uint32_t& instanceCount() const { return m_drawIndirectCommand.instanceCount; }
    DrawIndirectCommand& instanceCount(uint32_t instanceCount){ m_drawIndirectCommand.instanceCount = instanceCount; return *this; }
    const uint32_t& firstVertex() const { return m_drawIndirectCommand.firstVertex; }
    DrawIndirectCommand& firstVertex(uint32_t firstVertex){ m_drawIndirectCommand.firstVertex = firstVertex; return *this; }
    const uint32_t& firstInstance() const { return m_drawIndirectCommand.firstInstance; }
    DrawIndirectCommand& firstInstance(uint32_t firstInstance){ m_drawIndirectCommand.firstInstance = firstInstance; return *this; }
    operator VkDrawIndirectCommand&() { return m_drawIndirectCommand; }
    operator const VkDrawIndirectCommand&() const { return m_drawIndirectCommand; }
    VkDrawIndirectCommand& vkHandle() { return m_drawIndirectCommand; }
    const VkDrawIndirectCommand& vkHandle() const { return m_drawIndirectCommand; }

  private:
    VkDrawIndirectCommand m_drawIndirectCommand;
  };

  class DrawIndexedIndirectCommand
  {
  public:
    DrawIndexedIndirectCommand() : DrawIndexedIndirectCommand(0, 0, 0, 0, 0) {}
    DrawIndexedIndirectCommand(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) {m_drawIndexedIndirectCommand.indexCount = indexCount; m_drawIndexedIndirectCommand.instanceCount = instanceCount; m_drawIndexedIndirectCommand.firstIndex = firstIndex; m_drawIndexedIndirectCommand.vertexOffset = vertexOffset; m_drawIndexedIndirectCommand.firstInstance = firstInstance; }
    DrawIndexedIndirectCommand(const VkDrawIndexedIndirectCommand& other) : m_drawIndexedIndirectCommand{other} {}
    DrawIndexedIndirectCommand& operator=(const VkDrawIndexedIndirectCommand& other){ m_drawIndexedIndirectCommand = other; return *this; }

    const uint32_t& indexCount() const { return m_drawIndexedIndirectCommand.indexCount; }
    DrawIndexedIndirectCommand& indexCount(uint32_t indexCount){ m_drawIndexedIndirectCommand.indexCount = indexCount; return *this; }
    const uint32_t& instanceCount() const { return m_drawIndexedIndirectCommand.instanceCount; }
    DrawIndexedIndirectCommand& instanceCount(uint32_t instanceCount){ m_drawIndexedIndirectCommand.instanceCount = instanceCount; return *this; }
    const uint32_t& firstIndex() const { return m_drawIndexedIndirectCommand.firstIndex; }
    DrawIndexedIndirectCommand& firstIndex(uint32_t firstIndex){ m_drawIndexedIndirectCommand.firstIndex = firstIndex; return *this; }
    const int32_t& vertexOffset() const { return m_drawIndexedIndirectCommand.vertexOffset; }
    DrawIndexedIndirectCommand& vertexOffset(int32_t vertexOffset){ m_drawIndexedIndirectCommand.vertexOffset = vertexOffset; return *this; }
    const uint32_t& firstInstance() const { return m_drawIndexedIndirectCommand.firstInstance; }
    DrawIndexedIndirectCommand& firstInstance(uint32_t firstInstance){ m_drawIndexedIndirectCommand.firstInstance = firstInstance; return *this; }
    operator VkDrawIndexedIndirectCommand&() { return m_drawIndexedIndirectCommand; }
    operator const VkDrawIndexedIndirectCommand&() const { return m_drawIndexedIndirectCommand; }
    VkDrawIndexedIndirectCommand& vkHandle() { return m_drawIndexedIndirectCommand; }
    const VkDrawIndexedIndirectCommand& vkHandle() const { return m_drawIndexedIndirectCommand; }

  private:
    VkDrawIndexedIndirectCommand m_drawIndexedIndirectCommand;
  };

  class DispatchIndirectCommand
  {
  public:
    DispatchIndirectCommand() : DispatchIndirectCommand(0, 0, 0) {}
    DispatchIndirectCommand(uint32_t x, uint32_t y, uint32_t z) {m_dispatchIndirectCommand.x = x; m_dispatchIndirectCommand.y = y; m_dispatchIndirectCommand.z = z; }
    DispatchIndirectCommand(const VkDispatchIndirectCommand& other) : m_dispatchIndirectCommand{other} {}
    DispatchIndirectCommand& operator=(const VkDispatchIndirectCommand& other){ m_dispatchIndirectCommand = other; return *this; }

    const uint32_t& x() const { return m_dispatchIndirectCommand.x; }
    DispatchIndirectCommand& x(uint32_t x){ m_dispatchIndirectCommand.x = x; return *this; }
    const uint32_t& y() const { return m_dispatchIndirectCommand.y; }
    DispatchIndirectCommand& y(uint32_t y){ m_dispatchIndirectCommand.y = y; return *this; }
    const uint32_t& z() const { return m_dispatchIndirectCommand.z; }
    DispatchIndirectCommand& z(uint32_t z){ m_dispatchIndirectCommand.z = z; return *this; }
    operator VkDispatchIndirectCommand&() { return m_dispatchIndirectCommand; }
    operator const VkDispatchIndirectCommand&() const { return m_dispatchIndirectCommand; }
    VkDispatchIndirectCommand& vkHandle() { return m_dispatchIndirectCommand; }
    const VkDispatchIndirectCommand& vkHandle() const { return m_dispatchIndirectCommand; }

  private:
    VkDispatchIndirectCommand m_dispatchIndirectCommand;
  };

  class DescriptorImageInfo
  {
  public:
    DescriptorImageInfo() : DescriptorImageInfo(Sampler(), ImageView(), ImageLayout::Undefined) {}
    DescriptorImageInfo(Sampler sampler, ImageView imageView, ImageLayout imageLayout) {m_descriptorImageInfo.sampler = sampler; m_descriptorImageInfo.imageView = imageView; m_descriptorImageInfo.imageLayout = static_cast<VkImageLayout>( imageLayout ); }
    DescriptorImageInfo(const VkDescriptorImageInfo& other) : m_descriptorImageInfo{other} {}
    DescriptorImageInfo& operator=(const VkDescriptorImageInfo& other){ m_descriptorImageInfo = other; return *this; }

    const Sampler& sampler() const { return m_descriptorImageInfo.sampler; }
    DescriptorImageInfo& sampler(Sampler sampler){ m_descriptorImageInfo.sampler = sampler; return *this; }
    const ImageView& imageView() const { return m_descriptorImageInfo.imageView; }
    DescriptorImageInfo& imageView(ImageView imageView){ m_descriptorImageInfo.imageView = imageView; return *this; }
    const ImageLayout& imageLayout() const { return reinterpret_cast<const ImageLayout&>( m_descriptorImageInfo.imageLayout ); }
    DescriptorImageInfo& imageLayout(ImageLayout imageLayout){ m_descriptorImageInfo.imageLayout = static_cast<VkImageLayout>( imageLayout ); return *this; }
    operator VkDescriptorImageInfo&() { return m_descriptorImageInfo; }
    operator const VkDescriptorImageInfo&() const { return m_descriptorImageInfo; }
    VkDescriptorImageInfo& vkHandle() { return m_descriptorImageInfo; }
    const VkDescriptorImageInfo& vkHandle() const { return m_descriptorImageInfo; }

  private:
    VkDescriptorImageInfo m_descriptorImageInfo;
  };

  class AttachmentReference
  {
  public:
    AttachmentReference() : AttachmentReference(0, ImageLayout::Undefined) {}
    AttachmentReference(uint32_t attachment, ImageLayout layout) {m_attachmentReference.attachment = attachment; m_attachmentReference.layout = static_cast<VkImageLayout>( layout ); }
    AttachmentReference(const VkAttachmentReference& other) : m_attachmentReference{other} {}
    AttachmentReference& operator=(const VkAttachmentReference& other){ m_attachmentReference = other; return *this; }

    const uint32_t& attachment() const { return m_attachmentReference.attachment; }
    AttachmentReference& attachment(uint32_t attachment){ m_attachmentReference.attachment = attachment; return *this; }
    const ImageLayout& layout() const { return reinterpret_cast<const ImageLayout&>( m_attachmentReference.layout ); }
    AttachmentReference& layout(ImageLayout layout){ m_attachmentReference.layout = static_cast<VkImageLayout>( layout ); return *this; }
    operator VkAttachmentReference&() { return m_attachmentReference; }
    operator const VkAttachmentReference&() const { return m_attachmentReference; }
    VkAttachmentReference& vkHandle() { return m_attachmentReference; }
    const VkAttachmentReference& vkHandle() const { return m_attachmentReference; }

  private:
    VkAttachmentReference m_attachmentReference;
  };

  class ComponentMapping
  {
  public:
    ComponentMapping() : ComponentMapping(ComponentSwizzle::Identity, ComponentSwizzle::Identity, ComponentSwizzle::Identity, ComponentSwizzle::Identity) {}
    ComponentMapping(ComponentSwizzle r, ComponentSwizzle g, ComponentSwizzle b, ComponentSwizzle a) {m_componentMapping.r = static_cast<VkComponentSwizzle>( r ); m_componentMapping.g = static_cast<VkComponentSwizzle>( g ); m_componentMapping.b = static_cast<VkComponentSwizzle>( b ); m_componentMapping.a = static_cast<VkComponentSwizzle>( a ); }
    ComponentMapping(const VkComponentMapping& other) : m_componentMapping{other} {}
    ComponentMapping& operator=(const VkComponentMapping& other){ m_componentMapping = other; return *this; }

    const ComponentSwizzle& r() const { return reinterpret_cast<const ComponentSwizzle&>( m_componentMapping.r ); }
    ComponentMapping& r(ComponentSwizzle r){ m_componentMapping.r = static_cast<VkComponentSwizzle>( r ); return *this; }
    const ComponentSwizzle& g() const { return reinterpret_cast<const ComponentSwizzle&>( m_componentMapping.g ); }
    ComponentMapping& g(ComponentSwizzle g){ m_componentMapping.g = static_cast<VkComponentSwizzle>( g ); return *this; }
    const ComponentSwizzle& b() const { return reinterpret_cast<const ComponentSwizzle&>( m_componentMapping.b ); }
    ComponentMapping& b(ComponentSwizzle b){ m_componentMapping.b = static_cast<VkComponentSwizzle>( b ); return *this; }
    const ComponentSwizzle& a() const { return reinterpret_cast<const ComponentSwizzle&>( m_componentMapping.a ); }
    ComponentMapping& a(ComponentSwizzle a){ m_componentMapping.a = static_cast<VkComponentSwizzle>( a ); return *this; }
    operator VkComponentMapping&() { return m_componentMapping; }
    operator const VkComponentMapping&() const { return m_componentMapping; }
    VkComponentMapping& vkHandle() { return m_componentMapping; }
    const VkComponentMapping& vkHandle() const { return m_componentMapping; }

  private:
    VkComponentMapping m_componentMapping;
  };

  class DescriptorPoolSize
  {
  public:
    DescriptorPoolSize() : DescriptorPoolSize(DescriptorType::Sampler, 0) {}
    DescriptorPoolSize(DescriptorType type, uint32_t descriptorCount) {m_descriptorPoolSize.type = static_cast<VkDescriptorType>( type ); m_descriptorPoolSize.descriptorCount = descriptorCount; }
    DescriptorPoolSize(const VkDescriptorPoolSize& other) : m_descriptorPoolSize{other} {}
    DescriptorPoolSize& operator=(const VkDescriptorPoolSize& other){ m_descriptorPoolSize = other; return *this; }

    const DescriptorType& type() const { return reinterpret_cast<const DescriptorType&>( m_descriptorPoolSize.type ); }
    DescriptorPoolSize& type(DescriptorType type){ m_descriptorPoolSize.type = static_cast<VkDescriptorType>( type ); return *this; }
    const uint32_t& descriptorCount() const { return m_descriptorPoolSize.descriptorCount; }
    DescriptorPoolSize& descriptorCount(uint32_t descriptorCount){ m_descriptorPoolSize.descriptorCount = descriptorCount; return *this; }
    operator VkDescriptorPoolSize&() { return m_descriptorPoolSize; }
    operator const VkDescriptorPoolSize&() const { return m_descriptorPoolSize; }
    VkDescriptorPoolSize& vkHandle() { return m_descriptorPoolSize; }
    const VkDescriptorPoolSize& vkHandle() const { return m_descriptorPoolSize; }

  private:
    VkDescriptorPoolSize m_descriptorPoolSize;
  };

  class SubpassDescription
  {
  public:
    SubpassDescription() : SubpassDescription(SubpassDescriptionFlags(), PipelineBindPoint::Graphics, 0, nullptr, 0, nullptr, nullptr, nullptr, 0, nullptr) {}
    SubpassDescription(SubpassDescriptionFlags flags, PipelineBindPoint pipelineBindPoint, uint32_t inputAttachmentCount, const AttachmentReference* pInputAttachments, uint32_t colorAttachmentCount, const AttachmentReference* pColorAttachments, const AttachmentReference* pResolveAttachments, const AttachmentReference* pDepthStencilAttachment, uint32_t preserveAttachmentCount, const uint32_t* pPreserveAttachments) {m_subpassDescription.flags = static_cast<VkSubpassDescriptionFlags>( flags ); m_subpassDescription.pipelineBindPoint = static_cast<VkPipelineBindPoint>( pipelineBindPoint ); m_subpassDescription.inputAttachmentCount = inputAttachmentCount; m_subpassDescription.pInputAttachments = reinterpret_cast<const VkAttachmentReference*>( pInputAttachments ); m_subpassDescription.colorAttachmentCount = colorAttachmentCount; m_subpassDescription.pColorAttachments = reinterpret_cast<const VkAttachmentReference*>( pColorAttachments ); m_subpassDescription.pResolveAttachments = reinterpret_cast<const VkAttachmentReference*>( pResolveAttachments ); m_subpassDescription.pDepthStencilAttachment = reinterpret_cast<const VkAttachmentReference*>( pDepthStencilAttachment ); m_subpassDescription.preserveAttachmentCount = preserveAttachmentCount; m_subpassDescription.pPreserveAttachments = pPreserveAttachments; }
    SubpassDescription(const VkSubpassDescription& other) : m_subpassDescription{other} {}
    SubpassDescription& operator=(const VkSubpassDescription& other){ m_subpassDescription = other; return *this; }

    const SubpassDescriptionFlags& flags() const { return reinterpret_cast<const SubpassDescriptionFlags&>( m_subpassDescription.flags ); }
    SubpassDescription& flags(SubpassDescriptionFlags flags){ m_subpassDescription.flags = static_cast<VkSubpassDescriptionFlags>( flags ); return *this; }
    const PipelineBindPoint& pipelineBindPoint() const { return reinterpret_cast<const PipelineBindPoint&>( m_subpassDescription.pipelineBindPoint ); }
    SubpassDescription& pipelineBindPoint(PipelineBindPoint pipelineBindPoint){ m_subpassDescription.pipelineBindPoint = static_cast<VkPipelineBindPoint>( pipelineBindPoint ); return *this; }
    const uint32_t& inputAttachmentCount() const { return m_subpassDescription.inputAttachmentCount; }
    SubpassDescription& inputAttachmentCount(uint32_t inputAttachmentCount){ m_subpassDescription.inputAttachmentCount = inputAttachmentCount; return *this; }
    const AttachmentReference* pInputAttachments() const { return reinterpret_cast<const AttachmentReference*>( m_subpassDescription.pInputAttachments ); }
    SubpassDescription& pInputAttachments(const AttachmentReference* pInputAttachments){ m_subpassDescription.pInputAttachments = reinterpret_cast<const VkAttachmentReference*>( pInputAttachments ); return *this; }
    const uint32_t& colorAttachmentCount() const { return m_subpassDescription.colorAttachmentCount; }
    SubpassDescription& colorAttachmentCount(uint32_t colorAttachmentCount){ m_subpassDescription.colorAttachmentCount = colorAttachmentCount; return *this; }
    const AttachmentReference* pColorAttachments() const { return reinterpret_cast<const AttachmentReference*>( m_subpassDescription.pColorAttachments ); }
    SubpassDescription& pColorAttachments(const AttachmentReference* pColorAttachments){ m_subpassDescription.pColorAttachments = reinterpret_cast<const VkAttachmentReference*>( pColorAttachments ); return *this; }
    const AttachmentReference* pResolveAttachments() const { return reinterpret_cast<const AttachmentReference*>( m_subpassDescription.pResolveAttachments ); }
    SubpassDescription& pResolveAttachments(const AttachmentReference* pResolveAttachments){ m_subpassDescription.pResolveAttachments = reinterpret_cast<const VkAttachmentReference*>( pResolveAttachments ); return *this; }
    const AttachmentReference* pDepthStencilAttachment() const { return reinterpret_cast<const AttachmentReference*>( m_subpassDescription.pDepthStencilAttachment ); }
    SubpassDescription& pDepthStencilAttachment(const AttachmentReference* pDepthStencilAttachment){ m_subpassDescription.pDepthStencilAttachment = reinterpret_cast<const VkAttachmentReference*>( pDepthStencilAttachment ); return *this; }
    const uint32_t& preserveAttachmentCount() const { return m_subpassDescription.preserveAttachmentCount; }
    SubpassDescription& preserveAttachmentCount(uint32_t preserveAttachmentCount){ m_subpassDescription.preserveAttachmentCount = preserveAttachmentCount; return *this; }
    const uint32_t* pPreserveAttachments() const { return reinterpret_cast<const uint32_t*>( m_subpassDescription.pPreserveAttachments ); }
    SubpassDescription& pPreserveAttachments(const uint32_t* pPreserveAttachments){ m_subpassDescription.pPreserveAttachments = pPreserveAttachments; return *this; }
    operator VkSubpassDescription&() { return m_subpassDescription; }
    operator const VkSubpassDescription&() const { return m_subpassDescription; }
    VkSubpassDescription& vkHandle() { return m_subpassDescription; }
    const VkSubpassDescription& vkHandle() const { return m_subpassDescription; }

  private:
    VkSubpassDescription m_subpassDescription;
  };

  class StencilOpState
  {
  public:
    StencilOpState() : StencilOpState(StencilOp::Keep, StencilOp::Keep, StencilOp::Keep, CompareOp::Never, 0, 0, 0) {}
    StencilOpState(StencilOp failOp, StencilOp passOp, StencilOp depthFailOp, CompareOp compareOp, uint32_t compareMask, uint32_t writeMask, uint32_t reference) {m_stencilOpState.failOp = static_cast<VkStencilOp>( failOp ); m_stencilOpState.passOp = static_cast<VkStencilOp>( passOp ); m_stencilOpState.depthFailOp = static_cast<VkStencilOp>( depthFailOp ); m_stencilOpState.compareOp = static_cast<VkCompareOp>( compareOp ); m_stencilOpState.compareMask = compareMask; m_stencilOpState.writeMask = writeMask; m_stencilOpState.reference = reference; }
    StencilOpState(const VkStencilOpState& other) : m_stencilOpState{other} {}
    StencilOpState& operator=(const VkStencilOpState& other){ m_stencilOpState = other; return *this; }

    const StencilOp& failOp() const { return reinterpret_cast<const StencilOp&>( m_stencilOpState.failOp ); }
    StencilOpState& failOp(StencilOp failOp){ m_stencilOpState.failOp = static_cast<VkStencilOp>( failOp ); return *this; }
    const StencilOp& passOp() const { return reinterpret_cast<const StencilOp&>( m_stencilOpState.passOp ); }
    StencilOpState& passOp(StencilOp passOp){ m_stencilOpState.passOp = static_cast<VkStencilOp>( passOp ); return *this; }
    const StencilOp& depthFailOp() const { return reinterpret_cast<const StencilOp&>( m_stencilOpState.depthFailOp ); }
    StencilOpState& depthFailOp(StencilOp depthFailOp){ m_stencilOpState.depthFailOp = static_cast<VkStencilOp>( depthFailOp ); return *this; }
    const CompareOp& compareOp() const { return reinterpret_cast<const CompareOp&>( m_stencilOpState.compareOp ); }
    StencilOpState& compareOp(CompareOp compareOp){ m_stencilOpState.compareOp = static_cast<VkCompareOp>( compareOp ); return *this; }
    const uint32_t& compareMask() const { return m_stencilOpState.compareMask; }
    StencilOpState& compareMask(uint32_t compareMask){ m_stencilOpState.compareMask = compareMask; return *this; }
    const uint32_t& writeMask() const { return m_stencilOpState.writeMask; }
    StencilOpState& writeMask(uint32_t writeMask){ m_stencilOpState.writeMask = writeMask; return *this; }
    const uint32_t& reference() const { return m_stencilOpState.reference; }
    StencilOpState& reference(uint32_t reference){ m_stencilOpState.reference = reference; return *this; }
    operator VkStencilOpState&() { return m_stencilOpState; }
    operator const VkStencilOpState&() const { return m_stencilOpState; }
    VkStencilOpState& vkHandle() { return m_stencilOpState; }
    const VkStencilOpState& vkHandle() const { return m_stencilOpState; }

  private:
    VkStencilOpState m_stencilOpState;
  };

  class VertexInputBindingDescription
  {
  public:
    VertexInputBindingDescription() : VertexInputBindingDescription(0, 0, VertexInputRate::Vertex) {}
    VertexInputBindingDescription(uint32_t binding, uint32_t stride, VertexInputRate inputRate) {m_vertexInputBindingDescription.binding = binding; m_vertexInputBindingDescription.stride = stride; m_vertexInputBindingDescription.inputRate = static_cast<VkVertexInputRate>( inputRate ); }
    VertexInputBindingDescription(const VkVertexInputBindingDescription& other) : m_vertexInputBindingDescription{other} {}
    VertexInputBindingDescription& operator=(const VkVertexInputBindingDescription& other){ m_vertexInputBindingDescription = other; return *this; }

    const uint32_t& binding() const { return m_vertexInputBindingDescription.binding; }
    VertexInputBindingDescription& binding(uint32_t binding){ m_vertexInputBindingDescription.binding = binding; return *this; }
    const uint32_t& stride() const { return m_vertexInputBindingDescription.stride; }
    VertexInputBindingDescription& stride(uint32_t stride){ m_vertexInputBindingDescription.stride = stride; return *this; }
    const VertexInputRate& inputRate() const { return reinterpret_cast<const VertexInputRate&>( m_vertexInputBindingDescription.inputRate ); }
    VertexInputBindingDescription& inputRate(VertexInputRate inputRate){ m_vertexInputBindingDescription.inputRate = static_cast<VkVertexInputRate>( inputRate ); return *this; }
    operator VkVertexInputBindingDescription&() { return m_vertexInputBindingDescription; }
    operator const VkVertexInputBindingDescription&() const { return m_vertexInputBindingDescription; }
    VkVertexInputBindingDescription& vkHandle() { return m_vertexInputBindingDescription; }
    const VkVertexInputBindingDescription& vkHandle() const { return m_vertexInputBindingDescription; }

  private:
    VkVertexInputBindingDescription m_vertexInputBindingDescription;
  };

  class VertexInputAttributeDescription
  {
  public:
    VertexInputAttributeDescription() : VertexInputAttributeDescription(0, 0, Format::Undefined, 0) {}
    VertexInputAttributeDescription(uint32_t location, uint32_t binding, Format format, uint32_t offset) {m_vertexInputAttributeDescription.location = location; m_vertexInputAttributeDescription.binding = binding; m_vertexInputAttributeDescription.format = static_cast<VkFormat>( format ); m_vertexInputAttributeDescription.offset = offset; }
    VertexInputAttributeDescription(const VkVertexInputAttributeDescription& other) : m_vertexInputAttributeDescription{other} {}
    VertexInputAttributeDescription& operator=(const VkVertexInputAttributeDescription& other){ m_vertexInputAttributeDescription = other; return *this; }

    const uint32_t& location() const { return m_vertexInputAttributeDescription.location; }
    VertexInputAttributeDescription& location(uint32_t location){ m_vertexInputAttributeDescription.location = location; return *this; }
    const uint32_t& binding() const { return m_vertexInputAttributeDescription.binding; }
    VertexInputAttributeDescription& binding(uint32_t binding){ m_vertexInputAttributeDescription.binding = binding; return *this; }
    const Format& format() const { return reinterpret_cast<const Format&>( m_vertexInputAttributeDescription.format ); }
    VertexInputAttributeDescription& format(Format format){ m_vertexInputAttributeDescription.format = static_cast<VkFormat>( format ); return *this; }
    const uint32_t& offset() const { return m_vertexInputAttributeDescription.offset; }
    VertexInputAttributeDescription& offset(uint32_t offset){ m_vertexInputAttributeDescription.offset = offset; return *this; }
    operator VkVertexInputAttributeDescription&() { return m_vertexInputAttributeDescription; }
    operator const VkVertexInputAttributeDescription&() const { return m_vertexInputAttributeDescription; }
    VkVertexInputAttributeDescription& vkHandle() { return m_vertexInputAttributeDescription; }
    const VkVertexInputAttributeDescription& vkHandle() const { return m_vertexInputAttributeDescription; }

  private:
    VkVertexInputAttributeDescription m_vertexInputAttributeDescription;
  };

  class ApplicationInfo
  {
  public:
    ApplicationInfo() : ApplicationInfo(nullptr, 0, nullptr, 0, 0) {}
    ApplicationInfo(const char* pApplicationName, uint32_t applicationVersion, const char* pEngineName, uint32_t engineVersion, uint32_t apiVersion) {m_applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO; m_applicationInfo.pNext = nullptr; m_applicationInfo.pApplicationName = pApplicationName; m_applicationInfo.applicationVersion = applicationVersion; m_applicationInfo.pEngineName = pEngineName; m_applicationInfo.engineVersion = engineVersion; m_applicationInfo.apiVersion = apiVersion; }
    ApplicationInfo(const VkApplicationInfo& other) : m_applicationInfo{other} {}
    ApplicationInfo& operator=(const VkApplicationInfo& other){ m_applicationInfo = other; return *this; }

    const StructureType& sType() const { return reinterpret_cast<const StructureType&>( m_applicationInfo.sType ); }
    ApplicationInfo& sType(StructureType sType){ m_applicationInfo.sType = static_cast<VkStructureType>( sType ); return *this; }
    const void* pNext() const { return reinterpret_cast<const void*>( m_applicationInfo.pNext ); }
    ApplicationInfo& pNext(const void* pNext){ m_applicationInfo.pNext = pNext; return *this; }
    const char* pApplicationName() const { return reinterpret_cast<const char*>( m_applicationInfo.pApplicationName ); }
    ApplicationInfo& pApplicationName(const char* pApplicationName){ m_applicationInfo.pApplicationName = pApplicationName; return *this; }
    const uint32_t& applicationVersion() const { return m_applicationInfo.applicationVersion; }
    ApplicationInfo& applicationVersion(uint32_t applicationVersion){ m_applicationInfo.applicationVersion = applicationVersion; return *this; }
    const char* pEngineName() const { return reinterpret_cast<const char*>( m_applicationInfo.pEngineName ); }
    ApplicationInfo& pEngineName(const char* pEngineName){ m_applicationInfo.pEngineName = pEngineName; return *this; }
    const uint32_t& engineVersion() const { return m_applicationInfo.engineVersion; }
    ApplicationInfo& engineVersion(uint32_t engineVersion){ m_applicationInfo.engineVersion = engineVersion; return *this; }
    const uint32_t& apiVersion() const { return m_applicationInfo.apiVersion; }
    ApplicationInfo& apiVersion(uint32_t apiVersion){ m_applicationInfo.apiVersion = apiVersion; return *this; }
    operator VkApplicationInfo&() { return m_applicationInfo; }
    operator const VkApplicationInfo&() const { return m_applicationInfo; }
    VkApplicationInfo& vkHandle() { return m_applicationInfo; }
    const VkApplicationInfo& vkHandle() const { return m_applicationInfo; }

  private:
    VkApplicationInfo m_applicationInfo;
  };

  class DeviceQueueCreateInfo
  {
  public:
    DeviceQueueCreateInfo() : DeviceQueueCreateInfo(DeviceQueueCreateFlags(), 0, 0, nullptr) {}
    DeviceQueueCreateInfo(DeviceQueueCreateFlags flags, uint32_t queueFamilyIndex, uint32_t queueCount, const float* pQueuePriorities) {m_deviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO; m_deviceQueueCreateInfo.pNext = nullptr; m_deviceQueueCreateInfo.flags = static_cast<VkDeviceQueueCreateFlags>( flags ); m_deviceQueueCreateInfo.queueFamilyIndex = queueFamilyIndex; m_deviceQueueCreateInfo.queueCount = queueCount; m_deviceQueueCreateInfo.pQueuePriorities = pQueuePriorities; }
    DeviceQueueCreateInfo(const VkDeviceQueueCreateInfo& other) : m_deviceQueueCreateInfo{other} {}
    DeviceQueueCreateInfo& operator=(const VkDeviceQueueCreateInfo& other){ m_deviceQueueCreateInfo = other; return *this; }

    const StructureType& sType() const { return reinterpret_cast<const StructureType&>( m_deviceQueueCreateInfo.sType ); }
    DeviceQueueCreateInfo& sType(StructureType sType){ m_deviceQueueCreateInfo.sType = static_cast<VkStructureType>( sType ); return *this; }
    const void* pNext() const { return reinterpret_cast<const void*>( m_deviceQueueCreateInfo.pNext ); }
    DeviceQueueCreateInfo& pNext(const void* pNext){ m_deviceQueueCreateInfo.pNext = pNext; return *this; }
    const DeviceQueueCreateFlags& flags() const { return reinterpret_cast<const DeviceQueueCreateFlags&>( m_deviceQueueCreateInfo.flags ); }
    DeviceQueueCreateInfo& flags(DeviceQueueCreateFlags flags){ m_deviceQueueCreateInfo.flags = static_cast<VkDeviceQueueCreateFlags>( flags ); return *this; }
    const uint32_t& queueFamilyIndex() const { return m_deviceQueueCreateInfo.queueFamilyIndex; }
    DeviceQueueCreateInfo& queueFamilyIndex(uint32_t queueFamilyIndex){ m_deviceQueueCreateInfo.queueFamilyIndex = queueFamilyIndex; return *this; }
    const uint32_t& queueCount() const { return m_deviceQueueCreateInfo.queueCount; }
    DeviceQueueCreateInfo& queueCount(uint32_t queueCount){ m_deviceQueueCreateInfo.queueCount = queueCount; return *this; }
    const float* pQueuePriorities() const { return reinterpret_cast<const float*>( m_deviceQueueCreateInfo.pQueuePriorities ); }
    DeviceQueueCreateInfo& pQueuePriorities(const float* pQueuePriorities){ m_deviceQueueCreateInfo.pQueuePriorities = pQueuePriorities; return *this; }
    operator VkDeviceQueueCreateInfo&() { return m_deviceQueueCreateInfo; }
    operator const VkDeviceQueueCreateInfo&() const { return m_deviceQueueCreateInfo; }
    VkDeviceQueueCreateInfo& vkHandle() { return m_deviceQueueCreateInfo; }
    const VkDeviceQueueCreateInfo& vkHandle() const { return m_deviceQueueCreateInfo; }

  private:
    VkDeviceQueueCreateInfo m_deviceQueueCreateInfo;
  };

  class DeviceCreateInfo
  {
  public:
    DeviceCreateInfo() : DeviceCreateInfo(DeviceCreateFlags(), 0, nullptr, 0, nullptr, 0, nullptr, nullptr) {}
    DeviceCreateInfo(DeviceCreateFlags flags, uint32_t queueCreateInfoCount, const DeviceQueueCreateInfo* pQueueCreateInfos, uint32_t enabledLayerCount, const char* const* ppEnabledLayerNames, uint32_t enabledExtensionCount, const char* const* ppEnabledExtensionNames, const PhysicalDeviceFeatures* pEnabledFeatures) {m_deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO; m_deviceCreateInfo.pNext = nullptr; m_deviceCreateInfo.flags = static_cast<VkDeviceCreateFlags>( flags ); m_deviceCreateInfo.queueCreateInfoCount = queueCreateInfoCount; m_deviceCreateInfo.pQueueCreateInfos = reinterpret_cast<const VkDeviceQueueCreateInfo*>( pQueueCreateInfos ); m_deviceCreateInfo.enabledLayerCount = enabledLayerCount; m_deviceCreateInfo.ppEnabledLayerNames = ppEnabledLayerNames; m_deviceCreateInfo.enabledExtensionCount = enabledExtensionCount; m_deviceCreateInfo.ppEnabledExtensionNames = ppEnabledExtensionNames; m_deviceCreateInfo.pEnabledFeatures = reinterpret_cast<const VkPhysicalDeviceFeatures*>( pEnabledFeatures ); }
    DeviceCreateInfo(const VkDeviceCreateInfo& other) : m_deviceCreateInfo{other} {}
    DeviceCreateInfo& operator=(const VkDeviceCreateInfo& other){ m_deviceCreateInfo = other; return *this; }

    const StructureType& sType() const { return reinterpret_cast<const StructureType&>( m_deviceCreateInfo.sType ); }
    DeviceCreateInfo& sType(StructureType sType){ m_deviceCreateInfo.sType = static_cast<VkStructureType>( sType ); return *this; }
    const void* pNext() const { return reinterpret_cast<const void*>( m_deviceCreateInfo.pNext ); }
    DeviceCreateInfo& pNext(const void* pNext){ m_deviceCreateInfo.pNext = pNext; return *this; }
    const DeviceCreateFlags& flags() const { return reinterpret_cast<const DeviceCreateFlags&>( m_deviceCreateInfo.flags ); }
    DeviceCreateInfo& flags(DeviceCreateFlags flags){ m_deviceCreateInfo.flags = static_cast<VkDeviceCreateFlags>( flags ); return *this; }
    const uint32_t& queueCreateInfoCount() const { return m_deviceCreateInfo.queueCreateInfoCount; }
    DeviceCreateInfo& queueCreateInfoCount(uint32_t queueCreateInfoCount){ m_deviceCreateInfo.queueCreateInfoCount = queueCreateInfoCount; return *this; }
    const DeviceQueueCreateInfo* pQueueCreateInfos() const { return reinterpret_cast<const DeviceQueueCreateInfo*>( m_deviceCreateInfo.pQueueCreateInfos ); }
    DeviceCreateInfo& pQueueCreateInfos(const DeviceQueueCreateInfo* pQueueCreateInfos){ m_deviceCreateInfo.pQueueCreateInfos = reinterpret_cast<const VkDeviceQueueCreateInfo*>( pQueueCreateInfos ); return *this; }
    const uint32_t& enabledLayerCount() const { return m_deviceCreateInfo.enabledLayerCount; }
    DeviceCreateInfo& enabledLayerCount(uint32_t enabledLayerCount){ m_deviceCreateInfo.enabledLayerCount = enabledLayerCount; return *this; }
    const char* const* ppEnabledLayerNames() const { return reinterpret_cast<const char* const*>( m_deviceCreateInfo.ppEnabledLayerNames ); }
    DeviceCreateInfo& ppEnabledLayerNames(const char* const* ppEnabledLayerNames){ m_deviceCreateInfo.ppEnabledLayerNames = ppEnabledLayerNames; return *this; }
    const uint32_t& enabledExtensionCount() const { return m_deviceCreateInfo.enabledExtensionCount; }
    DeviceCreateInfo& enabledExtensionCount(uint32_t enabledExtensionCount){ m_deviceCreateInfo.enabledExtensionCount = enabledExtensionCount; return *this; }
    const char* const* ppEnabledExtensionNames() const { return reinterpret_cast<const char* const*>( m_deviceCreateInfo.ppEnabledExtensionNames ); }
    DeviceCreateInfo& ppEnabledExtensionNames(const char* const* ppEnabledExtensionNames){ m_deviceCreateInfo.ppEnabledExtensionNames = ppEnabledExtensionNames; return *this; }
    const PhysicalDeviceFeatures* pEnabledFeatures() const { return reinterpret_cast<const PhysicalDeviceFeatures*>( m_deviceCreateInfo.pEnabledFeatures ); }
    DeviceCreateInfo& pEnabledFeatures(const PhysicalDeviceFeatures* pEnabledFeatures){ m_deviceCreateInfo.pEnabledFeatures = reinterpret_cast<const VkPhysicalDeviceFeatures*>( pEnabledFeatures ); return *this; }
    operator VkDeviceCreateInfo&() { return m_deviceCreateInfo; }
    operator const VkDeviceCreateInfo&() const { return m_deviceCreateInfo; }
    VkDeviceCreateInfo& vkHandle() { return m_deviceCreateInfo; }
    const VkDeviceCreateInfo& vkHandle() const { return m_deviceCreateInfo; }

  private:
    VkDeviceCreateInfo m_deviceCreateInfo;
  };

  class InstanceCreateInfo
  {
  public:
    InstanceCreateInfo() : InstanceCreateInfo(InstanceCreateFlags(), nullptr, 0, nullptr, 0, nullptr) {}
    InstanceCreateInfo(InstanceCreateFlags flags, const ApplicationInfo* pApplicationInfo, uint32_t enabledLayerCount, const char* const* ppEnabledLayerNames, uint32_t enabledExtensionCount, const char* const* ppEnabledExtensionNames) {m_instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO; m_instanceCreateInfo.pNext = nullptr; m_instanceCreateInfo.flags = static_cast<VkInstanceCreateFlags>( flags ); m_instanceCreateInfo.pApplicationInfo = reinterpret_cast<const VkApplicationInfo*>( pApplicationInfo ); m_instanceCreateInfo.enabledLayerCount = enabledLayerCount; m_instanceCreateInfo.ppEnabledLayerNames = ppEnabledLayerNames; m_instanceCreateInfo.enabledExtensionCount = enabledExtensionCount; m_instanceCreateInfo.ppEnabledExtensionNames = ppEnabledExtensionNames; }
    InstanceCreateInfo(const VkInstanceCreateInfo& other) : m_instanceCreateInfo{other} {}
    InstanceCreateInfo& operator=(const VkInstanceCreateInfo& other){ m_instanceCreateInfo = other; return *this; }

    const StructureType& sType() const { return reinterpret_cast<const StructureType&>( m_instanceCreateInfo.sType ); }
    InstanceCreateInfo& sType(StructureType sType){ m_instanceCreateInfo.sType = static_cast<VkStructureType>( sType ); return *this; }
    const void* pNext() const { return reinterpret_cast<const void*>( m_instanceCreateInfo.pNext ); }
    InstanceCreateInfo& pNext(const void* pNext){ m_instanceCreateInfo.pNext = pNext; return *this; }
    const InstanceCreateFlags& flags() const { return reinterpret_cast<const InstanceCreateFlags&>( m_instanceCreateInfo.flags ); }
    InstanceCreateInfo& flags(InstanceCreateFlags flags){ m_instanceCreateInfo.flags = static_cast<VkInstanceCreateFlags>( flags ); return *this; }
    const ApplicationInfo* pApplicationInfo() const { return reinterpret_cast<const ApplicationInfo*>( m_instanceCreateInfo.pApplicationInfo ); }
    InstanceCreateInfo& pApplicationInfo(const ApplicationInfo* pApplicationInfo){ m_instanceCreateInfo.pApplicationInfo = reinterpret_cast<const VkApplicationInfo*>( pApplicationInfo ); return *this; }
    const uint32_t& enabledLayerCount() const { return m_instanceCreateInfo.enabledLayerCount; }
    InstanceCreateInfo& enabledLayerCount(uint32_t enabledLayerCount){ m_instanceCreateInfo.enabledLayerCount = enabledLayerCount; return *this; }
    const char* const* ppEnabledLayerNames() const { return reinterpret_cast<const char* const*>( m_instanceCreateInfo.ppEnabledLayerNames ); }
    InstanceCreateInfo& ppEnabledLayerNames(const char* const* ppEnabledLayerNames){ m_instanceCreateInfo.ppEnabledLayerNames = ppEnabledLayerNames; return *this; }
    const uint32_t& enabledExtensionCount() const { return m_instanceCreateInfo.enabledExtensionCount; }
    InstanceCreateInfo& enabledExtensionCount(uint32_t enabledExtensionCount){ m_instanceCreateInfo.enabledExtensionCount = enabledExtensionCount; return *this; }
    const char* const* ppEnabledExtensionNames() const { return reinterpret_cast<const char* const*>( m_instanceCreateInfo.ppEnabledExtensionNames ); }
    InstanceCreateInfo& ppEnabledExtensionNames(const char* const* ppEnabledExtensionNames){ m_instanceCreateInfo.ppEnabledExtensionNames = ppEnabledExtensionNames; return *this; }
    operator VkInstanceCreateInfo&() { return m_instanceCreateInfo; }
    operator const VkInstanceCreateInfo&() const { return m_instanceCreateInfo; }
    VkInstanceCreateInfo& vkHandle() { return m_instanceCreateInfo; }
    const VkInstanceCreateInfo& vkHandle() const { return m_instanceCreateInfo; }

  private:
    VkInstanceCreateInfo m_instanceCreateInfo;
  };

  class MemoryAllocateInfo
  {
  public:
    MemoryAllocateInfo() : MemoryAllocateInfo(0, 0) {}
    MemoryAllocateInfo(DeviceSize allocationSize, uint32_t memoryTypeIndex) {m_memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO; m_memoryAllocateInfo.pNext = nullptr; m_memoryAllocateInfo.allocationSize = allocationSize; m_memoryAllocateInfo.memoryTypeIndex = memoryTypeIndex; }
    MemoryAllocateInfo(const VkMemoryAllocateInfo& other) : m_memoryAllocateInfo{other} {}
    MemoryAllocateInfo& operator=(const VkMemoryAllocateInfo& other){ m_memoryAllocateInfo = other; return *this; }

    const StructureType& sType() const { return reinterpret_cast<const StructureType&>( m_memoryAllocateInfo.sType ); }
    MemoryAllocateInfo& sType(StructureType sType){ m_memoryAllocateInfo.sType = static_cast<VkStructureType>( sType ); return *this; }
    const void* pNext() const { return reinterpret_cast<const void*>( m_memoryAllocateInfo.pNext ); }
    MemoryAllocateInfo& pNext(const void* pNext){ m_memoryAllocateInfo.pNext = pNext; return *this; }
    const DeviceSize& allocationSize() const { return m_memoryAllocateInfo.allocationSize; }
    MemoryAllocateInfo& allocationSize(DeviceSize allocationSize){ m_memoryAllocateInfo.allocationSize = allocationSize; return *this; }
    const uint32_t& memoryTypeIndex() const { return m_memoryAllocateInfo.memoryTypeIndex; }
    MemoryAllocateInfo& memoryTypeIndex(uint32_t memoryTypeIndex){ m_memoryAllocateInfo.memoryTypeIndex = memoryTypeIndex; return *this; }
    operator VkMemoryAllocateInfo&() { return m_memoryAllocateInfo; }
    operator const VkMemoryAllocateInfo&() const { return m_memoryAllocateInfo; }
    VkMemoryAllocateInfo& vkHandle() { return m_memoryAllocateInfo; }
    const VkMemoryAllocateInfo& vkHandle() const { return m_memoryAllocateInfo; }

  private:
    VkMemoryAllocateInfo m_memoryAllocateInfo;
  };

  class MappedMemoryRange
  {
  public:
    MappedMemoryRange() : MappedMemoryRange(DeviceMemory(), 0, 0) {}
    MappedMemoryRange(DeviceMemory memory, DeviceSize offset, DeviceSize size) {m_mappedMemoryRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE; m_mappedMemoryRange.pNext = nullptr; m_mappedMemoryRange.memory = memory; m_mappedMemoryRange.offset = offset; m_mappedMemoryRange.size = size; }
    MappedMemoryRange(const VkMappedMemoryRange& other) : m_mappedMemoryRange{other} {}
    MappedMemoryRange& operator=(const VkMappedMemoryRange& other){ m_mappedMemoryRange = other; return *this; }

    const StructureType& sType() const { return reinterpret_cast<const StructureType&>( m_mappedMemoryRange.sType ); }
    MappedMemoryRange& sType(StructureType sType){ m_mappedMemoryRange.sType = static_cast<VkStructureType>( sType ); return *this; }
    const void* pNext() const { return reinterpret_cast<const void*>( m_mappedMemoryRange.pNext ); }
    MappedMemoryRange& pNext(const void* pNext){ m_mappedMemoryRange.pNext = pNext; return *this; }
    const DeviceMemory& memory() const { return m_mappedMemoryRange.memory; }
    MappedMemoryRange& memory(DeviceMemory memory){ m_mappedMemoryRange.memory = memory; return *this; }
    const DeviceSize& offset() const { return m_mappedMemoryRange.offset; }
    MappedMemoryRange& offset(DeviceSize offset){ m_mappedMemoryRange.offset = offset; return *this; }
    const DeviceSize& size() const { return m_mappedMemoryRange.size; }
    MappedMemoryRange& size(DeviceSize size){ m_mappedMemoryRange.size = size; return *this; }
    operator VkMappedMemoryRange&() { return m_mappedMemoryRange; }
    operator const VkMappedMemoryRange&() const { return m_mappedMemoryRange; }
    VkMappedMemoryRange& vkHandle() { return m_mappedMemoryRange; }
    const VkMappedMemoryRange& vkHandle() const { return m_mappedMemoryRange; }

  private:
    VkMappedMemoryRange m_mappedMemoryRange;
  };

  class WriteDescriptorSet
  {
  public:
    WriteDescriptorSet() : WriteDescriptorSet(DescriptorSet(), 0, 0, 0, DescriptorType::Sampler, nullptr, nullptr, nullptr) {}
    WriteDescriptorSet(DescriptorSet dstSet, uint32_t dstBinding, uint32_t dstArrayElement, uint32_t descriptorCount, DescriptorType descriptorType, const DescriptorImageInfo* pImageInfo, const DescriptorBufferInfo* pBufferInfo, const BufferView* pTexelBufferView) {m_writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET; m_writeDescriptorSet.pNext = nullptr; m_writeDescriptorSet.dstSet = dstSet; m_writeDescriptorSet.dstBinding = dstBinding; m_writeDescriptorSet.dstArrayElement = dstArrayElement; m_writeDescriptorSet.descriptorCount = descriptorCount; m_writeDescriptorSet.descriptorType = static_cast<VkDescriptorType>( descriptorType ); m_writeDescriptorSet.pImageInfo = reinterpret_cast<const VkDescriptorImageInfo*>( pImageInfo ); m_writeDescriptorSet.pBufferInfo = reinterpret_cast<const VkDescriptorBufferInfo*>( pBufferInfo ); m_writeDescriptorSet.pTexelBufferView = pTexelBufferView; }
    WriteDescriptorSet(const VkWriteDescriptorSet& other) : m_writeDescriptorSet{other} {}
    WriteDescriptorSet& operator=(const VkWriteDescriptorSet& other){ m_writeDescriptorSet = other; return *this; }

    const StructureType& sType() const { return reinterpret_cast<const StructureType&>( m_writeDescriptorSet.sType ); }
    WriteDescriptorSet& sType(StructureType sType){ m_writeDescriptorSet.sType = static_cast<VkStructureType>( sType ); return *this; }
    const void* pNext() const { return reinterpret_cast<const void*>( m_writeDescriptorSet.pNext ); }
    WriteDescriptorSet& pNext(const void* pNext){ m_writeDescriptorSet.pNext = pNext; return *this; }
    const DescriptorSet& dstSet() const { return m_writeDescriptorSet.dstSet; }
    WriteDescriptorSet& dstSet(DescriptorSet dstSet){ m_writeDescriptorSet.dstSet = dstSet; return *this; }
    const uint32_t& dstBinding() const { return m_writeDescriptorSet.dstBinding; }
    WriteDescriptorSet& dstBinding(uint32_t dstBinding){ m_writeDescriptorSet.dstBinding = dstBinding; return *this; }
    const uint32_t& dstArrayElement() const { return m_writeDescriptorSet.dstArrayElement; }
    WriteDescriptorSet& dstArrayElement(uint32_t dstArrayElement){ m_writeDescriptorSet.dstArrayElement = dstArrayElement; return *this; }
    const uint32_t& descriptorCount() const { return m_writeDescriptorSet.descriptorCount; }
    WriteDescriptorSet& descriptorCount(uint32_t descriptorCount){ m_writeDescriptorSet.descriptorCount = descriptorCount; return *this; }
    const DescriptorType& descriptorType() const { return reinterpret_cast<const DescriptorType&>( m_writeDescriptorSet.descriptorType ); }
    WriteDescriptorSet& descriptorType(DescriptorType descriptorType){ m_writeDescriptorSet.descriptorType = static_cast<VkDescriptorType>( descriptorType ); return *this; }
    const DescriptorImageInfo* pImageInfo() const { return reinterpret_cast<const DescriptorImageInfo*>( m_writeDescriptorSet.pImageInfo ); }
    WriteDescriptorSet& pImageInfo(const DescriptorImageInfo* pImageInfo){ m_writeDescriptorSet.pImageInfo = reinterpret_cast<const VkDescriptorImageInfo*>( pImageInfo ); return *this; }
    const DescriptorBufferInfo* pBufferInfo() const { return reinterpret_cast<const DescriptorBufferInfo*>( m_writeDescriptorSet.pBufferInfo ); }
    WriteDescriptorSet& pBufferInfo(const DescriptorBufferInfo* pBufferInfo){ m_writeDescriptorSet.pBufferInfo = reinterpret_cast<const VkDescriptorBufferInfo*>( pBufferInfo ); return *this; }
    const BufferView* pTexelBufferView() const { return reinterpret_cast<const BufferView*>( m_writeDescriptorSet.pTexelBufferView ); }
    WriteDescriptorSet& pTexelBufferView(const BufferView* pTexelBufferView){ m_writeDescriptorSet.pTexelBufferView = pTexelBufferView; return *this; }
    operator VkWriteDescriptorSet&() { return m_writeDescriptorSet; }
    operator const VkWriteDescriptorSet&() const { return m_writeDescriptorSet; }
    VkWriteDescriptorSet& vkHandle() { return m_writeDescriptorSet; }
    const VkWriteDescriptorSet& vkHandle() const { return m_writeDescriptorSet; }

  private:
    VkWriteDescriptorSet m_writeDescriptorSet;
  };

  class CopyDescriptorSet
  {
  public:
    CopyDescriptorSet() : CopyDescriptorSet(DescriptorSet(), 0, 0, DescriptorSet(), 0, 0, 0) {}
    CopyDescriptorSet(DescriptorSet srcSet, uint32_t srcBinding, uint32_t srcArrayElement, DescriptorSet dstSet, uint32_t dstBinding, uint32_t dstArrayElement, uint32_t descriptorCount) {m_copyDescriptorSet.sType = VK_STRUCTURE_TYPE_COPY_DESCRIPTOR_SET; m_copyDescriptorSet.pNext = nullptr; m_copyDescriptorSet.srcSet = srcSet; m_copyDescriptorSet.srcBinding = srcBinding; m_copyDescriptorSet.srcArrayElement = srcArrayElement; m_copyDescriptorSet.dstSet = dstSet; m_copyDescriptorSet.dstBinding = dstBinding; m_copyDescriptorSet.dstArrayElement = dstArrayElement; m_copyDescriptorSet.descriptorCount = descriptorCount; }
    CopyDescriptorSet(const VkCopyDescriptorSet& other) : m_copyDescriptorSet{other} {}
    CopyDescriptorSet& operator=(const VkCopyDescriptorSet& other){ m_copyDescriptorSet = other; return *this; }

    const StructureType& sType() const { return reinterpret_cast<const StructureType&>( m_copyDescriptorSet.sType ); }
    CopyDescriptorSet& sType(StructureType sType){ m_copyDescriptorSet.sType = static_cast<VkStructureType>( sType ); return *this; }
    const void* pNext() const { return reinterpret_cast<const void*>( m_copyDescriptorSet.pNext ); }
    CopyDescriptorSet& pNext(const void* pNext){ m_copyDescriptorSet.pNext = pNext; return *this; }
    const DescriptorSet& srcSet() const { return m_copyDescriptorSet.srcSet; }
    CopyDescriptorSet& srcSet(DescriptorSet srcSet){ m_copyDescriptorSet.srcSet = srcSet; return *this; }
    const uint32_t& srcBinding() const { return m_copyDescriptorSet.srcBinding; }
    CopyDescriptorSet& srcBinding(uint32_t srcBinding){ m_copyDescriptorSet.srcBinding = srcBinding; return *this; }
    const uint32_t& srcArrayElement() const { return m_copyDescriptorSet.srcArrayElement; }
    CopyDescriptorSet& srcArrayElement(uint32_t srcArrayElement){ m_copyDescriptorSet.srcArrayElement = srcArrayElement; return *this; }
    const DescriptorSet& dstSet() const { return m_copyDescriptorSet.dstSet; }
    CopyDescriptorSet& dstSet(DescriptorSet dstSet){ m_copyDescriptorSet.dstSet = dstSet; return *this; }
    const uint32_t& dstBinding() const { return m_copyDescriptorSet.dstBinding; }
    CopyDescriptorSet& dstBinding(uint32_t dstBinding){ m_copyDescriptorSet.dstBinding = dstBinding; return *this; }
    const uint32_t& dstArrayElement() const { return m_copyDescriptorSet.dstArrayElement; }
    CopyDescriptorSet& dstArrayElement(uint32_t dstArrayElement){ m_copyDescriptorSet.dstArrayElement = dstArrayElement; return *this; }
    const uint32_t& descriptorCount() const { return m_copyDescriptorSet.descriptorCount; }
    CopyDescriptorSet& descriptorCount(uint32_t descriptorCount){ m_copyDescriptorSet.descriptorCount = descriptorCount; return *this; }
    operator VkCopyDescriptorSet&() { return m_copyDescriptorSet; }
    operator const VkCopyDescriptorSet&() const { return m_copyDescriptorSet; }
    VkCopyDescriptorSet& vkHandle() { return m_copyDescriptorSet; }
    const VkCopyDescriptorSet& vkHandle() const { return m_copyDescriptorSet; }

  private:
    VkCopyDescriptorSet m_copyDescriptorSet;
  };

  class BufferViewCreateInfo
  {
  public:
    BufferViewCreateInfo() : BufferViewCreateInfo(BufferViewCreateFlags(), Buffer(), Format::Undefined, 0, 0) {}
    BufferViewCreateInfo(BufferViewCreateFlags flags, Buffer buffer, Format format, DeviceSize offset, DeviceSize range) {m_bufferViewCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO; m_bufferViewCreateInfo.pNext = nullptr; m_bufferViewCreateInfo.flags = static_cast<VkBufferViewCreateFlags>( flags ); m_bufferViewCreateInfo.buffer = buffer; m_bufferViewCreateInfo.format = static_cast<VkFormat>( format ); m_bufferViewCreateInfo.offset = offset; m_bufferViewCreateInfo.range = range; }
    BufferViewCreateInfo(const VkBufferViewCreateInfo& other) : m_bufferViewCreateInfo{other} {}
    BufferViewCreateInfo& operator=(const VkBufferViewCreateInfo& other){ m_bufferViewCreateInfo = other; return *this; }

    const StructureType& sType() const { return reinterpret_cast<const StructureType&>( m_bufferViewCreateInfo.sType ); }
    BufferViewCreateInfo& sType(StructureType sType){ m_bufferViewCreateInfo.sType = static_cast<VkStructureType>( sType ); return *this; }
    const void* pNext() const { return reinterpret_cast<const void*>( m_bufferViewCreateInfo.pNext ); }
    BufferViewCreateInfo& pNext(const void* pNext){ m_bufferViewCreateInfo.pNext = pNext; return *this; }
    const BufferViewCreateFlags& flags() const { return reinterpret_cast<const BufferViewCreateFlags&>( m_bufferViewCreateInfo.flags ); }
    BufferViewCreateInfo& flags(BufferViewCreateFlags flags){ m_bufferViewCreateInfo.flags = static_cast<VkBufferViewCreateFlags>( flags ); return *this; }
    const Buffer& buffer() const { return m_bufferViewCreateInfo.buffer; }
    BufferViewCreateInfo& buffer(Buffer buffer){ m_bufferViewCreateInfo.buffer = buffer; return *this; }
    const Format& format() const { return reinterpret_cast<const Format&>( m_bufferViewCreateInfo.format ); }
    BufferViewCreateInfo& format(Format format){ m_bufferViewCreateInfo.format = static_cast<VkFormat>( format ); return *this; }
    const DeviceSize& offset() const { return m_bufferViewCreateInfo.offset; }
    BufferViewCreateInfo& offset(DeviceSize offset){ m_bufferViewCreateInfo.offset = offset; return *this; }
    const DeviceSize& range() const { return m_bufferViewCreateInfo.range; }
    BufferViewCreateInfo& range(DeviceSize range){ m_bufferViewCreateInfo.range = range; return *this; }
    operator VkBufferViewCreateInfo&() { return m_bufferViewCreateInfo; }
    operator const VkBufferViewCreateInfo&() const { return m_bufferViewCreateInfo; }
    VkBufferViewCreateInfo& vkHandle() { return m_bufferViewCreateInfo; }
    const VkBufferViewCreateInfo& vkHandle() const { return m_bufferViewCreateInfo; }

  private:
    VkBufferViewCreateInfo m_bufferViewCreateInfo;
  };

  class ShaderModuleCreateInfo
  {
  public:
    ShaderModuleCreateInfo() : ShaderModuleCreateInfo(ShaderModuleCreateFlags(), 0, nullptr) {}
    ShaderModuleCreateInfo(ShaderModuleCreateFlags flags, size_t codeSize, const uint32_t* pCode) {m_shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO; m_shaderModuleCreateInfo.pNext = nullptr; m_shaderModuleCreateInfo.flags = static_cast<VkShaderModuleCreateFlags>( flags ); m_shaderModuleCreateInfo.codeSize = codeSize; m_shaderModuleCreateInfo.pCode = pCode; }
    ShaderModuleCreateInfo(const VkShaderModuleCreateInfo& other) : m_shaderModuleCreateInfo{other} {}
    ShaderModuleCreateInfo& operator=(const VkShaderModuleCreateInfo& other){ m_shaderModuleCreateInfo = other; return *this; }

    const StructureType& sType() const { return reinterpret_cast<const StructureType&>( m_shaderModuleCreateInfo.sType ); }
    ShaderModuleCreateInfo& sType(StructureType sType){ m_shaderModuleCreateInfo.sType = static_cast<VkStructureType>( sType ); return *this; }
    const void* pNext() const { return reinterpret_cast<const void*>( m_shaderModuleCreateInfo.pNext ); }
    ShaderModuleCreateInfo& pNext(const void* pNext){ m_shaderModuleCreateInfo.pNext = pNext; return *this; }
    const ShaderModuleCreateFlags& flags() const { return reinterpret_cast<const ShaderModuleCreateFlags&>( m_shaderModuleCreateInfo.flags ); }
    ShaderModuleCreateInfo& flags(ShaderModuleCreateFlags flags){ m_shaderModuleCreateInfo.flags = static_cast<VkShaderModuleCreateFlags>( flags ); return *this; }
    const size_t& codeSize() const { return m_shaderModuleCreateInfo.codeSize; }
    ShaderModuleCreateInfo& codeSize(size_t codeSize){ m_shaderModuleCreateInfo.codeSize = codeSize; return *this; }
    const uint32_t* pCode() const { return reinterpret_cast<const uint32_t*>( m_shaderModuleCreateInfo.pCode ); }
    ShaderModuleCreateInfo& pCode(const uint32_t* pCode){ m_shaderModuleCreateInfo.pCode = pCode; return *this; }
    operator VkShaderModuleCreateInfo&() { return m_shaderModuleCreateInfo; }
    operator const VkShaderModuleCreateInfo&() const { return m_shaderModuleCreateInfo; }
    VkShaderModuleCreateInfo& vkHandle() { return m_shaderModuleCreateInfo; }
    const VkShaderModuleCreateInfo& vkHandle() const { return m_shaderModuleCreateInfo; }

  private:
    VkShaderModuleCreateInfo m_shaderModuleCreateInfo;
  };

  class DescriptorSetAllocateInfo
  {
  public:
    DescriptorSetAllocateInfo() : DescriptorSetAllocateInfo(DescriptorPool(), 0, nullptr) {}
    DescriptorSetAllocateInfo(DescriptorPool descriptorPool, uint32_t descriptorSetCount, const DescriptorSetLayout* pSetLayouts) {m_descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO; m_descriptorSetAllocateInfo.pNext = nullptr; m_descriptorSetAllocateInfo.descriptorPool = descriptorPool; m_descriptorSetAllocateInfo.descriptorSetCount = descriptorSetCount; m_descriptorSetAllocateInfo.pSetLayouts = pSetLayouts; }
    DescriptorSetAllocateInfo(const VkDescriptorSetAllocateInfo& other) : m_descriptorSetAllocateInfo{other} {}
    DescriptorSetAllocateInfo& operator=(const VkDescriptorSetAllocateInfo& other){ m_descriptorSetAllocateInfo = other; return *this; }

    const StructureType& sType() const { return reinterpret_cast<const StructureType&>( m_descriptorSetAllocateInfo.sType ); }
    DescriptorSetAllocateInfo& sType(StructureType sType){ m_descriptorSetAllocateInfo.sType = static_cast<VkStructureType>( sType ); return *this; }
    const void* pNext() const { return reinterpret_cast<const void*>( m_descriptorSetAllocateInfo.pNext ); }
    DescriptorSetAllocateInfo& pNext(const void* pNext){ m_descriptorSetAllocateInfo.pNext = pNext; return *this; }
    const DescriptorPool& descriptorPool() const { return m_descriptorSetAllocateInfo.descriptorPool; }
    DescriptorSetAllocateInfo& descriptorPool(DescriptorPool descriptorPool){ m_descriptorSetAllocateInfo.descriptorPool = descriptorPool; return *this; }
    const uint32_t& descriptorSetCount() const { return m_descriptorSetAllocateInfo.descriptorSetCount; }
    DescriptorSetAllocateInfo& descriptorSetCount(uint32_t descriptorSetCount){ m_descriptorSetAllocateInfo.descriptorSetCount = descriptorSetCount; return *this; }
    const DescriptorSetLayout* pSetLayouts() const { return reinterpret_cast<const DescriptorSetLayout*>( m_descriptorSetAllocateInfo.pSetLayouts ); }
    DescriptorSetAllocateInfo& pSetLayouts(const DescriptorSetLayout* pSetLayouts){ m_descriptorSetAllocateInfo.pSetLayouts = pSetLayouts; return *this; }
    operator VkDescriptorSetAllocateInfo&() { return m_descriptorSetAllocateInfo; }
    operator const VkDescriptorSetAllocateInfo&() const { return m_descriptorSetAllocateInfo; }
    VkDescriptorSetAllocateInfo& vkHandle() { return m_descriptorSetAllocateInfo; }
    const VkDescriptorSetAllocateInfo& vkHandle() const { return m_descriptorSetAllocateInfo; }

  private:
    VkDescriptorSetAllocateInfo m_descriptorSetAllocateInfo;
  };

  class PipelineVertexInputStateCreateInfo
  {
  public:
    PipelineVertexInputStateCreateInfo() : PipelineVertexInputStateCreateInfo(PipelineVertexInputStateCreateFlags(), 0, nullptr, 0, nullptr) {}
    PipelineVertexInputStateCreateInfo(PipelineVertexInputStateCreateFlags flags, uint32_t vertexBindingDescriptionCount, const VertexInputBindingDescription* pVertexBindingDescriptions, uint32_t vertexAttributeDescriptionCount, const VertexInputAttributeDescription* pVertexAttributeDescriptions) {m_pipelineVertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO; m_pipelineVertexInputStateCreateInfo.pNext = nullptr; m_pipelineVertexInputStateCreateInfo.flags = static_cast<VkPipelineVertexInputStateCreateFlags>( flags ); m_pipelineVertexInputStateCreateInfo.vertexBindingDescriptionCount = vertexBindingDescriptionCount; m_pipelineVertexInputStateCreateInfo.pVertexBindingDescriptions = reinterpret_cast<const VkVertexInputBindingDescription*>( pVertexBindingDescriptions ); m_pipelineVertexInputStateCreateInfo.vertexAttributeDescriptionCount = vertexAttributeDescriptionCount; m_pipelineVertexInputStateCreateInfo.pVertexAttributeDescriptions = reinterpret_cast<const VkVertexInputAttributeDescription*>( pVertexAttributeDescriptions ); }
    PipelineVertexInputStateCreateInfo(const VkPipelineVertexInputStateCreateInfo& other) : m_pipelineVertexInputStateCreateInfo{other} {}
    PipelineVertexInputStateCreateInfo& operator=(const VkPipelineVertexInputStateCreateInfo& other){ m_pipelineVertexInputStateCreateInfo = other; return *this; }

    const StructureType& sType() const { return reinterpret_cast<const StructureType&>( m_pipelineVertexInputStateCreateInfo.sType ); }
    PipelineVertexInputStateCreateInfo& sType(StructureType sType){ m_pipelineVertexInputStateCreateInfo.sType = static_cast<VkStructureType>( sType ); return *this; }
    const void* pNext() const { return reinterpret_cast<const void*>( m_pipelineVertexInputStateCreateInfo.pNext ); }
    PipelineVertexInputStateCreateInfo& pNext(const void* pNext){ m_pipelineVertexInputStateCreateInfo.pNext = pNext; return *this; }
    const PipelineVertexInputStateCreateFlags& flags() const { return reinterpret_cast<const PipelineVertexInputStateCreateFlags&>( m_pipelineVertexInputStateCreateInfo.flags ); }
    PipelineVertexInputStateCreateInfo& flags(PipelineVertexInputStateCreateFlags flags){ m_pipelineVertexInputStateCreateInfo.flags = static_cast<VkPipelineVertexInputStateCreateFlags>( flags ); return *this; }
    const uint32_t& vertexBindingDescriptionCount() const { return m_pipelineVertexInputStateCreateInfo.vertexBindingDescriptionCount; }
    PipelineVertexInputStateCreateInfo& vertexBindingDescriptionCount(uint32_t vertexBindingDescriptionCount){ m_pipelineVertexInputStateCreateInfo.vertexBindingDescriptionCount = vertexBindingDescriptionCount; return *this; }
    const VertexInputBindingDescription* pVertexBindingDescriptions() const { return reinterpret_cast<const VertexInputBindingDescription*>( m_pipelineVertexInputStateCreateInfo.pVertexBindingDescriptions ); }
    PipelineVertexInputStateCreateInfo& pVertexBindingDescriptions(const VertexInputBindingDescription* pVertexBindingDescriptions){ m_pipelineVertexInputStateCreateInfo.pVertexBindingDescriptions = reinterpret_cast<const VkVertexInputBindingDescription*>( pVertexBindingDescriptions ); return *this; }
    const uint32_t& vertexAttributeDescriptionCount() const { return m_pipelineVertexInputStateCreateInfo.vertexAttributeDescriptionCount; }
    PipelineVertexInputStateCreateInfo& vertexAttributeDescriptionCount(uint32_t vertexAttributeDescriptionCount){ m_pipelineVertexInputStateCreateInfo.vertexAttributeDescriptionCount = vertexAttributeDescriptionCount; return *this; }
    const VertexInputAttributeDescription* pVertexAttributeDescriptions() const { return reinterpret_cast<const VertexInputAttributeDescription*>( m_pipelineVertexInputStateCreateInfo.pVertexAttributeDescriptions ); }
    PipelineVertexInputStateCreateInfo& pVertexAttributeDescriptions(const VertexInputAttributeDescription* pVertexAttributeDescriptions){ m_pipelineVertexInputStateCreateInfo.pVertexAttributeDescriptions = reinterpret_cast<const VkVertexInputAttributeDescription*>( pVertexAttributeDescriptions ); return *this; }
    operator VkPipelineVertexInputStateCreateInfo&() { return m_pipelineVertexInputStateCreateInfo; }
    operator const VkPipelineVertexInputStateCreateInfo&() const { return m_pipelineVertexInputStateCreateInfo; }
    VkPipelineVertexInputStateCreateInfo& vkHandle() { return m_pipelineVertexInputStateCreateInfo; }
    const VkPipelineVertexInputStateCreateInfo& vkHandle() const { return m_pipelineVertexInputStateCreateInfo; }

  private:
    VkPipelineVertexInputStateCreateInfo m_pipelineVertexInputStateCreateInfo;
  };

  class PipelineInputAssemblyStateCreateInfo
  {
  public:
    PipelineInputAssemblyStateCreateInfo() : PipelineInputAssemblyStateCreateInfo(PipelineInputAssemblyStateCreateFlags(), PrimitiveTopology::PointList, 0) {}
    PipelineInputAssemblyStateCreateInfo(PipelineInputAssemblyStateCreateFlags flags, PrimitiveTopology topology, Bool32 primitiveRestartEnable) {m_pipelineInputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO; m_pipelineInputAssemblyStateCreateInfo.pNext = nullptr; m_pipelineInputAssemblyStateCreateInfo.flags = static_cast<VkPipelineInputAssemblyStateCreateFlags>( flags ); m_pipelineInputAssemblyStateCreateInfo.topology = static_cast<VkPrimitiveTopology>( topology ); m_pipelineInputAssemblyStateCreateInfo.primitiveRestartEnable = primitiveRestartEnable; }
    PipelineInputAssemblyStateCreateInfo(const VkPipelineInputAssemblyStateCreateInfo& other) : m_pipelineInputAssemblyStateCreateInfo{other} {}
    PipelineInputAssemblyStateCreateInfo& operator=(const VkPipelineInputAssemblyStateCreateInfo& other){ m_pipelineInputAssemblyStateCreateInfo = other; return *this; }

    const StructureType& sType() const { return reinterpret_cast<const StructureType&>( m_pipelineInputAssemblyStateCreateInfo.sType ); }
    PipelineInputAssemblyStateCreateInfo& sType(StructureType sType){ m_pipelineInputAssemblyStateCreateInfo.sType = static_cast<VkStructureType>( sType ); return *this; }
    const void* pNext() const { return reinterpret_cast<const void*>( m_pipelineInputAssemblyStateCreateInfo.pNext ); }
    PipelineInputAssemblyStateCreateInfo& pNext(const void* pNext){ m_pipelineInputAssemblyStateCreateInfo.pNext = pNext; return *this; }
    const PipelineInputAssemblyStateCreateFlags& flags() const { return reinterpret_cast<const PipelineInputAssemblyStateCreateFlags&>( m_pipelineInputAssemblyStateCreateInfo.flags ); }
    PipelineInputAssemblyStateCreateInfo& flags(PipelineInputAssemblyStateCreateFlags flags){ m_pipelineInputAssemblyStateCreateInfo.flags = static_cast<VkPipelineInputAssemblyStateCreateFlags>( flags ); return *this; }
    const PrimitiveTopology& topology() const { return reinterpret_cast<const PrimitiveTopology&>( m_pipelineInputAssemblyStateCreateInfo.topology ); }
    PipelineInputAssemblyStateCreateInfo& topology(PrimitiveTopology topology){ m_pipelineInputAssemblyStateCreateInfo.topology = static_cast<VkPrimitiveTopology>( topology ); return *this; }
    const Bool32& primitiveRestartEnable() const { return m_pipelineInputAssemblyStateCreateInfo.primitiveRestartEnable; }
    PipelineInputAssemblyStateCreateInfo& primitiveRestartEnable(Bool32 primitiveRestartEnable){ m_pipelineInputAssemblyStateCreateInfo.primitiveRestartEnable = primitiveRestartEnable; return *this; }
    operator VkPipelineInputAssemblyStateCreateInfo&() { return m_pipelineInputAssemblyStateCreateInfo; }
    operator const VkPipelineInputAssemblyStateCreateInfo&() const { return m_pipelineInputAssemblyStateCreateInfo; }
    VkPipelineInputAssemblyStateCreateInfo& vkHandle() { return m_pipelineInputAssemblyStateCreateInfo; }
    const VkPipelineInputAssemblyStateCreateInfo& vkHandle() const { return m_pipelineInputAssemblyStateCreateInfo; }

  private:
    VkPipelineInputAssemblyStateCreateInfo m_pipelineInputAssemblyStateCreateInfo;
  };

  class PipelineTessellationStateCreateInfo
  {
  public:
    PipelineTessellationStateCreateInfo() : PipelineTessellationStateCreateInfo(PipelineTessellationStateCreateFlags(), 0) {}
    PipelineTessellationStateCreateInfo(PipelineTessellationStateCreateFlags flags, uint32_t patchControlPoints) {m_pipelineTessellationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO; m_pipelineTessellationStateCreateInfo.pNext = nullptr; m_pipelineTessellationStateCreateInfo.flags = static_cast<VkPipelineTessellationStateCreateFlags>( flags ); m_pipelineTessellationStateCreateInfo.patchControlPoints = patchControlPoints; }
    PipelineTessellationStateCreateInfo(const VkPipelineTessellationStateCreateInfo& other) : m_pipelineTessellationStateCreateInfo{other} {}
    PipelineTessellationStateCreateInfo& operator=(const VkPipelineTessellationStateCreateInfo& other){ m_pipelineTessellationStateCreateInfo = other; return *this; }

    const StructureType& sType() const { return reinterpret_cast<const StructureType&>( m_pipelineTessellationStateCreateInfo.sType ); }
    PipelineTessellationStateCreateInfo& sType(StructureType sType){ m_pipelineTessellationStateCreateInfo.sType = static_cast<VkStructureType>( sType ); return *this; }
    const void* pNext() const { return reinterpret_cast<const void*>( m_pipelineTessellationStateCreateInfo.pNext ); }
    PipelineTessellationStateCreateInfo& pNext(const void* pNext){ m_pipelineTessellationStateCreateInfo.pNext = pNext; return *this; }
    const PipelineTessellationStateCreateFlags& flags() const { return reinterpret_cast<const PipelineTessellationStateCreateFlags&>( m_pipelineTessellationStateCreateInfo.flags ); }
    PipelineTessellationStateCreateInfo& flags(PipelineTessellationStateCreateFlags flags){ m_pipelineTessellationStateCreateInfo.flags = static_cast<VkPipelineTessellationStateCreateFlags>( flags ); return *this; }
    const uint32_t& patchControlPoints() const { return m_pipelineTessellationStateCreateInfo.patchControlPoints; }
    PipelineTessellationStateCreateInfo& patchControlPoints(uint32_t patchControlPoints){ m_pipelineTessellationStateCreateInfo.patchControlPoints = patchControlPoints; return *this; }
    operator VkPipelineTessellationStateCreateInfo&() { return m_pipelineTessellationStateCreateInfo; }
    operator const VkPipelineTessellationStateCreateInfo&() const { return m_pipelineTessellationStateCreateInfo; }
    VkPipelineTessellationStateCreateInfo& vkHandle() { return m_pipelineTessellationStateCreateInfo; }
    const VkPipelineTessellationStateCreateInfo& vkHandle() const { return m_pipelineTessellationStateCreateInfo; }

  private:
    VkPipelineTessellationStateCreateInfo m_pipelineTessellationStateCreateInfo;
  };

  class PipelineViewportStateCreateInfo
  {
  public:
    PipelineViewportStateCreateInfo() : PipelineViewportStateCreateInfo(PipelineViewportStateCreateFlags(), 0, nullptr, 0, nullptr) {}
    PipelineViewportStateCreateInfo(PipelineViewportStateCreateFlags flags, uint32_t viewportCount, const Viewport* pViewports, uint32_t scissorCount, const Rect2D* pScissors) {m_pipelineViewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO; m_pipelineViewportStateCreateInfo.pNext = nullptr; m_pipelineViewportStateCreateInfo.flags = static_cast<VkPipelineViewportStateCreateFlags>( flags ); m_pipelineViewportStateCreateInfo.viewportCount = viewportCount; m_pipelineViewportStateCreateInfo.pViewports = reinterpret_cast<const VkViewport*>( pViewports ); m_pipelineViewportStateCreateInfo.scissorCount = scissorCount; m_pipelineViewportStateCreateInfo.pScissors = reinterpret_cast<const VkRect2D*>( pScissors ); }
    PipelineViewportStateCreateInfo(const VkPipelineViewportStateCreateInfo& other) : m_pipelineViewportStateCreateInfo{other} {}
    PipelineViewportStateCreateInfo& operator=(const VkPipelineViewportStateCreateInfo& other){ m_pipelineViewportStateCreateInfo = other; return *this; }

    const StructureType& sType() const { return reinterpret_cast<const StructureType&>( m_pipelineViewportStateCreateInfo.sType ); }
    PipelineViewportStateCreateInfo& sType(StructureType sType){ m_pipelineViewportStateCreateInfo.sType = static_cast<VkStructureType>( sType ); return *this; }
    const void* pNext() const { return reinterpret_cast<const void*>( m_pipelineViewportStateCreateInfo.pNext ); }
    PipelineViewportStateCreateInfo& pNext(const void* pNext){ m_pipelineViewportStateCreateInfo.pNext = pNext; return *this; }
    const PipelineViewportStateCreateFlags& flags() const { return reinterpret_cast<const PipelineViewportStateCreateFlags&>( m_pipelineViewportStateCreateInfo.flags ); }
    PipelineViewportStateCreateInfo& flags(PipelineViewportStateCreateFlags flags){ m_pipelineViewportStateCreateInfo.flags = static_cast<VkPipelineViewportStateCreateFlags>( flags ); return *this; }
    const uint32_t& viewportCount() const { return m_pipelineViewportStateCreateInfo.viewportCount; }
    PipelineViewportStateCreateInfo& viewportCount(uint32_t viewportCount){ m_pipelineViewportStateCreateInfo.viewportCount = viewportCount; return *this; }
    const Viewport* pViewports() const { return reinterpret_cast<const Viewport*>( m_pipelineViewportStateCreateInfo.pViewports ); }
    PipelineViewportStateCreateInfo& pViewports(const Viewport* pViewports){ m_pipelineViewportStateCreateInfo.pViewports = reinterpret_cast<const VkViewport*>( pViewports ); return *this; }
    const uint32_t& scissorCount() const { return m_pipelineViewportStateCreateInfo.scissorCount; }
    PipelineViewportStateCreateInfo& scissorCount(uint32_t scissorCount){ m_pipelineViewportStateCreateInfo.scissorCount = scissorCount; return *this; }
    const Rect2D* pScissors() const { return reinterpret_cast<const Rect2D*>( m_pipelineViewportStateCreateInfo.pScissors ); }
    PipelineViewportStateCreateInfo& pScissors(const Rect2D* pScissors){ m_pipelineViewportStateCreateInfo.pScissors = reinterpret_cast<const VkRect2D*>( pScissors ); return *this; }
    operator VkPipelineViewportStateCreateInfo&() { return m_pipelineViewportStateCreateInfo; }
    operator const VkPipelineViewportStateCreateInfo&() const { return m_pipelineViewportStateCreateInfo; }
    VkPipelineViewportStateCreateInfo& vkHandle() { return m_pipelineViewportStateCreateInfo; }
    const VkPipelineViewportStateCreateInfo& vkHandle() const { return m_pipelineViewportStateCreateInfo; }

  private:
    VkPipelineViewportStateCreateInfo m_pipelineViewportStateCreateInfo;
  };

  class PipelineRasterizationStateCreateInfo
  {
  public:
    PipelineRasterizationStateCreateInfo() : PipelineRasterizationStateCreateInfo(PipelineRasterizationStateCreateFlags(), 0, 0, PolygonMode::Fill, CullModeFlags(), FrontFace::CounterClockwise, 0, 0, 0, 0, 0) {}
    PipelineRasterizationStateCreateInfo(PipelineRasterizationStateCreateFlags flags, Bool32 depthClampEnable, Bool32 rasterizerDiscardEnable, PolygonMode polygonMode, CullModeFlags cullMode, FrontFace frontFace, Bool32 depthBiasEnable, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor, float lineWidth) {m_pipelineRasterizationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO; m_pipelineRasterizationStateCreateInfo.pNext = nullptr; m_pipelineRasterizationStateCreateInfo.flags = static_cast<VkPipelineRasterizationStateCreateFlags>( flags ); m_pipelineRasterizationStateCreateInfo.depthClampEnable = depthClampEnable; m_pipelineRasterizationStateCreateInfo.rasterizerDiscardEnable = rasterizerDiscardEnable; m_pipelineRasterizationStateCreateInfo.polygonMode = static_cast<VkPolygonMode>( polygonMode ); m_pipelineRasterizationStateCreateInfo.cullMode = static_cast<VkCullModeFlags>( cullMode ); m_pipelineRasterizationStateCreateInfo.frontFace = static_cast<VkFrontFace>( frontFace ); m_pipelineRasterizationStateCreateInfo.depthBiasEnable = depthBiasEnable; m_pipelineRasterizationStateCreateInfo.depthBiasConstantFactor = depthBiasConstantFactor; m_pipelineRasterizationStateCreateInfo.depthBiasClamp = depthBiasClamp; m_pipelineRasterizationStateCreateInfo.depthBiasSlopeFactor = depthBiasSlopeFactor; m_pipelineRasterizationStateCreateInfo.lineWidth = lineWidth; }
    PipelineRasterizationStateCreateInfo(const VkPipelineRasterizationStateCreateInfo& other) : m_pipelineRasterizationStateCreateInfo{other} {}
    PipelineRasterizationStateCreateInfo& operator=(const VkPipelineRasterizationStateCreateInfo& other){ m_pipelineRasterizationStateCreateInfo = other; return *this; }

    const StructureType& sType() const { return reinterpret_cast<const StructureType&>( m_pipelineRasterizationStateCreateInfo.sType ); }
    PipelineRasterizationStateCreateInfo& sType(StructureType sType){ m_pipelineRasterizationStateCreateInfo.sType = static_cast<VkStructureType>( sType ); return *this; }
    const void* pNext() const { return reinterpret_cast<const void*>( m_pipelineRasterizationStateCreateInfo.pNext ); }
    PipelineRasterizationStateCreateInfo& pNext(const void* pNext){ m_pipelineRasterizationStateCreateInfo.pNext = pNext; return *this; }
    const PipelineRasterizationStateCreateFlags& flags() const { return reinterpret_cast<const PipelineRasterizationStateCreateFlags&>( m_pipelineRasterizationStateCreateInfo.flags ); }
    PipelineRasterizationStateCreateInfo& flags(PipelineRasterizationStateCreateFlags flags){ m_pipelineRasterizationStateCreateInfo.flags = static_cast<VkPipelineRasterizationStateCreateFlags>( flags ); return *this; }
    const Bool32& depthClampEnable() const { return m_pipelineRasterizationStateCreateInfo.depthClampEnable; }
    PipelineRasterizationStateCreateInfo& depthClampEnable(Bool32 depthClampEnable){ m_pipelineRasterizationStateCreateInfo.depthClampEnable = depthClampEnable; return *this; }
    const Bool32& rasterizerDiscardEnable() const { return m_pipelineRasterizationStateCreateInfo.rasterizerDiscardEnable; }
    PipelineRasterizationStateCreateInfo& rasterizerDiscardEnable(Bool32 rasterizerDiscardEnable){ m_pipelineRasterizationStateCreateInfo.rasterizerDiscardEnable = rasterizerDiscardEnable; return *this; }
    const PolygonMode& polygonMode() const { return reinterpret_cast<const PolygonMode&>( m_pipelineRasterizationStateCreateInfo.polygonMode ); }
    PipelineRasterizationStateCreateInfo& polygonMode(PolygonMode polygonMode){ m_pipelineRasterizationStateCreateInfo.polygonMode = static_cast<VkPolygonMode>( polygonMode ); return *this; }
    const CullModeFlags& cullMode() const { return reinterpret_cast<const CullModeFlags&>( m_pipelineRasterizationStateCreateInfo.cullMode ); }
    PipelineRasterizationStateCreateInfo& cullMode(CullModeFlags cullMode){ m_pipelineRasterizationStateCreateInfo.cullMode = static_cast<VkCullModeFlags>( cullMode ); return *this; }
    const FrontFace& frontFace() const { return reinterpret_cast<const FrontFace&>( m_pipelineRasterizationStateCreateInfo.frontFace ); }
    PipelineRasterizationStateCreateInfo& frontFace(FrontFace frontFace){ m_pipelineRasterizationStateCreateInfo.frontFace = static_cast<VkFrontFace>( frontFace ); return *this; }
    const Bool32& depthBiasEnable() const { return m_pipelineRasterizationStateCreateInfo.depthBiasEnable; }
    PipelineRasterizationStateCreateInfo& depthBiasEnable(Bool32 depthBiasEnable){ m_pipelineRasterizationStateCreateInfo.depthBiasEnable = depthBiasEnable; return *this; }
    const float& depthBiasConstantFactor() const { return m_pipelineRasterizationStateCreateInfo.depthBiasConstantFactor; }
    PipelineRasterizationStateCreateInfo& depthBiasConstantFactor(float depthBiasConstantFactor){ m_pipelineRasterizationStateCreateInfo.depthBiasConstantFactor = depthBiasConstantFactor; return *this; }
    const float& depthBiasClamp() const { return m_pipelineRasterizationStateCreateInfo.depthBiasClamp; }
    PipelineRasterizationStateCreateInfo& depthBiasClamp(float depthBiasClamp){ m_pipelineRasterizationStateCreateInfo.depthBiasClamp = depthBiasClamp; return *this; }
    const float& depthBiasSlopeFactor() const { return m_pipelineRasterizationStateCreateInfo.depthBiasSlopeFactor; }
    PipelineRasterizationStateCreateInfo& depthBiasSlopeFactor(float depthBiasSlopeFactor){ m_pipelineRasterizationStateCreateInfo.depthBiasSlopeFactor = depthBiasSlopeFactor; return *this; }
    const float& lineWidth() const { return m_pipelineRasterizationStateCreateInfo.lineWidth; }
    PipelineRasterizationStateCreateInfo& lineWidth(float lineWidth){ m_pipelineRasterizationStateCreateInfo.lineWidth = lineWidth; return *this; }
    operator VkPipelineRasterizationStateCreateInfo&() { return m_pipelineRasterizationStateCreateInfo; }
    operator const VkPipelineRasterizationStateCreateInfo&() const { return m_pipelineRasterizationStateCreateInfo; }
    VkPipelineRasterizationStateCreateInfo& vkHandle() { return m_pipelineRasterizationStateCreateInfo; }
    const VkPipelineRasterizationStateCreateInfo& vkHandle() const { return m_pipelineRasterizationStateCreateInfo; }

  private:
    VkPipelineRasterizationStateCreateInfo m_pipelineRasterizationStateCreateInfo;
  };

  class PipelineDepthStencilStateCreateInfo
  {
  public:
    PipelineDepthStencilStateCreateInfo() : PipelineDepthStencilStateCreateInfo(PipelineDepthStencilStateCreateFlags(), 0, 0, CompareOp::Never, 0, 0, StencilOpState(), StencilOpState(), 0, 0) {}
    PipelineDepthStencilStateCreateInfo(PipelineDepthStencilStateCreateFlags flags, Bool32 depthTestEnable, Bool32 depthWriteEnable, CompareOp depthCompareOp, Bool32 depthBoundsTestEnable, Bool32 stencilTestEnable, StencilOpState front, StencilOpState back, float minDepthBounds, float maxDepthBounds) {m_pipelineDepthStencilStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO; m_pipelineDepthStencilStateCreateInfo.pNext = nullptr; m_pipelineDepthStencilStateCreateInfo.flags = static_cast<VkPipelineDepthStencilStateCreateFlags>( flags ); m_pipelineDepthStencilStateCreateInfo.depthTestEnable = depthTestEnable; m_pipelineDepthStencilStateCreateInfo.depthWriteEnable = depthWriteEnable; m_pipelineDepthStencilStateCreateInfo.depthCompareOp = static_cast<VkCompareOp>( depthCompareOp ); m_pipelineDepthStencilStateCreateInfo.depthBoundsTestEnable = depthBoundsTestEnable; m_pipelineDepthStencilStateCreateInfo.stencilTestEnable = stencilTestEnable; m_pipelineDepthStencilStateCreateInfo.front = static_cast<VkStencilOpState>( front ); m_pipelineDepthStencilStateCreateInfo.back = static_cast<VkStencilOpState>( back ); m_pipelineDepthStencilStateCreateInfo.minDepthBounds = minDepthBounds; m_pipelineDepthStencilStateCreateInfo.maxDepthBounds = maxDepthBounds; }
    PipelineDepthStencilStateCreateInfo(const VkPipelineDepthStencilStateCreateInfo& other) : m_pipelineDepthStencilStateCreateInfo{other} {}
    PipelineDepthStencilStateCreateInfo& operator=(const VkPipelineDepthStencilStateCreateInfo& other){ m_pipelineDepthStencilStateCreateInfo = other; return *this; }

    const StructureType& sType() const { return reinterpret_cast<const StructureType&>( m_pipelineDepthStencilStateCreateInfo.sType ); }
    PipelineDepthStencilStateCreateInfo& sType(StructureType sType){ m_pipelineDepthStencilStateCreateInfo.sType = static_cast<VkStructureType>( sType ); return *this; }
    const void* pNext() const { return reinterpret_cast<const void*>( m_pipelineDepthStencilStateCreateInfo.pNext ); }
    PipelineDepthStencilStateCreateInfo& pNext(const void* pNext){ m_pipelineDepthStencilStateCreateInfo.pNext = pNext; return *this; }
    const PipelineDepthStencilStateCreateFlags& flags() const { return reinterpret_cast<const PipelineDepthStencilStateCreateFlags&>( m_pipelineDepthStencilStateCreateInfo.flags ); }
    PipelineDepthStencilStateCreateInfo& flags(PipelineDepthStencilStateCreateFlags flags){ m_pipelineDepthStencilStateCreateInfo.flags = static_cast<VkPipelineDepthStencilStateCreateFlags>( flags ); return *this; }
    const Bool32& depthTestEnable() const { return m_pipelineDepthStencilStateCreateInfo.depthTestEnable; }
    PipelineDepthStencilStateCreateInfo& depthTestEnable(Bool32 depthTestEnable){ m_pipelineDepthStencilStateCreateInfo.depthTestEnable = depthTestEnable; return *this; }
    const Bool32& depthWriteEnable() const { return m_pipelineDepthStencilStateCreateInfo.depthWriteEnable; }
    PipelineDepthStencilStateCreateInfo& depthWriteEnable(Bool32 depthWriteEnable){ m_pipelineDepthStencilStateCreateInfo.depthWriteEnable = depthWriteEnable; return *this; }
    const CompareOp& depthCompareOp() const { return reinterpret_cast<const CompareOp&>( m_pipelineDepthStencilStateCreateInfo.depthCompareOp ); }
    PipelineDepthStencilStateCreateInfo& depthCompareOp(CompareOp depthCompareOp){ m_pipelineDepthStencilStateCreateInfo.depthCompareOp = static_cast<VkCompareOp>( depthCompareOp ); return *this; }
    const Bool32& depthBoundsTestEnable() const { return m_pipelineDepthStencilStateCreateInfo.depthBoundsTestEnable; }
    PipelineDepthStencilStateCreateInfo& depthBoundsTestEnable(Bool32 depthBoundsTestEnable){ m_pipelineDepthStencilStateCreateInfo.depthBoundsTestEnable = depthBoundsTestEnable; return *this; }
    const Bool32& stencilTestEnable() const { return m_pipelineDepthStencilStateCreateInfo.stencilTestEnable; }
    PipelineDepthStencilStateCreateInfo& stencilTestEnable(Bool32 stencilTestEnable){ m_pipelineDepthStencilStateCreateInfo.stencilTestEnable = stencilTestEnable; return *this; }
    const StencilOpState& front() const { return reinterpret_cast<const StencilOpState&>( m_pipelineDepthStencilStateCreateInfo.front ); }
    PipelineDepthStencilStateCreateInfo& front(StencilOpState front){ m_pipelineDepthStencilStateCreateInfo.front = static_cast<VkStencilOpState>( front ); return *this; }
    const StencilOpState& back() const { return reinterpret_cast<const StencilOpState&>( m_pipelineDepthStencilStateCreateInfo.back ); }
    PipelineDepthStencilStateCreateInfo& back(StencilOpState back){ m_pipelineDepthStencilStateCreateInfo.back = static_cast<VkStencilOpState>( back ); return *this; }
    const float& minDepthBounds() const { return m_pipelineDepthStencilStateCreateInfo.minDepthBounds; }
    PipelineDepthStencilStateCreateInfo& minDepthBounds(float minDepthBounds){ m_pipelineDepthStencilStateCreateInfo.minDepthBounds = minDepthBounds; return *this; }
    const float& maxDepthBounds() const { return m_pipelineDepthStencilStateCreateInfo.maxDepthBounds; }
    PipelineDepthStencilStateCreateInfo& maxDepthBounds(float maxDepthBounds){ m_pipelineDepthStencilStateCreateInfo.maxDepthBounds = maxDepthBounds; return *this; }
    operator VkPipelineDepthStencilStateCreateInfo&() { return m_pipelineDepthStencilStateCreateInfo; }
    operator const VkPipelineDepthStencilStateCreateInfo&() const { return m_pipelineDepthStencilStateCreateInfo; }
    VkPipelineDepthStencilStateCreateInfo& vkHandle() { return m_pipelineDepthStencilStateCreateInfo; }
    const VkPipelineDepthStencilStateCreateInfo& vkHandle() const { return m_pipelineDepthStencilStateCreateInfo; }

  private:
    VkPipelineDepthStencilStateCreateInfo m_pipelineDepthStencilStateCreateInfo;
  };

  class PipelineCacheCreateInfo
  {
  public:
    PipelineCacheCreateInfo() : PipelineCacheCreateInfo(PipelineCacheCreateFlags(), 0, nullptr) {}
    PipelineCacheCreateInfo(PipelineCacheCreateFlags flags, size_t initialDataSize, const void* pInitialData) {m_pipelineCacheCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO; m_pipelineCacheCreateInfo.pNext = nullptr; m_pipelineCacheCreateInfo.flags = static_cast<VkPipelineCacheCreateFlags>( flags ); m_pipelineCacheCreateInfo.initialDataSize = initialDataSize; m_pipelineCacheCreateInfo.pInitialData = pInitialData; }
    PipelineCacheCreateInfo(const VkPipelineCacheCreateInfo& other) : m_pipelineCacheCreateInfo{other} {}
    PipelineCacheCreateInfo& operator=(const VkPipelineCacheCreateInfo& other){ m_pipelineCacheCreateInfo = other; return *this; }

    const StructureType& sType() const { return reinterpret_cast<const StructureType&>( m_pipelineCacheCreateInfo.sType ); }
    PipelineCacheCreateInfo& sType(StructureType sType){ m_pipelineCacheCreateInfo.sType = static_cast<VkStructureType>( sType ); return *this; }
    const void* pNext() const { return reinterpret_cast<const void*>( m_pipelineCacheCreateInfo.pNext ); }
    PipelineCacheCreateInfo& pNext(const void* pNext){ m_pipelineCacheCreateInfo.pNext = pNext; return *this; }
    const PipelineCacheCreateFlags& flags() const { return reinterpret_cast<const PipelineCacheCreateFlags&>( m_pipelineCacheCreateInfo.flags ); }
    PipelineCacheCreateInfo& flags(PipelineCacheCreateFlags flags){ m_pipelineCacheCreateInfo.flags = static_cast<VkPipelineCacheCreateFlags>( flags ); return *this; }
    const size_t& initialDataSize() const { return m_pipelineCacheCreateInfo.initialDataSize; }
    PipelineCacheCreateInfo& initialDataSize(size_t initialDataSize){ m_pipelineCacheCreateInfo.initialDataSize = initialDataSize; return *this; }
    const void* pInitialData() const { return reinterpret_cast<const void*>( m_pipelineCacheCreateInfo.pInitialData ); }
    PipelineCacheCreateInfo& pInitialData(const void* pInitialData){ m_pipelineCacheCreateInfo.pInitialData = pInitialData; return *this; }
    operator VkPipelineCacheCreateInfo&() { return m_pipelineCacheCreateInfo; }
    operator const VkPipelineCacheCreateInfo&() const { return m_pipelineCacheCreateInfo; }
    VkPipelineCacheCreateInfo& vkHandle() { return m_pipelineCacheCreateInfo; }
    const VkPipelineCacheCreateInfo& vkHandle() const { return m_pipelineCacheCreateInfo; }

  private:
    VkPipelineCacheCreateInfo m_pipelineCacheCreateInfo;
  };

  class SamplerCreateInfo
  {
  public:
    SamplerCreateInfo() : SamplerCreateInfo(SamplerCreateFlags(), Filter::Nearest, Filter::Nearest, SamplerMipmapMode::Nearest, SamplerAddressMode::Repeat, SamplerAddressMode::Repeat, SamplerAddressMode::Repeat, 0, 0, 0, 0, CompareOp::Never, 0, 0, BorderColor::FloatTransparentBlack, 0) {}
    SamplerCreateInfo(SamplerCreateFlags flags, Filter magFilter, Filter minFilter, SamplerMipmapMode mipmapMode, SamplerAddressMode addressModeU, SamplerAddressMode addressModeV, SamplerAddressMode addressModeW, float mipLodBias, Bool32 anisotropyEnable, float maxAnisotropy, Bool32 compareEnable, CompareOp compareOp, float minLod, float maxLod, BorderColor borderColor, Bool32 unnormalizedCoordinates) {m_samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO; m_samplerCreateInfo.pNext = nullptr; m_samplerCreateInfo.flags = static_cast<VkSamplerCreateFlags>( flags ); m_samplerCreateInfo.magFilter = static_cast<VkFilter>( magFilter ); m_samplerCreateInfo.minFilter = static_cast<VkFilter>( minFilter ); m_samplerCreateInfo.mipmapMode = static_cast<VkSamplerMipmapMode>( mipmapMode ); m_samplerCreateInfo.addressModeU = static_cast<VkSamplerAddressMode>( addressModeU ); m_samplerCreateInfo.addressModeV = static_cast<VkSamplerAddressMode>( addressModeV ); m_samplerCreateInfo.addressModeW = static_cast<VkSamplerAddressMode>( addressModeW ); m_samplerCreateInfo.mipLodBias = mipLodBias; m_samplerCreateInfo.anisotropyEnable = anisotropyEnable; m_samplerCreateInfo.maxAnisotropy = maxAnisotropy; m_samplerCreateInfo.compareEnable = compareEnable; m_samplerCreateInfo.compareOp = static_cast<VkCompareOp>( compareOp ); m_samplerCreateInfo.minLod = minLod; m_samplerCreateInfo.maxLod = maxLod; m_samplerCreateInfo.borderColor = static_cast<VkBorderColor>( borderColor ); m_samplerCreateInfo.unnormalizedCoordinates = unnormalizedCoordinates; }
    SamplerCreateInfo(const VkSamplerCreateInfo& other) : m_samplerCreateInfo{other} {}
    SamplerCreateInfo& operator=(const VkSamplerCreateInfo& other){ m_samplerCreateInfo = other; return *this; }

    const StructureType& sType() const { return reinterpret_cast<const StructureType&>( m_samplerCreateInfo.sType ); }
    SamplerCreateInfo& sType(StructureType sType){ m_samplerCreateInfo.sType = static_cast<VkStructureType>( sType ); return *this; }
    const void* pNext() const { return reinterpret_cast<const void*>( m_samplerCreateInfo.pNext ); }
    SamplerCreateInfo& pNext(const void* pNext){ m_samplerCreateInfo.pNext = pNext; return *this; }
    const SamplerCreateFlags& flags() const { return reinterpret_cast<const SamplerCreateFlags&>( m_samplerCreateInfo.flags ); }
    SamplerCreateInfo& flags(SamplerCreateFlags flags){ m_samplerCreateInfo.flags = static_cast<VkSamplerCreateFlags>( flags ); return *this; }
    const Filter& magFilter() const { return reinterpret_cast<const Filter&>( m_samplerCreateInfo.magFilter ); }
    SamplerCreateInfo& magFilter(Filter magFilter){ m_samplerCreateInfo.magFilter = static_cast<VkFilter>( magFilter ); return *this; }
    const Filter& minFilter() const { return reinterpret_cast<const Filter&>( m_samplerCreateInfo.minFilter ); }
    SamplerCreateInfo& minFilter(Filter minFilter){ m_samplerCreateInfo.minFilter = static_cast<VkFilter>( minFilter ); return *this; }
    const SamplerMipmapMode& mipmapMode() const { return reinterpret_cast<const SamplerMipmapMode&>( m_samplerCreateInfo.mipmapMode ); }
    SamplerCreateInfo& mipmapMode(SamplerMipmapMode mipmapMode){ m_samplerCreateInfo.mipmapMode = static_cast<VkSamplerMipmapMode>( mipmapMode ); return *this; }
    const SamplerAddressMode& addressModeU() const { return reinterpret_cast<const SamplerAddressMode&>( m_samplerCreateInfo.addressModeU ); }
    SamplerCreateInfo& addressModeU(SamplerAddressMode addressModeU){ m_samplerCreateInfo.addressModeU = static_cast<VkSamplerAddressMode>( addressModeU ); return *this; }
    const SamplerAddressMode& addressModeV() const { return reinterpret_cast<const SamplerAddressMode&>( m_samplerCreateInfo.addressModeV ); }
    SamplerCreateInfo& addressModeV(SamplerAddressMode addressModeV){ m_samplerCreateInfo.addressModeV = static_cast<VkSamplerAddressMode>( addressModeV ); return *this; }
    const SamplerAddressMode& addressModeW() const { return reinterpret_cast<const SamplerAddressMode&>( m_samplerCreateInfo.addressModeW ); }
    SamplerCreateInfo& addressModeW(SamplerAddressMode addressModeW){ m_samplerCreateInfo.addressModeW = static_cast<VkSamplerAddressMode>( addressModeW ); return *this; }
    const float& mipLodBias() const { return m_samplerCreateInfo.mipLodBias; }
    SamplerCreateInfo& mipLodBias(float mipLodBias){ m_samplerCreateInfo.mipLodBias = mipLodBias; return *this; }
    const Bool32& anisotropyEnable() const { return m_samplerCreateInfo.anisotropyEnable; }
    SamplerCreateInfo& anisotropyEnable(Bool32 anisotropyEnable){ m_samplerCreateInfo.anisotropyEnable = anisotropyEnable; return *this; }
    const float& maxAnisotropy() const { return m_samplerCreateInfo.maxAnisotropy; }
    SamplerCreateInfo& maxAnisotropy(float maxAnisotropy){ m_samplerCreateInfo.maxAnisotropy = maxAnisotropy; return *this; }
    const Bool32& compareEnable() const { return m_samplerCreateInfo.compareEnable; }
    SamplerCreateInfo& compareEnable(Bool32 compareEnable){ m_samplerCreateInfo.compareEnable = compareEnable; return *this; }
    const CompareOp& compareOp() const { return reinterpret_cast<const CompareOp&>( m_samplerCreateInfo.compareOp ); }
    SamplerCreateInfo& compareOp(CompareOp compareOp){ m_samplerCreateInfo.compareOp = static_cast<VkCompareOp>( compareOp ); return *this; }
    const float& minLod() const { return m_samplerCreateInfo.minLod; }
    SamplerCreateInfo& minLod(float minLod){ m_samplerCreateInfo.minLod = minLod; return *this; }
    const float& maxLod() const { return m_samplerCreateInfo.maxLod; }
    SamplerCreateInfo& maxLod(float maxLod){ m_samplerCreateInfo.maxLod = maxLod; return *this; }
    const BorderColor& borderColor() const { return reinterpret_cast<const BorderColor&>( m_samplerCreateInfo.borderColor ); }
    SamplerCreateInfo& borderColor(BorderColor borderColor){ m_samplerCreateInfo.borderColor = static_cast<VkBorderColor>( borderColor ); return *this; }
    const Bool32& unnormalizedCoordinates() const { return m_samplerCreateInfo.unnormalizedCoordinates; }
    SamplerCreateInfo& unnormalizedCoordinates(Bool32 unnormalizedCoordinates){ m_samplerCreateInfo.unnormalizedCoordinates = unnormalizedCoordinates; return *this; }
    operator VkSamplerCreateInfo&() { return m_samplerCreateInfo; }
    operator const VkSamplerCreateInfo&() const { return m_samplerCreateInfo; }
    VkSamplerCreateInfo& vkHandle() { return m_samplerCreateInfo; }
    const VkSamplerCreateInfo& vkHandle() const { return m_samplerCreateInfo; }

  private:
    VkSamplerCreateInfo m_samplerCreateInfo;
  };

  class CommandBufferAllocateInfo
  {
  public:
    CommandBufferAllocateInfo() : CommandBufferAllocateInfo(CommandPool(), CommandBufferLevel::Primary, 0) {}
    CommandBufferAllocateInfo(CommandPool commandPool, CommandBufferLevel level, uint32_t commandBufferCount) {m_commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO; m_commandBufferAllocateInfo.pNext = nullptr; m_commandBufferAllocateInfo.commandPool = commandPool; m_commandBufferAllocateInfo.level = static_cast<VkCommandBufferLevel>( level ); m_commandBufferAllocateInfo.commandBufferCount = commandBufferCount; }
    CommandBufferAllocateInfo(const VkCommandBufferAllocateInfo& other) : m_commandBufferAllocateInfo{other} {}
    CommandBufferAllocateInfo& operator=(const VkCommandBufferAllocateInfo& other){ m_commandBufferAllocateInfo = other; return *this; }

    const StructureType& sType() const { return reinterpret_cast<const StructureType&>( m_commandBufferAllocateInfo.sType ); }
    CommandBufferAllocateInfo& sType(StructureType sType){ m_commandBufferAllocateInfo.sType = static_cast<VkStructureType>( sType ); return *this; }
    const void* pNext() const { return reinterpret_cast<const void*>( m_commandBufferAllocateInfo.pNext ); }
    CommandBufferAllocateInfo& pNext(const void* pNext){ m_commandBufferAllocateInfo.pNext = pNext; return *this; }
    const CommandPool& commandPool() const { return m_commandBufferAllocateInfo.commandPool; }
    CommandBufferAllocateInfo& commandPool(CommandPool commandPool){ m_commandBufferAllocateInfo.commandPool = commandPool; return *this; }
    const CommandBufferLevel& level() const { return reinterpret_cast<const CommandBufferLevel&>( m_commandBufferAllocateInfo.level ); }
    CommandBufferAllocateInfo& level(CommandBufferLevel level){ m_commandBufferAllocateInfo.level = static_cast<VkCommandBufferLevel>( level ); return *this; }
    const uint32_t& commandBufferCount() const { return m_commandBufferAllocateInfo.commandBufferCount; }
    CommandBufferAllocateInfo& commandBufferCount(uint32_t commandBufferCount){ m_commandBufferAllocateInfo.commandBufferCount = commandBufferCount; return *this; }
    operator VkCommandBufferAllocateInfo&() { return m_commandBufferAllocateInfo; }
    operator const VkCommandBufferAllocateInfo&() const { return m_commandBufferAllocateInfo; }
    VkCommandBufferAllocateInfo& vkHandle() { return m_commandBufferAllocateInfo; }
    const VkCommandBufferAllocateInfo& vkHandle() const { return m_commandBufferAllocateInfo; }

  private:
    VkCommandBufferAllocateInfo m_commandBufferAllocateInfo;
  };

  class RenderPassBeginInfo
  {
  public:
    RenderPassBeginInfo() : RenderPassBeginInfo(RenderPass(), Framebuffer(), Rect2D(), 0, nullptr) {}
    RenderPassBeginInfo(RenderPass renderPass, Framebuffer framebuffer, Rect2D renderArea, uint32_t clearValueCount, const ClearValue* pClearValues) {m_renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO; m_renderPassBeginInfo.pNext = nullptr; m_renderPassBeginInfo.renderPass = renderPass; m_renderPassBeginInfo.framebuffer = framebuffer; m_renderPassBeginInfo.renderArea = static_cast<VkRect2D>( renderArea ); m_renderPassBeginInfo.clearValueCount = clearValueCount; m_renderPassBeginInfo.pClearValues = reinterpret_cast<const VkClearValue*>( pClearValues ); }
    RenderPassBeginInfo(const VkRenderPassBeginInfo& other) : m_renderPassBeginInfo{other} {}
    RenderPassBeginInfo& operator=(const VkRenderPassBeginInfo& other){ m_renderPassBeginInfo = other; return *this; }

    const StructureType& sType() const { return reinterpret_cast<const StructureType&>( m_renderPassBeginInfo.sType ); }
    RenderPassBeginInfo& sType(StructureType sType){ m_renderPassBeginInfo.sType = static_cast<VkStructureType>( sType ); return *this; }
    const void* pNext() const { return reinterpret_cast<const void*>( m_renderPassBeginInfo.pNext ); }
    RenderPassBeginInfo& pNext(const void* pNext){ m_renderPassBeginInfo.pNext = pNext; return *this; }
    const RenderPass& renderPass() const { return m_renderPassBeginInfo.renderPass; }
    RenderPassBeginInfo& renderPass(RenderPass renderPass){ m_renderPassBeginInfo.renderPass = renderPass; return *this; }
    const Framebuffer& framebuffer() const { return m_renderPassBeginInfo.framebuffer; }
    RenderPassBeginInfo& framebuffer(Framebuffer framebuffer){ m_renderPassBeginInfo.framebuffer = framebuffer; return *this; }
    const Rect2D& renderArea() const { return reinterpret_cast<const Rect2D&>( m_renderPassBeginInfo.renderArea ); }
    RenderPassBeginInfo& renderArea(Rect2D renderArea){ m_renderPassBeginInfo.renderArea = static_cast<VkRect2D>( renderArea ); return *this; }
    const uint32_t& clearValueCount() const { return m_renderPassBeginInfo.clearValueCount; }
    RenderPassBeginInfo& clearValueCount(uint32_t clearValueCount){ m_renderPassBeginInfo.clearValueCount = clearValueCount; return *this; }
    const ClearValue* pClearValues() const { return reinterpret_cast<const ClearValue*>( m_renderPassBeginInfo.pClearValues ); }
    RenderPassBeginInfo& pClearValues(const ClearValue* pClearValues){ m_renderPassBeginInfo.pClearValues = reinterpret_cast<const VkClearValue*>( pClearValues ); return *this; }
    operator VkRenderPassBeginInfo&() { return m_renderPassBeginInfo; }
    operator const VkRenderPassBeginInfo&() const { return m_renderPassBeginInfo; }
    VkRenderPassBeginInfo& vkHandle() { return m_renderPassBeginInfo; }
    const VkRenderPassBeginInfo& vkHandle() const { return m_renderPassBeginInfo; }

  private:
    VkRenderPassBeginInfo m_renderPassBeginInfo;
  };

  class EventCreateInfo
  {
  public:
    EventCreateInfo() : EventCreateInfo(EventCreateFlags()) {}
    EventCreateInfo(EventCreateFlags flags) {m_eventCreateInfo.sType = VK_STRUCTURE_TYPE_EVENT_CREATE_INFO; m_eventCreateInfo.pNext = nullptr; m_eventCreateInfo.flags = static_cast<VkEventCreateFlags>( flags ); }
    EventCreateInfo(const VkEventCreateInfo& other) : m_eventCreateInfo{other} {}
    EventCreateInfo& operator=(const VkEventCreateInfo& other){ m_eventCreateInfo = other; return *this; }

    const StructureType& sType() const { return reinterpret_cast<const StructureType&>( m_eventCreateInfo.sType ); }
    EventCreateInfo& sType(StructureType sType){ m_eventCreateInfo.sType = static_cast<VkStructureType>( sType ); return *this; }
    const void* pNext() const { return reinterpret_cast<const void*>( m_eventCreateInfo.pNext ); }
    EventCreateInfo& pNext(const void* pNext){ m_eventCreateInfo.pNext = pNext; return *this; }
    const EventCreateFlags& flags() const { return reinterpret_cast<const EventCreateFlags&>( m_eventCreateInfo.flags ); }
    EventCreateInfo& flags(EventCreateFlags flags){ m_eventCreateInfo.flags = static_cast<VkEventCreateFlags>( flags ); return *this; }
    operator VkEventCreateInfo&() { return m_eventCreateInfo; }
    operator const VkEventCreateInfo&() const { return m_eventCreateInfo; }
    VkEventCreateInfo& vkHandle() { return m_eventCreateInfo; }
    const VkEventCreateInfo& vkHandle() const { return m_eventCreateInfo; }

  private:
    VkEventCreateInfo m_eventCreateInfo;
  };

  class SemaphoreCreateInfo
  {
  public:
    SemaphoreCreateInfo() : SemaphoreCreateInfo(SemaphoreCreateFlags()) {}
    SemaphoreCreateInfo(SemaphoreCreateFlags flags) {m_semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO; m_semaphoreCreateInfo.pNext = nullptr; m_semaphoreCreateInfo.flags = static_cast<VkSemaphoreCreateFlags>( flags ); }
    SemaphoreCreateInfo(const VkSemaphoreCreateInfo& other) : m_semaphoreCreateInfo{other} {}
    SemaphoreCreateInfo& operator=(const VkSemaphoreCreateInfo& other){ m_semaphoreCreateInfo = other; return *this; }

    const StructureType& sType() const { return reinterpret_cast<const StructureType&>( m_semaphoreCreateInfo.sType ); }
    SemaphoreCreateInfo& sType(StructureType sType){ m_semaphoreCreateInfo.sType = static_cast<VkStructureType>( sType ); return *this; }
    const void* pNext() const { return reinterpret_cast<const void*>( m_semaphoreCreateInfo.pNext ); }
    SemaphoreCreateInfo& pNext(const void* pNext){ m_semaphoreCreateInfo.pNext = pNext; return *this; }
    const SemaphoreCreateFlags& flags() const { return reinterpret_cast<const SemaphoreCreateFlags&>( m_semaphoreCreateInfo.flags ); }
    SemaphoreCreateInfo& flags(SemaphoreCreateFlags flags){ m_semaphoreCreateInfo.flags = static_cast<VkSemaphoreCreateFlags>( flags ); return *this; }
    operator VkSemaphoreCreateInfo&() { return m_semaphoreCreateInfo; }
    operator const VkSemaphoreCreateInfo&() const { return m_semaphoreCreateInfo; }
    VkSemaphoreCreateInfo& vkHandle() { return m_semaphoreCreateInfo; }
    const VkSemaphoreCreateInfo& vkHandle() const { return m_semaphoreCreateInfo; }

  private:
    VkSemaphoreCreateInfo m_semaphoreCreateInfo;
  };

  class FramebufferCreateInfo
  {
  public:
    FramebufferCreateInfo() : FramebufferCreateInfo(FramebufferCreateFlags(), RenderPass(), 0, nullptr, 0, 0, 0) {}
    FramebufferCreateInfo(FramebufferCreateFlags flags, RenderPass renderPass, uint32_t attachmentCount, const ImageView* pAttachments, uint32_t width, uint32_t height, uint32_t layers) {m_framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO; m_framebufferCreateInfo.pNext = nullptr; m_framebufferCreateInfo.flags = static_cast<VkFramebufferCreateFlags>( flags ); m_framebufferCreateInfo.renderPass = renderPass; m_framebufferCreateInfo.attachmentCount = attachmentCount; m_framebufferCreateInfo.pAttachments = pAttachments; m_framebufferCreateInfo.width = width; m_framebufferCreateInfo.height = height; m_framebufferCreateInfo.layers = layers; }
    FramebufferCreateInfo(const VkFramebufferCreateInfo& other) : m_framebufferCreateInfo{other} {}
    FramebufferCreateInfo& operator=(const VkFramebufferCreateInfo& other){ m_framebufferCreateInfo = other; return *this; }

    const StructureType& sType() const { return reinterpret_cast<const StructureType&>( m_framebufferCreateInfo.sType ); }
    FramebufferCreateInfo& sType(StructureType sType){ m_framebufferCreateInfo.sType = static_cast<VkStructureType>( sType ); return *this; }
    const void* pNext() const { return reinterpret_cast<const void*>( m_framebufferCreateInfo.pNext ); }
    FramebufferCreateInfo& pNext(const void* pNext){ m_framebufferCreateInfo.pNext = pNext; return *this; }
    const FramebufferCreateFlags& flags() const { return reinterpret_cast<const FramebufferCreateFlags&>( m_framebufferCreateInfo.flags ); }
    FramebufferCreateInfo& flags(FramebufferCreateFlags flags){ m_framebufferCreateInfo.flags = static_cast<VkFramebufferCreateFlags>( flags ); return *this; }
    const RenderPass& renderPass() const { return m_framebufferCreateInfo.renderPass; }
    FramebufferCreateInfo& renderPass(RenderPass renderPass){ m_framebufferCreateInfo.renderPass = renderPass; return *this; }
    const uint32_t& attachmentCount() const { return m_framebufferCreateInfo.attachmentCount; }
    FramebufferCreateInfo& attachmentCount(uint32_t attachmentCount){ m_framebufferCreateInfo.attachmentCount = attachmentCount; return *this; }
    const ImageView* pAttachments() const { return reinterpret_cast<const ImageView*>( m_framebufferCreateInfo.pAttachments ); }
    FramebufferCreateInfo& pAttachments(const ImageView* pAttachments){ m_framebufferCreateInfo.pAttachments = pAttachments; return *this; }
    const uint32_t& width() const { return m_framebufferCreateInfo.width; }
    FramebufferCreateInfo& width(uint32_t width){ m_framebufferCreateInfo.width = width; return *this; }
    const uint32_t& height() const { return m_framebufferCreateInfo.height; }
    FramebufferCreateInfo& height(uint32_t height){ m_framebufferCreateInfo.height = height; return *this; }
    const uint32_t& layers() const { return m_framebufferCreateInfo.layers; }
    FramebufferCreateInfo& layers(uint32_t layers){ m_framebufferCreateInfo.layers = layers; return *this; }
    operator VkFramebufferCreateInfo&() { return m_framebufferCreateInfo; }
    operator const VkFramebufferCreateInfo&() const { return m_framebufferCreateInfo; }
    VkFramebufferCreateInfo& vkHandle() { return m_framebufferCreateInfo; }
    const VkFramebufferCreateInfo& vkHandle() const { return m_framebufferCreateInfo; }

  private:
    VkFramebufferCreateInfo m_framebufferCreateInfo;
  };

  class PresentInfoKHR
  {
  public:
    PresentInfoKHR() : PresentInfoKHR(0, nullptr, 0, nullptr, nullptr, nullptr) {}
    PresentInfoKHR(uint32_t waitSemaphoreCount, const Semaphore* pWaitSemaphores, uint32_t swapchainCount, const SwapchainKHR* pSwapchains, const uint32_t* pImageIndices, Result* pResults) {m_presentInfoKHR.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR; m_presentInfoKHR.pNext = nullptr; m_presentInfoKHR.waitSemaphoreCount = waitSemaphoreCount; m_presentInfoKHR.pWaitSemaphores = pWaitSemaphores; m_presentInfoKHR.swapchainCount = swapchainCount; m_presentInfoKHR.pSwapchains = pSwapchains; m_presentInfoKHR.pImageIndices = pImageIndices; m_presentInfoKHR.pResults = reinterpret_cast<VkResult*>( pResults ); }
    PresentInfoKHR(const VkPresentInfoKHR& other) : m_presentInfoKHR{other} {}
    PresentInfoKHR& operator=(const VkPresentInfoKHR& other){ m_presentInfoKHR = other; return *this; }

    const StructureType& sType() const { return reinterpret_cast<const StructureType&>( m_presentInfoKHR.sType ); }
    PresentInfoKHR& sType(StructureType sType){ m_presentInfoKHR.sType = static_cast<VkStructureType>( sType ); return *this; }
    const void* pNext() const { return reinterpret_cast<const void*>( m_presentInfoKHR.pNext ); }
    PresentInfoKHR& pNext(const void* pNext){ m_presentInfoKHR.pNext = pNext; return *this; }
    const uint32_t& waitSemaphoreCount() const { return m_presentInfoKHR.waitSemaphoreCount; }
    PresentInfoKHR& waitSemaphoreCount(uint32_t waitSemaphoreCount){ m_presentInfoKHR.waitSemaphoreCount = waitSemaphoreCount; return *this; }
    const Semaphore* pWaitSemaphores() const { return reinterpret_cast<const Semaphore*>( m_presentInfoKHR.pWaitSemaphores ); }
    PresentInfoKHR& pWaitSemaphores(const Semaphore* pWaitSemaphores){ m_presentInfoKHR.pWaitSemaphores = pWaitSemaphores; return *this; }
    const uint32_t& swapchainCount() const { return m_presentInfoKHR.swapchainCount; }
    PresentInfoKHR& swapchainCount(uint32_t swapchainCount){ m_presentInfoKHR.swapchainCount = swapchainCount; return *this; }
    const SwapchainKHR* pSwapchains() const { return reinterpret_cast<const SwapchainKHR*>( m_presentInfoKHR.pSwapchains ); }
    PresentInfoKHR& pSwapchains(const SwapchainKHR* pSwapchains){ m_presentInfoKHR.pSwapchains = pSwapchains; return *this; }
    const uint32_t* pImageIndices() const { return reinterpret_cast<const uint32_t*>( m_presentInfoKHR.pImageIndices ); }
    PresentInfoKHR& pImageIndices(const uint32_t* pImageIndices){ m_presentInfoKHR.pImageIndices = pImageIndices; return *this; }
    const Result* pResults() const { return reinterpret_cast<Result*>( m_presentInfoKHR.pResults ); }
    PresentInfoKHR& pResults(Result* pResults){ m_presentInfoKHR.pResults = reinterpret_cast<VkResult*>( pResults ); return *this; }
    operator VkPresentInfoKHR&() { return m_presentInfoKHR; }
    operator const VkPresentInfoKHR&() const { return m_presentInfoKHR; }
    VkPresentInfoKHR& vkHandle() { return m_presentInfoKHR; }
    const VkPresentInfoKHR& vkHandle() const { return m_presentInfoKHR; }

  private:
    VkPresentInfoKHR m_presentInfoKHR;
  };

  class PipelineDynamicStateCreateInfo
  {
  public:
    PipelineDynamicStateCreateInfo() : PipelineDynamicStateCreateInfo(PipelineDynamicStateCreateFlags(), 0, nullptr) {}
    PipelineDynamicStateCreateInfo(PipelineDynamicStateCreateFlags flags, uint32_t dynamicStateCount, const DynamicState* pDynamicStates) {m_pipelineDynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO; m_pipelineDynamicStateCreateInfo.pNext = nullptr; m_pipelineDynamicStateCreateInfo.flags = static_cast<VkPipelineDynamicStateCreateFlags>( flags ); m_pipelineDynamicStateCreateInfo.dynamicStateCount = dynamicStateCount; m_pipelineDynamicStateCreateInfo.pDynamicStates = reinterpret_cast<const VkDynamicState*>( pDynamicStates ); }
    PipelineDynamicStateCreateInfo(const VkPipelineDynamicStateCreateInfo& other) : m_pipelineDynamicStateCreateInfo{other} {}
    PipelineDynamicStateCreateInfo& operator=(const VkPipelineDynamicStateCreateInfo& other){ m_pipelineDynamicStateCreateInfo = other; return *this; }

    const StructureType& sType() const { return reinterpret_cast<const StructureType&>( m_pipelineDynamicStateCreateInfo.sType ); }
    PipelineDynamicStateCreateInfo& sType(StructureType sType){ m_pipelineDynamicStateCreateInfo.sType = static_cast<VkStructureType>( sType ); return *this; }
    const void* pNext() const { return reinterpret_cast<const void*>( m_pipelineDynamicStateCreateInfo.pNext ); }
    PipelineDynamicStateCreateInfo& pNext(const void* pNext){ m_pipelineDynamicStateCreateInfo.pNext = pNext; return *this; }
    const PipelineDynamicStateCreateFlags& flags() const { return reinterpret_cast<const PipelineDynamicStateCreateFlags&>( m_pipelineDynamicStateCreateInfo.flags ); }
    PipelineDynamicStateCreateInfo& flags(PipelineDynamicStateCreateFlags flags){ m_pipelineDynamicStateCreateInfo.flags = static_cast<VkPipelineDynamicStateCreateFlags>( flags ); return *this; }
    const uint32_t& dynamicStateCount() const { return m_pipelineDynamicStateCreateInfo.dynamicStateCount; }
    PipelineDynamicStateCreateInfo& dynamicStateCount(uint32_t dynamicStateCount){ m_pipelineDynamicStateCreateInfo.dynamicStateCount = dynamicStateCount; return *this; }
    const DynamicState* pDynamicStates() const { return reinterpret_cast<const DynamicState*>( m_pipelineDynamicStateCreateInfo.pDynamicStates ); }
    PipelineDynamicStateCreateInfo& pDynamicStates(const DynamicState* pDynamicStates){ m_pipelineDynamicStateCreateInfo.pDynamicStates = reinterpret_cast<const VkDynamicState*>( pDynamicStates ); return *this; }
    operator VkPipelineDynamicStateCreateInfo&() { return m_pipelineDynamicStateCreateInfo; }
    operator const VkPipelineDynamicStateCreateInfo&() const { return m_pipelineDynamicStateCreateInfo; }
    VkPipelineDynamicStateCreateInfo& vkHandle() { return m_pipelineDynamicStateCreateInfo; }
    const VkPipelineDynamicStateCreateInfo& vkHandle() const { return m_pipelineDynamicStateCreateInfo; }

  private:
    VkPipelineDynamicStateCreateInfo m_pipelineDynamicStateCreateInfo;
  };

  class QueueFamilyProperties
  {
  public:
    const QueueFlags& queueFlags() const { return reinterpret_cast<const QueueFlags&>( m_queueFamilyProperties.queueFlags ); }
    const uint32_t& queueCount() const { return m_queueFamilyProperties.queueCount; }
    const uint32_t& timestampValidBits() const { return m_queueFamilyProperties.timestampValidBits; }
    const Extent3D& minImageTransferGranularity() const { return reinterpret_cast<const Extent3D&>( m_queueFamilyProperties.minImageTransferGranularity ); }
    operator VkQueueFamilyProperties&() { return m_queueFamilyProperties; }
    operator const VkQueueFamilyProperties&() const { return m_queueFamilyProperties; }
    VkQueueFamilyProperties& vkHandle() { return m_queueFamilyProperties; }
    const VkQueueFamilyProperties& vkHandle() const { return m_queueFamilyProperties; }

  private:
    VkQueueFamilyProperties m_queueFamilyProperties;
  };

  class MemoryType
  {
  public:
    const MemoryPropertyFlags& propertyFlags() const { return reinterpret_cast<const MemoryPropertyFlags&>( m_memoryType.propertyFlags ); }
    const uint32_t& heapIndex() const { return m_memoryType.heapIndex; }
    operator VkMemoryType&() { return m_memoryType; }
    operator const VkMemoryType&() const { return m_memoryType; }
    VkMemoryType& vkHandle() { return m_memoryType; }
    const VkMemoryType& vkHandle() const { return m_memoryType; }

  private:
    VkMemoryType m_memoryType;
  };

  class MemoryHeap
  {
  public:
    const DeviceSize& size() const { return m_memoryHeap.size; }
    const MemoryHeapFlags& flags() const { return reinterpret_cast<const MemoryHeapFlags&>( m_memoryHeap.flags ); }
    operator VkMemoryHeap&() { return m_memoryHeap; }
    operator const VkMemoryHeap&() const { return m_memoryHeap; }
    VkMemoryHeap& vkHandle() { return m_memoryHeap; }
    const VkMemoryHeap& vkHandle() const { return m_memoryHeap; }

  private:
    VkMemoryHeap m_memoryHeap;
  };

  class PhysicalDeviceMemoryProperties
  {
  public:
    const uint32_t& memoryTypeCount() const { return m_physicalDeviceMemoryProperties.memoryTypeCount; }
    const MemoryType* memoryTypes() const { return reinterpret_cast<const MemoryType*>( m_physicalDeviceMemoryProperties.memoryTypes ); }
    const uint32_t& memoryHeapCount() const { return m_physicalDeviceMemoryProperties.memoryHeapCount; }
    const MemoryHeap* memoryHeaps() const { return reinterpret_cast<const MemoryHeap*>( m_physicalDeviceMemoryProperties.memoryHeaps ); }
    operator VkPhysicalDeviceMemoryProperties&() { return m_physicalDeviceMemoryProperties; }
    operator const VkPhysicalDeviceMemoryProperties&() const { return m_physicalDeviceMemoryProperties; }
    VkPhysicalDeviceMemoryProperties& vkHandle() { return m_physicalDeviceMemoryProperties; }
    const VkPhysicalDeviceMemoryProperties& vkHandle() const { return m_physicalDeviceMemoryProperties; }

  private:
    VkPhysicalDeviceMemoryProperties m_physicalDeviceMemoryProperties;
  };

  class MemoryBarrier
  {
  public:
    MemoryBarrier() : MemoryBarrier(AccessFlags(), AccessFlags()) {}
    MemoryBarrier(AccessFlags srcAccessMask, AccessFlags dstAccessMask) {m_memoryBarrier.sType = VK_STRUCTURE_TYPE_MEMORY_BARRIER; m_memoryBarrier.pNext = nullptr; m_memoryBarrier.srcAccessMask = static_cast<VkAccessFlags>( srcAccessMask ); m_memoryBarrier.dstAccessMask = static_cast<VkAccessFlags>( dstAccessMask ); }
    MemoryBarrier(const VkMemoryBarrier& other) : m_memoryBarrier{other} {}
    MemoryBarrier& operator=(const VkMemoryBarrier& other){ m_memoryBarrier = other; return *this; }

    const StructureType& sType() const { return reinterpret_cast<const StructureType&>( m_memoryBarrier.sType ); }
    MemoryBarrier& sType(StructureType sType){ m_memoryBarrier.sType = static_cast<VkStructureType>( sType ); return *this; }
    const void* pNext() const { return reinterpret_cast<const void*>( m_memoryBarrier.pNext ); }
    MemoryBarrier& pNext(const void* pNext){ m_memoryBarrier.pNext = pNext; return *this; }
    const AccessFlags& srcAccessMask() const { return reinterpret_cast<const AccessFlags&>( m_memoryBarrier.srcAccessMask ); }
    MemoryBarrier& srcAccessMask(AccessFlags srcAccessMask){ m_memoryBarrier.srcAccessMask = static_cast<VkAccessFlags>( srcAccessMask ); return *this; }
    const AccessFlags& dstAccessMask() const { return reinterpret_cast<const AccessFlags&>( m_memoryBarrier.dstAccessMask ); }
    MemoryBarrier& dstAccessMask(AccessFlags dstAccessMask){ m_memoryBarrier.dstAccessMask = static_cast<VkAccessFlags>( dstAccessMask ); return *this; }
    operator VkMemoryBarrier&() { return m_memoryBarrier; }
    operator const VkMemoryBarrier&() const { return m_memoryBarrier; }
    VkMemoryBarrier& vkHandle() { return m_memoryBarrier; }
    const VkMemoryBarrier& vkHandle() const { return m_memoryBarrier; }

  private:
    VkMemoryBarrier m_memoryBarrier;
  };

  class BufferMemoryBarrier
  {
  public:
    BufferMemoryBarrier() : BufferMemoryBarrier(AccessFlags(), AccessFlags(), 0, 0, Buffer(), 0, 0) {}
    BufferMemoryBarrier(AccessFlags srcAccessMask, AccessFlags dstAccessMask, uint32_t srcQueueFamilyIndex, uint32_t dstQueueFamilyIndex, Buffer buffer, DeviceSize offset, DeviceSize size) {m_bufferMemoryBarrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER; m_bufferMemoryBarrier.pNext = nullptr; m_bufferMemoryBarrier.srcAccessMask = static_cast<VkAccessFlags>( srcAccessMask ); m_bufferMemoryBarrier.dstAccessMask = static_cast<VkAccessFlags>( dstAccessMask ); m_bufferMemoryBarrier.srcQueueFamilyIndex = srcQueueFamilyIndex; m_bufferMemoryBarrier.dstQueueFamilyIndex = dstQueueFamilyIndex; m_bufferMemoryBarrier.buffer = buffer; m_bufferMemoryBarrier.offset = offset; m_bufferMemoryBarrier.size = size; }
    BufferMemoryBarrier(const VkBufferMemoryBarrier& other) : m_bufferMemoryBarrier{other} {}
    BufferMemoryBarrier& operator=(const VkBufferMemoryBarrier& other){ m_bufferMemoryBarrier = other; return *this; }

    const StructureType& sType() const { return reinterpret_cast<const StructureType&>( m_bufferMemoryBarrier.sType ); }
    BufferMemoryBarrier& sType(StructureType sType){ m_bufferMemoryBarrier.sType = static_cast<VkStructureType>( sType ); return *this; }
    const void* pNext() const { return reinterpret_cast<const void*>( m_bufferMemoryBarrier.pNext ); }
    BufferMemoryBarrier& pNext(const void* pNext){ m_bufferMemoryBarrier.pNext = pNext; return *this; }
    const AccessFlags& srcAccessMask() const { return reinterpret_cast<const AccessFlags&>( m_bufferMemoryBarrier.srcAccessMask ); }
    BufferMemoryBarrier& srcAccessMask(AccessFlags srcAccessMask){ m_bufferMemoryBarrier.srcAccessMask = static_cast<VkAccessFlags>( srcAccessMask ); return *this; }
    const AccessFlags& dstAccessMask() const { return reinterpret_cast<const AccessFlags&>( m_bufferMemoryBarrier.dstAccessMask ); }
    BufferMemoryBarrier& dstAccessMask(AccessFlags dstAccessMask){ m_bufferMemoryBarrier.dstAccessMask = static_cast<VkAccessFlags>( dstAccessMask ); return *this; }
    const uint32_t& srcQueueFamilyIndex() const { return m_bufferMemoryBarrier.srcQueueFamilyIndex; }
    BufferMemoryBarrier& srcQueueFamilyIndex(uint32_t srcQueueFamilyIndex){ m_bufferMemoryBarrier.srcQueueFamilyIndex = srcQueueFamilyIndex; return *this; }
    const uint32_t& dstQueueFamilyIndex() const { return m_bufferMemoryBarrier.dstQueueFamilyIndex; }
    BufferMemoryBarrier& dstQueueFamilyIndex(uint32_t dstQueueFamilyIndex){ m_bufferMemoryBarrier.dstQueueFamilyIndex = dstQueueFamilyIndex; return *this; }
    const Buffer& buffer() const { return m_bufferMemoryBarrier.buffer; }
    BufferMemoryBarrier& buffer(Buffer buffer){ m_bufferMemoryBarrier.buffer = buffer; return *this; }
    const DeviceSize& offset() const { return m_bufferMemoryBarrier.offset; }
    BufferMemoryBarrier& offset(DeviceSize offset){ m_bufferMemoryBarrier.offset = offset; return *this; }
    const DeviceSize& size() const { return m_bufferMemoryBarrier.size; }
    BufferMemoryBarrier& size(DeviceSize size){ m_bufferMemoryBarrier.size = size; return *this; }
    operator VkBufferMemoryBarrier&() { return m_bufferMemoryBarrier; }
    operator const VkBufferMemoryBarrier&() const { return m_bufferMemoryBarrier; }
    VkBufferMemoryBarrier& vkHandle() { return m_bufferMemoryBarrier; }
    const VkBufferMemoryBarrier& vkHandle() const { return m_bufferMemoryBarrier; }

  private:
    VkBufferMemoryBarrier m_bufferMemoryBarrier;
  };

  class BufferCreateInfo
  {
  public:
    BufferCreateInfo() : BufferCreateInfo(BufferCreateFlags(), 0, BufferUsageFlags(), SharingMode::Exclusive, 0, nullptr) {}
    BufferCreateInfo(BufferCreateFlags flags, DeviceSize size, BufferUsageFlags usage, SharingMode sharingMode, uint32_t queueFamilyIndexCount, const uint32_t* pQueueFamilyIndices) {m_bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO; m_bufferCreateInfo.pNext = nullptr; m_bufferCreateInfo.flags = static_cast<VkBufferCreateFlags>( flags ); m_bufferCreateInfo.size = size; m_bufferCreateInfo.usage = static_cast<VkBufferUsageFlags>( usage ); m_bufferCreateInfo.sharingMode = static_cast<VkSharingMode>( sharingMode ); m_bufferCreateInfo.queueFamilyIndexCount = queueFamilyIndexCount; m_bufferCreateInfo.pQueueFamilyIndices = pQueueFamilyIndices; }
    BufferCreateInfo(const VkBufferCreateInfo& other) : m_bufferCreateInfo{other} {}
    BufferCreateInfo& operator=(const VkBufferCreateInfo& other){ m_bufferCreateInfo = other; return *this; }

    const StructureType& sType() const { return reinterpret_cast<const StructureType&>( m_bufferCreateInfo.sType ); }
    BufferCreateInfo& sType(StructureType sType){ m_bufferCreateInfo.sType = static_cast<VkStructureType>( sType ); return *this; }
    const void* pNext() const { return reinterpret_cast<const void*>( m_bufferCreateInfo.pNext ); }
    BufferCreateInfo& pNext(const void* pNext){ m_bufferCreateInfo.pNext = pNext; return *this; }
    const BufferCreateFlags& flags() const { return reinterpret_cast<const BufferCreateFlags&>( m_bufferCreateInfo.flags ); }
    BufferCreateInfo& flags(BufferCreateFlags flags){ m_bufferCreateInfo.flags = static_cast<VkBufferCreateFlags>( flags ); return *this; }
    const DeviceSize& size() const { return m_bufferCreateInfo.size; }
    BufferCreateInfo& size(DeviceSize size){ m_bufferCreateInfo.size = size; return *this; }
    const BufferUsageFlags& usage() const { return reinterpret_cast<const BufferUsageFlags&>( m_bufferCreateInfo.usage ); }
    BufferCreateInfo& usage(BufferUsageFlags usage){ m_bufferCreateInfo.usage = static_cast<VkBufferUsageFlags>( usage ); return *this; }
    const SharingMode& sharingMode() const { return reinterpret_cast<const SharingMode&>( m_bufferCreateInfo.sharingMode ); }
    BufferCreateInfo& sharingMode(SharingMode sharingMode){ m_bufferCreateInfo.sharingMode = static_cast<VkSharingMode>( sharingMode ); return *this; }
    const uint32_t& queueFamilyIndexCount() const { return m_bufferCreateInfo.queueFamilyIndexCount; }
    BufferCreateInfo& queueFamilyIndexCount(uint32_t queueFamilyIndexCount){ m_bufferCreateInfo.queueFamilyIndexCount = queueFamilyIndexCount; return *this; }
    const uint32_t* pQueueFamilyIndices() const { return reinterpret_cast<const uint32_t*>( m_bufferCreateInfo.pQueueFamilyIndices ); }
    BufferCreateInfo& pQueueFamilyIndices(const uint32_t* pQueueFamilyIndices){ m_bufferCreateInfo.pQueueFamilyIndices = pQueueFamilyIndices; return *this; }
    operator VkBufferCreateInfo&() { return m_bufferCreateInfo; }
    operator const VkBufferCreateInfo&() const { return m_bufferCreateInfo; }
    VkBufferCreateInfo& vkHandle() { return m_bufferCreateInfo; }
    const VkBufferCreateInfo& vkHandle() const { return m_bufferCreateInfo; }

  private:
    VkBufferCreateInfo m_bufferCreateInfo;
  };

  class DescriptorSetLayoutBinding
  {
  public:
    DescriptorSetLayoutBinding() : DescriptorSetLayoutBinding(0, DescriptorType::Sampler, 0, ShaderStageFlags(), nullptr) {}
    DescriptorSetLayoutBinding(uint32_t binding, DescriptorType descriptorType, uint32_t descriptorCount, ShaderStageFlags stageFlags, const Sampler* pImmutableSamplers) {m_descriptorSetLayoutBinding.binding = binding; m_descriptorSetLayoutBinding.descriptorType = static_cast<VkDescriptorType>( descriptorType ); m_descriptorSetLayoutBinding.descriptorCount = descriptorCount; m_descriptorSetLayoutBinding.stageFlags = static_cast<VkShaderStageFlags>( stageFlags ); m_descriptorSetLayoutBinding.pImmutableSamplers = pImmutableSamplers; }
    DescriptorSetLayoutBinding(const VkDescriptorSetLayoutBinding& other) : m_descriptorSetLayoutBinding{other} {}
    DescriptorSetLayoutBinding& operator=(const VkDescriptorSetLayoutBinding& other){ m_descriptorSetLayoutBinding = other; return *this; }

    const uint32_t& binding() const { return m_descriptorSetLayoutBinding.binding; }
    DescriptorSetLayoutBinding& binding(uint32_t binding){ m_descriptorSetLayoutBinding.binding = binding; return *this; }
    const DescriptorType& descriptorType() const { return reinterpret_cast<const DescriptorType&>( m_descriptorSetLayoutBinding.descriptorType ); }
    DescriptorSetLayoutBinding& descriptorType(DescriptorType descriptorType){ m_descriptorSetLayoutBinding.descriptorType = static_cast<VkDescriptorType>( descriptorType ); return *this; }
    const uint32_t& descriptorCount() const { return m_descriptorSetLayoutBinding.descriptorCount; }
    DescriptorSetLayoutBinding& descriptorCount(uint32_t descriptorCount){ m_descriptorSetLayoutBinding.descriptorCount = descriptorCount; return *this; }
    const ShaderStageFlags& stageFlags() const { return reinterpret_cast<const ShaderStageFlags&>( m_descriptorSetLayoutBinding.stageFlags ); }
    DescriptorSetLayoutBinding& stageFlags(ShaderStageFlags stageFlags){ m_descriptorSetLayoutBinding.stageFlags = static_cast<VkShaderStageFlags>( stageFlags ); return *this; }
    const Sampler* pImmutableSamplers() const { return reinterpret_cast<const Sampler*>( m_descriptorSetLayoutBinding.pImmutableSamplers ); }
    DescriptorSetLayoutBinding& pImmutableSamplers(const Sampler* pImmutableSamplers){ m_descriptorSetLayoutBinding.pImmutableSamplers = pImmutableSamplers; return *this; }
    operator VkDescriptorSetLayoutBinding&() { return m_descriptorSetLayoutBinding; }
    operator const VkDescriptorSetLayoutBinding&() const { return m_descriptorSetLayoutBinding; }
    VkDescriptorSetLayoutBinding& vkHandle() { return m_descriptorSetLayoutBinding; }
    const VkDescriptorSetLayoutBinding& vkHandle() const { return m_descriptorSetLayoutBinding; }

  private:
    VkDescriptorSetLayoutBinding m_descriptorSetLayoutBinding;
  };

  class DescriptorSetLayoutCreateInfo
  {
  public:
    DescriptorSetLayoutCreateInfo() : DescriptorSetLayoutCreateInfo(DescriptorSetLayoutCreateFlags(), 0, nullptr) {}
    DescriptorSetLayoutCreateInfo(DescriptorSetLayoutCreateFlags flags, uint32_t bindingCount, const DescriptorSetLayoutBinding* pBindings) {m_descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO; m_descriptorSetLayoutCreateInfo.pNext = nullptr; m_descriptorSetLayoutCreateInfo.flags = static_cast<VkDescriptorSetLayoutCreateFlags>( flags ); m_descriptorSetLayoutCreateInfo.bindingCount = bindingCount; m_descriptorSetLayoutCreateInfo.pBindings = reinterpret_cast<const VkDescriptorSetLayoutBinding*>( pBindings ); }
    DescriptorSetLayoutCreateInfo(const VkDescriptorSetLayoutCreateInfo& other) : m_descriptorSetLayoutCreateInfo{other} {}
    DescriptorSetLayoutCreateInfo& operator=(const VkDescriptorSetLayoutCreateInfo& other){ m_descriptorSetLayoutCreateInfo = other; return *this; }

    const StructureType& sType() const { return reinterpret_cast<const StructureType&>( m_descriptorSetLayoutCreateInfo.sType ); }
    DescriptorSetLayoutCreateInfo& sType(StructureType sType){ m_descriptorSetLayoutCreateInfo.sType = static_cast<VkStructureType>( sType ); return *this; }
    const void* pNext() const { return reinterpret_cast<const void*>( m_descriptorSetLayoutCreateInfo.pNext ); }
    DescriptorSetLayoutCreateInfo& pNext(const void* pNext){ m_descriptorSetLayoutCreateInfo.pNext = pNext; return *this; }
    const DescriptorSetLayoutCreateFlags& flags() const { return reinterpret_cast<const DescriptorSetLayoutCreateFlags&>( m_descriptorSetLayoutCreateInfo.flags ); }
    DescriptorSetLayoutCreateInfo& flags(DescriptorSetLayoutCreateFlags flags){ m_descriptorSetLayoutCreateInfo.flags = static_cast<VkDescriptorSetLayoutCreateFlags>( flags ); return *this; }
    const uint32_t& bindingCount() const { return m_descriptorSetLayoutCreateInfo.bindingCount; }
    DescriptorSetLayoutCreateInfo& bindingCount(uint32_t bindingCount){ m_descriptorSetLayoutCreateInfo.bindingCount = bindingCount; return *this; }
    const DescriptorSetLayoutBinding* pBindings() const { return reinterpret_cast<const DescriptorSetLayoutBinding*>( m_descriptorSetLayoutCreateInfo.pBindings ); }
    DescriptorSetLayoutCreateInfo& pBindings(const DescriptorSetLayoutBinding* pBindings){ m_descriptorSetLayoutCreateInfo.pBindings = reinterpret_cast<const VkDescriptorSetLayoutBinding*>( pBindings ); return *this; }
    operator VkDescriptorSetLayoutCreateInfo&() { return m_descriptorSetLayoutCreateInfo; }
    operator const VkDescriptorSetLayoutCreateInfo&() const { return m_descriptorSetLayoutCreateInfo; }
    VkDescriptorSetLayoutCreateInfo& vkHandle() { return m_descriptorSetLayoutCreateInfo; }
    const VkDescriptorSetLayoutCreateInfo& vkHandle() const { return m_descriptorSetLayoutCreateInfo; }

  private:
    VkDescriptorSetLayoutCreateInfo m_descriptorSetLayoutCreateInfo;
  };

  class PipelineShaderStageCreateInfo
  {
  public:
    PipelineShaderStageCreateInfo() : PipelineShaderStageCreateInfo(PipelineShaderStageCreateFlags(), ShaderStageFlagBits::Vertex, ShaderModule(), nullptr, nullptr) {}
    PipelineShaderStageCreateInfo(PipelineShaderStageCreateFlags flags, ShaderStageFlagBits stage, ShaderModule module, const char* pName, const SpecializationInfo* pSpecializationInfo) {m_pipelineShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO; m_pipelineShaderStageCreateInfo.pNext = nullptr; m_pipelineShaderStageCreateInfo.flags = static_cast<VkPipelineShaderStageCreateFlags>( flags ); m_pipelineShaderStageCreateInfo.stage = static_cast<VkShaderStageFlagBits>( stage ); m_pipelineShaderStageCreateInfo.module = module; m_pipelineShaderStageCreateInfo.pName = pName; m_pipelineShaderStageCreateInfo.pSpecializationInfo = reinterpret_cast<const VkSpecializationInfo*>( pSpecializationInfo ); }
    PipelineShaderStageCreateInfo(const VkPipelineShaderStageCreateInfo& other) : m_pipelineShaderStageCreateInfo{other} {}
    PipelineShaderStageCreateInfo& operator=(const VkPipelineShaderStageCreateInfo& other){ m_pipelineShaderStageCreateInfo = other; return *this; }

    const StructureType& sType() const { return reinterpret_cast<const StructureType&>( m_pipelineShaderStageCreateInfo.sType ); }
    PipelineShaderStageCreateInfo& sType(StructureType sType){ m_pipelineShaderStageCreateInfo.sType = static_cast<VkStructureType>( sType ); return *this; }
    const void* pNext() const { return reinterpret_cast<const void*>( m_pipelineShaderStageCreateInfo.pNext ); }
    PipelineShaderStageCreateInfo& pNext(const void* pNext){ m_pipelineShaderStageCreateInfo.pNext = pNext; return *this; }
    const PipelineShaderStageCreateFlags& flags() const { return reinterpret_cast<const PipelineShaderStageCreateFlags&>( m_pipelineShaderStageCreateInfo.flags ); }
    PipelineShaderStageCreateInfo& flags(PipelineShaderStageCreateFlags flags){ m_pipelineShaderStageCreateInfo.flags = static_cast<VkPipelineShaderStageCreateFlags>( flags ); return *this; }
    const ShaderStageFlagBits& stage() const { return reinterpret_cast<const ShaderStageFlagBits&>( m_pipelineShaderStageCreateInfo.stage ); }
    PipelineShaderStageCreateInfo& stage(ShaderStageFlagBits stage){ m_pipelineShaderStageCreateInfo.stage = static_cast<VkShaderStageFlagBits>( stage ); return *this; }
    const ShaderModule& module() const { return m_pipelineShaderStageCreateInfo.module; }
    PipelineShaderStageCreateInfo& module(ShaderModule module){ m_pipelineShaderStageCreateInfo.module = module; return *this; }
    const char* pName() const { return reinterpret_cast<const char*>( m_pipelineShaderStageCreateInfo.pName ); }
    PipelineShaderStageCreateInfo& pName(const char* pName){ m_pipelineShaderStageCreateInfo.pName = pName; return *this; }
    const SpecializationInfo* pSpecializationInfo() const { return reinterpret_cast<const SpecializationInfo*>( m_pipelineShaderStageCreateInfo.pSpecializationInfo ); }
    PipelineShaderStageCreateInfo& pSpecializationInfo(const SpecializationInfo* pSpecializationInfo){ m_pipelineShaderStageCreateInfo.pSpecializationInfo = reinterpret_cast<const VkSpecializationInfo*>( pSpecializationInfo ); return *this; }
    operator VkPipelineShaderStageCreateInfo&() { return m_pipelineShaderStageCreateInfo; }
    operator const VkPipelineShaderStageCreateInfo&() const { return m_pipelineShaderStageCreateInfo; }
    VkPipelineShaderStageCreateInfo& vkHandle() { return m_pipelineShaderStageCreateInfo; }
    const VkPipelineShaderStageCreateInfo& vkHandle() const { return m_pipelineShaderStageCreateInfo; }

  private:
    VkPipelineShaderStageCreateInfo m_pipelineShaderStageCreateInfo;
  };

  class PushConstantRange
  {
  public:
    PushConstantRange() : PushConstantRange(ShaderStageFlags(), 0, 0) {}
    PushConstantRange(ShaderStageFlags stageFlags, uint32_t offset, uint32_t size) {m_pushConstantRange.stageFlags = static_cast<VkShaderStageFlags>( stageFlags ); m_pushConstantRange.offset = offset; m_pushConstantRange.size = size; }
    PushConstantRange(const VkPushConstantRange& other) : m_pushConstantRange{other} {}
    PushConstantRange& operator=(const VkPushConstantRange& other){ m_pushConstantRange = other; return *this; }

    const ShaderStageFlags& stageFlags() const { return reinterpret_cast<const ShaderStageFlags&>( m_pushConstantRange.stageFlags ); }
    PushConstantRange& stageFlags(ShaderStageFlags stageFlags){ m_pushConstantRange.stageFlags = static_cast<VkShaderStageFlags>( stageFlags ); return *this; }
    const uint32_t& offset() const { return m_pushConstantRange.offset; }
    PushConstantRange& offset(uint32_t offset){ m_pushConstantRange.offset = offset; return *this; }
    const uint32_t& size() const { return m_pushConstantRange.size; }
    PushConstantRange& size(uint32_t size){ m_pushConstantRange.size = size; return *this; }
    operator VkPushConstantRange&() { return m_pushConstantRange; }
    operator const VkPushConstantRange&() const { return m_pushConstantRange; }
    VkPushConstantRange& vkHandle() { return m_pushConstantRange; }
    const VkPushConstantRange& vkHandle() const { return m_pushConstantRange; }

  private:
    VkPushConstantRange m_pushConstantRange;
  };

  class PipelineLayoutCreateInfo
  {
  public:
    PipelineLayoutCreateInfo() : PipelineLayoutCreateInfo(PipelineLayoutCreateFlags(), 0, nullptr, 0, nullptr) {}
    PipelineLayoutCreateInfo(PipelineLayoutCreateFlags flags, uint32_t setLayoutCount, const DescriptorSetLayout* pSetLayouts, uint32_t pushConstantRangeCount, const PushConstantRange* pPushConstantRanges) {m_pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO; m_pipelineLayoutCreateInfo.pNext = nullptr; m_pipelineLayoutCreateInfo.flags = static_cast<VkPipelineLayoutCreateFlags>( flags ); m_pipelineLayoutCreateInfo.setLayoutCount = setLayoutCount; m_pipelineLayoutCreateInfo.pSetLayouts = pSetLayouts; m_pipelineLayoutCreateInfo.pushConstantRangeCount = pushConstantRangeCount; m_pipelineLayoutCreateInfo.pPushConstantRanges = reinterpret_cast<const VkPushConstantRange*>( pPushConstantRanges ); }
    PipelineLayoutCreateInfo(const VkPipelineLayoutCreateInfo& other) : m_pipelineLayoutCreateInfo{other} {}
    PipelineLayoutCreateInfo& operator=(const VkPipelineLayoutCreateInfo& other){ m_pipelineLayoutCreateInfo = other; return *this; }

    const StructureType& sType() const { return reinterpret_cast<const StructureType&>( m_pipelineLayoutCreateInfo.sType ); }
    PipelineLayoutCreateInfo& sType(StructureType sType){ m_pipelineLayoutCreateInfo.sType = static_cast<VkStructureType>( sType ); return *this; }
    const void* pNext() const { return reinterpret_cast<const void*>( m_pipelineLayoutCreateInfo.pNext ); }
    PipelineLayoutCreateInfo& pNext(const void* pNext){ m_pipelineLayoutCreateInfo.pNext = pNext; return *this; }
    const PipelineLayoutCreateFlags& flags() const { return reinterpret_cast<const PipelineLayoutCreateFlags&>( m_pipelineLayoutCreateInfo.flags ); }
    PipelineLayoutCreateInfo& flags(PipelineLayoutCreateFlags flags){ m_pipelineLayoutCreateInfo.flags = static_cast<VkPipelineLayoutCreateFlags>( flags ); return *this; }
    const uint32_t& setLayoutCount() const { return m_pipelineLayoutCreateInfo.setLayoutCount; }
    PipelineLayoutCreateInfo& setLayoutCount(uint32_t setLayoutCount){ m_pipelineLayoutCreateInfo.setLayoutCount = setLayoutCount; return *this; }
    const DescriptorSetLayout* pSetLayouts() const { return reinterpret_cast<const DescriptorSetLayout*>( m_pipelineLayoutCreateInfo.pSetLayouts ); }
    PipelineLayoutCreateInfo& pSetLayouts(const DescriptorSetLayout* pSetLayouts){ m_pipelineLayoutCreateInfo.pSetLayouts = pSetLayouts; return *this; }
    const uint32_t& pushConstantRangeCount() const { return m_pipelineLayoutCreateInfo.pushConstantRangeCount; }
    PipelineLayoutCreateInfo& pushConstantRangeCount(uint32_t pushConstantRangeCount){ m_pipelineLayoutCreateInfo.pushConstantRangeCount = pushConstantRangeCount; return *this; }
    const PushConstantRange* pPushConstantRanges() const { return reinterpret_cast<const PushConstantRange*>( m_pipelineLayoutCreateInfo.pPushConstantRanges ); }
    PipelineLayoutCreateInfo& pPushConstantRanges(const PushConstantRange* pPushConstantRanges){ m_pipelineLayoutCreateInfo.pPushConstantRanges = reinterpret_cast<const VkPushConstantRange*>( pPushConstantRanges ); return *this; }
    operator VkPipelineLayoutCreateInfo&() { return m_pipelineLayoutCreateInfo; }
    operator const VkPipelineLayoutCreateInfo&() const { return m_pipelineLayoutCreateInfo; }
    VkPipelineLayoutCreateInfo& vkHandle() { return m_pipelineLayoutCreateInfo; }
    const VkPipelineLayoutCreateInfo& vkHandle() const { return m_pipelineLayoutCreateInfo; }

  private:
    VkPipelineLayoutCreateInfo m_pipelineLayoutCreateInfo;
  };

  class ComputePipelineCreateInfo
  {
  public:
    ComputePipelineCreateInfo() : ComputePipelineCreateInfo(PipelineCreateFlags(), PipelineShaderStageCreateInfo(), PipelineLayout(), Pipeline(), 0) {}
    ComputePipelineCreateInfo(PipelineCreateFlags flags, PipelineShaderStageCreateInfo stage, PipelineLayout layout, Pipeline basePipelineHandle, int32_t basePipelineIndex) {m_computePipelineCreateInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO; m_computePipelineCreateInfo.pNext = nullptr; m_computePipelineCreateInfo.flags = static_cast<VkPipelineCreateFlags>( flags ); m_computePipelineCreateInfo.stage = static_cast<VkPipelineShaderStageCreateInfo>( stage ); m_computePipelineCreateInfo.layout = layout; m_computePipelineCreateInfo.basePipelineHandle = basePipelineHandle; m_computePipelineCreateInfo.basePipelineIndex = basePipelineIndex; }
    ComputePipelineCreateInfo(const VkComputePipelineCreateInfo& other) : m_computePipelineCreateInfo{other} {}
    ComputePipelineCreateInfo& operator=(const VkComputePipelineCreateInfo& other){ m_computePipelineCreateInfo = other; return *this; }

    const StructureType& sType() const { return reinterpret_cast<const StructureType&>( m_computePipelineCreateInfo.sType ); }
    ComputePipelineCreateInfo& sType(StructureType sType){ m_computePipelineCreateInfo.sType = static_cast<VkStructureType>( sType ); return *this; }
    const void* pNext() const { return reinterpret_cast<const void*>( m_computePipelineCreateInfo.pNext ); }
    ComputePipelineCreateInfo& pNext(const void* pNext){ m_computePipelineCreateInfo.pNext = pNext; return *this; }
    const PipelineCreateFlags& flags() const { return reinterpret_cast<const PipelineCreateFlags&>( m_computePipelineCreateInfo.flags ); }
    ComputePipelineCreateInfo& flags(PipelineCreateFlags flags){ m_computePipelineCreateInfo.flags = static_cast<VkPipelineCreateFlags>( flags ); return *this; }
    const PipelineShaderStageCreateInfo& stage() const { return reinterpret_cast<const PipelineShaderStageCreateInfo&>( m_computePipelineCreateInfo.stage ); }
    ComputePipelineCreateInfo& stage(PipelineShaderStageCreateInfo stage){ m_computePipelineCreateInfo.stage = static_cast<VkPipelineShaderStageCreateInfo>( stage ); return *this; }
    const PipelineLayout& layout() const { return m_computePipelineCreateInfo.layout; }
    ComputePipelineCreateInfo& layout(PipelineLayout layout){ m_computePipelineCreateInfo.layout = layout; return *this; }
    const Pipeline& basePipelineHandle() const { return m_computePipelineCreateInfo.basePipelineHandle; }
    ComputePipelineCreateInfo& basePipelineHandle(Pipeline basePipelineHandle){ m_computePipelineCreateInfo.basePipelineHandle = basePipelineHandle; return *this; }
    const int32_t& basePipelineIndex() const { return m_computePipelineCreateInfo.basePipelineIndex; }
    ComputePipelineCreateInfo& basePipelineIndex(int32_t basePipelineIndex){ m_computePipelineCreateInfo.basePipelineIndex = basePipelineIndex; return *this; }
    operator VkComputePipelineCreateInfo&() { return m_computePipelineCreateInfo; }
    operator const VkComputePipelineCreateInfo&() const { return m_computePipelineCreateInfo; }
    VkComputePipelineCreateInfo& vkHandle() { return m_computePipelineCreateInfo; }
    const VkComputePipelineCreateInfo& vkHandle() const { return m_computePipelineCreateInfo; }

  private:
    VkComputePipelineCreateInfo m_computePipelineCreateInfo;
  };

  class PipelineColorBlendAttachmentState
  {
  public:
    PipelineColorBlendAttachmentState() : PipelineColorBlendAttachmentState(0, BlendFactor::Zero, BlendFactor::Zero, BlendOp::Add, BlendFactor::Zero, BlendFactor::Zero, BlendOp::Add, ColorComponentFlags()) {}
    PipelineColorBlendAttachmentState(Bool32 blendEnable, BlendFactor srcColorBlendFactor, BlendFactor dstColorBlendFactor, BlendOp colorBlendOp, BlendFactor srcAlphaBlendFactor, BlendFactor dstAlphaBlendFactor, BlendOp alphaBlendOp, ColorComponentFlags colorWriteMask) {m_pipelineColorBlendAttachmentState.blendEnable = blendEnable; m_pipelineColorBlendAttachmentState.srcColorBlendFactor = static_cast<VkBlendFactor>( srcColorBlendFactor ); m_pipelineColorBlendAttachmentState.dstColorBlendFactor = static_cast<VkBlendFactor>( dstColorBlendFactor ); m_pipelineColorBlendAttachmentState.colorBlendOp = static_cast<VkBlendOp>( colorBlendOp ); m_pipelineColorBlendAttachmentState.srcAlphaBlendFactor = static_cast<VkBlendFactor>( srcAlphaBlendFactor ); m_pipelineColorBlendAttachmentState.dstAlphaBlendFactor = static_cast<VkBlendFactor>( dstAlphaBlendFactor ); m_pipelineColorBlendAttachmentState.alphaBlendOp = static_cast<VkBlendOp>( alphaBlendOp ); m_pipelineColorBlendAttachmentState.colorWriteMask = static_cast<VkColorComponentFlags>( colorWriteMask ); }
    PipelineColorBlendAttachmentState(const VkPipelineColorBlendAttachmentState& other) : m_pipelineColorBlendAttachmentState{other} {}
    PipelineColorBlendAttachmentState& operator=(const VkPipelineColorBlendAttachmentState& other){ m_pipelineColorBlendAttachmentState = other; return *this; }

    const Bool32& blendEnable() const { return m_pipelineColorBlendAttachmentState.blendEnable; }
    PipelineColorBlendAttachmentState& blendEnable(Bool32 blendEnable){ m_pipelineColorBlendAttachmentState.blendEnable = blendEnable; return *this; }
    const BlendFactor& srcColorBlendFactor() const { return reinterpret_cast<const BlendFactor&>( m_pipelineColorBlendAttachmentState.srcColorBlendFactor ); }
    PipelineColorBlendAttachmentState& srcColorBlendFactor(BlendFactor srcColorBlendFactor){ m_pipelineColorBlendAttachmentState.srcColorBlendFactor = static_cast<VkBlendFactor>( srcColorBlendFactor ); return *this; }
    const BlendFactor& dstColorBlendFactor() const { return reinterpret_cast<const BlendFactor&>( m_pipelineColorBlendAttachmentState.dstColorBlendFactor ); }
    PipelineColorBlendAttachmentState& dstColorBlendFactor(BlendFactor dstColorBlendFactor){ m_pipelineColorBlendAttachmentState.dstColorBlendFactor = static_cast<VkBlendFactor>( dstColorBlendFactor ); return *this; }
    const BlendOp& colorBlendOp() const { return reinterpret_cast<const BlendOp&>( m_pipelineColorBlendAttachmentState.colorBlendOp ); }
    PipelineColorBlendAttachmentState& colorBlendOp(BlendOp colorBlendOp){ m_pipelineColorBlendAttachmentState.colorBlendOp = static_cast<VkBlendOp>( colorBlendOp ); return *this; }
    const BlendFactor& srcAlphaBlendFactor() const { return reinterpret_cast<const BlendFactor&>( m_pipelineColorBlendAttachmentState.srcAlphaBlendFactor ); }
    PipelineColorBlendAttachmentState& srcAlphaBlendFactor(BlendFactor srcAlphaBlendFactor){ m_pipelineColorBlendAttachmentState.srcAlphaBlendFactor = static_cast<VkBlendFactor>( srcAlphaBlendFactor ); return *this; }
    const BlendFactor& dstAlphaBlendFactor() const { return reinterpret_cast<const BlendFactor&>( m_pipelineColorBlendAttachmentState.dstAlphaBlendFactor ); }
    PipelineColorBlendAttachmentState& dstAlphaBlendFactor(BlendFactor dstAlphaBlendFactor){ m_pipelineColorBlendAttachmentState.dstAlphaBlendFactor = static_cast<VkBlendFactor>( dstAlphaBlendFactor ); return *this; }
    const BlendOp& alphaBlendOp() const { return reinterpret_cast<const BlendOp&>( m_pipelineColorBlendAttachmentState.alphaBlendOp ); }
    PipelineColorBlendAttachmentState& alphaBlendOp(BlendOp alphaBlendOp){ m_pipelineColorBlendAttachmentState.alphaBlendOp = static_cast<VkBlendOp>( alphaBlendOp ); return *this; }
    const ColorComponentFlags& colorWriteMask() const { return reinterpret_cast<const ColorComponentFlags&>( m_pipelineColorBlendAttachmentState.colorWriteMask ); }
    PipelineColorBlendAttachmentState& colorWriteMask(ColorComponentFlags colorWriteMask){ m_pipelineColorBlendAttachmentState.colorWriteMask = static_cast<VkColorComponentFlags>( colorWriteMask ); return *this; }
    operator VkPipelineColorBlendAttachmentState&() { return m_pipelineColorBlendAttachmentState; }
    operator const VkPipelineColorBlendAttachmentState&() const { return m_pipelineColorBlendAttachmentState; }
    VkPipelineColorBlendAttachmentState& vkHandle() { return m_pipelineColorBlendAttachmentState; }
    const VkPipelineColorBlendAttachmentState& vkHandle() const { return m_pipelineColorBlendAttachmentState; }

  private:
    VkPipelineColorBlendAttachmentState m_pipelineColorBlendAttachmentState;
  };

  class PipelineColorBlendStateCreateInfo
  {
  public:
    PipelineColorBlendStateCreateInfo() : PipelineColorBlendStateCreateInfo(PipelineColorBlendStateCreateFlags(), 0, LogicOp::Clear, 0, nullptr, { 0 }) {}
    PipelineColorBlendStateCreateInfo(PipelineColorBlendStateCreateFlags flags, Bool32 logicOpEnable, LogicOp logicOp, uint32_t attachmentCount, const PipelineColorBlendAttachmentState* pAttachments, std::array<float,4> const& blendConstants) {m_pipelineColorBlendStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO; m_pipelineColorBlendStateCreateInfo.pNext = nullptr; m_pipelineColorBlendStateCreateInfo.flags = static_cast<VkPipelineColorBlendStateCreateFlags>( flags ); m_pipelineColorBlendStateCreateInfo.logicOpEnable = logicOpEnable; m_pipelineColorBlendStateCreateInfo.logicOp = static_cast<VkLogicOp>( logicOp ); m_pipelineColorBlendStateCreateInfo.attachmentCount = attachmentCount; m_pipelineColorBlendStateCreateInfo.pAttachments = reinterpret_cast<const VkPipelineColorBlendAttachmentState*>( pAttachments ); memcpy(&m_pipelineColorBlendStateCreateInfo.blendConstants, blendConstants.data(), 4 * sizeof(float)); }
    PipelineColorBlendStateCreateInfo(const VkPipelineColorBlendStateCreateInfo& other) : m_pipelineColorBlendStateCreateInfo{other} {}
    PipelineColorBlendStateCreateInfo& operator=(const VkPipelineColorBlendStateCreateInfo& other){ m_pipelineColorBlendStateCreateInfo = other; return *this; }

    const StructureType& sType() const { return reinterpret_cast<const StructureType&>( m_pipelineColorBlendStateCreateInfo.sType ); }
    PipelineColorBlendStateCreateInfo& sType(StructureType sType){ m_pipelineColorBlendStateCreateInfo.sType = static_cast<VkStructureType>( sType ); return *this; }
    const void* pNext() const { return reinterpret_cast<const void*>( m_pipelineColorBlendStateCreateInfo.pNext ); }
    PipelineColorBlendStateCreateInfo& pNext(const void* pNext){ m_pipelineColorBlendStateCreateInfo.pNext = pNext; return *this; }
    const PipelineColorBlendStateCreateFlags& flags() const { return reinterpret_cast<const PipelineColorBlendStateCreateFlags&>( m_pipelineColorBlendStateCreateInfo.flags ); }
    PipelineColorBlendStateCreateInfo& flags(PipelineColorBlendStateCreateFlags flags){ m_pipelineColorBlendStateCreateInfo.flags = static_cast<VkPipelineColorBlendStateCreateFlags>( flags ); return *this; }
    const Bool32& logicOpEnable() const { return m_pipelineColorBlendStateCreateInfo.logicOpEnable; }
    PipelineColorBlendStateCreateInfo& logicOpEnable(Bool32 logicOpEnable){ m_pipelineColorBlendStateCreateInfo.logicOpEnable = logicOpEnable; return *this; }
    const LogicOp& logicOp() const { return reinterpret_cast<const LogicOp&>( m_pipelineColorBlendStateCreateInfo.logicOp ); }
    PipelineColorBlendStateCreateInfo& logicOp(LogicOp logicOp){ m_pipelineColorBlendStateCreateInfo.logicOp = static_cast<VkLogicOp>( logicOp ); return *this; }
    const uint32_t& attachmentCount() const { return m_pipelineColorBlendStateCreateInfo.attachmentCount; }
    PipelineColorBlendStateCreateInfo& attachmentCount(uint32_t attachmentCount){ m_pipelineColorBlendStateCreateInfo.attachmentCount = attachmentCount; return *this; }
    const PipelineColorBlendAttachmentState* pAttachments() const { return reinterpret_cast<const PipelineColorBlendAttachmentState*>( m_pipelineColorBlendStateCreateInfo.pAttachments ); }
    PipelineColorBlendStateCreateInfo& pAttachments(const PipelineColorBlendAttachmentState* pAttachments){ m_pipelineColorBlendStateCreateInfo.pAttachments = reinterpret_cast<const VkPipelineColorBlendAttachmentState*>( pAttachments ); return *this; }
    const float* blendConstants() const { return reinterpret_cast<const float*>( m_pipelineColorBlendStateCreateInfo.blendConstants ); }
    PipelineColorBlendStateCreateInfo& blendConstants(std::array<float,4> blendConstants){ memcpy(&m_pipelineColorBlendStateCreateInfo.blendConstants, blendConstants.data(), 4 * sizeof(float)); return *this; }
    operator VkPipelineColorBlendStateCreateInfo&() { return m_pipelineColorBlendStateCreateInfo; }
    operator const VkPipelineColorBlendStateCreateInfo&() const { return m_pipelineColorBlendStateCreateInfo; }
    VkPipelineColorBlendStateCreateInfo& vkHandle() { return m_pipelineColorBlendStateCreateInfo; }
    const VkPipelineColorBlendStateCreateInfo& vkHandle() const { return m_pipelineColorBlendStateCreateInfo; }

  private:
    VkPipelineColorBlendStateCreateInfo m_pipelineColorBlendStateCreateInfo;
  };

  class FenceCreateInfo
  {
  public:
    FenceCreateInfo() : FenceCreateInfo(FenceCreateFlags()) {}
    FenceCreateInfo(FenceCreateFlags flags) {m_fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO; m_fenceCreateInfo.pNext = nullptr; m_fenceCreateInfo.flags = static_cast<VkFenceCreateFlags>( flags ); }
    FenceCreateInfo(const VkFenceCreateInfo& other) : m_fenceCreateInfo{other} {}
    FenceCreateInfo& operator=(const VkFenceCreateInfo& other){ m_fenceCreateInfo = other; return *this; }

    const StructureType& sType() const { return reinterpret_cast<const StructureType&>( m_fenceCreateInfo.sType ); }
    FenceCreateInfo& sType(StructureType sType){ m_fenceCreateInfo.sType = static_cast<VkStructureType>( sType ); return *this; }
    const void* pNext() const { return reinterpret_cast<const void*>( m_fenceCreateInfo.pNext ); }
    FenceCreateInfo& pNext(const void* pNext){ m_fenceCreateInfo.pNext = pNext; return *this; }
    const FenceCreateFlags& flags() const { return reinterpret_cast<const FenceCreateFlags&>( m_fenceCreateInfo.flags ); }
    FenceCreateInfo& flags(FenceCreateFlags flags){ m_fenceCreateInfo.flags = static_cast<VkFenceCreateFlags>( flags ); return *this; }
    operator VkFenceCreateInfo&() { return m_fenceCreateInfo; }
    operator const VkFenceCreateInfo&() const { return m_fenceCreateInfo; }
    VkFenceCreateInfo& vkHandle() { return m_fenceCreateInfo; }
    const VkFenceCreateInfo& vkHandle() const { return m_fenceCreateInfo; }

  private:
    VkFenceCreateInfo m_fenceCreateInfo;
  };

  class FormatProperties
  {
  public:
    const FormatFeatureFlags& linearTilingFeatures() const { return reinterpret_cast<const FormatFeatureFlags&>( m_formatProperties.linearTilingFeatures ); }
    const FormatFeatureFlags& optimalTilingFeatures() const { return reinterpret_cast<const FormatFeatureFlags&>( m_formatProperties.optimalTilingFeatures ); }
    const FormatFeatureFlags& bufferFeatures() const { return reinterpret_cast<const FormatFeatureFlags&>( m_formatProperties.bufferFeatures ); }
    operator VkFormatProperties&() { return m_formatProperties; }
    operator const VkFormatProperties&() const { return m_formatProperties; }
    VkFormatProperties& vkHandle() { return m_formatProperties; }
    const VkFormatProperties& vkHandle() const { return m_formatProperties; }

  private:
    VkFormatProperties m_formatProperties;
  };

  class CommandBufferInheritanceInfo
  {
  public:
    CommandBufferInheritanceInfo() : CommandBufferInheritanceInfo(RenderPass(), 0, Framebuffer(), 0, QueryControlFlags(), QueryPipelineStatisticFlags()) {}
    CommandBufferInheritanceInfo(RenderPass renderPass, uint32_t subpass, Framebuffer framebuffer, Bool32 occlusionQueryEnable, QueryControlFlags queryFlags, QueryPipelineStatisticFlags pipelineStatistics) {m_commandBufferInheritanceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO; m_commandBufferInheritanceInfo.pNext = nullptr; m_commandBufferInheritanceInfo.renderPass = renderPass; m_commandBufferInheritanceInfo.subpass = subpass; m_commandBufferInheritanceInfo.framebuffer = framebuffer; m_commandBufferInheritanceInfo.occlusionQueryEnable = occlusionQueryEnable; m_commandBufferInheritanceInfo.queryFlags = static_cast<VkQueryControlFlags>( queryFlags ); m_commandBufferInheritanceInfo.pipelineStatistics = static_cast<VkQueryPipelineStatisticFlags>( pipelineStatistics ); }
    CommandBufferInheritanceInfo(const VkCommandBufferInheritanceInfo& other) : m_commandBufferInheritanceInfo{other} {}
    CommandBufferInheritanceInfo& operator=(const VkCommandBufferInheritanceInfo& other){ m_commandBufferInheritanceInfo = other; return *this; }

    const StructureType& sType() const { return reinterpret_cast<const StructureType&>( m_commandBufferInheritanceInfo.sType ); }
    CommandBufferInheritanceInfo& sType(StructureType sType){ m_commandBufferInheritanceInfo.sType = static_cast<VkStructureType>( sType ); return *this; }
    const void* pNext() const { return reinterpret_cast<const void*>( m_commandBufferInheritanceInfo.pNext ); }
    CommandBufferInheritanceInfo& pNext(const void* pNext){ m_commandBufferInheritanceInfo.pNext = pNext; return *this; }
    const RenderPass& renderPass() const { return m_commandBufferInheritanceInfo.renderPass; }
    CommandBufferInheritanceInfo& renderPass(RenderPass renderPass){ m_commandBufferInheritanceInfo.renderPass = renderPass; return *this; }
    const uint32_t& subpass() const { return m_commandBufferInheritanceInfo.subpass; }
    CommandBufferInheritanceInfo& subpass(uint32_t subpass){ m_commandBufferInheritanceInfo.subpass = subpass; return *this; }
    const Framebuffer& framebuffer() const { return m_commandBufferInheritanceInfo.framebuffer; }
    CommandBufferInheritanceInfo& framebuffer(Framebuffer framebuffer){ m_commandBufferInheritanceInfo.framebuffer = framebuffer; return *this; }
    const Bool32& occlusionQueryEnable() const { return m_commandBufferInheritanceInfo.occlusionQueryEnable; }
    CommandBufferInheritanceInfo& occlusionQueryEnable(Bool32 occlusionQueryEnable){ m_commandBufferInheritanceInfo.occlusionQueryEnable = occlusionQueryEnable; return *this; }
    const QueryControlFlags& queryFlags() const { return reinterpret_cast<const QueryControlFlags&>( m_commandBufferInheritanceInfo.queryFlags ); }
    CommandBufferInheritanceInfo& queryFlags(QueryControlFlags queryFlags){ m_commandBufferInheritanceInfo.queryFlags = static_cast<VkQueryControlFlags>( queryFlags ); return *this; }
    const QueryPipelineStatisticFlags& pipelineStatistics() const { return reinterpret_cast<const QueryPipelineStatisticFlags&>( m_commandBufferInheritanceInfo.pipelineStatistics ); }
    CommandBufferInheritanceInfo& pipelineStatistics(QueryPipelineStatisticFlags pipelineStatistics){ m_commandBufferInheritanceInfo.pipelineStatistics = static_cast<VkQueryPipelineStatisticFlags>( pipelineStatistics ); return *this; }
    operator VkCommandBufferInheritanceInfo&() { return m_commandBufferInheritanceInfo; }
    operator const VkCommandBufferInheritanceInfo&() const { return m_commandBufferInheritanceInfo; }
    VkCommandBufferInheritanceInfo& vkHandle() { return m_commandBufferInheritanceInfo; }
    const VkCommandBufferInheritanceInfo& vkHandle() const { return m_commandBufferInheritanceInfo; }

  private:
    VkCommandBufferInheritanceInfo m_commandBufferInheritanceInfo;
  };

  class CommandBufferBeginInfo
  {
  public:
    CommandBufferBeginInfo() : CommandBufferBeginInfo(CommandBufferUsageFlags(), nullptr) {}
    CommandBufferBeginInfo(CommandBufferUsageFlags flags, const CommandBufferInheritanceInfo* pInheritanceInfo) {m_commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO; m_commandBufferBeginInfo.pNext = nullptr; m_commandBufferBeginInfo.flags = static_cast<VkCommandBufferUsageFlags>( flags ); m_commandBufferBeginInfo.pInheritanceInfo = reinterpret_cast<const VkCommandBufferInheritanceInfo*>( pInheritanceInfo ); }
    CommandBufferBeginInfo(const VkCommandBufferBeginInfo& other) : m_commandBufferBeginInfo{other} {}
    CommandBufferBeginInfo& operator=(const VkCommandBufferBeginInfo& other){ m_commandBufferBeginInfo = other; return *this; }

    const StructureType& sType() const { return reinterpret_cast<const StructureType&>( m_commandBufferBeginInfo.sType ); }
    CommandBufferBeginInfo& sType(StructureType sType){ m_commandBufferBeginInfo.sType = static_cast<VkStructureType>( sType ); return *this; }
    const void* pNext() const { return reinterpret_cast<const void*>( m_commandBufferBeginInfo.pNext ); }
    CommandBufferBeginInfo& pNext(const void* pNext){ m_commandBufferBeginInfo.pNext = pNext; return *this; }
    const CommandBufferUsageFlags& flags() const { return reinterpret_cast<const CommandBufferUsageFlags&>( m_commandBufferBeginInfo.flags ); }
    CommandBufferBeginInfo& flags(CommandBufferUsageFlags flags){ m_commandBufferBeginInfo.flags = static_cast<VkCommandBufferUsageFlags>( flags ); return *this; }
    const CommandBufferInheritanceInfo* pInheritanceInfo() const { return reinterpret_cast<const CommandBufferInheritanceInfo*>( m_commandBufferBeginInfo.pInheritanceInfo ); }
    CommandBufferBeginInfo& pInheritanceInfo(const CommandBufferInheritanceInfo* pInheritanceInfo){ m_commandBufferBeginInfo.pInheritanceInfo = reinterpret_cast<const VkCommandBufferInheritanceInfo*>( pInheritanceInfo ); return *this; }
    operator VkCommandBufferBeginInfo&() { return m_commandBufferBeginInfo; }
    operator const VkCommandBufferBeginInfo&() const { return m_commandBufferBeginInfo; }
    VkCommandBufferBeginInfo& vkHandle() { return m_commandBufferBeginInfo; }
    const VkCommandBufferBeginInfo& vkHandle() const { return m_commandBufferBeginInfo; }

  private:
    VkCommandBufferBeginInfo m_commandBufferBeginInfo;
  };

  class QueryPoolCreateInfo
  {
  public:
    QueryPoolCreateInfo() : QueryPoolCreateInfo(QueryPoolCreateFlags(), QueryType::Occlusion, 0, QueryPipelineStatisticFlags()) {}
    QueryPoolCreateInfo(QueryPoolCreateFlags flags, QueryType queryType, uint32_t queryCount, QueryPipelineStatisticFlags pipelineStatistics) {m_queryPoolCreateInfo.sType = VK_STRUCTURE_TYPE_QUERY_POOL_CREATE_INFO; m_queryPoolCreateInfo.pNext = nullptr; m_queryPoolCreateInfo.flags = static_cast<VkQueryPoolCreateFlags>( flags ); m_queryPoolCreateInfo.queryType = static_cast<VkQueryType>( queryType ); m_queryPoolCreateInfo.queryCount = queryCount; m_queryPoolCreateInfo.pipelineStatistics = static_cast<VkQueryPipelineStatisticFlags>( pipelineStatistics ); }
    QueryPoolCreateInfo(const VkQueryPoolCreateInfo& other) : m_queryPoolCreateInfo{other} {}
    QueryPoolCreateInfo& operator=(const VkQueryPoolCreateInfo& other){ m_queryPoolCreateInfo = other; return *this; }

    const StructureType& sType() const { return reinterpret_cast<const StructureType&>( m_queryPoolCreateInfo.sType ); }
    QueryPoolCreateInfo& sType(StructureType sType){ m_queryPoolCreateInfo.sType = static_cast<VkStructureType>( sType ); return *this; }
    const void* pNext() const { return reinterpret_cast<const void*>( m_queryPoolCreateInfo.pNext ); }
    QueryPoolCreateInfo& pNext(const void* pNext){ m_queryPoolCreateInfo.pNext = pNext; return *this; }
    const QueryPoolCreateFlags& flags() const { return reinterpret_cast<const QueryPoolCreateFlags&>( m_queryPoolCreateInfo.flags ); }
    QueryPoolCreateInfo& flags(QueryPoolCreateFlags flags){ m_queryPoolCreateInfo.flags = static_cast<VkQueryPoolCreateFlags>( flags ); return *this; }
    const QueryType& queryType() const { return reinterpret_cast<const QueryType&>( m_queryPoolCreateInfo.queryType ); }
    QueryPoolCreateInfo& queryType(QueryType queryType){ m_queryPoolCreateInfo.queryType = static_cast<VkQueryType>( queryType ); return *this; }
    const uint32_t& queryCount() const { return m_queryPoolCreateInfo.queryCount; }
    QueryPoolCreateInfo& queryCount(uint32_t queryCount){ m_queryPoolCreateInfo.queryCount = queryCount; return *this; }
    const QueryPipelineStatisticFlags& pipelineStatistics() const { return reinterpret_cast<const QueryPipelineStatisticFlags&>( m_queryPoolCreateInfo.pipelineStatistics ); }
    QueryPoolCreateInfo& pipelineStatistics(QueryPipelineStatisticFlags pipelineStatistics){ m_queryPoolCreateInfo.pipelineStatistics = static_cast<VkQueryPipelineStatisticFlags>( pipelineStatistics ); return *this; }
    operator VkQueryPoolCreateInfo&() { return m_queryPoolCreateInfo; }
    operator const VkQueryPoolCreateInfo&() const { return m_queryPoolCreateInfo; }
    VkQueryPoolCreateInfo& vkHandle() { return m_queryPoolCreateInfo; }
    const VkQueryPoolCreateInfo& vkHandle() const { return m_queryPoolCreateInfo; }

  private:
    VkQueryPoolCreateInfo m_queryPoolCreateInfo;
  };

  class ImageSubresource
  {
  public:
    ImageSubresource() : ImageSubresource(ImageAspectFlags(), 0, 0) {}
    ImageSubresource(ImageAspectFlags aspectMask, uint32_t mipLevel, uint32_t arrayLayer) {m_imageSubresource.aspectMask = static_cast<VkImageAspectFlags>( aspectMask ); m_imageSubresource.mipLevel = mipLevel; m_imageSubresource.arrayLayer = arrayLayer; }
    ImageSubresource(const VkImageSubresource& other) : m_imageSubresource{other} {}
    ImageSubresource& operator=(const VkImageSubresource& other){ m_imageSubresource = other; return *this; }

    const ImageAspectFlags& aspectMask() const { return reinterpret_cast<const ImageAspectFlags&>( m_imageSubresource.aspectMask ); }
    ImageSubresource& aspectMask(ImageAspectFlags aspectMask){ m_imageSubresource.aspectMask = static_cast<VkImageAspectFlags>( aspectMask ); return *this; }
    const uint32_t& mipLevel() const { return m_imageSubresource.mipLevel; }
    ImageSubresource& mipLevel(uint32_t mipLevel){ m_imageSubresource.mipLevel = mipLevel; return *this; }
    const uint32_t& arrayLayer() const { return m_imageSubresource.arrayLayer; }
    ImageSubresource& arrayLayer(uint32_t arrayLayer){ m_imageSubresource.arrayLayer = arrayLayer; return *this; }
    operator VkImageSubresource&() { return m_imageSubresource; }
    operator const VkImageSubresource&() const { return m_imageSubresource; }
    VkImageSubresource& vkHandle() { return m_imageSubresource; }
    const VkImageSubresource& vkHandle() const { return m_imageSubresource; }

  private:
    VkImageSubresource m_imageSubresource;
  };

  class ImageSubresourceLayers
  {
  public:
    ImageSubresourceLayers() : ImageSubresourceLayers(ImageAspectFlags(), 0, 0, 0) {}
    ImageSubresourceLayers(ImageAspectFlags aspectMask, uint32_t mipLevel, uint32_t baseArrayLayer, uint32_t layerCount) {m_imageSubresourceLayers.aspectMask = static_cast<VkImageAspectFlags>( aspectMask ); m_imageSubresourceLayers.mipLevel = mipLevel; m_imageSubresourceLayers.baseArrayLayer = baseArrayLayer; m_imageSubresourceLayers.layerCount = layerCount; }
    ImageSubresourceLayers(const VkImageSubresourceLayers& other) : m_imageSubresourceLayers{other} {}
    ImageSubresourceLayers& operator=(const VkImageSubresourceLayers& other){ m_imageSubresourceLayers = other; return *this; }

    const ImageAspectFlags& aspectMask() const { return reinterpret_cast<const ImageAspectFlags&>( m_imageSubresourceLayers.aspectMask ); }
    ImageSubresourceLayers& aspectMask(ImageAspectFlags aspectMask){ m_imageSubresourceLayers.aspectMask = static_cast<VkImageAspectFlags>( aspectMask ); return *this; }
    const uint32_t& mipLevel() const { return m_imageSubresourceLayers.mipLevel; }
    ImageSubresourceLayers& mipLevel(uint32_t mipLevel){ m_imageSubresourceLayers.mipLevel = mipLevel; return *this; }
    const uint32_t& baseArrayLayer() const { return m_imageSubresourceLayers.baseArrayLayer; }
    ImageSubresourceLayers& baseArrayLayer(uint32_t baseArrayLayer){ m_imageSubresourceLayers.baseArrayLayer = baseArrayLayer; return *this; }
    const uint32_t& layerCount() const { return m_imageSubresourceLayers.layerCount; }
    ImageSubresourceLayers& layerCount(uint32_t layerCount){ m_imageSubresourceLayers.layerCount = layerCount; return *this; }
    operator VkImageSubresourceLayers&() { return m_imageSubresourceLayers; }
    operator const VkImageSubresourceLayers&() const { return m_imageSubresourceLayers; }
    VkImageSubresourceLayers& vkHandle() { return m_imageSubresourceLayers; }
    const VkImageSubresourceLayers& vkHandle() const { return m_imageSubresourceLayers; }

  private:
    VkImageSubresourceLayers m_imageSubresourceLayers;
  };

  class ImageSubresourceRange
  {
  public:
    ImageSubresourceRange() : ImageSubresourceRange(ImageAspectFlags(), 0, 0, 0, 0) {}
    ImageSubresourceRange(ImageAspectFlags aspectMask, uint32_t baseMipLevel, uint32_t levelCount, uint32_t baseArrayLayer, uint32_t layerCount) {m_imageSubresourceRange.aspectMask = static_cast<VkImageAspectFlags>( aspectMask ); m_imageSubresourceRange.baseMipLevel = baseMipLevel; m_imageSubresourceRange.levelCount = levelCount; m_imageSubresourceRange.baseArrayLayer = baseArrayLayer; m_imageSubresourceRange.layerCount = layerCount; }
    ImageSubresourceRange(const VkImageSubresourceRange& other) : m_imageSubresourceRange{other} {}
    ImageSubresourceRange& operator=(const VkImageSubresourceRange& other){ m_imageSubresourceRange = other; return *this; }

    const ImageAspectFlags& aspectMask() const { return reinterpret_cast<const ImageAspectFlags&>( m_imageSubresourceRange.aspectMask ); }
    ImageSubresourceRange& aspectMask(ImageAspectFlags aspectMask){ m_imageSubresourceRange.aspectMask = static_cast<VkImageAspectFlags>( aspectMask ); return *this; }
    const uint32_t& baseMipLevel() const { return m_imageSubresourceRange.baseMipLevel; }
    ImageSubresourceRange& baseMipLevel(uint32_t baseMipLevel){ m_imageSubresourceRange.baseMipLevel = baseMipLevel; return *this; }
    const uint32_t& levelCount() const { return m_imageSubresourceRange.levelCount; }
    ImageSubresourceRange& levelCount(uint32_t levelCount){ m_imageSubresourceRange.levelCount = levelCount; return *this; }
    const uint32_t& baseArrayLayer() const { return m_imageSubresourceRange.baseArrayLayer; }
    ImageSubresourceRange& baseArrayLayer(uint32_t baseArrayLayer){ m_imageSubresourceRange.baseArrayLayer = baseArrayLayer; return *this; }
    const uint32_t& layerCount() const { return m_imageSubresourceRange.layerCount; }
    ImageSubresourceRange& layerCount(uint32_t layerCount){ m_imageSubresourceRange.layerCount = layerCount; return *this; }
    operator VkImageSubresourceRange&() { return m_imageSubresourceRange; }
    operator const VkImageSubresourceRange&() const { return m_imageSubresourceRange; }
    VkImageSubresourceRange& vkHandle() { return m_imageSubresourceRange; }
    const VkImageSubresourceRange& vkHandle() const { return m_imageSubresourceRange; }

  private:
    VkImageSubresourceRange m_imageSubresourceRange;
  };

  class ImageMemoryBarrier
  {
  public:
    ImageMemoryBarrier() : ImageMemoryBarrier(AccessFlags(), AccessFlags(), ImageLayout::Undefined, ImageLayout::Undefined, 0, 0, Image(), ImageSubresourceRange()) {}
    ImageMemoryBarrier(AccessFlags srcAccessMask, AccessFlags dstAccessMask, ImageLayout oldLayout, ImageLayout newLayout, uint32_t srcQueueFamilyIndex, uint32_t dstQueueFamilyIndex, Image image, ImageSubresourceRange subresourceRange) {m_imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER; m_imageMemoryBarrier.pNext = nullptr; m_imageMemoryBarrier.srcAccessMask = static_cast<VkAccessFlags>( srcAccessMask ); m_imageMemoryBarrier.dstAccessMask = static_cast<VkAccessFlags>( dstAccessMask ); m_imageMemoryBarrier.oldLayout = static_cast<VkImageLayout>( oldLayout ); m_imageMemoryBarrier.newLayout = static_cast<VkImageLayout>( newLayout ); m_imageMemoryBarrier.srcQueueFamilyIndex = srcQueueFamilyIndex; m_imageMemoryBarrier.dstQueueFamilyIndex = dstQueueFamilyIndex; m_imageMemoryBarrier.image = image; m_imageMemoryBarrier.subresourceRange = static_cast<VkImageSubresourceRange>( subresourceRange ); }
    ImageMemoryBarrier(const VkImageMemoryBarrier& other) : m_imageMemoryBarrier{other} {}
    ImageMemoryBarrier& operator=(const VkImageMemoryBarrier& other){ m_imageMemoryBarrier = other; return *this; }

    const StructureType& sType() const { return reinterpret_cast<const StructureType&>( m_imageMemoryBarrier.sType ); }
    ImageMemoryBarrier& sType(StructureType sType){ m_imageMemoryBarrier.sType = static_cast<VkStructureType>( sType ); return *this; }
    const void* pNext() const { return reinterpret_cast<const void*>( m_imageMemoryBarrier.pNext ); }
    ImageMemoryBarrier& pNext(const void* pNext){ m_imageMemoryBarrier.pNext = pNext; return *this; }
    const AccessFlags& srcAccessMask() const { return reinterpret_cast<const AccessFlags&>( m_imageMemoryBarrier.srcAccessMask ); }
    ImageMemoryBarrier& srcAccessMask(AccessFlags srcAccessMask){ m_imageMemoryBarrier.srcAccessMask = static_cast<VkAccessFlags>( srcAccessMask ); return *this; }
    const AccessFlags& dstAccessMask() const { return reinterpret_cast<const AccessFlags&>( m_imageMemoryBarrier.dstAccessMask ); }
    ImageMemoryBarrier& dstAccessMask(AccessFlags dstAccessMask){ m_imageMemoryBarrier.dstAccessMask = static_cast<VkAccessFlags>( dstAccessMask ); return *this; }
    const ImageLayout& oldLayout() const { return reinterpret_cast<const ImageLayout&>( m_imageMemoryBarrier.oldLayout ); }
    ImageMemoryBarrier& oldLayout(ImageLayout oldLayout){ m_imageMemoryBarrier.oldLayout = static_cast<VkImageLayout>( oldLayout ); return *this; }
    const ImageLayout& newLayout() const { return reinterpret_cast<const ImageLayout&>( m_imageMemoryBarrier.newLayout ); }
    ImageMemoryBarrier& newLayout(ImageLayout newLayout){ m_imageMemoryBarrier.newLayout = static_cast<VkImageLayout>( newLayout ); return *this; }
    const uint32_t& srcQueueFamilyIndex() const { return m_imageMemoryBarrier.srcQueueFamilyIndex; }
    ImageMemoryBarrier& srcQueueFamilyIndex(uint32_t srcQueueFamilyIndex){ m_imageMemoryBarrier.srcQueueFamilyIndex = srcQueueFamilyIndex; return *this; }
    const uint32_t& dstQueueFamilyIndex() const { return m_imageMemoryBarrier.dstQueueFamilyIndex; }
    ImageMemoryBarrier& dstQueueFamilyIndex(uint32_t dstQueueFamilyIndex){ m_imageMemoryBarrier.dstQueueFamilyIndex = dstQueueFamilyIndex; return *this; }
    const Image& image() const { return m_imageMemoryBarrier.image; }
    ImageMemoryBarrier& image(Image image){ m_imageMemoryBarrier.image = image; return *this; }
    const ImageSubresourceRange& subresourceRange() const { return reinterpret_cast<const ImageSubresourceRange&>( m_imageMemoryBarrier.subresourceRange ); }
    ImageMemoryBarrier& subresourceRange(ImageSubresourceRange subresourceRange){ m_imageMemoryBarrier.subresourceRange = static_cast<VkImageSubresourceRange>( subresourceRange ); return *this; }
    operator VkImageMemoryBarrier&() { return m_imageMemoryBarrier; }
    operator const VkImageMemoryBarrier&() const { return m_imageMemoryBarrier; }
    VkImageMemoryBarrier& vkHandle() { return m_imageMemoryBarrier; }
    const VkImageMemoryBarrier& vkHandle() const { return m_imageMemoryBarrier; }

  private:
    VkImageMemoryBarrier m_imageMemoryBarrier;
  };

  class ImageViewCreateInfo
  {
  public:
    ImageViewCreateInfo() : ImageViewCreateInfo(ImageViewCreateFlags(), Image(), ImageViewType::e1D, Format::Undefined, ComponentMapping(), ImageSubresourceRange()) {}
    ImageViewCreateInfo(ImageViewCreateFlags flags, Image image, ImageViewType viewType, Format format, ComponentMapping components, ImageSubresourceRange subresourceRange) {m_imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO; m_imageViewCreateInfo.pNext = nullptr; m_imageViewCreateInfo.flags = static_cast<VkImageViewCreateFlags>( flags ); m_imageViewCreateInfo.image = image; m_imageViewCreateInfo.viewType = static_cast<VkImageViewType>( viewType ); m_imageViewCreateInfo.format = static_cast<VkFormat>( format ); m_imageViewCreateInfo.components = static_cast<VkComponentMapping>( components ); m_imageViewCreateInfo.subresourceRange = static_cast<VkImageSubresourceRange>( subresourceRange ); }
    ImageViewCreateInfo(const VkImageViewCreateInfo& other) : m_imageViewCreateInfo{other} {}
    ImageViewCreateInfo& operator=(const VkImageViewCreateInfo& other){ m_imageViewCreateInfo = other; return *this; }

    const StructureType& sType() const { return reinterpret_cast<const StructureType&>( m_imageViewCreateInfo.sType ); }
    ImageViewCreateInfo& sType(StructureType sType){ m_imageViewCreateInfo.sType = static_cast<VkStructureType>( sType ); return *this; }
    const void* pNext() const { return reinterpret_cast<const void*>( m_imageViewCreateInfo.pNext ); }
    ImageViewCreateInfo& pNext(const void* pNext){ m_imageViewCreateInfo.pNext = pNext; return *this; }
    const ImageViewCreateFlags& flags() const { return reinterpret_cast<const ImageViewCreateFlags&>( m_imageViewCreateInfo.flags ); }
    ImageViewCreateInfo& flags(ImageViewCreateFlags flags){ m_imageViewCreateInfo.flags = static_cast<VkImageViewCreateFlags>( flags ); return *this; }
    const Image& image() const { return m_imageViewCreateInfo.image; }
    ImageViewCreateInfo& image(Image image){ m_imageViewCreateInfo.image = image; return *this; }
    const ImageViewType& viewType() const { return reinterpret_cast<const ImageViewType&>( m_imageViewCreateInfo.viewType ); }
    ImageViewCreateInfo& viewType(ImageViewType viewType){ m_imageViewCreateInfo.viewType = static_cast<VkImageViewType>( viewType ); return *this; }
    const Format& format() const { return reinterpret_cast<const Format&>( m_imageViewCreateInfo.format ); }
    ImageViewCreateInfo& format(Format format){ m_imageViewCreateInfo.format = static_cast<VkFormat>( format ); return *this; }
    const ComponentMapping& components() const { return reinterpret_cast<const ComponentMapping&>( m_imageViewCreateInfo.components ); }
    ImageViewCreateInfo& components(ComponentMapping components){ m_imageViewCreateInfo.components = static_cast<VkComponentMapping>( components ); return *this; }
    const ImageSubresourceRange& subresourceRange() const { return reinterpret_cast<const ImageSubresourceRange&>( m_imageViewCreateInfo.subresourceRange ); }
    ImageViewCreateInfo& subresourceRange(ImageSubresourceRange subresourceRange){ m_imageViewCreateInfo.subresourceRange = static_cast<VkImageSubresourceRange>( subresourceRange ); return *this; }
    operator VkImageViewCreateInfo&() { return m_imageViewCreateInfo; }
    operator const VkImageViewCreateInfo&() const { return m_imageViewCreateInfo; }
    VkImageViewCreateInfo& vkHandle() { return m_imageViewCreateInfo; }
    const VkImageViewCreateInfo& vkHandle() const { return m_imageViewCreateInfo; }

  private:
    VkImageViewCreateInfo m_imageViewCreateInfo;
  };

  class ImageCopy
  {
  public:
    ImageCopy() : ImageCopy(ImageSubresourceLayers(), Offset3D(), ImageSubresourceLayers(), Offset3D(), Extent3D()) {}
    ImageCopy(ImageSubresourceLayers srcSubresource, Offset3D srcOffset, ImageSubresourceLayers dstSubresource, Offset3D dstOffset, Extent3D extent) {m_imageCopy.srcSubresource = static_cast<VkImageSubresourceLayers>( srcSubresource ); m_imageCopy.srcOffset = static_cast<VkOffset3D>( srcOffset ); m_imageCopy.dstSubresource = static_cast<VkImageSubresourceLayers>( dstSubresource ); m_imageCopy.dstOffset = static_cast<VkOffset3D>( dstOffset ); m_imageCopy.extent = static_cast<VkExtent3D>( extent ); }
    ImageCopy(const VkImageCopy& other) : m_imageCopy{other} {}
    ImageCopy& operator=(const VkImageCopy& other){ m_imageCopy = other; return *this; }

    const ImageSubresourceLayers& srcSubresource() const { return reinterpret_cast<const ImageSubresourceLayers&>( m_imageCopy.srcSubresource ); }
    ImageCopy& srcSubresource(ImageSubresourceLayers srcSubresource){ m_imageCopy.srcSubresource = static_cast<VkImageSubresourceLayers>( srcSubresource ); return *this; }
    const Offset3D& srcOffset() const { return reinterpret_cast<const Offset3D&>( m_imageCopy.srcOffset ); }
    ImageCopy& srcOffset(Offset3D srcOffset){ m_imageCopy.srcOffset = static_cast<VkOffset3D>( srcOffset ); return *this; }
    const ImageSubresourceLayers& dstSubresource() const { return reinterpret_cast<const ImageSubresourceLayers&>( m_imageCopy.dstSubresource ); }
    ImageCopy& dstSubresource(ImageSubresourceLayers dstSubresource){ m_imageCopy.dstSubresource = static_cast<VkImageSubresourceLayers>( dstSubresource ); return *this; }
    const Offset3D& dstOffset() const { return reinterpret_cast<const Offset3D&>( m_imageCopy.dstOffset ); }
    ImageCopy& dstOffset(Offset3D dstOffset){ m_imageCopy.dstOffset = static_cast<VkOffset3D>( dstOffset ); return *this; }
    const Extent3D& extent() const { return reinterpret_cast<const Extent3D&>( m_imageCopy.extent ); }
    ImageCopy& extent(Extent3D extent){ m_imageCopy.extent = static_cast<VkExtent3D>( extent ); return *this; }
    operator VkImageCopy&() { return m_imageCopy; }
    operator const VkImageCopy&() const { return m_imageCopy; }
    VkImageCopy& vkHandle() { return m_imageCopy; }
    const VkImageCopy& vkHandle() const { return m_imageCopy; }

  private:
    VkImageCopy m_imageCopy;
  };

  class ImageBlit
  {
  public:
    ImageBlit() : ImageBlit(ImageSubresourceLayers(), { Offset3D() }, ImageSubresourceLayers(), { Offset3D() }) {}
    ImageBlit(ImageSubresourceLayers srcSubresource, std::array<Offset3D,2> const& srcOffsets, ImageSubresourceLayers dstSubresource, std::array<Offset3D,2> const& dstOffsets) {m_imageBlit.srcSubresource = static_cast<VkImageSubresourceLayers>( srcSubresource ); memcpy(&m_imageBlit.srcOffsets, srcOffsets.data(), 2 * sizeof(Offset3D)); m_imageBlit.dstSubresource = static_cast<VkImageSubresourceLayers>( dstSubresource ); memcpy(&m_imageBlit.dstOffsets, dstOffsets.data(), 2 * sizeof(Offset3D)); }
    ImageBlit(const VkImageBlit& other) : m_imageBlit{other} {}
    ImageBlit& operator=(const VkImageBlit& other){ m_imageBlit = other; return *this; }

    const ImageSubresourceLayers& srcSubresource() const { return reinterpret_cast<const ImageSubresourceLayers&>( m_imageBlit.srcSubresource ); }
    ImageBlit& srcSubresource(ImageSubresourceLayers srcSubresource){ m_imageBlit.srcSubresource = static_cast<VkImageSubresourceLayers>( srcSubresource ); return *this; }
    const Offset3D* srcOffsets() const { return reinterpret_cast<const Offset3D*>( m_imageBlit.srcOffsets ); }
    ImageBlit& srcOffsets(std::array<Offset3D,2> srcOffsets){ memcpy(&m_imageBlit.srcOffsets, srcOffsets.data(), 2 * sizeof(Offset3D)); return *this; }
    const ImageSubresourceLayers& dstSubresource() const { return reinterpret_cast<const ImageSubresourceLayers&>( m_imageBlit.dstSubresource ); }
    ImageBlit& dstSubresource(ImageSubresourceLayers dstSubresource){ m_imageBlit.dstSubresource = static_cast<VkImageSubresourceLayers>( dstSubresource ); return *this; }
    const Offset3D* dstOffsets() const { return reinterpret_cast<const Offset3D*>( m_imageBlit.dstOffsets ); }
    ImageBlit& dstOffsets(std::array<Offset3D,2> dstOffsets){ memcpy(&m_imageBlit.dstOffsets, dstOffsets.data(), 2 * sizeof(Offset3D)); return *this; }
    operator VkImageBlit&() { return m_imageBlit; }
    operator const VkImageBlit&() const { return m_imageBlit; }
    VkImageBlit& vkHandle() { return m_imageBlit; }
    const VkImageBlit& vkHandle() const { return m_imageBlit; }

  private:
    VkImageBlit m_imageBlit;
  };

  class BufferImageCopy
  {
  public:
    BufferImageCopy() : BufferImageCopy(0, 0, 0, ImageSubresourceLayers(), Offset3D(), Extent3D()) {}
    BufferImageCopy(DeviceSize bufferOffset, uint32_t bufferRowLength, uint32_t bufferImageHeight, ImageSubresourceLayers imageSubresource, Offset3D imageOffset, Extent3D imageExtent) {m_bufferImageCopy.bufferOffset = bufferOffset; m_bufferImageCopy.bufferRowLength = bufferRowLength; m_bufferImageCopy.bufferImageHeight = bufferImageHeight; m_bufferImageCopy.imageSubresource = static_cast<VkImageSubresourceLayers>( imageSubresource ); m_bufferImageCopy.imageOffset = static_cast<VkOffset3D>( imageOffset ); m_bufferImageCopy.imageExtent = static_cast<VkExtent3D>( imageExtent ); }
    BufferImageCopy(const VkBufferImageCopy& other) : m_bufferImageCopy{other} {}
    BufferImageCopy& operator=(const VkBufferImageCopy& other){ m_bufferImageCopy = other; return *this; }

    const DeviceSize& bufferOffset() const { return m_bufferImageCopy.bufferOffset; }
    BufferImageCopy& bufferOffset(DeviceSize bufferOffset){ m_bufferImageCopy.bufferOffset = bufferOffset; return *this; }
    const uint32_t& bufferRowLength() const { return m_bufferImageCopy.bufferRowLength; }
    BufferImageCopy& bufferRowLength(uint32_t bufferRowLength){ m_bufferImageCopy.bufferRowLength = bufferRowLength; return *this; }
    const uint32_t& bufferImageHeight() const { return m_bufferImageCopy.bufferImageHeight; }
    BufferImageCopy& bufferImageHeight(uint32_t bufferImageHeight){ m_bufferImageCopy.bufferImageHeight = bufferImageHeight; return *this; }
    const ImageSubresourceLayers& imageSubresource() const { return reinterpret_cast<const ImageSubresourceLayers&>( m_bufferImageCopy.imageSubresource ); }
    BufferImageCopy& imageSubresource(ImageSubresourceLayers imageSubresource){ m_bufferImageCopy.imageSubresource = static_cast<VkImageSubresourceLayers>( imageSubresource ); return *this; }
    const Offset3D& imageOffset() const { return reinterpret_cast<const Offset3D&>( m_bufferImageCopy.imageOffset ); }
    BufferImageCopy& imageOffset(Offset3D imageOffset){ m_bufferImageCopy.imageOffset = static_cast<VkOffset3D>( imageOffset ); return *this; }
    const Extent3D& imageExtent() const { return reinterpret_cast<const Extent3D&>( m_bufferImageCopy.imageExtent ); }
    BufferImageCopy& imageExtent(Extent3D imageExtent){ m_bufferImageCopy.imageExtent = static_cast<VkExtent3D>( imageExtent ); return *this; }
    operator VkBufferImageCopy&() { return m_bufferImageCopy; }
    operator const VkBufferImageCopy&() const { return m_bufferImageCopy; }
    VkBufferImageCopy& vkHandle() { return m_bufferImageCopy; }
    const VkBufferImageCopy& vkHandle() const { return m_bufferImageCopy; }

  private:
    VkBufferImageCopy m_bufferImageCopy;
  };

  class ImageResolve
  {
  public:
    ImageResolve() : ImageResolve(ImageSubresourceLayers(), Offset3D(), ImageSubresourceLayers(), Offset3D(), Extent3D()) {}
    ImageResolve(ImageSubresourceLayers srcSubresource, Offset3D srcOffset, ImageSubresourceLayers dstSubresource, Offset3D dstOffset, Extent3D extent) {m_imageResolve.srcSubresource = static_cast<VkImageSubresourceLayers>( srcSubresource ); m_imageResolve.srcOffset = static_cast<VkOffset3D>( srcOffset ); m_imageResolve.dstSubresource = static_cast<VkImageSubresourceLayers>( dstSubresource ); m_imageResolve.dstOffset = static_cast<VkOffset3D>( dstOffset ); m_imageResolve.extent = static_cast<VkExtent3D>( extent ); }
    ImageResolve(const VkImageResolve& other) : m_imageResolve{other} {}
    ImageResolve& operator=(const VkImageResolve& other){ m_imageResolve = other; return *this; }

    const ImageSubresourceLayers& srcSubresource() const { return reinterpret_cast<const ImageSubresourceLayers&>( m_imageResolve.srcSubresource ); }
    ImageResolve& srcSubresource(ImageSubresourceLayers srcSubresource){ m_imageResolve.srcSubresource = static_cast<VkImageSubresourceLayers>( srcSubresource ); return *this; }
    const Offset3D& srcOffset() const { return reinterpret_cast<const Offset3D&>( m_imageResolve.srcOffset ); }
    ImageResolve& srcOffset(Offset3D srcOffset){ m_imageResolve.srcOffset = static_cast<VkOffset3D>( srcOffset ); return *this; }
    const ImageSubresourceLayers& dstSubresource() const { return reinterpret_cast<const ImageSubresourceLayers&>( m_imageResolve.dstSubresource ); }
    ImageResolve& dstSubresource(ImageSubresourceLayers dstSubresource){ m_imageResolve.dstSubresource = static_cast<VkImageSubresourceLayers>( dstSubresource ); return *this; }
    const Offset3D& dstOffset() const { return reinterpret_cast<const Offset3D&>( m_imageResolve.dstOffset ); }
    ImageResolve& dstOffset(Offset3D dstOffset){ m_imageResolve.dstOffset = static_cast<VkOffset3D>( dstOffset ); return *this; }
    const Extent3D& extent() const { return reinterpret_cast<const Extent3D&>( m_imageResolve.extent ); }
    ImageResolve& extent(Extent3D extent){ m_imageResolve.extent = static_cast<VkExtent3D>( extent ); return *this; }
    operator VkImageResolve&() { return m_imageResolve; }
    operator const VkImageResolve&() const { return m_imageResolve; }
    VkImageResolve& vkHandle() { return m_imageResolve; }
    const VkImageResolve& vkHandle() const { return m_imageResolve; }

  private:
    VkImageResolve m_imageResolve;
  };

  class ClearAttachment
  {
  public:
    ClearAttachment() : ClearAttachment(ImageAspectFlags(), 0, ClearValue()) {}
    ClearAttachment(ImageAspectFlags aspectMask, uint32_t colorAttachment, ClearValue clearValue) {m_clearAttachment.aspectMask = static_cast<VkImageAspectFlags>( aspectMask ); m_clearAttachment.colorAttachment = colorAttachment; m_clearAttachment.clearValue = static_cast<VkClearValue>( clearValue ); }
    ClearAttachment(const VkClearAttachment& other) : m_clearAttachment{other} {}
    ClearAttachment& operator=(const VkClearAttachment& other){ m_clearAttachment = other; return *this; }

    const ImageAspectFlags& aspectMask() const { return reinterpret_cast<const ImageAspectFlags&>( m_clearAttachment.aspectMask ); }
    ClearAttachment& aspectMask(ImageAspectFlags aspectMask){ m_clearAttachment.aspectMask = static_cast<VkImageAspectFlags>( aspectMask ); return *this; }
    const uint32_t& colorAttachment() const { return m_clearAttachment.colorAttachment; }
    ClearAttachment& colorAttachment(uint32_t colorAttachment){ m_clearAttachment.colorAttachment = colorAttachment; return *this; }
    const ClearValue& clearValue() const { return reinterpret_cast<const ClearValue&>( m_clearAttachment.clearValue ); }
    ClearAttachment& clearValue(ClearValue clearValue){ m_clearAttachment.clearValue = static_cast<VkClearValue>( clearValue ); return *this; }
    operator VkClearAttachment&() { return m_clearAttachment; }
    operator const VkClearAttachment&() const { return m_clearAttachment; }
    VkClearAttachment& vkHandle() { return m_clearAttachment; }
    const VkClearAttachment& vkHandle() const { return m_clearAttachment; }

  private:
    VkClearAttachment m_clearAttachment;
  };

  class SparseImageFormatProperties
  {
  public:
    const ImageAspectFlags& aspectMask() const { return reinterpret_cast<const ImageAspectFlags&>( m_sparseImageFormatProperties.aspectMask ); }
    const Extent3D& imageGranularity() const { return reinterpret_cast<const Extent3D&>( m_sparseImageFormatProperties.imageGranularity ); }
    const SparseImageFormatFlags& flags() const { return reinterpret_cast<const SparseImageFormatFlags&>( m_sparseImageFormatProperties.flags ); }
    operator VkSparseImageFormatProperties&() { return m_sparseImageFormatProperties; }
    operator const VkSparseImageFormatProperties&() const { return m_sparseImageFormatProperties; }
    VkSparseImageFormatProperties& vkHandle() { return m_sparseImageFormatProperties; }
    const VkSparseImageFormatProperties& vkHandle() const { return m_sparseImageFormatProperties; }

  private:
    VkSparseImageFormatProperties m_sparseImageFormatProperties;
  };

  class SparseImageMemoryRequirements
  {
  public:
    const SparseImageFormatProperties& formatProperties() const { return reinterpret_cast<const SparseImageFormatProperties&>( m_sparseImageMemoryRequirements.formatProperties ); }
    const uint32_t& imageMipTailFirstLod() const { return m_sparseImageMemoryRequirements.imageMipTailFirstLod; }
    const DeviceSize& imageMipTailSize() const { return m_sparseImageMemoryRequirements.imageMipTailSize; }
    const DeviceSize& imageMipTailOffset() const { return m_sparseImageMemoryRequirements.imageMipTailOffset; }
    const DeviceSize& imageMipTailStride() const { return m_sparseImageMemoryRequirements.imageMipTailStride; }
    operator VkSparseImageMemoryRequirements&() { return m_sparseImageMemoryRequirements; }
    operator const VkSparseImageMemoryRequirements&() const { return m_sparseImageMemoryRequirements; }
    VkSparseImageMemoryRequirements& vkHandle() { return m_sparseImageMemoryRequirements; }
    const VkSparseImageMemoryRequirements& vkHandle() const { return m_sparseImageMemoryRequirements; }

  private:
    VkSparseImageMemoryRequirements m_sparseImageMemoryRequirements;
  };

  class SparseMemoryBind
  {
  public:
    SparseMemoryBind() : SparseMemoryBind(0, 0, DeviceMemory(), 0, SparseMemoryBindFlags()) {}
    SparseMemoryBind(DeviceSize resourceOffset, DeviceSize size, DeviceMemory memory, DeviceSize memoryOffset, SparseMemoryBindFlags flags) {m_sparseMemoryBind.resourceOffset = resourceOffset; m_sparseMemoryBind.size = size; m_sparseMemoryBind.memory = memory; m_sparseMemoryBind.memoryOffset = memoryOffset; m_sparseMemoryBind.flags = static_cast<VkSparseMemoryBindFlags>( flags ); }
    SparseMemoryBind(const VkSparseMemoryBind& other) : m_sparseMemoryBind{other} {}
    SparseMemoryBind& operator=(const VkSparseMemoryBind& other){ m_sparseMemoryBind = other; return *this; }

    const DeviceSize& resourceOffset() const { return m_sparseMemoryBind.resourceOffset; }
    SparseMemoryBind& resourceOffset(DeviceSize resourceOffset){ m_sparseMemoryBind.resourceOffset = resourceOffset; return *this; }
    const DeviceSize& size() const { return m_sparseMemoryBind.size; }
    SparseMemoryBind& size(DeviceSize size){ m_sparseMemoryBind.size = size; return *this; }
    const DeviceMemory& memory() const { return m_sparseMemoryBind.memory; }
    SparseMemoryBind& memory(DeviceMemory memory){ m_sparseMemoryBind.memory = memory; return *this; }
    const DeviceSize& memoryOffset() const { return m_sparseMemoryBind.memoryOffset; }
    SparseMemoryBind& memoryOffset(DeviceSize memoryOffset){ m_sparseMemoryBind.memoryOffset = memoryOffset; return *this; }
    const SparseMemoryBindFlags& flags() const { return reinterpret_cast<const SparseMemoryBindFlags&>( m_sparseMemoryBind.flags ); }
    SparseMemoryBind& flags(SparseMemoryBindFlags flags){ m_sparseMemoryBind.flags = static_cast<VkSparseMemoryBindFlags>( flags ); return *this; }
    operator VkSparseMemoryBind&() { return m_sparseMemoryBind; }
    operator const VkSparseMemoryBind&() const { return m_sparseMemoryBind; }
    VkSparseMemoryBind& vkHandle() { return m_sparseMemoryBind; }
    const VkSparseMemoryBind& vkHandle() const { return m_sparseMemoryBind; }

  private:
    VkSparseMemoryBind m_sparseMemoryBind;
  };

  class SparseImageMemoryBind
  {
  public:
    SparseImageMemoryBind() : SparseImageMemoryBind(ImageSubresource(), Offset3D(), Extent3D(), DeviceMemory(), 0, SparseMemoryBindFlags()) {}
    SparseImageMemoryBind(ImageSubresource subresource, Offset3D offset, Extent3D extent, DeviceMemory memory, DeviceSize memoryOffset, SparseMemoryBindFlags flags) {m_sparseImageMemoryBind.subresource = static_cast<VkImageSubresource>( subresource ); m_sparseImageMemoryBind.offset = static_cast<VkOffset3D>( offset ); m_sparseImageMemoryBind.extent = static_cast<VkExtent3D>( extent ); m_sparseImageMemoryBind.memory = memory; m_sparseImageMemoryBind.memoryOffset = memoryOffset; m_sparseImageMemoryBind.flags = static_cast<VkSparseMemoryBindFlags>( flags ); }
    SparseImageMemoryBind(const VkSparseImageMemoryBind& other) : m_sparseImageMemoryBind{other} {}
    SparseImageMemoryBind& operator=(const VkSparseImageMemoryBind& other){ m_sparseImageMemoryBind = other; return *this; }

    const ImageSubresource& subresource() const { return reinterpret_cast<const ImageSubresource&>( m_sparseImageMemoryBind.subresource ); }
    SparseImageMemoryBind& subresource(ImageSubresource subresource){ m_sparseImageMemoryBind.subresource = static_cast<VkImageSubresource>( subresource ); return *this; }
    const Offset3D& offset() const { return reinterpret_cast<const Offset3D&>( m_sparseImageMemoryBind.offset ); }
    SparseImageMemoryBind& offset(Offset3D offset){ m_sparseImageMemoryBind.offset = static_cast<VkOffset3D>( offset ); return *this; }
    const Extent3D& extent() const { return reinterpret_cast<const Extent3D&>( m_sparseImageMemoryBind.extent ); }
    SparseImageMemoryBind& extent(Extent3D extent){ m_sparseImageMemoryBind.extent = static_cast<VkExtent3D>( extent ); return *this; }
    const DeviceMemory& memory() const { return m_sparseImageMemoryBind.memory; }
    SparseImageMemoryBind& memory(DeviceMemory memory){ m_sparseImageMemoryBind.memory = memory; return *this; }
    const DeviceSize& memoryOffset() const { return m_sparseImageMemoryBind.memoryOffset; }
    SparseImageMemoryBind& memoryOffset(DeviceSize memoryOffset){ m_sparseImageMemoryBind.memoryOffset = memoryOffset; return *this; }
    const SparseMemoryBindFlags& flags() const { return reinterpret_cast<const SparseMemoryBindFlags&>( m_sparseImageMemoryBind.flags ); }
    SparseImageMemoryBind& flags(SparseMemoryBindFlags flags){ m_sparseImageMemoryBind.flags = static_cast<VkSparseMemoryBindFlags>( flags ); return *this; }
    operator VkSparseImageMemoryBind&() { return m_sparseImageMemoryBind; }
    operator const VkSparseImageMemoryBind&() const { return m_sparseImageMemoryBind; }
    VkSparseImageMemoryBind& vkHandle() { return m_sparseImageMemoryBind; }
    const VkSparseImageMemoryBind& vkHandle() const { return m_sparseImageMemoryBind; }

  private:
    VkSparseImageMemoryBind m_sparseImageMemoryBind;
  };

  class SparseBufferMemoryBindInfo
  {
  public:
    SparseBufferMemoryBindInfo() : SparseBufferMemoryBindInfo(Buffer(), 0, nullptr) {}
    SparseBufferMemoryBindInfo(Buffer buffer, uint32_t bindCount, const SparseMemoryBind* pBinds) {m_sparseBufferMemoryBindInfo.buffer = buffer; m_sparseBufferMemoryBindInfo.bindCount = bindCount; m_sparseBufferMemoryBindInfo.pBinds = reinterpret_cast<const VkSparseMemoryBind*>( pBinds ); }
    SparseBufferMemoryBindInfo(const VkSparseBufferMemoryBindInfo& other) : m_sparseBufferMemoryBindInfo{other} {}
    SparseBufferMemoryBindInfo& operator=(const VkSparseBufferMemoryBindInfo& other){ m_sparseBufferMemoryBindInfo = other; return *this; }

    const Buffer& buffer() const { return m_sparseBufferMemoryBindInfo.buffer; }
    SparseBufferMemoryBindInfo& buffer(Buffer buffer){ m_sparseBufferMemoryBindInfo.buffer = buffer; return *this; }
    const uint32_t& bindCount() const { return m_sparseBufferMemoryBindInfo.bindCount; }
    SparseBufferMemoryBindInfo& bindCount(uint32_t bindCount){ m_sparseBufferMemoryBindInfo.bindCount = bindCount; return *this; }
    const SparseMemoryBind* pBinds() const { return reinterpret_cast<const SparseMemoryBind*>( m_sparseBufferMemoryBindInfo.pBinds ); }
    SparseBufferMemoryBindInfo& pBinds(const SparseMemoryBind* pBinds){ m_sparseBufferMemoryBindInfo.pBinds = reinterpret_cast<const VkSparseMemoryBind*>( pBinds ); return *this; }
    operator VkSparseBufferMemoryBindInfo&() { return m_sparseBufferMemoryBindInfo; }
    operator const VkSparseBufferMemoryBindInfo&() const { return m_sparseBufferMemoryBindInfo; }
    VkSparseBufferMemoryBindInfo& vkHandle() { return m_sparseBufferMemoryBindInfo; }
    const VkSparseBufferMemoryBindInfo& vkHandle() const { return m_sparseBufferMemoryBindInfo; }

  private:
    VkSparseBufferMemoryBindInfo m_sparseBufferMemoryBindInfo;
  };

  class SparseImageOpaqueMemoryBindInfo
  {
  public:
    SparseImageOpaqueMemoryBindInfo() : SparseImageOpaqueMemoryBindInfo(Image(), 0, nullptr) {}
    SparseImageOpaqueMemoryBindInfo(Image image, uint32_t bindCount, const SparseMemoryBind* pBinds) {m_sparseImageOpaqueMemoryBindInfo.image = image; m_sparseImageOpaqueMemoryBindInfo.bindCount = bindCount; m_sparseImageOpaqueMemoryBindInfo.pBinds = reinterpret_cast<const VkSparseMemoryBind*>( pBinds ); }
    SparseImageOpaqueMemoryBindInfo(const VkSparseImageOpaqueMemoryBindInfo& other) : m_sparseImageOpaqueMemoryBindInfo{other} {}
    SparseImageOpaqueMemoryBindInfo& operator=(const VkSparseImageOpaqueMemoryBindInfo& other){ m_sparseImageOpaqueMemoryBindInfo = other; return *this; }

    const Image& image() const { return m_sparseImageOpaqueMemoryBindInfo.image; }
    SparseImageOpaqueMemoryBindInfo& image(Image image){ m_sparseImageOpaqueMemoryBindInfo.image = image; return *this; }
    const uint32_t& bindCount() const { return m_sparseImageOpaqueMemoryBindInfo.bindCount; }
    SparseImageOpaqueMemoryBindInfo& bindCount(uint32_t bindCount){ m_sparseImageOpaqueMemoryBindInfo.bindCount = bindCount; return *this; }
    const SparseMemoryBind* pBinds() const { return reinterpret_cast<const SparseMemoryBind*>( m_sparseImageOpaqueMemoryBindInfo.pBinds ); }
    SparseImageOpaqueMemoryBindInfo& pBinds(const SparseMemoryBind* pBinds){ m_sparseImageOpaqueMemoryBindInfo.pBinds = reinterpret_cast<const VkSparseMemoryBind*>( pBinds ); return *this; }
    operator VkSparseImageOpaqueMemoryBindInfo&() { return m_sparseImageOpaqueMemoryBindInfo; }
    operator const VkSparseImageOpaqueMemoryBindInfo&() const { return m_sparseImageOpaqueMemoryBindInfo; }
    VkSparseImageOpaqueMemoryBindInfo& vkHandle() { return m_sparseImageOpaqueMemoryBindInfo; }
    const VkSparseImageOpaqueMemoryBindInfo& vkHandle() const { return m_sparseImageOpaqueMemoryBindInfo; }

  private:
    VkSparseImageOpaqueMemoryBindInfo m_sparseImageOpaqueMemoryBindInfo;
  };

  class SparseImageMemoryBindInfo
  {
  public:
    SparseImageMemoryBindInfo() : SparseImageMemoryBindInfo(Image(), 0, nullptr) {}
    SparseImageMemoryBindInfo(Image image, uint32_t bindCount, const SparseImageMemoryBind* pBinds) {m_sparseImageMemoryBindInfo.image = image; m_sparseImageMemoryBindInfo.bindCount = bindCount; m_sparseImageMemoryBindInfo.pBinds = reinterpret_cast<const VkSparseImageMemoryBind*>( pBinds ); }
    SparseImageMemoryBindInfo(const VkSparseImageMemoryBindInfo& other) : m_sparseImageMemoryBindInfo{other} {}
    SparseImageMemoryBindInfo& operator=(const VkSparseImageMemoryBindInfo& other){ m_sparseImageMemoryBindInfo = other; return *this; }

    const Image& image() const { return m_sparseImageMemoryBindInfo.image; }
    SparseImageMemoryBindInfo& image(Image image){ m_sparseImageMemoryBindInfo.image = image; return *this; }
    const uint32_t& bindCount() const { return m_sparseImageMemoryBindInfo.bindCount; }
    SparseImageMemoryBindInfo& bindCount(uint32_t bindCount){ m_sparseImageMemoryBindInfo.bindCount = bindCount; return *this; }
    const SparseImageMemoryBind* pBinds() const { return reinterpret_cast<const SparseImageMemoryBind*>( m_sparseImageMemoryBindInfo.pBinds ); }
    SparseImageMemoryBindInfo& pBinds(const SparseImageMemoryBind* pBinds){ m_sparseImageMemoryBindInfo.pBinds = reinterpret_cast<const VkSparseImageMemoryBind*>( pBinds ); return *this; }
    operator VkSparseImageMemoryBindInfo&() { return m_sparseImageMemoryBindInfo; }
    operator const VkSparseImageMemoryBindInfo&() const { return m_sparseImageMemoryBindInfo; }
    VkSparseImageMemoryBindInfo& vkHandle() { return m_sparseImageMemoryBindInfo; }
    const VkSparseImageMemoryBindInfo& vkHandle() const { return m_sparseImageMemoryBindInfo; }

  private:
    VkSparseImageMemoryBindInfo m_sparseImageMemoryBindInfo;
  };

  class BindSparseInfo
  {
  public:
    BindSparseInfo() : BindSparseInfo(0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr) {}
    BindSparseInfo(uint32_t waitSemaphoreCount, const Semaphore* pWaitSemaphores, uint32_t bufferBindCount, const SparseBufferMemoryBindInfo* pBufferBinds, uint32_t imageOpaqueBindCount, const SparseImageOpaqueMemoryBindInfo* pImageOpaqueBinds, uint32_t imageBindCount, const SparseImageMemoryBindInfo* pImageBinds, uint32_t signalSemaphoreCount, const Semaphore* pSignalSemaphores) {m_bindSparseInfo.sType = VK_STRUCTURE_TYPE_BIND_SPARSE_INFO; m_bindSparseInfo.pNext = nullptr; m_bindSparseInfo.waitSemaphoreCount = waitSemaphoreCount; m_bindSparseInfo.pWaitSemaphores = pWaitSemaphores; m_bindSparseInfo.bufferBindCount = bufferBindCount; m_bindSparseInfo.pBufferBinds = reinterpret_cast<const VkSparseBufferMemoryBindInfo*>( pBufferBinds ); m_bindSparseInfo.imageOpaqueBindCount = imageOpaqueBindCount; m_bindSparseInfo.pImageOpaqueBinds = reinterpret_cast<const VkSparseImageOpaqueMemoryBindInfo*>( pImageOpaqueBinds ); m_bindSparseInfo.imageBindCount = imageBindCount; m_bindSparseInfo.pImageBinds = reinterpret_cast<const VkSparseImageMemoryBindInfo*>( pImageBinds ); m_bindSparseInfo.signalSemaphoreCount = signalSemaphoreCount; m_bindSparseInfo.pSignalSemaphores = pSignalSemaphores; }
    BindSparseInfo(const VkBindSparseInfo& other) : m_bindSparseInfo{other} {}
    BindSparseInfo& operator=(const VkBindSparseInfo& other){ m_bindSparseInfo = other; return *this; }

    const StructureType& sType() const { return reinterpret_cast<const StructureType&>( m_bindSparseInfo.sType ); }
    BindSparseInfo& sType(StructureType sType){ m_bindSparseInfo.sType = static_cast<VkStructureType>( sType ); return *this; }
    const void* pNext() const { return reinterpret_cast<const void*>( m_bindSparseInfo.pNext ); }
    BindSparseInfo& pNext(const void* pNext){ m_bindSparseInfo.pNext = pNext; return *this; }
    const uint32_t& waitSemaphoreCount() const { return m_bindSparseInfo.waitSemaphoreCount; }
    BindSparseInfo& waitSemaphoreCount(uint32_t waitSemaphoreCount){ m_bindSparseInfo.waitSemaphoreCount = waitSemaphoreCount; return *this; }
    const Semaphore* pWaitSemaphores() const { return reinterpret_cast<const Semaphore*>( m_bindSparseInfo.pWaitSemaphores ); }
    BindSparseInfo& pWaitSemaphores(const Semaphore* pWaitSemaphores){ m_bindSparseInfo.pWaitSemaphores = pWaitSemaphores; return *this; }
    const uint32_t& bufferBindCount() const { return m_bindSparseInfo.bufferBindCount; }
    BindSparseInfo& bufferBindCount(uint32_t bufferBindCount){ m_bindSparseInfo.bufferBindCount = bufferBindCount; return *this; }
    const SparseBufferMemoryBindInfo* pBufferBinds() const { return reinterpret_cast<const SparseBufferMemoryBindInfo*>( m_bindSparseInfo.pBufferBinds ); }
    BindSparseInfo& pBufferBinds(const SparseBufferMemoryBindInfo* pBufferBinds){ m_bindSparseInfo.pBufferBinds = reinterpret_cast<const VkSparseBufferMemoryBindInfo*>( pBufferBinds ); return *this; }
    const uint32_t& imageOpaqueBindCount() const { return m_bindSparseInfo.imageOpaqueBindCount; }
    BindSparseInfo& imageOpaqueBindCount(uint32_t imageOpaqueBindCount){ m_bindSparseInfo.imageOpaqueBindCount = imageOpaqueBindCount; return *this; }
    const SparseImageOpaqueMemoryBindInfo* pImageOpaqueBinds() const { return reinterpret_cast<const SparseImageOpaqueMemoryBindInfo*>( m_bindSparseInfo.pImageOpaqueBinds ); }
    BindSparseInfo& pImageOpaqueBinds(const SparseImageOpaqueMemoryBindInfo* pImageOpaqueBinds){ m_bindSparseInfo.pImageOpaqueBinds = reinterpret_cast<const VkSparseImageOpaqueMemoryBindInfo*>( pImageOpaqueBinds ); return *this; }
    const uint32_t& imageBindCount() const { return m_bindSparseInfo.imageBindCount; }
    BindSparseInfo& imageBindCount(uint32_t imageBindCount){ m_bindSparseInfo.imageBindCount = imageBindCount; return *this; }
    const SparseImageMemoryBindInfo* pImageBinds() const { return reinterpret_cast<const SparseImageMemoryBindInfo*>( m_bindSparseInfo.pImageBinds ); }
    BindSparseInfo& pImageBinds(const SparseImageMemoryBindInfo* pImageBinds){ m_bindSparseInfo.pImageBinds = reinterpret_cast<const VkSparseImageMemoryBindInfo*>( pImageBinds ); return *this; }
    const uint32_t& signalSemaphoreCount() const { return m_bindSparseInfo.signalSemaphoreCount; }
    BindSparseInfo& signalSemaphoreCount(uint32_t signalSemaphoreCount){ m_bindSparseInfo.signalSemaphoreCount = signalSemaphoreCount; return *this; }
    const Semaphore* pSignalSemaphores() const { return reinterpret_cast<const Semaphore*>( m_bindSparseInfo.pSignalSemaphores ); }
    BindSparseInfo& pSignalSemaphores(const Semaphore* pSignalSemaphores){ m_bindSparseInfo.pSignalSemaphores = pSignalSemaphores; return *this; }
    operator VkBindSparseInfo&() { return m_bindSparseInfo; }
    operator const VkBindSparseInfo&() const { return m_bindSparseInfo; }
    VkBindSparseInfo& vkHandle() { return m_bindSparseInfo; }
    const VkBindSparseInfo& vkHandle() const { return m_bindSparseInfo; }

  private:
    VkBindSparseInfo m_bindSparseInfo;
  };

  class SubmitInfo
  {
  public:
    SubmitInfo() : SubmitInfo(0, nullptr, nullptr, 0, nullptr, 0, nullptr) {}
    SubmitInfo(uint32_t waitSemaphoreCount, const Semaphore* pWaitSemaphores, const PipelineStageFlags* pWaitDstStageMask, uint32_t commandBufferCount, const CommandBuffer* pCommandBuffers, uint32_t signalSemaphoreCount, const Semaphore* pSignalSemaphores) {m_submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO; m_submitInfo.pNext = nullptr; m_submitInfo.waitSemaphoreCount = waitSemaphoreCount; m_submitInfo.pWaitSemaphores = pWaitSemaphores; m_submitInfo.pWaitDstStageMask = reinterpret_cast<const VkPipelineStageFlags*>( pWaitDstStageMask ); m_submitInfo.commandBufferCount = commandBufferCount; m_submitInfo.pCommandBuffers = pCommandBuffers; m_submitInfo.signalSemaphoreCount = signalSemaphoreCount; m_submitInfo.pSignalSemaphores = pSignalSemaphores; }
    SubmitInfo(const VkSubmitInfo& other) : m_submitInfo{other} {}
    SubmitInfo& operator=(const VkSubmitInfo& other){ m_submitInfo = other; return *this; }

    const StructureType& sType() const { return reinterpret_cast<const StructureType&>( m_submitInfo.sType ); }
    SubmitInfo& sType(StructureType sType){ m_submitInfo.sType = static_cast<VkStructureType>( sType ); return *this; }
    const void* pNext() const { return reinterpret_cast<const void*>( m_submitInfo.pNext ); }
    SubmitInfo& pNext(const void* pNext){ m_submitInfo.pNext = pNext; return *this; }
    const uint32_t& waitSemaphoreCount() const { return m_submitInfo.waitSemaphoreCount; }
    SubmitInfo& waitSemaphoreCount(uint32_t waitSemaphoreCount){ m_submitInfo.waitSemaphoreCount = waitSemaphoreCount; return *this; }
    const Semaphore* pWaitSemaphores() const { return reinterpret_cast<const Semaphore*>( m_submitInfo.pWaitSemaphores ); }
    SubmitInfo& pWaitSemaphores(const Semaphore* pWaitSemaphores){ m_submitInfo.pWaitSemaphores = pWaitSemaphores; return *this; }
    const PipelineStageFlags* pWaitDstStageMask() const { return reinterpret_cast<const PipelineStageFlags*>( m_submitInfo.pWaitDstStageMask ); }
    SubmitInfo& pWaitDstStageMask(const PipelineStageFlags* pWaitDstStageMask){ m_submitInfo.pWaitDstStageMask = reinterpret_cast<const VkPipelineStageFlags*>( pWaitDstStageMask ); return *this; }
    const uint32_t& commandBufferCount() const { return m_submitInfo.commandBufferCount; }
    SubmitInfo& commandBufferCount(uint32_t commandBufferCount){ m_submitInfo.commandBufferCount = commandBufferCount; return *this; }
    const CommandBuffer* pCommandBuffers() const { return reinterpret_cast<const CommandBuffer*>( m_submitInfo.pCommandBuffers ); }
    SubmitInfo& pCommandBuffers(const CommandBuffer* pCommandBuffers){ m_submitInfo.pCommandBuffers = pCommandBuffers; return *this; }
    const uint32_t& signalSemaphoreCount() const { return m_submitInfo.signalSemaphoreCount; }
    SubmitInfo& signalSemaphoreCount(uint32_t signalSemaphoreCount){ m_submitInfo.signalSemaphoreCount = signalSemaphoreCount; return *this; }
    const Semaphore* pSignalSemaphores() const { return reinterpret_cast<const Semaphore*>( m_submitInfo.pSignalSemaphores ); }
    SubmitInfo& pSignalSemaphores(const Semaphore* pSignalSemaphores){ m_submitInfo.pSignalSemaphores = pSignalSemaphores; return *this; }
    operator VkSubmitInfo&() { return m_submitInfo; }
    operator const VkSubmitInfo&() const { return m_submitInfo; }
    VkSubmitInfo& vkHandle() { return m_submitInfo; }
    const VkSubmitInfo& vkHandle() const { return m_submitInfo; }

  private:
    VkSubmitInfo m_submitInfo;
  };

  class CommandPoolCreateInfo
  {
  public:
    CommandPoolCreateInfo() : CommandPoolCreateInfo(CommandPoolCreateFlags(), 0) {}
    CommandPoolCreateInfo(CommandPoolCreateFlags flags, uint32_t queueFamilyIndex) {m_commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO; m_commandPoolCreateInfo.pNext = nullptr; m_commandPoolCreateInfo.flags = static_cast<VkCommandPoolCreateFlags>( flags ); m_commandPoolCreateInfo.queueFamilyIndex = queueFamilyIndex; }
    CommandPoolCreateInfo(const VkCommandPoolCreateInfo& other) : m_commandPoolCreateInfo{other} {}
    CommandPoolCreateInfo& operator=(const VkCommandPoolCreateInfo& other){ m_commandPoolCreateInfo = other; return *this; }

    const StructureType& sType() const { return reinterpret_cast<const StructureType&>( m_commandPoolCreateInfo.sType ); }
    CommandPoolCreateInfo& sType(StructureType sType){ m_commandPoolCreateInfo.sType = static_cast<VkStructureType>( sType ); return *this; }
    const void* pNext() const { return reinterpret_cast<const void*>( m_commandPoolCreateInfo.pNext ); }
    CommandPoolCreateInfo& pNext(const void* pNext){ m_commandPoolCreateInfo.pNext = pNext; return *this; }
    const CommandPoolCreateFlags& flags() const { return reinterpret_cast<const CommandPoolCreateFlags&>( m_commandPoolCreateInfo.flags ); }
    CommandPoolCreateInfo& flags(CommandPoolCreateFlags flags){ m_commandPoolCreateInfo.flags = static_cast<VkCommandPoolCreateFlags>( flags ); return *this; }
    const uint32_t& queueFamilyIndex() const { return m_commandPoolCreateInfo.queueFamilyIndex; }
    CommandPoolCreateInfo& queueFamilyIndex(uint32_t queueFamilyIndex){ m_commandPoolCreateInfo.queueFamilyIndex = queueFamilyIndex; return *this; }
    operator VkCommandPoolCreateInfo&() { return m_commandPoolCreateInfo; }
    operator const VkCommandPoolCreateInfo&() const { return m_commandPoolCreateInfo; }
    VkCommandPoolCreateInfo& vkHandle() { return m_commandPoolCreateInfo; }
    const VkCommandPoolCreateInfo& vkHandle() const { return m_commandPoolCreateInfo; }

  private:
    VkCommandPoolCreateInfo m_commandPoolCreateInfo;
  };

  class ImageFormatProperties
  {
  public:
    const Extent3D& maxExtent() const { return reinterpret_cast<const Extent3D&>( m_imageFormatProperties.maxExtent ); }
    const uint32_t& maxMipLevels() const { return m_imageFormatProperties.maxMipLevels; }
    const uint32_t& maxArrayLayers() const { return m_imageFormatProperties.maxArrayLayers; }
    const SampleCountFlags& sampleCounts() const { return reinterpret_cast<const SampleCountFlags&>( m_imageFormatProperties.sampleCounts ); }
    const DeviceSize& maxResourceSize() const { return m_imageFormatProperties.maxResourceSize; }
    operator VkImageFormatProperties&() { return m_imageFormatProperties; }
    operator const VkImageFormatProperties&() const { return m_imageFormatProperties; }
    VkImageFormatProperties& vkHandle() { return m_imageFormatProperties; }
    const VkImageFormatProperties& vkHandle() const { return m_imageFormatProperties; }

  private:
    VkImageFormatProperties m_imageFormatProperties;
  };

  class ImageCreateInfo
  {
  public:
    ImageCreateInfo() : ImageCreateInfo(ImageCreateFlags(), ImageType::e1D, Format::Undefined, Extent3D(), 0, 0, SampleCountFlagBits::e1, ImageTiling::Optimal, ImageUsageFlags(), SharingMode::Exclusive, 0, nullptr, ImageLayout::Undefined) {}
    ImageCreateInfo(ImageCreateFlags flags, ImageType imageType, Format format, Extent3D extent, uint32_t mipLevels, uint32_t arrayLayers, SampleCountFlagBits samples, ImageTiling tiling, ImageUsageFlags usage, SharingMode sharingMode, uint32_t queueFamilyIndexCount, const uint32_t* pQueueFamilyIndices, ImageLayout initialLayout) {m_imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO; m_imageCreateInfo.pNext = nullptr; m_imageCreateInfo.flags = static_cast<VkImageCreateFlags>( flags ); m_imageCreateInfo.imageType = static_cast<VkImageType>( imageType ); m_imageCreateInfo.format = static_cast<VkFormat>( format ); m_imageCreateInfo.extent = static_cast<VkExtent3D>( extent ); m_imageCreateInfo.mipLevels = mipLevels; m_imageCreateInfo.arrayLayers = arrayLayers; m_imageCreateInfo.samples = static_cast<VkSampleCountFlagBits>( samples ); m_imageCreateInfo.tiling = static_cast<VkImageTiling>( tiling ); m_imageCreateInfo.usage = static_cast<VkImageUsageFlags>( usage ); m_imageCreateInfo.sharingMode = static_cast<VkSharingMode>( sharingMode ); m_imageCreateInfo.queueFamilyIndexCount = queueFamilyIndexCount; m_imageCreateInfo.pQueueFamilyIndices = pQueueFamilyIndices; m_imageCreateInfo.initialLayout = static_cast<VkImageLayout>( initialLayout ); }
    ImageCreateInfo(const VkImageCreateInfo& other) : m_imageCreateInfo{other} {}
    ImageCreateInfo& operator=(const VkImageCreateInfo& other){ m_imageCreateInfo = other; return *this; }

    const StructureType& sType() const { return reinterpret_cast<const StructureType&>( m_imageCreateInfo.sType ); }
    ImageCreateInfo& sType(StructureType sType){ m_imageCreateInfo.sType = static_cast<VkStructureType>( sType ); return *this; }
    const void* pNext() const { return reinterpret_cast<const void*>( m_imageCreateInfo.pNext ); }
    ImageCreateInfo& pNext(const void* pNext){ m_imageCreateInfo.pNext = pNext; return *this; }
    const ImageCreateFlags& flags() const { return reinterpret_cast<const ImageCreateFlags&>( m_imageCreateInfo.flags ); }
    ImageCreateInfo& flags(ImageCreateFlags flags){ m_imageCreateInfo.flags = static_cast<VkImageCreateFlags>( flags ); return *this; }
    const ImageType& imageType() const { return reinterpret_cast<const ImageType&>( m_imageCreateInfo.imageType ); }
    ImageCreateInfo& imageType(ImageType imageType){ m_imageCreateInfo.imageType = static_cast<VkImageType>( imageType ); return *this; }
    const Format& format() const { return reinterpret_cast<const Format&>( m_imageCreateInfo.format ); }
    ImageCreateInfo& format(Format format){ m_imageCreateInfo.format = static_cast<VkFormat>( format ); return *this; }
    const Extent3D& extent() const { return reinterpret_cast<const Extent3D&>( m_imageCreateInfo.extent ); }
    ImageCreateInfo& extent(Extent3D extent){ m_imageCreateInfo.extent = static_cast<VkExtent3D>( extent ); return *this; }
    const uint32_t& mipLevels() const { return m_imageCreateInfo.mipLevels; }
    ImageCreateInfo& mipLevels(uint32_t mipLevels){ m_imageCreateInfo.mipLevels = mipLevels; return *this; }
    const uint32_t& arrayLayers() const { return m_imageCreateInfo.arrayLayers; }
    ImageCreateInfo& arrayLayers(uint32_t arrayLayers){ m_imageCreateInfo.arrayLayers = arrayLayers; return *this; }
    const SampleCountFlagBits& samples() const { return reinterpret_cast<const SampleCountFlagBits&>( m_imageCreateInfo.samples ); }
    ImageCreateInfo& samples(SampleCountFlagBits samples){ m_imageCreateInfo.samples = static_cast<VkSampleCountFlagBits>( samples ); return *this; }
    const ImageTiling& tiling() const { return reinterpret_cast<const ImageTiling&>( m_imageCreateInfo.tiling ); }
    ImageCreateInfo& tiling(ImageTiling tiling){ m_imageCreateInfo.tiling = static_cast<VkImageTiling>( tiling ); return *this; }
    const ImageUsageFlags& usage() const { return reinterpret_cast<const ImageUsageFlags&>( m_imageCreateInfo.usage ); }
    ImageCreateInfo& usage(ImageUsageFlags usage){ m_imageCreateInfo.usage = static_cast<VkImageUsageFlags>( usage ); return *this; }
    const SharingMode& sharingMode() const { return reinterpret_cast<const SharingMode&>( m_imageCreateInfo.sharingMode ); }
    ImageCreateInfo& sharingMode(SharingMode sharingMode){ m_imageCreateInfo.sharingMode = static_cast<VkSharingMode>( sharingMode ); return *this; }
    const uint32_t& queueFamilyIndexCount() const { return m_imageCreateInfo.queueFamilyIndexCount; }
    ImageCreateInfo& queueFamilyIndexCount(uint32_t queueFamilyIndexCount){ m_imageCreateInfo.queueFamilyIndexCount = queueFamilyIndexCount; return *this; }
    const uint32_t* pQueueFamilyIndices() const { return reinterpret_cast<const uint32_t*>( m_imageCreateInfo.pQueueFamilyIndices ); }
    ImageCreateInfo& pQueueFamilyIndices(const uint32_t* pQueueFamilyIndices){ m_imageCreateInfo.pQueueFamilyIndices = pQueueFamilyIndices; return *this; }
    const ImageLayout& initialLayout() const { return reinterpret_cast<const ImageLayout&>( m_imageCreateInfo.initialLayout ); }
    ImageCreateInfo& initialLayout(ImageLayout initialLayout){ m_imageCreateInfo.initialLayout = static_cast<VkImageLayout>( initialLayout ); return *this; }
    operator VkImageCreateInfo&() { return m_imageCreateInfo; }
    operator const VkImageCreateInfo&() const { return m_imageCreateInfo; }
    VkImageCreateInfo& vkHandle() { return m_imageCreateInfo; }
    const VkImageCreateInfo& vkHandle() const { return m_imageCreateInfo; }

  private:
    VkImageCreateInfo m_imageCreateInfo;
  };

  class PipelineMultisampleStateCreateInfo
  {
  public:
    PipelineMultisampleStateCreateInfo() : PipelineMultisampleStateCreateInfo(PipelineMultisampleStateCreateFlags(), SampleCountFlagBits::e1, 0, 0, nullptr, 0, 0) {}
    PipelineMultisampleStateCreateInfo(PipelineMultisampleStateCreateFlags flags, SampleCountFlagBits rasterizationSamples, Bool32 sampleShadingEnable, float minSampleShading, const SampleMask* pSampleMask, Bool32 alphaToCoverageEnable, Bool32 alphaToOneEnable) {m_pipelineMultisampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO; m_pipelineMultisampleStateCreateInfo.pNext = nullptr; m_pipelineMultisampleStateCreateInfo.flags = static_cast<VkPipelineMultisampleStateCreateFlags>( flags ); m_pipelineMultisampleStateCreateInfo.rasterizationSamples = static_cast<VkSampleCountFlagBits>( rasterizationSamples ); m_pipelineMultisampleStateCreateInfo.sampleShadingEnable = sampleShadingEnable; m_pipelineMultisampleStateCreateInfo.minSampleShading = minSampleShading; m_pipelineMultisampleStateCreateInfo.pSampleMask = pSampleMask; m_pipelineMultisampleStateCreateInfo.alphaToCoverageEnable = alphaToCoverageEnable; m_pipelineMultisampleStateCreateInfo.alphaToOneEnable = alphaToOneEnable; }
    PipelineMultisampleStateCreateInfo(const VkPipelineMultisampleStateCreateInfo& other) : m_pipelineMultisampleStateCreateInfo{other} {}
    PipelineMultisampleStateCreateInfo& operator=(const VkPipelineMultisampleStateCreateInfo& other){ m_pipelineMultisampleStateCreateInfo = other; return *this; }

    const StructureType& sType() const { return reinterpret_cast<const StructureType&>( m_pipelineMultisampleStateCreateInfo.sType ); }
    PipelineMultisampleStateCreateInfo& sType(StructureType sType){ m_pipelineMultisampleStateCreateInfo.sType = static_cast<VkStructureType>( sType ); return *this; }
    const void* pNext() const { return reinterpret_cast<const void*>( m_pipelineMultisampleStateCreateInfo.pNext ); }
    PipelineMultisampleStateCreateInfo& pNext(const void* pNext){ m_pipelineMultisampleStateCreateInfo.pNext = pNext; return *this; }
    const PipelineMultisampleStateCreateFlags& flags() const { return reinterpret_cast<const PipelineMultisampleStateCreateFlags&>( m_pipelineMultisampleStateCreateInfo.flags ); }
    PipelineMultisampleStateCreateInfo& flags(PipelineMultisampleStateCreateFlags flags){ m_pipelineMultisampleStateCreateInfo.flags = static_cast<VkPipelineMultisampleStateCreateFlags>( flags ); return *this; }
    const SampleCountFlagBits& rasterizationSamples() const { return reinterpret_cast<const SampleCountFlagBits&>( m_pipelineMultisampleStateCreateInfo.rasterizationSamples ); }
    PipelineMultisampleStateCreateInfo& rasterizationSamples(SampleCountFlagBits rasterizationSamples){ m_pipelineMultisampleStateCreateInfo.rasterizationSamples = static_cast<VkSampleCountFlagBits>( rasterizationSamples ); return *this; }
    const Bool32& sampleShadingEnable() const { return m_pipelineMultisampleStateCreateInfo.sampleShadingEnable; }
    PipelineMultisampleStateCreateInfo& sampleShadingEnable(Bool32 sampleShadingEnable){ m_pipelineMultisampleStateCreateInfo.sampleShadingEnable = sampleShadingEnable; return *this; }
    const float& minSampleShading() const { return m_pipelineMultisampleStateCreateInfo.minSampleShading; }
    PipelineMultisampleStateCreateInfo& minSampleShading(float minSampleShading){ m_pipelineMultisampleStateCreateInfo.minSampleShading = minSampleShading; return *this; }
    const SampleMask* pSampleMask() const { return reinterpret_cast<const SampleMask*>( m_pipelineMultisampleStateCreateInfo.pSampleMask ); }
    PipelineMultisampleStateCreateInfo& pSampleMask(const SampleMask* pSampleMask){ m_pipelineMultisampleStateCreateInfo.pSampleMask = pSampleMask; return *this; }
    const Bool32& alphaToCoverageEnable() const { return m_pipelineMultisampleStateCreateInfo.alphaToCoverageEnable; }
    PipelineMultisampleStateCreateInfo& alphaToCoverageEnable(Bool32 alphaToCoverageEnable){ m_pipelineMultisampleStateCreateInfo.alphaToCoverageEnable = alphaToCoverageEnable; return *this; }
    const Bool32& alphaToOneEnable() const { return m_pipelineMultisampleStateCreateInfo.alphaToOneEnable; }
    PipelineMultisampleStateCreateInfo& alphaToOneEnable(Bool32 alphaToOneEnable){ m_pipelineMultisampleStateCreateInfo.alphaToOneEnable = alphaToOneEnable; return *this; }
    operator VkPipelineMultisampleStateCreateInfo&() { return m_pipelineMultisampleStateCreateInfo; }
    operator const VkPipelineMultisampleStateCreateInfo&() const { return m_pipelineMultisampleStateCreateInfo; }
    VkPipelineMultisampleStateCreateInfo& vkHandle() { return m_pipelineMultisampleStateCreateInfo; }
    const VkPipelineMultisampleStateCreateInfo& vkHandle() const { return m_pipelineMultisampleStateCreateInfo; }

  private:
    VkPipelineMultisampleStateCreateInfo m_pipelineMultisampleStateCreateInfo;
  };

  class GraphicsPipelineCreateInfo
  {
  public:
    GraphicsPipelineCreateInfo() : GraphicsPipelineCreateInfo(PipelineCreateFlags(), 0, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, PipelineLayout(), RenderPass(), 0, Pipeline(), 0) {}
    GraphicsPipelineCreateInfo(PipelineCreateFlags flags, uint32_t stageCount, const PipelineShaderStageCreateInfo* pStages, const PipelineVertexInputStateCreateInfo* pVertexInputState, const PipelineInputAssemblyStateCreateInfo* pInputAssemblyState, const PipelineTessellationStateCreateInfo* pTessellationState, const PipelineViewportStateCreateInfo* pViewportState, const PipelineRasterizationStateCreateInfo* pRasterizationState, const PipelineMultisampleStateCreateInfo* pMultisampleState, const PipelineDepthStencilStateCreateInfo* pDepthStencilState, const PipelineColorBlendStateCreateInfo* pColorBlendState, const PipelineDynamicStateCreateInfo* pDynamicState, PipelineLayout layout, RenderPass renderPass, uint32_t subpass, Pipeline basePipelineHandle, int32_t basePipelineIndex) {m_graphicsPipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO; m_graphicsPipelineCreateInfo.pNext = nullptr; m_graphicsPipelineCreateInfo.flags = static_cast<VkPipelineCreateFlags>( flags ); m_graphicsPipelineCreateInfo.stageCount = stageCount; m_graphicsPipelineCreateInfo.pStages = reinterpret_cast<const VkPipelineShaderStageCreateInfo*>( pStages ); m_graphicsPipelineCreateInfo.pVertexInputState = reinterpret_cast<const VkPipelineVertexInputStateCreateInfo*>( pVertexInputState ); m_graphicsPipelineCreateInfo.pInputAssemblyState = reinterpret_cast<const VkPipelineInputAssemblyStateCreateInfo*>( pInputAssemblyState ); m_graphicsPipelineCreateInfo.pTessellationState = reinterpret_cast<const VkPipelineTessellationStateCreateInfo*>( pTessellationState ); m_graphicsPipelineCreateInfo.pViewportState = reinterpret_cast<const VkPipelineViewportStateCreateInfo*>( pViewportState ); m_graphicsPipelineCreateInfo.pRasterizationState = reinterpret_cast<const VkPipelineRasterizationStateCreateInfo*>( pRasterizationState ); m_graphicsPipelineCreateInfo.pMultisampleState = reinterpret_cast<const VkPipelineMultisampleStateCreateInfo*>( pMultisampleState ); m_graphicsPipelineCreateInfo.pDepthStencilState = reinterpret_cast<const VkPipelineDepthStencilStateCreateInfo*>( pDepthStencilState ); m_graphicsPipelineCreateInfo.pColorBlendState = reinterpret_cast<const VkPipelineColorBlendStateCreateInfo*>( pColorBlendState ); m_graphicsPipelineCreateInfo.pDynamicState = reinterpret_cast<const VkPipelineDynamicStateCreateInfo*>( pDynamicState ); m_graphicsPipelineCreateInfo.layout = layout; m_graphicsPipelineCreateInfo.renderPass = renderPass; m_graphicsPipelineCreateInfo.subpass = subpass; m_graphicsPipelineCreateInfo.basePipelineHandle = basePipelineHandle; m_graphicsPipelineCreateInfo.basePipelineIndex = basePipelineIndex; }
    GraphicsPipelineCreateInfo(const VkGraphicsPipelineCreateInfo& other) : m_graphicsPipelineCreateInfo{other} {}
    GraphicsPipelineCreateInfo& operator=(const VkGraphicsPipelineCreateInfo& other){ m_graphicsPipelineCreateInfo = other; return *this; }

    const StructureType& sType() const { return reinterpret_cast<const StructureType&>( m_graphicsPipelineCreateInfo.sType ); }
    GraphicsPipelineCreateInfo& sType(StructureType sType){ m_graphicsPipelineCreateInfo.sType = static_cast<VkStructureType>( sType ); return *this; }
    const void* pNext() const { return reinterpret_cast<const void*>( m_graphicsPipelineCreateInfo.pNext ); }
    GraphicsPipelineCreateInfo& pNext(const void* pNext){ m_graphicsPipelineCreateInfo.pNext = pNext; return *this; }
    const PipelineCreateFlags& flags() const { return reinterpret_cast<const PipelineCreateFlags&>( m_graphicsPipelineCreateInfo.flags ); }
    GraphicsPipelineCreateInfo& flags(PipelineCreateFlags flags){ m_graphicsPipelineCreateInfo.flags = static_cast<VkPipelineCreateFlags>( flags ); return *this; }
    const uint32_t& stageCount() const { return m_graphicsPipelineCreateInfo.stageCount; }
    GraphicsPipelineCreateInfo& stageCount(uint32_t stageCount){ m_graphicsPipelineCreateInfo.stageCount = stageCount; return *this; }
    const PipelineShaderStageCreateInfo* pStages() const { return reinterpret_cast<const PipelineShaderStageCreateInfo*>( m_graphicsPipelineCreateInfo.pStages ); }
    GraphicsPipelineCreateInfo& pStages(const PipelineShaderStageCreateInfo* pStages){ m_graphicsPipelineCreateInfo.pStages = reinterpret_cast<const VkPipelineShaderStageCreateInfo*>( pStages ); return *this; }
    const PipelineVertexInputStateCreateInfo* pVertexInputState() const { return reinterpret_cast<const PipelineVertexInputStateCreateInfo*>( m_graphicsPipelineCreateInfo.pVertexInputState ); }
    GraphicsPipelineCreateInfo& pVertexInputState(const PipelineVertexInputStateCreateInfo* pVertexInputState){ m_graphicsPipelineCreateInfo.pVertexInputState = reinterpret_cast<const VkPipelineVertexInputStateCreateInfo*>( pVertexInputState ); return *this; }
    const PipelineInputAssemblyStateCreateInfo* pInputAssemblyState() const { return reinterpret_cast<const PipelineInputAssemblyStateCreateInfo*>( m_graphicsPipelineCreateInfo.pInputAssemblyState ); }
    GraphicsPipelineCreateInfo& pInputAssemblyState(const PipelineInputAssemblyStateCreateInfo* pInputAssemblyState){ m_graphicsPipelineCreateInfo.pInputAssemblyState = reinterpret_cast<const VkPipelineInputAssemblyStateCreateInfo*>( pInputAssemblyState ); return *this; }
    const PipelineTessellationStateCreateInfo* pTessellationState() const { return reinterpret_cast<const PipelineTessellationStateCreateInfo*>( m_graphicsPipelineCreateInfo.pTessellationState ); }
    GraphicsPipelineCreateInfo& pTessellationState(const PipelineTessellationStateCreateInfo* pTessellationState){ m_graphicsPipelineCreateInfo.pTessellationState = reinterpret_cast<const VkPipelineTessellationStateCreateInfo*>( pTessellationState ); return *this; }
    const PipelineViewportStateCreateInfo* pViewportState() const { return reinterpret_cast<const PipelineViewportStateCreateInfo*>( m_graphicsPipelineCreateInfo.pViewportState ); }
    GraphicsPipelineCreateInfo& pViewportState(const PipelineViewportStateCreateInfo* pViewportState){ m_graphicsPipelineCreateInfo.pViewportState = reinterpret_cast<const VkPipelineViewportStateCreateInfo*>( pViewportState ); return *this; }
    const PipelineRasterizationStateCreateInfo* pRasterizationState() const { return reinterpret_cast<const PipelineRasterizationStateCreateInfo*>( m_graphicsPipelineCreateInfo.pRasterizationState ); }
    GraphicsPipelineCreateInfo& pRasterizationState(const PipelineRasterizationStateCreateInfo* pRasterizationState){ m_graphicsPipelineCreateInfo.pRasterizationState = reinterpret_cast<const VkPipelineRasterizationStateCreateInfo*>( pRasterizationState ); return *this; }
    const PipelineMultisampleStateCreateInfo* pMultisampleState() const { return reinterpret_cast<const PipelineMultisampleStateCreateInfo*>( m_graphicsPipelineCreateInfo.pMultisampleState ); }
    GraphicsPipelineCreateInfo& pMultisampleState(const PipelineMultisampleStateCreateInfo* pMultisampleState){ m_graphicsPipelineCreateInfo.pMultisampleState = reinterpret_cast<const VkPipelineMultisampleStateCreateInfo*>( pMultisampleState ); return *this; }
    const PipelineDepthStencilStateCreateInfo* pDepthStencilState() const { return reinterpret_cast<const PipelineDepthStencilStateCreateInfo*>( m_graphicsPipelineCreateInfo.pDepthStencilState ); }
    GraphicsPipelineCreateInfo& pDepthStencilState(const PipelineDepthStencilStateCreateInfo* pDepthStencilState){ m_graphicsPipelineCreateInfo.pDepthStencilState = reinterpret_cast<const VkPipelineDepthStencilStateCreateInfo*>( pDepthStencilState ); return *this; }
    const PipelineColorBlendStateCreateInfo* pColorBlendState() const { return reinterpret_cast<const PipelineColorBlendStateCreateInfo*>( m_graphicsPipelineCreateInfo.pColorBlendState ); }
    GraphicsPipelineCreateInfo& pColorBlendState(const PipelineColorBlendStateCreateInfo* pColorBlendState){ m_graphicsPipelineCreateInfo.pColorBlendState = reinterpret_cast<const VkPipelineColorBlendStateCreateInfo*>( pColorBlendState ); return *this; }
    const PipelineDynamicStateCreateInfo* pDynamicState() const { return reinterpret_cast<const PipelineDynamicStateCreateInfo*>( m_graphicsPipelineCreateInfo.pDynamicState ); }
    GraphicsPipelineCreateInfo& pDynamicState(const PipelineDynamicStateCreateInfo* pDynamicState){ m_graphicsPipelineCreateInfo.pDynamicState = reinterpret_cast<const VkPipelineDynamicStateCreateInfo*>( pDynamicState ); return *this; }
    const PipelineLayout& layout() const { return m_graphicsPipelineCreateInfo.layout; }
    GraphicsPipelineCreateInfo& layout(PipelineLayout layout){ m_graphicsPipelineCreateInfo.layout = layout; return *this; }
    const RenderPass& renderPass() const { return m_graphicsPipelineCreateInfo.renderPass; }
    GraphicsPipelineCreateInfo& renderPass(RenderPass renderPass){ m_graphicsPipelineCreateInfo.renderPass = renderPass; return *this; }
    const uint32_t& subpass() const { return m_graphicsPipelineCreateInfo.subpass; }
    GraphicsPipelineCreateInfo& subpass(uint32_t subpass){ m_graphicsPipelineCreateInfo.subpass = subpass; return *this; }
    const Pipeline& basePipelineHandle() const { return m_graphicsPipelineCreateInfo.basePipelineHandle; }
    GraphicsPipelineCreateInfo& basePipelineHandle(Pipeline basePipelineHandle){ m_graphicsPipelineCreateInfo.basePipelineHandle = basePipelineHandle; return *this; }
    const int32_t& basePipelineIndex() const { return m_graphicsPipelineCreateInfo.basePipelineIndex; }
    GraphicsPipelineCreateInfo& basePipelineIndex(int32_t basePipelineIndex){ m_graphicsPipelineCreateInfo.basePipelineIndex = basePipelineIndex; return *this; }
    operator VkGraphicsPipelineCreateInfo&() { return m_graphicsPipelineCreateInfo; }
    operator const VkGraphicsPipelineCreateInfo&() const { return m_graphicsPipelineCreateInfo; }
    VkGraphicsPipelineCreateInfo& vkHandle() { return m_graphicsPipelineCreateInfo; }
    const VkGraphicsPipelineCreateInfo& vkHandle() const { return m_graphicsPipelineCreateInfo; }

  private:
    VkGraphicsPipelineCreateInfo m_graphicsPipelineCreateInfo;
  };

  class PhysicalDeviceLimits
  {
  public:
    const uint32_t& maxImageDimension1D() const { return m_physicalDeviceLimits.maxImageDimension1D; }
    const uint32_t& maxImageDimension2D() const { return m_physicalDeviceLimits.maxImageDimension2D; }
    const uint32_t& maxImageDimension3D() const { return m_physicalDeviceLimits.maxImageDimension3D; }
    const uint32_t& maxImageDimensionCube() const { return m_physicalDeviceLimits.maxImageDimensionCube; }
    const uint32_t& maxImageArrayLayers() const { return m_physicalDeviceLimits.maxImageArrayLayers; }
    const uint32_t& maxTexelBufferElements() const { return m_physicalDeviceLimits.maxTexelBufferElements; }
    const uint32_t& maxUniformBufferRange() const { return m_physicalDeviceLimits.maxUniformBufferRange; }
    const uint32_t& maxStorageBufferRange() const { return m_physicalDeviceLimits.maxStorageBufferRange; }
    const uint32_t& maxPushConstantsSize() const { return m_physicalDeviceLimits.maxPushConstantsSize; }
    const uint32_t& maxMemoryAllocationCount() const { return m_physicalDeviceLimits.maxMemoryAllocationCount; }
    const uint32_t& maxSamplerAllocationCount() const { return m_physicalDeviceLimits.maxSamplerAllocationCount; }
    const DeviceSize& bufferImageGranularity() const { return m_physicalDeviceLimits.bufferImageGranularity; }
    const DeviceSize& sparseAddressSpaceSize() const { return m_physicalDeviceLimits.sparseAddressSpaceSize; }
    const uint32_t& maxBoundDescriptorSets() const { return m_physicalDeviceLimits.maxBoundDescriptorSets; }
    const uint32_t& maxPerStageDescriptorSamplers() const { return m_physicalDeviceLimits.maxPerStageDescriptorSamplers; }
    const uint32_t& maxPerStageDescriptorUniformBuffers() const { return m_physicalDeviceLimits.maxPerStageDescriptorUniformBuffers; }
    const uint32_t& maxPerStageDescriptorStorageBuffers() const { return m_physicalDeviceLimits.maxPerStageDescriptorStorageBuffers; }
    const uint32_t& maxPerStageDescriptorSampledImages() const { return m_physicalDeviceLimits.maxPerStageDescriptorSampledImages; }
    const uint32_t& maxPerStageDescriptorStorageImages() const { return m_physicalDeviceLimits.maxPerStageDescriptorStorageImages; }
    const uint32_t& maxPerStageDescriptorInputAttachments() const { return m_physicalDeviceLimits.maxPerStageDescriptorInputAttachments; }
    const uint32_t& maxPerStageResources() const { return m_physicalDeviceLimits.maxPerStageResources; }
    const uint32_t& maxDescriptorSetSamplers() const { return m_physicalDeviceLimits.maxDescriptorSetSamplers; }
    const uint32_t& maxDescriptorSetUniformBuffers() const { return m_physicalDeviceLimits.maxDescriptorSetUniformBuffers; }
    const uint32_t& maxDescriptorSetUniformBuffersDynamic() const { return m_physicalDeviceLimits.maxDescriptorSetUniformBuffersDynamic; }
    const uint32_t& maxDescriptorSetStorageBuffers() const { return m_physicalDeviceLimits.maxDescriptorSetStorageBuffers; }
    const uint32_t& maxDescriptorSetStorageBuffersDynamic() const { return m_physicalDeviceLimits.maxDescriptorSetStorageBuffersDynamic; }
    const uint32_t& maxDescriptorSetSampledImages() const { return m_physicalDeviceLimits.maxDescriptorSetSampledImages; }
    const uint32_t& maxDescriptorSetStorageImages() const { return m_physicalDeviceLimits.maxDescriptorSetStorageImages; }
    const uint32_t& maxDescriptorSetInputAttachments() const { return m_physicalDeviceLimits.maxDescriptorSetInputAttachments; }
    const uint32_t& maxVertexInputAttributes() const { return m_physicalDeviceLimits.maxVertexInputAttributes; }
    const uint32_t& maxVertexInputBindings() const { return m_physicalDeviceLimits.maxVertexInputBindings; }
    const uint32_t& maxVertexInputAttributeOffset() const { return m_physicalDeviceLimits.maxVertexInputAttributeOffset; }
    const uint32_t& maxVertexInputBindingStride() const { return m_physicalDeviceLimits.maxVertexInputBindingStride; }
    const uint32_t& maxVertexOutputComponents() const { return m_physicalDeviceLimits.maxVertexOutputComponents; }
    const uint32_t& maxTessellationGenerationLevel() const { return m_physicalDeviceLimits.maxTessellationGenerationLevel; }
    const uint32_t& maxTessellationPatchSize() const { return m_physicalDeviceLimits.maxTessellationPatchSize; }
    const uint32_t& maxTessellationControlPerVertexInputComponents() const { return m_physicalDeviceLimits.maxTessellationControlPerVertexInputComponents; }
    const uint32_t& maxTessellationControlPerVertexOutputComponents() const { return m_physicalDeviceLimits.maxTessellationControlPerVertexOutputComponents; }
    const uint32_t& maxTessellationControlPerPatchOutputComponents() const { return m_physicalDeviceLimits.maxTessellationControlPerPatchOutputComponents; }
    const uint32_t& maxTessellationControlTotalOutputComponents() const { return m_physicalDeviceLimits.maxTessellationControlTotalOutputComponents; }
    const uint32_t& maxTessellationEvaluationInputComponents() const { return m_physicalDeviceLimits.maxTessellationEvaluationInputComponents; }
    const uint32_t& maxTessellationEvaluationOutputComponents() const { return m_physicalDeviceLimits.maxTessellationEvaluationOutputComponents; }
    const uint32_t& maxGeometryShaderInvocations() const { return m_physicalDeviceLimits.maxGeometryShaderInvocations; }
    const uint32_t& maxGeometryInputComponents() const { return m_physicalDeviceLimits.maxGeometryInputComponents; }
    const uint32_t& maxGeometryOutputComponents() const { return m_physicalDeviceLimits.maxGeometryOutputComponents; }
    const uint32_t& maxGeometryOutputVertices() const { return m_physicalDeviceLimits.maxGeometryOutputVertices; }
    const uint32_t& maxGeometryTotalOutputComponents() const { return m_physicalDeviceLimits.maxGeometryTotalOutputComponents; }
    const uint32_t& maxFragmentInputComponents() const { return m_physicalDeviceLimits.maxFragmentInputComponents; }
    const uint32_t& maxFragmentOutputAttachments() const { return m_physicalDeviceLimits.maxFragmentOutputAttachments; }
    const uint32_t& maxFragmentDualSrcAttachments() const { return m_physicalDeviceLimits.maxFragmentDualSrcAttachments; }
    const uint32_t& maxFragmentCombinedOutputResources() const { return m_physicalDeviceLimits.maxFragmentCombinedOutputResources; }
    const uint32_t& maxComputeSharedMemorySize() const { return m_physicalDeviceLimits.maxComputeSharedMemorySize; }
    const uint32_t* maxComputeWorkGroupCount() const { return reinterpret_cast<const uint32_t*>( m_physicalDeviceLimits.maxComputeWorkGroupCount ); }
    const uint32_t& maxComputeWorkGroupInvocations() const { return m_physicalDeviceLimits.maxComputeWorkGroupInvocations; }
    const uint32_t* maxComputeWorkGroupSize() const { return reinterpret_cast<const uint32_t*>( m_physicalDeviceLimits.maxComputeWorkGroupSize ); }
    const uint32_t& subPixelPrecisionBits() const { return m_physicalDeviceLimits.subPixelPrecisionBits; }
    const uint32_t& subTexelPrecisionBits() const { return m_physicalDeviceLimits.subTexelPrecisionBits; }
    const uint32_t& mipmapPrecisionBits() const { return m_physicalDeviceLimits.mipmapPrecisionBits; }
    const uint32_t& maxDrawIndexedIndexValue() const { return m_physicalDeviceLimits.maxDrawIndexedIndexValue; }
    const uint32_t& maxDrawIndirectCount() const { return m_physicalDeviceLimits.maxDrawIndirectCount; }
    const float& maxSamplerLodBias() const { return m_physicalDeviceLimits.maxSamplerLodBias; }
    const float& maxSamplerAnisotropy() const { return m_physicalDeviceLimits.maxSamplerAnisotropy; }
    const uint32_t& maxViewports() const { return m_physicalDeviceLimits.maxViewports; }
    const uint32_t* maxViewportDimensions() const { return reinterpret_cast<const uint32_t*>( m_physicalDeviceLimits.maxViewportDimensions ); }
    const float* viewportBoundsRange() const { return reinterpret_cast<const float*>( m_physicalDeviceLimits.viewportBoundsRange ); }
    const uint32_t& viewportSubPixelBits() const { return m_physicalDeviceLimits.viewportSubPixelBits; }
    const size_t& minMemoryMapAlignment() const { return m_physicalDeviceLimits.minMemoryMapAlignment; }
    const DeviceSize& minTexelBufferOffsetAlignment() const { return m_physicalDeviceLimits.minTexelBufferOffsetAlignment; }
    const DeviceSize& minUniformBufferOffsetAlignment() const { return m_physicalDeviceLimits.minUniformBufferOffsetAlignment; }
    const DeviceSize& minStorageBufferOffsetAlignment() const { return m_physicalDeviceLimits.minStorageBufferOffsetAlignment; }
    const int32_t& minTexelOffset() const { return m_physicalDeviceLimits.minTexelOffset; }
    const uint32_t& maxTexelOffset() const { return m_physicalDeviceLimits.maxTexelOffset; }
    const int32_t& minTexelGatherOffset() const { return m_physicalDeviceLimits.minTexelGatherOffset; }
    const uint32_t& maxTexelGatherOffset() const { return m_physicalDeviceLimits.maxTexelGatherOffset; }
    const float& minInterpolationOffset() const { return m_physicalDeviceLimits.minInterpolationOffset; }
    const float& maxInterpolationOffset() const { return m_physicalDeviceLimits.maxInterpolationOffset; }
    const uint32_t& subPixelInterpolationOffsetBits() const { return m_physicalDeviceLimits.subPixelInterpolationOffsetBits; }
    const uint32_t& maxFramebufferWidth() const { return m_physicalDeviceLimits.maxFramebufferWidth; }
    const uint32_t& maxFramebufferHeight() const { return m_physicalDeviceLimits.maxFramebufferHeight; }
    const uint32_t& maxFramebufferLayers() const { return m_physicalDeviceLimits.maxFramebufferLayers; }
    const SampleCountFlags& framebufferColorSampleCounts() const { return reinterpret_cast<const SampleCountFlags&>( m_physicalDeviceLimits.framebufferColorSampleCounts ); }
    const SampleCountFlags& framebufferDepthSampleCounts() const { return reinterpret_cast<const SampleCountFlags&>( m_physicalDeviceLimits.framebufferDepthSampleCounts ); }
    const SampleCountFlags& framebufferStencilSampleCounts() const { return reinterpret_cast<const SampleCountFlags&>( m_physicalDeviceLimits.framebufferStencilSampleCounts ); }
    const SampleCountFlags& framebufferNoAttachmentsSampleCounts() const { return reinterpret_cast<const SampleCountFlags&>( m_physicalDeviceLimits.framebufferNoAttachmentsSampleCounts ); }
    const uint32_t& maxColorAttachments() const { return m_physicalDeviceLimits.maxColorAttachments; }
    const SampleCountFlags& sampledImageColorSampleCounts() const { return reinterpret_cast<const SampleCountFlags&>( m_physicalDeviceLimits.sampledImageColorSampleCounts ); }
    const SampleCountFlags& sampledImageIntegerSampleCounts() const { return reinterpret_cast<const SampleCountFlags&>( m_physicalDeviceLimits.sampledImageIntegerSampleCounts ); }
    const SampleCountFlags& sampledImageDepthSampleCounts() const { return reinterpret_cast<const SampleCountFlags&>( m_physicalDeviceLimits.sampledImageDepthSampleCounts ); }
    const SampleCountFlags& sampledImageStencilSampleCounts() const { return reinterpret_cast<const SampleCountFlags&>( m_physicalDeviceLimits.sampledImageStencilSampleCounts ); }
    const SampleCountFlags& storageImageSampleCounts() const { return reinterpret_cast<const SampleCountFlags&>( m_physicalDeviceLimits.storageImageSampleCounts ); }
    const uint32_t& maxSampleMaskWords() const { return m_physicalDeviceLimits.maxSampleMaskWords; }
    const Bool32& timestampComputeAndGraphics() const { return m_physicalDeviceLimits.timestampComputeAndGraphics; }
    const float& timestampPeriod() const { return m_physicalDeviceLimits.timestampPeriod; }
    const uint32_t& maxClipDistances() const { return m_physicalDeviceLimits.maxClipDistances; }
    const uint32_t& maxCullDistances() const { return m_physicalDeviceLimits.maxCullDistances; }
    const uint32_t& maxCombinedClipAndCullDistances() const { return m_physicalDeviceLimits.maxCombinedClipAndCullDistances; }
    const uint32_t& discreteQueuePriorities() const { return m_physicalDeviceLimits.discreteQueuePriorities; }
    const float* pointSizeRange() const { return reinterpret_cast<const float*>( m_physicalDeviceLimits.pointSizeRange ); }
    const float* lineWidthRange() const { return reinterpret_cast<const float*>( m_physicalDeviceLimits.lineWidthRange ); }
    const float& pointSizeGranularity() const { return m_physicalDeviceLimits.pointSizeGranularity; }
    const float& lineWidthGranularity() const { return m_physicalDeviceLimits.lineWidthGranularity; }
    const Bool32& strictLines() const { return m_physicalDeviceLimits.strictLines; }
    const Bool32& standardSampleLocations() const { return m_physicalDeviceLimits.standardSampleLocations; }
    const DeviceSize& optimalBufferCopyOffsetAlignment() const { return m_physicalDeviceLimits.optimalBufferCopyOffsetAlignment; }
    const DeviceSize& optimalBufferCopyRowPitchAlignment() const { return m_physicalDeviceLimits.optimalBufferCopyRowPitchAlignment; }
    const DeviceSize& nonCoherentAtomSize() const { return m_physicalDeviceLimits.nonCoherentAtomSize; }
    operator VkPhysicalDeviceLimits&() { return m_physicalDeviceLimits; }
    operator const VkPhysicalDeviceLimits&() const { return m_physicalDeviceLimits; }
    VkPhysicalDeviceLimits& vkHandle() { return m_physicalDeviceLimits; }
    const VkPhysicalDeviceLimits& vkHandle() const { return m_physicalDeviceLimits; }

  private:
    VkPhysicalDeviceLimits m_physicalDeviceLimits;
  };

  class PhysicalDeviceProperties
  {
  public:
    const uint32_t& apiVersion() const { return m_physicalDeviceProperties.apiVersion; }
    const uint32_t& driverVersion() const { return m_physicalDeviceProperties.driverVersion; }
    const uint32_t& vendorID() const { return m_physicalDeviceProperties.vendorID; }
    const uint32_t& deviceID() const { return m_physicalDeviceProperties.deviceID; }
    const PhysicalDeviceType& deviceType() const { return reinterpret_cast<const PhysicalDeviceType&>( m_physicalDeviceProperties.deviceType ); }
    const char* deviceName() const { return reinterpret_cast<const char*>( m_physicalDeviceProperties.deviceName ); }
    const uint8_t* pipelineCacheUUID() const { return reinterpret_cast<const uint8_t*>( m_physicalDeviceProperties.pipelineCacheUUID ); }
    const PhysicalDeviceLimits& limits() const { return reinterpret_cast<const PhysicalDeviceLimits&>( m_physicalDeviceProperties.limits ); }
    const PhysicalDeviceSparseProperties& sparseProperties() const { return reinterpret_cast<const PhysicalDeviceSparseProperties&>( m_physicalDeviceProperties.sparseProperties ); }
    operator VkPhysicalDeviceProperties&() { return m_physicalDeviceProperties; }
    operator const VkPhysicalDeviceProperties&() const { return m_physicalDeviceProperties; }
    VkPhysicalDeviceProperties& vkHandle() { return m_physicalDeviceProperties; }
    const VkPhysicalDeviceProperties& vkHandle() const { return m_physicalDeviceProperties; }

  private:
    VkPhysicalDeviceProperties m_physicalDeviceProperties;
  };

  class AttachmentDescription
  {
  public:
    AttachmentDescription() : AttachmentDescription(AttachmentDescriptionFlags(), Format::Undefined, SampleCountFlagBits::e1, AttachmentLoadOp::Load, AttachmentStoreOp::Store, AttachmentLoadOp::Load, AttachmentStoreOp::Store, ImageLayout::Undefined, ImageLayout::Undefined) {}
    AttachmentDescription(AttachmentDescriptionFlags flags, Format format, SampleCountFlagBits samples, AttachmentLoadOp loadOp, AttachmentStoreOp storeOp, AttachmentLoadOp stencilLoadOp, AttachmentStoreOp stencilStoreOp, ImageLayout initialLayout, ImageLayout finalLayout) {m_attachmentDescription.flags = static_cast<VkAttachmentDescriptionFlags>( flags ); m_attachmentDescription.format = static_cast<VkFormat>( format ); m_attachmentDescription.samples = static_cast<VkSampleCountFlagBits>( samples ); m_attachmentDescription.loadOp = static_cast<VkAttachmentLoadOp>( loadOp ); m_attachmentDescription.storeOp = static_cast<VkAttachmentStoreOp>( storeOp ); m_attachmentDescription.stencilLoadOp = static_cast<VkAttachmentLoadOp>( stencilLoadOp ); m_attachmentDescription.stencilStoreOp = static_cast<VkAttachmentStoreOp>( stencilStoreOp ); m_attachmentDescription.initialLayout = static_cast<VkImageLayout>( initialLayout ); m_attachmentDescription.finalLayout = static_cast<VkImageLayout>( finalLayout ); }
    AttachmentDescription(const VkAttachmentDescription& other) : m_attachmentDescription{other} {}
    AttachmentDescription& operator=(const VkAttachmentDescription& other){ m_attachmentDescription = other; return *this; }

    const AttachmentDescriptionFlags& flags() const { return reinterpret_cast<const AttachmentDescriptionFlags&>( m_attachmentDescription.flags ); }
    AttachmentDescription& flags(AttachmentDescriptionFlags flags){ m_attachmentDescription.flags = static_cast<VkAttachmentDescriptionFlags>( flags ); return *this; }
    const Format& format() const { return reinterpret_cast<const Format&>( m_attachmentDescription.format ); }
    AttachmentDescription& format(Format format){ m_attachmentDescription.format = static_cast<VkFormat>( format ); return *this; }
    const SampleCountFlagBits& samples() const { return reinterpret_cast<const SampleCountFlagBits&>( m_attachmentDescription.samples ); }
    AttachmentDescription& samples(SampleCountFlagBits samples){ m_attachmentDescription.samples = static_cast<VkSampleCountFlagBits>( samples ); return *this; }
    const AttachmentLoadOp& loadOp() const { return reinterpret_cast<const AttachmentLoadOp&>( m_attachmentDescription.loadOp ); }
    AttachmentDescription& loadOp(AttachmentLoadOp loadOp){ m_attachmentDescription.loadOp = static_cast<VkAttachmentLoadOp>( loadOp ); return *this; }
    const AttachmentStoreOp& storeOp() const { return reinterpret_cast<const AttachmentStoreOp&>( m_attachmentDescription.storeOp ); }
    AttachmentDescription& storeOp(AttachmentStoreOp storeOp){ m_attachmentDescription.storeOp = static_cast<VkAttachmentStoreOp>( storeOp ); return *this; }
    const AttachmentLoadOp& stencilLoadOp() const { return reinterpret_cast<const AttachmentLoadOp&>( m_attachmentDescription.stencilLoadOp ); }
    AttachmentDescription& stencilLoadOp(AttachmentLoadOp stencilLoadOp){ m_attachmentDescription.stencilLoadOp = static_cast<VkAttachmentLoadOp>( stencilLoadOp ); return *this; }
    const AttachmentStoreOp& stencilStoreOp() const { return reinterpret_cast<const AttachmentStoreOp&>( m_attachmentDescription.stencilStoreOp ); }
    AttachmentDescription& stencilStoreOp(AttachmentStoreOp stencilStoreOp){ m_attachmentDescription.stencilStoreOp = static_cast<VkAttachmentStoreOp>( stencilStoreOp ); return *this; }
    const ImageLayout& initialLayout() const { return reinterpret_cast<const ImageLayout&>( m_attachmentDescription.initialLayout ); }
    AttachmentDescription& initialLayout(ImageLayout initialLayout){ m_attachmentDescription.initialLayout = static_cast<VkImageLayout>( initialLayout ); return *this; }
    const ImageLayout& finalLayout() const { return reinterpret_cast<const ImageLayout&>( m_attachmentDescription.finalLayout ); }
    AttachmentDescription& finalLayout(ImageLayout finalLayout){ m_attachmentDescription.finalLayout = static_cast<VkImageLayout>( finalLayout ); return *this; }
    operator VkAttachmentDescription&() { return m_attachmentDescription; }
    operator const VkAttachmentDescription&() const { return m_attachmentDescription; }
    VkAttachmentDescription& vkHandle() { return m_attachmentDescription; }
    const VkAttachmentDescription& vkHandle() const { return m_attachmentDescription; }

  private:
    VkAttachmentDescription m_attachmentDescription;
  };

  class DescriptorPoolCreateInfo
  {
  public:
    DescriptorPoolCreateInfo() : DescriptorPoolCreateInfo(DescriptorPoolCreateFlags(), 0, 0, nullptr) {}
    DescriptorPoolCreateInfo(DescriptorPoolCreateFlags flags, uint32_t maxSets, uint32_t poolSizeCount, const DescriptorPoolSize* pPoolSizes) {m_descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO; m_descriptorPoolCreateInfo.pNext = nullptr; m_descriptorPoolCreateInfo.flags = static_cast<VkDescriptorPoolCreateFlags>( flags ); m_descriptorPoolCreateInfo.maxSets = maxSets; m_descriptorPoolCreateInfo.poolSizeCount = poolSizeCount; m_descriptorPoolCreateInfo.pPoolSizes = reinterpret_cast<const VkDescriptorPoolSize*>( pPoolSizes ); }
    DescriptorPoolCreateInfo(const VkDescriptorPoolCreateInfo& other) : m_descriptorPoolCreateInfo{other} {}
    DescriptorPoolCreateInfo& operator=(const VkDescriptorPoolCreateInfo& other){ m_descriptorPoolCreateInfo = other; return *this; }

    const StructureType& sType() const { return reinterpret_cast<const StructureType&>( m_descriptorPoolCreateInfo.sType ); }
    DescriptorPoolCreateInfo& sType(StructureType sType){ m_descriptorPoolCreateInfo.sType = static_cast<VkStructureType>( sType ); return *this; }
    const void* pNext() const { return reinterpret_cast<const void*>( m_descriptorPoolCreateInfo.pNext ); }
    DescriptorPoolCreateInfo& pNext(const void* pNext){ m_descriptorPoolCreateInfo.pNext = pNext; return *this; }
    const DescriptorPoolCreateFlags& flags() const { return reinterpret_cast<const DescriptorPoolCreateFlags&>( m_descriptorPoolCreateInfo.flags ); }
    DescriptorPoolCreateInfo& flags(DescriptorPoolCreateFlags flags){ m_descriptorPoolCreateInfo.flags = static_cast<VkDescriptorPoolCreateFlags>( flags ); return *this; }
    const uint32_t& maxSets() const { return m_descriptorPoolCreateInfo.maxSets; }
    DescriptorPoolCreateInfo& maxSets(uint32_t maxSets){ m_descriptorPoolCreateInfo.maxSets = maxSets; return *this; }
    const uint32_t& poolSizeCount() const { return m_descriptorPoolCreateInfo.poolSizeCount; }
    DescriptorPoolCreateInfo& poolSizeCount(uint32_t poolSizeCount){ m_descriptorPoolCreateInfo.poolSizeCount = poolSizeCount; return *this; }
    const DescriptorPoolSize* pPoolSizes() const { return reinterpret_cast<const DescriptorPoolSize*>( m_descriptorPoolCreateInfo.pPoolSizes ); }
    DescriptorPoolCreateInfo& pPoolSizes(const DescriptorPoolSize* pPoolSizes){ m_descriptorPoolCreateInfo.pPoolSizes = reinterpret_cast<const VkDescriptorPoolSize*>( pPoolSizes ); return *this; }
    operator VkDescriptorPoolCreateInfo&() { return m_descriptorPoolCreateInfo; }
    operator const VkDescriptorPoolCreateInfo&() const { return m_descriptorPoolCreateInfo; }
    VkDescriptorPoolCreateInfo& vkHandle() { return m_descriptorPoolCreateInfo; }
    const VkDescriptorPoolCreateInfo& vkHandle() const { return m_descriptorPoolCreateInfo; }

  private:
    VkDescriptorPoolCreateInfo m_descriptorPoolCreateInfo;
  };

  class SubpassDependency
  {
  public:
    SubpassDependency() : SubpassDependency(0, 0, PipelineStageFlags(), PipelineStageFlags(), AccessFlags(), AccessFlags(), DependencyFlags()) {}
    SubpassDependency(uint32_t srcSubpass, uint32_t dstSubpass, PipelineStageFlags srcStageMask, PipelineStageFlags dstStageMask, AccessFlags srcAccessMask, AccessFlags dstAccessMask, DependencyFlags dependencyFlags) {m_subpassDependency.srcSubpass = srcSubpass; m_subpassDependency.dstSubpass = dstSubpass; m_subpassDependency.srcStageMask = static_cast<VkPipelineStageFlags>( srcStageMask ); m_subpassDependency.dstStageMask = static_cast<VkPipelineStageFlags>( dstStageMask ); m_subpassDependency.srcAccessMask = static_cast<VkAccessFlags>( srcAccessMask ); m_subpassDependency.dstAccessMask = static_cast<VkAccessFlags>( dstAccessMask ); m_subpassDependency.dependencyFlags = static_cast<VkDependencyFlags>( dependencyFlags ); }
    SubpassDependency(const VkSubpassDependency& other) : m_subpassDependency{other} {}
    SubpassDependency& operator=(const VkSubpassDependency& other){ m_subpassDependency = other; return *this; }

    const uint32_t& srcSubpass() const { return m_subpassDependency.srcSubpass; }
    SubpassDependency& srcSubpass(uint32_t srcSubpass){ m_subpassDependency.srcSubpass = srcSubpass; return *this; }
    const uint32_t& dstSubpass() const { return m_subpassDependency.dstSubpass; }
    SubpassDependency& dstSubpass(uint32_t dstSubpass){ m_subpassDependency.dstSubpass = dstSubpass; return *this; }
    const PipelineStageFlags& srcStageMask() const { return reinterpret_cast<const PipelineStageFlags&>( m_subpassDependency.srcStageMask ); }
    SubpassDependency& srcStageMask(PipelineStageFlags srcStageMask){ m_subpassDependency.srcStageMask = static_cast<VkPipelineStageFlags>( srcStageMask ); return *this; }
    const PipelineStageFlags& dstStageMask() const { return reinterpret_cast<const PipelineStageFlags&>( m_subpassDependency.dstStageMask ); }
    SubpassDependency& dstStageMask(PipelineStageFlags dstStageMask){ m_subpassDependency.dstStageMask = static_cast<VkPipelineStageFlags>( dstStageMask ); return *this; }
    const AccessFlags& srcAccessMask() const { return reinterpret_cast<const AccessFlags&>( m_subpassDependency.srcAccessMask ); }
    SubpassDependency& srcAccessMask(AccessFlags srcAccessMask){ m_subpassDependency.srcAccessMask = static_cast<VkAccessFlags>( srcAccessMask ); return *this; }
    const AccessFlags& dstAccessMask() const { return reinterpret_cast<const AccessFlags&>( m_subpassDependency.dstAccessMask ); }
    SubpassDependency& dstAccessMask(AccessFlags dstAccessMask){ m_subpassDependency.dstAccessMask = static_cast<VkAccessFlags>( dstAccessMask ); return *this; }
    const DependencyFlags& dependencyFlags() const { return reinterpret_cast<const DependencyFlags&>( m_subpassDependency.dependencyFlags ); }
    SubpassDependency& dependencyFlags(DependencyFlags dependencyFlags){ m_subpassDependency.dependencyFlags = static_cast<VkDependencyFlags>( dependencyFlags ); return *this; }
    operator VkSubpassDependency&() { return m_subpassDependency; }
    operator const VkSubpassDependency&() const { return m_subpassDependency; }
    VkSubpassDependency& vkHandle() { return m_subpassDependency; }
    const VkSubpassDependency& vkHandle() const { return m_subpassDependency; }

  private:
    VkSubpassDependency m_subpassDependency;
  };

  class RenderPassCreateInfo
  {
  public:
    RenderPassCreateInfo() : RenderPassCreateInfo(RenderPassCreateFlags(), 0, nullptr, 0, nullptr, 0, nullptr) {}
    RenderPassCreateInfo(RenderPassCreateFlags flags, uint32_t attachmentCount, const AttachmentDescription* pAttachments, uint32_t subpassCount, const SubpassDescription* pSubpasses, uint32_t dependencyCount, const SubpassDependency* pDependencies) {m_renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO; m_renderPassCreateInfo.pNext = nullptr; m_renderPassCreateInfo.flags = static_cast<VkRenderPassCreateFlags>( flags ); m_renderPassCreateInfo.attachmentCount = attachmentCount; m_renderPassCreateInfo.pAttachments = reinterpret_cast<const VkAttachmentDescription*>( pAttachments ); m_renderPassCreateInfo.subpassCount = subpassCount; m_renderPassCreateInfo.pSubpasses = reinterpret_cast<const VkSubpassDescription*>( pSubpasses ); m_renderPassCreateInfo.dependencyCount = dependencyCount; m_renderPassCreateInfo.pDependencies = reinterpret_cast<const VkSubpassDependency*>( pDependencies ); }
    RenderPassCreateInfo(const VkRenderPassCreateInfo& other) : m_renderPassCreateInfo{other} {}
    RenderPassCreateInfo& operator=(const VkRenderPassCreateInfo& other){ m_renderPassCreateInfo = other; return *this; }

    const StructureType& sType() const { return reinterpret_cast<const StructureType&>( m_renderPassCreateInfo.sType ); }
    RenderPassCreateInfo& sType(StructureType sType){ m_renderPassCreateInfo.sType = static_cast<VkStructureType>( sType ); return *this; }
    const void* pNext() const { return reinterpret_cast<const void*>( m_renderPassCreateInfo.pNext ); }
    RenderPassCreateInfo& pNext(const void* pNext){ m_renderPassCreateInfo.pNext = pNext; return *this; }
    const RenderPassCreateFlags& flags() const { return reinterpret_cast<const RenderPassCreateFlags&>( m_renderPassCreateInfo.flags ); }
    RenderPassCreateInfo& flags(RenderPassCreateFlags flags){ m_renderPassCreateInfo.flags = static_cast<VkRenderPassCreateFlags>( flags ); return *this; }
    const uint32_t& attachmentCount() const { return m_renderPassCreateInfo.attachmentCount; }
    RenderPassCreateInfo& attachmentCount(uint32_t attachmentCount){ m_renderPassCreateInfo.attachmentCount = attachmentCount; return *this; }
    const AttachmentDescription* pAttachments() const { return reinterpret_cast<const AttachmentDescription*>( m_renderPassCreateInfo.pAttachments ); }
    RenderPassCreateInfo& pAttachments(const AttachmentDescription* pAttachments){ m_renderPassCreateInfo.pAttachments = reinterpret_cast<const VkAttachmentDescription*>( pAttachments ); return *this; }
    const uint32_t& subpassCount() const { return m_renderPassCreateInfo.subpassCount; }
    RenderPassCreateInfo& subpassCount(uint32_t subpassCount){ m_renderPassCreateInfo.subpassCount = subpassCount; return *this; }
    const SubpassDescription* pSubpasses() const { return reinterpret_cast<const SubpassDescription*>( m_renderPassCreateInfo.pSubpasses ); }
    RenderPassCreateInfo& pSubpasses(const SubpassDescription* pSubpasses){ m_renderPassCreateInfo.pSubpasses = reinterpret_cast<const VkSubpassDescription*>( pSubpasses ); return *this; }
    const uint32_t& dependencyCount() const { return m_renderPassCreateInfo.dependencyCount; }
    RenderPassCreateInfo& dependencyCount(uint32_t dependencyCount){ m_renderPassCreateInfo.dependencyCount = dependencyCount; return *this; }
    const SubpassDependency* pDependencies() const { return reinterpret_cast<const SubpassDependency*>( m_renderPassCreateInfo.pDependencies ); }
    RenderPassCreateInfo& pDependencies(const SubpassDependency* pDependencies){ m_renderPassCreateInfo.pDependencies = reinterpret_cast<const VkSubpassDependency*>( pDependencies ); return *this; }
    operator VkRenderPassCreateInfo&() { return m_renderPassCreateInfo; }
    operator const VkRenderPassCreateInfo&() const { return m_renderPassCreateInfo; }
    VkRenderPassCreateInfo& vkHandle() { return m_renderPassCreateInfo; }
    const VkRenderPassCreateInfo& vkHandle() const { return m_renderPassCreateInfo; }

  private:
    VkRenderPassCreateInfo m_renderPassCreateInfo;
  };

  class SurfaceFormatKHR
  {
  public:
    SurfaceFormatKHR() : SurfaceFormatKHR(Format::Undefined, ColorSpaceKHR::VkColorspaceSrgbNonlinearKHR) {}
    SurfaceFormatKHR(Format format, ColorSpaceKHR colorSpace) {m_surfaceFormatKHR.format = static_cast<VkFormat>( format ); m_surfaceFormatKHR.colorSpace = static_cast<VkColorSpaceKHR>( colorSpace ); }
    SurfaceFormatKHR(const VkSurfaceFormatKHR& other) : m_surfaceFormatKHR{other} {}
    SurfaceFormatKHR& operator=(const VkSurfaceFormatKHR& other){ m_surfaceFormatKHR = other; return *this; }

    const Format& format() const { return reinterpret_cast<const Format&>( m_surfaceFormatKHR.format ); }
    SurfaceFormatKHR& format(Format format){ m_surfaceFormatKHR.format = static_cast<VkFormat>( format ); return *this; }
    const ColorSpaceKHR& colorSpace() const { return reinterpret_cast<const ColorSpaceKHR&>( m_surfaceFormatKHR.colorSpace ); }
    SurfaceFormatKHR& colorSpace(ColorSpaceKHR colorSpace){ m_surfaceFormatKHR.colorSpace = static_cast<VkColorSpaceKHR>( colorSpace ); return *this; }
    operator VkSurfaceFormatKHR&() { return m_surfaceFormatKHR; }
    operator const VkSurfaceFormatKHR&() const { return m_surfaceFormatKHR; }
    VkSurfaceFormatKHR& vkHandle() { return m_surfaceFormatKHR; }
    const VkSurfaceFormatKHR& vkHandle() const { return m_surfaceFormatKHR; }

  private:
    VkSurfaceFormatKHR m_surfaceFormatKHR;
  };

  class SurfaceCapabilitiesKHR
  {
  public:
    SurfaceCapabilitiesKHR() : SurfaceCapabilitiesKHR(0, 0, Extent2D(), Extent2D(), Extent2D(), 0, SurfaceTransformFlagsKHR(), SurfaceTransformFlagBitsKHR::IdentityKHR, CompositeAlphaFlagsKHR(), ImageUsageFlags()) {}
    SurfaceCapabilitiesKHR(uint32_t minImageCount, uint32_t maxImageCount, Extent2D currentExtent, Extent2D minImageExtent, Extent2D maxImageExtent, uint32_t maxImageArrayLayers, SurfaceTransformFlagsKHR supportedTransforms, SurfaceTransformFlagBitsKHR currentTransform, CompositeAlphaFlagsKHR supportedCompositeAlpha, ImageUsageFlags supportedUsageFlags) {m_surfaceCapabilitiesKHR.minImageCount = minImageCount; m_surfaceCapabilitiesKHR.maxImageCount = maxImageCount; m_surfaceCapabilitiesKHR.currentExtent = static_cast<VkExtent2D>( currentExtent ); m_surfaceCapabilitiesKHR.minImageExtent = static_cast<VkExtent2D>( minImageExtent ); m_surfaceCapabilitiesKHR.maxImageExtent = static_cast<VkExtent2D>( maxImageExtent ); m_surfaceCapabilitiesKHR.maxImageArrayLayers = maxImageArrayLayers; m_surfaceCapabilitiesKHR.supportedTransforms = static_cast<VkSurfaceTransformFlagsKHR>( supportedTransforms ); m_surfaceCapabilitiesKHR.currentTransform = static_cast<VkSurfaceTransformFlagBitsKHR>( currentTransform ); m_surfaceCapabilitiesKHR.supportedCompositeAlpha = static_cast<VkCompositeAlphaFlagsKHR>( supportedCompositeAlpha ); m_surfaceCapabilitiesKHR.supportedUsageFlags = static_cast<VkImageUsageFlags>( supportedUsageFlags ); }
    SurfaceCapabilitiesKHR(const VkSurfaceCapabilitiesKHR& other) : m_surfaceCapabilitiesKHR{other} {}
    SurfaceCapabilitiesKHR& operator=(const VkSurfaceCapabilitiesKHR& other){ m_surfaceCapabilitiesKHR = other; return *this; }

    const uint32_t& minImageCount() const { return m_surfaceCapabilitiesKHR.minImageCount; }
    SurfaceCapabilitiesKHR& minImageCount(uint32_t minImageCount){ m_surfaceCapabilitiesKHR.minImageCount = minImageCount; return *this; }
    const uint32_t& maxImageCount() const { return m_surfaceCapabilitiesKHR.maxImageCount; }
    SurfaceCapabilitiesKHR& maxImageCount(uint32_t maxImageCount){ m_surfaceCapabilitiesKHR.maxImageCount = maxImageCount; return *this; }
    const Extent2D& currentExtent() const { return reinterpret_cast<const Extent2D&>( m_surfaceCapabilitiesKHR.currentExtent ); }
    SurfaceCapabilitiesKHR& currentExtent(Extent2D currentExtent){ m_surfaceCapabilitiesKHR.currentExtent = static_cast<VkExtent2D>( currentExtent ); return *this; }
    const Extent2D& minImageExtent() const { return reinterpret_cast<const Extent2D&>( m_surfaceCapabilitiesKHR.minImageExtent ); }
    SurfaceCapabilitiesKHR& minImageExtent(Extent2D minImageExtent){ m_surfaceCapabilitiesKHR.minImageExtent = static_cast<VkExtent2D>( minImageExtent ); return *this; }
    const Extent2D& maxImageExtent() const { return reinterpret_cast<const Extent2D&>( m_surfaceCapabilitiesKHR.maxImageExtent ); }
    SurfaceCapabilitiesKHR& maxImageExtent(Extent2D maxImageExtent){ m_surfaceCapabilitiesKHR.maxImageExtent = static_cast<VkExtent2D>( maxImageExtent ); return *this; }
    const uint32_t& maxImageArrayLayers() const { return m_surfaceCapabilitiesKHR.maxImageArrayLayers; }
    SurfaceCapabilitiesKHR& maxImageArrayLayers(uint32_t maxImageArrayLayers){ m_surfaceCapabilitiesKHR.maxImageArrayLayers = maxImageArrayLayers; return *this; }
    const SurfaceTransformFlagsKHR& supportedTransforms() const { return reinterpret_cast<const SurfaceTransformFlagsKHR&>( m_surfaceCapabilitiesKHR.supportedTransforms ); }
    SurfaceCapabilitiesKHR& supportedTransforms(SurfaceTransformFlagsKHR supportedTransforms){ m_surfaceCapabilitiesKHR.supportedTransforms = static_cast<VkSurfaceTransformFlagsKHR>( supportedTransforms ); return *this; }
    const SurfaceTransformFlagBitsKHR& currentTransform() const { return reinterpret_cast<const SurfaceTransformFlagBitsKHR&>( m_surfaceCapabilitiesKHR.currentTransform ); }
    SurfaceCapabilitiesKHR& currentTransform(SurfaceTransformFlagBitsKHR currentTransform){ m_surfaceCapabilitiesKHR.currentTransform = static_cast<VkSurfaceTransformFlagBitsKHR>( currentTransform ); return *this; }
    const CompositeAlphaFlagsKHR& supportedCompositeAlpha() const { return reinterpret_cast<const CompositeAlphaFlagsKHR&>( m_surfaceCapabilitiesKHR.supportedCompositeAlpha ); }
    SurfaceCapabilitiesKHR& supportedCompositeAlpha(CompositeAlphaFlagsKHR supportedCompositeAlpha){ m_surfaceCapabilitiesKHR.supportedCompositeAlpha = static_cast<VkCompositeAlphaFlagsKHR>( supportedCompositeAlpha ); return *this; }
    const ImageUsageFlags& supportedUsageFlags() const { return reinterpret_cast<const ImageUsageFlags&>( m_surfaceCapabilitiesKHR.supportedUsageFlags ); }
    SurfaceCapabilitiesKHR& supportedUsageFlags(ImageUsageFlags supportedUsageFlags){ m_surfaceCapabilitiesKHR.supportedUsageFlags = static_cast<VkImageUsageFlags>( supportedUsageFlags ); return *this; }
    operator VkSurfaceCapabilitiesKHR&() { return m_surfaceCapabilitiesKHR; }
    operator const VkSurfaceCapabilitiesKHR&() const { return m_surfaceCapabilitiesKHR; }
    VkSurfaceCapabilitiesKHR& vkHandle() { return m_surfaceCapabilitiesKHR; }
    const VkSurfaceCapabilitiesKHR& vkHandle() const { return m_surfaceCapabilitiesKHR; }

  private:
    VkSurfaceCapabilitiesKHR m_surfaceCapabilitiesKHR;
  };

  class SwapchainCreateInfoKHR
  {
  public:
    SwapchainCreateInfoKHR() : SwapchainCreateInfoKHR(SwapchainCreateFlagsKHR(), SurfaceKHR(), 0, Format::Undefined, ColorSpaceKHR::VkColorspaceSrgbNonlinearKHR, Extent2D(), 0, ImageUsageFlags(), SharingMode::Exclusive, 0, nullptr, SurfaceTransformFlagBitsKHR::IdentityKHR, CompositeAlphaFlagBitsKHR::OpaqueKHR, PresentModeKHR::ImmediateKHR, 0, SwapchainKHR()) {}
    SwapchainCreateInfoKHR(SwapchainCreateFlagsKHR flags, SurfaceKHR surface, uint32_t minImageCount, Format imageFormat, ColorSpaceKHR imageColorSpace, Extent2D imageExtent, uint32_t imageArrayLayers, ImageUsageFlags imageUsage, SharingMode imageSharingMode, uint32_t queueFamilyIndexCount, const uint32_t* pQueueFamilyIndices, SurfaceTransformFlagBitsKHR preTransform, CompositeAlphaFlagBitsKHR compositeAlpha, PresentModeKHR presentMode, Bool32 clipped, SwapchainKHR oldSwapchain) {m_swapchainCreateInfoKHR.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR; m_swapchainCreateInfoKHR.pNext = nullptr; m_swapchainCreateInfoKHR.flags = static_cast<VkSwapchainCreateFlagsKHR>( flags ); m_swapchainCreateInfoKHR.surface = surface; m_swapchainCreateInfoKHR.minImageCount = minImageCount; m_swapchainCreateInfoKHR.imageFormat = static_cast<VkFormat>( imageFormat ); m_swapchainCreateInfoKHR.imageColorSpace = static_cast<VkColorSpaceKHR>( imageColorSpace ); m_swapchainCreateInfoKHR.imageExtent = static_cast<VkExtent2D>( imageExtent ); m_swapchainCreateInfoKHR.imageArrayLayers = imageArrayLayers; m_swapchainCreateInfoKHR.imageUsage = static_cast<VkImageUsageFlags>( imageUsage ); m_swapchainCreateInfoKHR.imageSharingMode = static_cast<VkSharingMode>( imageSharingMode ); m_swapchainCreateInfoKHR.queueFamilyIndexCount = queueFamilyIndexCount; m_swapchainCreateInfoKHR.pQueueFamilyIndices = pQueueFamilyIndices; m_swapchainCreateInfoKHR.preTransform = static_cast<VkSurfaceTransformFlagBitsKHR>( preTransform ); m_swapchainCreateInfoKHR.compositeAlpha = static_cast<VkCompositeAlphaFlagBitsKHR>( compositeAlpha ); m_swapchainCreateInfoKHR.presentMode = static_cast<VkPresentModeKHR>( presentMode ); m_swapchainCreateInfoKHR.clipped = clipped; m_swapchainCreateInfoKHR.oldSwapchain = oldSwapchain; }
    SwapchainCreateInfoKHR(const VkSwapchainCreateInfoKHR& other) : m_swapchainCreateInfoKHR{other} {}
    SwapchainCreateInfoKHR& operator=(const VkSwapchainCreateInfoKHR& other){ m_swapchainCreateInfoKHR = other; return *this; }

    const StructureType& sType() const { return reinterpret_cast<const StructureType&>( m_swapchainCreateInfoKHR.sType ); }
    SwapchainCreateInfoKHR& sType(StructureType sType){ m_swapchainCreateInfoKHR.sType = static_cast<VkStructureType>( sType ); return *this; }
    const void* pNext() const { return reinterpret_cast<const void*>( m_swapchainCreateInfoKHR.pNext ); }
    SwapchainCreateInfoKHR& pNext(const void* pNext){ m_swapchainCreateInfoKHR.pNext = pNext; return *this; }
    const SwapchainCreateFlagsKHR& flags() const { return reinterpret_cast<const SwapchainCreateFlagsKHR&>( m_swapchainCreateInfoKHR.flags ); }
    SwapchainCreateInfoKHR& flags(SwapchainCreateFlagsKHR flags){ m_swapchainCreateInfoKHR.flags = static_cast<VkSwapchainCreateFlagsKHR>( flags ); return *this; }
    const SurfaceKHR& surface() const { return m_swapchainCreateInfoKHR.surface; }
    SwapchainCreateInfoKHR& surface(SurfaceKHR surface){ m_swapchainCreateInfoKHR.surface = surface; return *this; }
    const uint32_t& minImageCount() const { return m_swapchainCreateInfoKHR.minImageCount; }
    SwapchainCreateInfoKHR& minImageCount(uint32_t minImageCount){ m_swapchainCreateInfoKHR.minImageCount = minImageCount; return *this; }
    const Format& imageFormat() const { return reinterpret_cast<const Format&>( m_swapchainCreateInfoKHR.imageFormat ); }
    SwapchainCreateInfoKHR& imageFormat(Format imageFormat){ m_swapchainCreateInfoKHR.imageFormat = static_cast<VkFormat>( imageFormat ); return *this; }
    const ColorSpaceKHR& imageColorSpace() const { return reinterpret_cast<const ColorSpaceKHR&>( m_swapchainCreateInfoKHR.imageColorSpace ); }
    SwapchainCreateInfoKHR& imageColorSpace(ColorSpaceKHR imageColorSpace){ m_swapchainCreateInfoKHR.imageColorSpace = static_cast<VkColorSpaceKHR>( imageColorSpace ); return *this; }
    const Extent2D& imageExtent() const { return reinterpret_cast<const Extent2D&>( m_swapchainCreateInfoKHR.imageExtent ); }
    SwapchainCreateInfoKHR& imageExtent(Extent2D imageExtent){ m_swapchainCreateInfoKHR.imageExtent = static_cast<VkExtent2D>( imageExtent ); return *this; }
    const uint32_t& imageArrayLayers() const { return m_swapchainCreateInfoKHR.imageArrayLayers; }
    SwapchainCreateInfoKHR& imageArrayLayers(uint32_t imageArrayLayers){ m_swapchainCreateInfoKHR.imageArrayLayers = imageArrayLayers; return *this; }
    const ImageUsageFlags& imageUsage() const { return reinterpret_cast<const ImageUsageFlags&>( m_swapchainCreateInfoKHR.imageUsage ); }
    SwapchainCreateInfoKHR& imageUsage(ImageUsageFlags imageUsage){ m_swapchainCreateInfoKHR.imageUsage = static_cast<VkImageUsageFlags>( imageUsage ); return *this; }
    const SharingMode& imageSharingMode() const { return reinterpret_cast<const SharingMode&>( m_swapchainCreateInfoKHR.imageSharingMode ); }
    SwapchainCreateInfoKHR& imageSharingMode(SharingMode imageSharingMode){ m_swapchainCreateInfoKHR.imageSharingMode = static_cast<VkSharingMode>( imageSharingMode ); return *this; }
    const uint32_t& queueFamilyIndexCount() const { return m_swapchainCreateInfoKHR.queueFamilyIndexCount; }
    SwapchainCreateInfoKHR& queueFamilyIndexCount(uint32_t queueFamilyIndexCount){ m_swapchainCreateInfoKHR.queueFamilyIndexCount = queueFamilyIndexCount; return *this; }
    const uint32_t* pQueueFamilyIndices() const { return reinterpret_cast<const uint32_t*>( m_swapchainCreateInfoKHR.pQueueFamilyIndices ); }
    SwapchainCreateInfoKHR& pQueueFamilyIndices(const uint32_t* pQueueFamilyIndices){ m_swapchainCreateInfoKHR.pQueueFamilyIndices = pQueueFamilyIndices; return *this; }
    const SurfaceTransformFlagBitsKHR& preTransform() const { return reinterpret_cast<const SurfaceTransformFlagBitsKHR&>( m_swapchainCreateInfoKHR.preTransform ); }
    SwapchainCreateInfoKHR& preTransform(SurfaceTransformFlagBitsKHR preTransform){ m_swapchainCreateInfoKHR.preTransform = static_cast<VkSurfaceTransformFlagBitsKHR>( preTransform ); return *this; }
    const CompositeAlphaFlagBitsKHR& compositeAlpha() const { return reinterpret_cast<const CompositeAlphaFlagBitsKHR&>( m_swapchainCreateInfoKHR.compositeAlpha ); }
    SwapchainCreateInfoKHR& compositeAlpha(CompositeAlphaFlagBitsKHR compositeAlpha){ m_swapchainCreateInfoKHR.compositeAlpha = static_cast<VkCompositeAlphaFlagBitsKHR>( compositeAlpha ); return *this; }
    const PresentModeKHR& presentMode() const { return reinterpret_cast<const PresentModeKHR&>( m_swapchainCreateInfoKHR.presentMode ); }
    SwapchainCreateInfoKHR& presentMode(PresentModeKHR presentMode){ m_swapchainCreateInfoKHR.presentMode = static_cast<VkPresentModeKHR>( presentMode ); return *this; }
    const Bool32& clipped() const { return m_swapchainCreateInfoKHR.clipped; }
    SwapchainCreateInfoKHR& clipped(Bool32 clipped){ m_swapchainCreateInfoKHR.clipped = clipped; return *this; }
    const SwapchainKHR& oldSwapchain() const { return m_swapchainCreateInfoKHR.oldSwapchain; }
    SwapchainCreateInfoKHR& oldSwapchain(SwapchainKHR oldSwapchain){ m_swapchainCreateInfoKHR.oldSwapchain = oldSwapchain; return *this; }
    operator VkSwapchainCreateInfoKHR&() { return m_swapchainCreateInfoKHR; }
    operator const VkSwapchainCreateInfoKHR&() const { return m_swapchainCreateInfoKHR; }
    VkSwapchainCreateInfoKHR& vkHandle() { return m_swapchainCreateInfoKHR; }
    const VkSwapchainCreateInfoKHR& vkHandle() const { return m_swapchainCreateInfoKHR; }

  private:
    VkSwapchainCreateInfoKHR m_swapchainCreateInfoKHR;
  };

  class DebugReportCallbackCreateInfoEXT
  {
  public:
    DebugReportCallbackCreateInfoEXT(DebugReportFlagsEXT flags, PFN_vkDebugReportCallbackEXT pfnCallback, void* pUserData) {m_debugReportCallbackCreateInfoEXT.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT; m_debugReportCallbackCreateInfoEXT.pNext = nullptr; m_debugReportCallbackCreateInfoEXT.flags = static_cast<VkDebugReportFlagsEXT>( flags ); m_debugReportCallbackCreateInfoEXT.pfnCallback = pfnCallback; m_debugReportCallbackCreateInfoEXT.pUserData = pUserData; }
    DebugReportCallbackCreateInfoEXT(const VkDebugReportCallbackCreateInfoEXT& other) : m_debugReportCallbackCreateInfoEXT{other} {}
    DebugReportCallbackCreateInfoEXT& operator=(const VkDebugReportCallbackCreateInfoEXT& other){ m_debugReportCallbackCreateInfoEXT = other; return *this; }

    const StructureType& sType() const { return reinterpret_cast<const StructureType&>( m_debugReportCallbackCreateInfoEXT.sType ); }
    DebugReportCallbackCreateInfoEXT& sType(StructureType sType){ m_debugReportCallbackCreateInfoEXT.sType = static_cast<VkStructureType>( sType ); return *this; }
    const void* pNext() const { return reinterpret_cast<const void*>( m_debugReportCallbackCreateInfoEXT.pNext ); }
    DebugReportCallbackCreateInfoEXT& pNext(const void* pNext){ m_debugReportCallbackCreateInfoEXT.pNext = pNext; return *this; }
    const DebugReportFlagsEXT& flags() const { return reinterpret_cast<const DebugReportFlagsEXT&>( m_debugReportCallbackCreateInfoEXT.flags ); }
    DebugReportCallbackCreateInfoEXT& flags(DebugReportFlagsEXT flags){ m_debugReportCallbackCreateInfoEXT.flags = static_cast<VkDebugReportFlagsEXT>( flags ); return *this; }
    const PFN_vkDebugReportCallbackEXT& pfnCallback() const { return m_debugReportCallbackCreateInfoEXT.pfnCallback; }
    DebugReportCallbackCreateInfoEXT& pfnCallback(PFN_vkDebugReportCallbackEXT pfnCallback){ m_debugReportCallbackCreateInfoEXT.pfnCallback = pfnCallback; return *this; }
    const void* pUserData() const { return reinterpret_cast<void*>( m_debugReportCallbackCreateInfoEXT.pUserData ); }
    DebugReportCallbackCreateInfoEXT& pUserData(void* pUserData){ m_debugReportCallbackCreateInfoEXT.pUserData = pUserData; return *this; }
    operator VkDebugReportCallbackCreateInfoEXT&() { return m_debugReportCallbackCreateInfoEXT; }
    operator const VkDebugReportCallbackCreateInfoEXT&() const { return m_debugReportCallbackCreateInfoEXT; }
    VkDebugReportCallbackCreateInfoEXT& vkHandle() { return m_debugReportCallbackCreateInfoEXT; }
    const VkDebugReportCallbackCreateInfoEXT& vkHandle() const { return m_debugReportCallbackCreateInfoEXT; }

  private:
    VkDebugReportCallbackCreateInfoEXT m_debugReportCallbackCreateInfoEXT;
  };

  class DisplayPlanePropertiesKHR
  {
  public:
    DisplayPlanePropertiesKHR() : DisplayPlanePropertiesKHR(DisplayKHR(), 0) {}
    DisplayPlanePropertiesKHR(DisplayKHR currentDisplay, uint32_t currentStackIndex) {m_displayPlanePropertiesKHR.currentDisplay = currentDisplay; m_displayPlanePropertiesKHR.currentStackIndex = currentStackIndex; }
    DisplayPlanePropertiesKHR(const VkDisplayPlanePropertiesKHR& other) : m_displayPlanePropertiesKHR{other} {}
    DisplayPlanePropertiesKHR& operator=(const VkDisplayPlanePropertiesKHR& other){ m_displayPlanePropertiesKHR = other; return *this; }

    const DisplayKHR& currentDisplay() const { return m_displayPlanePropertiesKHR.currentDisplay; }
    DisplayPlanePropertiesKHR& currentDisplay(DisplayKHR currentDisplay){ m_displayPlanePropertiesKHR.currentDisplay = currentDisplay; return *this; }
    const uint32_t& currentStackIndex() const { return m_displayPlanePropertiesKHR.currentStackIndex; }
    DisplayPlanePropertiesKHR& currentStackIndex(uint32_t currentStackIndex){ m_displayPlanePropertiesKHR.currentStackIndex = currentStackIndex; return *this; }
    operator VkDisplayPlanePropertiesKHR&() { return m_displayPlanePropertiesKHR; }
    operator const VkDisplayPlanePropertiesKHR&() const { return m_displayPlanePropertiesKHR; }
    VkDisplayPlanePropertiesKHR& vkHandle() { return m_displayPlanePropertiesKHR; }
    const VkDisplayPlanePropertiesKHR& vkHandle() const { return m_displayPlanePropertiesKHR; }

  private:
    VkDisplayPlanePropertiesKHR m_displayPlanePropertiesKHR;
  };

  class DisplayModeParametersKHR
  {
  public:
    DisplayModeParametersKHR() : DisplayModeParametersKHR(Extent2D(), 0) {}
    DisplayModeParametersKHR(Extent2D visibleRegion, uint32_t refreshRate) {m_displayModeParametersKHR.visibleRegion = static_cast<VkExtent2D>( visibleRegion ); m_displayModeParametersKHR.refreshRate = refreshRate; }
    DisplayModeParametersKHR(const VkDisplayModeParametersKHR& other) : m_displayModeParametersKHR{other} {}
    DisplayModeParametersKHR& operator=(const VkDisplayModeParametersKHR& other){ m_displayModeParametersKHR = other; return *this; }

    const Extent2D& visibleRegion() const { return reinterpret_cast<const Extent2D&>( m_displayModeParametersKHR.visibleRegion ); }
    DisplayModeParametersKHR& visibleRegion(Extent2D visibleRegion){ m_displayModeParametersKHR.visibleRegion = static_cast<VkExtent2D>( visibleRegion ); return *this; }
    const uint32_t& refreshRate() const { return m_displayModeParametersKHR.refreshRate; }
    DisplayModeParametersKHR& refreshRate(uint32_t refreshRate){ m_displayModeParametersKHR.refreshRate = refreshRate; return *this; }
    operator VkDisplayModeParametersKHR&() { return m_displayModeParametersKHR; }
    operator const VkDisplayModeParametersKHR&() const { return m_displayModeParametersKHR; }
    VkDisplayModeParametersKHR& vkHandle() { return m_displayModeParametersKHR; }
    const VkDisplayModeParametersKHR& vkHandle() const { return m_displayModeParametersKHR; }

  private:
    VkDisplayModeParametersKHR m_displayModeParametersKHR;
  };

  class DisplayModePropertiesKHR
  {
  public:
    DisplayModePropertiesKHR() : DisplayModePropertiesKHR(DisplayModeKHR(), DisplayModeParametersKHR()) {}
    DisplayModePropertiesKHR(DisplayModeKHR displayMode, DisplayModeParametersKHR parameters) {m_displayModePropertiesKHR.displayMode = displayMode; m_displayModePropertiesKHR.parameters = static_cast<VkDisplayModeParametersKHR>( parameters ); }
    DisplayModePropertiesKHR(const VkDisplayModePropertiesKHR& other) : m_displayModePropertiesKHR{other} {}
    DisplayModePropertiesKHR& operator=(const VkDisplayModePropertiesKHR& other){ m_displayModePropertiesKHR = other; return *this; }

    const DisplayModeKHR& displayMode() const { return m_displayModePropertiesKHR.displayMode; }
    DisplayModePropertiesKHR& displayMode(DisplayModeKHR displayMode){ m_displayModePropertiesKHR.displayMode = displayMode; return *this; }
    const DisplayModeParametersKHR& parameters() const { return reinterpret_cast<const DisplayModeParametersKHR&>( m_displayModePropertiesKHR.parameters ); }
    DisplayModePropertiesKHR& parameters(DisplayModeParametersKHR parameters){ m_displayModePropertiesKHR.parameters = static_cast<VkDisplayModeParametersKHR>( parameters ); return *this; }
    operator VkDisplayModePropertiesKHR&() { return m_displayModePropertiesKHR; }
    operator const VkDisplayModePropertiesKHR&() const { return m_displayModePropertiesKHR; }
    VkDisplayModePropertiesKHR& vkHandle() { return m_displayModePropertiesKHR; }
    const VkDisplayModePropertiesKHR& vkHandle() const { return m_displayModePropertiesKHR; }

  private:
    VkDisplayModePropertiesKHR m_displayModePropertiesKHR;
  };

  class DisplayModeCreateInfoKHR
  {
  public:
    DisplayModeCreateInfoKHR() : DisplayModeCreateInfoKHR(DisplayModeCreateFlagsKHR(), DisplayModeParametersKHR()) {}
    DisplayModeCreateInfoKHR(DisplayModeCreateFlagsKHR flags, DisplayModeParametersKHR parameters) {m_displayModeCreateInfoKHR.sType = VK_STRUCTURE_TYPE_DISPLAY_MODE_CREATE_INFO_KHR; m_displayModeCreateInfoKHR.pNext = nullptr; m_displayModeCreateInfoKHR.flags = static_cast<VkDisplayModeCreateFlagsKHR>( flags ); m_displayModeCreateInfoKHR.parameters = static_cast<VkDisplayModeParametersKHR>( parameters ); }
    DisplayModeCreateInfoKHR(const VkDisplayModeCreateInfoKHR& other) : m_displayModeCreateInfoKHR{other} {}
    DisplayModeCreateInfoKHR& operator=(const VkDisplayModeCreateInfoKHR& other){ m_displayModeCreateInfoKHR = other; return *this; }

    const StructureType& sType() const { return reinterpret_cast<const StructureType&>( m_displayModeCreateInfoKHR.sType ); }
    DisplayModeCreateInfoKHR& sType(StructureType sType){ m_displayModeCreateInfoKHR.sType = static_cast<VkStructureType>( sType ); return *this; }
    const void* pNext() const { return reinterpret_cast<const void*>( m_displayModeCreateInfoKHR.pNext ); }
    DisplayModeCreateInfoKHR& pNext(const void* pNext){ m_displayModeCreateInfoKHR.pNext = pNext; return *this; }
    const DisplayModeCreateFlagsKHR& flags() const { return reinterpret_cast<const DisplayModeCreateFlagsKHR&>( m_displayModeCreateInfoKHR.flags ); }
    DisplayModeCreateInfoKHR& flags(DisplayModeCreateFlagsKHR flags){ m_displayModeCreateInfoKHR.flags = static_cast<VkDisplayModeCreateFlagsKHR>( flags ); return *this; }
    const DisplayModeParametersKHR& parameters() const { return reinterpret_cast<const DisplayModeParametersKHR&>( m_displayModeCreateInfoKHR.parameters ); }
    DisplayModeCreateInfoKHR& parameters(DisplayModeParametersKHR parameters){ m_displayModeCreateInfoKHR.parameters = static_cast<VkDisplayModeParametersKHR>( parameters ); return *this; }
    operator VkDisplayModeCreateInfoKHR&() { return m_displayModeCreateInfoKHR; }
    operator const VkDisplayModeCreateInfoKHR&() const { return m_displayModeCreateInfoKHR; }
    VkDisplayModeCreateInfoKHR& vkHandle() { return m_displayModeCreateInfoKHR; }
    const VkDisplayModeCreateInfoKHR& vkHandle() const { return m_displayModeCreateInfoKHR; }

  private:
    VkDisplayModeCreateInfoKHR m_displayModeCreateInfoKHR;
  };

  class DisplayPlaneCapabilitiesKHR
  {
  public:
    DisplayPlaneCapabilitiesKHR() : DisplayPlaneCapabilitiesKHR(DisplayPlaneAlphaFlagsKHR(), Offset2D(), Offset2D(), Extent2D(), Extent2D(), Offset2D(), Offset2D(), Extent2D(), Extent2D()) {}
    DisplayPlaneCapabilitiesKHR(DisplayPlaneAlphaFlagsKHR supportedAlpha, Offset2D minSrcPosition, Offset2D maxSrcPosition, Extent2D minSrcExtent, Extent2D maxSrcExtent, Offset2D minDstPosition, Offset2D maxDstPosition, Extent2D minDstExtent, Extent2D maxDstExtent) {m_displayPlaneCapabilitiesKHR.supportedAlpha = static_cast<VkDisplayPlaneAlphaFlagsKHR>( supportedAlpha ); m_displayPlaneCapabilitiesKHR.minSrcPosition = static_cast<VkOffset2D>( minSrcPosition ); m_displayPlaneCapabilitiesKHR.maxSrcPosition = static_cast<VkOffset2D>( maxSrcPosition ); m_displayPlaneCapabilitiesKHR.minSrcExtent = static_cast<VkExtent2D>( minSrcExtent ); m_displayPlaneCapabilitiesKHR.maxSrcExtent = static_cast<VkExtent2D>( maxSrcExtent ); m_displayPlaneCapabilitiesKHR.minDstPosition = static_cast<VkOffset2D>( minDstPosition ); m_displayPlaneCapabilitiesKHR.maxDstPosition = static_cast<VkOffset2D>( maxDstPosition ); m_displayPlaneCapabilitiesKHR.minDstExtent = static_cast<VkExtent2D>( minDstExtent ); m_displayPlaneCapabilitiesKHR.maxDstExtent = static_cast<VkExtent2D>( maxDstExtent ); }
    DisplayPlaneCapabilitiesKHR(const VkDisplayPlaneCapabilitiesKHR& other) : m_displayPlaneCapabilitiesKHR{other} {}
    DisplayPlaneCapabilitiesKHR& operator=(const VkDisplayPlaneCapabilitiesKHR& other){ m_displayPlaneCapabilitiesKHR = other; return *this; }

    const DisplayPlaneAlphaFlagsKHR& supportedAlpha() const { return reinterpret_cast<const DisplayPlaneAlphaFlagsKHR&>( m_displayPlaneCapabilitiesKHR.supportedAlpha ); }
    DisplayPlaneCapabilitiesKHR& supportedAlpha(DisplayPlaneAlphaFlagsKHR supportedAlpha){ m_displayPlaneCapabilitiesKHR.supportedAlpha = static_cast<VkDisplayPlaneAlphaFlagsKHR>( supportedAlpha ); return *this; }
    const Offset2D& minSrcPosition() const { return reinterpret_cast<const Offset2D&>( m_displayPlaneCapabilitiesKHR.minSrcPosition ); }
    DisplayPlaneCapabilitiesKHR& minSrcPosition(Offset2D minSrcPosition){ m_displayPlaneCapabilitiesKHR.minSrcPosition = static_cast<VkOffset2D>( minSrcPosition ); return *this; }
    const Offset2D& maxSrcPosition() const { return reinterpret_cast<const Offset2D&>( m_displayPlaneCapabilitiesKHR.maxSrcPosition ); }
    DisplayPlaneCapabilitiesKHR& maxSrcPosition(Offset2D maxSrcPosition){ m_displayPlaneCapabilitiesKHR.maxSrcPosition = static_cast<VkOffset2D>( maxSrcPosition ); return *this; }
    const Extent2D& minSrcExtent() const { return reinterpret_cast<const Extent2D&>( m_displayPlaneCapabilitiesKHR.minSrcExtent ); }
    DisplayPlaneCapabilitiesKHR& minSrcExtent(Extent2D minSrcExtent){ m_displayPlaneCapabilitiesKHR.minSrcExtent = static_cast<VkExtent2D>( minSrcExtent ); return *this; }
    const Extent2D& maxSrcExtent() const { return reinterpret_cast<const Extent2D&>( m_displayPlaneCapabilitiesKHR.maxSrcExtent ); }
    DisplayPlaneCapabilitiesKHR& maxSrcExtent(Extent2D maxSrcExtent){ m_displayPlaneCapabilitiesKHR.maxSrcExtent = static_cast<VkExtent2D>( maxSrcExtent ); return *this; }
    const Offset2D& minDstPosition() const { return reinterpret_cast<const Offset2D&>( m_displayPlaneCapabilitiesKHR.minDstPosition ); }
    DisplayPlaneCapabilitiesKHR& minDstPosition(Offset2D minDstPosition){ m_displayPlaneCapabilitiesKHR.minDstPosition = static_cast<VkOffset2D>( minDstPosition ); return *this; }
    const Offset2D& maxDstPosition() const { return reinterpret_cast<const Offset2D&>( m_displayPlaneCapabilitiesKHR.maxDstPosition ); }
    DisplayPlaneCapabilitiesKHR& maxDstPosition(Offset2D maxDstPosition){ m_displayPlaneCapabilitiesKHR.maxDstPosition = static_cast<VkOffset2D>( maxDstPosition ); return *this; }
    const Extent2D& minDstExtent() const { return reinterpret_cast<const Extent2D&>( m_displayPlaneCapabilitiesKHR.minDstExtent ); }
    DisplayPlaneCapabilitiesKHR& minDstExtent(Extent2D minDstExtent){ m_displayPlaneCapabilitiesKHR.minDstExtent = static_cast<VkExtent2D>( minDstExtent ); return *this; }
    const Extent2D& maxDstExtent() const { return reinterpret_cast<const Extent2D&>( m_displayPlaneCapabilitiesKHR.maxDstExtent ); }
    DisplayPlaneCapabilitiesKHR& maxDstExtent(Extent2D maxDstExtent){ m_displayPlaneCapabilitiesKHR.maxDstExtent = static_cast<VkExtent2D>( maxDstExtent ); return *this; }
    operator VkDisplayPlaneCapabilitiesKHR&() { return m_displayPlaneCapabilitiesKHR; }
    operator const VkDisplayPlaneCapabilitiesKHR&() const { return m_displayPlaneCapabilitiesKHR; }
    VkDisplayPlaneCapabilitiesKHR& vkHandle() { return m_displayPlaneCapabilitiesKHR; }
    const VkDisplayPlaneCapabilitiesKHR& vkHandle() const { return m_displayPlaneCapabilitiesKHR; }

  private:
    VkDisplayPlaneCapabilitiesKHR m_displayPlaneCapabilitiesKHR;
  };

  class DisplayPropertiesKHR
  {
  public:
    DisplayPropertiesKHR() : DisplayPropertiesKHR(DisplayKHR(), nullptr, Extent2D(), Extent2D(), SurfaceTransformFlagsKHR(), 0, 0) {}
    DisplayPropertiesKHR(DisplayKHR display, const char* displayName, Extent2D physicalDimensions, Extent2D physicalResolution, SurfaceTransformFlagsKHR supportedTransforms, Bool32 planeReorderPossible, Bool32 persistentContent) {m_displayPropertiesKHR.display = display; m_displayPropertiesKHR.displayName = displayName; m_displayPropertiesKHR.physicalDimensions = static_cast<VkExtent2D>( physicalDimensions ); m_displayPropertiesKHR.physicalResolution = static_cast<VkExtent2D>( physicalResolution ); m_displayPropertiesKHR.supportedTransforms = static_cast<VkSurfaceTransformFlagsKHR>( supportedTransforms ); m_displayPropertiesKHR.planeReorderPossible = planeReorderPossible; m_displayPropertiesKHR.persistentContent = persistentContent; }
    DisplayPropertiesKHR(const VkDisplayPropertiesKHR& other) : m_displayPropertiesKHR{other} {}
    DisplayPropertiesKHR& operator=(const VkDisplayPropertiesKHR& other){ m_displayPropertiesKHR = other; return *this; }

    const DisplayKHR& display() const { return m_displayPropertiesKHR.display; }
    DisplayPropertiesKHR& display(DisplayKHR display){ m_displayPropertiesKHR.display = display; return *this; }
    const char* displayName() const { return reinterpret_cast<const char*>( m_displayPropertiesKHR.displayName ); }
    DisplayPropertiesKHR& displayName(const char* displayName){ m_displayPropertiesKHR.displayName = displayName; return *this; }
    const Extent2D& physicalDimensions() const { return reinterpret_cast<const Extent2D&>( m_displayPropertiesKHR.physicalDimensions ); }
    DisplayPropertiesKHR& physicalDimensions(Extent2D physicalDimensions){ m_displayPropertiesKHR.physicalDimensions = static_cast<VkExtent2D>( physicalDimensions ); return *this; }
    const Extent2D& physicalResolution() const { return reinterpret_cast<const Extent2D&>( m_displayPropertiesKHR.physicalResolution ); }
    DisplayPropertiesKHR& physicalResolution(Extent2D physicalResolution){ m_displayPropertiesKHR.physicalResolution = static_cast<VkExtent2D>( physicalResolution ); return *this; }
    const SurfaceTransformFlagsKHR& supportedTransforms() const { return reinterpret_cast<const SurfaceTransformFlagsKHR&>( m_displayPropertiesKHR.supportedTransforms ); }
    DisplayPropertiesKHR& supportedTransforms(SurfaceTransformFlagsKHR supportedTransforms){ m_displayPropertiesKHR.supportedTransforms = static_cast<VkSurfaceTransformFlagsKHR>( supportedTransforms ); return *this; }
    const Bool32& planeReorderPossible() const { return m_displayPropertiesKHR.planeReorderPossible; }
    DisplayPropertiesKHR& planeReorderPossible(Bool32 planeReorderPossible){ m_displayPropertiesKHR.planeReorderPossible = planeReorderPossible; return *this; }
    const Bool32& persistentContent() const { return m_displayPropertiesKHR.persistentContent; }
    DisplayPropertiesKHR& persistentContent(Bool32 persistentContent){ m_displayPropertiesKHR.persistentContent = persistentContent; return *this; }
    operator VkDisplayPropertiesKHR&() { return m_displayPropertiesKHR; }
    operator const VkDisplayPropertiesKHR&() const { return m_displayPropertiesKHR; }
    VkDisplayPropertiesKHR& vkHandle() { return m_displayPropertiesKHR; }
    const VkDisplayPropertiesKHR& vkHandle() const { return m_displayPropertiesKHR; }

  private:
    VkDisplayPropertiesKHR m_displayPropertiesKHR;
  };

  class DisplaySurfaceCreateInfoKHR
  {
  public:
    DisplaySurfaceCreateInfoKHR() : DisplaySurfaceCreateInfoKHR(DisplaySurfaceCreateFlagsKHR(), DisplayModeKHR(), 0, 0, SurfaceTransformFlagBitsKHR::IdentityKHR, 0, DisplayPlaneAlphaFlagBitsKHR::OpaqueKHR, Extent2D()) {}
    DisplaySurfaceCreateInfoKHR(DisplaySurfaceCreateFlagsKHR flags, DisplayModeKHR displayMode, uint32_t planeIndex, uint32_t planeStackIndex, SurfaceTransformFlagBitsKHR transform, float globalAlpha, DisplayPlaneAlphaFlagBitsKHR alphaMode, Extent2D imageExtent) {m_displaySurfaceCreateInfoKHR.sType = VK_STRUCTURE_TYPE_DISPLAY_SURFACE_CREATE_INFO_KHR; m_displaySurfaceCreateInfoKHR.pNext = nullptr; m_displaySurfaceCreateInfoKHR.flags = static_cast<VkDisplaySurfaceCreateFlagsKHR>( flags ); m_displaySurfaceCreateInfoKHR.displayMode = displayMode; m_displaySurfaceCreateInfoKHR.planeIndex = planeIndex; m_displaySurfaceCreateInfoKHR.planeStackIndex = planeStackIndex; m_displaySurfaceCreateInfoKHR.transform = static_cast<VkSurfaceTransformFlagBitsKHR>( transform ); m_displaySurfaceCreateInfoKHR.globalAlpha = globalAlpha; m_displaySurfaceCreateInfoKHR.alphaMode = static_cast<VkDisplayPlaneAlphaFlagBitsKHR>( alphaMode ); m_displaySurfaceCreateInfoKHR.imageExtent = static_cast<VkExtent2D>( imageExtent ); }
    DisplaySurfaceCreateInfoKHR(const VkDisplaySurfaceCreateInfoKHR& other) : m_displaySurfaceCreateInfoKHR{other} {}
    DisplaySurfaceCreateInfoKHR& operator=(const VkDisplaySurfaceCreateInfoKHR& other){ m_displaySurfaceCreateInfoKHR = other; return *this; }

    const StructureType& sType() const { return reinterpret_cast<const StructureType&>( m_displaySurfaceCreateInfoKHR.sType ); }
    DisplaySurfaceCreateInfoKHR& sType(StructureType sType){ m_displaySurfaceCreateInfoKHR.sType = static_cast<VkStructureType>( sType ); return *this; }
    const void* pNext() const { return reinterpret_cast<const void*>( m_displaySurfaceCreateInfoKHR.pNext ); }
    DisplaySurfaceCreateInfoKHR& pNext(const void* pNext){ m_displaySurfaceCreateInfoKHR.pNext = pNext; return *this; }
    const DisplaySurfaceCreateFlagsKHR& flags() const { return reinterpret_cast<const DisplaySurfaceCreateFlagsKHR&>( m_displaySurfaceCreateInfoKHR.flags ); }
    DisplaySurfaceCreateInfoKHR& flags(DisplaySurfaceCreateFlagsKHR flags){ m_displaySurfaceCreateInfoKHR.flags = static_cast<VkDisplaySurfaceCreateFlagsKHR>( flags ); return *this; }
    const DisplayModeKHR& displayMode() const { return m_displaySurfaceCreateInfoKHR.displayMode; }
    DisplaySurfaceCreateInfoKHR& displayMode(DisplayModeKHR displayMode){ m_displaySurfaceCreateInfoKHR.displayMode = displayMode; return *this; }
    const uint32_t& planeIndex() const { return m_displaySurfaceCreateInfoKHR.planeIndex; }
    DisplaySurfaceCreateInfoKHR& planeIndex(uint32_t planeIndex){ m_displaySurfaceCreateInfoKHR.planeIndex = planeIndex; return *this; }
    const uint32_t& planeStackIndex() const { return m_displaySurfaceCreateInfoKHR.planeStackIndex; }
    DisplaySurfaceCreateInfoKHR& planeStackIndex(uint32_t planeStackIndex){ m_displaySurfaceCreateInfoKHR.planeStackIndex = planeStackIndex; return *this; }
    const SurfaceTransformFlagBitsKHR& transform() const { return reinterpret_cast<const SurfaceTransformFlagBitsKHR&>( m_displaySurfaceCreateInfoKHR.transform ); }
    DisplaySurfaceCreateInfoKHR& transform(SurfaceTransformFlagBitsKHR transform){ m_displaySurfaceCreateInfoKHR.transform = static_cast<VkSurfaceTransformFlagBitsKHR>( transform ); return *this; }
    const float& globalAlpha() const { return m_displaySurfaceCreateInfoKHR.globalAlpha; }
    DisplaySurfaceCreateInfoKHR& globalAlpha(float globalAlpha){ m_displaySurfaceCreateInfoKHR.globalAlpha = globalAlpha; return *this; }
    const DisplayPlaneAlphaFlagBitsKHR& alphaMode() const { return reinterpret_cast<const DisplayPlaneAlphaFlagBitsKHR&>( m_displaySurfaceCreateInfoKHR.alphaMode ); }
    DisplaySurfaceCreateInfoKHR& alphaMode(DisplayPlaneAlphaFlagBitsKHR alphaMode){ m_displaySurfaceCreateInfoKHR.alphaMode = static_cast<VkDisplayPlaneAlphaFlagBitsKHR>( alphaMode ); return *this; }
    const Extent2D& imageExtent() const { return reinterpret_cast<const Extent2D&>( m_displaySurfaceCreateInfoKHR.imageExtent ); }
    DisplaySurfaceCreateInfoKHR& imageExtent(Extent2D imageExtent){ m_displaySurfaceCreateInfoKHR.imageExtent = static_cast<VkExtent2D>( imageExtent ); return *this; }
    operator VkDisplaySurfaceCreateInfoKHR&() { return m_displaySurfaceCreateInfoKHR; }
    operator const VkDisplaySurfaceCreateInfoKHR&() const { return m_displaySurfaceCreateInfoKHR; }
    VkDisplaySurfaceCreateInfoKHR& vkHandle() { return m_displaySurfaceCreateInfoKHR; }
    const VkDisplaySurfaceCreateInfoKHR& vkHandle() const { return m_displaySurfaceCreateInfoKHR; }

  private:
    VkDisplaySurfaceCreateInfoKHR m_displaySurfaceCreateInfoKHR;
  };

  class DisplayPresentInfoKHR
  {
  public:
    DisplayPresentInfoKHR() : DisplayPresentInfoKHR(Rect2D(), Rect2D(), 0) {}
    DisplayPresentInfoKHR(Rect2D srcRect, Rect2D dstRect, Bool32 persistent) {m_displayPresentInfoKHR.sType = VK_STRUCTURE_TYPE_DISPLAY_PRESENT_INFO_KHR; m_displayPresentInfoKHR.pNext = nullptr; m_displayPresentInfoKHR.srcRect = static_cast<VkRect2D>( srcRect ); m_displayPresentInfoKHR.dstRect = static_cast<VkRect2D>( dstRect ); m_displayPresentInfoKHR.persistent = persistent; }
    DisplayPresentInfoKHR(const VkDisplayPresentInfoKHR& other) : m_displayPresentInfoKHR{other} {}
    DisplayPresentInfoKHR& operator=(const VkDisplayPresentInfoKHR& other){ m_displayPresentInfoKHR = other; return *this; }

    const StructureType& sType() const { return reinterpret_cast<const StructureType&>( m_displayPresentInfoKHR.sType ); }
    DisplayPresentInfoKHR& sType(StructureType sType){ m_displayPresentInfoKHR.sType = static_cast<VkStructureType>( sType ); return *this; }
    const void* pNext() const { return reinterpret_cast<const void*>( m_displayPresentInfoKHR.pNext ); }
    DisplayPresentInfoKHR& pNext(const void* pNext){ m_displayPresentInfoKHR.pNext = pNext; return *this; }
    const Rect2D& srcRect() const { return reinterpret_cast<const Rect2D&>( m_displayPresentInfoKHR.srcRect ); }
    DisplayPresentInfoKHR& srcRect(Rect2D srcRect){ m_displayPresentInfoKHR.srcRect = static_cast<VkRect2D>( srcRect ); return *this; }
    const Rect2D& dstRect() const { return reinterpret_cast<const Rect2D&>( m_displayPresentInfoKHR.dstRect ); }
    DisplayPresentInfoKHR& dstRect(Rect2D dstRect){ m_displayPresentInfoKHR.dstRect = static_cast<VkRect2D>( dstRect ); return *this; }
    const Bool32& persistent() const { return m_displayPresentInfoKHR.persistent; }
    DisplayPresentInfoKHR& persistent(Bool32 persistent){ m_displayPresentInfoKHR.persistent = persistent; return *this; }
    operator VkDisplayPresentInfoKHR&() { return m_displayPresentInfoKHR; }
    operator const VkDisplayPresentInfoKHR&() const { return m_displayPresentInfoKHR; }
    VkDisplayPresentInfoKHR& vkHandle() { return m_displayPresentInfoKHR; }
    const VkDisplayPresentInfoKHR& vkHandle() const { return m_displayPresentInfoKHR; }

  private:
    VkDisplayPresentInfoKHR m_displayPresentInfoKHR;
  };

#ifdef VK_USE_PLATFORM_XLIB_KHR

  class XlibSurfaceCreateInfoKHR
  {
  public:
    XlibSurfaceCreateInfoKHR() : XlibSurfaceCreateInfoKHR(XlibSurfaceCreateFlagsKHR(), nullptr, 0) {}
    XlibSurfaceCreateInfoKHR(XlibSurfaceCreateFlagsKHR flags, Display* dpy, Window window) {m_xlibSurfaceCreateInfoKHR.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR; m_xlibSurfaceCreateInfoKHR.pNext = nullptr; m_xlibSurfaceCreateInfoKHR.flags = static_cast<VkXlibSurfaceCreateFlagsKHR>( flags ); m_xlibSurfaceCreateInfoKHR.dpy = dpy; m_xlibSurfaceCreateInfoKHR.window = window; }
    XlibSurfaceCreateInfoKHR(const VkXlibSurfaceCreateInfoKHR& other) : m_xlibSurfaceCreateInfoKHR{other} {}
    XlibSurfaceCreateInfoKHR& operator=(const VkXlibSurfaceCreateInfoKHR& other){ m_xlibSurfaceCreateInfoKHR = other; return *this; }

    const StructureType& sType() const { return reinterpret_cast<const StructureType&>( m_xlibSurfaceCreateInfoKHR.sType ); }
    XlibSurfaceCreateInfoKHR& sType(StructureType sType){ m_xlibSurfaceCreateInfoKHR.sType = static_cast<VkStructureType>( sType ); return *this; }
    const void* pNext() const { return reinterpret_cast<const void*>( m_xlibSurfaceCreateInfoKHR.pNext ); }
    XlibSurfaceCreateInfoKHR& pNext(const void* pNext){ m_xlibSurfaceCreateInfoKHR.pNext = pNext; return *this; }
    const XlibSurfaceCreateFlagsKHR& flags() const { return reinterpret_cast<const XlibSurfaceCreateFlagsKHR&>( m_xlibSurfaceCreateInfoKHR.flags ); }
    XlibSurfaceCreateInfoKHR& flags(XlibSurfaceCreateFlagsKHR flags){ m_xlibSurfaceCreateInfoKHR.flags = static_cast<VkXlibSurfaceCreateFlagsKHR>( flags ); return *this; }
    const Display* dpy() const { return reinterpret_cast<Display*>( m_xlibSurfaceCreateInfoKHR.dpy ); }
    XlibSurfaceCreateInfoKHR& dpy(Display* dpy){ m_xlibSurfaceCreateInfoKHR.dpy = dpy; return *this; }
    const Window& window() const { return m_xlibSurfaceCreateInfoKHR.window; }
    XlibSurfaceCreateInfoKHR& window(Window window){ m_xlibSurfaceCreateInfoKHR.window = window; return *this; }
    operator VkXlibSurfaceCreateInfoKHR&() { return m_xlibSurfaceCreateInfoKHR; }
    operator const VkXlibSurfaceCreateInfoKHR&() const { return m_xlibSurfaceCreateInfoKHR; }
    VkXlibSurfaceCreateInfoKHR& vkHandle() { return m_xlibSurfaceCreateInfoKHR; }
    const VkXlibSurfaceCreateInfoKHR& vkHandle() const { return m_xlibSurfaceCreateInfoKHR; }

  private:
    VkXlibSurfaceCreateInfoKHR m_xlibSurfaceCreateInfoKHR;
  };

#endif /* VK_USE_PLATFORM_XLIB_KHR */

#ifdef VK_USE_PLATFORM_XCB_KHR

  class XcbSurfaceCreateInfoKHR
  {
  public:
    XcbSurfaceCreateInfoKHR() : XcbSurfaceCreateInfoKHR(XcbSurfaceCreateFlagsKHR(), nullptr, 0) {}
    XcbSurfaceCreateInfoKHR(XcbSurfaceCreateFlagsKHR flags, xcb_connection_t* connection, xcb_window_t window) {m_xcbSurfaceCreateInfoKHR.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR; m_xcbSurfaceCreateInfoKHR.pNext = nullptr; m_xcbSurfaceCreateInfoKHR.flags = static_cast<VkXcbSurfaceCreateFlagsKHR>( flags ); m_xcbSurfaceCreateInfoKHR.connection = connection; m_xcbSurfaceCreateInfoKHR.window = window; }
    XcbSurfaceCreateInfoKHR(const VkXcbSurfaceCreateInfoKHR& other) : m_xcbSurfaceCreateInfoKHR{other} {}
    XcbSurfaceCreateInfoKHR& operator=(const VkXcbSurfaceCreateInfoKHR& other){ m_xcbSurfaceCreateInfoKHR = other; return *this; }

    const StructureType& sType() const { return reinterpret_cast<const StructureType&>( m_xcbSurfaceCreateInfoKHR.sType ); }
    XcbSurfaceCreateInfoKHR& sType(StructureType sType){ m_xcbSurfaceCreateInfoKHR.sType = static_cast<VkStructureType>( sType ); return *this; }
    const void* pNext() const { return reinterpret_cast<const void*>( m_xcbSurfaceCreateInfoKHR.pNext ); }
    XcbSurfaceCreateInfoKHR& pNext(const void* pNext){ m_xcbSurfaceCreateInfoKHR.pNext = pNext; return *this; }
    const XcbSurfaceCreateFlagsKHR& flags() const { return reinterpret_cast<const XcbSurfaceCreateFlagsKHR&>( m_xcbSurfaceCreateInfoKHR.flags ); }
    XcbSurfaceCreateInfoKHR& flags(XcbSurfaceCreateFlagsKHR flags){ m_xcbSurfaceCreateInfoKHR.flags = static_cast<VkXcbSurfaceCreateFlagsKHR>( flags ); return *this; }
    const xcb_connection_t* connection() const { return reinterpret_cast<xcb_connection_t*>( m_xcbSurfaceCreateInfoKHR.connection ); }
    XcbSurfaceCreateInfoKHR& connection(xcb_connection_t* connection){ m_xcbSurfaceCreateInfoKHR.connection = connection; return *this; }
    const xcb_window_t& window() const { return m_xcbSurfaceCreateInfoKHR.window; }
    XcbSurfaceCreateInfoKHR& window(xcb_window_t window){ m_xcbSurfaceCreateInfoKHR.window = window; return *this; }
    operator VkXcbSurfaceCreateInfoKHR&() { return m_xcbSurfaceCreateInfoKHR; }
    operator const VkXcbSurfaceCreateInfoKHR&() const { return m_xcbSurfaceCreateInfoKHR; }
    VkXcbSurfaceCreateInfoKHR& vkHandle() { return m_xcbSurfaceCreateInfoKHR; }
    const VkXcbSurfaceCreateInfoKHR& vkHandle() const { return m_xcbSurfaceCreateInfoKHR; }

  private:
    VkXcbSurfaceCreateInfoKHR m_xcbSurfaceCreateInfoKHR;
  };

#endif /* VK_USE_PLATFORM_XCB_KHR */

#ifdef VK_USE_PLATFORM_WAYLAND_KHR

  class WaylandSurfaceCreateInfoKHR
  {
  public:
    WaylandSurfaceCreateInfoKHR() : WaylandSurfaceCreateInfoKHR(WaylandSurfaceCreateFlagsKHR(), nullptr, nullptr) {}
    WaylandSurfaceCreateInfoKHR(WaylandSurfaceCreateFlagsKHR flags, struct wl_display* display, struct wl_surface* surface) {m_waylandSurfaceCreateInfoKHR.sType = VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR; m_waylandSurfaceCreateInfoKHR.pNext = nullptr; m_waylandSurfaceCreateInfoKHR.flags = static_cast<VkWaylandSurfaceCreateFlagsKHR>( flags ); m_waylandSurfaceCreateInfoKHR.display = display; m_waylandSurfaceCreateInfoKHR.surface = surface; }
    WaylandSurfaceCreateInfoKHR(const VkWaylandSurfaceCreateInfoKHR& other) : m_waylandSurfaceCreateInfoKHR{other} {}
    WaylandSurfaceCreateInfoKHR& operator=(const VkWaylandSurfaceCreateInfoKHR& other){ m_waylandSurfaceCreateInfoKHR = other; return *this; }

    const StructureType& sType() const { return reinterpret_cast<const StructureType&>( m_waylandSurfaceCreateInfoKHR.sType ); }
    WaylandSurfaceCreateInfoKHR& sType(StructureType sType){ m_waylandSurfaceCreateInfoKHR.sType = static_cast<VkStructureType>( sType ); return *this; }
    const void* pNext() const { return reinterpret_cast<const void*>( m_waylandSurfaceCreateInfoKHR.pNext ); }
    WaylandSurfaceCreateInfoKHR& pNext(const void* pNext){ m_waylandSurfaceCreateInfoKHR.pNext = pNext; return *this; }
    const WaylandSurfaceCreateFlagsKHR& flags() const { return reinterpret_cast<const WaylandSurfaceCreateFlagsKHR&>( m_waylandSurfaceCreateInfoKHR.flags ); }
    WaylandSurfaceCreateInfoKHR& flags(WaylandSurfaceCreateFlagsKHR flags){ m_waylandSurfaceCreateInfoKHR.flags = static_cast<VkWaylandSurfaceCreateFlagsKHR>( flags ); return *this; }
    const struct wl_display* display() const { return reinterpret_cast<struct wl_display*>( m_waylandSurfaceCreateInfoKHR.display ); }
    WaylandSurfaceCreateInfoKHR& display(struct wl_display* display){ m_waylandSurfaceCreateInfoKHR.display = display; return *this; }
    const struct wl_surface* surface() const { return reinterpret_cast<struct wl_surface*>( m_waylandSurfaceCreateInfoKHR.surface ); }
    WaylandSurfaceCreateInfoKHR& surface(struct wl_surface* surface){ m_waylandSurfaceCreateInfoKHR.surface = surface; return *this; }
    operator VkWaylandSurfaceCreateInfoKHR&() { return m_waylandSurfaceCreateInfoKHR; }
    operator const VkWaylandSurfaceCreateInfoKHR&() const { return m_waylandSurfaceCreateInfoKHR; }
    VkWaylandSurfaceCreateInfoKHR& vkHandle() { return m_waylandSurfaceCreateInfoKHR; }
    const VkWaylandSurfaceCreateInfoKHR& vkHandle() const { return m_waylandSurfaceCreateInfoKHR; }

  private:
    VkWaylandSurfaceCreateInfoKHR m_waylandSurfaceCreateInfoKHR;
  };

#endif /* VK_USE_PLATFORM_WAYLAND_KHR */

#ifdef VK_USE_PLATFORM_MIR_KHR

  class MirSurfaceCreateInfoKHR
  {
  public:
    MirSurfaceCreateInfoKHR() : MirSurfaceCreateInfoKHR(MirSurfaceCreateFlagsKHR(), nullptr, nullptr) {}
    MirSurfaceCreateInfoKHR(MirSurfaceCreateFlagsKHR flags, MirConnection* connection, MirSurface* mirSurface) {m_mirSurfaceCreateInfoKHR.sType = VK_STRUCTURE_TYPE_MIR_SURFACE_CREATE_INFO_KHR; m_mirSurfaceCreateInfoKHR.pNext = nullptr; m_mirSurfaceCreateInfoKHR.flags = static_cast<VkMirSurfaceCreateFlagsKHR>( flags ); m_mirSurfaceCreateInfoKHR.connection = connection; m_mirSurfaceCreateInfoKHR.mirSurface = mirSurface; }
    MirSurfaceCreateInfoKHR(const VkMirSurfaceCreateInfoKHR& other) : m_mirSurfaceCreateInfoKHR{other} {}
    MirSurfaceCreateInfoKHR& operator=(const VkMirSurfaceCreateInfoKHR& other){ m_mirSurfaceCreateInfoKHR = other; return *this; }

    const StructureType& sType() const { return reinterpret_cast<const StructureType&>( m_mirSurfaceCreateInfoKHR.sType ); }
    MirSurfaceCreateInfoKHR& sType(StructureType sType){ m_mirSurfaceCreateInfoKHR.sType = static_cast<VkStructureType>( sType ); return *this; }
    const void* pNext() const { return reinterpret_cast<const void*>( m_mirSurfaceCreateInfoKHR.pNext ); }
    MirSurfaceCreateInfoKHR& pNext(const void* pNext){ m_mirSurfaceCreateInfoKHR.pNext = pNext; return *this; }
    const MirSurfaceCreateFlagsKHR& flags() const { return reinterpret_cast<const MirSurfaceCreateFlagsKHR&>( m_mirSurfaceCreateInfoKHR.flags ); }
    MirSurfaceCreateInfoKHR& flags(MirSurfaceCreateFlagsKHR flags){ m_mirSurfaceCreateInfoKHR.flags = static_cast<VkMirSurfaceCreateFlagsKHR>( flags ); return *this; }
    const MirConnection* connection() const { return reinterpret_cast<MirConnection*>( m_mirSurfaceCreateInfoKHR.connection ); }
    MirSurfaceCreateInfoKHR& connection(MirConnection* connection){ m_mirSurfaceCreateInfoKHR.connection = connection; return *this; }
    const MirSurface* mirSurface() const { return reinterpret_cast<MirSurface*>( m_mirSurfaceCreateInfoKHR.mirSurface ); }
    MirSurfaceCreateInfoKHR& mirSurface(MirSurface* mirSurface){ m_mirSurfaceCreateInfoKHR.mirSurface = mirSurface; return *this; }
    operator VkMirSurfaceCreateInfoKHR&() { return m_mirSurfaceCreateInfoKHR; }
    operator const VkMirSurfaceCreateInfoKHR&() const { return m_mirSurfaceCreateInfoKHR; }
    VkMirSurfaceCreateInfoKHR& vkHandle() { return m_mirSurfaceCreateInfoKHR; }
    const VkMirSurfaceCreateInfoKHR& vkHandle() const { return m_mirSurfaceCreateInfoKHR; }

  private:
    VkMirSurfaceCreateInfoKHR m_mirSurfaceCreateInfoKHR;
  };

#endif /* VK_USE_PLATFORM_MIR_KHR */

#ifdef VK_USE_PLATFORM_ANDROID_KHR

  class AndroidSurfaceCreateInfoKHR
  {
  public:
    AndroidSurfaceCreateInfoKHR() : AndroidSurfaceCreateInfoKHR(AndroidSurfaceCreateFlagsKHR(), nullptr) {}
    AndroidSurfaceCreateInfoKHR(AndroidSurfaceCreateFlagsKHR flags, ANativeWindow* window) {m_androidSurfaceCreateInfoKHR.sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR; m_androidSurfaceCreateInfoKHR.pNext = nullptr; m_androidSurfaceCreateInfoKHR.flags = static_cast<VkAndroidSurfaceCreateFlagsKHR>( flags ); m_androidSurfaceCreateInfoKHR.window = window; }
    AndroidSurfaceCreateInfoKHR(const VkAndroidSurfaceCreateInfoKHR& other) : m_androidSurfaceCreateInfoKHR{other} {}
    AndroidSurfaceCreateInfoKHR& operator=(const VkAndroidSurfaceCreateInfoKHR& other){ m_androidSurfaceCreateInfoKHR = other; return *this; }

    const StructureType& sType() const { return reinterpret_cast<const StructureType&>( m_androidSurfaceCreateInfoKHR.sType ); }
    AndroidSurfaceCreateInfoKHR& sType(StructureType sType){ m_androidSurfaceCreateInfoKHR.sType = static_cast<VkStructureType>( sType ); return *this; }
    const void* pNext() const { return reinterpret_cast<const void*>( m_androidSurfaceCreateInfoKHR.pNext ); }
    AndroidSurfaceCreateInfoKHR& pNext(const void* pNext){ m_androidSurfaceCreateInfoKHR.pNext = pNext; return *this; }
    const AndroidSurfaceCreateFlagsKHR& flags() const { return reinterpret_cast<const AndroidSurfaceCreateFlagsKHR&>( m_androidSurfaceCreateInfoKHR.flags ); }
    AndroidSurfaceCreateInfoKHR& flags(AndroidSurfaceCreateFlagsKHR flags){ m_androidSurfaceCreateInfoKHR.flags = static_cast<VkAndroidSurfaceCreateFlagsKHR>( flags ); return *this; }
    const ANativeWindow* window() const { return reinterpret_cast<ANativeWindow*>( m_androidSurfaceCreateInfoKHR.window ); }
    AndroidSurfaceCreateInfoKHR& window(ANativeWindow* window){ m_androidSurfaceCreateInfoKHR.window = window; return *this; }
    operator VkAndroidSurfaceCreateInfoKHR&() { return m_androidSurfaceCreateInfoKHR; }
    operator const VkAndroidSurfaceCreateInfoKHR&() const { return m_androidSurfaceCreateInfoKHR; }
    VkAndroidSurfaceCreateInfoKHR& vkHandle() { return m_androidSurfaceCreateInfoKHR; }
    const VkAndroidSurfaceCreateInfoKHR& vkHandle() const { return m_androidSurfaceCreateInfoKHR; }

  private:
    VkAndroidSurfaceCreateInfoKHR m_androidSurfaceCreateInfoKHR;
  };

#endif /* VK_USE_PLATFORM_ANDROID_KHR */

#ifdef VK_USE_PLATFORM_WIN32_KHR

  class Win32SurfaceCreateInfoKHR
  {
  public:
    Win32SurfaceCreateInfoKHR() : Win32SurfaceCreateInfoKHR(Win32SurfaceCreateFlagsKHR(), 0, 0) {}
    Win32SurfaceCreateInfoKHR(Win32SurfaceCreateFlagsKHR flags, HINSTANCE hinstance, HWND hwnd) {m_win32SurfaceCreateInfoKHR.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR; m_win32SurfaceCreateInfoKHR.pNext = nullptr; m_win32SurfaceCreateInfoKHR.flags = static_cast<VkWin32SurfaceCreateFlagsKHR>( flags ); m_win32SurfaceCreateInfoKHR.hinstance = hinstance; m_win32SurfaceCreateInfoKHR.hwnd = hwnd; }
    Win32SurfaceCreateInfoKHR(const VkWin32SurfaceCreateInfoKHR& other) : m_win32SurfaceCreateInfoKHR{other} {}
    Win32SurfaceCreateInfoKHR& operator=(const VkWin32SurfaceCreateInfoKHR& other){ m_win32SurfaceCreateInfoKHR = other; return *this; }

    const StructureType& sType() const { return reinterpret_cast<const StructureType&>( m_win32SurfaceCreateInfoKHR.sType ); }
    Win32SurfaceCreateInfoKHR& sType(StructureType sType){ m_win32SurfaceCreateInfoKHR.sType = static_cast<VkStructureType>( sType ); return *this; }
    const void* pNext() const { return reinterpret_cast<const void*>( m_win32SurfaceCreateInfoKHR.pNext ); }
    Win32SurfaceCreateInfoKHR& pNext(const void* pNext){ m_win32SurfaceCreateInfoKHR.pNext = pNext; return *this; }
    const Win32SurfaceCreateFlagsKHR& flags() const { return reinterpret_cast<const Win32SurfaceCreateFlagsKHR&>( m_win32SurfaceCreateInfoKHR.flags ); }
    Win32SurfaceCreateInfoKHR& flags(Win32SurfaceCreateFlagsKHR flags){ m_win32SurfaceCreateInfoKHR.flags = static_cast<VkWin32SurfaceCreateFlagsKHR>( flags ); return *this; }
    const HINSTANCE& hinstance() const { return m_win32SurfaceCreateInfoKHR.hinstance; }
    Win32SurfaceCreateInfoKHR& hinstance(HINSTANCE hinstance){ m_win32SurfaceCreateInfoKHR.hinstance = hinstance; return *this; }
    const HWND& hwnd() const { return m_win32SurfaceCreateInfoKHR.hwnd; }
    Win32SurfaceCreateInfoKHR& hwnd(HWND hwnd){ m_win32SurfaceCreateInfoKHR.hwnd = hwnd; return *this; }
    operator VkWin32SurfaceCreateInfoKHR&() { return m_win32SurfaceCreateInfoKHR; }
    operator const VkWin32SurfaceCreateInfoKHR&() const { return m_win32SurfaceCreateInfoKHR; }
    VkWin32SurfaceCreateInfoKHR& vkHandle() { return m_win32SurfaceCreateInfoKHR; }
    const VkWin32SurfaceCreateInfoKHR& vkHandle() const { return m_win32SurfaceCreateInfoKHR; }

  private:
    VkWin32SurfaceCreateInfoKHR m_win32SurfaceCreateInfoKHR;
  };

#endif /* VK_USE_PLATFORM_WIN32_KHR */

}
