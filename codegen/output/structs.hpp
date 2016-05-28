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

	ApplicationInfo(const char* xpApplicationName = {}, uint32_t xapplicationVersion = {}, const char* xpEngineName = {}, uint32_t xengineVersion = {}, uint32_t xapiVersion = {}) : pApplicationName(xpApplicationName), applicationVersion(xapplicationVersion), pEngineName(xpEngineName), engineVersion(xengineVersion), apiVersion(xapiVersion) {}
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

	InstanceCreateInfo(InstanceCreateFlags xflags = {}, const ApplicationInfo* xpApplicationInfo = {}, uint32_t xenabledLayerCount = {}, const char** xppEnabledLayerNames = {}, uint32_t xenabledExtensionCount = {}, const char** xppEnabledExtensionNames = {}) : flags(xflags), pApplicationInfo(xpApplicationInfo), enabledLayerCount(xenabledLayerCount), ppEnabledLayerNames(xppEnabledLayerNames), enabledExtensionCount(xenabledExtensionCount), ppEnabledExtensionNames(xppEnabledExtensionNames) {}
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

	AllocationCallbacks(void* xpUserData = {}, PfnAllocationFunction xpfnAllocation = {}, PfnReallocationFunction xpfnReallocation = {}, PfnFreeFunction xpfnFree = {}, PfnInternalAllocationNotification xpfnInternalAllocation = {}, PfnInternalFreeNotification xpfnInternalFree = {}) : pUserData(xpUserData), pfnAllocation(xpfnAllocation), pfnReallocation(xpfnReallocation), pfnFree(xpfnFree), pfnInternalAllocation(xpfnInternalAllocation), pfnInternalFree(xpfnInternalFree) {}
	operator const VkAllocationCallbacks&() const { return reinterpret_cast<const VkAllocationCallbacks&>(*this); }
};
struct PhysicalDeviceFeatures
{
	Bool32 robustBufferAccess {};
	Bool32 fullDrawIndexUint32 {};
	Bool32 imageCubeArray {};
	Bool32 independentBlend {};
	Bool32 geometryShader {};
	Bool32 tessellationShader {};
	Bool32 sampleRateShading {};
	Bool32 dualSrcBlend {};
	Bool32 logicOp {};
	Bool32 multiDrawIndirect {};
	Bool32 drawIndirectFirstInstance {};
	Bool32 depthClamp {};
	Bool32 depthBiasClamp {};
	Bool32 fillModeNonSolid {};
	Bool32 depthBounds {};
	Bool32 wideLines {};
	Bool32 largePoints {};
	Bool32 alphaToOne {};
	Bool32 multiViewport {};
	Bool32 samplerAnisotropy {};
	Bool32 textureCompressionETC2 {};
	Bool32 textureCompressionASTC_LDR {};
	Bool32 textureCompressionBC {};
	Bool32 occlusionQueryPrecise {};
	Bool32 pipelineStatisticsQuery {};
	Bool32 vertexPipelineStoresAndAtomics {};
	Bool32 fragmentStoresAndAtomics {};
	Bool32 shaderTessellationAndGeometryPointSize {};
	Bool32 shaderImageGatherExtended {};
	Bool32 shaderStorageImageExtendedFormats {};
	Bool32 shaderStorageImageMultisample {};
	Bool32 shaderStorageImageReadWithoutFormat {};
	Bool32 shaderStorageImageWriteWithoutFormat {};
	Bool32 shaderUniformBufferArrayDynamicIndexing {};
	Bool32 shaderSampledImageArrayDynamicIndexing {};
	Bool32 shaderStorageBufferArrayDynamicIndexing {};
	Bool32 shaderStorageImageArrayDynamicIndexing {};
	Bool32 shaderClipDistance {};
	Bool32 shaderCullDistance {};
	Bool32 shaderFloat64 {};
	Bool32 shaderInt64 {};
	Bool32 shaderInt16 {};
	Bool32 shaderResourceResidency {};
	Bool32 shaderResourceMinLod {};
	Bool32 sparseBinding {};
	Bool32 sparseResidencyBuffer {};
	Bool32 sparseResidencyImage2D {};
	Bool32 sparseResidencyImage3D {};
	Bool32 sparseResidency2Samples {};
	Bool32 sparseResidency4Samples {};
	Bool32 sparseResidency8Samples {};
	Bool32 sparseResidency16Samples {};
	Bool32 sparseResidencyAliased {};
	Bool32 variableMultisampleRate {};
	Bool32 inheritedQueries {};

	PhysicalDeviceFeatures(Bool32 xrobustBufferAccess = {}, Bool32 xfullDrawIndexUint32 = {}, Bool32 ximageCubeArray = {}, Bool32 xindependentBlend = {}, Bool32 xgeometryShader = {}, Bool32 xtessellationShader = {}, Bool32 xsampleRateShading = {}, Bool32 xdualSrcBlend = {}, Bool32 xlogicOp = {}, Bool32 xmultiDrawIndirect = {}, Bool32 xdrawIndirectFirstInstance = {}, Bool32 xdepthClamp = {}, Bool32 xdepthBiasClamp = {}, Bool32 xfillModeNonSolid = {}, Bool32 xdepthBounds = {}, Bool32 xwideLines = {}, Bool32 xlargePoints = {}, Bool32 xalphaToOne = {}, Bool32 xmultiViewport = {}, Bool32 xsamplerAnisotropy = {}, Bool32 xtextureCompressionETC2 = {}, Bool32 xtextureCompressionASTC_LDR = {}, Bool32 xtextureCompressionBC = {}, Bool32 xocclusionQueryPrecise = {}, Bool32 xpipelineStatisticsQuery = {}, Bool32 xvertexPipelineStoresAndAtomics = {}, Bool32 xfragmentStoresAndAtomics = {}, Bool32 xshaderTessellationAndGeometryPointSize = {}, Bool32 xshaderImageGatherExtended = {}, Bool32 xshaderStorageImageExtendedFormats = {}, Bool32 xshaderStorageImageMultisample = {}, Bool32 xshaderStorageImageReadWithoutFormat = {}, Bool32 xshaderStorageImageWriteWithoutFormat = {}, Bool32 xshaderUniformBufferArrayDynamicIndexing = {}, Bool32 xshaderSampledImageArrayDynamicIndexing = {}, Bool32 xshaderStorageBufferArrayDynamicIndexing = {}, Bool32 xshaderStorageImageArrayDynamicIndexing = {}, Bool32 xshaderClipDistance = {}, Bool32 xshaderCullDistance = {}, Bool32 xshaderFloat64 = {}, Bool32 xshaderInt64 = {}, Bool32 xshaderInt16 = {}, Bool32 xshaderResourceResidency = {}, Bool32 xshaderResourceMinLod = {}, Bool32 xsparseBinding = {}, Bool32 xsparseResidencyBuffer = {}, Bool32 xsparseResidencyImage2D = {}, Bool32 xsparseResidencyImage3D = {}, Bool32 xsparseResidency2Samples = {}, Bool32 xsparseResidency4Samples = {}, Bool32 xsparseResidency8Samples = {}, Bool32 xsparseResidency16Samples = {}, Bool32 xsparseResidencyAliased = {}, Bool32 xvariableMultisampleRate = {}, Bool32 xinheritedQueries = {}) : robustBufferAccess(xrobustBufferAccess), fullDrawIndexUint32(xfullDrawIndexUint32), imageCubeArray(ximageCubeArray), independentBlend(xindependentBlend), geometryShader(xgeometryShader), tessellationShader(xtessellationShader), sampleRateShading(xsampleRateShading), dualSrcBlend(xdualSrcBlend), logicOp(xlogicOp), multiDrawIndirect(xmultiDrawIndirect), drawIndirectFirstInstance(xdrawIndirectFirstInstance), depthClamp(xdepthClamp), depthBiasClamp(xdepthBiasClamp), fillModeNonSolid(xfillModeNonSolid), depthBounds(xdepthBounds), wideLines(xwideLines), largePoints(xlargePoints), alphaToOne(xalphaToOne), multiViewport(xmultiViewport), samplerAnisotropy(xsamplerAnisotropy), textureCompressionETC2(xtextureCompressionETC2), textureCompressionASTC_LDR(xtextureCompressionASTC_LDR), textureCompressionBC(xtextureCompressionBC), occlusionQueryPrecise(xocclusionQueryPrecise), pipelineStatisticsQuery(xpipelineStatisticsQuery), vertexPipelineStoresAndAtomics(xvertexPipelineStoresAndAtomics), fragmentStoresAndAtomics(xfragmentStoresAndAtomics), shaderTessellationAndGeometryPointSize(xshaderTessellationAndGeometryPointSize), shaderImageGatherExtended(xshaderImageGatherExtended), shaderStorageImageExtendedFormats(xshaderStorageImageExtendedFormats), shaderStorageImageMultisample(xshaderStorageImageMultisample), shaderStorageImageReadWithoutFormat(xshaderStorageImageReadWithoutFormat), shaderStorageImageWriteWithoutFormat(xshaderStorageImageWriteWithoutFormat), shaderUniformBufferArrayDynamicIndexing(xshaderUniformBufferArrayDynamicIndexing), shaderSampledImageArrayDynamicIndexing(xshaderSampledImageArrayDynamicIndexing), shaderStorageBufferArrayDynamicIndexing(xshaderStorageBufferArrayDynamicIndexing), shaderStorageImageArrayDynamicIndexing(xshaderStorageImageArrayDynamicIndexing), shaderClipDistance(xshaderClipDistance), shaderCullDistance(xshaderCullDistance), shaderFloat64(xshaderFloat64), shaderInt64(xshaderInt64), shaderInt16(xshaderInt16), shaderResourceResidency(xshaderResourceResidency), shaderResourceMinLod(xshaderResourceMinLod), sparseBinding(xsparseBinding), sparseResidencyBuffer(xsparseResidencyBuffer), sparseResidencyImage2D(xsparseResidencyImage2D), sparseResidencyImage3D(xsparseResidencyImage3D), sparseResidency2Samples(xsparseResidency2Samples), sparseResidency4Samples(xsparseResidency4Samples), sparseResidency8Samples(xsparseResidency8Samples), sparseResidency16Samples(xsparseResidency16Samples), sparseResidencyAliased(xsparseResidencyAliased), variableMultisampleRate(xvariableMultisampleRate), inheritedQueries(xinheritedQueries) {}
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

	Extent3D(uint32_t xwidth = {}, uint32_t xheight = {}, uint32_t xdepth = {}) : width(xwidth), height(xheight), depth(xdepth) {}
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
	Bool32 timestampComputeAndGraphics {};
	float timestampPeriod {};
	uint32_t maxClipDistances {};
	uint32_t maxCullDistances {};
	uint32_t maxCombinedClipAndCullDistances {};
	uint32_t discreteQueuePriorities {};
	float pointSizeRange[2] {};
	float lineWidthRange[2] {};
	float pointSizeGranularity {};
	float lineWidthGranularity {};
	Bool32 strictLines {};
	Bool32 standardSampleLocations {};
	DeviceSize optimalBufferCopyOffsetAlignment {};
	DeviceSize optimalBufferCopyRowPitchAlignment {};
	DeviceSize nonCoherentAtomSize {};

	operator const VkPhysicalDeviceLimits&() const { return reinterpret_cast<const VkPhysicalDeviceLimits&>(*this); }
};
struct PhysicalDeviceSparseProperties
{
	Bool32 residencyStandard2DBlockShape {};
	Bool32 residencyStandard2DMultisampleBlockShape {};
	Bool32 residencyStandard3DBlockShape {};
	Bool32 residencyAlignedMipSize {};
	Bool32 residencyNonResidentStrict {};

	operator const VkPhysicalDeviceSparseProperties&() const { return reinterpret_cast<const VkPhysicalDeviceSparseProperties&>(*this); }
};
struct PhysicalDeviceProperties
{
	uint32_t apiVersion {};
	uint32_t driverVersion {};
	uint32_t vendorID {};
	uint32_t deviceID {};
	PhysicalDeviceType deviceType {};
	char deviceName[maxPhysicalDeviceNameSize] {};
	uint8_t pipelineCacheUUID[uuidSize] {};
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
	MemoryType memoryTypes[maxMemoryTypes] {};
	uint32_t memoryHeapCount {};
	MemoryHeap memoryHeaps[maxMemoryHeaps] {};

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

	DeviceQueueCreateInfo(DeviceQueueCreateFlags xflags = {}, uint32_t xqueueFamilyIndex = {}, uint32_t xqueueCount = {}, const float* xpQueuePriorities = {}) : flags(xflags), queueFamilyIndex(xqueueFamilyIndex), queueCount(xqueueCount), pQueuePriorities(xpQueuePriorities) {}
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

	DeviceCreateInfo(DeviceCreateFlags xflags = {}, uint32_t xqueueCreateInfoCount = {}, const DeviceQueueCreateInfo* xpQueueCreateInfos = {}, uint32_t xenabledLayerCount = {}, const char** xppEnabledLayerNames = {}, uint32_t xenabledExtensionCount = {}, const char** xppEnabledExtensionNames = {}, const PhysicalDeviceFeatures* xpEnabledFeatures = {}) : flags(xflags), queueCreateInfoCount(xqueueCreateInfoCount), pQueueCreateInfos(xpQueueCreateInfos), enabledLayerCount(xenabledLayerCount), ppEnabledLayerNames(xppEnabledLayerNames), enabledExtensionCount(xenabledExtensionCount), ppEnabledExtensionNames(xppEnabledExtensionNames), pEnabledFeatures(xpEnabledFeatures) {}
	operator const VkDeviceCreateInfo&() const { return reinterpret_cast<const VkDeviceCreateInfo&>(*this); }
};
struct ExtensionProperties
{
	char extensionName[maxExtensionNameSize] {};
	uint32_t specVersion {};

	operator const VkExtensionProperties&() const { return reinterpret_cast<const VkExtensionProperties&>(*this); }
};
struct LayerProperties
{
	char layerName[maxExtensionNameSize] {};
	uint32_t specVersion {};
	uint32_t implementationVersion {};
	char description[maxDescriptionSize] {};

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

	SubmitInfo(uint32_t xwaitSemaphoreCount = {}, const Semaphore* xpWaitSemaphores = {}, const PipelineStageFlags* xpWaitDstStageMask = {}, uint32_t xcommandBufferCount = {}, const CommandBuffer* xpCommandBuffers = {}, uint32_t xsignalSemaphoreCount = {}, const Semaphore* xpSignalSemaphores = {}) : waitSemaphoreCount(xwaitSemaphoreCount), pWaitSemaphores(xpWaitSemaphores), pWaitDstStageMask(xpWaitDstStageMask), commandBufferCount(xcommandBufferCount), pCommandBuffers(xpCommandBuffers), signalSemaphoreCount(xsignalSemaphoreCount), pSignalSemaphores(xpSignalSemaphores) {}
	operator const VkSubmitInfo&() const { return reinterpret_cast<const VkSubmitInfo&>(*this); }
};
struct MemoryAllocateInfo
{
	StructureType sType {StructureType::memoryAllocateInfo};
	const void* pNext {};
	DeviceSize allocationSize {};
	uint32_t memoryTypeIndex {};

	MemoryAllocateInfo(DeviceSize xallocationSize = {}, uint32_t xmemoryTypeIndex = {}) : allocationSize(xallocationSize), memoryTypeIndex(xmemoryTypeIndex) {}
	operator const VkMemoryAllocateInfo&() const { return reinterpret_cast<const VkMemoryAllocateInfo&>(*this); }
};
struct MappedMemoryRange
{
	StructureType sType {StructureType::mappedMemoryRange};
	const void* pNext {};
	DeviceMemory memory {};
	DeviceSize offset {};
	DeviceSize size {};

	MappedMemoryRange(DeviceMemory xmemory = {}, DeviceSize xoffset = {}, DeviceSize xsize = {}) : memory(xmemory), offset(xoffset), size(xsize) {}
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

	SparseMemoryBind(DeviceSize xresourceOffset = {}, DeviceSize xsize = {}, DeviceMemory xmemory = {}, DeviceSize xmemoryOffset = {}, SparseMemoryBindFlags xflags = {}) : resourceOffset(xresourceOffset), size(xsize), memory(xmemory), memoryOffset(xmemoryOffset), flags(xflags) {}
	operator const VkSparseMemoryBind&() const { return reinterpret_cast<const VkSparseMemoryBind&>(*this); }
};
struct SparseBufferMemoryBindInfo
{
	Buffer buffer {};
	uint32_t bindCount {};
	const SparseMemoryBind* pBinds {};

	SparseBufferMemoryBindInfo(Buffer xbuffer = {}, uint32_t xbindCount = {}, const SparseMemoryBind* xpBinds = {}) : buffer(xbuffer), bindCount(xbindCount), pBinds(xpBinds) {}
	operator const VkSparseBufferMemoryBindInfo&() const { return reinterpret_cast<const VkSparseBufferMemoryBindInfo&>(*this); }
};
struct SparseImageOpaqueMemoryBindInfo
{
	Image image {};
	uint32_t bindCount {};
	const SparseMemoryBind* pBinds {};

	SparseImageOpaqueMemoryBindInfo(Image ximage = {}, uint32_t xbindCount = {}, const SparseMemoryBind* xpBinds = {}) : image(ximage), bindCount(xbindCount), pBinds(xpBinds) {}
	operator const VkSparseImageOpaqueMemoryBindInfo&() const { return reinterpret_cast<const VkSparseImageOpaqueMemoryBindInfo&>(*this); }
};
struct ImageSubresource
{
	ImageAspectFlags aspectMask {};
	uint32_t mipLevel {};
	uint32_t arrayLayer {};

	ImageSubresource(ImageAspectFlags xaspectMask = {}, uint32_t xmipLevel = {}, uint32_t xarrayLayer = {}) : aspectMask(xaspectMask), mipLevel(xmipLevel), arrayLayer(xarrayLayer) {}
	operator const VkImageSubresource&() const { return reinterpret_cast<const VkImageSubresource&>(*this); }
};
struct Offset3D
{
	int32_t x {};
	int32_t y {};
	int32_t z {};

	Offset3D(int32_t xx = {}, int32_t xy = {}, int32_t xz = {}) : x(xx), y(xy), z(xz) {}
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

	SparseImageMemoryBind(ImageSubresource xsubresource = {}, Offset3D xoffset = {}, Extent3D xextent = {}, DeviceMemory xmemory = {}, DeviceSize xmemoryOffset = {}, SparseMemoryBindFlags xflags = {}) : subresource(xsubresource), offset(xoffset), extent(xextent), memory(xmemory), memoryOffset(xmemoryOffset), flags(xflags) {}
	operator const VkSparseImageMemoryBind&() const { return reinterpret_cast<const VkSparseImageMemoryBind&>(*this); }
};
struct SparseImageMemoryBindInfo
{
	Image image {};
	uint32_t bindCount {};
	const SparseImageMemoryBind* pBinds {};

	SparseImageMemoryBindInfo(Image ximage = {}, uint32_t xbindCount = {}, const SparseImageMemoryBind* xpBinds = {}) : image(ximage), bindCount(xbindCount), pBinds(xpBinds) {}
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

	BindSparseInfo(uint32_t xwaitSemaphoreCount = {}, const Semaphore* xpWaitSemaphores = {}, uint32_t xbufferBindCount = {}, const SparseBufferMemoryBindInfo* xpBufferBinds = {}, uint32_t ximageOpaqueBindCount = {}, const SparseImageOpaqueMemoryBindInfo* xpImageOpaqueBinds = {}, uint32_t ximageBindCount = {}, const SparseImageMemoryBindInfo* xpImageBinds = {}, uint32_t xsignalSemaphoreCount = {}, const Semaphore* xpSignalSemaphores = {}) : waitSemaphoreCount(xwaitSemaphoreCount), pWaitSemaphores(xpWaitSemaphores), bufferBindCount(xbufferBindCount), pBufferBinds(xpBufferBinds), imageOpaqueBindCount(ximageOpaqueBindCount), pImageOpaqueBinds(xpImageOpaqueBinds), imageBindCount(ximageBindCount), pImageBinds(xpImageBinds), signalSemaphoreCount(xsignalSemaphoreCount), pSignalSemaphores(xpSignalSemaphores) {}
	operator const VkBindSparseInfo&() const { return reinterpret_cast<const VkBindSparseInfo&>(*this); }
};
struct FenceCreateInfo
{
	StructureType sType {StructureType::fenceCreateInfo};
	const void* pNext {};
	FenceCreateFlags flags {};

	FenceCreateInfo(FenceCreateFlags xflags = {}) : flags(xflags) {}
	operator const VkFenceCreateInfo&() const { return reinterpret_cast<const VkFenceCreateInfo&>(*this); }
};
struct SemaphoreCreateInfo
{
	StructureType sType {StructureType::semaphoreCreateInfo};
	const void* pNext {};
	SemaphoreCreateFlags flags {};

	SemaphoreCreateInfo(SemaphoreCreateFlags xflags = {}) : flags(xflags) {}
	operator const VkSemaphoreCreateInfo&() const { return reinterpret_cast<const VkSemaphoreCreateInfo&>(*this); }
};
struct EventCreateInfo
{
	StructureType sType {StructureType::eventCreateInfo};
	const void* pNext {};
	EventCreateFlags flags {};

	EventCreateInfo(EventCreateFlags xflags = {}) : flags(xflags) {}
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

	QueryPoolCreateInfo(QueryPoolCreateFlags xflags = {}, QueryType xqueryType = {}, uint32_t xqueryCount = {}, QueryPipelineStatisticFlags xpipelineStatistics = {}) : flags(xflags), queryType(xqueryType), queryCount(xqueryCount), pipelineStatistics(xpipelineStatistics) {}
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

	BufferCreateInfo(BufferCreateFlags xflags = {}, DeviceSize xsize = {}, BufferUsageFlags xusage = {}, SharingMode xsharingMode = {}, uint32_t xqueueFamilyIndexCount = {}, const uint32_t* xpQueueFamilyIndices = {}) : flags(xflags), size(xsize), usage(xusage), sharingMode(xsharingMode), queueFamilyIndexCount(xqueueFamilyIndexCount), pQueueFamilyIndices(xpQueueFamilyIndices) {}
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

	BufferViewCreateInfo(BufferViewCreateFlags xflags = {}, Buffer xbuffer = {}, Format xformat = {}, DeviceSize xoffset = {}, DeviceSize xrange = {}) : flags(xflags), buffer(xbuffer), format(xformat), offset(xoffset), range(xrange) {}
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

	ImageCreateInfo(ImageCreateFlags xflags = {}, ImageType ximageType = {}, Format xformat = {}, Extent3D xextent = {}, uint32_t xmipLevels = {}, uint32_t xarrayLayers = {}, SampleCountBits xsamples = {}, ImageTiling xtiling = {}, ImageUsageFlags xusage = {}, SharingMode xsharingMode = {}, uint32_t xqueueFamilyIndexCount = {}, const uint32_t* xpQueueFamilyIndices = {}, ImageLayout xinitialLayout = {}) : flags(xflags), imageType(ximageType), format(xformat), extent(xextent), mipLevels(xmipLevels), arrayLayers(xarrayLayers), samples(xsamples), tiling(xtiling), usage(xusage), sharingMode(xsharingMode), queueFamilyIndexCount(xqueueFamilyIndexCount), pQueueFamilyIndices(xpQueueFamilyIndices), initialLayout(xinitialLayout) {}
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

	ComponentMapping(ComponentSwizzle xr = {}, ComponentSwizzle xg = {}, ComponentSwizzle xb = {}, ComponentSwizzle xa = {}) : r(xr), g(xg), b(xb), a(xa) {}
	operator const VkComponentMapping&() const { return reinterpret_cast<const VkComponentMapping&>(*this); }
};
struct ImageSubresourceRange
{
	ImageAspectFlags aspectMask {};
	uint32_t baseMipLevel {};
	uint32_t levelCount {};
	uint32_t baseArrayLayer {};
	uint32_t layerCount {};

	ImageSubresourceRange(ImageAspectFlags xaspectMask = {}, uint32_t xbaseMipLevel = {}, uint32_t xlevelCount = {}, uint32_t xbaseArrayLayer = {}, uint32_t xlayerCount = {}) : aspectMask(xaspectMask), baseMipLevel(xbaseMipLevel), levelCount(xlevelCount), baseArrayLayer(xbaseArrayLayer), layerCount(xlayerCount) {}
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

	ImageViewCreateInfo(ImageViewCreateFlags xflags = {}, Image ximage = {}, ImageViewType xviewType = {}, Format xformat = {}, ComponentMapping xcomponents = {}, ImageSubresourceRange xsubresourceRange = {}) : flags(xflags), image(ximage), viewType(xviewType), format(xformat), components(xcomponents), subresourceRange(xsubresourceRange) {}
	operator const VkImageViewCreateInfo&() const { return reinterpret_cast<const VkImageViewCreateInfo&>(*this); }
};
struct ShaderModuleCreateInfo
{
	StructureType sType {StructureType::shaderModuleCreateInfo};
	const void* pNext {};
	ShaderModuleCreateFlags flags {};
	size_t codeSize {};
	const uint32_t* pCode {};

	ShaderModuleCreateInfo(ShaderModuleCreateFlags xflags = {}, size_t xcodeSize = {}, const uint32_t* xpCode = {}) : flags(xflags), codeSize(xcodeSize), pCode(xpCode) {}
	operator const VkShaderModuleCreateInfo&() const { return reinterpret_cast<const VkShaderModuleCreateInfo&>(*this); }
};
struct PipelineCacheCreateInfo
{
	StructureType sType {StructureType::pipelineCacheCreateInfo};
	const void* pNext {};
	PipelineCacheCreateFlags flags {};
	size_t initialDataSize {};
	const void* pInitialData {};

	PipelineCacheCreateInfo(PipelineCacheCreateFlags xflags = {}, size_t xinitialDataSize = {}, const void* xpInitialData = {}) : flags(xflags), initialDataSize(xinitialDataSize), pInitialData(xpInitialData) {}
	operator const VkPipelineCacheCreateInfo&() const { return reinterpret_cast<const VkPipelineCacheCreateInfo&>(*this); }
};
struct SpecializationMapEntry
{
	uint32_t constantID {};
	uint32_t offset {};
	size_t size {};

	SpecializationMapEntry(uint32_t xconstantID = {}, uint32_t xoffset = {}, size_t xsize = {}) : constantID(xconstantID), offset(xoffset), size(xsize) {}
	operator const VkSpecializationMapEntry&() const { return reinterpret_cast<const VkSpecializationMapEntry&>(*this); }
};
struct SpecializationInfo
{
	uint32_t mapEntryCount {};
	const SpecializationMapEntry* pMapEntries {};
	size_t dataSize {};
	const void* pData {};

	SpecializationInfo(uint32_t xmapEntryCount = {}, const SpecializationMapEntry* xpMapEntries = {}, size_t xdataSize = {}, const void* xpData = {}) : mapEntryCount(xmapEntryCount), pMapEntries(xpMapEntries), dataSize(xdataSize), pData(xpData) {}
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

	PipelineShaderStageCreateInfo(PipelineShaderStageCreateFlags xflags = {}, ShaderStageBits xstage = {}, ShaderModule xmodule = {}, const char* xpName = {}, const SpecializationInfo* xpSpecializationInfo = {}) : flags(xflags), stage(xstage), module(xmodule), pName(xpName), pSpecializationInfo(xpSpecializationInfo) {}
	operator const VkPipelineShaderStageCreateInfo&() const { return reinterpret_cast<const VkPipelineShaderStageCreateInfo&>(*this); }
};
struct VertexInputBindingDescription
{
	uint32_t binding {};
	uint32_t stride {};
	VertexInputRate inputRate {};

	VertexInputBindingDescription(uint32_t xbinding = {}, uint32_t xstride = {}, VertexInputRate xinputRate = {}) : binding(xbinding), stride(xstride), inputRate(xinputRate) {}
	operator const VkVertexInputBindingDescription&() const { return reinterpret_cast<const VkVertexInputBindingDescription&>(*this); }
};
struct VertexInputAttributeDescription
{
	uint32_t location {};
	uint32_t binding {};
	Format format {};
	uint32_t offset {};

	VertexInputAttributeDescription(uint32_t xlocation = {}, uint32_t xbinding = {}, Format xformat = {}, uint32_t xoffset = {}) : location(xlocation), binding(xbinding), format(xformat), offset(xoffset) {}
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

	PipelineVertexInputStateCreateInfo(PipelineVertexInputStateCreateFlags xflags = {}, uint32_t xvertexBindingDescriptionCount = {}, const VertexInputBindingDescription* xpVertexBindingDescriptions = {}, uint32_t xvertexAttributeDescriptionCount = {}, const VertexInputAttributeDescription* xpVertexAttributeDescriptions = {}) : flags(xflags), vertexBindingDescriptionCount(xvertexBindingDescriptionCount), pVertexBindingDescriptions(xpVertexBindingDescriptions), vertexAttributeDescriptionCount(xvertexAttributeDescriptionCount), pVertexAttributeDescriptions(xpVertexAttributeDescriptions) {}
	operator const VkPipelineVertexInputStateCreateInfo&() const { return reinterpret_cast<const VkPipelineVertexInputStateCreateInfo&>(*this); }
};
struct PipelineInputAssemblyStateCreateInfo
{
	StructureType sType {StructureType::pipelineInputAssemblyStateCreateInfo};
	const void* pNext {};
	PipelineInputAssemblyStateCreateFlags flags {};
	PrimitiveTopology topology {};
	Bool32 primitiveRestartEnable {};

	PipelineInputAssemblyStateCreateInfo(PipelineInputAssemblyStateCreateFlags xflags = {}, PrimitiveTopology xtopology = {}, Bool32 xprimitiveRestartEnable = {}) : flags(xflags), topology(xtopology), primitiveRestartEnable(xprimitiveRestartEnable) {}
	operator const VkPipelineInputAssemblyStateCreateInfo&() const { return reinterpret_cast<const VkPipelineInputAssemblyStateCreateInfo&>(*this); }
};
struct PipelineTessellationStateCreateInfo
{
	StructureType sType {StructureType::pipelineTessellationStateCreateInfo};
	const void* pNext {};
	PipelineTessellationStateCreateFlags flags {};
	uint32_t patchControlPoints {};

	PipelineTessellationStateCreateInfo(PipelineTessellationStateCreateFlags xflags = {}, uint32_t xpatchControlPoints = {}) : flags(xflags), patchControlPoints(xpatchControlPoints) {}
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

	Viewport(float xx = {}, float xy = {}, float xwidth = {}, float xheight = {}, float xminDepth = {}, float xmaxDepth = {}) : x(xx), y(xy), width(xwidth), height(xheight), minDepth(xminDepth), maxDepth(xmaxDepth) {}
	operator const VkViewport&() const { return reinterpret_cast<const VkViewport&>(*this); }
};
struct Offset2D
{
	int32_t x {};
	int32_t y {};

	Offset2D(int32_t xx = {}, int32_t xy = {}) : x(xx), y(xy) {}
	operator const VkOffset2D&() const { return reinterpret_cast<const VkOffset2D&>(*this); }
};
struct Extent2D
{
	uint32_t width {};
	uint32_t height {};

	Extent2D(uint32_t xwidth = {}, uint32_t xheight = {}) : width(xwidth), height(xheight) {}
	operator const VkExtent2D&() const { return reinterpret_cast<const VkExtent2D&>(*this); }
};
struct Rect2D
{
	Offset2D offset {};
	Extent2D extent {};

	Rect2D(Offset2D xoffset = {}, Extent2D xextent = {}) : offset(xoffset), extent(xextent) {}
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

	PipelineViewportStateCreateInfo(PipelineViewportStateCreateFlags xflags = {}, uint32_t xviewportCount = {}, const Viewport* xpViewports = {}, uint32_t xscissorCount = {}, const Rect2D* xpScissors = {}) : flags(xflags), viewportCount(xviewportCount), pViewports(xpViewports), scissorCount(xscissorCount), pScissors(xpScissors) {}
	operator const VkPipelineViewportStateCreateInfo&() const { return reinterpret_cast<const VkPipelineViewportStateCreateInfo&>(*this); }
};
struct PipelineRasterizationStateCreateInfo
{
	StructureType sType {StructureType::pipelineRasterizationStateCreateInfo};
	const void* pNext {};
	PipelineRasterizationStateCreateFlags flags {};
	Bool32 depthClampEnable {};
	Bool32 rasterizerDiscardEnable {};
	PolygonMode polygonMode {};
	CullModeFlags cullMode {};
	FrontFace frontFace {};
	Bool32 depthBiasEnable {};
	float depthBiasConstantFactor {};
	float depthBiasClamp {};
	float depthBiasSlopeFactor {};
	float lineWidth {};

	PipelineRasterizationStateCreateInfo(PipelineRasterizationStateCreateFlags xflags = {}, Bool32 xdepthClampEnable = {}, Bool32 xrasterizerDiscardEnable = {}, PolygonMode xpolygonMode = {}, CullModeFlags xcullMode = {}, FrontFace xfrontFace = {}, Bool32 xdepthBiasEnable = {}, float xdepthBiasConstantFactor = {}, float xdepthBiasClamp = {}, float xdepthBiasSlopeFactor = {}, float xlineWidth = {}) : flags(xflags), depthClampEnable(xdepthClampEnable), rasterizerDiscardEnable(xrasterizerDiscardEnable), polygonMode(xpolygonMode), cullMode(xcullMode), frontFace(xfrontFace), depthBiasEnable(xdepthBiasEnable), depthBiasConstantFactor(xdepthBiasConstantFactor), depthBiasClamp(xdepthBiasClamp), depthBiasSlopeFactor(xdepthBiasSlopeFactor), lineWidth(xlineWidth) {}
	operator const VkPipelineRasterizationStateCreateInfo&() const { return reinterpret_cast<const VkPipelineRasterizationStateCreateInfo&>(*this); }
};
struct PipelineMultisampleStateCreateInfo
{
	StructureType sType {StructureType::pipelineMultisampleStateCreateInfo};
	const void* pNext {};
	PipelineMultisampleStateCreateFlags flags {};
	SampleCountBits rasterizationSamples {};
	Bool32 sampleShadingEnable {};
	float minSampleShading {};
	const SampleMask* pSampleMask {};
	Bool32 alphaToCoverageEnable {};
	Bool32 alphaToOneEnable {};

	PipelineMultisampleStateCreateInfo(PipelineMultisampleStateCreateFlags xflags = {}, SampleCountBits xrasterizationSamples = {}, Bool32 xsampleShadingEnable = {}, float xminSampleShading = {}, const SampleMask* xpSampleMask = {}, Bool32 xalphaToCoverageEnable = {}, Bool32 xalphaToOneEnable = {}) : flags(xflags), rasterizationSamples(xrasterizationSamples), sampleShadingEnable(xsampleShadingEnable), minSampleShading(xminSampleShading), pSampleMask(xpSampleMask), alphaToCoverageEnable(xalphaToCoverageEnable), alphaToOneEnable(xalphaToOneEnable) {}
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

	StencilOpState(StencilOp xfailOp = {}, StencilOp xpassOp = {}, StencilOp xdepthFailOp = {}, CompareOp xcompareOp = {}, uint32_t xcompareMask = {}, uint32_t xwriteMask = {}, uint32_t xreference = {}) : failOp(xfailOp), passOp(xpassOp), depthFailOp(xdepthFailOp), compareOp(xcompareOp), compareMask(xcompareMask), writeMask(xwriteMask), reference(xreference) {}
	operator const VkStencilOpState&() const { return reinterpret_cast<const VkStencilOpState&>(*this); }
};
struct PipelineDepthStencilStateCreateInfo
{
	StructureType sType {StructureType::pipelineDepthStencilStateCreateInfo};
	const void* pNext {};
	PipelineDepthStencilStateCreateFlags flags {};
	Bool32 depthTestEnable {};
	Bool32 depthWriteEnable {};
	CompareOp depthCompareOp {};
	Bool32 depthBoundsTestEnable {};
	Bool32 stencilTestEnable {};
	StencilOpState front {};
	StencilOpState back {};
	float minDepthBounds {};
	float maxDepthBounds {};

	PipelineDepthStencilStateCreateInfo(PipelineDepthStencilStateCreateFlags xflags = {}, Bool32 xdepthTestEnable = {}, Bool32 xdepthWriteEnable = {}, CompareOp xdepthCompareOp = {}, Bool32 xdepthBoundsTestEnable = {}, Bool32 xstencilTestEnable = {}, StencilOpState xfront = {}, StencilOpState xback = {}, float xminDepthBounds = {}, float xmaxDepthBounds = {}) : flags(xflags), depthTestEnable(xdepthTestEnable), depthWriteEnable(xdepthWriteEnable), depthCompareOp(xdepthCompareOp), depthBoundsTestEnable(xdepthBoundsTestEnable), stencilTestEnable(xstencilTestEnable), front(xfront), back(xback), minDepthBounds(xminDepthBounds), maxDepthBounds(xmaxDepthBounds) {}
	operator const VkPipelineDepthStencilStateCreateInfo&() const { return reinterpret_cast<const VkPipelineDepthStencilStateCreateInfo&>(*this); }
};
struct PipelineColorBlendAttachmentState
{
	Bool32 blendEnable {};
	BlendFactor srcColorBlendFactor {};
	BlendFactor dstColorBlendFactor {};
	BlendOp colorBlendOp {};
	BlendFactor srcAlphaBlendFactor {};
	BlendFactor dstAlphaBlendFactor {};
	BlendOp alphaBlendOp {};
	ColorComponentFlags colorWriteMask {};

	PipelineColorBlendAttachmentState(Bool32 xblendEnable = {}, BlendFactor xsrcColorBlendFactor = {}, BlendFactor xdstColorBlendFactor = {}, BlendOp xcolorBlendOp = {}, BlendFactor xsrcAlphaBlendFactor = {}, BlendFactor xdstAlphaBlendFactor = {}, BlendOp xalphaBlendOp = {}, ColorComponentFlags xcolorWriteMask = {}) : blendEnable(xblendEnable), srcColorBlendFactor(xsrcColorBlendFactor), dstColorBlendFactor(xdstColorBlendFactor), colorBlendOp(xcolorBlendOp), srcAlphaBlendFactor(xsrcAlphaBlendFactor), dstAlphaBlendFactor(xdstAlphaBlendFactor), alphaBlendOp(xalphaBlendOp), colorWriteMask(xcolorWriteMask) {}
	operator const VkPipelineColorBlendAttachmentState&() const { return reinterpret_cast<const VkPipelineColorBlendAttachmentState&>(*this); }
};
struct PipelineColorBlendStateCreateInfo
{
	StructureType sType {StructureType::pipelineColorBlendStateCreateInfo};
	const void* pNext {};
	PipelineColorBlendStateCreateFlags flags {};
	Bool32 logicOpEnable {};
	LogicOp logicOp {};
	uint32_t attachmentCount {};
	const PipelineColorBlendAttachmentState* pAttachments {};
	float blendConstants[4] {};

	PipelineColorBlendStateCreateInfo(PipelineColorBlendStateCreateFlags xflags = {}, Bool32 xlogicOpEnable = {}, LogicOp xlogicOp = {}, uint32_t xattachmentCount = {}, const PipelineColorBlendAttachmentState* xpAttachments = {}, float xblendConstants = {}) : flags(xflags), logicOpEnable(xlogicOpEnable), logicOp(xlogicOp), attachmentCount(xattachmentCount), pAttachments(xpAttachments), blendConstants(xblendConstants) {}
	operator const VkPipelineColorBlendStateCreateInfo&() const { return reinterpret_cast<const VkPipelineColorBlendStateCreateInfo&>(*this); }
};
struct PipelineDynamicStateCreateInfo
{
	StructureType sType {StructureType::pipelineDynamicStateCreateInfo};
	const void* pNext {};
	PipelineDynamicStateCreateFlags flags {};
	uint32_t dynamicStateCount {};
	const DynamicState* pDynamicStates {};

	PipelineDynamicStateCreateInfo(PipelineDynamicStateCreateFlags xflags = {}, uint32_t xdynamicStateCount = {}, const DynamicState* xpDynamicStates = {}) : flags(xflags), dynamicStateCount(xdynamicStateCount), pDynamicStates(xpDynamicStates) {}
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

	GraphicsPipelineCreateInfo(PipelineCreateFlags xflags = {}, uint32_t xstageCount = {}, const PipelineShaderStageCreateInfo* xpStages = {}, const PipelineVertexInputStateCreateInfo* xpVertexInputState = {}, const PipelineInputAssemblyStateCreateInfo* xpInputAssemblyState = {}, const PipelineTessellationStateCreateInfo* xpTessellationState = {}, const PipelineViewportStateCreateInfo* xpViewportState = {}, const PipelineRasterizationStateCreateInfo* xpRasterizationState = {}, const PipelineMultisampleStateCreateInfo* xpMultisampleState = {}, const PipelineDepthStencilStateCreateInfo* xpDepthStencilState = {}, const PipelineColorBlendStateCreateInfo* xpColorBlendState = {}, const PipelineDynamicStateCreateInfo* xpDynamicState = {}, PipelineLayout xlayout = {}, RenderPass xrenderPass = {}, uint32_t xsubpass = {}, Pipeline xbasePipelineHandle = {}, int32_t xbasePipelineIndex = {}) : flags(xflags), stageCount(xstageCount), pStages(xpStages), pVertexInputState(xpVertexInputState), pInputAssemblyState(xpInputAssemblyState), pTessellationState(xpTessellationState), pViewportState(xpViewportState), pRasterizationState(xpRasterizationState), pMultisampleState(xpMultisampleState), pDepthStencilState(xpDepthStencilState), pColorBlendState(xpColorBlendState), pDynamicState(xpDynamicState), layout(xlayout), renderPass(xrenderPass), subpass(xsubpass), basePipelineHandle(xbasePipelineHandle), basePipelineIndex(xbasePipelineIndex) {}
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

	ComputePipelineCreateInfo(PipelineCreateFlags xflags = {}, PipelineShaderStageCreateInfo xstage = {}, PipelineLayout xlayout = {}, Pipeline xbasePipelineHandle = {}, int32_t xbasePipelineIndex = {}) : flags(xflags), stage(xstage), layout(xlayout), basePipelineHandle(xbasePipelineHandle), basePipelineIndex(xbasePipelineIndex) {}
	operator const VkComputePipelineCreateInfo&() const { return reinterpret_cast<const VkComputePipelineCreateInfo&>(*this); }
};
struct PushConstantRange
{
	ShaderStageFlags stageFlags {};
	uint32_t offset {};
	uint32_t size {};

	PushConstantRange(ShaderStageFlags xstageFlags = {}, uint32_t xoffset = {}, uint32_t xsize = {}) : stageFlags(xstageFlags), offset(xoffset), size(xsize) {}
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

	PipelineLayoutCreateInfo(PipelineLayoutCreateFlags xflags = {}, uint32_t xsetLayoutCount = {}, const DescriptorSetLayout* xpSetLayouts = {}, uint32_t xpushConstantRangeCount = {}, const PushConstantRange* xpPushConstantRanges = {}) : flags(xflags), setLayoutCount(xsetLayoutCount), pSetLayouts(xpSetLayouts), pushConstantRangeCount(xpushConstantRangeCount), pPushConstantRanges(xpPushConstantRanges) {}
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
	Bool32 anisotropyEnable {};
	float maxAnisotropy {};
	Bool32 compareEnable {};
	CompareOp compareOp {};
	float minLod {};
	float maxLod {};
	BorderColor borderColor {};
	Bool32 unnormalizedCoordinates {};

	SamplerCreateInfo(SamplerCreateFlags xflags = {}, Filter xmagFilter = {}, Filter xminFilter = {}, SamplerMipmapMode xmipmapMode = {}, SamplerAddressMode xaddressModeU = {}, SamplerAddressMode xaddressModeV = {}, SamplerAddressMode xaddressModeW = {}, float xmipLodBias = {}, Bool32 xanisotropyEnable = {}, float xmaxAnisotropy = {}, Bool32 xcompareEnable = {}, CompareOp xcompareOp = {}, float xminLod = {}, float xmaxLod = {}, BorderColor xborderColor = {}, Bool32 xunnormalizedCoordinates = {}) : flags(xflags), magFilter(xmagFilter), minFilter(xminFilter), mipmapMode(xmipmapMode), addressModeU(xaddressModeU), addressModeV(xaddressModeV), addressModeW(xaddressModeW), mipLodBias(xmipLodBias), anisotropyEnable(xanisotropyEnable), maxAnisotropy(xmaxAnisotropy), compareEnable(xcompareEnable), compareOp(xcompareOp), minLod(xminLod), maxLod(xmaxLod), borderColor(xborderColor), unnormalizedCoordinates(xunnormalizedCoordinates) {}
	operator const VkSamplerCreateInfo&() const { return reinterpret_cast<const VkSamplerCreateInfo&>(*this); }
};
struct DescriptorSetLayoutBinding
{
	uint32_t binding {};
	DescriptorType descriptorType {};
	uint32_t descriptorCount {};
	ShaderStageFlags stageFlags {};
	const Sampler* pImmutableSamplers {};

	DescriptorSetLayoutBinding(uint32_t xbinding = {}, DescriptorType xdescriptorType = {}, uint32_t xdescriptorCount = {}, ShaderStageFlags xstageFlags = {}, const Sampler* xpImmutableSamplers = {}) : binding(xbinding), descriptorType(xdescriptorType), descriptorCount(xdescriptorCount), stageFlags(xstageFlags), pImmutableSamplers(xpImmutableSamplers) {}
	operator const VkDescriptorSetLayoutBinding&() const { return reinterpret_cast<const VkDescriptorSetLayoutBinding&>(*this); }
};
struct DescriptorSetLayoutCreateInfo
{
	StructureType sType {StructureType::descriptorSetLayoutCreateInfo};
	const void* pNext {};
	DescriptorSetLayoutCreateFlags flags {};
	uint32_t bindingCount {};
	const DescriptorSetLayoutBinding* pBindings {};

	DescriptorSetLayoutCreateInfo(DescriptorSetLayoutCreateFlags xflags = {}, uint32_t xbindingCount = {}, const DescriptorSetLayoutBinding* xpBindings = {}) : flags(xflags), bindingCount(xbindingCount), pBindings(xpBindings) {}
	operator const VkDescriptorSetLayoutCreateInfo&() const { return reinterpret_cast<const VkDescriptorSetLayoutCreateInfo&>(*this); }
};
struct DescriptorPoolSize
{
	DescriptorType type {};
	uint32_t descriptorCount {};

	DescriptorPoolSize(DescriptorType xtype = {}, uint32_t xdescriptorCount = {}) : type(xtype), descriptorCount(xdescriptorCount) {}
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

	DescriptorPoolCreateInfo(DescriptorPoolCreateFlags xflags = {}, uint32_t xmaxSets = {}, uint32_t xpoolSizeCount = {}, const DescriptorPoolSize* xpPoolSizes = {}) : flags(xflags), maxSets(xmaxSets), poolSizeCount(xpoolSizeCount), pPoolSizes(xpPoolSizes) {}
	operator const VkDescriptorPoolCreateInfo&() const { return reinterpret_cast<const VkDescriptorPoolCreateInfo&>(*this); }
};
struct DescriptorSetAllocateInfo
{
	StructureType sType {StructureType::descriptorSetAllocateInfo};
	const void* pNext {};
	DescriptorPool descriptorPool {};
	uint32_t descriptorSetCount {};
	const DescriptorSetLayout* pSetLayouts {};

	DescriptorSetAllocateInfo(DescriptorPool xdescriptorPool = {}, uint32_t xdescriptorSetCount = {}, const DescriptorSetLayout* xpSetLayouts = {}) : descriptorPool(xdescriptorPool), descriptorSetCount(xdescriptorSetCount), pSetLayouts(xpSetLayouts) {}
	operator const VkDescriptorSetAllocateInfo&() const { return reinterpret_cast<const VkDescriptorSetAllocateInfo&>(*this); }
};
struct DescriptorImageInfo
{
	Sampler sampler {};
	ImageView imageView {};
	ImageLayout imageLayout {};

	DescriptorImageInfo(Sampler xsampler = {}, ImageView ximageView = {}, ImageLayout ximageLayout = {}) : sampler(xsampler), imageView(ximageView), imageLayout(ximageLayout) {}
	operator const VkDescriptorImageInfo&() const { return reinterpret_cast<const VkDescriptorImageInfo&>(*this); }
};
struct DescriptorBufferInfo
{
	Buffer buffer {};
	DeviceSize offset {};
	DeviceSize range {};

	DescriptorBufferInfo(Buffer xbuffer = {}, DeviceSize xoffset = {}, DeviceSize xrange = {}) : buffer(xbuffer), offset(xoffset), range(xrange) {}
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

	WriteDescriptorSet(DescriptorSet xdstSet = {}, uint32_t xdstBinding = {}, uint32_t xdstArrayElement = {}, uint32_t xdescriptorCount = {}, DescriptorType xdescriptorType = {}, const DescriptorImageInfo* xpImageInfo = {}, const DescriptorBufferInfo* xpBufferInfo = {}, const BufferView* xpTexelBufferView = {}) : dstSet(xdstSet), dstBinding(xdstBinding), dstArrayElement(xdstArrayElement), descriptorCount(xdescriptorCount), descriptorType(xdescriptorType), pImageInfo(xpImageInfo), pBufferInfo(xpBufferInfo), pTexelBufferView(xpTexelBufferView) {}
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

	CopyDescriptorSet(DescriptorSet xsrcSet = {}, uint32_t xsrcBinding = {}, uint32_t xsrcArrayElement = {}, DescriptorSet xdstSet = {}, uint32_t xdstBinding = {}, uint32_t xdstArrayElement = {}, uint32_t xdescriptorCount = {}) : srcSet(xsrcSet), srcBinding(xsrcBinding), srcArrayElement(xsrcArrayElement), dstSet(xdstSet), dstBinding(xdstBinding), dstArrayElement(xdstArrayElement), descriptorCount(xdescriptorCount) {}
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

	FramebufferCreateInfo(FramebufferCreateFlags xflags = {}, RenderPass xrenderPass = {}, uint32_t xattachmentCount = {}, const ImageView* xpAttachments = {}, uint32_t xwidth = {}, uint32_t xheight = {}, uint32_t xlayers = {}) : flags(xflags), renderPass(xrenderPass), attachmentCount(xattachmentCount), pAttachments(xpAttachments), width(xwidth), height(xheight), layers(xlayers) {}
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

	AttachmentDescription(AttachmentDescriptionFlags xflags = {}, Format xformat = {}, SampleCountBits xsamples = {}, AttachmentLoadOp xloadOp = {}, AttachmentStoreOp xstoreOp = {}, AttachmentLoadOp xstencilLoadOp = {}, AttachmentStoreOp xstencilStoreOp = {}, ImageLayout xinitialLayout = {}, ImageLayout xfinalLayout = {}) : flags(xflags), format(xformat), samples(xsamples), loadOp(xloadOp), storeOp(xstoreOp), stencilLoadOp(xstencilLoadOp), stencilStoreOp(xstencilStoreOp), initialLayout(xinitialLayout), finalLayout(xfinalLayout) {}
	operator const VkAttachmentDescription&() const { return reinterpret_cast<const VkAttachmentDescription&>(*this); }
};
struct AttachmentReference
{
	uint32_t attachment {};
	ImageLayout layout {};

	AttachmentReference(uint32_t xattachment = {}, ImageLayout xlayout = {}) : attachment(xattachment), layout(xlayout) {}
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

	SubpassDescription(SubpassDescriptionFlags xflags = {}, PipelineBindPoint xpipelineBindPoint = {}, uint32_t xinputAttachmentCount = {}, const AttachmentReference* xpInputAttachments = {}, uint32_t xcolorAttachmentCount = {}, const AttachmentReference* xpColorAttachments = {}, const AttachmentReference* xpResolveAttachments = {}, const AttachmentReference* xpDepthStencilAttachment = {}, uint32_t xpreserveAttachmentCount = {}, const uint32_t* xpPreserveAttachments = {}) : flags(xflags), pipelineBindPoint(xpipelineBindPoint), inputAttachmentCount(xinputAttachmentCount), pInputAttachments(xpInputAttachments), colorAttachmentCount(xcolorAttachmentCount), pColorAttachments(xpColorAttachments), pResolveAttachments(xpResolveAttachments), pDepthStencilAttachment(xpDepthStencilAttachment), preserveAttachmentCount(xpreserveAttachmentCount), pPreserveAttachments(xpPreserveAttachments) {}
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

	SubpassDependency(uint32_t xsrcSubpass = {}, uint32_t xdstSubpass = {}, PipelineStageFlags xsrcStageMask = {}, PipelineStageFlags xdstStageMask = {}, AccessFlags xsrcAccessMask = {}, AccessFlags xdstAccessMask = {}, DependencyFlags xdependencyFlags = {}) : srcSubpass(xsrcSubpass), dstSubpass(xdstSubpass), srcStageMask(xsrcStageMask), dstStageMask(xdstStageMask), srcAccessMask(xsrcAccessMask), dstAccessMask(xdstAccessMask), dependencyFlags(xdependencyFlags) {}
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

	RenderPassCreateInfo(RenderPassCreateFlags xflags = {}, uint32_t xattachmentCount = {}, const AttachmentDescription* xpAttachments = {}, uint32_t xsubpassCount = {}, const SubpassDescription* xpSubpasses = {}, uint32_t xdependencyCount = {}, const SubpassDependency* xpDependencies = {}) : flags(xflags), attachmentCount(xattachmentCount), pAttachments(xpAttachments), subpassCount(xsubpassCount), pSubpasses(xpSubpasses), dependencyCount(xdependencyCount), pDependencies(xpDependencies) {}
	operator const VkRenderPassCreateInfo&() const { return reinterpret_cast<const VkRenderPassCreateInfo&>(*this); }
};
struct CommandPoolCreateInfo
{
	StructureType sType {StructureType::commandPoolCreateInfo};
	const void* pNext {};
	CommandPoolCreateFlags flags {};
	uint32_t queueFamilyIndex {};

	CommandPoolCreateInfo(CommandPoolCreateFlags xflags = {}, uint32_t xqueueFamilyIndex = {}) : flags(xflags), queueFamilyIndex(xqueueFamilyIndex) {}
	operator const VkCommandPoolCreateInfo&() const { return reinterpret_cast<const VkCommandPoolCreateInfo&>(*this); }
};
struct CommandBufferAllocateInfo
{
	StructureType sType {StructureType::commandBufferAllocateInfo};
	const void* pNext {};
	CommandPool commandPool {};
	CommandBufferLevel level {};
	uint32_t commandBufferCount {};

	CommandBufferAllocateInfo(CommandPool xcommandPool = {}, CommandBufferLevel xlevel = {}, uint32_t xcommandBufferCount = {}) : commandPool(xcommandPool), level(xlevel), commandBufferCount(xcommandBufferCount) {}
	operator const VkCommandBufferAllocateInfo&() const { return reinterpret_cast<const VkCommandBufferAllocateInfo&>(*this); }
};
struct CommandBufferInheritanceInfo
{
	StructureType sType {StructureType::commandBufferInheritanceInfo};
	const void* pNext {};
	RenderPass renderPass {};
	uint32_t subpass {};
	Framebuffer framebuffer {};
	Bool32 occlusionQueryEnable {};
	QueryControlFlags queryFlags {};
	QueryPipelineStatisticFlags pipelineStatistics {};

	CommandBufferInheritanceInfo(RenderPass xrenderPass = {}, uint32_t xsubpass = {}, Framebuffer xframebuffer = {}, Bool32 xocclusionQueryEnable = {}, QueryControlFlags xqueryFlags = {}, QueryPipelineStatisticFlags xpipelineStatistics = {}) : renderPass(xrenderPass), subpass(xsubpass), framebuffer(xframebuffer), occlusionQueryEnable(xocclusionQueryEnable), queryFlags(xqueryFlags), pipelineStatistics(xpipelineStatistics) {}
	operator const VkCommandBufferInheritanceInfo&() const { return reinterpret_cast<const VkCommandBufferInheritanceInfo&>(*this); }
};
struct CommandBufferBeginInfo
{
	StructureType sType {StructureType::commandBufferBeginInfo};
	const void* pNext {};
	CommandBufferUsageFlags flags {};
	const CommandBufferInheritanceInfo* pInheritanceInfo {};

	CommandBufferBeginInfo(CommandBufferUsageFlags xflags = {}, const CommandBufferInheritanceInfo* xpInheritanceInfo = {}) : flags(xflags), pInheritanceInfo(xpInheritanceInfo) {}
	operator const VkCommandBufferBeginInfo&() const { return reinterpret_cast<const VkCommandBufferBeginInfo&>(*this); }
};
struct BufferCopy
{
	DeviceSize srcOffset {};
	DeviceSize dstOffset {};
	DeviceSize size {};

	BufferCopy(DeviceSize xsrcOffset = {}, DeviceSize xdstOffset = {}, DeviceSize xsize = {}) : srcOffset(xsrcOffset), dstOffset(xdstOffset), size(xsize) {}
	operator const VkBufferCopy&() const { return reinterpret_cast<const VkBufferCopy&>(*this); }
};
struct ImageSubresourceLayers
{
	ImageAspectFlags aspectMask {};
	uint32_t mipLevel {};
	uint32_t baseArrayLayer {};
	uint32_t layerCount {};

	ImageSubresourceLayers(ImageAspectFlags xaspectMask = {}, uint32_t xmipLevel = {}, uint32_t xbaseArrayLayer = {}, uint32_t xlayerCount = {}) : aspectMask(xaspectMask), mipLevel(xmipLevel), baseArrayLayer(xbaseArrayLayer), layerCount(xlayerCount) {}
	operator const VkImageSubresourceLayers&() const { return reinterpret_cast<const VkImageSubresourceLayers&>(*this); }
};
struct ImageCopy
{
	ImageSubresourceLayers srcSubresource {};
	Offset3D srcOffset {};
	ImageSubresourceLayers dstSubresource {};
	Offset3D dstOffset {};
	Extent3D extent {};

	ImageCopy(ImageSubresourceLayers xsrcSubresource = {}, Offset3D xsrcOffset = {}, ImageSubresourceLayers xdstSubresource = {}, Offset3D xdstOffset = {}, Extent3D xextent = {}) : srcSubresource(xsrcSubresource), srcOffset(xsrcOffset), dstSubresource(xdstSubresource), dstOffset(xdstOffset), extent(xextent) {}
	operator const VkImageCopy&() const { return reinterpret_cast<const VkImageCopy&>(*this); }
};
struct ImageBlit
{
	ImageSubresourceLayers srcSubresource {};
	Offset3D srcOffsets[2] {};
	ImageSubresourceLayers dstSubresource {};
	Offset3D dstOffsets[2] {};

	ImageBlit(ImageSubresourceLayers xsrcSubresource = {}, Offset3D xsrcOffsets = {}, ImageSubresourceLayers xdstSubresource = {}, Offset3D xdstOffsets = {}) : srcSubresource(xsrcSubresource), srcOffsets(xsrcOffsets), dstSubresource(xdstSubresource), dstOffsets(xdstOffsets) {}
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

	BufferImageCopy(DeviceSize xbufferOffset = {}, uint32_t xbufferRowLength = {}, uint32_t xbufferImageHeight = {}, ImageSubresourceLayers ximageSubresource = {}, Offset3D ximageOffset = {}, Extent3D ximageExtent = {}) : bufferOffset(xbufferOffset), bufferRowLength(xbufferRowLength), bufferImageHeight(xbufferImageHeight), imageSubresource(ximageSubresource), imageOffset(ximageOffset), imageExtent(ximageExtent) {}
	operator const VkBufferImageCopy&() const { return reinterpret_cast<const VkBufferImageCopy&>(*this); }
};
union ClearColorValue
{
	float float32[4] {};
	int32_t int32[4];
	uint32_t uint32[4];

	ClearColorValue(float xfloat32 = {}, int32_t xint32 = {}, uint32_t xuint32 = {}) : float32(xfloat32), int32(xint32), uint32(xuint32) {}
	operator const VkClearColorValue&() const { return reinterpret_cast<const VkClearColorValue&>(*this); }
};
struct ClearDepthStencilValue
{
	float depth {};
	uint32_t stencil {};

	ClearDepthStencilValue(float xdepth = {}, uint32_t xstencil = {}) : depth(xdepth), stencil(xstencil) {}
	operator const VkClearDepthStencilValue&() const { return reinterpret_cast<const VkClearDepthStencilValue&>(*this); }
};
union ClearValue
{
	ClearColorValue color {};
	ClearDepthStencilValue depthStencil;

	ClearValue(ClearColorValue xcolor = {}, ClearDepthStencilValue xdepthStencil = {}) : color(xcolor), depthStencil(xdepthStencil) {}
	operator const VkClearValue&() const { return reinterpret_cast<const VkClearValue&>(*this); }
};
struct ClearAttachment
{
	ImageAspectFlags aspectMask {};
	uint32_t colorAttachment {};
	ClearValue clearValue {};

	ClearAttachment(ImageAspectFlags xaspectMask = {}, uint32_t xcolorAttachment = {}, ClearValue xclearValue = {}) : aspectMask(xaspectMask), colorAttachment(xcolorAttachment), clearValue(xclearValue) {}
	operator const VkClearAttachment&() const { return reinterpret_cast<const VkClearAttachment&>(*this); }
};
struct ClearRect
{
	Rect2D rect {};
	uint32_t baseArrayLayer {};
	uint32_t layerCount {};

	ClearRect(Rect2D xrect = {}, uint32_t xbaseArrayLayer = {}, uint32_t xlayerCount = {}) : rect(xrect), baseArrayLayer(xbaseArrayLayer), layerCount(xlayerCount) {}
	operator const VkClearRect&() const { return reinterpret_cast<const VkClearRect&>(*this); }
};
struct ImageResolve
{
	ImageSubresourceLayers srcSubresource {};
	Offset3D srcOffset {};
	ImageSubresourceLayers dstSubresource {};
	Offset3D dstOffset {};
	Extent3D extent {};

	ImageResolve(ImageSubresourceLayers xsrcSubresource = {}, Offset3D xsrcOffset = {}, ImageSubresourceLayers xdstSubresource = {}, Offset3D xdstOffset = {}, Extent3D xextent = {}) : srcSubresource(xsrcSubresource), srcOffset(xsrcOffset), dstSubresource(xdstSubresource), dstOffset(xdstOffset), extent(xextent) {}
	operator const VkImageResolve&() const { return reinterpret_cast<const VkImageResolve&>(*this); }
};
struct MemoryBarrier
{
	StructureType sType {StructureType::memoryBarrier};
	const void* pNext {};
	AccessFlags srcAccessMask {};
	AccessFlags dstAccessMask {};

	MemoryBarrier(AccessFlags xsrcAccessMask = {}, AccessFlags xdstAccessMask = {}) : srcAccessMask(xsrcAccessMask), dstAccessMask(xdstAccessMask) {}
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

	BufferMemoryBarrier(AccessFlags xsrcAccessMask = {}, AccessFlags xdstAccessMask = {}, uint32_t xsrcQueueFamilyIndex = {}, uint32_t xdstQueueFamilyIndex = {}, Buffer xbuffer = {}, DeviceSize xoffset = {}, DeviceSize xsize = {}) : srcAccessMask(xsrcAccessMask), dstAccessMask(xdstAccessMask), srcQueueFamilyIndex(xsrcQueueFamilyIndex), dstQueueFamilyIndex(xdstQueueFamilyIndex), buffer(xbuffer), offset(xoffset), size(xsize) {}
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

	ImageMemoryBarrier(AccessFlags xsrcAccessMask = {}, AccessFlags xdstAccessMask = {}, ImageLayout xoldLayout = {}, ImageLayout xnewLayout = {}, uint32_t xsrcQueueFamilyIndex = {}, uint32_t xdstQueueFamilyIndex = {}, Image ximage = {}, ImageSubresourceRange xsubresourceRange = {}) : srcAccessMask(xsrcAccessMask), dstAccessMask(xdstAccessMask), oldLayout(xoldLayout), newLayout(xnewLayout), srcQueueFamilyIndex(xsrcQueueFamilyIndex), dstQueueFamilyIndex(xdstQueueFamilyIndex), image(ximage), subresourceRange(xsubresourceRange) {}
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

	RenderPassBeginInfo(RenderPass xrenderPass = {}, Framebuffer xframebuffer = {}, Rect2D xrenderArea = {}, uint32_t xclearValueCount = {}, const ClearValue* xpClearValues = {}) : renderPass(xrenderPass), framebuffer(xframebuffer), renderArea(xrenderArea), clearValueCount(xclearValueCount), pClearValues(xpClearValues) {}
	operator const VkRenderPassBeginInfo&() const { return reinterpret_cast<const VkRenderPassBeginInfo&>(*this); }
};
struct DispatchIndirectCommand
{
	uint32_t x {};
	uint32_t y {};
	uint32_t z {};

	DispatchIndirectCommand(uint32_t xx = {}, uint32_t xy = {}, uint32_t xz = {}) : x(xx), y(xy), z(xz) {}
	operator const VkDispatchIndirectCommand&() const { return reinterpret_cast<const VkDispatchIndirectCommand&>(*this); }
};
struct DrawIndexedIndirectCommand
{
	uint32_t indexCount {};
	uint32_t instanceCount {};
	uint32_t firstIndex {};
	int32_t vertexOffset {};
	uint32_t firstInstance {};

	DrawIndexedIndirectCommand(uint32_t xindexCount = {}, uint32_t xinstanceCount = {}, uint32_t xfirstIndex = {}, int32_t xvertexOffset = {}, uint32_t xfirstInstance = {}) : indexCount(xindexCount), instanceCount(xinstanceCount), firstIndex(xfirstIndex), vertexOffset(xvertexOffset), firstInstance(xfirstInstance) {}
	operator const VkDrawIndexedIndirectCommand&() const { return reinterpret_cast<const VkDrawIndexedIndirectCommand&>(*this); }
};
struct DrawIndirectCommand
{
	uint32_t vertexCount {};
	uint32_t instanceCount {};
	uint32_t firstVertex {};
	uint32_t firstInstance {};

	DrawIndirectCommand(uint32_t xvertexCount = {}, uint32_t xinstanceCount = {}, uint32_t xfirstVertex = {}, uint32_t xfirstInstance = {}) : vertexCount(xvertexCount), instanceCount(xinstanceCount), firstVertex(xfirstVertex), firstInstance(xfirstInstance) {}
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

	SurfaceCapabilitiesKHR(uint32_t xminImageCount = {}, uint32_t xmaxImageCount = {}, Extent2D xcurrentExtent = {}, Extent2D xminImageExtent = {}, Extent2D xmaxImageExtent = {}, uint32_t xmaxImageArrayLayers = {}, SurfaceTransformFlagsKHR xsupportedTransforms = {}, SurfaceTransformBitsKHR xcurrentTransform = {}, CompositeAlphaFlagsKHR xsupportedCompositeAlpha = {}, ImageUsageFlags xsupportedUsageFlags = {}) : minImageCount(xminImageCount), maxImageCount(xmaxImageCount), currentExtent(xcurrentExtent), minImageExtent(xminImageExtent), maxImageExtent(xmaxImageExtent), maxImageArrayLayers(xmaxImageArrayLayers), supportedTransforms(xsupportedTransforms), currentTransform(xcurrentTransform), supportedCompositeAlpha(xsupportedCompositeAlpha), supportedUsageFlags(xsupportedUsageFlags) {}
	operator const VkSurfaceCapabilitiesKHR&() const { return reinterpret_cast<const VkSurfaceCapabilitiesKHR&>(*this); }
};
struct SurfaceFormatKHR
{
	Format format {};
	ColorSpaceKHR colorSpace {};

	SurfaceFormatKHR(Format xformat = {}, ColorSpaceKHR xcolorSpace = {}) : format(xformat), colorSpace(xcolorSpace) {}
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
	Bool32 clipped {};
	SwapchainKHR oldSwapchain {};

	SwapchainCreateInfoKHR(SwapchainCreateFlagsKHR xflags = {}, SurfaceKHR xsurface = {}, uint32_t xminImageCount = {}, Format ximageFormat = {}, ColorSpaceKHR ximageColorSpace = {}, Extent2D ximageExtent = {}, uint32_t ximageArrayLayers = {}, ImageUsageFlags ximageUsage = {}, SharingMode ximageSharingMode = {}, uint32_t xqueueFamilyIndexCount = {}, const uint32_t* xpQueueFamilyIndices = {}, SurfaceTransformBitsKHR xpreTransform = {}, CompositeAlphaBitsKHR xcompositeAlpha = {}, PresentModeKHR xpresentMode = {}, Bool32 xclipped = {}, SwapchainKHR xoldSwapchain = {}) : flags(xflags), surface(xsurface), minImageCount(xminImageCount), imageFormat(ximageFormat), imageColorSpace(ximageColorSpace), imageExtent(ximageExtent), imageArrayLayers(ximageArrayLayers), imageUsage(ximageUsage), imageSharingMode(ximageSharingMode), queueFamilyIndexCount(xqueueFamilyIndexCount), pQueueFamilyIndices(xpQueueFamilyIndices), preTransform(xpreTransform), compositeAlpha(xcompositeAlpha), presentMode(xpresentMode), clipped(xclipped), oldSwapchain(xoldSwapchain) {}
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

	PresentInfoKHR(uint32_t xwaitSemaphoreCount = {}, const Semaphore* xpWaitSemaphores = {}, uint32_t xswapchainCount = {}, const SwapchainKHR* xpSwapchains = {}, const uint32_t* xpImageIndices = {}, Result* xpResults = {}) : waitSemaphoreCount(xwaitSemaphoreCount), pWaitSemaphores(xpWaitSemaphores), swapchainCount(xswapchainCount), pSwapchains(xpSwapchains), pImageIndices(xpImageIndices), pResults(xpResults) {}
	operator const VkPresentInfoKHR&() const { return reinterpret_cast<const VkPresentInfoKHR&>(*this); }
};

struct DisplayPropertiesKHR
{
	DisplayKHR display {};
	const char* displayName {};
	Extent2D physicalDimensions {};
	Extent2D physicalResolution {};
	SurfaceTransformFlagsKHR supportedTransforms {};
	Bool32 planeReorderPossible {};
	Bool32 persistentContent {};

	DisplayPropertiesKHR(DisplayKHR xdisplay = {}, const char* xdisplayName = {}, Extent2D xphysicalDimensions = {}, Extent2D xphysicalResolution = {}, SurfaceTransformFlagsKHR xsupportedTransforms = {}, Bool32 xplaneReorderPossible = {}, Bool32 xpersistentContent = {}) : display(xdisplay), displayName(xdisplayName), physicalDimensions(xphysicalDimensions), physicalResolution(xphysicalResolution), supportedTransforms(xsupportedTransforms), planeReorderPossible(xplaneReorderPossible), persistentContent(xpersistentContent) {}
	operator const VkDisplayPropertiesKHR&() const { return reinterpret_cast<const VkDisplayPropertiesKHR&>(*this); }
};
struct DisplayModeParametersKHR
{
	Extent2D visibleRegion {};
	uint32_t refreshRate {};

	DisplayModeParametersKHR(Extent2D xvisibleRegion = {}, uint32_t xrefreshRate = {}) : visibleRegion(xvisibleRegion), refreshRate(xrefreshRate) {}
	operator const VkDisplayModeParametersKHR&() const { return reinterpret_cast<const VkDisplayModeParametersKHR&>(*this); }
};
struct DisplayModePropertiesKHR
{
	DisplayModeKHR displayMode {};
	DisplayModeParametersKHR parameters {};

	DisplayModePropertiesKHR(DisplayModeKHR xdisplayMode = {}, DisplayModeParametersKHR xparameters = {}) : displayMode(xdisplayMode), parameters(xparameters) {}
	operator const VkDisplayModePropertiesKHR&() const { return reinterpret_cast<const VkDisplayModePropertiesKHR&>(*this); }
};
struct DisplayModeCreateInfoKHR
{
	StructureType sType {StructureType::displayModeCreateInfoKHR};
	const void* pNext {};
	DisplayModeCreateFlagsKHR flags {};
	DisplayModeParametersKHR parameters {};

	DisplayModeCreateInfoKHR(DisplayModeCreateFlagsKHR xflags = {}, DisplayModeParametersKHR xparameters = {}) : flags(xflags), parameters(xparameters) {}
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

	DisplayPlaneCapabilitiesKHR(DisplayPlaneAlphaFlagsKHR xsupportedAlpha = {}, Offset2D xminSrcPosition = {}, Offset2D xmaxSrcPosition = {}, Extent2D xminSrcExtent = {}, Extent2D xmaxSrcExtent = {}, Offset2D xminDstPosition = {}, Offset2D xmaxDstPosition = {}, Extent2D xminDstExtent = {}, Extent2D xmaxDstExtent = {}) : supportedAlpha(xsupportedAlpha), minSrcPosition(xminSrcPosition), maxSrcPosition(xmaxSrcPosition), minSrcExtent(xminSrcExtent), maxSrcExtent(xmaxSrcExtent), minDstPosition(xminDstPosition), maxDstPosition(xmaxDstPosition), minDstExtent(xminDstExtent), maxDstExtent(xmaxDstExtent) {}
	operator const VkDisplayPlaneCapabilitiesKHR&() const { return reinterpret_cast<const VkDisplayPlaneCapabilitiesKHR&>(*this); }
};
struct DisplayPlanePropertiesKHR
{
	DisplayKHR currentDisplay {};
	uint32_t currentStackIndex {};

	DisplayPlanePropertiesKHR(DisplayKHR xcurrentDisplay = {}, uint32_t xcurrentStackIndex = {}) : currentDisplay(xcurrentDisplay), currentStackIndex(xcurrentStackIndex) {}
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

	DisplaySurfaceCreateInfoKHR(DisplaySurfaceCreateFlagsKHR xflags = {}, DisplayModeKHR xdisplayMode = {}, uint32_t xplaneIndex = {}, uint32_t xplaneStackIndex = {}, SurfaceTransformBitsKHR xtransform = {}, float xglobalAlpha = {}, DisplayPlaneAlphaBitsKHR xalphaMode = {}, Extent2D ximageExtent = {}) : flags(xflags), displayMode(xdisplayMode), planeIndex(xplaneIndex), planeStackIndex(xplaneStackIndex), transform(xtransform), globalAlpha(xglobalAlpha), alphaMode(xalphaMode), imageExtent(ximageExtent) {}
	operator const VkDisplaySurfaceCreateInfoKHR&() const { return reinterpret_cast<const VkDisplaySurfaceCreateInfoKHR&>(*this); }
};

struct DisplayPresentInfoKHR
{
	StructureType sType {StructureType::displayPresentInfoKHR};
	const void* pNext {};
	Rect2D srcRect {};
	Rect2D dstRect {};
	Bool32 persistent {};

	DisplayPresentInfoKHR(Rect2D xsrcRect = {}, Rect2D xdstRect = {}, Bool32 xpersistent = {}) : srcRect(xsrcRect), dstRect(xdstRect), persistent(xpersistent) {}
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

	XlibSurfaceCreateInfoKHR(XlibSurfaceCreateFlagsKHR xflags = {}, Display* xdpy = {}, Window xwindow = {}) : flags(xflags), dpy(xdpy), window(xwindow) {}
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

	XcbSurfaceCreateInfoKHR(XcbSurfaceCreateFlagsKHR xflags = {}, xcb_connection_t* xconnection = {}, xcb_window_t xwindow = {}) : flags(xflags), connection(xconnection), window(xwindow) {}
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

	WaylandSurfaceCreateInfoKHR(WaylandSurfaceCreateFlagsKHR xflags = {}, wl_display* xdisplay = {}, wl_surface* xsurface = {}) : flags(xflags), display(xdisplay), surface(xsurface) {}
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

	MirSurfaceCreateInfoKHR(MirSurfaceCreateFlagsKHR xflags = {}, MirConnection* xconnection = {}, MirSurface* xmirSurface = {}) : flags(xflags), connection(xconnection), mirSurface(xmirSurface) {}
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

	AndroidSurfaceCreateInfoKHR(AndroidSurfaceCreateFlagsKHR xflags = {}, ANativeWindow* xwindow = {}) : flags(xflags), window(xwindow) {}
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

	Win32SurfaceCreateInfoKHR(Win32SurfaceCreateFlagsKHR xflags = {}, HINSTANCE xhinstance = {}, HWND xhwnd = {}) : flags(xflags), hinstance(xhinstance), hwnd(xhwnd) {}
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

	DebugReportCallbackCreateInfoEXT(DebugReportFlagsEXT xflags = {}, PfnDebugReportCallbackEXT xpfnCallback = {}, void* xpUserData = {}) : flags(xflags), pfnCallback(xpfnCallback), pUserData(xpUserData) {}
	operator const VkDebugReportCallbackCreateInfoEXT&() const { return reinterpret_cast<const VkDebugReportCallbackCreateInfoEXT&>(*this); }
};

struct PipelineRasterizationStateRasterizationOrderAMD
{
	StructureType sType {StructureType::pipelineRasterizationStateRasterizationOrderAMD};
	const void* pNext {};
	RasterizationOrderAMD rasterizationOrder {};

	PipelineRasterizationStateRasterizationOrderAMD(RasterizationOrderAMD xrasterizationOrder = {}) : rasterizationOrder(xrasterizationOrder) {}
	operator const VkPipelineRasterizationStateRasterizationOrderAMD&() const { return reinterpret_cast<const VkPipelineRasterizationStateRasterizationOrderAMD&>(*this); }
};

struct DebugMarkerObjectNameInfoEXT
{
	StructureType sType {StructureType::debugMarkerObjectNameInfoEXT};
	const void* pNext {};
	DebugReportObjectTypeEXT objectType {};
	uint64_t object {};
	const char* pObjectName {};

	DebugMarkerObjectNameInfoEXT(DebugReportObjectTypeEXT xobjectType = {}, uint64_t xobject = {}, const char* xpObjectName = {}) : objectType(xobjectType), object(xobject), pObjectName(xpObjectName) {}
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

	DebugMarkerObjectTagInfoEXT(DebugReportObjectTypeEXT xobjectType = {}, uint64_t xobject = {}, uint64_t xtagName = {}, size_t xtagSize = {}, const void* xpTag = {}) : objectType(xobjectType), object(xobject), tagName(xtagName), tagSize(xtagSize), pTag(xpTag) {}
	operator const VkDebugMarkerObjectTagInfoEXT&() const { return reinterpret_cast<const VkDebugMarkerObjectTagInfoEXT&>(*this); }
};
struct DebugMarkerMarkerInfoEXT
{
	StructureType sType {StructureType::debugMarkerMarkerInfoEXT};
	const void* pNext {};
	const char* pMarkerName {};
	float color[4] {};

	DebugMarkerMarkerInfoEXT(const char* xpMarkerName = {}, float xcolor = {}) : pMarkerName(xpMarkerName), color(xcolor) {}
	operator const VkDebugMarkerMarkerInfoEXT&() const { return reinterpret_cast<const VkDebugMarkerMarkerInfoEXT&>(*this); }
};



} //namespace vk