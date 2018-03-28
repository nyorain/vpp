// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

// Automaitcally generated vulkan header, see nyorain/vkpp
// Do not edit manually, rather edit the codegen files.

#pragma once

#include <vkpp/handle.hpp> // VK_DEFINE_HANDLE
#include <vkpp/flags.hpp>
#include <cstddef>

#include <vulkan/vk_platform.h>

// Generated for vulkan version: 1.0.20

// Utility forward declarations
namespace nytl {

#ifndef NYTL_INCLUDE_FWD_SPAN
#define NYTL_INCLUDE_FWD_SPAN
	template<typename T, std::size_t N = 0>
	class Span;
#endif // header guard
	
} // namespace nytl

namespace vk {

using nytl::Span; // span.hpp
using nytl::Flags; // flags.hpp

enum class DummyEnum : int32_t {};

constexpr auto lodClampNone = 1000.0f;
constexpr auto remainingMipLevels = (~0U);
constexpr auto remainingArrayLayers = (~0U);
constexpr auto wholeSize = (~0ULL);
constexpr auto attachmentUnused = (~0U);
constexpr auto queueFamilyIgnored = (~0U);
constexpr auto subpassExternal = (~0U);
constexpr auto maxPhysicalDeviceNameSize = 256;
constexpr auto uuidSize = 16;
constexpr auto maxMemoryTypes = 32;
constexpr auto maxMemoryHeaps = 16;
constexpr auto maxExtensionNameSize = 256;
constexpr auto maxDescriptionSize = 256;

VK_DEFINE_HANDLE(Instance)
VK_DEFINE_HANDLE(PhysicalDevice)
VK_DEFINE_HANDLE(Device)
VK_DEFINE_HANDLE(Queue)
VK_DEFINE_NON_DISPATCHABLE_HANDLE(Semaphore)
VK_DEFINE_HANDLE(CommandBuffer)
VK_DEFINE_NON_DISPATCHABLE_HANDLE(Fence)
VK_DEFINE_NON_DISPATCHABLE_HANDLE(DeviceMemory)
VK_DEFINE_NON_DISPATCHABLE_HANDLE(Buffer)
VK_DEFINE_NON_DISPATCHABLE_HANDLE(Image)
VK_DEFINE_NON_DISPATCHABLE_HANDLE(Event)
VK_DEFINE_NON_DISPATCHABLE_HANDLE(QueryPool)
VK_DEFINE_NON_DISPATCHABLE_HANDLE(BufferView)
VK_DEFINE_NON_DISPATCHABLE_HANDLE(ImageView)
VK_DEFINE_NON_DISPATCHABLE_HANDLE(ShaderModule)
VK_DEFINE_NON_DISPATCHABLE_HANDLE(PipelineCache)
VK_DEFINE_NON_DISPATCHABLE_HANDLE(PipelineLayout)
VK_DEFINE_NON_DISPATCHABLE_HANDLE(RenderPass)
VK_DEFINE_NON_DISPATCHABLE_HANDLE(Pipeline)
VK_DEFINE_NON_DISPATCHABLE_HANDLE(DescriptorSetLayout)
VK_DEFINE_NON_DISPATCHABLE_HANDLE(Sampler)
VK_DEFINE_NON_DISPATCHABLE_HANDLE(DescriptorPool)
VK_DEFINE_NON_DISPATCHABLE_HANDLE(DescriptorSet)
VK_DEFINE_NON_DISPATCHABLE_HANDLE(Framebuffer)
VK_DEFINE_NON_DISPATCHABLE_HANDLE(CommandPool)

using Bool32 = uint32_t;
using DeviceSize = uint64_t;
using SampleMask = uint32_t;

enum class PipelineCacheHeaderVersion : int32_t;
enum class StructureType : int32_t;
enum class SystemAllocationScope : int32_t;
enum class InternalAllocationType : int32_t;
enum class Result : int32_t;
enum class Format : int32_t;
enum class FormatFeatureBits : int32_t;
enum class ImageType : int32_t;
enum class ImageTiling : int32_t;
enum class ImageUsageBits : int32_t;
enum class ImageCreateBits : int32_t;
enum class SampleCountBits : int32_t;
enum class PhysicalDeviceType : int32_t;
enum class QueueBits : int32_t;
enum class MemoryPropertyBits : int32_t;
enum class MemoryHeapBits : int32_t;
enum class PipelineStageBits : int32_t;
enum class ImageAspectBits : int32_t;
enum class SparseImageFormatBits : int32_t;
enum class SparseMemoryBindBits : int32_t;
enum class FenceCreateBits : int32_t;
enum class QueryType : int32_t;
enum class QueryPipelineStatisticBits : int32_t;
enum class QueryResultBits : int32_t;
enum class BufferCreateBits : int32_t;
enum class BufferUsageBits : int32_t;
enum class SharingMode : int32_t;
enum class ImageLayout : int32_t;
enum class ImageViewType : int32_t;
enum class ComponentSwizzle : int32_t;
enum class PipelineCreateBits : int32_t;
enum class ShaderStageBits : int32_t;
enum class VertexInputRate : int32_t;
enum class PrimitiveTopology : int32_t;
enum class PolygonMode : int32_t;
enum class CullModeBits : int32_t;
enum class FrontFace : int32_t;
enum class CompareOp : int32_t;
enum class StencilOp : int32_t;
enum class LogicOp : int32_t;
enum class BlendFactor : int32_t;
enum class BlendOp : int32_t;
enum class ColorComponentBits : int32_t;
enum class DynamicState : int32_t;
enum class Filter : int32_t;
enum class SamplerMipmapMode : int32_t;
enum class SamplerAddressMode : int32_t;
enum class BorderColor : int32_t;
enum class DescriptorType : int32_t;
enum class DescriptorPoolCreateBits : int32_t;
enum class AttachmentDescriptionBits : int32_t;
enum class AttachmentLoadOp : int32_t;
enum class AttachmentStoreOp : int32_t;
enum class PipelineBindPoint : int32_t;
enum class AccessBits : int32_t;
enum class DependencyBits : int32_t;
enum class CommandPoolCreateBits : int32_t;
enum class CommandPoolResetBits : int32_t;
enum class CommandBufferLevel : int32_t;
enum class CommandBufferUsageBits : int32_t;
enum class QueryControlBits : int32_t;
enum class CommandBufferResetBits : int32_t;
enum class StencilFaceBits : int32_t;
enum class IndexType : int32_t;
enum class SubpassContents : int32_t;

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

using PfnAllocationFunction = void*(*VKAPI_PTR)(void* pUserData, size_t size, size_t alignment, SystemAllocationScope allocationScope);
using PfnReallocationFunction = void*(*VKAPI_PTR)(void* pUserData, void* pOriginal, size_t size, size_t alignment, SystemAllocationScope allocationScope);
using PfnFreeFunction = void(*VKAPI_PTR)(void* pUserData, void* pMemory);
using PfnInternalAllocationNotification = void(*VKAPI_PTR)(void* pUserData, size_t size, InternalAllocationType allocationType, SystemAllocationScope allocationScope);
using PfnInternalFreeNotification = void(*VKAPI_PTR)(void* pUserData, size_t size, InternalAllocationType allocationType, SystemAllocationScope allocationScope);
using PfnVoidFunction = void(*VKAPI_PTR)();

VK_DEFINE_NON_DISPATCHABLE_HANDLE(SurfaceKHR)

enum class SurfaceTransformBitsKHR : int32_t;
enum class CompositeAlphaBitsKHR : int32_t;
enum class ColorSpaceKHR : int32_t;
enum class PresentModeKHR : int32_t;

using SurfaceTransformFlagsKHR = Flags<SurfaceTransformBitsKHR>;
using CompositeAlphaFlagsKHR = Flags<CompositeAlphaBitsKHR>;

struct SurfaceCapabilitiesKHR;
struct SurfaceFormatKHR;

using PfnDestroySurfaceKHR = void(*VKAPI_PTR)(Instance instance, SurfaceKHR surface, const AllocationCallbacks* pAllocator);
using PfnGetPhysicalDeviceSurfaceSupportKHR = Result(*VKAPI_PTR)(PhysicalDevice physicalDevice, uint32_t queueFamilyIndex, SurfaceKHR surface, Bool32* pSupported);
using PfnGetPhysicalDeviceSurfaceCapabilitiesKHR = Result(*VKAPI_PTR)(PhysicalDevice physicalDevice, SurfaceKHR surface, SurfaceCapabilitiesKHR* pSurfaceCapabilities);
using PfnGetPhysicalDeviceSurfaceFormatsKHR = Result(*VKAPI_PTR)(PhysicalDevice physicalDevice, SurfaceKHR surface, uint32_t* pSurfaceFormatCount, SurfaceFormatKHR* pSurfaceFormats);
using PfnGetPhysicalDeviceSurfacePresentModesKHR = Result(*VKAPI_PTR)(PhysicalDevice physicalDevice, SurfaceKHR surface, uint32_t* pPresentModeCount, PresentModeKHR* pPresentModes);

VK_DEFINE_NON_DISPATCHABLE_HANDLE(SwapchainKHR)

using SwapchainCreateFlagsKHR = Flags<DummyEnum>;

struct SwapchainCreateInfoKHR;
struct PresentInfoKHR;

using PfnCreateSwapchainKHR = Result(*VKAPI_PTR)(Device device, const SwapchainCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, SwapchainKHR* pSwapchain);
using PfnDestroySwapchainKHR = void(*VKAPI_PTR)(Device device, SwapchainKHR swapchain, const AllocationCallbacks* pAllocator);
using PfnGetSwapchainImagesKHR = Result(*VKAPI_PTR)(Device device, SwapchainKHR swapchain, uint32_t* pSwapchainImageCount, Image* pSwapchainImages);
using PfnAcquireNextImageKHR = Result(*VKAPI_PTR)(Device device, SwapchainKHR swapchain, uint64_t timeout, Semaphore semaphore, Fence fence, uint32_t* pImageIndex);
using PfnQueuePresentKHR = Result(*VKAPI_PTR)(Queue queue, const PresentInfoKHR* pPresentInfo);

VK_DEFINE_NON_DISPATCHABLE_HANDLE(DisplayKHR)
VK_DEFINE_NON_DISPATCHABLE_HANDLE(DisplayModeKHR)

enum class DisplayPlaneAlphaBitsKHR : int32_t;

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

using PfnGetPhysicalDeviceDisplayPropertiesKHR = Result(*VKAPI_PTR)(PhysicalDevice physicalDevice, uint32_t* pPropertyCount, DisplayPropertiesKHR* pProperties);
using PfnGetPhysicalDeviceDisplayPlanePropertiesKHR = Result(*VKAPI_PTR)(PhysicalDevice physicalDevice, uint32_t* pPropertyCount, DisplayPlanePropertiesKHR* pProperties);
using PfnGetDisplayPlaneSupportedDisplaysKHR = Result(*VKAPI_PTR)(PhysicalDevice physicalDevice, uint32_t planeIndex, uint32_t* pDisplayCount, DisplayKHR* pDisplays);
using PfnGetDisplayModePropertiesKHR = Result(*VKAPI_PTR)(PhysicalDevice physicalDevice, DisplayKHR display, uint32_t* pPropertyCount, DisplayModePropertiesKHR* pProperties);
using PfnCreateDisplayModeKHR = Result(*VKAPI_PTR)(PhysicalDevice physicalDevice, DisplayKHR display, const DisplayModeCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, DisplayModeKHR* pMode);
using PfnGetDisplayPlaneCapabilitiesKHR = Result(*VKAPI_PTR)(PhysicalDevice physicalDevice, DisplayModeKHR mode, uint32_t planeIndex, DisplayPlaneCapabilitiesKHR* pCapabilities);
using PfnCreateDisplayPlaneSurfaceKHR = Result(*VKAPI_PTR)(Instance instance, const DisplaySurfaceCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceKHR* pSurface);

struct DisplayPresentInfoKHR;

using PfnCreateSharedSwapchainsKHR = Result(*VKAPI_PTR)(Device device, uint32_t swapchainCount, const SwapchainCreateInfoKHR* pCreateInfos, const AllocationCallbacks* pAllocator, SwapchainKHR* pSwapchains);

#ifdef VK_USE_PLATFORM_XLIB_KHR

using XlibSurfaceCreateFlagsKHR = Flags<DummyEnum>;

struct XlibSurfaceCreateInfoKHR;

using PfnCreateXlibSurfaceKHR = Result(*VKAPI_PTR)(Instance instance, const XlibSurfaceCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceKHR* pSurface);
using PfnGetPhysicalDeviceXlibPresentationSupportKHR = Bool32(*VKAPI_PTR)(PhysicalDevice physicalDevice, uint32_t queueFamilyIndex, Display* dpy, VisualID visualID);

#endif //VK_USE_PLATFORM_XLIB_KHR

#ifdef VK_USE_PLATFORM_XCB_KHR

using XcbSurfaceCreateFlagsKHR = Flags<DummyEnum>;

struct XcbSurfaceCreateInfoKHR;

using PfnCreateXcbSurfaceKHR = Result(*VKAPI_PTR)(Instance instance, const XcbSurfaceCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceKHR* pSurface);
using PfnGetPhysicalDeviceXcbPresentationSupportKHR = Bool32(*VKAPI_PTR)(PhysicalDevice physicalDevice, uint32_t queueFamilyIndex, xcb_connection_t* connection, xcb_visualid_t visual_id);

#endif //VK_USE_PLATFORM_XCB_KHR

#ifdef VK_USE_PLATFORM_WAYLAND_KHR

using WaylandSurfaceCreateFlagsKHR = Flags<DummyEnum>;

struct WaylandSurfaceCreateInfoKHR;

using PfnCreateWaylandSurfaceKHR = Result(*VKAPI_PTR)(Instance instance, const WaylandSurfaceCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceKHR* pSurface);
using PfnGetPhysicalDeviceWaylandPresentationSupportKHR = Bool32(*VKAPI_PTR)(PhysicalDevice physicalDevice, uint32_t queueFamilyIndex, wl_display* display);

#endif //VK_USE_PLATFORM_WAYLAND_KHR

#ifdef VK_USE_PLATFORM_MIR_KHR

using MirSurfaceCreateFlagsKHR = Flags<DummyEnum>;

struct MirSurfaceCreateInfoKHR;

using PfnCreateMirSurfaceKHR = Result(*VKAPI_PTR)(Instance instance, const MirSurfaceCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceKHR* pSurface);
using PfnGetPhysicalDeviceMirPresentationSupportKHR = Bool32(*VKAPI_PTR)(PhysicalDevice physicalDevice, uint32_t queueFamilyIndex, MirConnection* connection);

#endif //VK_USE_PLATFORM_MIR_KHR

#ifdef VK_USE_PLATFORM_ANDROID_KHR

using AndroidSurfaceCreateFlagsKHR = Flags<DummyEnum>;

struct AndroidSurfaceCreateInfoKHR;

using PfnCreateAndroidSurfaceKHR = Result(*VKAPI_PTR)(Instance instance, const AndroidSurfaceCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceKHR* pSurface);

#endif //VK_USE_PLATFORM_ANDROID_KHR

#ifdef VK_USE_PLATFORM_WIN32_KHR

using Win32SurfaceCreateFlagsKHR = Flags<DummyEnum>;

struct Win32SurfaceCreateInfoKHR;

using PfnCreateWin32SurfaceKHR = Result(*VKAPI_PTR)(Instance instance, const Win32SurfaceCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceKHR* pSurface);
using PfnGetPhysicalDeviceWin32PresentationSupportKHR = Bool32(*VKAPI_PTR)(PhysicalDevice physicalDevice, uint32_t queueFamilyIndex);

#endif //VK_USE_PLATFORM_WIN32_KHR

VK_DEFINE_NON_DISPATCHABLE_HANDLE(DebugReportCallbackEXT)

enum class DebugReportObjectTypeEXT : int32_t;
enum class DebugReportErrorEXT : int32_t;
enum class DebugReportBitsEXT : int32_t;

using DebugReportFlagsEXT = Flags<DebugReportBitsEXT>;

struct DebugReportCallbackCreateInfoEXT;

using PfnDebugReportCallbackEXT = Bool32(*VKAPI_PTR)(DebugReportFlagsEXT flags, DebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char* const pLayerPrefix, const char* pMessage, void* pUserData);

using PfnCreateDebugReportCallbackEXT = Result(*VKAPI_PTR)(Instance instance, const DebugReportCallbackCreateInfoEXT* pCreateInfo, const AllocationCallbacks* pAllocator, DebugReportCallbackEXT* pCallback);
using PfnDestroyDebugReportCallbackEXT = void(*VKAPI_PTR)(Instance instance, DebugReportCallbackEXT callback, const AllocationCallbacks* pAllocator);
using PfnDebugReportMessageEXT = void(*VKAPI_PTR)(Instance instance, DebugReportFlagsEXT flags, DebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char* pLayerPrefix, const char* pMessage);

enum class RasterizationOrderAMD : int32_t;

struct PipelineRasterizationStateRasterizationOrderAMD;

struct DebugMarkerObjectNameInfoEXT;
struct DebugMarkerObjectTagInfoEXT;
struct DebugMarkerMarkerInfoEXT;

using PfnDebugMarkerSetObjectTagEXT = Result(*VKAPI_PTR)(Device device, DebugMarkerObjectTagInfoEXT* pTagInfo);
using PfnDebugMarkerSetObjectNameEXT = Result(*VKAPI_PTR)(Device device, DebugMarkerObjectNameInfoEXT* pNameInfo);
using PfnCmdDebugMarkerBeginEXT = void(*VKAPI_PTR)(CommandBuffer commandBuffer, DebugMarkerMarkerInfoEXT* pMarkerInfo);
using PfnCmdDebugMarkerEndEXT = void(*VKAPI_PTR)(CommandBuffer commandBuffer);
using PfnCmdDebugMarkerInsertEXT = void(*VKAPI_PTR)(CommandBuffer commandBuffer, DebugMarkerMarkerInfoEXT* pMarkerInfo);

struct DedicatedAllocationImageCreateInfoNV;
struct DedicatedAllocationBufferCreateInfoNV;
struct DedicatedAllocationMemoryAllocateInfoNV;


} // namespace vk

// The specification (vk.xml) itself is published under the following license:

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
    

