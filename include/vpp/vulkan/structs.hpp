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
namespace vk
{

struct ApplicationInfo
{
	StructureType sType {StructureType::applicationInfo};
	const void* pNext {};
	const char* pApplicationName {};
	uint32_t applicationVersion {};
	const char* pEngineName {};
	uint32_t engineVersion {};
	uint32_t apiVersion {};

	operator const VkApplicationInfo&() const { return reinterpret_cast<const VkApplicationInfo&>(*this); }
};
struct InstanceCreateInfo
{
	StructureType sType {StructureType::instanceCreateInfo};
	const void* pNext {};
	InstanceCreateFlags flags {};
	const ApplicationInfo* pApplicationInfo {};
	uint32_t enabledLayerCount {};
	const char** ppEnabledLayerNames {};
	uint32_t enabledExtensionCount {};
	const char** ppEnabledExtensionNames {};

	operator const VkInstanceCreateInfo&() const { return reinterpret_cast<const VkInstanceCreateInfo&>(*this); }
};
struct AllocationCallbacks
{
	void* pUserData {};
	PfnAllocationFunction pfnAllocation {};
	PfnReallocationFunction pfnReallocation {};
	PfnFreeFunction pfnFree {};
	PfnInternalAllocationNotification pfnInternalAllocation {};
	PfnInternalFreeNotification pfnInternalFree {};

	operator const VkAllocationCallbacks&() const { return reinterpret_cast<const VkAllocationCallbacks&>(*this); }
};
struct PhysicalDeviceFeatures
{
	bool robustBufferAccess {};
	bool fullDrawIndexUint32 {};
	bool imageCubeArray {};
	bool independentBlend {};
	bool geometryShader {};
	bool tessellationShader {};
	bool sampleRateShading {};
	bool dualSrcBlend {};
	bool logicOp {};
	bool multiDrawIndirect {};
	bool drawIndirectFirstInstance {};
	bool depthClamp {};
	bool depthBiasClamp {};
	bool fillModeNonSolid {};
	bool depthBounds {};
	bool wideLines {};
	bool largePoints {};
	bool alphaToOne {};
	bool multiViewport {};
	bool samplerAnisotropy {};
	bool textureCompressionETC2 {};
	bool textureCompressionASTC_LDR {};
	bool textureCompressionBC {};
	bool occlusionQueryPrecise {};
	bool pipelineStatisticsQuery {};
	bool vertexPipelineStoresAndAtomics {};
	bool fragmentStoresAndAtomics {};
	bool shaderTessellationAndGeometryPointSize {};
	bool shaderImageGatherExtended {};
	bool shaderStorageImageExtendedFormats {};
	bool shaderStorageImageMultisample {};
	bool shaderStorageImageReadWithoutFormat {};
	bool shaderStorageImageWriteWithoutFormat {};
	bool shaderUniformBufferArrayDynamicIndexing {};
	bool shaderSampledImageArrayDynamicIndexing {};
	bool shaderStorageBufferArrayDynamicIndexing {};
	bool shaderStorageImageArrayDynamicIndexing {};
	bool shaderClipDistance {};
	bool shaderCullDistance {};
	bool shaderFloat64 {};
	bool shaderInt64 {};
	bool shaderInt16 {};
	bool shaderResourceResidency {};
	bool shaderResourceMinLod {};
	bool sparseBinding {};
	bool sparseResidencyBuffer {};
	bool sparseResidencyImage2D {};
	bool sparseResidencyImage3D {};
	bool sparseResidency2Samples {};
	bool sparseResidency4Samples {};
	bool sparseResidency8Samples {};
	bool sparseResidency16Samples {};
	bool sparseResidencyAliased {};
	bool variableMultisampleRate {};
	bool inheritedQueries {};

	operator const VkPhysicalDeviceFeatures&() const { return reinterpret_cast<const VkPhysicalDeviceFeatures&>(*this); }
};
struct FormatProperties
{
	FormatFeatureFlags linearTilingFeatures {};
	FormatFeatureFlags optimalTilingFeatures {};
	FormatFeatureFlags bufferFeatures {};

	operator const VkFormatProperties&() const { return reinterpret_cast<const VkFormatProperties&>(*this); }
};
struct Extent3D
{
	uint32_t width {};
	uint32_t height {};
	uint32_t depth {};

	operator const VkExtent3D&() const { return reinterpret_cast<const VkExtent3D&>(*this); }
};
struct ImageFormatProperties
{
	Extent3D maxExtent {};
	uint32_t maxMipLevels {};
	uint32_t maxArrayLayers {};
	SampleCountFlags sampleCounts {};
	DeviceSize maxResourceSize {};

	operator const VkImageFormatProperties&() const { return reinterpret_cast<const VkImageFormatProperties&>(*this); }
};
struct PhysicalDeviceLimits
{
	uint32_t maxImageDimension1D {};
	uint32_t maxImageDimension2D {};
	uint32_t maxImageDimension3D {};
	uint32_t maxImageDimensionCube {};
	uint32_t maxImageArrayLayers {};
	uint32_t maxTexelBufferElements {};
	uint32_t maxUniformBufferRange {};
	uint32_t maxStorageBufferRange {};
	uint32_t maxPushConstantsSize {};
	uint32_t maxMemoryAllocationCount {};
	uint32_t maxSamplerAllocationCount {};
	DeviceSize bufferImageGranularity {};
	DeviceSize sparseAddressSpaceSize {};
	uint32_t maxBoundDescriptorSets {};
	uint32_t maxPerStageDescriptorSamplers {};
	uint32_t maxPerStageDescriptorUniformBuffers {};
	uint32_t maxPerStageDescriptorStorageBuffers {};
	uint32_t maxPerStageDescriptorSampledImages {};
	uint32_t maxPerStageDescriptorStorageImages {};
	uint32_t maxPerStageDescriptorInputAttachments {};
	uint32_t maxPerStageResources {};
	uint32_t maxDescriptorSetSamplers {};
	uint32_t maxDescriptorSetUniformBuffers {};
	uint32_t maxDescriptorSetUniformBuffersDynamic {};
	uint32_t maxDescriptorSetStorageBuffers {};
	uint32_t maxDescriptorSetStorageBuffersDynamic {};
	uint32_t maxDescriptorSetSampledImages {};
	uint32_t maxDescriptorSetStorageImages {};
	uint32_t maxDescriptorSetInputAttachments {};
	uint32_t maxVertexInputAttributes {};
	uint32_t maxVertexInputBindings {};
	uint32_t maxVertexInputAttributeOffset {};
	uint32_t maxVertexInputBindingStride {};
	uint32_t maxVertexOutputComponents {};
	uint32_t maxTessellationGenerationLevel {};
	uint32_t maxTessellationPatchSize {};
	uint32_t maxTessellationControlPerVertexInputComponents {};
	uint32_t maxTessellationControlPerVertexOutputComponents {};
	uint32_t maxTessellationControlPerPatchOutputComponents {};
	uint32_t maxTessellationControlTotalOutputComponents {};
	uint32_t maxTessellationEvaluationInputComponents {};
	uint32_t maxTessellationEvaluationOutputComponents {};
	uint32_t maxGeometryShaderInvocations {};
	uint32_t maxGeometryInputComponents {};
	uint32_t maxGeometryOutputComponents {};
	uint32_t maxGeometryOutputVertices {};
	uint32_t maxGeometryTotalOutputComponents {};
	uint32_t maxFragmentInputComponents {};
	uint32_t maxFragmentOutputAttachments {};
	uint32_t maxFragmentDualSrcAttachments {};
	uint32_t maxFragmentCombinedOutputResources {};
	uint32_t maxComputeSharedMemorySize {};
	uint32_t maxComputeWorkGroupCount[3] {};
	uint32_t maxComputeWorkGroupInvocations {};
	uint32_t maxComputeWorkGroupSize[3] {};
	uint32_t subPixelPrecisionBits {};
	uint32_t subTexelPrecisionBits {};
	uint32_t mipmapPrecisionBits {};
	uint32_t maxDrawIndexedIndexValue {};
	uint32_t maxDrawIndirectCount {};
	float maxSamplerLodBias {};
	float maxSamplerAnisotropy {};
	uint32_t maxViewports {};
	uint32_t maxViewportDimensions[2] {};
	float viewportBoundsRange[2] {};
	uint32_t viewportSubPixelBits {};
	size_t minMemoryMapAlignment {};
	DeviceSize minTexelBufferOffsetAlignment {};
	DeviceSize minUniformBufferOffsetAlignment {};
	DeviceSize minStorageBufferOffsetAlignment {};
	int32_t minTexelOffset {};
	uint32_t maxTexelOffset {};
	int32_t minTexelGatherOffset {};
	uint32_t maxTexelGatherOffset {};
	float minInterpolationOffset {};
	float maxInterpolationOffset {};
	uint32_t subPixelInterpolationOffsetBits {};
	uint32_t maxFramebufferWidth {};
	uint32_t maxFramebufferHeight {};
	uint32_t maxFramebufferLayers {};
	SampleCountFlags framebufferColorSampleCounts {};
	SampleCountFlags framebufferDepthSampleCounts {};
	SampleCountFlags framebufferStencilSampleCounts {};
	SampleCountFlags framebufferNoAttachmentsSampleCounts {};
	uint32_t maxColorAttachments {};
	SampleCountFlags sampledImageColorSampleCounts {};
	SampleCountFlags sampledImageIntegerSampleCounts {};
	SampleCountFlags sampledImageDepthSampleCounts {};
	SampleCountFlags sampledImageStencilSampleCounts {};
	SampleCountFlags storageImageSampleCounts {};
	uint32_t maxSampleMaskWords {};
	bool timestampComputeAndGraphics {};
	float timestampPeriod {};
	uint32_t maxClipDistances {};
	uint32_t maxCullDistances {};
	uint32_t maxCombinedClipAndCullDistances {};
	uint32_t discreteQueuePriorities {};
	float pointSizeRange[2] {};
	float lineWidthRange[2] {};
	float pointSizeGranularity {};
	float lineWidthGranularity {};
	bool strictLines {};
	bool standardSampleLocations {};
	DeviceSize optimalBufferCopyOffsetAlignment {};
	DeviceSize optimalBufferCopyRowPitchAlignment {};
	DeviceSize nonCoherentAtomSize {};

	operator const VkPhysicalDeviceLimits&() const { return reinterpret_cast<const VkPhysicalDeviceLimits&>(*this); }
};
struct PhysicalDeviceSparseProperties
{
	bool residencyStandard2DBlockShape {};
	bool residencyStandard2DMultisampleBlockShape {};
	bool residencyStandard3DBlockShape {};
	bool residencyAlignedMipSize {};
	bool residencyNonResidentStrict {};

	operator const VkPhysicalDeviceSparseProperties&() const { return reinterpret_cast<const VkPhysicalDeviceSparseProperties&>(*this); }
};
struct PhysicalDeviceProperties
{
	uint32_t apiVersion {};
	uint32_t driverVersion {};
	uint32_t vendorID {};
	uint32_t deviceID {};
	PhysicalDeviceType deviceType {};
	char deviceName {};
	uint8_t pipelineCacheUUID {};
	PhysicalDeviceLimits limits {};
	PhysicalDeviceSparseProperties sparseProperties {};

	operator const VkPhysicalDeviceProperties&() const { return reinterpret_cast<const VkPhysicalDeviceProperties&>(*this); }
};
struct QueueFamilyProperties
{
	QueueFlags queueFlags {};
	uint32_t queueCount {};
	uint32_t timestampValidBits {};
	Extent3D minImageTransferGranularity {};

	operator const VkQueueFamilyProperties&() const { return reinterpret_cast<const VkQueueFamilyProperties&>(*this); }
};
struct MemoryType
{
	MemoryPropertyFlags propertyFlags {};
	uint32_t heapIndex {};

	operator const VkMemoryType&() const { return reinterpret_cast<const VkMemoryType&>(*this); }
};
struct MemoryHeap
{
	DeviceSize size {};
	MemoryHeapFlags flags {};

	operator const VkMemoryHeap&() const { return reinterpret_cast<const VkMemoryHeap&>(*this); }
};
struct PhysicalDeviceMemoryProperties
{
	uint32_t memoryTypeCount {};
	MemoryType memoryTypes {};
	uint32_t memoryHeapCount {};
	MemoryHeap memoryHeaps {};

	operator const VkPhysicalDeviceMemoryProperties&() const { return reinterpret_cast<const VkPhysicalDeviceMemoryProperties&>(*this); }
};
struct DeviceQueueCreateInfo
{
	StructureType sType {StructureType::deviceQueueCreateInfo};
	const void* pNext {};
	DeviceQueueCreateFlags flags {};
	uint32_t queueFamilyIndex {};
	uint32_t queueCount {};
	const float* pQueuePriorities {};

	operator const VkDeviceQueueCreateInfo&() const { return reinterpret_cast<const VkDeviceQueueCreateInfo&>(*this); }
};
struct DeviceCreateInfo
{
	StructureType sType {StructureType::deviceCreateInfo};
	const void* pNext {};
	DeviceCreateFlags flags {};
	uint32_t queueCreateInfoCount {};
	const DeviceQueueCreateInfo* pQueueCreateInfos {};
	uint32_t enabledLayerCount {};
	const char** ppEnabledLayerNames {};
	uint32_t enabledExtensionCount {};
	const char** ppEnabledExtensionNames {};
	const PhysicalDeviceFeatures* pEnabledFeatures {};

	operator const VkDeviceCreateInfo&() const { return reinterpret_cast<const VkDeviceCreateInfo&>(*this); }
};
struct ExtensionProperties
{
	char extensionName {};
	uint32_t specVersion {};

	operator const VkExtensionProperties&() const { return reinterpret_cast<const VkExtensionProperties&>(*this); }
};
struct LayerProperties
{
	char layerName {};
	uint32_t specVersion {};
	uint32_t implementationVersion {};
	char description {};

	operator const VkLayerProperties&() const { return reinterpret_cast<const VkLayerProperties&>(*this); }
};
struct SubmitInfo
{
	StructureType sType {StructureType::submitInfo};
	const void* pNext {};
	uint32_t waitSemaphoreCount {};
	const Semaphore* pWaitSemaphores {};
	const PipelineStageFlags* pWaitDstStageMask {};
	uint32_t commandBufferCount {};
	const CommandBuffer* pCommandBuffers {};
	uint32_t signalSemaphoreCount {};
	const Semaphore* pSignalSemaphores {};

	operator const VkSubmitInfo&() const { return reinterpret_cast<const VkSubmitInfo&>(*this); }
};
struct MemoryAllocateInfo
{
	StructureType sType {StructureType::memoryAllocateInfo};
	const void* pNext {};
	DeviceSize allocationSize {};
	uint32_t memoryTypeIndex {};

	operator const VkMemoryAllocateInfo&() const { return reinterpret_cast<const VkMemoryAllocateInfo&>(*this); }
};
struct MappedMemoryRange
{
	StructureType sType {StructureType::mappedMemoryRange};
	const void* pNext {};
	DeviceMemory memory {};
	DeviceSize offset {};
	DeviceSize size {};

	operator const VkMappedMemoryRange&() const { return reinterpret_cast<const VkMappedMemoryRange&>(*this); }
};
struct MemoryRequirements
{
	DeviceSize size {};
	DeviceSize alignment {};
	uint32_t memoryTypeBits {};

	operator const VkMemoryRequirements&() const { return reinterpret_cast<const VkMemoryRequirements&>(*this); }
};
struct SparseImageFormatProperties
{
	ImageAspectFlags aspectMask {};
	Extent3D imageGranularity {};
	SparseImageFormatFlags flags {};

	operator const VkSparseImageFormatProperties&() const { return reinterpret_cast<const VkSparseImageFormatProperties&>(*this); }
};
struct SparseImageMemoryRequirements
{
	SparseImageFormatProperties formatProperties {};
	uint32_t imageMipTailFirstLod {};
	DeviceSize imageMipTailSize {};
	DeviceSize imageMipTailOffset {};
	DeviceSize imageMipTailStride {};

	operator const VkSparseImageMemoryRequirements&() const { return reinterpret_cast<const VkSparseImageMemoryRequirements&>(*this); }
};
struct SparseMemoryBind
{
	DeviceSize resourceOffset {};
	DeviceSize size {};
	DeviceMemory memory {};
	DeviceSize memoryOffset {};
	SparseMemoryBindFlags flags {};

	operator const VkSparseMemoryBind&() const { return reinterpret_cast<const VkSparseMemoryBind&>(*this); }
};
struct SparseBufferMemoryBindInfo
{
	Buffer buffer {};
	uint32_t bindCount {};
	const SparseMemoryBind* pBinds {};

	operator const VkSparseBufferMemoryBindInfo&() const { return reinterpret_cast<const VkSparseBufferMemoryBindInfo&>(*this); }
};
struct SparseImageOpaqueMemoryBindInfo
{
	Image image {};
	uint32_t bindCount {};
	const SparseMemoryBind* pBinds {};

	operator const VkSparseImageOpaqueMemoryBindInfo&() const { return reinterpret_cast<const VkSparseImageOpaqueMemoryBindInfo&>(*this); }
};
struct ImageSubresource
{
	ImageAspectFlags aspectMask {};
	uint32_t mipLevel {};
	uint32_t arrayLayer {};

	operator const VkImageSubresource&() const { return reinterpret_cast<const VkImageSubresource&>(*this); }
};
struct Offset3D
{
	int32_t x {};
	int32_t y {};
	int32_t z {};

	operator const VkOffset3D&() const { return reinterpret_cast<const VkOffset3D&>(*this); }
};
struct SparseImageMemoryBind
{
	ImageSubresource subresource {};
	Offset3D offset {};
	Extent3D extent {};
	DeviceMemory memory {};
	DeviceSize memoryOffset {};
	SparseMemoryBindFlags flags {};

	operator const VkSparseImageMemoryBind&() const { return reinterpret_cast<const VkSparseImageMemoryBind&>(*this); }
};
struct SparseImageMemoryBindInfo
{
	Image image {};
	uint32_t bindCount {};
	const SparseImageMemoryBind* pBinds {};

	operator const VkSparseImageMemoryBindInfo&() const { return reinterpret_cast<const VkSparseImageMemoryBindInfo&>(*this); }
};
struct BindSparseInfo
{
	StructureType sType {StructureType::bindSparseInfo};
	const void* pNext {};
	uint32_t waitSemaphoreCount {};
	const Semaphore* pWaitSemaphores {};
	uint32_t bufferBindCount {};
	const SparseBufferMemoryBindInfo* pBufferBinds {};
	uint32_t imageOpaqueBindCount {};
	const SparseImageOpaqueMemoryBindInfo* pImageOpaqueBinds {};
	uint32_t imageBindCount {};
	const SparseImageMemoryBindInfo* pImageBinds {};
	uint32_t signalSemaphoreCount {};
	const Semaphore* pSignalSemaphores {};

	operator const VkBindSparseInfo&() const { return reinterpret_cast<const VkBindSparseInfo&>(*this); }
};
struct FenceCreateInfo
{
	StructureType sType {StructureType::fenceCreateInfo};
	const void* pNext {};
	FenceCreateFlags flags {};

	operator const VkFenceCreateInfo&() const { return reinterpret_cast<const VkFenceCreateInfo&>(*this); }
};
struct SemaphoreCreateInfo
{
	StructureType sType {StructureType::semaphoreCreateInfo};
	const void* pNext {};
	SemaphoreCreateFlags flags {};

	operator const VkSemaphoreCreateInfo&() const { return reinterpret_cast<const VkSemaphoreCreateInfo&>(*this); }
};
struct EventCreateInfo
{
	StructureType sType {StructureType::eventCreateInfo};
	const void* pNext {};
	EventCreateFlags flags {};

	operator const VkEventCreateInfo&() const { return reinterpret_cast<const VkEventCreateInfo&>(*this); }
};
struct QueryPoolCreateInfo
{
	StructureType sType {StructureType::queryPoolCreateInfo};
	const void* pNext {};
	QueryPoolCreateFlags flags {};
	QueryType queryType {};
	uint32_t queryCount {};
	QueryPipelineStatisticFlags pipelineStatistics {};

	operator const VkQueryPoolCreateInfo&() const { return reinterpret_cast<const VkQueryPoolCreateInfo&>(*this); }
};
struct BufferCreateInfo
{
	StructureType sType {StructureType::bufferCreateInfo};
	const void* pNext {};
	BufferCreateFlags flags {};
	DeviceSize size {};
	BufferUsageFlags usage {};
	SharingMode sharingMode {};
	uint32_t queueFamilyIndexCount {};
	const uint32_t* pQueueFamilyIndices {};

	operator const VkBufferCreateInfo&() const { return reinterpret_cast<const VkBufferCreateInfo&>(*this); }
};
struct BufferViewCreateInfo
{
	StructureType sType {StructureType::bufferViewCreateInfo};
	const void* pNext {};
	BufferViewCreateFlags flags {};
	Buffer buffer {};
	Format format {};
	DeviceSize offset {};
	DeviceSize range {};

	operator const VkBufferViewCreateInfo&() const { return reinterpret_cast<const VkBufferViewCreateInfo&>(*this); }
};
struct ImageCreateInfo
{
	StructureType sType {StructureType::imageCreateInfo};
	const void* pNext {};
	ImageCreateFlags flags {};
	ImageType imageType {};
	Format format {};
	Extent3D extent {};
	uint32_t mipLevels {};
	uint32_t arrayLayers {};
	SampleCountBits samples {};
	ImageTiling tiling {};
	ImageUsageFlags usage {};
	SharingMode sharingMode {};
	uint32_t queueFamilyIndexCount {};
	const uint32_t* pQueueFamilyIndices {};
	ImageLayout initialLayout {};

	operator const VkImageCreateInfo&() const { return reinterpret_cast<const VkImageCreateInfo&>(*this); }
};
struct SubresourceLayout
{
	DeviceSize offset {};
	DeviceSize size {};
	DeviceSize rowPitch {};
	DeviceSize arrayPitch {};
	DeviceSize depthPitch {};

	operator const VkSubresourceLayout&() const { return reinterpret_cast<const VkSubresourceLayout&>(*this); }
};
struct ComponentMapping
{
	ComponentSwizzle r {};
	ComponentSwizzle g {};
	ComponentSwizzle b {};
	ComponentSwizzle a {};

	operator const VkComponentMapping&() const { return reinterpret_cast<const VkComponentMapping&>(*this); }
};
struct ImageSubresourceRange
{
	ImageAspectFlags aspectMask {};
	uint32_t baseMipLevel {};
	uint32_t levelCount {};
	uint32_t baseArrayLayer {};
	uint32_t layerCount {};

	operator const VkImageSubresourceRange&() const { return reinterpret_cast<const VkImageSubresourceRange&>(*this); }
};
struct ImageViewCreateInfo
{
	StructureType sType {StructureType::imageViewCreateInfo};
	const void* pNext {};
	ImageViewCreateFlags flags {};
	Image image {};
	ImageViewType viewType {};
	Format format {};
	ComponentMapping components {};
	ImageSubresourceRange subresourceRange {};

	operator const VkImageViewCreateInfo&() const { return reinterpret_cast<const VkImageViewCreateInfo&>(*this); }
};
struct ShaderModuleCreateInfo
{
	StructureType sType {StructureType::shaderModuleCreateInfo};
	const void* pNext {};
	ShaderModuleCreateFlags flags {};
	size_t codeSize {};
	const uint32_t* pCode {};

	operator const VkShaderModuleCreateInfo&() const { return reinterpret_cast<const VkShaderModuleCreateInfo&>(*this); }
};
struct PipelineCacheCreateInfo
{
	StructureType sType {StructureType::pipelineCacheCreateInfo};
	const void* pNext {};
	PipelineCacheCreateFlags flags {};
	size_t initialDataSize {};
	const void* pInitialData {};

	operator const VkPipelineCacheCreateInfo&() const { return reinterpret_cast<const VkPipelineCacheCreateInfo&>(*this); }
};
struct SpecializationMapEntry
{
	uint32_t constantID {};
	uint32_t offset {};
	size_t size {};

	operator const VkSpecializationMapEntry&() const { return reinterpret_cast<const VkSpecializationMapEntry&>(*this); }
};
struct SpecializationInfo
{
	uint32_t mapEntryCount {};
	const SpecializationMapEntry* pMapEntries {};
	size_t dataSize {};
	const void* pData {};

	operator const VkSpecializationInfo&() const { return reinterpret_cast<const VkSpecializationInfo&>(*this); }
};
struct PipelineShaderStageCreateInfo
{
	StructureType sType {StructureType::pipelineShaderStageCreateInfo};
	const void* pNext {};
	PipelineShaderStageCreateFlags flags {};
	ShaderStageBits stage {};
	ShaderModule module {};
	const char* pName {};
	const SpecializationInfo* pSpecializationInfo {};

	operator const VkPipelineShaderStageCreateInfo&() const { return reinterpret_cast<const VkPipelineShaderStageCreateInfo&>(*this); }
};
struct VertexInputBindingDescription
{
	uint32_t binding {};
	uint32_t stride {};
	VertexInputRate inputRate {};

	operator const VkVertexInputBindingDescription&() const { return reinterpret_cast<const VkVertexInputBindingDescription&>(*this); }
};
struct VertexInputAttributeDescription
{
	uint32_t location {};
	uint32_t binding {};
	Format format {};
	uint32_t offset {};

	operator const VkVertexInputAttributeDescription&() const { return reinterpret_cast<const VkVertexInputAttributeDescription&>(*this); }
};
struct PipelineVertexInputStateCreateInfo
{
	StructureType sType {StructureType::pipelineVertexInputStateCreateInfo};
	const void* pNext {};
	PipelineVertexInputStateCreateFlags flags {};
	uint32_t vertexBindingDescriptionCount {};
	const VertexInputBindingDescription* pVertexBindingDescriptions {};
	uint32_t vertexAttributeDescriptionCount {};
	const VertexInputAttributeDescription* pVertexAttributeDescriptions {};

	operator const VkPipelineVertexInputStateCreateInfo&() const { return reinterpret_cast<const VkPipelineVertexInputStateCreateInfo&>(*this); }
};
struct PipelineInputAssemblyStateCreateInfo
{
	StructureType sType {StructureType::pipelineInputAssemblyStateCreateInfo};
	const void* pNext {};
	PipelineInputAssemblyStateCreateFlags flags {};
	PrimitiveTopology topology {};
	bool primitiveRestartEnable {};

	operator const VkPipelineInputAssemblyStateCreateInfo&() const { return reinterpret_cast<const VkPipelineInputAssemblyStateCreateInfo&>(*this); }
};
struct PipelineTessellationStateCreateInfo
{
	StructureType sType {StructureType::pipelineTessellationStateCreateInfo};
	const void* pNext {};
	PipelineTessellationStateCreateFlags flags {};
	uint32_t patchControlPoints {};

	operator const VkPipelineTessellationStateCreateInfo&() const { return reinterpret_cast<const VkPipelineTessellationStateCreateInfo&>(*this); }
};
struct Viewport
{
	float x {};
	float y {};
	float width {};
	float height {};
	float minDepth {};
	float maxDepth {};

	operator const VkViewport&() const { return reinterpret_cast<const VkViewport&>(*this); }
};
struct Offset2D
{
	int32_t x {};
	int32_t y {};

	operator const VkOffset2D&() const { return reinterpret_cast<const VkOffset2D&>(*this); }
};
struct Extent2D
{
	uint32_t width {};
	uint32_t height {};

	operator const VkExtent2D&() const { return reinterpret_cast<const VkExtent2D&>(*this); }
};
struct Rect2D
{
	Offset2D offset {};
	Extent2D extent {};

	operator const VkRect2D&() const { return reinterpret_cast<const VkRect2D&>(*this); }
};
struct PipelineViewportStateCreateInfo
{
	StructureType sType {StructureType::pipelineViewportStateCreateInfo};
	const void* pNext {};
	PipelineViewportStateCreateFlags flags {};
	uint32_t viewportCount {};
	const Viewport* pViewports {};
	uint32_t scissorCount {};
	const Rect2D* pScissors {};

	operator const VkPipelineViewportStateCreateInfo&() const { return reinterpret_cast<const VkPipelineViewportStateCreateInfo&>(*this); }
};
struct PipelineRasterizationStateCreateInfo
{
	StructureType sType {StructureType::pipelineRasterizationStateCreateInfo};
	const void* pNext {};
	PipelineRasterizationStateCreateFlags flags {};
	bool depthClampEnable {};
	bool rasterizerDiscardEnable {};
	PolygonMode polygonMode {};
	CullModeFlags cullMode {};
	FrontFace frontFace {};
	bool depthBiasEnable {};
	float depthBiasConstantFactor {};
	float depthBiasClamp {};
	float depthBiasSlopeFactor {};
	float lineWidth {};

	operator const VkPipelineRasterizationStateCreateInfo&() const { return reinterpret_cast<const VkPipelineRasterizationStateCreateInfo&>(*this); }
};
struct PipelineMultisampleStateCreateInfo
{
	StructureType sType {StructureType::pipelineMultisampleStateCreateInfo};
	const void* pNext {};
	PipelineMultisampleStateCreateFlags flags {};
	SampleCountBits rasterizationSamples {};
	bool sampleShadingEnable {};
	float minSampleShading {};
	const SampleMask* pSampleMask {};
	bool alphaToCoverageEnable {};
	bool alphaToOneEnable {};

	operator const VkPipelineMultisampleStateCreateInfo&() const { return reinterpret_cast<const VkPipelineMultisampleStateCreateInfo&>(*this); }
};
struct StencilOpState
{
	StencilOp failOp {};
	StencilOp passOp {};
	StencilOp depthFailOp {};
	CompareOp compareOp {};
	uint32_t compareMask {};
	uint32_t writeMask {};
	uint32_t reference {};

	operator const VkStencilOpState&() const { return reinterpret_cast<const VkStencilOpState&>(*this); }
};
struct PipelineDepthStencilStateCreateInfo
{
	StructureType sType {StructureType::pipelineDepthStencilStateCreateInfo};
	const void* pNext {};
	PipelineDepthStencilStateCreateFlags flags {};
	bool depthTestEnable {};
	bool depthWriteEnable {};
	CompareOp depthCompareOp {};
	bool depthBoundsTestEnable {};
	bool stencilTestEnable {};
	StencilOpState front {};
	StencilOpState back {};
	float minDepthBounds {};
	float maxDepthBounds {};

	operator const VkPipelineDepthStencilStateCreateInfo&() const { return reinterpret_cast<const VkPipelineDepthStencilStateCreateInfo&>(*this); }
};
struct PipelineColorBlendAttachmentState
{
	bool blendEnable {};
	BlendFactor srcColorBlendFactor {};
	BlendFactor dstColorBlendFactor {};
	BlendOp colorBlendOp {};
	BlendFactor srcAlphaBlendFactor {};
	BlendFactor dstAlphaBlendFactor {};
	BlendOp alphaBlendOp {};
	ColorComponentFlags colorWriteMask {};

	operator const VkPipelineColorBlendAttachmentState&() const { return reinterpret_cast<const VkPipelineColorBlendAttachmentState&>(*this); }
};
struct PipelineColorBlendStateCreateInfo
{
	StructureType sType {StructureType::pipelineColorBlendStateCreateInfo};
	const void* pNext {};
	PipelineColorBlendStateCreateFlags flags {};
	bool logicOpEnable {};
	LogicOp logicOp {};
	uint32_t attachmentCount {};
	const PipelineColorBlendAttachmentState* pAttachments {};
	float blendConstants[4] {};

	operator const VkPipelineColorBlendStateCreateInfo&() const { return reinterpret_cast<const VkPipelineColorBlendStateCreateInfo&>(*this); }
};
struct PipelineDynamicStateCreateInfo
{
	StructureType sType {StructureType::pipelineDynamicStateCreateInfo};
	const void* pNext {};
	PipelineDynamicStateCreateFlags flags {};
	uint32_t dynamicStateCount {};
	const DynamicState* pDynamicStates {};

	operator const VkPipelineDynamicStateCreateInfo&() const { return reinterpret_cast<const VkPipelineDynamicStateCreateInfo&>(*this); }
};
struct GraphicsPipelineCreateInfo
{
	StructureType sType {StructureType::graphicsPipelineCreateInfo};
	const void* pNext {};
	PipelineCreateFlags flags {};
	uint32_t stageCount {};
	const PipelineShaderStageCreateInfo* pStages {};
	const PipelineVertexInputStateCreateInfo* pVertexInputState {};
	const PipelineInputAssemblyStateCreateInfo* pInputAssemblyState {};
	const PipelineTessellationStateCreateInfo* pTessellationState {};
	const PipelineViewportStateCreateInfo* pViewportState {};
	const PipelineRasterizationStateCreateInfo* pRasterizationState {};
	const PipelineMultisampleStateCreateInfo* pMultisampleState {};
	const PipelineDepthStencilStateCreateInfo* pDepthStencilState {};
	const PipelineColorBlendStateCreateInfo* pColorBlendState {};
	const PipelineDynamicStateCreateInfo* pDynamicState {};
	PipelineLayout layout {};
	RenderPass renderPass {};
	uint32_t subpass {};
	Pipeline basePipelineHandle {};
	int32_t basePipelineIndex {};

	operator const VkGraphicsPipelineCreateInfo&() const { return reinterpret_cast<const VkGraphicsPipelineCreateInfo&>(*this); }
};
struct ComputePipelineCreateInfo
{
	StructureType sType {StructureType::computePipelineCreateInfo};
	const void* pNext {};
	PipelineCreateFlags flags {};
	PipelineShaderStageCreateInfo stage {};
	PipelineLayout layout {};
	Pipeline basePipelineHandle {};
	int32_t basePipelineIndex {};

	operator const VkComputePipelineCreateInfo&() const { return reinterpret_cast<const VkComputePipelineCreateInfo&>(*this); }
};
struct PushConstantRange
{
	ShaderStageFlags stageFlags {};
	uint32_t offset {};
	uint32_t size {};

	operator const VkPushConstantRange&() const { return reinterpret_cast<const VkPushConstantRange&>(*this); }
};
struct PipelineLayoutCreateInfo
{
	StructureType sType {StructureType::pipelineLayoutCreateInfo};
	const void* pNext {};
	PipelineLayoutCreateFlags flags {};
	uint32_t setLayoutCount {};
	const DescriptorSetLayout* pSetLayouts {};
	uint32_t pushConstantRangeCount {};
	const PushConstantRange* pPushConstantRanges {};

	operator const VkPipelineLayoutCreateInfo&() const { return reinterpret_cast<const VkPipelineLayoutCreateInfo&>(*this); }
};
struct SamplerCreateInfo
{
	StructureType sType {StructureType::samplerCreateInfo};
	const void* pNext {};
	SamplerCreateFlags flags {};
	Filter magFilter {};
	Filter minFilter {};
	SamplerMipmapMode mipmapMode {};
	SamplerAddressMode addressModeU {};
	SamplerAddressMode addressModeV {};
	SamplerAddressMode addressModeW {};
	float mipLodBias {};
	bool anisotropyEnable {};
	float maxAnisotropy {};
	bool compareEnable {};
	CompareOp compareOp {};
	float minLod {};
	float maxLod {};
	BorderColor borderColor {};
	bool unnormalizedCoordinates {};

	operator const VkSamplerCreateInfo&() const { return reinterpret_cast<const VkSamplerCreateInfo&>(*this); }
};
struct DescriptorSetLayoutBinding
{
	uint32_t binding {};
	DescriptorType descriptorType {};
	uint32_t descriptorCount {};
	ShaderStageFlags stageFlags {};
	const Sampler* pImmutableSamplers {};

	operator const VkDescriptorSetLayoutBinding&() const { return reinterpret_cast<const VkDescriptorSetLayoutBinding&>(*this); }
};
struct DescriptorSetLayoutCreateInfo
{
	StructureType sType {StructureType::descriptorSetLayoutCreateInfo};
	const void* pNext {};
	DescriptorSetLayoutCreateFlags flags {};
	uint32_t bindingCount {};
	const DescriptorSetLayoutBinding* pBindings {};

	operator const VkDescriptorSetLayoutCreateInfo&() const { return reinterpret_cast<const VkDescriptorSetLayoutCreateInfo&>(*this); }
};
struct DescriptorPoolSize
{
	DescriptorType type {};
	uint32_t descriptorCount {};

	operator const VkDescriptorPoolSize&() const { return reinterpret_cast<const VkDescriptorPoolSize&>(*this); }
};
struct DescriptorPoolCreateInfo
{
	StructureType sType {StructureType::descriptorPoolCreateInfo};
	const void* pNext {};
	DescriptorPoolCreateFlags flags {};
	uint32_t maxSets {};
	uint32_t poolSizeCount {};
	const DescriptorPoolSize* pPoolSizes {};

	operator const VkDescriptorPoolCreateInfo&() const { return reinterpret_cast<const VkDescriptorPoolCreateInfo&>(*this); }
};
struct DescriptorSetAllocateInfo
{
	StructureType sType {StructureType::descriptorSetAllocateInfo};
	const void* pNext {};
	DescriptorPool descriptorPool {};
	uint32_t descriptorSetCount {};
	const DescriptorSetLayout* pSetLayouts {};

	operator const VkDescriptorSetAllocateInfo&() const { return reinterpret_cast<const VkDescriptorSetAllocateInfo&>(*this); }
};
struct DescriptorImageInfo
{
	Sampler sampler {};
	ImageView imageView {};
	ImageLayout imageLayout {};

	operator const VkDescriptorImageInfo&() const { return reinterpret_cast<const VkDescriptorImageInfo&>(*this); }
};
struct DescriptorBufferInfo
{
	Buffer buffer {};
	DeviceSize offset {};
	DeviceSize range {};

	operator const VkDescriptorBufferInfo&() const { return reinterpret_cast<const VkDescriptorBufferInfo&>(*this); }
};
struct WriteDescriptorSet
{
	StructureType sType {StructureType::writeDescriptorSet};
	const void* pNext {};
	DescriptorSet dstSet {};
	uint32_t dstBinding {};
	uint32_t dstArrayElement {};
	uint32_t descriptorCount {};
	DescriptorType descriptorType {};
	const DescriptorImageInfo* pImageInfo {};
	const DescriptorBufferInfo* pBufferInfo {};
	const BufferView* pTexelBufferView {};

	operator const VkWriteDescriptorSet&() const { return reinterpret_cast<const VkWriteDescriptorSet&>(*this); }
};
struct CopyDescriptorSet
{
	StructureType sType {StructureType::copyDescriptorSet};
	const void* pNext {};
	DescriptorSet srcSet {};
	uint32_t srcBinding {};
	uint32_t srcArrayElement {};
	DescriptorSet dstSet {};
	uint32_t dstBinding {};
	uint32_t dstArrayElement {};
	uint32_t descriptorCount {};

	operator const VkCopyDescriptorSet&() const { return reinterpret_cast<const VkCopyDescriptorSet&>(*this); }
};
struct FramebufferCreateInfo
{
	StructureType sType {StructureType::framebufferCreateInfo};
	const void* pNext {};
	FramebufferCreateFlags flags {};
	RenderPass renderPass {};
	uint32_t attachmentCount {};
	const ImageView* pAttachments {};
	uint32_t width {};
	uint32_t height {};
	uint32_t layers {};

	operator const VkFramebufferCreateInfo&() const { return reinterpret_cast<const VkFramebufferCreateInfo&>(*this); }
};
struct AttachmentDescription
{
	AttachmentDescriptionFlags flags {};
	Format format {};
	SampleCountBits samples {};
	AttachmentLoadOp loadOp {};
	AttachmentStoreOp storeOp {};
	AttachmentLoadOp stencilLoadOp {};
	AttachmentStoreOp stencilStoreOp {};
	ImageLayout initialLayout {};
	ImageLayout finalLayout {};

	operator const VkAttachmentDescription&() const { return reinterpret_cast<const VkAttachmentDescription&>(*this); }
};
struct AttachmentReference
{
	uint32_t attachment {};
	ImageLayout layout {};

	operator const VkAttachmentReference&() const { return reinterpret_cast<const VkAttachmentReference&>(*this); }
};
struct SubpassDescription
{
	SubpassDescriptionFlags flags {};
	PipelineBindPoint pipelineBindPoint {};
	uint32_t inputAttachmentCount {};
	const AttachmentReference* pInputAttachments {};
	uint32_t colorAttachmentCount {};
	const AttachmentReference* pColorAttachments {};
	const AttachmentReference* pResolveAttachments {};
	const AttachmentReference* pDepthStencilAttachment {};
	uint32_t preserveAttachmentCount {};
	const uint32_t* pPreserveAttachments {};

	operator const VkSubpassDescription&() const { return reinterpret_cast<const VkSubpassDescription&>(*this); }
};
struct SubpassDependency
{
	uint32_t srcSubpass {};
	uint32_t dstSubpass {};
	PipelineStageFlags srcStageMask {};
	PipelineStageFlags dstStageMask {};
	AccessFlags srcAccessMask {};
	AccessFlags dstAccessMask {};
	DependencyFlags dependencyFlags {};

	operator const VkSubpassDependency&() const { return reinterpret_cast<const VkSubpassDependency&>(*this); }
};
struct RenderPassCreateInfo
{
	StructureType sType {StructureType::renderPassCreateInfo};
	const void* pNext {};
	RenderPassCreateFlags flags {};
	uint32_t attachmentCount {};
	const AttachmentDescription* pAttachments {};
	uint32_t subpassCount {};
	const SubpassDescription* pSubpasses {};
	uint32_t dependencyCount {};
	const SubpassDependency* pDependencies {};

	operator const VkRenderPassCreateInfo&() const { return reinterpret_cast<const VkRenderPassCreateInfo&>(*this); }
};
struct CommandPoolCreateInfo
{
	StructureType sType {StructureType::commandPoolCreateInfo};
	const void* pNext {};
	CommandPoolCreateFlags flags {};
	uint32_t queueFamilyIndex {};

	operator const VkCommandPoolCreateInfo&() const { return reinterpret_cast<const VkCommandPoolCreateInfo&>(*this); }
};
struct CommandBufferAllocateInfo
{
	StructureType sType {StructureType::commandBufferAllocateInfo};
	const void* pNext {};
	CommandPool commandPool {};
	CommandBufferLevel level {};
	uint32_t commandBufferCount {};

	operator const VkCommandBufferAllocateInfo&() const { return reinterpret_cast<const VkCommandBufferAllocateInfo&>(*this); }
};
struct CommandBufferInheritanceInfo
{
	StructureType sType {StructureType::commandBufferInheritanceInfo};
	const void* pNext {};
	RenderPass renderPass {};
	uint32_t subpass {};
	Framebuffer framebuffer {};
	bool occlusionQueryEnable {};
	QueryControlFlags queryFlags {};
	QueryPipelineStatisticFlags pipelineStatistics {};

	operator const VkCommandBufferInheritanceInfo&() const { return reinterpret_cast<const VkCommandBufferInheritanceInfo&>(*this); }
};
struct CommandBufferBeginInfo
{
	StructureType sType {StructureType::commandBufferBeginInfo};
	const void* pNext {};
	CommandBufferUsageFlags flags {};
	const CommandBufferInheritanceInfo* pInheritanceInfo {};

	operator const VkCommandBufferBeginInfo&() const { return reinterpret_cast<const VkCommandBufferBeginInfo&>(*this); }
};
struct BufferCopy
{
	DeviceSize srcOffset {};
	DeviceSize dstOffset {};
	DeviceSize size {};

	operator const VkBufferCopy&() const { return reinterpret_cast<const VkBufferCopy&>(*this); }
};
struct ImageSubresourceLayers
{
	ImageAspectFlags aspectMask {};
	uint32_t mipLevel {};
	uint32_t baseArrayLayer {};
	uint32_t layerCount {};

	operator const VkImageSubresourceLayers&() const { return reinterpret_cast<const VkImageSubresourceLayers&>(*this); }
};
struct ImageCopy
{
	ImageSubresourceLayers srcSubresource {};
	Offset3D srcOffset {};
	ImageSubresourceLayers dstSubresource {};
	Offset3D dstOffset {};
	Extent3D extent {};

	operator const VkImageCopy&() const { return reinterpret_cast<const VkImageCopy&>(*this); }
};
struct ImageBlit
{
	ImageSubresourceLayers srcSubresource {};
	Offset3D srcOffsets[2] {};
	ImageSubresourceLayers dstSubresource {};
	Offset3D dstOffsets[2] {};

	operator const VkImageBlit&() const { return reinterpret_cast<const VkImageBlit&>(*this); }
};
struct BufferImageCopy
{
	DeviceSize bufferOffset {};
	uint32_t bufferRowLength {};
	uint32_t bufferImageHeight {};
	ImageSubresourceLayers imageSubresource {};
	Offset3D imageOffset {};
	Extent3D imageExtent {};

	operator const VkBufferImageCopy&() const { return reinterpret_cast<const VkBufferImageCopy&>(*this); }
};
union ClearColorValue
{
	float float32[4] {};
	int32_t int32[4];
	uint32_t uint32[4];

	operator const VkClearColorValue&() const { return reinterpret_cast<const VkClearColorValue&>(*this); }
};
struct ClearDepthStencilValue
{
	float depth {};
	uint32_t stencil {};

	operator const VkClearDepthStencilValue&() const { return reinterpret_cast<const VkClearDepthStencilValue&>(*this); }
};
union ClearValue
{
	ClearColorValue color {};
	ClearDepthStencilValue depthStencil;

	operator const VkClearValue&() const { return reinterpret_cast<const VkClearValue&>(*this); }
};
struct ClearAttachment
{
	ImageAspectFlags aspectMask {};
	uint32_t colorAttachment {};
	ClearValue clearValue {};

	operator const VkClearAttachment&() const { return reinterpret_cast<const VkClearAttachment&>(*this); }
};
struct ClearRect
{
	Rect2D rect {};
	uint32_t baseArrayLayer {};
	uint32_t layerCount {};

	operator const VkClearRect&() const { return reinterpret_cast<const VkClearRect&>(*this); }
};
struct ImageResolve
{
	ImageSubresourceLayers srcSubresource {};
	Offset3D srcOffset {};
	ImageSubresourceLayers dstSubresource {};
	Offset3D dstOffset {};
	Extent3D extent {};

	operator const VkImageResolve&() const { return reinterpret_cast<const VkImageResolve&>(*this); }
};
struct MemoryBarrier
{
	StructureType sType {StructureType::memoryBarrier};
	const void* pNext {};
	AccessFlags srcAccessMask {};
	AccessFlags dstAccessMask {};

	operator const VkMemoryBarrier&() const { return reinterpret_cast<const VkMemoryBarrier&>(*this); }
};
struct BufferMemoryBarrier
{
	StructureType sType {StructureType::bufferMemoryBarrier};
	const void* pNext {};
	AccessFlags srcAccessMask {};
	AccessFlags dstAccessMask {};
	uint32_t srcQueueFamilyIndex {};
	uint32_t dstQueueFamilyIndex {};
	Buffer buffer {};
	DeviceSize offset {};
	DeviceSize size {};

	operator const VkBufferMemoryBarrier&() const { return reinterpret_cast<const VkBufferMemoryBarrier&>(*this); }
};
struct ImageMemoryBarrier
{
	StructureType sType {StructureType::imageMemoryBarrier};
	const void* pNext {};
	AccessFlags srcAccessMask {};
	AccessFlags dstAccessMask {};
	ImageLayout oldLayout {};
	ImageLayout newLayout {};
	uint32_t srcQueueFamilyIndex {};
	uint32_t dstQueueFamilyIndex {};
	Image image {};
	ImageSubresourceRange subresourceRange {};

	operator const VkImageMemoryBarrier&() const { return reinterpret_cast<const VkImageMemoryBarrier&>(*this); }
};
struct RenderPassBeginInfo
{
	StructureType sType {StructureType::renderPassBeginInfo};
	const void* pNext {};
	RenderPass renderPass {};
	Framebuffer framebuffer {};
	Rect2D renderArea {};
	uint32_t clearValueCount {};
	const ClearValue* pClearValues {};

	operator const VkRenderPassBeginInfo&() const { return reinterpret_cast<const VkRenderPassBeginInfo&>(*this); }
};
struct DispatchIndirectCommand
{
	uint32_t x {};
	uint32_t y {};
	uint32_t z {};

	operator const VkDispatchIndirectCommand&() const { return reinterpret_cast<const VkDispatchIndirectCommand&>(*this); }
};
struct DrawIndexedIndirectCommand
{
	uint32_t indexCount {};
	uint32_t instanceCount {};
	uint32_t firstIndex {};
	int32_t vertexOffset {};
	uint32_t firstInstance {};

	operator const VkDrawIndexedIndirectCommand&() const { return reinterpret_cast<const VkDrawIndexedIndirectCommand&>(*this); }
};
struct DrawIndirectCommand
{
	uint32_t vertexCount {};
	uint32_t instanceCount {};
	uint32_t firstVertex {};
	uint32_t firstInstance {};

	operator const VkDrawIndirectCommand&() const { return reinterpret_cast<const VkDrawIndirectCommand&>(*this); }
};

struct SurfaceCapabilitiesKHR
{
	uint32_t minImageCount {};
	uint32_t maxImageCount {};
	Extent2D currentExtent {};
	Extent2D minImageExtent {};
	Extent2D maxImageExtent {};
	uint32_t maxImageArrayLayers {};
	SurfaceTransformFlagsKHR supportedTransforms {};
	SurfaceTransformBitsKHR currentTransform {};
	CompositeAlphaFlagsKHR supportedCompositeAlpha {};
	ImageUsageFlags supportedUsageFlags {};

	operator const VkSurfaceCapabilitiesKHR&() const { return reinterpret_cast<const VkSurfaceCapabilitiesKHR&>(*this); }
};
struct SurfaceFormatKHR
{
	Format format {};
	ColorSpaceKHR colorSpace {};

	operator const VkSurfaceFormatKHR&() const { return reinterpret_cast<const VkSurfaceFormatKHR&>(*this); }
};

struct SwapchainCreateInfoKHR
{
	StructureType sType {StructureType::swapchainCreateInfoKHR};
	const void* pNext {};
	SwapchainCreateFlagsKHR flags {};
	SurfaceKHR surface {};
	uint32_t minImageCount {};
	Format imageFormat {};
	ColorSpaceKHR imageColorSpace {};
	Extent2D imageExtent {};
	uint32_t imageArrayLayers {};
	ImageUsageFlags imageUsage {};
	SharingMode imageSharingMode {};
	uint32_t queueFamilyIndexCount {};
	const uint32_t* pQueueFamilyIndices {};
	SurfaceTransformBitsKHR preTransform {};
	CompositeAlphaBitsKHR compositeAlpha {};
	PresentModeKHR presentMode {};
	bool clipped {};
	SwapchainKHR oldSwapchain {};

	operator const VkSwapchainCreateInfoKHR&() const { return reinterpret_cast<const VkSwapchainCreateInfoKHR&>(*this); }
};
struct PresentInfoKHR
{
	StructureType sType {StructureType::presentInfoKHR};
	const void* pNext {};
	uint32_t waitSemaphoreCount {};
	const Semaphore* pWaitSemaphores {};
	uint32_t swapchainCount {};
	const SwapchainKHR* pSwapchains {};
	const uint32_t* pImageIndices {};
	Result* pResults {};

	operator const VkPresentInfoKHR&() const { return reinterpret_cast<const VkPresentInfoKHR&>(*this); }
};

struct DisplayPropertiesKHR
{
	DisplayKHR display {};
	const char* displayName {};
	Extent2D physicalDimensions {};
	Extent2D physicalResolution {};
	SurfaceTransformFlagsKHR supportedTransforms {};
	bool planeReorderPossible {};
	bool persistentContent {};

	operator const VkDisplayPropertiesKHR&() const { return reinterpret_cast<const VkDisplayPropertiesKHR&>(*this); }
};
struct DisplayModeParametersKHR
{
	Extent2D visibleRegion {};
	uint32_t refreshRate {};

	operator const VkDisplayModeParametersKHR&() const { return reinterpret_cast<const VkDisplayModeParametersKHR&>(*this); }
};
struct DisplayModePropertiesKHR
{
	DisplayModeKHR displayMode {};
	DisplayModeParametersKHR parameters {};

	operator const VkDisplayModePropertiesKHR&() const { return reinterpret_cast<const VkDisplayModePropertiesKHR&>(*this); }
};
struct DisplayModeCreateInfoKHR
{
	StructureType sType {StructureType::displayModeCreateInfoKHR};
	const void* pNext {};
	DisplayModeCreateFlagsKHR flags {};
	DisplayModeParametersKHR parameters {};

	operator const VkDisplayModeCreateInfoKHR&() const { return reinterpret_cast<const VkDisplayModeCreateInfoKHR&>(*this); }
};
struct DisplayPlaneCapabilitiesKHR
{
	DisplayPlaneAlphaFlagsKHR supportedAlpha {};
	Offset2D minSrcPosition {};
	Offset2D maxSrcPosition {};
	Extent2D minSrcExtent {};
	Extent2D maxSrcExtent {};
	Offset2D minDstPosition {};
	Offset2D maxDstPosition {};
	Extent2D minDstExtent {};
	Extent2D maxDstExtent {};

	operator const VkDisplayPlaneCapabilitiesKHR&() const { return reinterpret_cast<const VkDisplayPlaneCapabilitiesKHR&>(*this); }
};
struct DisplayPlanePropertiesKHR
{
	DisplayKHR currentDisplay {};
	uint32_t currentStackIndex {};

	operator const VkDisplayPlanePropertiesKHR&() const { return reinterpret_cast<const VkDisplayPlanePropertiesKHR&>(*this); }
};
struct DisplaySurfaceCreateInfoKHR
{
	StructureType sType {StructureType::displaySurfaceCreateInfoKHR};
	const void* pNext {};
	DisplaySurfaceCreateFlagsKHR flags {};
	DisplayModeKHR displayMode {};
	uint32_t planeIndex {};
	uint32_t planeStackIndex {};
	SurfaceTransformBitsKHR transform {};
	float globalAlpha {};
	DisplayPlaneAlphaBitsKHR alphaMode {};
	Extent2D imageExtent {};

	operator const VkDisplaySurfaceCreateInfoKHR&() const { return reinterpret_cast<const VkDisplaySurfaceCreateInfoKHR&>(*this); }
};

struct DisplayPresentInfoKHR
{
	StructureType sType {StructureType::displayPresentInfoKHR};
	const void* pNext {};
	Rect2D srcRect {};
	Rect2D dstRect {};
	bool persistent {};

	operator const VkDisplayPresentInfoKHR&() const { return reinterpret_cast<const VkDisplayPresentInfoKHR&>(*this); }
};

#ifdef VK_USE_PLATFORM_XLIB_KHR

struct XlibSurfaceCreateInfoKHR
{
	StructureType sType {StructureType::xlibSurfaceCreateInfoKHR};
	const void* pNext {};
	XlibSurfaceCreateFlagsKHR flags {};
	Display* dpy {};
	Window window {};

	operator const VkXlibSurfaceCreateInfoKHR&() const { return reinterpret_cast<const VkXlibSurfaceCreateInfoKHR&>(*this); }
};

#endif //VK_USE_PLATFORM_XLIB_KHR

#ifdef VK_USE_PLATFORM_XCB_KHR

struct XcbSurfaceCreateInfoKHR
{
	StructureType sType {StructureType::xcbSurfaceCreateInfoKHR};
	const void* pNext {};
	XcbSurfaceCreateFlagsKHR flags {};
	xcb_connection_t* connection {};
	xcb_window_t window {};

	operator const VkXcbSurfaceCreateInfoKHR&() const { return reinterpret_cast<const VkXcbSurfaceCreateInfoKHR&>(*this); }
};

#endif //VK_USE_PLATFORM_XCB_KHR

#ifdef VK_USE_PLATFORM_WAYLAND_KHR

struct WaylandSurfaceCreateInfoKHR
{
	StructureType sType {StructureType::waylandSurfaceCreateInfoKHR};
	const void* pNext {};
	WaylandSurfaceCreateFlagsKHR flags {};
	wl_display* display {};
	wl_surface* surface {};

	operator const VkWaylandSurfaceCreateInfoKHR&() const { return reinterpret_cast<const VkWaylandSurfaceCreateInfoKHR&>(*this); }
};

#endif //VK_USE_PLATFORM_WAYLAND_KHR

#ifdef VK_USE_PLATFORM_MIR_KHR

struct MirSurfaceCreateInfoKHR
{
	StructureType sType {StructureType::mirSurfaceCreateInfoKHR};
	const void* pNext {};
	MirSurfaceCreateFlagsKHR flags {};
	MirConnection* connection {};
	MirSurface* mirSurface {};

	operator const VkMirSurfaceCreateInfoKHR&() const { return reinterpret_cast<const VkMirSurfaceCreateInfoKHR&>(*this); }
};

#endif //VK_USE_PLATFORM_MIR_KHR

#ifdef VK_USE_PLATFORM_ANDROID_KHR

struct AndroidSurfaceCreateInfoKHR
{
	StructureType sType {StructureType::androidSurfaceCreateInfoKHR};
	const void* pNext {};
	AndroidSurfaceCreateFlagsKHR flags {};
	ANativeWindow* window {};

	operator const VkAndroidSurfaceCreateInfoKHR&() const { return reinterpret_cast<const VkAndroidSurfaceCreateInfoKHR&>(*this); }
};

#endif //VK_USE_PLATFORM_ANDROID_KHR

#ifdef VK_USE_PLATFORM_WIN32_KHR

struct Win32SurfaceCreateInfoKHR
{
	StructureType sType {StructureType::win32SurfaceCreateInfoKHR};
	const void* pNext {};
	Win32SurfaceCreateFlagsKHR flags {};
	HINSTANCE hinstance {};
	HWND hwnd {};

	operator const VkWin32SurfaceCreateInfoKHR&() const { return reinterpret_cast<const VkWin32SurfaceCreateInfoKHR&>(*this); }
};

#endif //VK_USE_PLATFORM_WIN32_KHR

struct DebugReportCallbackCreateInfoEXT
{
	StructureType sType {StructureType::debugReportCallbackCreateInfoEXT};
	const void* pNext {};
	DebugReportFlagsEXT flags {};
	PfnDebugReportCallbackEXT pfnCallback {};
	void* pUserData {};

	operator const VkDebugReportCallbackCreateInfoEXT&() const { return reinterpret_cast<const VkDebugReportCallbackCreateInfoEXT&>(*this); }
};

struct PipelineRasterizationStateRasterizationOrderAMD
{
	StructureType sType {StructureType::pipelineRasterizationStateRasterizationOrderAMD};
	const void* pNext {};
	RasterizationOrderAMD rasterizationOrder {};

	operator const VkPipelineRasterizationStateRasterizationOrderAMD&() const { return reinterpret_cast<const VkPipelineRasterizationStateRasterizationOrderAMD&>(*this); }
};

struct DebugMarkerObjectNameInfoEXT
{
	StructureType sType {StructureType::debugMarkerObjectNameInfoEXT};
	const void* pNext {};
	DebugReportObjectTypeEXT objectType {};
	uint64_t object {};
	const char* pObjectName {};

	operator const VkDebugMarkerObjectNameInfoEXT&() const { return reinterpret_cast<const VkDebugMarkerObjectNameInfoEXT&>(*this); }
};
struct DebugMarkerObjectTagInfoEXT
{
	StructureType sType {StructureType::debugMarkerObjectTagInfoEXT};
	const void* pNext {};
	DebugReportObjectTypeEXT objectType {};
	uint64_t object {};
	uint64_t tagName {};
	size_t tagSize {};
	const void* pTag {};

	operator const VkDebugMarkerObjectTagInfoEXT&() const { return reinterpret_cast<const VkDebugMarkerObjectTagInfoEXT&>(*this); }
};
struct DebugMarkerMarkerInfoEXT
{
	StructureType sType {StructureType::debugMarkerMarkerInfoEXT};
	const void* pNext {};
	const char* pMarkerName {};
	float color[4] {};

	operator const VkDebugMarkerMarkerInfoEXT&() const { return reinterpret_cast<const VkDebugMarkerMarkerInfoEXT&>(*this); }
};



} //namespace vk