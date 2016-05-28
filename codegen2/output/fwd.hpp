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

#include "flags.hpp"

#include <vulkan/vulkan.h>
#include <cstdint>
namespace vk
{

enum class DummyEnum : std::int32_t {};

constexpr auto lodClampNone = 1000.0f;
constexpr auto remainingMipLevels = (~0U);
constexpr auto remainingArrayLayers = (~0U);
constexpr auto wholeSize = (~0ULL);
constexpr auto attachmentUnused = (~0U);
constexpr auto queueFamilyIgnored = (~0U);
constexpr auto subpassExternal = (~0U);

using Instance = VkInstance;
using PhysicalDevice = VkPhysicalDevice;
using Device = VkDevice;
using Queue = VkQueue;
using Semaphore = VkSemaphore;
using CommandBuffer = VkCommandBuffer;
using Fence = VkFence;
using DeviceMemory = VkDeviceMemory;
using Buffer = VkBuffer;
using Image = VkImage;
using Event = VkEvent;
using QueryPool = VkQueryPool;
using BufferView = VkBufferView;
using ImageView = VkImageView;
using ShaderModule = VkShaderModule;
using PipelineCache = VkPipelineCache;
using PipelineLayout = VkPipelineLayout;
using RenderPass = VkRenderPass;
using Pipeline = VkPipeline;
using DescriptorSetLayout = VkDescriptorSetLayout;
using Sampler = VkSampler;
using DescriptorPool = VkDescriptorPool;
using DescriptorSet = VkDescriptorSet;
using Framebuffer = VkFramebuffer;
using CommandPool = VkCommandPool;

using Bool32 = uint32_t;
using DeviceSize = uint64_t;
using SampleMask = uint32_t;

enum class PipelineCacheHeaderVersion : std::int32_t;
enum class StructureType : std::int32_t;
enum class SystemAllocationScope : std::int32_t;
enum class InternalAllocationType : std::int32_t;
enum class Result : std::int32_t;
enum class Format : std::int32_t;
enum class FormatFeatureBits : std::int32_t;
enum class ImageType : std::int32_t;
enum class ImageTiling : std::int32_t;
enum class ImageUsageBits : std::int32_t;
enum class ImageCreateBits : std::int32_t;
enum class SampleCountBits : std::int32_t;
enum class PhysicalDeviceType : std::int32_t;
enum class QueueBits : std::int32_t;
enum class MemoryPropertyBits : std::int32_t;
enum class MemoryHeapBits : std::int32_t;
enum class PipelineStageBits : std::int32_t;
enum class ImageAspectBits : std::int32_t;
enum class SparseImageFormatBits : std::int32_t;
enum class SparseMemoryBindBits : std::int32_t;
enum class FenceCreateBits : std::int32_t;
enum class QueryType : std::int32_t;
enum class QueryPipelineStatisticBits : std::int32_t;
enum class QueryResultBits : std::int32_t;
enum class BufferCreateBits : std::int32_t;
enum class BufferUsageBits : std::int32_t;
enum class SharingMode : std::int32_t;
enum class ImageLayout : std::int32_t;
enum class ImageViewType : std::int32_t;
enum class ComponentSwizzle : std::int32_t;
enum class PipelineCreateBits : std::int32_t;
enum class ShaderStageBits : std::int32_t;
enum class VertexInputRate : std::int32_t;
enum class PrimitiveTopology : std::int32_t;
enum class PolygonMode : std::int32_t;
enum class CullModeBits : std::int32_t;
enum class FrontFace : std::int32_t;
enum class CompareOp : std::int32_t;
enum class StencilOp : std::int32_t;
enum class LogicOp : std::int32_t;
enum class BlendFactor : std::int32_t;
enum class BlendOp : std::int32_t;
enum class ColorComponentBits : std::int32_t;
enum class DynamicState : std::int32_t;
enum class Filter : std::int32_t;
enum class SamplerMipmapMode : std::int32_t;
enum class SamplerAddressMode : std::int32_t;
enum class BorderColor : std::int32_t;
enum class DescriptorType : std::int32_t;
enum class DescriptorPoolCreateBits : std::int32_t;
enum class AttachmentDescriptionBits : std::int32_t;
enum class AttachmentLoadOp : std::int32_t;
enum class AttachmentStoreOp : std::int32_t;
enum class PipelineBindPoint : std::int32_t;
enum class AccessBits : std::int32_t;
enum class DependencyBits : std::int32_t;
enum class CommandPoolCreateBits : std::int32_t;
enum class CommandPoolResetBits : std::int32_t;
enum class CommandBufferLevel : std::int32_t;
enum class CommandBufferUsageBits : std::int32_t;
enum class QueryControlBits : std::int32_t;
enum class CommandBufferResetBits : std::int32_t;
enum class StencilFaceBits : std::int32_t;
enum class IndexType : std::int32_t;
enum class SubpassContents : std::int32_t;

using InstanceCreateFlags = Flags<DummyEnum>;
using FormatFeatureFlags = Flags<FormatFeatureBits>;
using ImageUsageFlags = Flags<ImageUsageBits>;
using ImageCreateFlags = Flags<ImageCreateBits>;
using SampleCountFlags = Flags<SampleCountBits>;
using QueueFlags = Flags<QueueBits>;
using MemoryPropertyFlags = Flags<MemoryPropertyBits>;
using MemoryHeapFlags = Flags<MemoryHeapBits>;
using DeviceCreateFlags = Flags<DummyEnum>;
using DeviceQueueCreateFlags = Flags<DummyEnum>;
using PipelineStageFlags = Flags<PipelineStageBits>;
using MemoryMapFlags = Flags<DummyEnum>;
using ImageAspectFlags = Flags<ImageAspectBits>;
using SparseImageFormatFlags = Flags<SparseImageFormatBits>;
using SparseMemoryBindFlags = Flags<SparseMemoryBindBits>;
using FenceCreateFlags = Flags<FenceCreateBits>;
using SemaphoreCreateFlags = Flags<DummyEnum>;
using EventCreateFlags = Flags<DummyEnum>;
using QueryPoolCreateFlags = Flags<DummyEnum>;
using QueryPipelineStatisticFlags = Flags<QueryPipelineStatisticBits>;
using QueryResultFlags = Flags<QueryResultBits>;
using BufferCreateFlags = Flags<BufferCreateBits>;
using BufferUsageFlags = Flags<BufferUsageBits>;
using BufferViewCreateFlags = Flags<DummyEnum>;
using ImageViewCreateFlags = Flags<DummyEnum>;
using ShaderModuleCreateFlags = Flags<DummyEnum>;
using PipelineCacheCreateFlags = Flags<DummyEnum>;
using PipelineCreateFlags = Flags<PipelineCreateBits>;
using PipelineShaderStageCreateFlags = Flags<DummyEnum>;
using PipelineVertexInputStateCreateFlags = Flags<DummyEnum>;
using PipelineInputAssemblyStateCreateFlags = Flags<DummyEnum>;
using PipelineTessellationStateCreateFlags = Flags<DummyEnum>;
using PipelineViewportStateCreateFlags = Flags<DummyEnum>;
using PipelineRasterizationStateCreateFlags = Flags<DummyEnum>;
using CullModeFlags = Flags<CullModeBits>;
using PipelineMultisampleStateCreateFlags = Flags<DummyEnum>;
using PipelineDepthStencilStateCreateFlags = Flags<DummyEnum>;
using PipelineColorBlendStateCreateFlags = Flags<DummyEnum>;
using ColorComponentFlags = Flags<ColorComponentBits>;
using PipelineDynamicStateCreateFlags = Flags<DummyEnum>;
using PipelineLayoutCreateFlags = Flags<DummyEnum>;
using ShaderStageFlags = Flags<ShaderStageBits>;
using SamplerCreateFlags = Flags<DummyEnum>;
using DescriptorSetLayoutCreateFlags = Flags<DummyEnum>;
using DescriptorPoolCreateFlags = Flags<DescriptorPoolCreateBits>;
using DescriptorPoolResetFlags = Flags<DummyEnum>;
using FramebufferCreateFlags = Flags<DummyEnum>;
using RenderPassCreateFlags = Flags<DummyEnum>;
using AttachmentDescriptionFlags = Flags<AttachmentDescriptionBits>;
using SubpassDescriptionFlags = Flags<DummyEnum>;
using AccessFlags = Flags<AccessBits>;
using DependencyFlags = Flags<DependencyBits>;
using CommandPoolCreateFlags = Flags<CommandPoolCreateBits>;
using CommandPoolResetFlags = Flags<CommandPoolResetBits>;
using CommandBufferUsageFlags = Flags<CommandBufferUsageBits>;
using QueryControlFlags = Flags<QueryControlBits>;
using CommandBufferResetFlags = Flags<CommandBufferResetBits>;
using StencilFaceFlags = Flags<StencilFaceBits>;

struct ApplicationInfo;
struct InstanceCreateInfo;
struct AllocationCallbacks;
struct PhysicalDeviceFeatures;
struct FormatProperties;
struct Extent3D;
struct ImageFormatProperties;
struct PhysicalDeviceLimits;
struct PhysicalDeviceSparseProperties;
struct PhysicalDeviceProperties;
struct QueueFamilyProperties;
struct MemoryType;
struct MemoryHeap;
struct PhysicalDeviceMemoryProperties;
struct DeviceQueueCreateInfo;
struct DeviceCreateInfo;
struct ExtensionProperties;
struct LayerProperties;
struct SubmitInfo;
struct MemoryAllocateInfo;
struct MappedMemoryRange;
struct MemoryRequirements;
struct SparseImageFormatProperties;
struct SparseImageMemoryRequirements;
struct SparseMemoryBind;
struct SparseBufferMemoryBindInfo;
struct SparseImageOpaqueMemoryBindInfo;
struct ImageSubresource;
struct Offset3D;
struct SparseImageMemoryBind;
struct SparseImageMemoryBindInfo;
struct BindSparseInfo;
struct FenceCreateInfo;
struct SemaphoreCreateInfo;
struct EventCreateInfo;
struct QueryPoolCreateInfo;
struct BufferCreateInfo;
struct BufferViewCreateInfo;
struct ImageCreateInfo;
struct SubresourceLayout;
struct ComponentMapping;
struct ImageSubresourceRange;
struct ImageViewCreateInfo;
struct ShaderModuleCreateInfo;
struct PipelineCacheCreateInfo;
struct SpecializationMapEntry;
struct SpecializationInfo;
struct PipelineShaderStageCreateInfo;
struct VertexInputBindingDescription;
struct VertexInputAttributeDescription;
struct PipelineVertexInputStateCreateInfo;
struct PipelineInputAssemblyStateCreateInfo;
struct PipelineTessellationStateCreateInfo;
struct Viewport;
struct Offset2D;
struct Extent2D;
struct Rect2D;
struct PipelineViewportStateCreateInfo;
struct PipelineRasterizationStateCreateInfo;
struct PipelineMultisampleStateCreateInfo;
struct StencilOpState;
struct PipelineDepthStencilStateCreateInfo;
struct PipelineColorBlendAttachmentState;
struct PipelineColorBlendStateCreateInfo;
struct PipelineDynamicStateCreateInfo;
struct GraphicsPipelineCreateInfo;
struct ComputePipelineCreateInfo;
struct PushConstantRange;
struct PipelineLayoutCreateInfo;
struct SamplerCreateInfo;
struct DescriptorSetLayoutBinding;
struct DescriptorSetLayoutCreateInfo;
struct DescriptorPoolSize;
struct DescriptorPoolCreateInfo;
struct DescriptorSetAllocateInfo;
struct DescriptorImageInfo;
struct DescriptorBufferInfo;
struct WriteDescriptorSet;
struct CopyDescriptorSet;
struct FramebufferCreateInfo;
struct AttachmentDescription;
struct AttachmentReference;
struct SubpassDescription;
struct SubpassDependency;
struct RenderPassCreateInfo;
struct CommandPoolCreateInfo;
struct CommandBufferAllocateInfo;
struct CommandBufferInheritanceInfo;
struct CommandBufferBeginInfo;
struct BufferCopy;
struct ImageSubresourceLayers;
struct ImageCopy;
struct ImageBlit;
struct BufferImageCopy;
union ClearColorValue;
struct ClearDepthStencilValue;
union ClearValue;
struct ClearAttachment;
struct ClearRect;
struct ImageResolve;
struct MemoryBarrier;
struct BufferMemoryBarrier;
struct ImageMemoryBarrier;
struct RenderPassBeginInfo;
struct DispatchIndirectCommand;
struct DrawIndexedIndirectCommand;
struct DrawIndirectCommand;

using PfnAllocationFunction = void*(*)(void* pUserData, size_t size, size_t alignment, SystemAllocationScope allocationScope);
using PfnReallocationFunction = void*(*)(void* pUserData, void* pOriginal, size_t size, size_t alignment, SystemAllocationScope allocationScope);
using PfnFreeFunction = void(*)(void* pUserData, void* pMemory);
using PfnInternalAllocationNotification = void(*)(void* pUserData, size_t size, InternalAllocationType allocationType, SystemAllocationScope allocationScope);
using PfnInternalFreeNotification = void(*)(void* pUserData, size_t size, InternalAllocationType allocationType, SystemAllocationScope allocationScope);
using PfnVoidFunction = void(*)();

constexpr auto khrSurfaceSpecVersion = 25;
constexpr auto khrSurfaceExtensionName = "VK_KHR_surface";
constexpr auto colorspaceSrgbNonlinearKhr = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;

using SurfaceKHR = VkSurfaceKHR;

enum class SurfaceTransformBitsKHR : std::int32_t;
enum class CompositeAlphaBitsKHR : std::int32_t;
enum class ColorSpaceKHR : std::int32_t;
enum class PresentModeKHR : std::int32_t;

using SurfaceTransformFlagsKHR = Flags<SurfaceTransformBitsKHR>;
using CompositeAlphaFlagsKHR = Flags<CompositeAlphaBitsKHR>;

struct SurfaceCapabilitiesKHR;
struct SurfaceFormatKHR;

constexpr auto khrSwapchainSpecVersion = 68;
constexpr auto khrSwapchainExtensionName = "VK_KHR_swapchain";

using SwapchainKHR = VkSwapchainKHR;

using SwapchainCreateFlagsKHR = Flags<DummyEnum>;

struct SwapchainCreateInfoKHR;
struct PresentInfoKHR;

constexpr auto khrDisplaySpecVersion = 21;
constexpr auto khrDisplayExtensionName = "VK_KHR_display";

using DisplayKHR = VkDisplayKHR;
using DisplayModeKHR = VkDisplayModeKHR;

enum class DisplayPlaneAlphaBitsKHR : std::int32_t;

using DisplayPlaneAlphaFlagsKHR = Flags<DisplayPlaneAlphaBitsKHR>;
using DisplayModeCreateFlagsKHR = Flags<DummyEnum>;
using DisplaySurfaceCreateFlagsKHR = Flags<DummyEnum>;

struct DisplayPropertiesKHR;
struct DisplayModeParametersKHR;
struct DisplayModePropertiesKHR;
struct DisplayModeCreateInfoKHR;
struct DisplayPlaneCapabilitiesKHR;
struct DisplayPlanePropertiesKHR;
struct DisplaySurfaceCreateInfoKHR;

constexpr auto khrDisplaySwapchainSpecVersion = 9;
constexpr auto khrDisplaySwapchainExtensionName = "VK_KHR_display_swapchain";

struct DisplayPresentInfoKHR;

#ifdef VK_USE_PLATFORM_XLIB_KHR

constexpr auto khrXlibSurfaceSpecVersion = 6;
constexpr auto khrXlibSurfaceExtensionName = "VK_KHR_xlib_surface";

using XlibSurfaceCreateFlagsKHR = Flags<DummyEnum>;

struct XlibSurfaceCreateInfoKHR;

#endif //VK_USE_PLATFORM_XLIB_KHR

#ifdef VK_USE_PLATFORM_XCB_KHR

constexpr auto khrXcbSurfaceSpecVersion = 6;
constexpr auto khrXcbSurfaceExtensionName = "VK_KHR_xcb_surface";

using XcbSurfaceCreateFlagsKHR = Flags<DummyEnum>;

struct XcbSurfaceCreateInfoKHR;

#endif //VK_USE_PLATFORM_XCB_KHR

#ifdef VK_USE_PLATFORM_WAYLAND_KHR

constexpr auto khrWaylandSurfaceSpecVersion = 5;
constexpr auto khrWaylandSurfaceExtensionName = "VK_KHR_wayland_surface";

using WaylandSurfaceCreateFlagsKHR = Flags<DummyEnum>;

struct WaylandSurfaceCreateInfoKHR;

#endif //VK_USE_PLATFORM_WAYLAND_KHR

#ifdef VK_USE_PLATFORM_MIR_KHR

constexpr auto khrMirSurfaceSpecVersion = 4;
constexpr auto khrMirSurfaceExtensionName = "VK_KHR_mir_surface";

using MirSurfaceCreateFlagsKHR = Flags<DummyEnum>;

struct MirSurfaceCreateInfoKHR;

#endif //VK_USE_PLATFORM_MIR_KHR

#ifdef VK_USE_PLATFORM_ANDROID_KHR

constexpr auto khrAndroidSurfaceSpecVersion = 6;
constexpr auto khrAndroidSurfaceExtensionName = "VK_KHR_android_surface";

using AndroidSurfaceCreateFlagsKHR = Flags<DummyEnum>;

struct AndroidSurfaceCreateInfoKHR;

#endif //VK_USE_PLATFORM_ANDROID_KHR

#ifdef VK_USE_PLATFORM_WIN32_KHR

constexpr auto khrWin32SurfaceSpecVersion = 5;
constexpr auto khrWin32SurfaceExtensionName = "VK_KHR_win32_surface";

using Win32SurfaceCreateFlagsKHR = Flags<DummyEnum>;

struct Win32SurfaceCreateInfoKHR;

#endif //VK_USE_PLATFORM_WIN32_KHR

constexpr auto androidNativeBufferSpecVersion = 4;
constexpr auto androidNativeBufferNumber = 11;
constexpr auto androidNativeBufferName = "VK_ANDROID_native_buffer";

constexpr auto extDebugReportSpecVersion = 2;
constexpr auto extDebugReportExtensionName = "VK_EXT_debug_report";
constexpr auto structureTypeDebugReportCreateInfoExt = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;

using DebugReportCallbackEXT = VkDebugReportCallbackEXT;

enum class DebugReportObjectTypeEXT : std::int32_t;
enum class DebugReportErrorEXT : std::int32_t;
enum class DebugReportBitsEXT : std::int32_t;

using DebugReportFlagsEXT = Flags<DebugReportBitsEXT>;

struct DebugReportCallbackCreateInfoEXT;

using PfnDebugReportCallbackEXT = bool(*)(DebugReportFlagsEXT flags, DebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char* pLayerPrefix, const char* pMessage, void* pUserData);

constexpr auto nvGlslShaderSpecVersion = 1;
constexpr auto nvGlslShaderExtensionName = "VK_NV_glsl_shader";

constexpr auto nvExtension1SpecVersion = 0;
constexpr auto nvExtension1ExtensionName = "VK_NV_extension_1";

constexpr auto khrSamplerMirrorClampToEdgeSpecVersion = 1;
constexpr auto khrSamplerMirrorClampToEdgeExtensionName = "VK_KHR_sampler_mirror_clamp_to_edge";

constexpr auto imgFilterCubicSpecVersion = 1;
constexpr auto imgFilterCubicExtensionName = "VK_IMG_filter_cubic";

constexpr auto amdExtension1SpecVersion = 0;
constexpr auto amdExtension1ExtensionName = "VK_AMD_extension_1";

constexpr auto amdExtension2SpecVersion = 0;
constexpr auto amdExtension2ExtensionName = "VK_AMD_extension_2";

constexpr auto amdRasterizationOrderSpecVersion = 1;
constexpr auto amdRasterizationOrderExtensionName = "VK_AMD_rasterization_order";

enum class RasterizationOrderAMD : std::int32_t;

struct PipelineRasterizationStateRasterizationOrderAMD;

constexpr auto amdExtension4SpecVersion = 0;
constexpr auto amdExtension4ExtensionName = "VK_AMD_extension_4";

constexpr auto amdExtension5SpecVersion = 0;
constexpr auto amdExtension5ExtensionName = "VK_AMD_extension_5";

constexpr auto amdExtension6SpecVersion = 0;
constexpr auto amdExtension6ExtensionName = "VK_AMD_extension_6";

constexpr auto extDebugMarkerSpecVersion = 3;
constexpr auto extDebugMarkerExtensionName = "VK_EXT_debug_marker";

struct DebugMarkerObjectNameInfoEXT;
struct DebugMarkerObjectTagInfoEXT;
struct DebugMarkerMarkerInfoEXT;



} //namespace vk