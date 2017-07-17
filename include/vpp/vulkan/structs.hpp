// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

// Automaitcally generated vulkan header file for the nyorain/vpp library.
// Do not edit manually, rather edit the codegen files.

#pragma once

#include "fwd.hpp"
#include "enums.hpp"

#include <array>
#include <vulkan/vulkan.h>

// static_assert(VK_HEADER_VERSION >=  48, "Newer vulkan header required");

namespace vk {

struct ApplicationInfo {
	StructureType sType {StructureType::applicationInfo};
	const void* pNext {};
	const char* pApplicationName {};
	uint32_t applicationVersion {};
	const char* pEngineName {};
	uint32_t engineVersion {};
	uint32_t apiVersion {};

	ApplicationInfo(const char* xpApplicationName = {}, uint32_t xapplicationVersion = {}, const char* xpEngineName = {}, uint32_t xengineVersion = {}, uint32_t xapiVersion = {}) : pApplicationName(xpApplicationName), applicationVersion(xapplicationVersion), pEngineName(xpEngineName), engineVersion(xengineVersion), apiVersion(xapiVersion) {}

	const VkApplicationInfo& vkHandle() const { return reinterpret_cast<const VkApplicationInfo&>(*this); }
	VkApplicationInfo& vkHandle() { return reinterpret_cast<VkApplicationInfo&>(*this); }

	operator const VkApplicationInfo&() const { return vkHandle(); };
	operator VkApplicationInfo&() { return vkHandle(); };
};

struct InstanceCreateInfo {
	StructureType sType {StructureType::instanceCreateInfo};
	const void* pNext {};
	InstanceCreateFlags flags {};
	const ApplicationInfo* pApplicationInfo {};
	uint32_t enabledLayerCount {};
	const char* const* ppEnabledLayerNames {};
	uint32_t enabledExtensionCount {};
	const char* const* ppEnabledExtensionNames {};

	InstanceCreateInfo(InstanceCreateFlags xflags = {}, const ApplicationInfo* xpApplicationInfo = {}, uint32_t xenabledLayerCount = {}, const char* const* xppEnabledLayerNames = {}, uint32_t xenabledExtensionCount = {}, const char* const* xppEnabledExtensionNames = {}) : flags(xflags), pApplicationInfo(xpApplicationInfo), enabledLayerCount(xenabledLayerCount), ppEnabledLayerNames(xppEnabledLayerNames), enabledExtensionCount(xenabledExtensionCount), ppEnabledExtensionNames(xppEnabledExtensionNames) {}

	const VkInstanceCreateInfo& vkHandle() const { return reinterpret_cast<const VkInstanceCreateInfo&>(*this); }
	VkInstanceCreateInfo& vkHandle() { return reinterpret_cast<VkInstanceCreateInfo&>(*this); }

	operator const VkInstanceCreateInfo&() const { return vkHandle(); };
	operator VkInstanceCreateInfo&() { return vkHandle(); };
};

struct AllocationCallbacks {
	void* pUserData {};
	PfnAllocationFunction pfnAllocation {};
	PfnReallocationFunction pfnReallocation {};
	PfnFreeFunction pfnFree {};
	PfnInternalAllocationNotification pfnInternalAllocation {};
	PfnInternalFreeNotification pfnInternalFree {};

	const VkAllocationCallbacks& vkHandle() const { return reinterpret_cast<const VkAllocationCallbacks&>(*this); }
	VkAllocationCallbacks& vkHandle() { return reinterpret_cast<VkAllocationCallbacks&>(*this); }

	operator const VkAllocationCallbacks&() const { return vkHandle(); };
	operator VkAllocationCallbacks&() { return vkHandle(); };
};

struct PhysicalDeviceFeatures {
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

	const VkPhysicalDeviceFeatures& vkHandle() const { return reinterpret_cast<const VkPhysicalDeviceFeatures&>(*this); }
	VkPhysicalDeviceFeatures& vkHandle() { return reinterpret_cast<VkPhysicalDeviceFeatures&>(*this); }

	operator const VkPhysicalDeviceFeatures&() const { return vkHandle(); };
	operator VkPhysicalDeviceFeatures&() { return vkHandle(); };
};

struct FormatProperties {
	FormatFeatureFlags linearTilingFeatures {};
	FormatFeatureFlags optimalTilingFeatures {};
	FormatFeatureFlags bufferFeatures {};

	const VkFormatProperties& vkHandle() const { return reinterpret_cast<const VkFormatProperties&>(*this); }
	VkFormatProperties& vkHandle() { return reinterpret_cast<VkFormatProperties&>(*this); }

	operator const VkFormatProperties&() const { return vkHandle(); };
	operator VkFormatProperties&() { return vkHandle(); };
};

struct Extent3D {
	uint32_t width {};
	uint32_t height {};
	uint32_t depth {};

	const VkExtent3D& vkHandle() const { return reinterpret_cast<const VkExtent3D&>(*this); }
	VkExtent3D& vkHandle() { return reinterpret_cast<VkExtent3D&>(*this); }

	operator const VkExtent3D&() const { return vkHandle(); };
	operator VkExtent3D&() { return vkHandle(); };
};

struct ImageFormatProperties {
	Extent3D maxExtent {};
	uint32_t maxMipLevels {};
	uint32_t maxArrayLayers {};
	SampleCountFlags sampleCounts {};
	DeviceSize maxResourceSize {};

	const VkImageFormatProperties& vkHandle() const { return reinterpret_cast<const VkImageFormatProperties&>(*this); }
	VkImageFormatProperties& vkHandle() { return reinterpret_cast<VkImageFormatProperties&>(*this); }

	operator const VkImageFormatProperties&() const { return vkHandle(); };
	operator VkImageFormatProperties&() { return vkHandle(); };
};

struct PhysicalDeviceLimits {
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
	std::array<uint32_t, 3> maxComputeWorkGroupCount {};
	uint32_t maxComputeWorkGroupInvocations {};
	std::array<uint32_t, 3> maxComputeWorkGroupSize {};
	uint32_t subPixelPrecisionBits {};
	uint32_t subTexelPrecisionBits {};
	uint32_t mipmapPrecisionBits {};
	uint32_t maxDrawIndexedIndexValue {};
	uint32_t maxDrawIndirectCount {};
	float maxSamplerLodBias {};
	float maxSamplerAnisotropy {};
	uint32_t maxViewports {};
	std::array<uint32_t, 2> maxViewportDimensions {};
	std::array<float, 2> viewportBoundsRange {};
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
	std::array<float, 2> pointSizeRange {};
	std::array<float, 2> lineWidthRange {};
	float pointSizeGranularity {};
	float lineWidthGranularity {};
	Bool32 strictLines {};
	Bool32 standardSampleLocations {};
	DeviceSize optimalBufferCopyOffsetAlignment {};
	DeviceSize optimalBufferCopyRowPitchAlignment {};
	DeviceSize nonCoherentAtomSize {};

	const VkPhysicalDeviceLimits& vkHandle() const { return reinterpret_cast<const VkPhysicalDeviceLimits&>(*this); }
	VkPhysicalDeviceLimits& vkHandle() { return reinterpret_cast<VkPhysicalDeviceLimits&>(*this); }

	operator const VkPhysicalDeviceLimits&() const { return vkHandle(); };
	operator VkPhysicalDeviceLimits&() { return vkHandle(); };
};

struct PhysicalDeviceSparseProperties {
	Bool32 residencyStandard2DBlockShape {};
	Bool32 residencyStandard2DMultisampleBlockShape {};
	Bool32 residencyStandard3DBlockShape {};
	Bool32 residencyAlignedMipSize {};
	Bool32 residencyNonResidentStrict {};

	const VkPhysicalDeviceSparseProperties& vkHandle() const { return reinterpret_cast<const VkPhysicalDeviceSparseProperties&>(*this); }
	VkPhysicalDeviceSparseProperties& vkHandle() { return reinterpret_cast<VkPhysicalDeviceSparseProperties&>(*this); }

	operator const VkPhysicalDeviceSparseProperties&() const { return vkHandle(); };
	operator VkPhysicalDeviceSparseProperties&() { return vkHandle(); };
};

struct PhysicalDeviceProperties {
	uint32_t apiVersion {};
	uint32_t driverVersion {};
	uint32_t vendorID {};
	uint32_t deviceID {};
	PhysicalDeviceType deviceType {};
	std::array<char, maxPhysicalDeviceNameSize> deviceName {};
	std::array<uint8_t, uuidSize> pipelineCacheUUID {};
	PhysicalDeviceLimits limits {};
	PhysicalDeviceSparseProperties sparseProperties {};

	const VkPhysicalDeviceProperties& vkHandle() const { return reinterpret_cast<const VkPhysicalDeviceProperties&>(*this); }
	VkPhysicalDeviceProperties& vkHandle() { return reinterpret_cast<VkPhysicalDeviceProperties&>(*this); }

	operator const VkPhysicalDeviceProperties&() const { return vkHandle(); };
	operator VkPhysicalDeviceProperties&() { return vkHandle(); };
};

struct QueueFamilyProperties {
	QueueFlags queueFlags {};
	uint32_t queueCount {};
	uint32_t timestampValidBits {};
	Extent3D minImageTransferGranularity {};

	const VkQueueFamilyProperties& vkHandle() const { return reinterpret_cast<const VkQueueFamilyProperties&>(*this); }
	VkQueueFamilyProperties& vkHandle() { return reinterpret_cast<VkQueueFamilyProperties&>(*this); }

	operator const VkQueueFamilyProperties&() const { return vkHandle(); };
	operator VkQueueFamilyProperties&() { return vkHandle(); };
};

struct MemoryType {
	MemoryPropertyFlags propertyFlags {};
	uint32_t heapIndex {};

	const VkMemoryType& vkHandle() const { return reinterpret_cast<const VkMemoryType&>(*this); }
	VkMemoryType& vkHandle() { return reinterpret_cast<VkMemoryType&>(*this); }

	operator const VkMemoryType&() const { return vkHandle(); };
	operator VkMemoryType&() { return vkHandle(); };
};

struct MemoryHeap {
	DeviceSize size {};
	MemoryHeapFlags flags {};

	const VkMemoryHeap& vkHandle() const { return reinterpret_cast<const VkMemoryHeap&>(*this); }
	VkMemoryHeap& vkHandle() { return reinterpret_cast<VkMemoryHeap&>(*this); }

	operator const VkMemoryHeap&() const { return vkHandle(); };
	operator VkMemoryHeap&() { return vkHandle(); };
};

struct PhysicalDeviceMemoryProperties {
	uint32_t memoryTypeCount {};
	std::array<MemoryType, maxMemoryTypes> memoryTypes {};
	uint32_t memoryHeapCount {};
	std::array<MemoryHeap, maxMemoryHeaps> memoryHeaps {};

	const VkPhysicalDeviceMemoryProperties& vkHandle() const { return reinterpret_cast<const VkPhysicalDeviceMemoryProperties&>(*this); }
	VkPhysicalDeviceMemoryProperties& vkHandle() { return reinterpret_cast<VkPhysicalDeviceMemoryProperties&>(*this); }

	operator const VkPhysicalDeviceMemoryProperties&() const { return vkHandle(); };
	operator VkPhysicalDeviceMemoryProperties&() { return vkHandle(); };
};

struct DeviceQueueCreateInfo {
	StructureType sType {StructureType::deviceQueueCreateInfo};
	const void* pNext {};
	DeviceQueueCreateFlags flags {};
	uint32_t queueFamilyIndex {};
	uint32_t queueCount {};
	const float* pQueuePriorities {};

	DeviceQueueCreateInfo(DeviceQueueCreateFlags xflags = {}, uint32_t xqueueFamilyIndex = {}, uint32_t xqueueCount = {}, const float* xpQueuePriorities = {}) : flags(xflags), queueFamilyIndex(xqueueFamilyIndex), queueCount(xqueueCount), pQueuePriorities(xpQueuePriorities) {}

	const VkDeviceQueueCreateInfo& vkHandle() const { return reinterpret_cast<const VkDeviceQueueCreateInfo&>(*this); }
	VkDeviceQueueCreateInfo& vkHandle() { return reinterpret_cast<VkDeviceQueueCreateInfo&>(*this); }

	operator const VkDeviceQueueCreateInfo&() const { return vkHandle(); };
	operator VkDeviceQueueCreateInfo&() { return vkHandle(); };
};

struct DeviceCreateInfo {
	StructureType sType {StructureType::deviceCreateInfo};
	const void* pNext {};
	DeviceCreateFlags flags {};
	uint32_t queueCreateInfoCount {};
	const DeviceQueueCreateInfo* pQueueCreateInfos {};
	uint32_t enabledLayerCount {};
	const char* const* ppEnabledLayerNames {};
	uint32_t enabledExtensionCount {};
	const char* const* ppEnabledExtensionNames {};
	const PhysicalDeviceFeatures* pEnabledFeatures {};

	DeviceCreateInfo(DeviceCreateFlags xflags = {}, uint32_t xqueueCreateInfoCount = {}, const DeviceQueueCreateInfo* xpQueueCreateInfos = {}, uint32_t xenabledLayerCount = {}, const char* const* xppEnabledLayerNames = {}, uint32_t xenabledExtensionCount = {}, const char* const* xppEnabledExtensionNames = {}, const PhysicalDeviceFeatures* xpEnabledFeatures = {}) : flags(xflags), queueCreateInfoCount(xqueueCreateInfoCount), pQueueCreateInfos(xpQueueCreateInfos), enabledLayerCount(xenabledLayerCount), ppEnabledLayerNames(xppEnabledLayerNames), enabledExtensionCount(xenabledExtensionCount), ppEnabledExtensionNames(xppEnabledExtensionNames), pEnabledFeatures(xpEnabledFeatures) {}

	const VkDeviceCreateInfo& vkHandle() const { return reinterpret_cast<const VkDeviceCreateInfo&>(*this); }
	VkDeviceCreateInfo& vkHandle() { return reinterpret_cast<VkDeviceCreateInfo&>(*this); }

	operator const VkDeviceCreateInfo&() const { return vkHandle(); };
	operator VkDeviceCreateInfo&() { return vkHandle(); };
};

struct ExtensionProperties {
	std::array<char, maxExtensionNameSize> extensionName {};
	uint32_t specVersion {};

	const VkExtensionProperties& vkHandle() const { return reinterpret_cast<const VkExtensionProperties&>(*this); }
	VkExtensionProperties& vkHandle() { return reinterpret_cast<VkExtensionProperties&>(*this); }

	operator const VkExtensionProperties&() const { return vkHandle(); };
	operator VkExtensionProperties&() { return vkHandle(); };
};

struct LayerProperties {
	std::array<char, maxExtensionNameSize> layerName {};
	uint32_t specVersion {};
	uint32_t implementationVersion {};
	std::array<char, maxDescriptionSize> description {};

	const VkLayerProperties& vkHandle() const { return reinterpret_cast<const VkLayerProperties&>(*this); }
	VkLayerProperties& vkHandle() { return reinterpret_cast<VkLayerProperties&>(*this); }

	operator const VkLayerProperties&() const { return vkHandle(); };
	operator VkLayerProperties&() { return vkHandle(); };
};

struct SubmitInfo {
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

	const VkSubmitInfo& vkHandle() const { return reinterpret_cast<const VkSubmitInfo&>(*this); }
	VkSubmitInfo& vkHandle() { return reinterpret_cast<VkSubmitInfo&>(*this); }

	operator const VkSubmitInfo&() const { return vkHandle(); };
	operator VkSubmitInfo&() { return vkHandle(); };
};

struct MemoryAllocateInfo {
	StructureType sType {StructureType::memoryAllocateInfo};
	const void* pNext {};
	DeviceSize allocationSize {};
	uint32_t memoryTypeIndex {};

	MemoryAllocateInfo(DeviceSize xallocationSize = {}, uint32_t xmemoryTypeIndex = {}) : allocationSize(xallocationSize), memoryTypeIndex(xmemoryTypeIndex) {}

	const VkMemoryAllocateInfo& vkHandle() const { return reinterpret_cast<const VkMemoryAllocateInfo&>(*this); }
	VkMemoryAllocateInfo& vkHandle() { return reinterpret_cast<VkMemoryAllocateInfo&>(*this); }

	operator const VkMemoryAllocateInfo&() const { return vkHandle(); };
	operator VkMemoryAllocateInfo&() { return vkHandle(); };
};

struct MappedMemoryRange {
	StructureType sType {StructureType::mappedMemoryRange};
	const void* pNext {};
	DeviceMemory memory {};
	DeviceSize offset {};
	DeviceSize size {};

	MappedMemoryRange(DeviceMemory xmemory = {}, DeviceSize xoffset = {}, DeviceSize xsize = {}) : memory(xmemory), offset(xoffset), size(xsize) {}

	const VkMappedMemoryRange& vkHandle() const { return reinterpret_cast<const VkMappedMemoryRange&>(*this); }
	VkMappedMemoryRange& vkHandle() { return reinterpret_cast<VkMappedMemoryRange&>(*this); }

	operator const VkMappedMemoryRange&() const { return vkHandle(); };
	operator VkMappedMemoryRange&() { return vkHandle(); };
};

struct MemoryRequirements {
	DeviceSize size {};
	DeviceSize alignment {};
	uint32_t memoryTypeBits {};

	const VkMemoryRequirements& vkHandle() const { return reinterpret_cast<const VkMemoryRequirements&>(*this); }
	VkMemoryRequirements& vkHandle() { return reinterpret_cast<VkMemoryRequirements&>(*this); }

	operator const VkMemoryRequirements&() const { return vkHandle(); };
	operator VkMemoryRequirements&() { return vkHandle(); };
};

struct SparseImageFormatProperties {
	ImageAspectFlags aspectMask {};
	Extent3D imageGranularity {};
	SparseImageFormatFlags flags {};

	const VkSparseImageFormatProperties& vkHandle() const { return reinterpret_cast<const VkSparseImageFormatProperties&>(*this); }
	VkSparseImageFormatProperties& vkHandle() { return reinterpret_cast<VkSparseImageFormatProperties&>(*this); }

	operator const VkSparseImageFormatProperties&() const { return vkHandle(); };
	operator VkSparseImageFormatProperties&() { return vkHandle(); };
};

struct SparseImageMemoryRequirements {
	SparseImageFormatProperties formatProperties {};
	uint32_t imageMipTailFirstLod {};
	DeviceSize imageMipTailSize {};
	DeviceSize imageMipTailOffset {};
	DeviceSize imageMipTailStride {};

	const VkSparseImageMemoryRequirements& vkHandle() const { return reinterpret_cast<const VkSparseImageMemoryRequirements&>(*this); }
	VkSparseImageMemoryRequirements& vkHandle() { return reinterpret_cast<VkSparseImageMemoryRequirements&>(*this); }

	operator const VkSparseImageMemoryRequirements&() const { return vkHandle(); };
	operator VkSparseImageMemoryRequirements&() { return vkHandle(); };
};

struct SparseMemoryBind {
	DeviceSize resourceOffset {};
	DeviceSize size {};
	DeviceMemory memory {};
	DeviceSize memoryOffset {};
	SparseMemoryBindFlags flags {};

	const VkSparseMemoryBind& vkHandle() const { return reinterpret_cast<const VkSparseMemoryBind&>(*this); }
	VkSparseMemoryBind& vkHandle() { return reinterpret_cast<VkSparseMemoryBind&>(*this); }

	operator const VkSparseMemoryBind&() const { return vkHandle(); };
	operator VkSparseMemoryBind&() { return vkHandle(); };
};

struct SparseBufferMemoryBindInfo {
	Buffer buffer {};
	uint32_t bindCount {};
	const SparseMemoryBind* pBinds {};

	const VkSparseBufferMemoryBindInfo& vkHandle() const { return reinterpret_cast<const VkSparseBufferMemoryBindInfo&>(*this); }
	VkSparseBufferMemoryBindInfo& vkHandle() { return reinterpret_cast<VkSparseBufferMemoryBindInfo&>(*this); }

	operator const VkSparseBufferMemoryBindInfo&() const { return vkHandle(); };
	operator VkSparseBufferMemoryBindInfo&() { return vkHandle(); };
};

struct SparseImageOpaqueMemoryBindInfo {
	Image image {};
	uint32_t bindCount {};
	const SparseMemoryBind* pBinds {};

	const VkSparseImageOpaqueMemoryBindInfo& vkHandle() const { return reinterpret_cast<const VkSparseImageOpaqueMemoryBindInfo&>(*this); }
	VkSparseImageOpaqueMemoryBindInfo& vkHandle() { return reinterpret_cast<VkSparseImageOpaqueMemoryBindInfo&>(*this); }

	operator const VkSparseImageOpaqueMemoryBindInfo&() const { return vkHandle(); };
	operator VkSparseImageOpaqueMemoryBindInfo&() { return vkHandle(); };
};

struct ImageSubresource {
	ImageAspectFlags aspectMask {};
	uint32_t mipLevel {};
	uint32_t arrayLayer {};

	const VkImageSubresource& vkHandle() const { return reinterpret_cast<const VkImageSubresource&>(*this); }
	VkImageSubresource& vkHandle() { return reinterpret_cast<VkImageSubresource&>(*this); }

	operator const VkImageSubresource&() const { return vkHandle(); };
	operator VkImageSubresource&() { return vkHandle(); };
};

struct Offset3D {
	int32_t x {};
	int32_t y {};
	int32_t z {};

	const VkOffset3D& vkHandle() const { return reinterpret_cast<const VkOffset3D&>(*this); }
	VkOffset3D& vkHandle() { return reinterpret_cast<VkOffset3D&>(*this); }

	operator const VkOffset3D&() const { return vkHandle(); };
	operator VkOffset3D&() { return vkHandle(); };
};

struct SparseImageMemoryBind {
	ImageSubresource subresource {};
	Offset3D offset {};
	Extent3D extent {};
	DeviceMemory memory {};
	DeviceSize memoryOffset {};
	SparseMemoryBindFlags flags {};

	const VkSparseImageMemoryBind& vkHandle() const { return reinterpret_cast<const VkSparseImageMemoryBind&>(*this); }
	VkSparseImageMemoryBind& vkHandle() { return reinterpret_cast<VkSparseImageMemoryBind&>(*this); }

	operator const VkSparseImageMemoryBind&() const { return vkHandle(); };
	operator VkSparseImageMemoryBind&() { return vkHandle(); };
};

struct SparseImageMemoryBindInfo {
	Image image {};
	uint32_t bindCount {};
	const SparseImageMemoryBind* pBinds {};

	const VkSparseImageMemoryBindInfo& vkHandle() const { return reinterpret_cast<const VkSparseImageMemoryBindInfo&>(*this); }
	VkSparseImageMemoryBindInfo& vkHandle() { return reinterpret_cast<VkSparseImageMemoryBindInfo&>(*this); }

	operator const VkSparseImageMemoryBindInfo&() const { return vkHandle(); };
	operator VkSparseImageMemoryBindInfo&() { return vkHandle(); };
};

struct BindSparseInfo {
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

	const VkBindSparseInfo& vkHandle() const { return reinterpret_cast<const VkBindSparseInfo&>(*this); }
	VkBindSparseInfo& vkHandle() { return reinterpret_cast<VkBindSparseInfo&>(*this); }

	operator const VkBindSparseInfo&() const { return vkHandle(); };
	operator VkBindSparseInfo&() { return vkHandle(); };
};

struct FenceCreateInfo {
	StructureType sType {StructureType::fenceCreateInfo};
	const void* pNext {};
	FenceCreateFlags flags {};

	FenceCreateInfo(FenceCreateFlags xflags = {}) : flags(xflags) {}

	const VkFenceCreateInfo& vkHandle() const { return reinterpret_cast<const VkFenceCreateInfo&>(*this); }
	VkFenceCreateInfo& vkHandle() { return reinterpret_cast<VkFenceCreateInfo&>(*this); }

	operator const VkFenceCreateInfo&() const { return vkHandle(); };
	operator VkFenceCreateInfo&() { return vkHandle(); };
};

struct SemaphoreCreateInfo {
	StructureType sType {StructureType::semaphoreCreateInfo};
	const void* pNext {};
	SemaphoreCreateFlags flags {};

	SemaphoreCreateInfo(SemaphoreCreateFlags xflags = {}) : flags(xflags) {}

	const VkSemaphoreCreateInfo& vkHandle() const { return reinterpret_cast<const VkSemaphoreCreateInfo&>(*this); }
	VkSemaphoreCreateInfo& vkHandle() { return reinterpret_cast<VkSemaphoreCreateInfo&>(*this); }

	operator const VkSemaphoreCreateInfo&() const { return vkHandle(); };
	operator VkSemaphoreCreateInfo&() { return vkHandle(); };
};

struct EventCreateInfo {
	StructureType sType {StructureType::eventCreateInfo};
	const void* pNext {};
	EventCreateFlags flags {};

	EventCreateInfo(EventCreateFlags xflags = {}) : flags(xflags) {}

	const VkEventCreateInfo& vkHandle() const { return reinterpret_cast<const VkEventCreateInfo&>(*this); }
	VkEventCreateInfo& vkHandle() { return reinterpret_cast<VkEventCreateInfo&>(*this); }

	operator const VkEventCreateInfo&() const { return vkHandle(); };
	operator VkEventCreateInfo&() { return vkHandle(); };
};

struct QueryPoolCreateInfo {
	StructureType sType {StructureType::queryPoolCreateInfo};
	const void* pNext {};
	QueryPoolCreateFlags flags {};
	QueryType queryType {};
	uint32_t queryCount {};
	QueryPipelineStatisticFlags pipelineStatistics {};

	QueryPoolCreateInfo(QueryPoolCreateFlags xflags = {}, QueryType xqueryType = {}, uint32_t xqueryCount = {}, QueryPipelineStatisticFlags xpipelineStatistics = {}) : flags(xflags), queryType(xqueryType), queryCount(xqueryCount), pipelineStatistics(xpipelineStatistics) {}

	const VkQueryPoolCreateInfo& vkHandle() const { return reinterpret_cast<const VkQueryPoolCreateInfo&>(*this); }
	VkQueryPoolCreateInfo& vkHandle() { return reinterpret_cast<VkQueryPoolCreateInfo&>(*this); }

	operator const VkQueryPoolCreateInfo&() const { return vkHandle(); };
	operator VkQueryPoolCreateInfo&() { return vkHandle(); };
};

struct BufferCreateInfo {
	StructureType sType {StructureType::bufferCreateInfo};
	const void* pNext {};
	BufferCreateFlags flags {};
	DeviceSize size {};
	BufferUsageFlags usage {};
	SharingMode sharingMode {};
	uint32_t queueFamilyIndexCount {};
	const uint32_t* pQueueFamilyIndices {};

	BufferCreateInfo(BufferCreateFlags xflags = {}, DeviceSize xsize = {}, BufferUsageFlags xusage = {}, SharingMode xsharingMode = {}, uint32_t xqueueFamilyIndexCount = {}, const uint32_t* xpQueueFamilyIndices = {}) : flags(xflags), size(xsize), usage(xusage), sharingMode(xsharingMode), queueFamilyIndexCount(xqueueFamilyIndexCount), pQueueFamilyIndices(xpQueueFamilyIndices) {}

	const VkBufferCreateInfo& vkHandle() const { return reinterpret_cast<const VkBufferCreateInfo&>(*this); }
	VkBufferCreateInfo& vkHandle() { return reinterpret_cast<VkBufferCreateInfo&>(*this); }

	operator const VkBufferCreateInfo&() const { return vkHandle(); };
	operator VkBufferCreateInfo&() { return vkHandle(); };
};

struct BufferViewCreateInfo {
	StructureType sType {StructureType::bufferViewCreateInfo};
	const void* pNext {};
	BufferViewCreateFlags flags {};
	Buffer buffer {};
	Format format {};
	DeviceSize offset {};
	DeviceSize range {};

	BufferViewCreateInfo(BufferViewCreateFlags xflags = {}, Buffer xbuffer = {}, Format xformat = {}, DeviceSize xoffset = {}, DeviceSize xrange = {}) : flags(xflags), buffer(xbuffer), format(xformat), offset(xoffset), range(xrange) {}

	const VkBufferViewCreateInfo& vkHandle() const { return reinterpret_cast<const VkBufferViewCreateInfo&>(*this); }
	VkBufferViewCreateInfo& vkHandle() { return reinterpret_cast<VkBufferViewCreateInfo&>(*this); }

	operator const VkBufferViewCreateInfo&() const { return vkHandle(); };
	operator VkBufferViewCreateInfo&() { return vkHandle(); };
};

struct ImageCreateInfo {
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

	const VkImageCreateInfo& vkHandle() const { return reinterpret_cast<const VkImageCreateInfo&>(*this); }
	VkImageCreateInfo& vkHandle() { return reinterpret_cast<VkImageCreateInfo&>(*this); }

	operator const VkImageCreateInfo&() const { return vkHandle(); };
	operator VkImageCreateInfo&() { return vkHandle(); };
};

struct SubresourceLayout {
	DeviceSize offset {};
	DeviceSize size {};
	DeviceSize rowPitch {};
	DeviceSize arrayPitch {};
	DeviceSize depthPitch {};

	const VkSubresourceLayout& vkHandle() const { return reinterpret_cast<const VkSubresourceLayout&>(*this); }
	VkSubresourceLayout& vkHandle() { return reinterpret_cast<VkSubresourceLayout&>(*this); }

	operator const VkSubresourceLayout&() const { return vkHandle(); };
	operator VkSubresourceLayout&() { return vkHandle(); };
};

struct ComponentMapping {
	ComponentSwizzle r {};
	ComponentSwizzle g {};
	ComponentSwizzle b {};
	ComponentSwizzle a {};

	const VkComponentMapping& vkHandle() const { return reinterpret_cast<const VkComponentMapping&>(*this); }
	VkComponentMapping& vkHandle() { return reinterpret_cast<VkComponentMapping&>(*this); }

	operator const VkComponentMapping&() const { return vkHandle(); };
	operator VkComponentMapping&() { return vkHandle(); };
};

struct ImageSubresourceRange {
	ImageAspectFlags aspectMask {};
	uint32_t baseMipLevel {};
	uint32_t levelCount {};
	uint32_t baseArrayLayer {};
	uint32_t layerCount {};

	const VkImageSubresourceRange& vkHandle() const { return reinterpret_cast<const VkImageSubresourceRange&>(*this); }
	VkImageSubresourceRange& vkHandle() { return reinterpret_cast<VkImageSubresourceRange&>(*this); }

	operator const VkImageSubresourceRange&() const { return vkHandle(); };
	operator VkImageSubresourceRange&() { return vkHandle(); };
};

struct ImageViewCreateInfo {
	StructureType sType {StructureType::imageViewCreateInfo};
	const void* pNext {};
	ImageViewCreateFlags flags {};
	Image image {};
	ImageViewType viewType {};
	Format format {};
	ComponentMapping components {};
	ImageSubresourceRange subresourceRange {};

	ImageViewCreateInfo(ImageViewCreateFlags xflags = {}, Image ximage = {}, ImageViewType xviewType = {}, Format xformat = {}, ComponentMapping xcomponents = {}, ImageSubresourceRange xsubresourceRange = {}) : flags(xflags), image(ximage), viewType(xviewType), format(xformat), components(xcomponents), subresourceRange(xsubresourceRange) {}

	const VkImageViewCreateInfo& vkHandle() const { return reinterpret_cast<const VkImageViewCreateInfo&>(*this); }
	VkImageViewCreateInfo& vkHandle() { return reinterpret_cast<VkImageViewCreateInfo&>(*this); }

	operator const VkImageViewCreateInfo&() const { return vkHandle(); };
	operator VkImageViewCreateInfo&() { return vkHandle(); };
};

struct ShaderModuleCreateInfo {
	StructureType sType {StructureType::shaderModuleCreateInfo};
	const void* pNext {};
	ShaderModuleCreateFlags flags {};
	size_t codeSize {};
	const uint32_t* pCode {};

	ShaderModuleCreateInfo(ShaderModuleCreateFlags xflags = {}, size_t xcodeSize = {}, const uint32_t* xpCode = {}) : flags(xflags), codeSize(xcodeSize), pCode(xpCode) {}

	const VkShaderModuleCreateInfo& vkHandle() const { return reinterpret_cast<const VkShaderModuleCreateInfo&>(*this); }
	VkShaderModuleCreateInfo& vkHandle() { return reinterpret_cast<VkShaderModuleCreateInfo&>(*this); }

	operator const VkShaderModuleCreateInfo&() const { return vkHandle(); };
	operator VkShaderModuleCreateInfo&() { return vkHandle(); };
};

struct PipelineCacheCreateInfo {
	StructureType sType {StructureType::pipelineCacheCreateInfo};
	const void* pNext {};
	PipelineCacheCreateFlags flags {};
	size_t initialDataSize {};
	const void* pInitialData {};

	PipelineCacheCreateInfo(PipelineCacheCreateFlags xflags = {}, size_t xinitialDataSize = {}, const void* xpInitialData = {}) : flags(xflags), initialDataSize(xinitialDataSize), pInitialData(xpInitialData) {}

	const VkPipelineCacheCreateInfo& vkHandle() const { return reinterpret_cast<const VkPipelineCacheCreateInfo&>(*this); }
	VkPipelineCacheCreateInfo& vkHandle() { return reinterpret_cast<VkPipelineCacheCreateInfo&>(*this); }

	operator const VkPipelineCacheCreateInfo&() const { return vkHandle(); };
	operator VkPipelineCacheCreateInfo&() { return vkHandle(); };
};

struct SpecializationMapEntry {
	uint32_t constantID {};
	uint32_t offset {};
	size_t size {};

	const VkSpecializationMapEntry& vkHandle() const { return reinterpret_cast<const VkSpecializationMapEntry&>(*this); }
	VkSpecializationMapEntry& vkHandle() { return reinterpret_cast<VkSpecializationMapEntry&>(*this); }

	operator const VkSpecializationMapEntry&() const { return vkHandle(); };
	operator VkSpecializationMapEntry&() { return vkHandle(); };
};

struct SpecializationInfo {
	uint32_t mapEntryCount {};
	const SpecializationMapEntry* pMapEntries {};
	size_t dataSize {};
	const void* pData {};

	const VkSpecializationInfo& vkHandle() const { return reinterpret_cast<const VkSpecializationInfo&>(*this); }
	VkSpecializationInfo& vkHandle() { return reinterpret_cast<VkSpecializationInfo&>(*this); }

	operator const VkSpecializationInfo&() const { return vkHandle(); };
	operator VkSpecializationInfo&() { return vkHandle(); };
};

struct PipelineShaderStageCreateInfo {
	StructureType sType {StructureType::pipelineShaderStageCreateInfo};
	const void* pNext {};
	PipelineShaderStageCreateFlags flags {};
	ShaderStageBits stage {};
	ShaderModule module {};
	const char* pName {};
	const SpecializationInfo* pSpecializationInfo {};

	PipelineShaderStageCreateInfo(PipelineShaderStageCreateFlags xflags = {}, ShaderStageBits xstage = {}, ShaderModule xmodule = {}, const char* xpName = {}, const SpecializationInfo* xpSpecializationInfo = {}) : flags(xflags), stage(xstage), module(xmodule), pName(xpName), pSpecializationInfo(xpSpecializationInfo) {}

	const VkPipelineShaderStageCreateInfo& vkHandle() const { return reinterpret_cast<const VkPipelineShaderStageCreateInfo&>(*this); }
	VkPipelineShaderStageCreateInfo& vkHandle() { return reinterpret_cast<VkPipelineShaderStageCreateInfo&>(*this); }

	operator const VkPipelineShaderStageCreateInfo&() const { return vkHandle(); };
	operator VkPipelineShaderStageCreateInfo&() { return vkHandle(); };
};

struct VertexInputBindingDescription {
	uint32_t binding {};
	uint32_t stride {};
	VertexInputRate inputRate {};

	const VkVertexInputBindingDescription& vkHandle() const { return reinterpret_cast<const VkVertexInputBindingDescription&>(*this); }
	VkVertexInputBindingDescription& vkHandle() { return reinterpret_cast<VkVertexInputBindingDescription&>(*this); }

	operator const VkVertexInputBindingDescription&() const { return vkHandle(); };
	operator VkVertexInputBindingDescription&() { return vkHandle(); };
};

struct VertexInputAttributeDescription {
	uint32_t location {};
	uint32_t binding {};
	Format format {};
	uint32_t offset {};

	const VkVertexInputAttributeDescription& vkHandle() const { return reinterpret_cast<const VkVertexInputAttributeDescription&>(*this); }
	VkVertexInputAttributeDescription& vkHandle() { return reinterpret_cast<VkVertexInputAttributeDescription&>(*this); }

	operator const VkVertexInputAttributeDescription&() const { return vkHandle(); };
	operator VkVertexInputAttributeDescription&() { return vkHandle(); };
};

struct PipelineVertexInputStateCreateInfo {
	StructureType sType {StructureType::pipelineVertexInputStateCreateInfo};
	const void* pNext {};
	PipelineVertexInputStateCreateFlags flags {};
	uint32_t vertexBindingDescriptionCount {};
	const VertexInputBindingDescription* pVertexBindingDescriptions {};
	uint32_t vertexAttributeDescriptionCount {};
	const VertexInputAttributeDescription* pVertexAttributeDescriptions {};

	PipelineVertexInputStateCreateInfo(PipelineVertexInputStateCreateFlags xflags = {}, uint32_t xvertexBindingDescriptionCount = {}, const VertexInputBindingDescription* xpVertexBindingDescriptions = {}, uint32_t xvertexAttributeDescriptionCount = {}, const VertexInputAttributeDescription* xpVertexAttributeDescriptions = {}) : flags(xflags), vertexBindingDescriptionCount(xvertexBindingDescriptionCount), pVertexBindingDescriptions(xpVertexBindingDescriptions), vertexAttributeDescriptionCount(xvertexAttributeDescriptionCount), pVertexAttributeDescriptions(xpVertexAttributeDescriptions) {}

	const VkPipelineVertexInputStateCreateInfo& vkHandle() const { return reinterpret_cast<const VkPipelineVertexInputStateCreateInfo&>(*this); }
	VkPipelineVertexInputStateCreateInfo& vkHandle() { return reinterpret_cast<VkPipelineVertexInputStateCreateInfo&>(*this); }

	operator const VkPipelineVertexInputStateCreateInfo&() const { return vkHandle(); };
	operator VkPipelineVertexInputStateCreateInfo&() { return vkHandle(); };
};

struct PipelineInputAssemblyStateCreateInfo {
	StructureType sType {StructureType::pipelineInputAssemblyStateCreateInfo};
	const void* pNext {};
	PipelineInputAssemblyStateCreateFlags flags {};
	PrimitiveTopology topology {};
	Bool32 primitiveRestartEnable {};

	PipelineInputAssemblyStateCreateInfo(PipelineInputAssemblyStateCreateFlags xflags = {}, PrimitiveTopology xtopology = {}, Bool32 xprimitiveRestartEnable = {}) : flags(xflags), topology(xtopology), primitiveRestartEnable(xprimitiveRestartEnable) {}

	const VkPipelineInputAssemblyStateCreateInfo& vkHandle() const { return reinterpret_cast<const VkPipelineInputAssemblyStateCreateInfo&>(*this); }
	VkPipelineInputAssemblyStateCreateInfo& vkHandle() { return reinterpret_cast<VkPipelineInputAssemblyStateCreateInfo&>(*this); }

	operator const VkPipelineInputAssemblyStateCreateInfo&() const { return vkHandle(); };
	operator VkPipelineInputAssemblyStateCreateInfo&() { return vkHandle(); };
};

struct PipelineTessellationStateCreateInfo {
	StructureType sType {StructureType::pipelineTessellationStateCreateInfo};
	const void* pNext {};
	PipelineTessellationStateCreateFlags flags {};
	uint32_t patchControlPoints {};

	PipelineTessellationStateCreateInfo(PipelineTessellationStateCreateFlags xflags = {}, uint32_t xpatchControlPoints = {}) : flags(xflags), patchControlPoints(xpatchControlPoints) {}

	const VkPipelineTessellationStateCreateInfo& vkHandle() const { return reinterpret_cast<const VkPipelineTessellationStateCreateInfo&>(*this); }
	VkPipelineTessellationStateCreateInfo& vkHandle() { return reinterpret_cast<VkPipelineTessellationStateCreateInfo&>(*this); }

	operator const VkPipelineTessellationStateCreateInfo&() const { return vkHandle(); };
	operator VkPipelineTessellationStateCreateInfo&() { return vkHandle(); };
};

struct Viewport {
	float x {};
	float y {};
	float width {};
	float height {};
	float minDepth {};
	float maxDepth {};

	const VkViewport& vkHandle() const { return reinterpret_cast<const VkViewport&>(*this); }
	VkViewport& vkHandle() { return reinterpret_cast<VkViewport&>(*this); }

	operator const VkViewport&() const { return vkHandle(); };
	operator VkViewport&() { return vkHandle(); };
};

struct Offset2D {
	int32_t x {};
	int32_t y {};

	const VkOffset2D& vkHandle() const { return reinterpret_cast<const VkOffset2D&>(*this); }
	VkOffset2D& vkHandle() { return reinterpret_cast<VkOffset2D&>(*this); }

	operator const VkOffset2D&() const { return vkHandle(); };
	operator VkOffset2D&() { return vkHandle(); };
};

struct Extent2D {
	uint32_t width {};
	uint32_t height {};

	const VkExtent2D& vkHandle() const { return reinterpret_cast<const VkExtent2D&>(*this); }
	VkExtent2D& vkHandle() { return reinterpret_cast<VkExtent2D&>(*this); }

	operator const VkExtent2D&() const { return vkHandle(); };
	operator VkExtent2D&() { return vkHandle(); };
};

struct Rect2D {
	Offset2D offset {};
	Extent2D extent {};

	const VkRect2D& vkHandle() const { return reinterpret_cast<const VkRect2D&>(*this); }
	VkRect2D& vkHandle() { return reinterpret_cast<VkRect2D&>(*this); }

	operator const VkRect2D&() const { return vkHandle(); };
	operator VkRect2D&() { return vkHandle(); };
};

struct PipelineViewportStateCreateInfo {
	StructureType sType {StructureType::pipelineViewportStateCreateInfo};
	const void* pNext {};
	PipelineViewportStateCreateFlags flags {};
	uint32_t viewportCount {};
	const Viewport* pViewports {};
	uint32_t scissorCount {};
	const Rect2D* pScissors {};

	PipelineViewportStateCreateInfo(PipelineViewportStateCreateFlags xflags = {}, uint32_t xviewportCount = {}, const Viewport* xpViewports = {}, uint32_t xscissorCount = {}, const Rect2D* xpScissors = {}) : flags(xflags), viewportCount(xviewportCount), pViewports(xpViewports), scissorCount(xscissorCount), pScissors(xpScissors) {}

	const VkPipelineViewportStateCreateInfo& vkHandle() const { return reinterpret_cast<const VkPipelineViewportStateCreateInfo&>(*this); }
	VkPipelineViewportStateCreateInfo& vkHandle() { return reinterpret_cast<VkPipelineViewportStateCreateInfo&>(*this); }

	operator const VkPipelineViewportStateCreateInfo&() const { return vkHandle(); };
	operator VkPipelineViewportStateCreateInfo&() { return vkHandle(); };
};

struct PipelineRasterizationStateCreateInfo {
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

	const VkPipelineRasterizationStateCreateInfo& vkHandle() const { return reinterpret_cast<const VkPipelineRasterizationStateCreateInfo&>(*this); }
	VkPipelineRasterizationStateCreateInfo& vkHandle() { return reinterpret_cast<VkPipelineRasterizationStateCreateInfo&>(*this); }

	operator const VkPipelineRasterizationStateCreateInfo&() const { return vkHandle(); };
	operator VkPipelineRasterizationStateCreateInfo&() { return vkHandle(); };
};

struct PipelineMultisampleStateCreateInfo {
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

	const VkPipelineMultisampleStateCreateInfo& vkHandle() const { return reinterpret_cast<const VkPipelineMultisampleStateCreateInfo&>(*this); }
	VkPipelineMultisampleStateCreateInfo& vkHandle() { return reinterpret_cast<VkPipelineMultisampleStateCreateInfo&>(*this); }

	operator const VkPipelineMultisampleStateCreateInfo&() const { return vkHandle(); };
	operator VkPipelineMultisampleStateCreateInfo&() { return vkHandle(); };
};

struct StencilOpState {
	StencilOp failOp {};
	StencilOp passOp {};
	StencilOp depthFailOp {};
	CompareOp compareOp {};
	uint32_t compareMask {};
	uint32_t writeMask {};
	uint32_t reference {};

	const VkStencilOpState& vkHandle() const { return reinterpret_cast<const VkStencilOpState&>(*this); }
	VkStencilOpState& vkHandle() { return reinterpret_cast<VkStencilOpState&>(*this); }

	operator const VkStencilOpState&() const { return vkHandle(); };
	operator VkStencilOpState&() { return vkHandle(); };
};

struct PipelineDepthStencilStateCreateInfo {
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

	const VkPipelineDepthStencilStateCreateInfo& vkHandle() const { return reinterpret_cast<const VkPipelineDepthStencilStateCreateInfo&>(*this); }
	VkPipelineDepthStencilStateCreateInfo& vkHandle() { return reinterpret_cast<VkPipelineDepthStencilStateCreateInfo&>(*this); }

	operator const VkPipelineDepthStencilStateCreateInfo&() const { return vkHandle(); };
	operator VkPipelineDepthStencilStateCreateInfo&() { return vkHandle(); };
};

struct PipelineColorBlendAttachmentState {
	Bool32 blendEnable {};
	BlendFactor srcColorBlendFactor {};
	BlendFactor dstColorBlendFactor {};
	BlendOp colorBlendOp {};
	BlendFactor srcAlphaBlendFactor {};
	BlendFactor dstAlphaBlendFactor {};
	BlendOp alphaBlendOp {};
	ColorComponentFlags colorWriteMask {};

	const VkPipelineColorBlendAttachmentState& vkHandle() const { return reinterpret_cast<const VkPipelineColorBlendAttachmentState&>(*this); }
	VkPipelineColorBlendAttachmentState& vkHandle() { return reinterpret_cast<VkPipelineColorBlendAttachmentState&>(*this); }

	operator const VkPipelineColorBlendAttachmentState&() const { return vkHandle(); };
	operator VkPipelineColorBlendAttachmentState&() { return vkHandle(); };
};

struct PipelineColorBlendStateCreateInfo {
	StructureType sType {StructureType::pipelineColorBlendStateCreateInfo};
	const void* pNext {};
	PipelineColorBlendStateCreateFlags flags {};
	Bool32 logicOpEnable {};
	LogicOp logicOp {};
	uint32_t attachmentCount {};
	const PipelineColorBlendAttachmentState* pAttachments {};
	std::array<float, 4> blendConstants {};

	PipelineColorBlendStateCreateInfo(PipelineColorBlendStateCreateFlags xflags = {}, Bool32 xlogicOpEnable = {}, LogicOp xlogicOp = {}, uint32_t xattachmentCount = {}, const PipelineColorBlendAttachmentState* xpAttachments = {}, std::array<float, 4> xblendConstants = {}) : flags(xflags), logicOpEnable(xlogicOpEnable), logicOp(xlogicOp), attachmentCount(xattachmentCount), pAttachments(xpAttachments), blendConstants(xblendConstants) {}

	const VkPipelineColorBlendStateCreateInfo& vkHandle() const { return reinterpret_cast<const VkPipelineColorBlendStateCreateInfo&>(*this); }
	VkPipelineColorBlendStateCreateInfo& vkHandle() { return reinterpret_cast<VkPipelineColorBlendStateCreateInfo&>(*this); }

	operator const VkPipelineColorBlendStateCreateInfo&() const { return vkHandle(); };
	operator VkPipelineColorBlendStateCreateInfo&() { return vkHandle(); };
};

struct PipelineDynamicStateCreateInfo {
	StructureType sType {StructureType::pipelineDynamicStateCreateInfo};
	const void* pNext {};
	PipelineDynamicStateCreateFlags flags {};
	uint32_t dynamicStateCount {};
	const DynamicState* pDynamicStates {};

	PipelineDynamicStateCreateInfo(PipelineDynamicStateCreateFlags xflags = {}, uint32_t xdynamicStateCount = {}, const DynamicState* xpDynamicStates = {}) : flags(xflags), dynamicStateCount(xdynamicStateCount), pDynamicStates(xpDynamicStates) {}

	const VkPipelineDynamicStateCreateInfo& vkHandle() const { return reinterpret_cast<const VkPipelineDynamicStateCreateInfo&>(*this); }
	VkPipelineDynamicStateCreateInfo& vkHandle() { return reinterpret_cast<VkPipelineDynamicStateCreateInfo&>(*this); }

	operator const VkPipelineDynamicStateCreateInfo&() const { return vkHandle(); };
	operator VkPipelineDynamicStateCreateInfo&() { return vkHandle(); };
};

struct GraphicsPipelineCreateInfo {
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

	const VkGraphicsPipelineCreateInfo& vkHandle() const { return reinterpret_cast<const VkGraphicsPipelineCreateInfo&>(*this); }
	VkGraphicsPipelineCreateInfo& vkHandle() { return reinterpret_cast<VkGraphicsPipelineCreateInfo&>(*this); }

	operator const VkGraphicsPipelineCreateInfo&() const { return vkHandle(); };
	operator VkGraphicsPipelineCreateInfo&() { return vkHandle(); };
};

struct ComputePipelineCreateInfo {
	StructureType sType {StructureType::computePipelineCreateInfo};
	const void* pNext {};
	PipelineCreateFlags flags {};
	PipelineShaderStageCreateInfo stage {};
	PipelineLayout layout {};
	Pipeline basePipelineHandle {};
	int32_t basePipelineIndex {};

	ComputePipelineCreateInfo(PipelineCreateFlags xflags = {}, PipelineShaderStageCreateInfo xstage = {}, PipelineLayout xlayout = {}, Pipeline xbasePipelineHandle = {}, int32_t xbasePipelineIndex = {}) : flags(xflags), stage(xstage), layout(xlayout), basePipelineHandle(xbasePipelineHandle), basePipelineIndex(xbasePipelineIndex) {}

	const VkComputePipelineCreateInfo& vkHandle() const { return reinterpret_cast<const VkComputePipelineCreateInfo&>(*this); }
	VkComputePipelineCreateInfo& vkHandle() { return reinterpret_cast<VkComputePipelineCreateInfo&>(*this); }

	operator const VkComputePipelineCreateInfo&() const { return vkHandle(); };
	operator VkComputePipelineCreateInfo&() { return vkHandle(); };
};

struct PushConstantRange {
	ShaderStageFlags stageFlags {};
	uint32_t offset {};
	uint32_t size {};

	const VkPushConstantRange& vkHandle() const { return reinterpret_cast<const VkPushConstantRange&>(*this); }
	VkPushConstantRange& vkHandle() { return reinterpret_cast<VkPushConstantRange&>(*this); }

	operator const VkPushConstantRange&() const { return vkHandle(); };
	operator VkPushConstantRange&() { return vkHandle(); };
};

struct PipelineLayoutCreateInfo {
	StructureType sType {StructureType::pipelineLayoutCreateInfo};
	const void* pNext {};
	PipelineLayoutCreateFlags flags {};
	uint32_t setLayoutCount {};
	const DescriptorSetLayout* pSetLayouts {};
	uint32_t pushConstantRangeCount {};
	const PushConstantRange* pPushConstantRanges {};

	PipelineLayoutCreateInfo(PipelineLayoutCreateFlags xflags = {}, uint32_t xsetLayoutCount = {}, const DescriptorSetLayout* xpSetLayouts = {}, uint32_t xpushConstantRangeCount = {}, const PushConstantRange* xpPushConstantRanges = {}) : flags(xflags), setLayoutCount(xsetLayoutCount), pSetLayouts(xpSetLayouts), pushConstantRangeCount(xpushConstantRangeCount), pPushConstantRanges(xpPushConstantRanges) {}

	const VkPipelineLayoutCreateInfo& vkHandle() const { return reinterpret_cast<const VkPipelineLayoutCreateInfo&>(*this); }
	VkPipelineLayoutCreateInfo& vkHandle() { return reinterpret_cast<VkPipelineLayoutCreateInfo&>(*this); }

	operator const VkPipelineLayoutCreateInfo&() const { return vkHandle(); };
	operator VkPipelineLayoutCreateInfo&() { return vkHandle(); };
};

struct SamplerCreateInfo {
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

	const VkSamplerCreateInfo& vkHandle() const { return reinterpret_cast<const VkSamplerCreateInfo&>(*this); }
	VkSamplerCreateInfo& vkHandle() { return reinterpret_cast<VkSamplerCreateInfo&>(*this); }

	operator const VkSamplerCreateInfo&() const { return vkHandle(); };
	operator VkSamplerCreateInfo&() { return vkHandle(); };
};

struct DescriptorSetLayoutBinding {
	uint32_t binding {};
	DescriptorType descriptorType {};
	uint32_t descriptorCount {};
	ShaderStageFlags stageFlags {};
	const Sampler* pImmutableSamplers {};

	const VkDescriptorSetLayoutBinding& vkHandle() const { return reinterpret_cast<const VkDescriptorSetLayoutBinding&>(*this); }
	VkDescriptorSetLayoutBinding& vkHandle() { return reinterpret_cast<VkDescriptorSetLayoutBinding&>(*this); }

	operator const VkDescriptorSetLayoutBinding&() const { return vkHandle(); };
	operator VkDescriptorSetLayoutBinding&() { return vkHandle(); };
};

struct DescriptorSetLayoutCreateInfo {
	StructureType sType {StructureType::descriptorSetLayoutCreateInfo};
	const void* pNext {};
	DescriptorSetLayoutCreateFlags flags {};
	uint32_t bindingCount {};
	const DescriptorSetLayoutBinding* pBindings {};

	DescriptorSetLayoutCreateInfo(DescriptorSetLayoutCreateFlags xflags = {}, uint32_t xbindingCount = {}, const DescriptorSetLayoutBinding* xpBindings = {}) : flags(xflags), bindingCount(xbindingCount), pBindings(xpBindings) {}

	const VkDescriptorSetLayoutCreateInfo& vkHandle() const { return reinterpret_cast<const VkDescriptorSetLayoutCreateInfo&>(*this); }
	VkDescriptorSetLayoutCreateInfo& vkHandle() { return reinterpret_cast<VkDescriptorSetLayoutCreateInfo&>(*this); }

	operator const VkDescriptorSetLayoutCreateInfo&() const { return vkHandle(); };
	operator VkDescriptorSetLayoutCreateInfo&() { return vkHandle(); };
};

struct DescriptorPoolSize {
	DescriptorType type {};
	uint32_t descriptorCount {};

	const VkDescriptorPoolSize& vkHandle() const { return reinterpret_cast<const VkDescriptorPoolSize&>(*this); }
	VkDescriptorPoolSize& vkHandle() { return reinterpret_cast<VkDescriptorPoolSize&>(*this); }

	operator const VkDescriptorPoolSize&() const { return vkHandle(); };
	operator VkDescriptorPoolSize&() { return vkHandle(); };
};

struct DescriptorPoolCreateInfo {
	StructureType sType {StructureType::descriptorPoolCreateInfo};
	const void* pNext {};
	DescriptorPoolCreateFlags flags {};
	uint32_t maxSets {};
	uint32_t poolSizeCount {};
	const DescriptorPoolSize* pPoolSizes {};

	DescriptorPoolCreateInfo(DescriptorPoolCreateFlags xflags = {}, uint32_t xmaxSets = {}, uint32_t xpoolSizeCount = {}, const DescriptorPoolSize* xpPoolSizes = {}) : flags(xflags), maxSets(xmaxSets), poolSizeCount(xpoolSizeCount), pPoolSizes(xpPoolSizes) {}

	const VkDescriptorPoolCreateInfo& vkHandle() const { return reinterpret_cast<const VkDescriptorPoolCreateInfo&>(*this); }
	VkDescriptorPoolCreateInfo& vkHandle() { return reinterpret_cast<VkDescriptorPoolCreateInfo&>(*this); }

	operator const VkDescriptorPoolCreateInfo&() const { return vkHandle(); };
	operator VkDescriptorPoolCreateInfo&() { return vkHandle(); };
};

struct DescriptorSetAllocateInfo {
	StructureType sType {StructureType::descriptorSetAllocateInfo};
	const void* pNext {};
	DescriptorPool descriptorPool {};
	uint32_t descriptorSetCount {};
	const DescriptorSetLayout* pSetLayouts {};

	DescriptorSetAllocateInfo(DescriptorPool xdescriptorPool = {}, uint32_t xdescriptorSetCount = {}, const DescriptorSetLayout* xpSetLayouts = {}) : descriptorPool(xdescriptorPool), descriptorSetCount(xdescriptorSetCount), pSetLayouts(xpSetLayouts) {}

	const VkDescriptorSetAllocateInfo& vkHandle() const { return reinterpret_cast<const VkDescriptorSetAllocateInfo&>(*this); }
	VkDescriptorSetAllocateInfo& vkHandle() { return reinterpret_cast<VkDescriptorSetAllocateInfo&>(*this); }

	operator const VkDescriptorSetAllocateInfo&() const { return vkHandle(); };
	operator VkDescriptorSetAllocateInfo&() { return vkHandle(); };
};

struct DescriptorImageInfo {
	Sampler sampler {};
	ImageView imageView {};
	ImageLayout imageLayout {};

	const VkDescriptorImageInfo& vkHandle() const { return reinterpret_cast<const VkDescriptorImageInfo&>(*this); }
	VkDescriptorImageInfo& vkHandle() { return reinterpret_cast<VkDescriptorImageInfo&>(*this); }

	operator const VkDescriptorImageInfo&() const { return vkHandle(); };
	operator VkDescriptorImageInfo&() { return vkHandle(); };
};

struct DescriptorBufferInfo {
	Buffer buffer {};
	DeviceSize offset {};
	DeviceSize range {};

	const VkDescriptorBufferInfo& vkHandle() const { return reinterpret_cast<const VkDescriptorBufferInfo&>(*this); }
	VkDescriptorBufferInfo& vkHandle() { return reinterpret_cast<VkDescriptorBufferInfo&>(*this); }

	operator const VkDescriptorBufferInfo&() const { return vkHandle(); };
	operator VkDescriptorBufferInfo&() { return vkHandle(); };
};

struct WriteDescriptorSet {
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

	const VkWriteDescriptorSet& vkHandle() const { return reinterpret_cast<const VkWriteDescriptorSet&>(*this); }
	VkWriteDescriptorSet& vkHandle() { return reinterpret_cast<VkWriteDescriptorSet&>(*this); }

	operator const VkWriteDescriptorSet&() const { return vkHandle(); };
	operator VkWriteDescriptorSet&() { return vkHandle(); };
};

struct CopyDescriptorSet {
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

	const VkCopyDescriptorSet& vkHandle() const { return reinterpret_cast<const VkCopyDescriptorSet&>(*this); }
	VkCopyDescriptorSet& vkHandle() { return reinterpret_cast<VkCopyDescriptorSet&>(*this); }

	operator const VkCopyDescriptorSet&() const { return vkHandle(); };
	operator VkCopyDescriptorSet&() { return vkHandle(); };
};

struct FramebufferCreateInfo {
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

	const VkFramebufferCreateInfo& vkHandle() const { return reinterpret_cast<const VkFramebufferCreateInfo&>(*this); }
	VkFramebufferCreateInfo& vkHandle() { return reinterpret_cast<VkFramebufferCreateInfo&>(*this); }

	operator const VkFramebufferCreateInfo&() const { return vkHandle(); };
	operator VkFramebufferCreateInfo&() { return vkHandle(); };
};

struct AttachmentDescription {
	AttachmentDescriptionFlags flags {};
	Format format {};
	SampleCountBits samples {};
	AttachmentLoadOp loadOp {};
	AttachmentStoreOp storeOp {};
	AttachmentLoadOp stencilLoadOp {};
	AttachmentStoreOp stencilStoreOp {};
	ImageLayout initialLayout {};
	ImageLayout finalLayout {};

	const VkAttachmentDescription& vkHandle() const { return reinterpret_cast<const VkAttachmentDescription&>(*this); }
	VkAttachmentDescription& vkHandle() { return reinterpret_cast<VkAttachmentDescription&>(*this); }

	operator const VkAttachmentDescription&() const { return vkHandle(); };
	operator VkAttachmentDescription&() { return vkHandle(); };
};

struct AttachmentReference {
	uint32_t attachment {};
	ImageLayout layout {};

	const VkAttachmentReference& vkHandle() const { return reinterpret_cast<const VkAttachmentReference&>(*this); }
	VkAttachmentReference& vkHandle() { return reinterpret_cast<VkAttachmentReference&>(*this); }

	operator const VkAttachmentReference&() const { return vkHandle(); };
	operator VkAttachmentReference&() { return vkHandle(); };
};

struct SubpassDescription {
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

	const VkSubpassDescription& vkHandle() const { return reinterpret_cast<const VkSubpassDescription&>(*this); }
	VkSubpassDescription& vkHandle() { return reinterpret_cast<VkSubpassDescription&>(*this); }

	operator const VkSubpassDescription&() const { return vkHandle(); };
	operator VkSubpassDescription&() { return vkHandle(); };
};

struct SubpassDependency {
	uint32_t srcSubpass {};
	uint32_t dstSubpass {};
	PipelineStageFlags srcStageMask {};
	PipelineStageFlags dstStageMask {};
	AccessFlags srcAccessMask {};
	AccessFlags dstAccessMask {};
	DependencyFlags dependencyFlags {};

	const VkSubpassDependency& vkHandle() const { return reinterpret_cast<const VkSubpassDependency&>(*this); }
	VkSubpassDependency& vkHandle() { return reinterpret_cast<VkSubpassDependency&>(*this); }

	operator const VkSubpassDependency&() const { return vkHandle(); };
	operator VkSubpassDependency&() { return vkHandle(); };
};

struct RenderPassCreateInfo {
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

	const VkRenderPassCreateInfo& vkHandle() const { return reinterpret_cast<const VkRenderPassCreateInfo&>(*this); }
	VkRenderPassCreateInfo& vkHandle() { return reinterpret_cast<VkRenderPassCreateInfo&>(*this); }

	operator const VkRenderPassCreateInfo&() const { return vkHandle(); };
	operator VkRenderPassCreateInfo&() { return vkHandle(); };
};

struct CommandPoolCreateInfo {
	StructureType sType {StructureType::commandPoolCreateInfo};
	const void* pNext {};
	CommandPoolCreateFlags flags {};
	uint32_t queueFamilyIndex {};

	CommandPoolCreateInfo(CommandPoolCreateFlags xflags = {}, uint32_t xqueueFamilyIndex = {}) : flags(xflags), queueFamilyIndex(xqueueFamilyIndex) {}

	const VkCommandPoolCreateInfo& vkHandle() const { return reinterpret_cast<const VkCommandPoolCreateInfo&>(*this); }
	VkCommandPoolCreateInfo& vkHandle() { return reinterpret_cast<VkCommandPoolCreateInfo&>(*this); }

	operator const VkCommandPoolCreateInfo&() const { return vkHandle(); };
	operator VkCommandPoolCreateInfo&() { return vkHandle(); };
};

struct CommandBufferAllocateInfo {
	StructureType sType {StructureType::commandBufferAllocateInfo};
	const void* pNext {};
	CommandPool commandPool {};
	CommandBufferLevel level {};
	uint32_t commandBufferCount {};

	CommandBufferAllocateInfo(CommandPool xcommandPool = {}, CommandBufferLevel xlevel = {}, uint32_t xcommandBufferCount = {}) : commandPool(xcommandPool), level(xlevel), commandBufferCount(xcommandBufferCount) {}

	const VkCommandBufferAllocateInfo& vkHandle() const { return reinterpret_cast<const VkCommandBufferAllocateInfo&>(*this); }
	VkCommandBufferAllocateInfo& vkHandle() { return reinterpret_cast<VkCommandBufferAllocateInfo&>(*this); }

	operator const VkCommandBufferAllocateInfo&() const { return vkHandle(); };
	operator VkCommandBufferAllocateInfo&() { return vkHandle(); };
};

struct CommandBufferInheritanceInfo {
	StructureType sType {StructureType::commandBufferInheritanceInfo};
	const void* pNext {};
	RenderPass renderPass {};
	uint32_t subpass {};
	Framebuffer framebuffer {};
	Bool32 occlusionQueryEnable {};
	QueryControlFlags queryFlags {};
	QueryPipelineStatisticFlags pipelineStatistics {};

	CommandBufferInheritanceInfo(RenderPass xrenderPass = {}, uint32_t xsubpass = {}, Framebuffer xframebuffer = {}, Bool32 xocclusionQueryEnable = {}, QueryControlFlags xqueryFlags = {}, QueryPipelineStatisticFlags xpipelineStatistics = {}) : renderPass(xrenderPass), subpass(xsubpass), framebuffer(xframebuffer), occlusionQueryEnable(xocclusionQueryEnable), queryFlags(xqueryFlags), pipelineStatistics(xpipelineStatistics) {}

	const VkCommandBufferInheritanceInfo& vkHandle() const { return reinterpret_cast<const VkCommandBufferInheritanceInfo&>(*this); }
	VkCommandBufferInheritanceInfo& vkHandle() { return reinterpret_cast<VkCommandBufferInheritanceInfo&>(*this); }

	operator const VkCommandBufferInheritanceInfo&() const { return vkHandle(); };
	operator VkCommandBufferInheritanceInfo&() { return vkHandle(); };
};

struct CommandBufferBeginInfo {
	StructureType sType {StructureType::commandBufferBeginInfo};
	const void* pNext {};
	CommandBufferUsageFlags flags {};
	const CommandBufferInheritanceInfo* pInheritanceInfo {};

	CommandBufferBeginInfo(CommandBufferUsageFlags xflags = {}, const CommandBufferInheritanceInfo* xpInheritanceInfo = {}) : flags(xflags), pInheritanceInfo(xpInheritanceInfo) {}

	const VkCommandBufferBeginInfo& vkHandle() const { return reinterpret_cast<const VkCommandBufferBeginInfo&>(*this); }
	VkCommandBufferBeginInfo& vkHandle() { return reinterpret_cast<VkCommandBufferBeginInfo&>(*this); }

	operator const VkCommandBufferBeginInfo&() const { return vkHandle(); };
	operator VkCommandBufferBeginInfo&() { return vkHandle(); };
};

struct BufferCopy {
	DeviceSize srcOffset {};
	DeviceSize dstOffset {};
	DeviceSize size {};

	const VkBufferCopy& vkHandle() const { return reinterpret_cast<const VkBufferCopy&>(*this); }
	VkBufferCopy& vkHandle() { return reinterpret_cast<VkBufferCopy&>(*this); }

	operator const VkBufferCopy&() const { return vkHandle(); };
	operator VkBufferCopy&() { return vkHandle(); };
};

struct ImageSubresourceLayers {
	ImageAspectFlags aspectMask {};
	uint32_t mipLevel {};
	uint32_t baseArrayLayer {};
	uint32_t layerCount {};

	const VkImageSubresourceLayers& vkHandle() const { return reinterpret_cast<const VkImageSubresourceLayers&>(*this); }
	VkImageSubresourceLayers& vkHandle() { return reinterpret_cast<VkImageSubresourceLayers&>(*this); }

	operator const VkImageSubresourceLayers&() const { return vkHandle(); };
	operator VkImageSubresourceLayers&() { return vkHandle(); };
};

struct ImageCopy {
	ImageSubresourceLayers srcSubresource {};
	Offset3D srcOffset {};
	ImageSubresourceLayers dstSubresource {};
	Offset3D dstOffset {};
	Extent3D extent {};

	const VkImageCopy& vkHandle() const { return reinterpret_cast<const VkImageCopy&>(*this); }
	VkImageCopy& vkHandle() { return reinterpret_cast<VkImageCopy&>(*this); }

	operator const VkImageCopy&() const { return vkHandle(); };
	operator VkImageCopy&() { return vkHandle(); };
};

struct ImageBlit {
	ImageSubresourceLayers srcSubresource {};
	std::array<Offset3D, 2> srcOffsets {};
	ImageSubresourceLayers dstSubresource {};
	std::array<Offset3D, 2> dstOffsets {};

	const VkImageBlit& vkHandle() const { return reinterpret_cast<const VkImageBlit&>(*this); }
	VkImageBlit& vkHandle() { return reinterpret_cast<VkImageBlit&>(*this); }

	operator const VkImageBlit&() const { return vkHandle(); };
	operator VkImageBlit&() { return vkHandle(); };
};

struct BufferImageCopy {
	DeviceSize bufferOffset {};
	uint32_t bufferRowLength {};
	uint32_t bufferImageHeight {};
	ImageSubresourceLayers imageSubresource {};
	Offset3D imageOffset {};
	Extent3D imageExtent {};

	const VkBufferImageCopy& vkHandle() const { return reinterpret_cast<const VkBufferImageCopy&>(*this); }
	VkBufferImageCopy& vkHandle() { return reinterpret_cast<VkBufferImageCopy&>(*this); }

	operator const VkBufferImageCopy&() const { return vkHandle(); };
	operator VkBufferImageCopy&() { return vkHandle(); };
};

union ClearColorValue {
	std::array<float, 4> float32 {};
	std::array<int32_t, 4> int32;
	std::array<uint32_t, 4> uint32;

	const VkClearColorValue& vkHandle() const { return reinterpret_cast<const VkClearColorValue&>(*this); }
	VkClearColorValue& vkHandle() { return reinterpret_cast<VkClearColorValue&>(*this); }

	operator const VkClearColorValue&() const { return vkHandle(); };
	operator VkClearColorValue&() { return vkHandle(); };
};

struct ClearDepthStencilValue {
	float depth {};
	uint32_t stencil {};

	const VkClearDepthStencilValue& vkHandle() const { return reinterpret_cast<const VkClearDepthStencilValue&>(*this); }
	VkClearDepthStencilValue& vkHandle() { return reinterpret_cast<VkClearDepthStencilValue&>(*this); }

	operator const VkClearDepthStencilValue&() const { return vkHandle(); };
	operator VkClearDepthStencilValue&() { return vkHandle(); };
};

union ClearValue {
	ClearColorValue color {};
	ClearDepthStencilValue depthStencil;

	const VkClearValue& vkHandle() const { return reinterpret_cast<const VkClearValue&>(*this); }
	VkClearValue& vkHandle() { return reinterpret_cast<VkClearValue&>(*this); }

	operator const VkClearValue&() const { return vkHandle(); };
	operator VkClearValue&() { return vkHandle(); };
};

struct ClearAttachment {
	ImageAspectFlags aspectMask {};
	uint32_t colorAttachment {};
	ClearValue clearValue {};

	const VkClearAttachment& vkHandle() const { return reinterpret_cast<const VkClearAttachment&>(*this); }
	VkClearAttachment& vkHandle() { return reinterpret_cast<VkClearAttachment&>(*this); }

	operator const VkClearAttachment&() const { return vkHandle(); };
	operator VkClearAttachment&() { return vkHandle(); };
};

struct ClearRect {
	Rect2D rect {};
	uint32_t baseArrayLayer {};
	uint32_t layerCount {};

	const VkClearRect& vkHandle() const { return reinterpret_cast<const VkClearRect&>(*this); }
	VkClearRect& vkHandle() { return reinterpret_cast<VkClearRect&>(*this); }

	operator const VkClearRect&() const { return vkHandle(); };
	operator VkClearRect&() { return vkHandle(); };
};

struct ImageResolve {
	ImageSubresourceLayers srcSubresource {};
	Offset3D srcOffset {};
	ImageSubresourceLayers dstSubresource {};
	Offset3D dstOffset {};
	Extent3D extent {};

	const VkImageResolve& vkHandle() const { return reinterpret_cast<const VkImageResolve&>(*this); }
	VkImageResolve& vkHandle() { return reinterpret_cast<VkImageResolve&>(*this); }

	operator const VkImageResolve&() const { return vkHandle(); };
	operator VkImageResolve&() { return vkHandle(); };
};

struct MemoryBarrier {
	StructureType sType {StructureType::memoryBarrier};
	const void* pNext {};
	AccessFlags srcAccessMask {};
	AccessFlags dstAccessMask {};

	MemoryBarrier(AccessFlags xsrcAccessMask = {}, AccessFlags xdstAccessMask = {}) : srcAccessMask(xsrcAccessMask), dstAccessMask(xdstAccessMask) {}

	const VkMemoryBarrier& vkHandle() const { return reinterpret_cast<const VkMemoryBarrier&>(*this); }
	VkMemoryBarrier& vkHandle() { return reinterpret_cast<VkMemoryBarrier&>(*this); }

	operator const VkMemoryBarrier&() const { return vkHandle(); };
	operator VkMemoryBarrier&() { return vkHandle(); };
};

struct BufferMemoryBarrier {
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

	const VkBufferMemoryBarrier& vkHandle() const { return reinterpret_cast<const VkBufferMemoryBarrier&>(*this); }
	VkBufferMemoryBarrier& vkHandle() { return reinterpret_cast<VkBufferMemoryBarrier&>(*this); }

	operator const VkBufferMemoryBarrier&() const { return vkHandle(); };
	operator VkBufferMemoryBarrier&() { return vkHandle(); };
};

struct ImageMemoryBarrier {
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

	const VkImageMemoryBarrier& vkHandle() const { return reinterpret_cast<const VkImageMemoryBarrier&>(*this); }
	VkImageMemoryBarrier& vkHandle() { return reinterpret_cast<VkImageMemoryBarrier&>(*this); }

	operator const VkImageMemoryBarrier&() const { return vkHandle(); };
	operator VkImageMemoryBarrier&() { return vkHandle(); };
};

struct RenderPassBeginInfo {
	StructureType sType {StructureType::renderPassBeginInfo};
	const void* pNext {};
	RenderPass renderPass {};
	Framebuffer framebuffer {};
	Rect2D renderArea {};
	uint32_t clearValueCount {};
	const ClearValue* pClearValues {};

	RenderPassBeginInfo(RenderPass xrenderPass = {}, Framebuffer xframebuffer = {}, Rect2D xrenderArea = {}, uint32_t xclearValueCount = {}, const ClearValue* xpClearValues = {}) : renderPass(xrenderPass), framebuffer(xframebuffer), renderArea(xrenderArea), clearValueCount(xclearValueCount), pClearValues(xpClearValues) {}

	const VkRenderPassBeginInfo& vkHandle() const { return reinterpret_cast<const VkRenderPassBeginInfo&>(*this); }
	VkRenderPassBeginInfo& vkHandle() { return reinterpret_cast<VkRenderPassBeginInfo&>(*this); }

	operator const VkRenderPassBeginInfo&() const { return vkHandle(); };
	operator VkRenderPassBeginInfo&() { return vkHandle(); };
};

struct DispatchIndirectCommand {
	uint32_t x {};
	uint32_t y {};
	uint32_t z {};

	const VkDispatchIndirectCommand& vkHandle() const { return reinterpret_cast<const VkDispatchIndirectCommand&>(*this); }
	VkDispatchIndirectCommand& vkHandle() { return reinterpret_cast<VkDispatchIndirectCommand&>(*this); }

	operator const VkDispatchIndirectCommand&() const { return vkHandle(); };
	operator VkDispatchIndirectCommand&() { return vkHandle(); };
};

struct DrawIndexedIndirectCommand {
	uint32_t indexCount {};
	uint32_t instanceCount {};
	uint32_t firstIndex {};
	int32_t vertexOffset {};
	uint32_t firstInstance {};

	const VkDrawIndexedIndirectCommand& vkHandle() const { return reinterpret_cast<const VkDrawIndexedIndirectCommand&>(*this); }
	VkDrawIndexedIndirectCommand& vkHandle() { return reinterpret_cast<VkDrawIndexedIndirectCommand&>(*this); }

	operator const VkDrawIndexedIndirectCommand&() const { return vkHandle(); };
	operator VkDrawIndexedIndirectCommand&() { return vkHandle(); };
};

struct DrawIndirectCommand {
	uint32_t vertexCount {};
	uint32_t instanceCount {};
	uint32_t firstVertex {};
	uint32_t firstInstance {};

	const VkDrawIndirectCommand& vkHandle() const { return reinterpret_cast<const VkDrawIndirectCommand&>(*this); }
	VkDrawIndirectCommand& vkHandle() { return reinterpret_cast<VkDrawIndirectCommand&>(*this); }

	operator const VkDrawIndirectCommand&() const { return vkHandle(); };
	operator VkDrawIndirectCommand&() { return vkHandle(); };
};


struct SurfaceCapabilitiesKHR {
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

	const VkSurfaceCapabilitiesKHR& vkHandle() const { return reinterpret_cast<const VkSurfaceCapabilitiesKHR&>(*this); }
	VkSurfaceCapabilitiesKHR& vkHandle() { return reinterpret_cast<VkSurfaceCapabilitiesKHR&>(*this); }

	operator const VkSurfaceCapabilitiesKHR&() const { return vkHandle(); };
	operator VkSurfaceCapabilitiesKHR&() { return vkHandle(); };
};

struct SurfaceFormatKHR {
	Format format {};
	ColorSpaceKHR colorSpace {};

	const VkSurfaceFormatKHR& vkHandle() const { return reinterpret_cast<const VkSurfaceFormatKHR&>(*this); }
	VkSurfaceFormatKHR& vkHandle() { return reinterpret_cast<VkSurfaceFormatKHR&>(*this); }

	operator const VkSurfaceFormatKHR&() const { return vkHandle(); };
	operator VkSurfaceFormatKHR&() { return vkHandle(); };
};


struct SwapchainCreateInfoKHR {
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

	const VkSwapchainCreateInfoKHR& vkHandle() const { return reinterpret_cast<const VkSwapchainCreateInfoKHR&>(*this); }
	VkSwapchainCreateInfoKHR& vkHandle() { return reinterpret_cast<VkSwapchainCreateInfoKHR&>(*this); }

	operator const VkSwapchainCreateInfoKHR&() const { return vkHandle(); };
	operator VkSwapchainCreateInfoKHR&() { return vkHandle(); };
};

struct PresentInfoKHR {
	StructureType sType {StructureType::presentInfoKHR};
	const void* pNext {};
	uint32_t waitSemaphoreCount {};
	const Semaphore* pWaitSemaphores {};
	uint32_t swapchainCount {};
	const SwapchainKHR* pSwapchains {};
	const uint32_t* pImageIndices {};
	Result* pResults {};

	PresentInfoKHR(uint32_t xwaitSemaphoreCount = {}, const Semaphore* xpWaitSemaphores = {}, uint32_t xswapchainCount = {}, const SwapchainKHR* xpSwapchains = {}, const uint32_t* xpImageIndices = {}, Result* xpResults = {}) : waitSemaphoreCount(xwaitSemaphoreCount), pWaitSemaphores(xpWaitSemaphores), swapchainCount(xswapchainCount), pSwapchains(xpSwapchains), pImageIndices(xpImageIndices), pResults(xpResults) {}

	const VkPresentInfoKHR& vkHandle() const { return reinterpret_cast<const VkPresentInfoKHR&>(*this); }
	VkPresentInfoKHR& vkHandle() { return reinterpret_cast<VkPresentInfoKHR&>(*this); }

	operator const VkPresentInfoKHR&() const { return vkHandle(); };
	operator VkPresentInfoKHR&() { return vkHandle(); };
};


struct DisplayPropertiesKHR {
	DisplayKHR display {};
	const char* displayName {};
	Extent2D physicalDimensions {};
	Extent2D physicalResolution {};
	SurfaceTransformFlagsKHR supportedTransforms {};
	Bool32 planeReorderPossible {};
	Bool32 persistentContent {};

	const VkDisplayPropertiesKHR& vkHandle() const { return reinterpret_cast<const VkDisplayPropertiesKHR&>(*this); }
	VkDisplayPropertiesKHR& vkHandle() { return reinterpret_cast<VkDisplayPropertiesKHR&>(*this); }

	operator const VkDisplayPropertiesKHR&() const { return vkHandle(); };
	operator VkDisplayPropertiesKHR&() { return vkHandle(); };
};

struct DisplayModeParametersKHR {
	Extent2D visibleRegion {};
	uint32_t refreshRate {};

	const VkDisplayModeParametersKHR& vkHandle() const { return reinterpret_cast<const VkDisplayModeParametersKHR&>(*this); }
	VkDisplayModeParametersKHR& vkHandle() { return reinterpret_cast<VkDisplayModeParametersKHR&>(*this); }

	operator const VkDisplayModeParametersKHR&() const { return vkHandle(); };
	operator VkDisplayModeParametersKHR&() { return vkHandle(); };
};

struct DisplayModePropertiesKHR {
	DisplayModeKHR displayMode {};
	DisplayModeParametersKHR parameters {};

	const VkDisplayModePropertiesKHR& vkHandle() const { return reinterpret_cast<const VkDisplayModePropertiesKHR&>(*this); }
	VkDisplayModePropertiesKHR& vkHandle() { return reinterpret_cast<VkDisplayModePropertiesKHR&>(*this); }

	operator const VkDisplayModePropertiesKHR&() const { return vkHandle(); };
	operator VkDisplayModePropertiesKHR&() { return vkHandle(); };
};

struct DisplayModeCreateInfoKHR {
	StructureType sType {StructureType::displayModeCreateInfoKHR};
	const void* pNext {};
	DisplayModeCreateFlagsKHR flags {};
	DisplayModeParametersKHR parameters {};

	DisplayModeCreateInfoKHR(DisplayModeCreateFlagsKHR xflags = {}, DisplayModeParametersKHR xparameters = {}) : flags(xflags), parameters(xparameters) {}

	const VkDisplayModeCreateInfoKHR& vkHandle() const { return reinterpret_cast<const VkDisplayModeCreateInfoKHR&>(*this); }
	VkDisplayModeCreateInfoKHR& vkHandle() { return reinterpret_cast<VkDisplayModeCreateInfoKHR&>(*this); }

	operator const VkDisplayModeCreateInfoKHR&() const { return vkHandle(); };
	operator VkDisplayModeCreateInfoKHR&() { return vkHandle(); };
};

struct DisplayPlaneCapabilitiesKHR {
	DisplayPlaneAlphaFlagsKHR supportedAlpha {};
	Offset2D minSrcPosition {};
	Offset2D maxSrcPosition {};
	Extent2D minSrcExtent {};
	Extent2D maxSrcExtent {};
	Offset2D minDstPosition {};
	Offset2D maxDstPosition {};
	Extent2D minDstExtent {};
	Extent2D maxDstExtent {};

	const VkDisplayPlaneCapabilitiesKHR& vkHandle() const { return reinterpret_cast<const VkDisplayPlaneCapabilitiesKHR&>(*this); }
	VkDisplayPlaneCapabilitiesKHR& vkHandle() { return reinterpret_cast<VkDisplayPlaneCapabilitiesKHR&>(*this); }

	operator const VkDisplayPlaneCapabilitiesKHR&() const { return vkHandle(); };
	operator VkDisplayPlaneCapabilitiesKHR&() { return vkHandle(); };
};

struct DisplayPlanePropertiesKHR {
	DisplayKHR currentDisplay {};
	uint32_t currentStackIndex {};

	const VkDisplayPlanePropertiesKHR& vkHandle() const { return reinterpret_cast<const VkDisplayPlanePropertiesKHR&>(*this); }
	VkDisplayPlanePropertiesKHR& vkHandle() { return reinterpret_cast<VkDisplayPlanePropertiesKHR&>(*this); }

	operator const VkDisplayPlanePropertiesKHR&() const { return vkHandle(); };
	operator VkDisplayPlanePropertiesKHR&() { return vkHandle(); };
};

struct DisplaySurfaceCreateInfoKHR {
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

	const VkDisplaySurfaceCreateInfoKHR& vkHandle() const { return reinterpret_cast<const VkDisplaySurfaceCreateInfoKHR&>(*this); }
	VkDisplaySurfaceCreateInfoKHR& vkHandle() { return reinterpret_cast<VkDisplaySurfaceCreateInfoKHR&>(*this); }

	operator const VkDisplaySurfaceCreateInfoKHR&() const { return vkHandle(); };
	operator VkDisplaySurfaceCreateInfoKHR&() { return vkHandle(); };
};


struct DisplayPresentInfoKHR {
	StructureType sType {StructureType::displayPresentInfoKHR};
	const void* pNext {};
	Rect2D srcRect {};
	Rect2D dstRect {};
	Bool32 persistent {};

	DisplayPresentInfoKHR(Rect2D xsrcRect = {}, Rect2D xdstRect = {}, Bool32 xpersistent = {}) : srcRect(xsrcRect), dstRect(xdstRect), persistent(xpersistent) {}

	const VkDisplayPresentInfoKHR& vkHandle() const { return reinterpret_cast<const VkDisplayPresentInfoKHR&>(*this); }
	VkDisplayPresentInfoKHR& vkHandle() { return reinterpret_cast<VkDisplayPresentInfoKHR&>(*this); }

	operator const VkDisplayPresentInfoKHR&() const { return vkHandle(); };
	operator VkDisplayPresentInfoKHR&() { return vkHandle(); };
};


#ifdef VK_USE_PLATFORM_XLIB_KHR

struct XlibSurfaceCreateInfoKHR {
	StructureType sType {StructureType::xlibSurfaceCreateInfoKHR};
	const void* pNext {};
	XlibSurfaceCreateFlagsKHR flags {};
	Display* dpy {};
	Window window {};

	XlibSurfaceCreateInfoKHR(XlibSurfaceCreateFlagsKHR xflags = {}, Display* xdpy = {}, Window xwindow = {}) : flags(xflags), dpy(xdpy), window(xwindow) {}

	const VkXlibSurfaceCreateInfoKHR& vkHandle() const { return reinterpret_cast<const VkXlibSurfaceCreateInfoKHR&>(*this); }
	VkXlibSurfaceCreateInfoKHR& vkHandle() { return reinterpret_cast<VkXlibSurfaceCreateInfoKHR&>(*this); }

	operator const VkXlibSurfaceCreateInfoKHR&() const { return vkHandle(); };
	operator VkXlibSurfaceCreateInfoKHR&() { return vkHandle(); };
};


#endif //VK_USE_PLATFORM_XLIB_KHR

#ifdef VK_USE_PLATFORM_XCB_KHR

struct XcbSurfaceCreateInfoKHR {
	StructureType sType {StructureType::xcbSurfaceCreateInfoKHR};
	const void* pNext {};
	XcbSurfaceCreateFlagsKHR flags {};
	xcb_connection_t* connection {};
	xcb_window_t window {};

	XcbSurfaceCreateInfoKHR(XcbSurfaceCreateFlagsKHR xflags = {}, xcb_connection_t* xconnection = {}, xcb_window_t xwindow = {}) : flags(xflags), connection(xconnection), window(xwindow) {}

	const VkXcbSurfaceCreateInfoKHR& vkHandle() const { return reinterpret_cast<const VkXcbSurfaceCreateInfoKHR&>(*this); }
	VkXcbSurfaceCreateInfoKHR& vkHandle() { return reinterpret_cast<VkXcbSurfaceCreateInfoKHR&>(*this); }

	operator const VkXcbSurfaceCreateInfoKHR&() const { return vkHandle(); };
	operator VkXcbSurfaceCreateInfoKHR&() { return vkHandle(); };
};


#endif //VK_USE_PLATFORM_XCB_KHR

#ifdef VK_USE_PLATFORM_WAYLAND_KHR

struct WaylandSurfaceCreateInfoKHR {
	StructureType sType {StructureType::waylandSurfaceCreateInfoKHR};
	const void* pNext {};
	WaylandSurfaceCreateFlagsKHR flags {};
	wl_display* display {};
	wl_surface* surface {};

	WaylandSurfaceCreateInfoKHR(WaylandSurfaceCreateFlagsKHR xflags = {}, wl_display* xdisplay = {}, wl_surface* xsurface = {}) : flags(xflags), display(xdisplay), surface(xsurface) {}

	const VkWaylandSurfaceCreateInfoKHR& vkHandle() const { return reinterpret_cast<const VkWaylandSurfaceCreateInfoKHR&>(*this); }
	VkWaylandSurfaceCreateInfoKHR& vkHandle() { return reinterpret_cast<VkWaylandSurfaceCreateInfoKHR&>(*this); }

	operator const VkWaylandSurfaceCreateInfoKHR&() const { return vkHandle(); };
	operator VkWaylandSurfaceCreateInfoKHR&() { return vkHandle(); };
};


#endif //VK_USE_PLATFORM_WAYLAND_KHR

#ifdef VK_USE_PLATFORM_MIR_KHR

struct MirSurfaceCreateInfoKHR {
	StructureType sType {StructureType::mirSurfaceCreateInfoKHR};
	const void* pNext {};
	MirSurfaceCreateFlagsKHR flags {};
	MirConnection* connection {};
	MirSurface* mirSurface {};

	MirSurfaceCreateInfoKHR(MirSurfaceCreateFlagsKHR xflags = {}, MirConnection* xconnection = {}, MirSurface* xmirSurface = {}) : flags(xflags), connection(xconnection), mirSurface(xmirSurface) {}

	const VkMirSurfaceCreateInfoKHR& vkHandle() const { return reinterpret_cast<const VkMirSurfaceCreateInfoKHR&>(*this); }
	VkMirSurfaceCreateInfoKHR& vkHandle() { return reinterpret_cast<VkMirSurfaceCreateInfoKHR&>(*this); }

	operator const VkMirSurfaceCreateInfoKHR&() const { return vkHandle(); };
	operator VkMirSurfaceCreateInfoKHR&() { return vkHandle(); };
};


#endif //VK_USE_PLATFORM_MIR_KHR

#ifdef VK_USE_PLATFORM_ANDROID_KHR

struct AndroidSurfaceCreateInfoKHR {
	StructureType sType {StructureType::androidSurfaceCreateInfoKHR};
	const void* pNext {};
	AndroidSurfaceCreateFlagsKHR flags {};
	ANativeWindow* window {};

	AndroidSurfaceCreateInfoKHR(AndroidSurfaceCreateFlagsKHR xflags = {}, ANativeWindow* xwindow = {}) : flags(xflags), window(xwindow) {}

	const VkAndroidSurfaceCreateInfoKHR& vkHandle() const { return reinterpret_cast<const VkAndroidSurfaceCreateInfoKHR&>(*this); }
	VkAndroidSurfaceCreateInfoKHR& vkHandle() { return reinterpret_cast<VkAndroidSurfaceCreateInfoKHR&>(*this); }

	operator const VkAndroidSurfaceCreateInfoKHR&() const { return vkHandle(); };
	operator VkAndroidSurfaceCreateInfoKHR&() { return vkHandle(); };
};


#endif //VK_USE_PLATFORM_ANDROID_KHR

#ifdef VK_USE_PLATFORM_WIN32_KHR

struct Win32SurfaceCreateInfoKHR {
	StructureType sType {StructureType::win32SurfaceCreateInfoKHR};
	const void* pNext {};
	Win32SurfaceCreateFlagsKHR flags {};
	HINSTANCE hinstance {};
	HWND hwnd {};

	Win32SurfaceCreateInfoKHR(Win32SurfaceCreateFlagsKHR xflags = {}, HINSTANCE xhinstance = {}, HWND xhwnd = {}) : flags(xflags), hinstance(xhinstance), hwnd(xhwnd) {}

	const VkWin32SurfaceCreateInfoKHR& vkHandle() const { return reinterpret_cast<const VkWin32SurfaceCreateInfoKHR&>(*this); }
	VkWin32SurfaceCreateInfoKHR& vkHandle() { return reinterpret_cast<VkWin32SurfaceCreateInfoKHR&>(*this); }

	operator const VkWin32SurfaceCreateInfoKHR&() const { return vkHandle(); };
	operator VkWin32SurfaceCreateInfoKHR&() { return vkHandle(); };
};


#endif //VK_USE_PLATFORM_WIN32_KHR

struct DebugReportCallbackCreateInfoEXT {
	StructureType sType {StructureType::debugReportCallbackCreateInfoEXT};
	const void* pNext {};
	DebugReportFlagsEXT flags {};
	PfnDebugReportCallbackEXT pfnCallback {};
	void* pUserData {};

	DebugReportCallbackCreateInfoEXT(DebugReportFlagsEXT xflags = {}, PfnDebugReportCallbackEXT xpfnCallback = {}, void* xpUserData = {}) : flags(xflags), pfnCallback(xpfnCallback), pUserData(xpUserData) {}

	const VkDebugReportCallbackCreateInfoEXT& vkHandle() const { return reinterpret_cast<const VkDebugReportCallbackCreateInfoEXT&>(*this); }
	VkDebugReportCallbackCreateInfoEXT& vkHandle() { return reinterpret_cast<VkDebugReportCallbackCreateInfoEXT&>(*this); }

	operator const VkDebugReportCallbackCreateInfoEXT&() const { return vkHandle(); };
	operator VkDebugReportCallbackCreateInfoEXT&() { return vkHandle(); };
};


struct PipelineRasterizationStateRasterizationOrderAMD {
	StructureType sType {StructureType::pipelineRasterizationStateRasterizationOrderAMD};
	const void* pNext {};
	RasterizationOrderAMD rasterizationOrder {};

	PipelineRasterizationStateRasterizationOrderAMD(RasterizationOrderAMD xrasterizationOrder = {}) : rasterizationOrder(xrasterizationOrder) {}

	const VkPipelineRasterizationStateRasterizationOrderAMD& vkHandle() const { return reinterpret_cast<const VkPipelineRasterizationStateRasterizationOrderAMD&>(*this); }
	VkPipelineRasterizationStateRasterizationOrderAMD& vkHandle() { return reinterpret_cast<VkPipelineRasterizationStateRasterizationOrderAMD&>(*this); }

	operator const VkPipelineRasterizationStateRasterizationOrderAMD&() const { return vkHandle(); };
	operator VkPipelineRasterizationStateRasterizationOrderAMD&() { return vkHandle(); };
};


struct DebugMarkerObjectNameInfoEXT {
	StructureType sType {StructureType::debugMarkerObjectNameInfoEXT};
	const void* pNext {};
	DebugReportObjectTypeEXT objectType {};
	uint64_t object {};
	const char* pObjectName {};

	DebugMarkerObjectNameInfoEXT(DebugReportObjectTypeEXT xobjectType = {}, uint64_t xobject = {}, const char* xpObjectName = {}) : objectType(xobjectType), object(xobject), pObjectName(xpObjectName) {}

	const VkDebugMarkerObjectNameInfoEXT& vkHandle() const { return reinterpret_cast<const VkDebugMarkerObjectNameInfoEXT&>(*this); }
	VkDebugMarkerObjectNameInfoEXT& vkHandle() { return reinterpret_cast<VkDebugMarkerObjectNameInfoEXT&>(*this); }

	operator const VkDebugMarkerObjectNameInfoEXT&() const { return vkHandle(); };
	operator VkDebugMarkerObjectNameInfoEXT&() { return vkHandle(); };
};

struct DebugMarkerObjectTagInfoEXT {
	StructureType sType {StructureType::debugMarkerObjectTagInfoEXT};
	const void* pNext {};
	DebugReportObjectTypeEXT objectType {};
	uint64_t object {};
	uint64_t tagName {};
	size_t tagSize {};
	const void* pTag {};

	DebugMarkerObjectTagInfoEXT(DebugReportObjectTypeEXT xobjectType = {}, uint64_t xobject = {}, uint64_t xtagName = {}, size_t xtagSize = {}, const void* xpTag = {}) : objectType(xobjectType), object(xobject), tagName(xtagName), tagSize(xtagSize), pTag(xpTag) {}

	const VkDebugMarkerObjectTagInfoEXT& vkHandle() const { return reinterpret_cast<const VkDebugMarkerObjectTagInfoEXT&>(*this); }
	VkDebugMarkerObjectTagInfoEXT& vkHandle() { return reinterpret_cast<VkDebugMarkerObjectTagInfoEXT&>(*this); }

	operator const VkDebugMarkerObjectTagInfoEXT&() const { return vkHandle(); };
	operator VkDebugMarkerObjectTagInfoEXT&() { return vkHandle(); };
};

struct DebugMarkerMarkerInfoEXT {
	StructureType sType {StructureType::debugMarkerMarkerInfoEXT};
	const void* pNext {};
	const char* pMarkerName {};
	std::array<float, 4> color {};

	DebugMarkerMarkerInfoEXT(const char* xpMarkerName = {}, std::array<float, 4> xcolor = {}) : pMarkerName(xpMarkerName), color(xcolor) {}

	const VkDebugMarkerMarkerInfoEXT& vkHandle() const { return reinterpret_cast<const VkDebugMarkerMarkerInfoEXT&>(*this); }
	VkDebugMarkerMarkerInfoEXT& vkHandle() { return reinterpret_cast<VkDebugMarkerMarkerInfoEXT&>(*this); }

	operator const VkDebugMarkerMarkerInfoEXT&() const { return vkHandle(); };
	operator VkDebugMarkerMarkerInfoEXT&() { return vkHandle(); };
};


struct DedicatedAllocationImageCreateInfoNV {
	StructureType sType {StructureType::dedicatedAllocationImageCreateInfoNV};
	const void* pNext {};
	Bool32 dedicatedAllocation {};

	DedicatedAllocationImageCreateInfoNV(Bool32 xdedicatedAllocation = {}) : dedicatedAllocation(xdedicatedAllocation) {}

	const VkDedicatedAllocationImageCreateInfoNV& vkHandle() const { return reinterpret_cast<const VkDedicatedAllocationImageCreateInfoNV&>(*this); }
	VkDedicatedAllocationImageCreateInfoNV& vkHandle() { return reinterpret_cast<VkDedicatedAllocationImageCreateInfoNV&>(*this); }

	operator const VkDedicatedAllocationImageCreateInfoNV&() const { return vkHandle(); };
	operator VkDedicatedAllocationImageCreateInfoNV&() { return vkHandle(); };
};

struct DedicatedAllocationBufferCreateInfoNV {
	StructureType sType {StructureType::dedicatedAllocationBufferCreateInfoNV};
	const void* pNext {};
	Bool32 dedicatedAllocation {};

	DedicatedAllocationBufferCreateInfoNV(Bool32 xdedicatedAllocation = {}) : dedicatedAllocation(xdedicatedAllocation) {}

	const VkDedicatedAllocationBufferCreateInfoNV& vkHandle() const { return reinterpret_cast<const VkDedicatedAllocationBufferCreateInfoNV&>(*this); }
	VkDedicatedAllocationBufferCreateInfoNV& vkHandle() { return reinterpret_cast<VkDedicatedAllocationBufferCreateInfoNV&>(*this); }

	operator const VkDedicatedAllocationBufferCreateInfoNV&() const { return vkHandle(); };
	operator VkDedicatedAllocationBufferCreateInfoNV&() { return vkHandle(); };
};

struct DedicatedAllocationMemoryAllocateInfoNV {
	StructureType sType {StructureType::dedicatedAllocationMemoryAllocateInfoNV};
	const void* pNext {};
	Image image {};
	Buffer buffer {};

	DedicatedAllocationMemoryAllocateInfoNV(Image ximage = {}, Buffer xbuffer = {}) : image(ximage), buffer(xbuffer) {}

	const VkDedicatedAllocationMemoryAllocateInfoNV& vkHandle() const { return reinterpret_cast<const VkDedicatedAllocationMemoryAllocateInfoNV&>(*this); }
	VkDedicatedAllocationMemoryAllocateInfoNV& vkHandle() { return reinterpret_cast<VkDedicatedAllocationMemoryAllocateInfoNV&>(*this); }

	operator const VkDedicatedAllocationMemoryAllocateInfoNV&() const { return vkHandle(); };
	operator VkDedicatedAllocationMemoryAllocateInfoNV&() { return vkHandle(); };
};


struct RenderPassMultiviewCreateInfoKHX {
	StructureType sType {StructureType::renderPassMultiviewCreateInfoKHX};
	const void* pNext {};
	uint32_t subpassCount {};
	const uint32_t* pViewMasks {};
	uint32_t dependencyCount {};
	const int32_t* pViewOffsets {};
	uint32_t correlationMaskCount {};
	const uint32_t* pCorrelationMasks {};

	RenderPassMultiviewCreateInfoKHX(uint32_t xsubpassCount = {}, const uint32_t* xpViewMasks = {}, uint32_t xdependencyCount = {}, const int32_t* xpViewOffsets = {}, uint32_t xcorrelationMaskCount = {}, const uint32_t* xpCorrelationMasks = {}) : subpassCount(xsubpassCount), pViewMasks(xpViewMasks), dependencyCount(xdependencyCount), pViewOffsets(xpViewOffsets), correlationMaskCount(xcorrelationMaskCount), pCorrelationMasks(xpCorrelationMasks) {}

	const VkRenderPassMultiviewCreateInfoKHX& vkHandle() const { return reinterpret_cast<const VkRenderPassMultiviewCreateInfoKHX&>(*this); }
	VkRenderPassMultiviewCreateInfoKHX& vkHandle() { return reinterpret_cast<VkRenderPassMultiviewCreateInfoKHX&>(*this); }

	operator const VkRenderPassMultiviewCreateInfoKHX&() const { return vkHandle(); };
	operator VkRenderPassMultiviewCreateInfoKHX&() { return vkHandle(); };
};

struct PhysicalDeviceMultiviewFeaturesKHX {
	StructureType sType {StructureType::physicalDeviceMultiviewFeaturesKHX};
	void* pNext {};
	Bool32 multiview {};
	Bool32 multiviewGeometryShader {};
	Bool32 multiviewTessellationShader {};

	PhysicalDeviceMultiviewFeaturesKHX(Bool32 xmultiview = {}, Bool32 xmultiviewGeometryShader = {}, Bool32 xmultiviewTessellationShader = {}) : multiview(xmultiview), multiviewGeometryShader(xmultiviewGeometryShader), multiviewTessellationShader(xmultiviewTessellationShader) {}

	const VkPhysicalDeviceMultiviewFeaturesKHX& vkHandle() const { return reinterpret_cast<const VkPhysicalDeviceMultiviewFeaturesKHX&>(*this); }
	VkPhysicalDeviceMultiviewFeaturesKHX& vkHandle() { return reinterpret_cast<VkPhysicalDeviceMultiviewFeaturesKHX&>(*this); }

	operator const VkPhysicalDeviceMultiviewFeaturesKHX&() const { return vkHandle(); };
	operator VkPhysicalDeviceMultiviewFeaturesKHX&() { return vkHandle(); };
};

struct PhysicalDeviceMultiviewPropertiesKHX {
	StructureType sType {StructureType::physicalDeviceMultiviewPropertiesKHX};
	void* pNext {};
	uint32_t maxMultiviewViewCount {};
	uint32_t maxMultiviewInstanceIndex {};

	const VkPhysicalDeviceMultiviewPropertiesKHX& vkHandle() const { return reinterpret_cast<const VkPhysicalDeviceMultiviewPropertiesKHX&>(*this); }
	VkPhysicalDeviceMultiviewPropertiesKHX& vkHandle() { return reinterpret_cast<VkPhysicalDeviceMultiviewPropertiesKHX&>(*this); }

	operator const VkPhysicalDeviceMultiviewPropertiesKHX&() const { return vkHandle(); };
	operator VkPhysicalDeviceMultiviewPropertiesKHX&() { return vkHandle(); };
};


struct ExternalImageFormatPropertiesNV {
	ImageFormatProperties imageFormatProperties {};
	ExternalMemoryFeatureFlagsNV externalMemoryFeatures {};
	ExternalMemoryHandleTypeFlagsNV exportFromImportedHandleTypes {};
	ExternalMemoryHandleTypeFlagsNV compatibleHandleTypes {};

	const VkExternalImageFormatPropertiesNV& vkHandle() const { return reinterpret_cast<const VkExternalImageFormatPropertiesNV&>(*this); }
	VkExternalImageFormatPropertiesNV& vkHandle() { return reinterpret_cast<VkExternalImageFormatPropertiesNV&>(*this); }

	operator const VkExternalImageFormatPropertiesNV&() const { return vkHandle(); };
	operator VkExternalImageFormatPropertiesNV&() { return vkHandle(); };
};


struct ExternalMemoryImageCreateInfoNV {
	StructureType sType {StructureType::externalMemoryImageCreateInfoNV};
	const void* pNext {};
	ExternalMemoryHandleTypeFlagsNV handleTypes {};

	ExternalMemoryImageCreateInfoNV(ExternalMemoryHandleTypeFlagsNV xhandleTypes = {}) : handleTypes(xhandleTypes) {}

	const VkExternalMemoryImageCreateInfoNV& vkHandle() const { return reinterpret_cast<const VkExternalMemoryImageCreateInfoNV&>(*this); }
	VkExternalMemoryImageCreateInfoNV& vkHandle() { return reinterpret_cast<VkExternalMemoryImageCreateInfoNV&>(*this); }

	operator const VkExternalMemoryImageCreateInfoNV&() const { return vkHandle(); };
	operator VkExternalMemoryImageCreateInfoNV&() { return vkHandle(); };
};

struct ExportMemoryAllocateInfoNV {
	StructureType sType {StructureType::exportMemoryAllocateInfoNV};
	const void* pNext {};
	ExternalMemoryHandleTypeFlagsNV handleTypes {};

	ExportMemoryAllocateInfoNV(ExternalMemoryHandleTypeFlagsNV xhandleTypes = {}) : handleTypes(xhandleTypes) {}

	const VkExportMemoryAllocateInfoNV& vkHandle() const { return reinterpret_cast<const VkExportMemoryAllocateInfoNV&>(*this); }
	VkExportMemoryAllocateInfoNV& vkHandle() { return reinterpret_cast<VkExportMemoryAllocateInfoNV&>(*this); }

	operator const VkExportMemoryAllocateInfoNV&() const { return vkHandle(); };
	operator VkExportMemoryAllocateInfoNV&() { return vkHandle(); };
};


#ifdef VK_USE_PLATFORM_WIN32_KHR

struct ImportMemoryWin32HandleInfoNV {
	StructureType sType {StructureType::importMemoryWin32HandleInfoNV};
	const void* pNext {};
	ExternalMemoryHandleTypeFlagsNV handleType {};
	HANDLE handle {};

	ImportMemoryWin32HandleInfoNV(ExternalMemoryHandleTypeFlagsNV xhandleType = {}, HANDLE xhandle = {}) : handleType(xhandleType), handle(xhandle) {}

	const VkImportMemoryWin32HandleInfoNV& vkHandle() const { return reinterpret_cast<const VkImportMemoryWin32HandleInfoNV&>(*this); }
	VkImportMemoryWin32HandleInfoNV& vkHandle() { return reinterpret_cast<VkImportMemoryWin32HandleInfoNV&>(*this); }

	operator const VkImportMemoryWin32HandleInfoNV&() const { return vkHandle(); };
	operator VkImportMemoryWin32HandleInfoNV&() { return vkHandle(); };
};

struct ExportMemoryWin32HandleInfoNV {
	StructureType sType {StructureType::exportMemoryWin32HandleInfoNV};
	const void* pNext {};
	const SECURITY_ATTRIBUTES* pAttributes {};
	DWORD dwAccess {};

	ExportMemoryWin32HandleInfoNV(const SECURITY_ATTRIBUTES* xpAttributes = {}, DWORD xdwAccess = {}) : pAttributes(xpAttributes), dwAccess(xdwAccess) {}

	const VkExportMemoryWin32HandleInfoNV& vkHandle() const { return reinterpret_cast<const VkExportMemoryWin32HandleInfoNV&>(*this); }
	VkExportMemoryWin32HandleInfoNV& vkHandle() { return reinterpret_cast<VkExportMemoryWin32HandleInfoNV&>(*this); }

	operator const VkExportMemoryWin32HandleInfoNV&() const { return vkHandle(); };
	operator VkExportMemoryWin32HandleInfoNV&() { return vkHandle(); };
};


#endif //VK_USE_PLATFORM_WIN32_KHR

#ifdef VK_USE_PLATFORM_WIN32_KHR

struct Win32KeyedMutexAcquireReleaseInfoNV {
	StructureType sType {StructureType::win32KeyedMutexAcquireReleaseInfoNV};
	const void* pNext {};
	uint32_t acquireCount {};
	const DeviceMemory* pAcquireSyncs {};
	const uint64_t* pAcquireKeys {};
	const uint32_t* pAcquireTimeoutMilliseconds {};
	uint32_t releaseCount {};
	const DeviceMemory* pReleaseSyncs {};
	const uint64_t* pReleaseKeys {};

	Win32KeyedMutexAcquireReleaseInfoNV(uint32_t xacquireCount = {}, const DeviceMemory* xpAcquireSyncs = {}, const uint64_t* xpAcquireKeys = {}, const uint32_t* xpAcquireTimeoutMilliseconds = {}, uint32_t xreleaseCount = {}, const DeviceMemory* xpReleaseSyncs = {}, const uint64_t* xpReleaseKeys = {}) : acquireCount(xacquireCount), pAcquireSyncs(xpAcquireSyncs), pAcquireKeys(xpAcquireKeys), pAcquireTimeoutMilliseconds(xpAcquireTimeoutMilliseconds), releaseCount(xreleaseCount), pReleaseSyncs(xpReleaseSyncs), pReleaseKeys(xpReleaseKeys) {}

	const VkWin32KeyedMutexAcquireReleaseInfoNV& vkHandle() const { return reinterpret_cast<const VkWin32KeyedMutexAcquireReleaseInfoNV&>(*this); }
	VkWin32KeyedMutexAcquireReleaseInfoNV& vkHandle() { return reinterpret_cast<VkWin32KeyedMutexAcquireReleaseInfoNV&>(*this); }

	operator const VkWin32KeyedMutexAcquireReleaseInfoNV&() const { return vkHandle(); };
	operator VkWin32KeyedMutexAcquireReleaseInfoNV&() { return vkHandle(); };
};


#endif //VK_USE_PLATFORM_WIN32_KHR

struct PhysicalDeviceFeatures2KHR {
	StructureType sType {StructureType::physicalDeviceFeatures2KHR};
	void* pNext {};
	PhysicalDeviceFeatures features {};

	PhysicalDeviceFeatures2KHR(PhysicalDeviceFeatures xfeatures = {}) : features(xfeatures) {}

	const VkPhysicalDeviceFeatures2KHR& vkHandle() const { return reinterpret_cast<const VkPhysicalDeviceFeatures2KHR&>(*this); }
	VkPhysicalDeviceFeatures2KHR& vkHandle() { return reinterpret_cast<VkPhysicalDeviceFeatures2KHR&>(*this); }

	operator const VkPhysicalDeviceFeatures2KHR&() const { return vkHandle(); };
	operator VkPhysicalDeviceFeatures2KHR&() { return vkHandle(); };
};

struct PhysicalDeviceProperties2KHR {
	StructureType sType {StructureType::physicalDeviceProperties2KHR};
	void* pNext {};
	PhysicalDeviceProperties properties {};

	const VkPhysicalDeviceProperties2KHR& vkHandle() const { return reinterpret_cast<const VkPhysicalDeviceProperties2KHR&>(*this); }
	VkPhysicalDeviceProperties2KHR& vkHandle() { return reinterpret_cast<VkPhysicalDeviceProperties2KHR&>(*this); }

	operator const VkPhysicalDeviceProperties2KHR&() const { return vkHandle(); };
	operator VkPhysicalDeviceProperties2KHR&() { return vkHandle(); };
};

struct FormatProperties2KHR {
	StructureType sType {StructureType::formatProperties2KHR};
	void* pNext {};
	FormatProperties formatProperties {};

	const VkFormatProperties2KHR& vkHandle() const { return reinterpret_cast<const VkFormatProperties2KHR&>(*this); }
	VkFormatProperties2KHR& vkHandle() { return reinterpret_cast<VkFormatProperties2KHR&>(*this); }

	operator const VkFormatProperties2KHR&() const { return vkHandle(); };
	operator VkFormatProperties2KHR&() { return vkHandle(); };
};

struct ImageFormatProperties2KHR {
	StructureType sType {StructureType::imageFormatProperties2KHR};
	void* pNext {};
	ImageFormatProperties imageFormatProperties {};

	const VkImageFormatProperties2KHR& vkHandle() const { return reinterpret_cast<const VkImageFormatProperties2KHR&>(*this); }
	VkImageFormatProperties2KHR& vkHandle() { return reinterpret_cast<VkImageFormatProperties2KHR&>(*this); }

	operator const VkImageFormatProperties2KHR&() const { return vkHandle(); };
	operator VkImageFormatProperties2KHR&() { return vkHandle(); };
};

struct PhysicalDeviceImageFormatInfo2KHR {
	StructureType sType {StructureType::physicalDeviceImageFormatInfo2KHR};
	const void* pNext {};
	Format format {};
	ImageType type {};
	ImageTiling tiling {};
	ImageUsageFlags usage {};
	ImageCreateFlags flags {};

	PhysicalDeviceImageFormatInfo2KHR(Format xformat = {}, ImageType xtype = {}, ImageTiling xtiling = {}, ImageUsageFlags xusage = {}, ImageCreateFlags xflags = {}) : format(xformat), type(xtype), tiling(xtiling), usage(xusage), flags(xflags) {}

	const VkPhysicalDeviceImageFormatInfo2KHR& vkHandle() const { return reinterpret_cast<const VkPhysicalDeviceImageFormatInfo2KHR&>(*this); }
	VkPhysicalDeviceImageFormatInfo2KHR& vkHandle() { return reinterpret_cast<VkPhysicalDeviceImageFormatInfo2KHR&>(*this); }

	operator const VkPhysicalDeviceImageFormatInfo2KHR&() const { return vkHandle(); };
	operator VkPhysicalDeviceImageFormatInfo2KHR&() { return vkHandle(); };
};

struct QueueFamilyProperties2KHR {
	StructureType sType {StructureType::queueFamilyProperties2KHR};
	void* pNext {};
	QueueFamilyProperties queueFamilyProperties {};

	const VkQueueFamilyProperties2KHR& vkHandle() const { return reinterpret_cast<const VkQueueFamilyProperties2KHR&>(*this); }
	VkQueueFamilyProperties2KHR& vkHandle() { return reinterpret_cast<VkQueueFamilyProperties2KHR&>(*this); }

	operator const VkQueueFamilyProperties2KHR&() const { return vkHandle(); };
	operator VkQueueFamilyProperties2KHR&() { return vkHandle(); };
};

struct PhysicalDeviceMemoryProperties2KHR {
	StructureType sType {StructureType::physicalDeviceMemoryProperties2KHR};
	void* pNext {};
	PhysicalDeviceMemoryProperties memoryProperties {};

	const VkPhysicalDeviceMemoryProperties2KHR& vkHandle() const { return reinterpret_cast<const VkPhysicalDeviceMemoryProperties2KHR&>(*this); }
	VkPhysicalDeviceMemoryProperties2KHR& vkHandle() { return reinterpret_cast<VkPhysicalDeviceMemoryProperties2KHR&>(*this); }

	operator const VkPhysicalDeviceMemoryProperties2KHR&() const { return vkHandle(); };
	operator VkPhysicalDeviceMemoryProperties2KHR&() { return vkHandle(); };
};

struct SparseImageFormatProperties2KHR {
	StructureType sType {StructureType::sparseImageFormatProperties2KHR};
	void* pNext {};
	SparseImageFormatProperties properties {};

	const VkSparseImageFormatProperties2KHR& vkHandle() const { return reinterpret_cast<const VkSparseImageFormatProperties2KHR&>(*this); }
	VkSparseImageFormatProperties2KHR& vkHandle() { return reinterpret_cast<VkSparseImageFormatProperties2KHR&>(*this); }

	operator const VkSparseImageFormatProperties2KHR&() const { return vkHandle(); };
	operator VkSparseImageFormatProperties2KHR&() { return vkHandle(); };
};

struct PhysicalDeviceSparseImageFormatInfo2KHR {
	StructureType sType {StructureType::physicalDeviceSparseImageFormatInfo2KHR};
	const void* pNext {};
	Format format {};
	ImageType type {};
	SampleCountBits samples {};
	ImageUsageFlags usage {};
	ImageTiling tiling {};

	PhysicalDeviceSparseImageFormatInfo2KHR(Format xformat = {}, ImageType xtype = {}, SampleCountBits xsamples = {}, ImageUsageFlags xusage = {}, ImageTiling xtiling = {}) : format(xformat), type(xtype), samples(xsamples), usage(xusage), tiling(xtiling) {}

	const VkPhysicalDeviceSparseImageFormatInfo2KHR& vkHandle() const { return reinterpret_cast<const VkPhysicalDeviceSparseImageFormatInfo2KHR&>(*this); }
	VkPhysicalDeviceSparseImageFormatInfo2KHR& vkHandle() { return reinterpret_cast<VkPhysicalDeviceSparseImageFormatInfo2KHR&>(*this); }

	operator const VkPhysicalDeviceSparseImageFormatInfo2KHR&() const { return vkHandle(); };
	operator VkPhysicalDeviceSparseImageFormatInfo2KHR&() { return vkHandle(); };
};


struct MemoryAllocateFlagsInfoKHX {
	StructureType sType {StructureType::memoryAllocateFlagsInfoKHX};
	const void* pNext {};
	MemoryAllocateFlagsKHX flags {};
	uint32_t deviceMask {};

	MemoryAllocateFlagsInfoKHX(MemoryAllocateFlagsKHX xflags = {}, uint32_t xdeviceMask = {}) : flags(xflags), deviceMask(xdeviceMask) {}

	const VkMemoryAllocateFlagsInfoKHX& vkHandle() const { return reinterpret_cast<const VkMemoryAllocateFlagsInfoKHX&>(*this); }
	VkMemoryAllocateFlagsInfoKHX& vkHandle() { return reinterpret_cast<VkMemoryAllocateFlagsInfoKHX&>(*this); }

	operator const VkMemoryAllocateFlagsInfoKHX&() const { return vkHandle(); };
	operator VkMemoryAllocateFlagsInfoKHX&() { return vkHandle(); };
};

struct BindBufferMemoryInfoKHX {
	StructureType sType {StructureType::bindBufferMemoryInfoKHX};
	const void* pNext {};
	Buffer buffer {};
	DeviceMemory memory {};
	DeviceSize memoryOffset {};
	uint32_t deviceIndexCount {};
	const uint32_t* pDeviceIndices {};

	BindBufferMemoryInfoKHX(Buffer xbuffer = {}, DeviceMemory xmemory = {}, DeviceSize xmemoryOffset = {}, uint32_t xdeviceIndexCount = {}, const uint32_t* xpDeviceIndices = {}) : buffer(xbuffer), memory(xmemory), memoryOffset(xmemoryOffset), deviceIndexCount(xdeviceIndexCount), pDeviceIndices(xpDeviceIndices) {}

	const VkBindBufferMemoryInfoKHX& vkHandle() const { return reinterpret_cast<const VkBindBufferMemoryInfoKHX&>(*this); }
	VkBindBufferMemoryInfoKHX& vkHandle() { return reinterpret_cast<VkBindBufferMemoryInfoKHX&>(*this); }

	operator const VkBindBufferMemoryInfoKHX&() const { return vkHandle(); };
	operator VkBindBufferMemoryInfoKHX&() { return vkHandle(); };
};

struct BindImageMemoryInfoKHX {
	StructureType sType {StructureType::bindImageMemoryInfoKHX};
	const void* pNext {};
	Image image {};
	DeviceMemory memory {};
	DeviceSize memoryOffset {};
	uint32_t deviceIndexCount {};
	const uint32_t* pDeviceIndices {};
	uint32_t SFRRectCount {};
	const Rect2D* pSFRRects {};

	BindImageMemoryInfoKHX(Image ximage = {}, DeviceMemory xmemory = {}, DeviceSize xmemoryOffset = {}, uint32_t xdeviceIndexCount = {}, const uint32_t* xpDeviceIndices = {}, uint32_t xSFRRectCount = {}, const Rect2D* xpSFRRects = {}) : image(ximage), memory(xmemory), memoryOffset(xmemoryOffset), deviceIndexCount(xdeviceIndexCount), pDeviceIndices(xpDeviceIndices), SFRRectCount(xSFRRectCount), pSFRRects(xpSFRRects) {}

	const VkBindImageMemoryInfoKHX& vkHandle() const { return reinterpret_cast<const VkBindImageMemoryInfoKHX&>(*this); }
	VkBindImageMemoryInfoKHX& vkHandle() { return reinterpret_cast<VkBindImageMemoryInfoKHX&>(*this); }

	operator const VkBindImageMemoryInfoKHX&() const { return vkHandle(); };
	operator VkBindImageMemoryInfoKHX&() { return vkHandle(); };
};

struct DeviceGroupRenderPassBeginInfoKHX {
	StructureType sType {StructureType::deviceGroupRenderPassBeginInfoKHX};
	const void* pNext {};
	uint32_t deviceMask {};
	uint32_t deviceRenderAreaCount {};
	const Rect2D* pDeviceRenderAreas {};

	DeviceGroupRenderPassBeginInfoKHX(uint32_t xdeviceMask = {}, uint32_t xdeviceRenderAreaCount = {}, const Rect2D* xpDeviceRenderAreas = {}) : deviceMask(xdeviceMask), deviceRenderAreaCount(xdeviceRenderAreaCount), pDeviceRenderAreas(xpDeviceRenderAreas) {}

	const VkDeviceGroupRenderPassBeginInfoKHX& vkHandle() const { return reinterpret_cast<const VkDeviceGroupRenderPassBeginInfoKHX&>(*this); }
	VkDeviceGroupRenderPassBeginInfoKHX& vkHandle() { return reinterpret_cast<VkDeviceGroupRenderPassBeginInfoKHX&>(*this); }

	operator const VkDeviceGroupRenderPassBeginInfoKHX&() const { return vkHandle(); };
	operator VkDeviceGroupRenderPassBeginInfoKHX&() { return vkHandle(); };
};

struct DeviceGroupCommandBufferBeginInfoKHX {
	StructureType sType {StructureType::deviceGroupCommandBufferBeginInfoKHX};
	const void* pNext {};
	uint32_t deviceMask {};

	DeviceGroupCommandBufferBeginInfoKHX(uint32_t xdeviceMask = {}) : deviceMask(xdeviceMask) {}

	const VkDeviceGroupCommandBufferBeginInfoKHX& vkHandle() const { return reinterpret_cast<const VkDeviceGroupCommandBufferBeginInfoKHX&>(*this); }
	VkDeviceGroupCommandBufferBeginInfoKHX& vkHandle() { return reinterpret_cast<VkDeviceGroupCommandBufferBeginInfoKHX&>(*this); }

	operator const VkDeviceGroupCommandBufferBeginInfoKHX&() const { return vkHandle(); };
	operator VkDeviceGroupCommandBufferBeginInfoKHX&() { return vkHandle(); };
};

struct DeviceGroupSubmitInfoKHX {
	StructureType sType {StructureType::deviceGroupSubmitInfoKHX};
	const void* pNext {};
	uint32_t waitSemaphoreCount {};
	const uint32_t* pWaitSemaphoreDeviceIndices {};
	uint32_t commandBufferCount {};
	const uint32_t* pCommandBufferDeviceMasks {};
	uint32_t signalSemaphoreCount {};
	const uint32_t* pSignalSemaphoreDeviceIndices {};

	DeviceGroupSubmitInfoKHX(uint32_t xwaitSemaphoreCount = {}, const uint32_t* xpWaitSemaphoreDeviceIndices = {}, uint32_t xcommandBufferCount = {}, const uint32_t* xpCommandBufferDeviceMasks = {}, uint32_t xsignalSemaphoreCount = {}, const uint32_t* xpSignalSemaphoreDeviceIndices = {}) : waitSemaphoreCount(xwaitSemaphoreCount), pWaitSemaphoreDeviceIndices(xpWaitSemaphoreDeviceIndices), commandBufferCount(xcommandBufferCount), pCommandBufferDeviceMasks(xpCommandBufferDeviceMasks), signalSemaphoreCount(xsignalSemaphoreCount), pSignalSemaphoreDeviceIndices(xpSignalSemaphoreDeviceIndices) {}

	const VkDeviceGroupSubmitInfoKHX& vkHandle() const { return reinterpret_cast<const VkDeviceGroupSubmitInfoKHX&>(*this); }
	VkDeviceGroupSubmitInfoKHX& vkHandle() { return reinterpret_cast<VkDeviceGroupSubmitInfoKHX&>(*this); }

	operator const VkDeviceGroupSubmitInfoKHX&() const { return vkHandle(); };
	operator VkDeviceGroupSubmitInfoKHX&() { return vkHandle(); };
};

struct DeviceGroupBindSparseInfoKHX {
	StructureType sType {StructureType::deviceGroupBindSparseInfoKHX};
	const void* pNext {};
	uint32_t resourceDeviceIndex {};
	uint32_t memoryDeviceIndex {};

	DeviceGroupBindSparseInfoKHX(uint32_t xresourceDeviceIndex = {}, uint32_t xmemoryDeviceIndex = {}) : resourceDeviceIndex(xresourceDeviceIndex), memoryDeviceIndex(xmemoryDeviceIndex) {}

	const VkDeviceGroupBindSparseInfoKHX& vkHandle() const { return reinterpret_cast<const VkDeviceGroupBindSparseInfoKHX&>(*this); }
	VkDeviceGroupBindSparseInfoKHX& vkHandle() { return reinterpret_cast<VkDeviceGroupBindSparseInfoKHX&>(*this); }

	operator const VkDeviceGroupBindSparseInfoKHX&() const { return vkHandle(); };
	operator VkDeviceGroupBindSparseInfoKHX&() { return vkHandle(); };
};

struct DeviceGroupPresentCapabilitiesKHX {
	StructureType sType {StructureType::deviceGroupPresentCapabilitiesKHX};
	const void* pNext {};
	std::array<uint32_t, maxDeviceGroupSizeKhx> presentMask {};
	DeviceGroupPresentModeFlagsKHX modes {};

	const VkDeviceGroupPresentCapabilitiesKHX& vkHandle() const { return reinterpret_cast<const VkDeviceGroupPresentCapabilitiesKHX&>(*this); }
	VkDeviceGroupPresentCapabilitiesKHX& vkHandle() { return reinterpret_cast<VkDeviceGroupPresentCapabilitiesKHX&>(*this); }

	operator const VkDeviceGroupPresentCapabilitiesKHX&() const { return vkHandle(); };
	operator VkDeviceGroupPresentCapabilitiesKHX&() { return vkHandle(); };
};

struct ImageSwapchainCreateInfoKHX {
	StructureType sType {StructureType::imageSwapchainCreateInfoKHX};
	const void* pNext {};
	SwapchainKHR swapchain {};

	ImageSwapchainCreateInfoKHX(SwapchainKHR xswapchain = {}) : swapchain(xswapchain) {}

	const VkImageSwapchainCreateInfoKHX& vkHandle() const { return reinterpret_cast<const VkImageSwapchainCreateInfoKHX&>(*this); }
	VkImageSwapchainCreateInfoKHX& vkHandle() { return reinterpret_cast<VkImageSwapchainCreateInfoKHX&>(*this); }

	operator const VkImageSwapchainCreateInfoKHX&() const { return vkHandle(); };
	operator VkImageSwapchainCreateInfoKHX&() { return vkHandle(); };
};

struct BindImageMemorySwapchainInfoKHX {
	StructureType sType {StructureType::bindImageMemorySwapchainInfoKHX};
	const void* pNext {};
	SwapchainKHR swapchain {};
	uint32_t imageIndex {};

	BindImageMemorySwapchainInfoKHX(SwapchainKHR xswapchain = {}, uint32_t ximageIndex = {}) : swapchain(xswapchain), imageIndex(ximageIndex) {}

	const VkBindImageMemorySwapchainInfoKHX& vkHandle() const { return reinterpret_cast<const VkBindImageMemorySwapchainInfoKHX&>(*this); }
	VkBindImageMemorySwapchainInfoKHX& vkHandle() { return reinterpret_cast<VkBindImageMemorySwapchainInfoKHX&>(*this); }

	operator const VkBindImageMemorySwapchainInfoKHX&() const { return vkHandle(); };
	operator VkBindImageMemorySwapchainInfoKHX&() { return vkHandle(); };
};

struct AcquireNextImageInfoKHX {
	StructureType sType {StructureType::acquireNextImageInfoKHX};
	const void* pNext {};
	SwapchainKHR swapchain {};
	uint64_t timeout {};
	Semaphore semaphore {};
	Fence fence {};
	uint32_t deviceMask {};

	AcquireNextImageInfoKHX(SwapchainKHR xswapchain = {}, uint64_t xtimeout = {}, Semaphore xsemaphore = {}, Fence xfence = {}, uint32_t xdeviceMask = {}) : swapchain(xswapchain), timeout(xtimeout), semaphore(xsemaphore), fence(xfence), deviceMask(xdeviceMask) {}

	const VkAcquireNextImageInfoKHX& vkHandle() const { return reinterpret_cast<const VkAcquireNextImageInfoKHX&>(*this); }
	VkAcquireNextImageInfoKHX& vkHandle() { return reinterpret_cast<VkAcquireNextImageInfoKHX&>(*this); }

	operator const VkAcquireNextImageInfoKHX&() const { return vkHandle(); };
	operator VkAcquireNextImageInfoKHX&() { return vkHandle(); };
};

struct DeviceGroupPresentInfoKHX {
	StructureType sType {StructureType::deviceGroupPresentInfoKHX};
	const void* pNext {};
	uint32_t swapchainCount {};
	const uint32_t* pDeviceMasks {};
	DeviceGroupPresentModeBitsKHX mode {};

	DeviceGroupPresentInfoKHX(uint32_t xswapchainCount = {}, const uint32_t* xpDeviceMasks = {}, DeviceGroupPresentModeBitsKHX xmode = {}) : swapchainCount(xswapchainCount), pDeviceMasks(xpDeviceMasks), mode(xmode) {}

	const VkDeviceGroupPresentInfoKHX& vkHandle() const { return reinterpret_cast<const VkDeviceGroupPresentInfoKHX&>(*this); }
	VkDeviceGroupPresentInfoKHX& vkHandle() { return reinterpret_cast<VkDeviceGroupPresentInfoKHX&>(*this); }

	operator const VkDeviceGroupPresentInfoKHX&() const { return vkHandle(); };
	operator VkDeviceGroupPresentInfoKHX&() { return vkHandle(); };
};

struct DeviceGroupSwapchainCreateInfoKHX {
	StructureType sType {StructureType::deviceGroupSwapchainCreateInfoKHX};
	const void* pNext {};
	DeviceGroupPresentModeFlagsKHX modes {};

	DeviceGroupSwapchainCreateInfoKHX(DeviceGroupPresentModeFlagsKHX xmodes = {}) : modes(xmodes) {}

	const VkDeviceGroupSwapchainCreateInfoKHX& vkHandle() const { return reinterpret_cast<const VkDeviceGroupSwapchainCreateInfoKHX&>(*this); }
	VkDeviceGroupSwapchainCreateInfoKHX& vkHandle() { return reinterpret_cast<VkDeviceGroupSwapchainCreateInfoKHX&>(*this); }

	operator const VkDeviceGroupSwapchainCreateInfoKHX&() const { return vkHandle(); };
	operator VkDeviceGroupSwapchainCreateInfoKHX&() { return vkHandle(); };
};


struct ValidationFlagsEXT {
	StructureType sType {StructureType::validationFlagsEXT};
	const void* pNext {};
	uint32_t disabledValidationCheckCount {};
	ValidationCheckEXT* pDisabledValidationChecks {};

	ValidationFlagsEXT(uint32_t xdisabledValidationCheckCount = {}, ValidationCheckEXT* xpDisabledValidationChecks = {}) : disabledValidationCheckCount(xdisabledValidationCheckCount), pDisabledValidationChecks(xpDisabledValidationChecks) {}

	const VkValidationFlagsEXT& vkHandle() const { return reinterpret_cast<const VkValidationFlagsEXT&>(*this); }
	VkValidationFlagsEXT& vkHandle() { return reinterpret_cast<VkValidationFlagsEXT&>(*this); }

	operator const VkValidationFlagsEXT&() const { return vkHandle(); };
	operator VkValidationFlagsEXT&() { return vkHandle(); };
};


#ifdef VK_USE_PLATFORM_VI_NN

struct ViSurfaceCreateInfoNN {
	StructureType sType {StructureType::viSurfaceCreateInfoNN};
	const void* pNext {};
	ViSurfaceCreateFlagsNN flags {};
	void* window {};

	ViSurfaceCreateInfoNN(ViSurfaceCreateFlagsNN xflags = {}, void* xwindow = {}) : flags(xflags), window(xwindow) {}

	const VkViSurfaceCreateInfoNN& vkHandle() const { return reinterpret_cast<const VkViSurfaceCreateInfoNN&>(*this); }
	VkViSurfaceCreateInfoNN& vkHandle() { return reinterpret_cast<VkViSurfaceCreateInfoNN&>(*this); }

	operator const VkViSurfaceCreateInfoNN&() const { return vkHandle(); };
	operator VkViSurfaceCreateInfoNN&() { return vkHandle(); };
};


#endif //VK_USE_PLATFORM_VI_NN

struct PhysicalDeviceGroupPropertiesKHX {
	StructureType sType {StructureType::physicalDeviceGroupPropertiesKHX};
	void* pNext {};
	uint32_t physicalDeviceCount {};
	std::array<PhysicalDevice, maxDeviceGroupSizeKhx> physicalDevices {};
	Bool32 subsetAllocation {};

	const VkPhysicalDeviceGroupPropertiesKHX& vkHandle() const { return reinterpret_cast<const VkPhysicalDeviceGroupPropertiesKHX&>(*this); }
	VkPhysicalDeviceGroupPropertiesKHX& vkHandle() { return reinterpret_cast<VkPhysicalDeviceGroupPropertiesKHX&>(*this); }

	operator const VkPhysicalDeviceGroupPropertiesKHX&() const { return vkHandle(); };
	operator VkPhysicalDeviceGroupPropertiesKHX&() { return vkHandle(); };
};

struct DeviceGroupDeviceCreateInfoKHX {
	StructureType sType {StructureType::deviceGroupDeviceCreateInfoKHX};
	const void* pNext {};
	uint32_t physicalDeviceCount {};
	const PhysicalDevice* pPhysicalDevices {};

	DeviceGroupDeviceCreateInfoKHX(uint32_t xphysicalDeviceCount = {}, const PhysicalDevice* xpPhysicalDevices = {}) : physicalDeviceCount(xphysicalDeviceCount), pPhysicalDevices(xpPhysicalDevices) {}

	const VkDeviceGroupDeviceCreateInfoKHX& vkHandle() const { return reinterpret_cast<const VkDeviceGroupDeviceCreateInfoKHX&>(*this); }
	VkDeviceGroupDeviceCreateInfoKHX& vkHandle() { return reinterpret_cast<VkDeviceGroupDeviceCreateInfoKHX&>(*this); }

	operator const VkDeviceGroupDeviceCreateInfoKHX&() const { return vkHandle(); };
	operator VkDeviceGroupDeviceCreateInfoKHX&() { return vkHandle(); };
};


struct ExternalMemoryPropertiesKHX {
	ExternalMemoryFeatureFlagsKHX externalMemoryFeatures {};
	ExternalMemoryHandleTypeFlagsKHX exportFromImportedHandleTypes {};
	ExternalMemoryHandleTypeFlagsKHX compatibleHandleTypes {};

	const VkExternalMemoryPropertiesKHX& vkHandle() const { return reinterpret_cast<const VkExternalMemoryPropertiesKHX&>(*this); }
	VkExternalMemoryPropertiesKHX& vkHandle() { return reinterpret_cast<VkExternalMemoryPropertiesKHX&>(*this); }

	operator const VkExternalMemoryPropertiesKHX&() const { return vkHandle(); };
	operator VkExternalMemoryPropertiesKHX&() { return vkHandle(); };
};

struct PhysicalDeviceExternalImageFormatInfoKHX {
	StructureType sType {StructureType::physicalDeviceExternalImageFormatInfoKHX};
	const void* pNext {};
	ExternalMemoryHandleTypeBitsKHX handleType {};

	PhysicalDeviceExternalImageFormatInfoKHX(ExternalMemoryHandleTypeBitsKHX xhandleType = {}) : handleType(xhandleType) {}

	const VkPhysicalDeviceExternalImageFormatInfoKHX& vkHandle() const { return reinterpret_cast<const VkPhysicalDeviceExternalImageFormatInfoKHX&>(*this); }
	VkPhysicalDeviceExternalImageFormatInfoKHX& vkHandle() { return reinterpret_cast<VkPhysicalDeviceExternalImageFormatInfoKHX&>(*this); }

	operator const VkPhysicalDeviceExternalImageFormatInfoKHX&() const { return vkHandle(); };
	operator VkPhysicalDeviceExternalImageFormatInfoKHX&() { return vkHandle(); };
};

struct ExternalImageFormatPropertiesKHX {
	StructureType sType {StructureType::externalImageFormatPropertiesKHX};
	void* pNext {};
	ExternalMemoryPropertiesKHX externalMemoryProperties {};

	const VkExternalImageFormatPropertiesKHX& vkHandle() const { return reinterpret_cast<const VkExternalImageFormatPropertiesKHX&>(*this); }
	VkExternalImageFormatPropertiesKHX& vkHandle() { return reinterpret_cast<VkExternalImageFormatPropertiesKHX&>(*this); }

	operator const VkExternalImageFormatPropertiesKHX&() const { return vkHandle(); };
	operator VkExternalImageFormatPropertiesKHX&() { return vkHandle(); };
};

struct PhysicalDeviceExternalBufferInfoKHX {
	StructureType sType {StructureType::physicalDeviceExternalBufferInfoKHX};
	const void* pNext {};
	BufferCreateFlags flags {};
	BufferUsageFlags usage {};
	ExternalMemoryHandleTypeBitsKHX handleType {};

	PhysicalDeviceExternalBufferInfoKHX(BufferCreateFlags xflags = {}, BufferUsageFlags xusage = {}, ExternalMemoryHandleTypeBitsKHX xhandleType = {}) : flags(xflags), usage(xusage), handleType(xhandleType) {}

	const VkPhysicalDeviceExternalBufferInfoKHX& vkHandle() const { return reinterpret_cast<const VkPhysicalDeviceExternalBufferInfoKHX&>(*this); }
	VkPhysicalDeviceExternalBufferInfoKHX& vkHandle() { return reinterpret_cast<VkPhysicalDeviceExternalBufferInfoKHX&>(*this); }

	operator const VkPhysicalDeviceExternalBufferInfoKHX&() const { return vkHandle(); };
	operator VkPhysicalDeviceExternalBufferInfoKHX&() { return vkHandle(); };
};

struct ExternalBufferPropertiesKHX {
	StructureType sType {StructureType::externalBufferPropertiesKHX};
	void* pNext {};
	ExternalMemoryPropertiesKHX externalMemoryProperties {};

	const VkExternalBufferPropertiesKHX& vkHandle() const { return reinterpret_cast<const VkExternalBufferPropertiesKHX&>(*this); }
	VkExternalBufferPropertiesKHX& vkHandle() { return reinterpret_cast<VkExternalBufferPropertiesKHX&>(*this); }

	operator const VkExternalBufferPropertiesKHX&() const { return vkHandle(); };
	operator VkExternalBufferPropertiesKHX&() { return vkHandle(); };
};

struct PhysicalDeviceIDPropertiesKHX {
	StructureType sType {StructureType::physicalDeviceIDPropertiesKHX};
	void* pNext {};
	std::array<uint8_t, uuidSize> deviceUUID {};
	std::array<uint8_t, uuidSize> driverUUID {};
	std::array<uint8_t, luidSizeKhx> deviceLUID {};
	Bool32 deviceLUIDValid {};

	const VkPhysicalDeviceIDPropertiesKHX& vkHandle() const { return reinterpret_cast<const VkPhysicalDeviceIDPropertiesKHX&>(*this); }
	VkPhysicalDeviceIDPropertiesKHX& vkHandle() { return reinterpret_cast<VkPhysicalDeviceIDPropertiesKHX&>(*this); }

	operator const VkPhysicalDeviceIDPropertiesKHX&() const { return vkHandle(); };
	operator VkPhysicalDeviceIDPropertiesKHX&() { return vkHandle(); };
};


struct ExternalMemoryImageCreateInfoKHX {
	StructureType sType {StructureType::externalMemoryImageCreateInfoKHX};
	const void* pNext {};
	ExternalMemoryHandleTypeFlagsKHX handleTypes {};

	ExternalMemoryImageCreateInfoKHX(ExternalMemoryHandleTypeFlagsKHX xhandleTypes = {}) : handleTypes(xhandleTypes) {}

	const VkExternalMemoryImageCreateInfoKHX& vkHandle() const { return reinterpret_cast<const VkExternalMemoryImageCreateInfoKHX&>(*this); }
	VkExternalMemoryImageCreateInfoKHX& vkHandle() { return reinterpret_cast<VkExternalMemoryImageCreateInfoKHX&>(*this); }

	operator const VkExternalMemoryImageCreateInfoKHX&() const { return vkHandle(); };
	operator VkExternalMemoryImageCreateInfoKHX&() { return vkHandle(); };
};

struct ExternalMemoryBufferCreateInfoKHX {
	StructureType sType {StructureType::externalMemoryBufferCreateInfoKHX};
	const void* pNext {};
	ExternalMemoryHandleTypeFlagsKHX handleTypes {};

	ExternalMemoryBufferCreateInfoKHX(ExternalMemoryHandleTypeFlagsKHX xhandleTypes = {}) : handleTypes(xhandleTypes) {}

	const VkExternalMemoryBufferCreateInfoKHX& vkHandle() const { return reinterpret_cast<const VkExternalMemoryBufferCreateInfoKHX&>(*this); }
	VkExternalMemoryBufferCreateInfoKHX& vkHandle() { return reinterpret_cast<VkExternalMemoryBufferCreateInfoKHX&>(*this); }

	operator const VkExternalMemoryBufferCreateInfoKHX&() const { return vkHandle(); };
	operator VkExternalMemoryBufferCreateInfoKHX&() { return vkHandle(); };
};

struct ExportMemoryAllocateInfoKHX {
	StructureType sType {StructureType::exportMemoryAllocateInfoKHX};
	const void* pNext {};
	ExternalMemoryHandleTypeFlagsKHX handleTypes {};

	ExportMemoryAllocateInfoKHX(ExternalMemoryHandleTypeFlagsKHX xhandleTypes = {}) : handleTypes(xhandleTypes) {}

	const VkExportMemoryAllocateInfoKHX& vkHandle() const { return reinterpret_cast<const VkExportMemoryAllocateInfoKHX&>(*this); }
	VkExportMemoryAllocateInfoKHX& vkHandle() { return reinterpret_cast<VkExportMemoryAllocateInfoKHX&>(*this); }

	operator const VkExportMemoryAllocateInfoKHX&() const { return vkHandle(); };
	operator VkExportMemoryAllocateInfoKHX&() { return vkHandle(); };
};


#ifdef VK_USE_PLATFORM_WIN32_KHX

struct ImportMemoryWin32HandleInfoKHX {
	StructureType sType {StructureType::importMemoryWin32HandleInfoKHX};
	const void* pNext {};
	ExternalMemoryHandleTypeBitsKHX handleType {};
	HANDLE handle {};

	ImportMemoryWin32HandleInfoKHX(ExternalMemoryHandleTypeBitsKHX xhandleType = {}, HANDLE xhandle = {}) : handleType(xhandleType), handle(xhandle) {}

	const VkImportMemoryWin32HandleInfoKHX& vkHandle() const { return reinterpret_cast<const VkImportMemoryWin32HandleInfoKHX&>(*this); }
	VkImportMemoryWin32HandleInfoKHX& vkHandle() { return reinterpret_cast<VkImportMemoryWin32HandleInfoKHX&>(*this); }

	operator const VkImportMemoryWin32HandleInfoKHX&() const { return vkHandle(); };
	operator VkImportMemoryWin32HandleInfoKHX&() { return vkHandle(); };
};

struct ExportMemoryWin32HandleInfoKHX {
	StructureType sType {StructureType::exportMemoryWin32HandleInfoKHX};
	const void* pNext {};
	const SECURITY_ATTRIBUTES* pAttributes {};
	DWORD dwAccess {};
	LPCWSTR name {};

	ExportMemoryWin32HandleInfoKHX(const SECURITY_ATTRIBUTES* xpAttributes = {}, DWORD xdwAccess = {}, LPCWSTR xname = {}) : pAttributes(xpAttributes), dwAccess(xdwAccess), name(xname) {}

	const VkExportMemoryWin32HandleInfoKHX& vkHandle() const { return reinterpret_cast<const VkExportMemoryWin32HandleInfoKHX&>(*this); }
	VkExportMemoryWin32HandleInfoKHX& vkHandle() { return reinterpret_cast<VkExportMemoryWin32HandleInfoKHX&>(*this); }

	operator const VkExportMemoryWin32HandleInfoKHX&() const { return vkHandle(); };
	operator VkExportMemoryWin32HandleInfoKHX&() { return vkHandle(); };
};

struct MemoryWin32HandlePropertiesKHX {
	StructureType sType {StructureType::memoryWin32HandlePropertiesKHX};
	void* pNext {};
	uint32_t memoryTypeBits {};

	const VkMemoryWin32HandlePropertiesKHX& vkHandle() const { return reinterpret_cast<const VkMemoryWin32HandlePropertiesKHX&>(*this); }
	VkMemoryWin32HandlePropertiesKHX& vkHandle() { return reinterpret_cast<VkMemoryWin32HandlePropertiesKHX&>(*this); }

	operator const VkMemoryWin32HandlePropertiesKHX&() const { return vkHandle(); };
	operator VkMemoryWin32HandlePropertiesKHX&() { return vkHandle(); };
};


#endif //VK_USE_PLATFORM_WIN32_KHX

struct ImportMemoryFdInfoKHX {
	StructureType sType {StructureType::importMemoryFdInfoKHX};
	const void* pNext {};
	ExternalMemoryHandleTypeBitsKHX handleType {};
	int fd {};

	ImportMemoryFdInfoKHX(ExternalMemoryHandleTypeBitsKHX xhandleType = {}, int xfd = {}) : handleType(xhandleType), fd(xfd) {}

	const VkImportMemoryFdInfoKHX& vkHandle() const { return reinterpret_cast<const VkImportMemoryFdInfoKHX&>(*this); }
	VkImportMemoryFdInfoKHX& vkHandle() { return reinterpret_cast<VkImportMemoryFdInfoKHX&>(*this); }

	operator const VkImportMemoryFdInfoKHX&() const { return vkHandle(); };
	operator VkImportMemoryFdInfoKHX&() { return vkHandle(); };
};

struct MemoryFdPropertiesKHX {
	StructureType sType {StructureType::memoryFdPropertiesKHX};
	void* pNext {};
	uint32_t memoryTypeBits {};

	const VkMemoryFdPropertiesKHX& vkHandle() const { return reinterpret_cast<const VkMemoryFdPropertiesKHX&>(*this); }
	VkMemoryFdPropertiesKHX& vkHandle() { return reinterpret_cast<VkMemoryFdPropertiesKHX&>(*this); }

	operator const VkMemoryFdPropertiesKHX&() const { return vkHandle(); };
	operator VkMemoryFdPropertiesKHX&() { return vkHandle(); };
};


#ifdef VK_USE_PLATFORM_WIN32_KHR

struct Win32KeyedMutexAcquireReleaseInfoKHX {
	StructureType sType {StructureType::win32KeyedMutexAcquireReleaseInfoKHX};
	const void* pNext {};
	uint32_t acquireCount {};
	const DeviceMemory* pAcquireSyncs {};
	const uint64_t* pAcquireKeys {};
	const uint32_t* pAcquireTimeouts {};
	uint32_t releaseCount {};
	const DeviceMemory* pReleaseSyncs {};
	const uint64_t* pReleaseKeys {};

	Win32KeyedMutexAcquireReleaseInfoKHX(uint32_t xacquireCount = {}, const DeviceMemory* xpAcquireSyncs = {}, const uint64_t* xpAcquireKeys = {}, const uint32_t* xpAcquireTimeouts = {}, uint32_t xreleaseCount = {}, const DeviceMemory* xpReleaseSyncs = {}, const uint64_t* xpReleaseKeys = {}) : acquireCount(xacquireCount), pAcquireSyncs(xpAcquireSyncs), pAcquireKeys(xpAcquireKeys), pAcquireTimeouts(xpAcquireTimeouts), releaseCount(xreleaseCount), pReleaseSyncs(xpReleaseSyncs), pReleaseKeys(xpReleaseKeys) {}

	const VkWin32KeyedMutexAcquireReleaseInfoKHX& vkHandle() const { return reinterpret_cast<const VkWin32KeyedMutexAcquireReleaseInfoKHX&>(*this); }
	VkWin32KeyedMutexAcquireReleaseInfoKHX& vkHandle() { return reinterpret_cast<VkWin32KeyedMutexAcquireReleaseInfoKHX&>(*this); }

	operator const VkWin32KeyedMutexAcquireReleaseInfoKHX&() const { return vkHandle(); };
	operator VkWin32KeyedMutexAcquireReleaseInfoKHX&() { return vkHandle(); };
};


#endif //VK_USE_PLATFORM_WIN32_KHR

struct PhysicalDeviceExternalSemaphoreInfoKHX {
	StructureType sType {StructureType::physicalDeviceExternalSemaphoreInfoKHX};
	const void* pNext {};
	ExternalSemaphoreHandleTypeBitsKHX handleType {};

	PhysicalDeviceExternalSemaphoreInfoKHX(ExternalSemaphoreHandleTypeBitsKHX xhandleType = {}) : handleType(xhandleType) {}

	const VkPhysicalDeviceExternalSemaphoreInfoKHX& vkHandle() const { return reinterpret_cast<const VkPhysicalDeviceExternalSemaphoreInfoKHX&>(*this); }
	VkPhysicalDeviceExternalSemaphoreInfoKHX& vkHandle() { return reinterpret_cast<VkPhysicalDeviceExternalSemaphoreInfoKHX&>(*this); }

	operator const VkPhysicalDeviceExternalSemaphoreInfoKHX&() const { return vkHandle(); };
	operator VkPhysicalDeviceExternalSemaphoreInfoKHX&() { return vkHandle(); };
};

struct ExternalSemaphorePropertiesKHX {
	StructureType sType {StructureType::externalSemaphorePropertiesKHX};
	void* pNext {};
	ExternalSemaphoreHandleTypeFlagsKHX exportFromImportedHandleTypes {};
	ExternalSemaphoreHandleTypeFlagsKHX compatibleHandleTypes {};
	ExternalSemaphoreFeatureFlagsKHX externalSemaphoreFeatures {};

	const VkExternalSemaphorePropertiesKHX& vkHandle() const { return reinterpret_cast<const VkExternalSemaphorePropertiesKHX&>(*this); }
	VkExternalSemaphorePropertiesKHX& vkHandle() { return reinterpret_cast<VkExternalSemaphorePropertiesKHX&>(*this); }

	operator const VkExternalSemaphorePropertiesKHX&() const { return vkHandle(); };
	operator VkExternalSemaphorePropertiesKHX&() { return vkHandle(); };
};


struct ExportSemaphoreCreateInfoKHX {
	StructureType sType {StructureType::exportSemaphoreCreateInfoKHX};
	const void* pNext {};
	ExternalSemaphoreHandleTypeFlagsKHX handleTypes {};

	ExportSemaphoreCreateInfoKHX(ExternalSemaphoreHandleTypeFlagsKHX xhandleTypes = {}) : handleTypes(xhandleTypes) {}

	const VkExportSemaphoreCreateInfoKHX& vkHandle() const { return reinterpret_cast<const VkExportSemaphoreCreateInfoKHX&>(*this); }
	VkExportSemaphoreCreateInfoKHX& vkHandle() { return reinterpret_cast<VkExportSemaphoreCreateInfoKHX&>(*this); }

	operator const VkExportSemaphoreCreateInfoKHX&() const { return vkHandle(); };
	operator VkExportSemaphoreCreateInfoKHX&() { return vkHandle(); };
};


#ifdef VK_USE_PLATFORM_WIN32_KHX

struct ImportSemaphoreWin32HandleInfoKHX {
	StructureType sType {StructureType::importSemaphoreWin32HandleInfoKHX};
	const void* pNext {};
	Semaphore semaphore {};
	ExternalSemaphoreHandleTypeFlagsKHX handleType {};
	HANDLE handle {};

	ImportSemaphoreWin32HandleInfoKHX(Semaphore xsemaphore = {}, ExternalSemaphoreHandleTypeFlagsKHX xhandleType = {}, HANDLE xhandle = {}) : semaphore(xsemaphore), handleType(xhandleType), handle(xhandle) {}

	const VkImportSemaphoreWin32HandleInfoKHX& vkHandle() const { return reinterpret_cast<const VkImportSemaphoreWin32HandleInfoKHX&>(*this); }
	VkImportSemaphoreWin32HandleInfoKHX& vkHandle() { return reinterpret_cast<VkImportSemaphoreWin32HandleInfoKHX&>(*this); }

	operator const VkImportSemaphoreWin32HandleInfoKHX&() const { return vkHandle(); };
	operator VkImportSemaphoreWin32HandleInfoKHX&() { return vkHandle(); };
};

struct ExportSemaphoreWin32HandleInfoKHX {
	StructureType sType {StructureType::exportSemaphoreWin32HandleInfoKHX};
	const void* pNext {};
	const SECURITY_ATTRIBUTES* pAttributes {};
	DWORD dwAccess {};
	LPCWSTR name {};

	ExportSemaphoreWin32HandleInfoKHX(const SECURITY_ATTRIBUTES* xpAttributes = {}, DWORD xdwAccess = {}, LPCWSTR xname = {}) : pAttributes(xpAttributes), dwAccess(xdwAccess), name(xname) {}

	const VkExportSemaphoreWin32HandleInfoKHX& vkHandle() const { return reinterpret_cast<const VkExportSemaphoreWin32HandleInfoKHX&>(*this); }
	VkExportSemaphoreWin32HandleInfoKHX& vkHandle() { return reinterpret_cast<VkExportSemaphoreWin32HandleInfoKHX&>(*this); }

	operator const VkExportSemaphoreWin32HandleInfoKHX&() const { return vkHandle(); };
	operator VkExportSemaphoreWin32HandleInfoKHX&() { return vkHandle(); };
};

struct D3D12FenceSubmitInfoKHX {
	StructureType sType {StructureType::d3D12FenceSubmitInfoKHX};
	const void* pNext {};
	uint32_t waitSemaphoreValuesCount {};
	const uint64_t* pWaitSemaphoreValues {};
	uint32_t signalSemaphoreValuesCount {};
	const uint64_t* pSignalSemaphoreValues {};

	D3D12FenceSubmitInfoKHX(uint32_t xwaitSemaphoreValuesCount = {}, const uint64_t* xpWaitSemaphoreValues = {}, uint32_t xsignalSemaphoreValuesCount = {}, const uint64_t* xpSignalSemaphoreValues = {}) : waitSemaphoreValuesCount(xwaitSemaphoreValuesCount), pWaitSemaphoreValues(xpWaitSemaphoreValues), signalSemaphoreValuesCount(xsignalSemaphoreValuesCount), pSignalSemaphoreValues(xpSignalSemaphoreValues) {}

	const VkD3D12FenceSubmitInfoKHX& vkHandle() const { return reinterpret_cast<const VkD3D12FenceSubmitInfoKHX&>(*this); }
	VkD3D12FenceSubmitInfoKHX& vkHandle() { return reinterpret_cast<VkD3D12FenceSubmitInfoKHX&>(*this); }

	operator const VkD3D12FenceSubmitInfoKHX&() const { return vkHandle(); };
	operator VkD3D12FenceSubmitInfoKHX&() { return vkHandle(); };
};


#endif //VK_USE_PLATFORM_WIN32_KHX

struct ImportSemaphoreFdInfoKHX {
	StructureType sType {StructureType::importSemaphoreFdInfoKHX};
	const void* pNext {};
	Semaphore semaphore {};
	ExternalSemaphoreHandleTypeBitsKHX handleType {};
	int fd {};

	ImportSemaphoreFdInfoKHX(Semaphore xsemaphore = {}, ExternalSemaphoreHandleTypeBitsKHX xhandleType = {}, int xfd = {}) : semaphore(xsemaphore), handleType(xhandleType), fd(xfd) {}

	const VkImportSemaphoreFdInfoKHX& vkHandle() const { return reinterpret_cast<const VkImportSemaphoreFdInfoKHX&>(*this); }
	VkImportSemaphoreFdInfoKHX& vkHandle() { return reinterpret_cast<VkImportSemaphoreFdInfoKHX&>(*this); }

	operator const VkImportSemaphoreFdInfoKHX&() const { return vkHandle(); };
	operator VkImportSemaphoreFdInfoKHX&() { return vkHandle(); };
};


struct PhysicalDevicePushDescriptorPropertiesKHR {
	StructureType sType {StructureType::physicalDevicePushDescriptorPropertiesKHR};
	void* pNext {};
	uint32_t maxPushDescriptors {};

	PhysicalDevicePushDescriptorPropertiesKHR(uint32_t xmaxPushDescriptors = {}) : maxPushDescriptors(xmaxPushDescriptors) {}

	const VkPhysicalDevicePushDescriptorPropertiesKHR& vkHandle() const { return reinterpret_cast<const VkPhysicalDevicePushDescriptorPropertiesKHR&>(*this); }
	VkPhysicalDevicePushDescriptorPropertiesKHR& vkHandle() { return reinterpret_cast<VkPhysicalDevicePushDescriptorPropertiesKHR&>(*this); }

	operator const VkPhysicalDevicePushDescriptorPropertiesKHR&() const { return vkHandle(); };
	operator VkPhysicalDevicePushDescriptorPropertiesKHR&() { return vkHandle(); };
};


struct RectLayerKHR {
	Offset2D offset {};
	Extent2D extent {};
	uint32_t layer {};

	const VkRectLayerKHR& vkHandle() const { return reinterpret_cast<const VkRectLayerKHR&>(*this); }
	VkRectLayerKHR& vkHandle() { return reinterpret_cast<VkRectLayerKHR&>(*this); }

	operator const VkRectLayerKHR&() const { return vkHandle(); };
	operator VkRectLayerKHR&() { return vkHandle(); };
};

struct PresentRegionKHR {
	uint32_t rectangleCount {};
	const RectLayerKHR* pRectangles {};

	const VkPresentRegionKHR& vkHandle() const { return reinterpret_cast<const VkPresentRegionKHR&>(*this); }
	VkPresentRegionKHR& vkHandle() { return reinterpret_cast<VkPresentRegionKHR&>(*this); }

	operator const VkPresentRegionKHR&() const { return vkHandle(); };
	operator VkPresentRegionKHR&() { return vkHandle(); };
};

struct PresentRegionsKHR {
	StructureType sType {StructureType::presentRegionsKHR};
	const void* pNext {};
	uint32_t swapchainCount {};
	const PresentRegionKHR* pRegions {};

	PresentRegionsKHR(uint32_t xswapchainCount = {}, const PresentRegionKHR* xpRegions = {}) : swapchainCount(xswapchainCount), pRegions(xpRegions) {}

	const VkPresentRegionsKHR& vkHandle() const { return reinterpret_cast<const VkPresentRegionsKHR&>(*this); }
	VkPresentRegionsKHR& vkHandle() { return reinterpret_cast<VkPresentRegionsKHR&>(*this); }

	operator const VkPresentRegionsKHR&() const { return vkHandle(); };
	operator VkPresentRegionsKHR&() { return vkHandle(); };
};


struct DescriptorUpdateTemplateEntryKHR {
	uint32_t dstBinding {};
	uint32_t dstArrayElement {};
	uint32_t descriptorCount {};
	DescriptorType descriptorType {};
	size_t offset {};
	size_t stride {};

	const VkDescriptorUpdateTemplateEntryKHR& vkHandle() const { return reinterpret_cast<const VkDescriptorUpdateTemplateEntryKHR&>(*this); }
	VkDescriptorUpdateTemplateEntryKHR& vkHandle() { return reinterpret_cast<VkDescriptorUpdateTemplateEntryKHR&>(*this); }

	operator const VkDescriptorUpdateTemplateEntryKHR&() const { return vkHandle(); };
	operator VkDescriptorUpdateTemplateEntryKHR&() { return vkHandle(); };
};

struct DescriptorUpdateTemplateCreateInfoKHR {
	StructureType sType {StructureType::descriptorUpdateTemplateCreateInfoKHR};
	void* pNext {};
	DescriptorUpdateTemplateCreateFlagsKHR flags {};
	uint32_t descriptorUpdateEntryCount {};
	const DescriptorUpdateTemplateEntryKHR* pDescriptorUpdateEntries {};
	DescriptorUpdateTemplateTypeKHR templateType {};
	DescriptorSetLayout descriptorSetLayout {};
	PipelineBindPoint pipelineBindPoint {};
	PipelineLayout pipelineLayout {};
	uint32_t set {};

	DescriptorUpdateTemplateCreateInfoKHR(DescriptorUpdateTemplateCreateFlagsKHR xflags = {}, uint32_t xdescriptorUpdateEntryCount = {}, const DescriptorUpdateTemplateEntryKHR* xpDescriptorUpdateEntries = {}, DescriptorUpdateTemplateTypeKHR xtemplateType = {}, DescriptorSetLayout xdescriptorSetLayout = {}, PipelineBindPoint xpipelineBindPoint = {}, PipelineLayout xpipelineLayout = {}, uint32_t xset = {}) : flags(xflags), descriptorUpdateEntryCount(xdescriptorUpdateEntryCount), pDescriptorUpdateEntries(xpDescriptorUpdateEntries), templateType(xtemplateType), descriptorSetLayout(xdescriptorSetLayout), pipelineBindPoint(xpipelineBindPoint), pipelineLayout(xpipelineLayout), set(xset) {}

	const VkDescriptorUpdateTemplateCreateInfoKHR& vkHandle() const { return reinterpret_cast<const VkDescriptorUpdateTemplateCreateInfoKHR&>(*this); }
	VkDescriptorUpdateTemplateCreateInfoKHR& vkHandle() { return reinterpret_cast<VkDescriptorUpdateTemplateCreateInfoKHR&>(*this); }

	operator const VkDescriptorUpdateTemplateCreateInfoKHR&() const { return vkHandle(); };
	operator VkDescriptorUpdateTemplateCreateInfoKHR&() { return vkHandle(); };
};


struct DeviceGeneratedCommandsFeaturesNVX {
	StructureType sType {StructureType::deviceGeneratedCommandsFeaturesNVX};
	const void* pNext {};
	Bool32 computeBindingPointSupport {};

	DeviceGeneratedCommandsFeaturesNVX(Bool32 xcomputeBindingPointSupport = {}) : computeBindingPointSupport(xcomputeBindingPointSupport) {}

	const VkDeviceGeneratedCommandsFeaturesNVX& vkHandle() const { return reinterpret_cast<const VkDeviceGeneratedCommandsFeaturesNVX&>(*this); }
	VkDeviceGeneratedCommandsFeaturesNVX& vkHandle() { return reinterpret_cast<VkDeviceGeneratedCommandsFeaturesNVX&>(*this); }

	operator const VkDeviceGeneratedCommandsFeaturesNVX&() const { return vkHandle(); };
	operator VkDeviceGeneratedCommandsFeaturesNVX&() { return vkHandle(); };
};

struct DeviceGeneratedCommandsLimitsNVX {
	StructureType sType {StructureType::deviceGeneratedCommandsLimitsNVX};
	const void* pNext {};
	uint32_t maxIndirectCommandsLayoutTokenCount {};
	uint32_t maxObjectEntryCounts {};
	uint32_t minSequenceCountBufferOffsetAlignment {};
	uint32_t minSequenceIndexBufferOffsetAlignment {};
	uint32_t minCommandsTokenBufferOffsetAlignment {};

	DeviceGeneratedCommandsLimitsNVX(uint32_t xmaxIndirectCommandsLayoutTokenCount = {}, uint32_t xmaxObjectEntryCounts = {}, uint32_t xminSequenceCountBufferOffsetAlignment = {}, uint32_t xminSequenceIndexBufferOffsetAlignment = {}, uint32_t xminCommandsTokenBufferOffsetAlignment = {}) : maxIndirectCommandsLayoutTokenCount(xmaxIndirectCommandsLayoutTokenCount), maxObjectEntryCounts(xmaxObjectEntryCounts), minSequenceCountBufferOffsetAlignment(xminSequenceCountBufferOffsetAlignment), minSequenceIndexBufferOffsetAlignment(xminSequenceIndexBufferOffsetAlignment), minCommandsTokenBufferOffsetAlignment(xminCommandsTokenBufferOffsetAlignment) {}

	const VkDeviceGeneratedCommandsLimitsNVX& vkHandle() const { return reinterpret_cast<const VkDeviceGeneratedCommandsLimitsNVX&>(*this); }
	VkDeviceGeneratedCommandsLimitsNVX& vkHandle() { return reinterpret_cast<VkDeviceGeneratedCommandsLimitsNVX&>(*this); }

	operator const VkDeviceGeneratedCommandsLimitsNVX&() const { return vkHandle(); };
	operator VkDeviceGeneratedCommandsLimitsNVX&() { return vkHandle(); };
};

struct IndirectCommandsTokenNVX {
	IndirectCommandsTokenTypeNVX tokenType {};
	Buffer buffer {};
	DeviceSize offset {};

	const VkIndirectCommandsTokenNVX& vkHandle() const { return reinterpret_cast<const VkIndirectCommandsTokenNVX&>(*this); }
	VkIndirectCommandsTokenNVX& vkHandle() { return reinterpret_cast<VkIndirectCommandsTokenNVX&>(*this); }

	operator const VkIndirectCommandsTokenNVX&() const { return vkHandle(); };
	operator VkIndirectCommandsTokenNVX&() { return vkHandle(); };
};

struct IndirectCommandsLayoutTokenNVX {
	IndirectCommandsTokenTypeNVX tokenType {};
	uint32_t bindingUnit {};
	uint32_t dynamicCount {};
	uint32_t divisor {};

	const VkIndirectCommandsLayoutTokenNVX& vkHandle() const { return reinterpret_cast<const VkIndirectCommandsLayoutTokenNVX&>(*this); }
	VkIndirectCommandsLayoutTokenNVX& vkHandle() { return reinterpret_cast<VkIndirectCommandsLayoutTokenNVX&>(*this); }

	operator const VkIndirectCommandsLayoutTokenNVX&() const { return vkHandle(); };
	operator VkIndirectCommandsLayoutTokenNVX&() { return vkHandle(); };
};

struct IndirectCommandsLayoutCreateInfoNVX {
	StructureType sType {StructureType::indirectCommandsLayoutCreateInfoNVX};
	const void* pNext {};
	PipelineBindPoint pipelineBindPoint {};
	IndirectCommandsLayoutUsageFlagsNVX flags {};
	uint32_t tokenCount {};
	const IndirectCommandsLayoutTokenNVX* pTokens {};

	IndirectCommandsLayoutCreateInfoNVX(PipelineBindPoint xpipelineBindPoint = {}, IndirectCommandsLayoutUsageFlagsNVX xflags = {}, uint32_t xtokenCount = {}, const IndirectCommandsLayoutTokenNVX* xpTokens = {}) : pipelineBindPoint(xpipelineBindPoint), flags(xflags), tokenCount(xtokenCount), pTokens(xpTokens) {}

	const VkIndirectCommandsLayoutCreateInfoNVX& vkHandle() const { return reinterpret_cast<const VkIndirectCommandsLayoutCreateInfoNVX&>(*this); }
	VkIndirectCommandsLayoutCreateInfoNVX& vkHandle() { return reinterpret_cast<VkIndirectCommandsLayoutCreateInfoNVX&>(*this); }

	operator const VkIndirectCommandsLayoutCreateInfoNVX&() const { return vkHandle(); };
	operator VkIndirectCommandsLayoutCreateInfoNVX&() { return vkHandle(); };
};

struct CmdProcessCommandsInfoNVX {
	StructureType sType {StructureType::cmdProcessCommandsInfoNVX};
	const void* pNext {};
	ObjectTableNVX objectTable {};
	IndirectCommandsLayoutNVX indirectCommandsLayout {};
	uint32_t indirectCommandsTokenCount {};
	const IndirectCommandsTokenNVX* pIndirectCommandsTokens {};
	uint32_t maxSequencesCount {};
	CommandBuffer targetCommandBuffer {};
	Buffer sequencesCountBuffer {};
	DeviceSize sequencesCountOffset {};
	Buffer sequencesIndexBuffer {};
	DeviceSize sequencesIndexOffset {};

	CmdProcessCommandsInfoNVX(ObjectTableNVX xobjectTable = {}, IndirectCommandsLayoutNVX xindirectCommandsLayout = {}, uint32_t xindirectCommandsTokenCount = {}, const IndirectCommandsTokenNVX* xpIndirectCommandsTokens = {}, uint32_t xmaxSequencesCount = {}, CommandBuffer xtargetCommandBuffer = {}, Buffer xsequencesCountBuffer = {}, DeviceSize xsequencesCountOffset = {}, Buffer xsequencesIndexBuffer = {}, DeviceSize xsequencesIndexOffset = {}) : objectTable(xobjectTable), indirectCommandsLayout(xindirectCommandsLayout), indirectCommandsTokenCount(xindirectCommandsTokenCount), pIndirectCommandsTokens(xpIndirectCommandsTokens), maxSequencesCount(xmaxSequencesCount), targetCommandBuffer(xtargetCommandBuffer), sequencesCountBuffer(xsequencesCountBuffer), sequencesCountOffset(xsequencesCountOffset), sequencesIndexBuffer(xsequencesIndexBuffer), sequencesIndexOffset(xsequencesIndexOffset) {}

	const VkCmdProcessCommandsInfoNVX& vkHandle() const { return reinterpret_cast<const VkCmdProcessCommandsInfoNVX&>(*this); }
	VkCmdProcessCommandsInfoNVX& vkHandle() { return reinterpret_cast<VkCmdProcessCommandsInfoNVX&>(*this); }

	operator const VkCmdProcessCommandsInfoNVX&() const { return vkHandle(); };
	operator VkCmdProcessCommandsInfoNVX&() { return vkHandle(); };
};

struct CmdReserveSpaceForCommandsInfoNVX {
	StructureType sType {StructureType::cmdReserveSpaceForCommandsInfoNVX};
	const void* pNext {};
	ObjectTableNVX objectTable {};
	IndirectCommandsLayoutNVX indirectCommandsLayout {};
	uint32_t maxSequencesCount {};

	CmdReserveSpaceForCommandsInfoNVX(ObjectTableNVX xobjectTable = {}, IndirectCommandsLayoutNVX xindirectCommandsLayout = {}, uint32_t xmaxSequencesCount = {}) : objectTable(xobjectTable), indirectCommandsLayout(xindirectCommandsLayout), maxSequencesCount(xmaxSequencesCount) {}

	const VkCmdReserveSpaceForCommandsInfoNVX& vkHandle() const { return reinterpret_cast<const VkCmdReserveSpaceForCommandsInfoNVX&>(*this); }
	VkCmdReserveSpaceForCommandsInfoNVX& vkHandle() { return reinterpret_cast<VkCmdReserveSpaceForCommandsInfoNVX&>(*this); }

	operator const VkCmdReserveSpaceForCommandsInfoNVX&() const { return vkHandle(); };
	operator VkCmdReserveSpaceForCommandsInfoNVX&() { return vkHandle(); };
};

struct ObjectTableCreateInfoNVX {
	StructureType sType {StructureType::objectTableCreateInfoNVX};
	const void* pNext {};
	uint32_t objectCount {};
	const ObjectEntryTypeNVX* pObjectEntryTypes {};
	const uint32_t* pObjectEntryCounts {};
	const ObjectEntryUsageFlagsNVX* pObjectEntryUsageFlags {};
	uint32_t maxUniformBuffersPerDescriptor {};
	uint32_t maxStorageBuffersPerDescriptor {};
	uint32_t maxStorageImagesPerDescriptor {};
	uint32_t maxSampledImagesPerDescriptor {};
	uint32_t maxPipelineLayouts {};

	ObjectTableCreateInfoNVX(uint32_t xobjectCount = {}, const ObjectEntryTypeNVX* xpObjectEntryTypes = {}, const uint32_t* xpObjectEntryCounts = {}, const ObjectEntryUsageFlagsNVX* xpObjectEntryUsageFlags = {}, uint32_t xmaxUniformBuffersPerDescriptor = {}, uint32_t xmaxStorageBuffersPerDescriptor = {}, uint32_t xmaxStorageImagesPerDescriptor = {}, uint32_t xmaxSampledImagesPerDescriptor = {}, uint32_t xmaxPipelineLayouts = {}) : objectCount(xobjectCount), pObjectEntryTypes(xpObjectEntryTypes), pObjectEntryCounts(xpObjectEntryCounts), pObjectEntryUsageFlags(xpObjectEntryUsageFlags), maxUniformBuffersPerDescriptor(xmaxUniformBuffersPerDescriptor), maxStorageBuffersPerDescriptor(xmaxStorageBuffersPerDescriptor), maxStorageImagesPerDescriptor(xmaxStorageImagesPerDescriptor), maxSampledImagesPerDescriptor(xmaxSampledImagesPerDescriptor), maxPipelineLayouts(xmaxPipelineLayouts) {}

	const VkObjectTableCreateInfoNVX& vkHandle() const { return reinterpret_cast<const VkObjectTableCreateInfoNVX&>(*this); }
	VkObjectTableCreateInfoNVX& vkHandle() { return reinterpret_cast<VkObjectTableCreateInfoNVX&>(*this); }

	operator const VkObjectTableCreateInfoNVX&() const { return vkHandle(); };
	operator VkObjectTableCreateInfoNVX&() { return vkHandle(); };
};

struct ObjectTableEntryNVX {
	ObjectEntryTypeNVX type {};
	ObjectEntryUsageFlagsNVX flags {};

	const VkObjectTableEntryNVX& vkHandle() const { return reinterpret_cast<const VkObjectTableEntryNVX&>(*this); }
	VkObjectTableEntryNVX& vkHandle() { return reinterpret_cast<VkObjectTableEntryNVX&>(*this); }

	operator const VkObjectTableEntryNVX&() const { return vkHandle(); };
	operator VkObjectTableEntryNVX&() { return vkHandle(); };
};

struct ObjectTablePipelineEntryNVX {
	ObjectEntryTypeNVX type {};
	ObjectEntryUsageFlagsNVX flags {};
	Pipeline pipeline {};

	const VkObjectTablePipelineEntryNVX& vkHandle() const { return reinterpret_cast<const VkObjectTablePipelineEntryNVX&>(*this); }
	VkObjectTablePipelineEntryNVX& vkHandle() { return reinterpret_cast<VkObjectTablePipelineEntryNVX&>(*this); }

	operator const VkObjectTablePipelineEntryNVX&() const { return vkHandle(); };
	operator VkObjectTablePipelineEntryNVX&() { return vkHandle(); };
};

struct ObjectTableDescriptorSetEntryNVX {
	ObjectEntryTypeNVX type {};
	ObjectEntryUsageFlagsNVX flags {};
	PipelineLayout pipelineLayout {};
	DescriptorSet descriptorSet {};

	const VkObjectTableDescriptorSetEntryNVX& vkHandle() const { return reinterpret_cast<const VkObjectTableDescriptorSetEntryNVX&>(*this); }
	VkObjectTableDescriptorSetEntryNVX& vkHandle() { return reinterpret_cast<VkObjectTableDescriptorSetEntryNVX&>(*this); }

	operator const VkObjectTableDescriptorSetEntryNVX&() const { return vkHandle(); };
	operator VkObjectTableDescriptorSetEntryNVX&() { return vkHandle(); };
};

struct ObjectTableVertexBufferEntryNVX {
	ObjectEntryTypeNVX type {};
	ObjectEntryUsageFlagsNVX flags {};
	Buffer buffer {};

	const VkObjectTableVertexBufferEntryNVX& vkHandle() const { return reinterpret_cast<const VkObjectTableVertexBufferEntryNVX&>(*this); }
	VkObjectTableVertexBufferEntryNVX& vkHandle() { return reinterpret_cast<VkObjectTableVertexBufferEntryNVX&>(*this); }

	operator const VkObjectTableVertexBufferEntryNVX&() const { return vkHandle(); };
	operator VkObjectTableVertexBufferEntryNVX&() { return vkHandle(); };
};

struct ObjectTableIndexBufferEntryNVX {
	ObjectEntryTypeNVX type {};
	ObjectEntryUsageFlagsNVX flags {};
	Buffer buffer {};
	IndexType indexType {};

	const VkObjectTableIndexBufferEntryNVX& vkHandle() const { return reinterpret_cast<const VkObjectTableIndexBufferEntryNVX&>(*this); }
	VkObjectTableIndexBufferEntryNVX& vkHandle() { return reinterpret_cast<VkObjectTableIndexBufferEntryNVX&>(*this); }

	operator const VkObjectTableIndexBufferEntryNVX&() const { return vkHandle(); };
	operator VkObjectTableIndexBufferEntryNVX&() { return vkHandle(); };
};

struct ObjectTablePushConstantEntryNVX {
	ObjectEntryTypeNVX type {};
	ObjectEntryUsageFlagsNVX flags {};
	PipelineLayout pipelineLayout {};
	ShaderStageFlags stageFlags {};

	const VkObjectTablePushConstantEntryNVX& vkHandle() const { return reinterpret_cast<const VkObjectTablePushConstantEntryNVX&>(*this); }
	VkObjectTablePushConstantEntryNVX& vkHandle() { return reinterpret_cast<VkObjectTablePushConstantEntryNVX&>(*this); }

	operator const VkObjectTablePushConstantEntryNVX&() const { return vkHandle(); };
	operator VkObjectTablePushConstantEntryNVX&() { return vkHandle(); };
};


struct ViewportWScalingNV {
	float xcoeff {};
	float ycoeff {};

	const VkViewportWScalingNV& vkHandle() const { return reinterpret_cast<const VkViewportWScalingNV&>(*this); }
	VkViewportWScalingNV& vkHandle() { return reinterpret_cast<VkViewportWScalingNV&>(*this); }

	operator const VkViewportWScalingNV&() const { return vkHandle(); };
	operator VkViewportWScalingNV&() { return vkHandle(); };
};

struct PipelineViewportWScalingStateCreateInfoNV {
	StructureType sType {StructureType::pipelineViewportWScalingStateCreateInfoNV};
	const void* pNext {};
	Bool32 viewportWScalingEnable {};
	uint32_t viewportCount {};
	const ViewportWScalingNV* pViewportWScalings {};

	PipelineViewportWScalingStateCreateInfoNV(Bool32 xviewportWScalingEnable = {}, uint32_t xviewportCount = {}, const ViewportWScalingNV* xpViewportWScalings = {}) : viewportWScalingEnable(xviewportWScalingEnable), viewportCount(xviewportCount), pViewportWScalings(xpViewportWScalings) {}

	const VkPipelineViewportWScalingStateCreateInfoNV& vkHandle() const { return reinterpret_cast<const VkPipelineViewportWScalingStateCreateInfoNV&>(*this); }
	VkPipelineViewportWScalingStateCreateInfoNV& vkHandle() { return reinterpret_cast<VkPipelineViewportWScalingStateCreateInfoNV&>(*this); }

	operator const VkPipelineViewportWScalingStateCreateInfoNV&() const { return vkHandle(); };
	operator VkPipelineViewportWScalingStateCreateInfoNV&() { return vkHandle(); };
};


struct SurfaceCapabilities2EXT {
	StructureType sType {StructureType::surfaceCapabilities2EXT};
	void* pNext {};
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
	SurfaceCounterFlagsEXT supportedSurfaceCounters {};

	const VkSurfaceCapabilities2EXT& vkHandle() const { return reinterpret_cast<const VkSurfaceCapabilities2EXT&>(*this); }
	VkSurfaceCapabilities2EXT& vkHandle() { return reinterpret_cast<VkSurfaceCapabilities2EXT&>(*this); }

	operator const VkSurfaceCapabilities2EXT&() const { return vkHandle(); };
	operator VkSurfaceCapabilities2EXT&() { return vkHandle(); };
};


struct DisplayPowerInfoEXT {
	StructureType sType {StructureType::displayPowerInfoEXT};
	const void* pNext {};
	DisplayPowerStateEXT powerState {};

	DisplayPowerInfoEXT(DisplayPowerStateEXT xpowerState = {}) : powerState(xpowerState) {}

	const VkDisplayPowerInfoEXT& vkHandle() const { return reinterpret_cast<const VkDisplayPowerInfoEXT&>(*this); }
	VkDisplayPowerInfoEXT& vkHandle() { return reinterpret_cast<VkDisplayPowerInfoEXT&>(*this); }

	operator const VkDisplayPowerInfoEXT&() const { return vkHandle(); };
	operator VkDisplayPowerInfoEXT&() { return vkHandle(); };
};

struct DeviceEventInfoEXT {
	StructureType sType {StructureType::deviceEventInfoEXT};
	const void* pNext {};
	DeviceEventTypeEXT deviceEvent {};

	DeviceEventInfoEXT(DeviceEventTypeEXT xdeviceEvent = {}) : deviceEvent(xdeviceEvent) {}

	const VkDeviceEventInfoEXT& vkHandle() const { return reinterpret_cast<const VkDeviceEventInfoEXT&>(*this); }
	VkDeviceEventInfoEXT& vkHandle() { return reinterpret_cast<VkDeviceEventInfoEXT&>(*this); }

	operator const VkDeviceEventInfoEXT&() const { return vkHandle(); };
	operator VkDeviceEventInfoEXT&() { return vkHandle(); };
};

struct DisplayEventInfoEXT {
	StructureType sType {StructureType::displayEventInfoEXT};
	const void* pNext {};
	DisplayEventTypeEXT displayEvent {};

	DisplayEventInfoEXT(DisplayEventTypeEXT xdisplayEvent = {}) : displayEvent(xdisplayEvent) {}

	const VkDisplayEventInfoEXT& vkHandle() const { return reinterpret_cast<const VkDisplayEventInfoEXT&>(*this); }
	VkDisplayEventInfoEXT& vkHandle() { return reinterpret_cast<VkDisplayEventInfoEXT&>(*this); }

	operator const VkDisplayEventInfoEXT&() const { return vkHandle(); };
	operator VkDisplayEventInfoEXT&() { return vkHandle(); };
};

struct SwapchainCounterCreateInfoEXT {
	StructureType sType {StructureType::swapchainCounterCreateInfoEXT};
	const void* pNext {};
	SurfaceCounterFlagsEXT surfaceCounters {};

	SwapchainCounterCreateInfoEXT(SurfaceCounterFlagsEXT xsurfaceCounters = {}) : surfaceCounters(xsurfaceCounters) {}

	const VkSwapchainCounterCreateInfoEXT& vkHandle() const { return reinterpret_cast<const VkSwapchainCounterCreateInfoEXT&>(*this); }
	VkSwapchainCounterCreateInfoEXT& vkHandle() { return reinterpret_cast<VkSwapchainCounterCreateInfoEXT&>(*this); }

	operator const VkSwapchainCounterCreateInfoEXT&() const { return vkHandle(); };
	operator VkSwapchainCounterCreateInfoEXT&() { return vkHandle(); };
};


struct RefreshCycleDurationGOOGLE {
	uint64_t refreshDuration {};

	const VkRefreshCycleDurationGOOGLE& vkHandle() const { return reinterpret_cast<const VkRefreshCycleDurationGOOGLE&>(*this); }
	VkRefreshCycleDurationGOOGLE& vkHandle() { return reinterpret_cast<VkRefreshCycleDurationGOOGLE&>(*this); }

	operator const VkRefreshCycleDurationGOOGLE&() const { return vkHandle(); };
	operator VkRefreshCycleDurationGOOGLE&() { return vkHandle(); };
};

struct PastPresentationTimingGOOGLE {
	uint32_t presentID {};
	uint64_t desiredPresentTime {};
	uint64_t actualPresentTime {};
	uint64_t earliestPresentTime {};
	uint64_t presentMargin {};

	const VkPastPresentationTimingGOOGLE& vkHandle() const { return reinterpret_cast<const VkPastPresentationTimingGOOGLE&>(*this); }
	VkPastPresentationTimingGOOGLE& vkHandle() { return reinterpret_cast<VkPastPresentationTimingGOOGLE&>(*this); }

	operator const VkPastPresentationTimingGOOGLE&() const { return vkHandle(); };
	operator VkPastPresentationTimingGOOGLE&() { return vkHandle(); };
};

struct PresentTimeGOOGLE {
	uint32_t presentID {};
	uint64_t desiredPresentTime {};

	const VkPresentTimeGOOGLE& vkHandle() const { return reinterpret_cast<const VkPresentTimeGOOGLE&>(*this); }
	VkPresentTimeGOOGLE& vkHandle() { return reinterpret_cast<VkPresentTimeGOOGLE&>(*this); }

	operator const VkPresentTimeGOOGLE&() const { return vkHandle(); };
	operator VkPresentTimeGOOGLE&() { return vkHandle(); };
};

struct PresentTimesInfoGOOGLE {
	StructureType sType {StructureType::presentTimesInfoGOOGLE};
	const void* pNext {};
	uint32_t swapchainCount {};
	const PresentTimeGOOGLE* pTimes {};

	PresentTimesInfoGOOGLE(uint32_t xswapchainCount = {}, const PresentTimeGOOGLE* xpTimes = {}) : swapchainCount(xswapchainCount), pTimes(xpTimes) {}

	const VkPresentTimesInfoGOOGLE& vkHandle() const { return reinterpret_cast<const VkPresentTimesInfoGOOGLE&>(*this); }
	VkPresentTimesInfoGOOGLE& vkHandle() { return reinterpret_cast<VkPresentTimesInfoGOOGLE&>(*this); }

	operator const VkPresentTimesInfoGOOGLE&() const { return vkHandle(); };
	operator VkPresentTimesInfoGOOGLE&() { return vkHandle(); };
};


struct PhysicalDeviceMultiviewPerViewAttributesPropertiesNVX {
	StructureType sType {StructureType::physicalDeviceMultiviewPerViewAttributesPropertiesNVX};
	void* pNext {};
	Bool32 perViewPositionAllComponents {};

	const VkPhysicalDeviceMultiviewPerViewAttributesPropertiesNVX& vkHandle() const { return reinterpret_cast<const VkPhysicalDeviceMultiviewPerViewAttributesPropertiesNVX&>(*this); }
	VkPhysicalDeviceMultiviewPerViewAttributesPropertiesNVX& vkHandle() { return reinterpret_cast<VkPhysicalDeviceMultiviewPerViewAttributesPropertiesNVX&>(*this); }

	operator const VkPhysicalDeviceMultiviewPerViewAttributesPropertiesNVX&() const { return vkHandle(); };
	operator VkPhysicalDeviceMultiviewPerViewAttributesPropertiesNVX&() { return vkHandle(); };
};


struct ViewportSwizzleNV {
	ViewportCoordinateSwizzleNV x {};
	ViewportCoordinateSwizzleNV y {};
	ViewportCoordinateSwizzleNV z {};
	ViewportCoordinateSwizzleNV w {};

	const VkViewportSwizzleNV& vkHandle() const { return reinterpret_cast<const VkViewportSwizzleNV&>(*this); }
	VkViewportSwizzleNV& vkHandle() { return reinterpret_cast<VkViewportSwizzleNV&>(*this); }

	operator const VkViewportSwizzleNV&() const { return vkHandle(); };
	operator VkViewportSwizzleNV&() { return vkHandle(); };
};

struct PipelineViewportSwizzleStateCreateInfoNV {
	StructureType sType {StructureType::pipelineViewportSwizzleStateCreateInfoNV};
	const void* pNext {};
	PipelineViewportSwizzleStateCreateFlagsNV flags {};
	uint32_t viewportCount {};
	const ViewportSwizzleNV* pViewportSwizzles {};

	PipelineViewportSwizzleStateCreateInfoNV(PipelineViewportSwizzleStateCreateFlagsNV xflags = {}, uint32_t xviewportCount = {}, const ViewportSwizzleNV* xpViewportSwizzles = {}) : flags(xflags), viewportCount(xviewportCount), pViewportSwizzles(xpViewportSwizzles) {}

	const VkPipelineViewportSwizzleStateCreateInfoNV& vkHandle() const { return reinterpret_cast<const VkPipelineViewportSwizzleStateCreateInfoNV&>(*this); }
	VkPipelineViewportSwizzleStateCreateInfoNV& vkHandle() { return reinterpret_cast<VkPipelineViewportSwizzleStateCreateInfoNV&>(*this); }

	operator const VkPipelineViewportSwizzleStateCreateInfoNV&() const { return vkHandle(); };
	operator VkPipelineViewportSwizzleStateCreateInfoNV&() { return vkHandle(); };
};


struct PhysicalDeviceDiscardRectanglePropertiesEXT {
	StructureType sType {StructureType::physicalDeviceDiscardRectanglePropertiesEXT};
	void* pNext {};
	uint32_t maxDiscardRectangles {};

	PhysicalDeviceDiscardRectanglePropertiesEXT(uint32_t xmaxDiscardRectangles = {}) : maxDiscardRectangles(xmaxDiscardRectangles) {}

	const VkPhysicalDeviceDiscardRectanglePropertiesEXT& vkHandle() const { return reinterpret_cast<const VkPhysicalDeviceDiscardRectanglePropertiesEXT&>(*this); }
	VkPhysicalDeviceDiscardRectanglePropertiesEXT& vkHandle() { return reinterpret_cast<VkPhysicalDeviceDiscardRectanglePropertiesEXT&>(*this); }

	operator const VkPhysicalDeviceDiscardRectanglePropertiesEXT&() const { return vkHandle(); };
	operator VkPhysicalDeviceDiscardRectanglePropertiesEXT&() { return vkHandle(); };
};

struct PipelineDiscardRectangleStateCreateInfoEXT {
	StructureType sType {StructureType::pipelineDiscardRectangleStateCreateInfoEXT};
	const void* pNext {};
	PipelineDiscardRectangleStateCreateFlagsEXT flags {};
	DiscardRectangleModeEXT discardRectangleMode {};
	uint32_t discardRectangleCount {};
	const Rect2D* pDiscardRectangles {};

	PipelineDiscardRectangleStateCreateInfoEXT(PipelineDiscardRectangleStateCreateFlagsEXT xflags = {}, DiscardRectangleModeEXT xdiscardRectangleMode = {}, uint32_t xdiscardRectangleCount = {}, const Rect2D* xpDiscardRectangles = {}) : flags(xflags), discardRectangleMode(xdiscardRectangleMode), discardRectangleCount(xdiscardRectangleCount), pDiscardRectangles(xpDiscardRectangles) {}

	const VkPipelineDiscardRectangleStateCreateInfoEXT& vkHandle() const { return reinterpret_cast<const VkPipelineDiscardRectangleStateCreateInfoEXT&>(*this); }
	VkPipelineDiscardRectangleStateCreateInfoEXT& vkHandle() { return reinterpret_cast<VkPipelineDiscardRectangleStateCreateInfoEXT&>(*this); }

	operator const VkPipelineDiscardRectangleStateCreateInfoEXT&() const { return vkHandle(); };
	operator VkPipelineDiscardRectangleStateCreateInfoEXT&() { return vkHandle(); };
};


struct XYColorEXT {
	float x {};
	float y {};

	const VkXYColorEXT& vkHandle() const { return reinterpret_cast<const VkXYColorEXT&>(*this); }
	VkXYColorEXT& vkHandle() { return reinterpret_cast<VkXYColorEXT&>(*this); }

	operator const VkXYColorEXT&() const { return vkHandle(); };
	operator VkXYColorEXT&() { return vkHandle(); };
};

struct HdrMetadataEXT {
	StructureType sType {StructureType::hdrMetadataEXT};
	const void* pNext {};
	XYColorEXT displayPrimaryRed {};
	XYColorEXT displayPrimaryGreen {};
	XYColorEXT displayPrimaryBlue {};
	XYColorEXT whitePoint {};
	float maxLuminance {};
	float minLuminance {};
	float maxContentLightLevel {};
	float maxFrameAverageLightLevel {};

	HdrMetadataEXT(XYColorEXT xdisplayPrimaryRed = {}, XYColorEXT xdisplayPrimaryGreen = {}, XYColorEXT xdisplayPrimaryBlue = {}, XYColorEXT xwhitePoint = {}, float xmaxLuminance = {}, float xminLuminance = {}, float xmaxContentLightLevel = {}, float xmaxFrameAverageLightLevel = {}) : displayPrimaryRed(xdisplayPrimaryRed), displayPrimaryGreen(xdisplayPrimaryGreen), displayPrimaryBlue(xdisplayPrimaryBlue), whitePoint(xwhitePoint), maxLuminance(xmaxLuminance), minLuminance(xminLuminance), maxContentLightLevel(xmaxContentLightLevel), maxFrameAverageLightLevel(xmaxFrameAverageLightLevel) {}

	const VkHdrMetadataEXT& vkHandle() const { return reinterpret_cast<const VkHdrMetadataEXT&>(*this); }
	VkHdrMetadataEXT& vkHandle() { return reinterpret_cast<VkHdrMetadataEXT&>(*this); }

	operator const VkHdrMetadataEXT&() const { return vkHandle(); };
	operator VkHdrMetadataEXT&() { return vkHandle(); };
};


#ifdef VK_USE_PLATFORM_IOS_MVK

struct IOSSurfaceCreateInfoMVK {
	StructureType sType {StructureType::iOSSurfaceCreateInfoMVK};
	const void* pNext {};
	IOSSurfaceCreateFlagsMVK flags {};
	const void* pView {};

	IOSSurfaceCreateInfoMVK(IOSSurfaceCreateFlagsMVK xflags = {}, const void* xpView = {}) : flags(xflags), pView(xpView) {}

	const VkIOSSurfaceCreateInfoMVK& vkHandle() const { return reinterpret_cast<const VkIOSSurfaceCreateInfoMVK&>(*this); }
	VkIOSSurfaceCreateInfoMVK& vkHandle() { return reinterpret_cast<VkIOSSurfaceCreateInfoMVK&>(*this); }

	operator const VkIOSSurfaceCreateInfoMVK&() const { return vkHandle(); };
	operator VkIOSSurfaceCreateInfoMVK&() { return vkHandle(); };
};


#endif //VK_USE_PLATFORM_IOS_MVK

#ifdef VK_USE_PLATFORM_MACOS_MVK

struct MacOSSurfaceCreateInfoMVK {
	StructureType sType {StructureType::macOSSurfaceCreateInfoMVK};
	const void* pNext {};
	MacOSSurfaceCreateFlagsMVK flags {};
	const void* pView {};

	MacOSSurfaceCreateInfoMVK(MacOSSurfaceCreateFlagsMVK xflags = {}, const void* xpView = {}) : flags(xflags), pView(xpView) {}

	const VkMacOSSurfaceCreateInfoMVK& vkHandle() const { return reinterpret_cast<const VkMacOSSurfaceCreateInfoMVK&>(*this); }
	VkMacOSSurfaceCreateInfoMVK& vkHandle() { return reinterpret_cast<VkMacOSSurfaceCreateInfoMVK&>(*this); }

	operator const VkMacOSSurfaceCreateInfoMVK&() const { return vkHandle(); };
	operator VkMacOSSurfaceCreateInfoMVK&() { return vkHandle(); };
};


#endif //VK_USE_PLATFORM_MACOS_MVK


} // namespace vk

// The specification (vk.xml) itself is published under the following license:

// Copyright (c) 2015-2017 The Khronos Group Inc.
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
    