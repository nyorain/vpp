// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

// Automaitcally generated vulkan header file for the nyorain/vpp library.
// Do not edit manually, rather edit the codegen files.

#pragma once

#include "flags.hpp"
#include "handle.hpp"

#include <vulkan/vk_platform.h>

namespace vk {

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
enum class DescriptorSetLayoutCreateBits : int32_t;
enum class DescriptorType : int32_t;
enum class DescriptorPoolCreateBits : int32_t;
enum class AttachmentDescriptionBits : int32_t;
enum class AttachmentLoadOp : int32_t;
enum class AttachmentStoreOp : int32_t;
enum class SubpassDescriptionBits : int32_t;
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
enum class ObjectType : int32_t;

using InstanceCreateFlags = nytl::Flags<DummyEnum>;
using FormatFeatureFlags = nytl::Flags<FormatFeatureBits>;
using ImageUsageFlags = nytl::Flags<ImageUsageBits>;
using ImageCreateFlags = nytl::Flags<ImageCreateBits>;
using SampleCountFlags = nytl::Flags<SampleCountBits>;
using QueueFlags = nytl::Flags<QueueBits>;
using MemoryPropertyFlags = nytl::Flags<MemoryPropertyBits>;
using MemoryHeapFlags = nytl::Flags<MemoryHeapBits>;
using DeviceCreateFlags = nytl::Flags<DummyEnum>;
using DeviceQueueCreateFlags = nytl::Flags<DummyEnum>;
using PipelineStageFlags = nytl::Flags<PipelineStageBits>;
using MemoryMapFlags = nytl::Flags<DummyEnum>;
using ImageAspectFlags = nytl::Flags<ImageAspectBits>;
using SparseImageFormatFlags = nytl::Flags<SparseImageFormatBits>;
using SparseMemoryBindFlags = nytl::Flags<SparseMemoryBindBits>;
using FenceCreateFlags = nytl::Flags<FenceCreateBits>;
using SemaphoreCreateFlags = nytl::Flags<DummyEnum>;
using EventCreateFlags = nytl::Flags<DummyEnum>;
using QueryPoolCreateFlags = nytl::Flags<DummyEnum>;
using QueryPipelineStatisticFlags = nytl::Flags<QueryPipelineStatisticBits>;
using QueryResultFlags = nytl::Flags<QueryResultBits>;
using BufferCreateFlags = nytl::Flags<BufferCreateBits>;
using BufferUsageFlags = nytl::Flags<BufferUsageBits>;
using BufferViewCreateFlags = nytl::Flags<DummyEnum>;
using ImageViewCreateFlags = nytl::Flags<DummyEnum>;
using ShaderModuleCreateFlags = nytl::Flags<DummyEnum>;
using PipelineCacheCreateFlags = nytl::Flags<DummyEnum>;
using PipelineCreateFlags = nytl::Flags<PipelineCreateBits>;
using PipelineShaderStageCreateFlags = nytl::Flags<DummyEnum>;
using PipelineVertexInputStateCreateFlags = nytl::Flags<DummyEnum>;
using PipelineInputAssemblyStateCreateFlags = nytl::Flags<DummyEnum>;
using PipelineTessellationStateCreateFlags = nytl::Flags<DummyEnum>;
using PipelineViewportStateCreateFlags = nytl::Flags<DummyEnum>;
using PipelineRasterizationStateCreateFlags = nytl::Flags<DummyEnum>;
using CullModeFlags = nytl::Flags<CullModeBits>;
using PipelineMultisampleStateCreateFlags = nytl::Flags<DummyEnum>;
using PipelineDepthStencilStateCreateFlags = nytl::Flags<DummyEnum>;
using PipelineColorBlendStateCreateFlags = nytl::Flags<DummyEnum>;
using ColorComponentFlags = nytl::Flags<ColorComponentBits>;
using PipelineDynamicStateCreateFlags = nytl::Flags<DummyEnum>;
using PipelineLayoutCreateFlags = nytl::Flags<DummyEnum>;
using ShaderStageFlags = nytl::Flags<ShaderStageBits>;
using SamplerCreateFlags = nytl::Flags<DummyEnum>;
using DescriptorSetLayoutCreateFlags = nytl::Flags<DescriptorSetLayoutCreateBits>;
using DescriptorPoolCreateFlags = nytl::Flags<DescriptorPoolCreateBits>;
using DescriptorPoolResetFlags = nytl::Flags<DummyEnum>;
using FramebufferCreateFlags = nytl::Flags<DummyEnum>;
using RenderPassCreateFlags = nytl::Flags<DummyEnum>;
using AttachmentDescriptionFlags = nytl::Flags<AttachmentDescriptionBits>;
using SubpassDescriptionFlags = nytl::Flags<SubpassDescriptionBits>;
using AccessFlags = nytl::Flags<AccessBits>;
using DependencyFlags = nytl::Flags<DependencyBits>;
using CommandPoolCreateFlags = nytl::Flags<CommandPoolCreateBits>;
using CommandPoolResetFlags = nytl::Flags<CommandPoolResetBits>;
using CommandBufferUsageFlags = nytl::Flags<CommandBufferUsageBits>;
using QueryControlFlags = nytl::Flags<QueryControlBits>;
using CommandBufferResetFlags = nytl::Flags<CommandBufferResetBits>;
using StencilFaceFlags = nytl::Flags<StencilFaceBits>;

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

using SurfaceTransformFlagsKHR = nytl::Flags<SurfaceTransformBitsKHR>;
using CompositeAlphaFlagsKHR = nytl::Flags<CompositeAlphaBitsKHR>;

struct SurfaceCapabilitiesKHR;
struct SurfaceFormatKHR;

using PfnDestroySurfaceKHR = void(*VKAPI_PTR)(Instance instance, SurfaceKHR surface, const AllocationCallbacks* pAllocator);
using PfnGetPhysicalDeviceSurfaceSupportKHR = Result(*VKAPI_PTR)(PhysicalDevice physicalDevice, uint32_t queueFamilyIndex, SurfaceKHR surface, Bool32* pSupported);
using PfnGetPhysicalDeviceSurfaceCapabilitiesKHR = Result(*VKAPI_PTR)(PhysicalDevice physicalDevice, SurfaceKHR surface, SurfaceCapabilitiesKHR* pSurfaceCapabilities);
using PfnGetPhysicalDeviceSurfaceFormatsKHR = Result(*VKAPI_PTR)(PhysicalDevice physicalDevice, SurfaceKHR surface, uint32_t* pSurfaceFormatCount, SurfaceFormatKHR* pSurfaceFormats);
using PfnGetPhysicalDeviceSurfacePresentModesKHR = Result(*VKAPI_PTR)(PhysicalDevice physicalDevice, SurfaceKHR surface, uint32_t* pPresentModeCount, PresentModeKHR* pPresentModes);

VK_DEFINE_NON_DISPATCHABLE_HANDLE(SwapchainKHR)

enum class SwapchainCreateBitsKHR : int32_t;

using SwapchainCreateFlagsKHR = nytl::Flags<SwapchainCreateBitsKHR>;

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

using DisplayPlaneAlphaFlagsKHR = nytl::Flags<DisplayPlaneAlphaBitsKHR>;
using DisplayModeCreateFlagsKHR = nytl::Flags<DummyEnum>;
using DisplaySurfaceCreateFlagsKHR = nytl::Flags<DummyEnum>;

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

using XlibSurfaceCreateFlagsKHR = nytl::Flags<DummyEnum>;

struct XlibSurfaceCreateInfoKHR;

using PfnCreateXlibSurfaceKHR = Result(*VKAPI_PTR)(Instance instance, const XlibSurfaceCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceKHR* pSurface);
using PfnGetPhysicalDeviceXlibPresentationSupportKHR = Bool32(*VKAPI_PTR)(PhysicalDevice physicalDevice, uint32_t queueFamilyIndex, Display* dpy, VisualID visualID);

#endif //VK_USE_PLATFORM_XLIB_KHR

#ifdef VK_USE_PLATFORM_XCB_KHR

using XcbSurfaceCreateFlagsKHR = nytl::Flags<DummyEnum>;

struct XcbSurfaceCreateInfoKHR;

using PfnCreateXcbSurfaceKHR = Result(*VKAPI_PTR)(Instance instance, const XcbSurfaceCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceKHR* pSurface);
using PfnGetPhysicalDeviceXcbPresentationSupportKHR = Bool32(*VKAPI_PTR)(PhysicalDevice physicalDevice, uint32_t queueFamilyIndex, xcb_connection_t* connection, xcb_visualid_t visual_id);

#endif //VK_USE_PLATFORM_XCB_KHR

#ifdef VK_USE_PLATFORM_WAYLAND_KHR

using WaylandSurfaceCreateFlagsKHR = nytl::Flags<DummyEnum>;

struct WaylandSurfaceCreateInfoKHR;

using PfnCreateWaylandSurfaceKHR = Result(*VKAPI_PTR)(Instance instance, const WaylandSurfaceCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceKHR* pSurface);
using PfnGetPhysicalDeviceWaylandPresentationSupportKHR = Bool32(*VKAPI_PTR)(PhysicalDevice physicalDevice, uint32_t queueFamilyIndex, wl_display* display);

#endif //VK_USE_PLATFORM_WAYLAND_KHR

#ifdef VK_USE_PLATFORM_MIR_KHR

using MirSurfaceCreateFlagsKHR = nytl::Flags<DummyEnum>;

struct MirSurfaceCreateInfoKHR;

using PfnCreateMirSurfaceKHR = Result(*VKAPI_PTR)(Instance instance, const MirSurfaceCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceKHR* pSurface);
using PfnGetPhysicalDeviceMirPresentationSupportKHR = Bool32(*VKAPI_PTR)(PhysicalDevice physicalDevice, uint32_t queueFamilyIndex, MirConnection* connection);

#endif //VK_USE_PLATFORM_MIR_KHR

#ifdef VK_USE_PLATFORM_ANDROID_KHR

using AndroidSurfaceCreateFlagsKHR = nytl::Flags<DummyEnum>;

struct AndroidSurfaceCreateInfoKHR;

using PfnCreateAndroidSurfaceKHR = Result(*VKAPI_PTR)(Instance instance, const AndroidSurfaceCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceKHR* pSurface);

#endif //VK_USE_PLATFORM_ANDROID_KHR

#ifdef VK_USE_PLATFORM_WIN32_KHR

using Win32SurfaceCreateFlagsKHR = nytl::Flags<DummyEnum>;

struct Win32SurfaceCreateInfoKHR;

using PfnCreateWin32SurfaceKHR = Result(*VKAPI_PTR)(Instance instance, const Win32SurfaceCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceKHR* pSurface);
using PfnGetPhysicalDeviceWin32PresentationSupportKHR = Bool32(*VKAPI_PTR)(PhysicalDevice physicalDevice, uint32_t queueFamilyIndex);

#endif //VK_USE_PLATFORM_WIN32_KHR

VK_DEFINE_NON_DISPATCHABLE_HANDLE(DebugReportCallbackEXT)

enum class DebugReportObjectTypeEXT : int32_t;
enum class DebugReportBitsEXT : int32_t;

using DebugReportFlagsEXT = nytl::Flags<DebugReportBitsEXT>;

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

using PfnDebugMarkerSetObjectTagEXT = Result(*VKAPI_PTR)(Device device, const DebugMarkerObjectTagInfoEXT* pTagInfo);
using PfnDebugMarkerSetObjectNameEXT = Result(*VKAPI_PTR)(Device device, const DebugMarkerObjectNameInfoEXT* pNameInfo);
using PfnCmdDebugMarkerBeginEXT = void(*VKAPI_PTR)(CommandBuffer commandBuffer, const DebugMarkerMarkerInfoEXT* pMarkerInfo);
using PfnCmdDebugMarkerEndEXT = void(*VKAPI_PTR)(CommandBuffer commandBuffer);
using PfnCmdDebugMarkerInsertEXT = void(*VKAPI_PTR)(CommandBuffer commandBuffer, const DebugMarkerMarkerInfoEXT* pMarkerInfo);

struct DedicatedAllocationImageCreateInfoNV;
struct DedicatedAllocationBufferCreateInfoNV;
struct DedicatedAllocationMemoryAllocateInfoNV;

using PfnCmdDrawIndirectCountAMD = void(*VKAPI_PTR)(CommandBuffer commandBuffer, Buffer buffer, DeviceSize offset, Buffer countBuffer, DeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride);
using PfnCmdDrawIndexedIndirectCountAMD = void(*VKAPI_PTR)(CommandBuffer commandBuffer, Buffer buffer, DeviceSize offset, Buffer countBuffer, DeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride);

struct TextureLODGatherFormatPropertiesAMD;

struct RenderPassMultiviewCreateInfoKHX;
struct PhysicalDeviceMultiviewFeaturesKHX;
struct PhysicalDeviceMultiviewPropertiesKHX;

enum class ExternalMemoryHandleTypeBitsNV : int32_t;
enum class ExternalMemoryFeatureBitsNV : int32_t;

using ExternalMemoryHandleTypeFlagsNV = nytl::Flags<ExternalMemoryHandleTypeBitsNV>;
using ExternalMemoryFeatureFlagsNV = nytl::Flags<ExternalMemoryFeatureBitsNV>;

struct ExternalImageFormatPropertiesNV;

using PfnGetPhysicalDeviceExternalImageFormatPropertiesNV = Result(*VKAPI_PTR)(PhysicalDevice physicalDevice, Format format, ImageType type, ImageTiling tiling, ImageUsageFlags usage, ImageCreateFlags flags, ExternalMemoryHandleTypeFlagsNV externalHandleType, ExternalImageFormatPropertiesNV* pExternalImageFormatProperties);

struct ExternalMemoryImageCreateInfoNV;
struct ExportMemoryAllocateInfoNV;

#ifdef VK_USE_PLATFORM_WIN32_KHR

struct ImportMemoryWin32HandleInfoNV;
struct ExportMemoryWin32HandleInfoNV;

using PfnGetMemoryWin32HandleNV = Result(*VKAPI_PTR)(Device device, DeviceMemory memory, ExternalMemoryHandleTypeFlagsNV handleType, HANDLE* pHandle);

#endif //VK_USE_PLATFORM_WIN32_KHR

#ifdef VK_USE_PLATFORM_WIN32_KHR

struct Win32KeyedMutexAcquireReleaseInfoNV;

#endif //VK_USE_PLATFORM_WIN32_KHR

struct PhysicalDeviceFeatures2KHR;
struct PhysicalDeviceProperties2KHR;
struct FormatProperties2KHR;
struct ImageFormatProperties2KHR;
struct PhysicalDeviceImageFormatInfo2KHR;
struct QueueFamilyProperties2KHR;
struct PhysicalDeviceMemoryProperties2KHR;
struct SparseImageFormatProperties2KHR;
struct PhysicalDeviceSparseImageFormatInfo2KHR;

using PfnGetPhysicalDeviceFeatures2KHR = void(*VKAPI_PTR)(PhysicalDevice physicalDevice, PhysicalDeviceFeatures2KHR* pFeatures);
using PfnGetPhysicalDeviceProperties2KHR = void(*VKAPI_PTR)(PhysicalDevice physicalDevice, PhysicalDeviceProperties2KHR* pProperties);
using PfnGetPhysicalDeviceFormatProperties2KHR = void(*VKAPI_PTR)(PhysicalDevice physicalDevice, Format format, FormatProperties2KHR* pFormatProperties);
using PfnGetPhysicalDeviceImageFormatProperties2KHR = Result(*VKAPI_PTR)(PhysicalDevice physicalDevice, const PhysicalDeviceImageFormatInfo2KHR* pImageFormatInfo, ImageFormatProperties2KHR* pImageFormatProperties);
using PfnGetPhysicalDeviceQueueFamilyProperties2KHR = void(*VKAPI_PTR)(PhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, QueueFamilyProperties2KHR* pQueueFamilyProperties);
using PfnGetPhysicalDeviceMemoryProperties2KHR = void(*VKAPI_PTR)(PhysicalDevice physicalDevice, PhysicalDeviceMemoryProperties2KHR* pMemoryProperties);
using PfnGetPhysicalDeviceSparseImageFormatProperties2KHR = void(*VKAPI_PTR)(PhysicalDevice physicalDevice, const PhysicalDeviceSparseImageFormatInfo2KHR* pFormatInfo, uint32_t* pPropertyCount, SparseImageFormatProperties2KHR* pProperties);

constexpr auto maxDeviceGroupSizeKhx = 32;

enum class PeerMemoryFeatureBitsKHX : int32_t;
enum class MemoryAllocateBitsKHX : int32_t;
enum class DeviceGroupPresentModeBitsKHX : int32_t;

using PeerMemoryFeatureFlagsKHX = nytl::Flags<PeerMemoryFeatureBitsKHX>;
using MemoryAllocateFlagsKHX = nytl::Flags<MemoryAllocateBitsKHX>;
using DeviceGroupPresentModeFlagsKHX = nytl::Flags<DeviceGroupPresentModeBitsKHX>;

struct MemoryAllocateFlagsInfoKHX;
struct DeviceGroupRenderPassBeginInfoKHX;
struct DeviceGroupCommandBufferBeginInfoKHX;
struct DeviceGroupSubmitInfoKHX;
struct DeviceGroupBindSparseInfoKHX;
struct BindBufferMemoryDeviceGroupInfoKHX;
struct BindImageMemoryDeviceGroupInfoKHX;
struct DeviceGroupPresentCapabilitiesKHX;
struct ImageSwapchainCreateInfoKHX;
struct BindImageMemorySwapchainInfoKHX;
struct AcquireNextImageInfoKHX;
struct DeviceGroupPresentInfoKHX;
struct DeviceGroupSwapchainCreateInfoKHX;

using PfnGetDeviceGroupPeerMemoryFeaturesKHX = void(*VKAPI_PTR)(Device device, uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, PeerMemoryFeatureFlagsKHX* pPeerMemoryFeatures);
using PfnCmdSetDeviceMaskKHX = void(*VKAPI_PTR)(CommandBuffer commandBuffer, uint32_t deviceMask);
using PfnCmdDispatchBaseKHX = void(*VKAPI_PTR)(CommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ);
using PfnGetDeviceGroupPresentCapabilitiesKHX = Result(*VKAPI_PTR)(Device device, DeviceGroupPresentCapabilitiesKHX* pDeviceGroupPresentCapabilities);
using PfnGetDeviceGroupSurfacePresentModesKHX = Result(*VKAPI_PTR)(Device device, SurfaceKHR surface, DeviceGroupPresentModeFlagsKHX* pModes);
using PfnGetPhysicalDevicePresentRectanglesKHX = Result(*VKAPI_PTR)(PhysicalDevice physicalDevice, SurfaceKHR surface, uint32_t* pRectCount, Rect2D* pRects);
using PfnAcquireNextImage2KHX = Result(*VKAPI_PTR)(Device device, const AcquireNextImageInfoKHX* pAcquireInfo, uint32_t* pImageIndex);

enum class ValidationCheckEXT : int32_t;

struct ValidationFlagsEXT;

#ifdef VK_USE_PLATFORM_VI_NN

using ViSurfaceCreateFlagsNN = nytl::Flags<DummyEnum>;

struct ViSurfaceCreateInfoNN;

using PfnCreateViSurfaceNN = Result(*VKAPI_PTR)(Instance instance, const ViSurfaceCreateInfoNN* pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceKHR* pSurface);

#endif //VK_USE_PLATFORM_VI_NN

using CommandPoolTrimFlagsKHR = nytl::Flags<DummyEnum>;

using PfnTrimCommandPoolKHR = void(*VKAPI_PTR)(Device device, CommandPool commandPool, CommandPoolTrimFlagsKHR flags);

struct PhysicalDeviceGroupPropertiesKHX;
struct DeviceGroupDeviceCreateInfoKHX;

using PfnEnumeratePhysicalDeviceGroupsKHX = Result(*VKAPI_PTR)(Instance instance, uint32_t* pPhysicalDeviceGroupCount, PhysicalDeviceGroupPropertiesKHX* pPhysicalDeviceGroupProperties);

constexpr auto luidSizeKhr = 8;

enum class ExternalMemoryHandleTypeBitsKHR : int32_t;
enum class ExternalMemoryFeatureBitsKHR : int32_t;

using ExternalMemoryHandleTypeFlagsKHR = nytl::Flags<ExternalMemoryHandleTypeBitsKHR>;
using ExternalMemoryFeatureFlagsKHR = nytl::Flags<ExternalMemoryFeatureBitsKHR>;

struct ExternalMemoryPropertiesKHR;
struct PhysicalDeviceExternalImageFormatInfoKHR;
struct ExternalImageFormatPropertiesKHR;
struct PhysicalDeviceExternalBufferInfoKHR;
struct ExternalBufferPropertiesKHR;
struct PhysicalDeviceIDPropertiesKHR;

using PfnGetPhysicalDeviceExternalBufferPropertiesKHR = void(*VKAPI_PTR)(PhysicalDevice physicalDevice, const PhysicalDeviceExternalBufferInfoKHR* pExternalBufferInfo, ExternalBufferPropertiesKHR* pExternalBufferProperties);

constexpr auto queueFamilyExternalKhr = (~0U-1);

struct ExternalMemoryImageCreateInfoKHR;
struct ExternalMemoryBufferCreateInfoKHR;
struct ExportMemoryAllocateInfoKHR;

#ifdef VK_USE_PLATFORM_WIN32_KHR

struct ImportMemoryWin32HandleInfoKHR;
struct ExportMemoryWin32HandleInfoKHR;
struct MemoryWin32HandlePropertiesKHR;
struct MemoryGetWin32HandleInfoKHR;

using PfnGetMemoryWin32HandleKHR = Result(*VKAPI_PTR)(Device device, const MemoryGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle);
using PfnGetMemoryWin32HandlePropertiesKHR = Result(*VKAPI_PTR)(Device device, ExternalMemoryHandleTypeBitsKHR handleType, HANDLE handle, MemoryWin32HandlePropertiesKHR* pMemoryWin32HandleProperties);

#endif //VK_USE_PLATFORM_WIN32_KHR

struct ImportMemoryFdInfoKHR;
struct MemoryFdPropertiesKHR;
struct MemoryGetFdInfoKHR;

using PfnGetMemoryFdKHR = Result(*VKAPI_PTR)(Device device, const MemoryGetFdInfoKHR* pGetFdInfo, int* pFd);
using PfnGetMemoryFdPropertiesKHR = Result(*VKAPI_PTR)(Device device, ExternalMemoryHandleTypeBitsKHR handleType, int fd, MemoryFdPropertiesKHR* pMemoryFdProperties);

#ifdef VK_USE_PLATFORM_WIN32_KHR

struct Win32KeyedMutexAcquireReleaseInfoKHR;

#endif //VK_USE_PLATFORM_WIN32_KHR

enum class ExternalSemaphoreHandleTypeBitsKHR : int32_t;
enum class ExternalSemaphoreFeatureBitsKHR : int32_t;

using ExternalSemaphoreHandleTypeFlagsKHR = nytl::Flags<ExternalSemaphoreHandleTypeBitsKHR>;
using ExternalSemaphoreFeatureFlagsKHR = nytl::Flags<ExternalSemaphoreFeatureBitsKHR>;

struct PhysicalDeviceExternalSemaphoreInfoKHR;
struct ExternalSemaphorePropertiesKHR;

using PfnGetPhysicalDeviceExternalSemaphorePropertiesKHR = void(*VKAPI_PTR)(PhysicalDevice physicalDevice, const PhysicalDeviceExternalSemaphoreInfoKHR* pExternalSemaphoreInfo, ExternalSemaphorePropertiesKHR* pExternalSemaphoreProperties);

enum class SemaphoreImportBitsKHR : int32_t;

using SemaphoreImportFlagsKHR = nytl::Flags<SemaphoreImportBitsKHR>;

struct ExportSemaphoreCreateInfoKHR;

#ifdef VK_USE_PLATFORM_WIN32_KHR

struct ImportSemaphoreWin32HandleInfoKHR;
struct ExportSemaphoreWin32HandleInfoKHR;
struct D3D12FenceSubmitInfoKHR;
struct SemaphoreGetWin32HandleInfoKHR;

using PfnImportSemaphoreWin32HandleKHR = Result(*VKAPI_PTR)(Device device, const ImportSemaphoreWin32HandleInfoKHR* pImportSemaphoreWin32HandleInfo);
using PfnGetSemaphoreWin32HandleKHR = Result(*VKAPI_PTR)(Device device, const SemaphoreGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle);

#endif //VK_USE_PLATFORM_WIN32_KHR

struct ImportSemaphoreFdInfoKHR;
struct SemaphoreGetFdInfoKHR;

using PfnImportSemaphoreFdKHR = Result(*VKAPI_PTR)(Device device, const ImportSemaphoreFdInfoKHR* pImportSemaphoreFdInfo);
using PfnGetSemaphoreFdKHR = Result(*VKAPI_PTR)(Device device, const SemaphoreGetFdInfoKHR* pGetFdInfo, int* pFd);

struct PhysicalDevicePushDescriptorPropertiesKHR;

using PfnCmdPushDescriptorSetKHR = void(*VKAPI_PTR)(CommandBuffer commandBuffer, PipelineBindPoint pipelineBindPoint, PipelineLayout layout, uint32_t set, uint32_t descriptorWriteCount, const WriteDescriptorSet* pDescriptorWrites);

struct PhysicalDevice16BitStorageFeaturesKHR;

struct RectLayerKHR;
struct PresentRegionKHR;
struct PresentRegionsKHR;

VK_DEFINE_NON_DISPATCHABLE_HANDLE(DescriptorUpdateTemplateKHR)

enum class DescriptorUpdateTemplateTypeKHR : int32_t;

using DescriptorUpdateTemplateCreateFlagsKHR = nytl::Flags<DummyEnum>;

struct DescriptorUpdateTemplateEntryKHR;
struct DescriptorUpdateTemplateCreateInfoKHR;

using PfnCreateDescriptorUpdateTemplateKHR = Result(*VKAPI_PTR)(Device device, const DescriptorUpdateTemplateCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, DescriptorUpdateTemplateKHR* pDescriptorUpdateTemplate);
using PfnDestroyDescriptorUpdateTemplateKHR = void(*VKAPI_PTR)(Device device, DescriptorUpdateTemplateKHR descriptorUpdateTemplate, const AllocationCallbacks* pAllocator);
using PfnUpdateDescriptorSetWithTemplateKHR = void(*VKAPI_PTR)(Device device, DescriptorSet descriptorSet, DescriptorUpdateTemplateKHR descriptorUpdateTemplate, const void* pData);
using PfnCmdPushDescriptorSetWithTemplateKHR = void(*VKAPI_PTR)(CommandBuffer commandBuffer, DescriptorUpdateTemplateKHR descriptorUpdateTemplate, PipelineLayout layout, uint32_t set, const void* pData);

VK_DEFINE_NON_DISPATCHABLE_HANDLE(ObjectTableNVX)
VK_DEFINE_NON_DISPATCHABLE_HANDLE(IndirectCommandsLayoutNVX)

enum class IndirectCommandsLayoutUsageBitsNVX : int32_t;
enum class ObjectEntryUsageBitsNVX : int32_t;
enum class IndirectCommandsTokenTypeNVX : int32_t;
enum class ObjectEntryTypeNVX : int32_t;

using IndirectCommandsLayoutUsageFlagsNVX = nytl::Flags<IndirectCommandsLayoutUsageBitsNVX>;
using ObjectEntryUsageFlagsNVX = nytl::Flags<ObjectEntryUsageBitsNVX>;

struct DeviceGeneratedCommandsFeaturesNVX;
struct DeviceGeneratedCommandsLimitsNVX;
struct IndirectCommandsTokenNVX;
struct IndirectCommandsLayoutTokenNVX;
struct IndirectCommandsLayoutCreateInfoNVX;
struct CmdProcessCommandsInfoNVX;
struct CmdReserveSpaceForCommandsInfoNVX;
struct ObjectTableCreateInfoNVX;
struct ObjectTableEntryNVX;
struct ObjectTablePipelineEntryNVX;
struct ObjectTableDescriptorSetEntryNVX;
struct ObjectTableVertexBufferEntryNVX;
struct ObjectTableIndexBufferEntryNVX;
struct ObjectTablePushConstantEntryNVX;

using PfnCmdProcessCommandsNVX = void(*VKAPI_PTR)(CommandBuffer commandBuffer, const CmdProcessCommandsInfoNVX* pProcessCommandsInfo);
using PfnCmdReserveSpaceForCommandsNVX = void(*VKAPI_PTR)(CommandBuffer commandBuffer, const CmdReserveSpaceForCommandsInfoNVX* pReserveSpaceInfo);
using PfnCreateIndirectCommandsLayoutNVX = Result(*VKAPI_PTR)(Device device, const IndirectCommandsLayoutCreateInfoNVX* pCreateInfo, const AllocationCallbacks* pAllocator, IndirectCommandsLayoutNVX* pIndirectCommandsLayout);
using PfnDestroyIndirectCommandsLayoutNVX = void(*VKAPI_PTR)(Device device, IndirectCommandsLayoutNVX indirectCommandsLayout, const AllocationCallbacks* pAllocator);
using PfnCreateObjectTableNVX = Result(*VKAPI_PTR)(Device device, const ObjectTableCreateInfoNVX* pCreateInfo, const AllocationCallbacks* pAllocator, ObjectTableNVX* pObjectTable);
using PfnDestroyObjectTableNVX = void(*VKAPI_PTR)(Device device, ObjectTableNVX objectTable, const AllocationCallbacks* pAllocator);
using PfnRegisterObjectsNVX = Result(*VKAPI_PTR)(Device device, ObjectTableNVX objectTable, uint32_t objectCount, const ObjectTableEntryNVX* const* ppObjectTableEntries, const uint32_t* pObjectIndices);
using PfnUnregisterObjectsNVX = Result(*VKAPI_PTR)(Device device, ObjectTableNVX objectTable, uint32_t objectCount, const ObjectEntryTypeNVX* pObjectEntryTypes, const uint32_t* pObjectIndices);
using PfnGetPhysicalDeviceGeneratedCommandsPropertiesNVX = void(*VKAPI_PTR)(PhysicalDevice physicalDevice, DeviceGeneratedCommandsFeaturesNVX* pFeatures, DeviceGeneratedCommandsLimitsNVX* pLimits);

struct ViewportWScalingNV;
struct PipelineViewportWScalingStateCreateInfoNV;

using PfnCmdSetViewportWScalingNV = void(*VKAPI_PTR)(CommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const ViewportWScalingNV* pViewportWScalings);

using PfnReleaseDisplayEXT = Result(*VKAPI_PTR)(PhysicalDevice physicalDevice, DisplayKHR display);

#ifdef VK_USE_PLATFORM_XLIB_XRANDR_EXT

using PfnAcquireXlibDisplayEXT = Result(*VKAPI_PTR)(PhysicalDevice physicalDevice, Display* dpy, DisplayKHR display);
using PfnGetRandROutputDisplayEXT = Result(*VKAPI_PTR)(PhysicalDevice physicalDevice, Display* dpy, RROutput rrOutput, DisplayKHR* pDisplay);

#endif //VK_USE_PLATFORM_XLIB_XRANDR_EXT

enum class SurfaceCounterBitsEXT : int32_t;

using SurfaceCounterFlagsEXT = nytl::Flags<SurfaceCounterBitsEXT>;

struct SurfaceCapabilities2EXT;

using PfnGetPhysicalDeviceSurfaceCapabilities2EXT = Result(*VKAPI_PTR)(PhysicalDevice physicalDevice, SurfaceKHR surface, SurfaceCapabilities2EXT* pSurfaceCapabilities);

enum class DisplayPowerStateEXT : int32_t;
enum class DeviceEventTypeEXT : int32_t;
enum class DisplayEventTypeEXT : int32_t;

struct DisplayPowerInfoEXT;
struct DeviceEventInfoEXT;
struct DisplayEventInfoEXT;
struct SwapchainCounterCreateInfoEXT;

using PfnDisplayPowerControlEXT = Result(*VKAPI_PTR)(Device device, DisplayKHR display, const DisplayPowerInfoEXT* pDisplayPowerInfo);
using PfnRegisterDeviceEventEXT = Result(*VKAPI_PTR)(Device device, const DeviceEventInfoEXT* pDeviceEventInfo, const AllocationCallbacks* pAllocator, Fence* pFence);
using PfnRegisterDisplayEventEXT = Result(*VKAPI_PTR)(Device device, DisplayKHR display, const DisplayEventInfoEXT* pDisplayEventInfo, const AllocationCallbacks* pAllocator, Fence* pFence);
using PfnGetSwapchainCounterEXT = Result(*VKAPI_PTR)(Device device, SwapchainKHR swapchain, SurfaceCounterBitsEXT counter, uint64_t* pCounterValue);

struct RefreshCycleDurationGOOGLE;
struct PastPresentationTimingGOOGLE;
struct PresentTimeGOOGLE;
struct PresentTimesInfoGOOGLE;

using PfnGetRefreshCycleDurationGOOGLE = Result(*VKAPI_PTR)(Device device, SwapchainKHR swapchain, RefreshCycleDurationGOOGLE* pDisplayTimingProperties);
using PfnGetPastPresentationTimingGOOGLE = Result(*VKAPI_PTR)(Device device, SwapchainKHR swapchain, uint32_t* pPresentationTimingCount, PastPresentationTimingGOOGLE* pPresentationTimings);

struct PhysicalDeviceMultiviewPerViewAttributesPropertiesNVX;

enum class ViewportCoordinateSwizzleNV : int32_t;

using PipelineViewportSwizzleStateCreateFlagsNV = nytl::Flags<DummyEnum>;

struct ViewportSwizzleNV;
struct PipelineViewportSwizzleStateCreateInfoNV;

enum class DiscardRectangleModeEXT : int32_t;

using PipelineDiscardRectangleStateCreateFlagsEXT = nytl::Flags<DummyEnum>;

struct PhysicalDeviceDiscardRectanglePropertiesEXT;
struct PipelineDiscardRectangleStateCreateInfoEXT;

using PfnCmdSetDiscardRectangleEXT = void(*VKAPI_PTR)(CommandBuffer commandBuffer, uint32_t firstDiscardRectangle, uint32_t discardRectangleCount, const Rect2D* pDiscardRectangles);

struct XYColorEXT;
struct HdrMetadataEXT;

using PfnSetHdrMetadataEXT = void(*VKAPI_PTR)(Device device, uint32_t swapchainCount, const SwapchainKHR* pSwapchains, const HdrMetadataEXT* pMetadata);

struct SharedPresentSurfaceCapabilitiesKHR;

using PfnGetSwapchainStatusKHR = Result(*VKAPI_PTR)(Device device, SwapchainKHR swapchain);

enum class ExternalFenceHandleTypeBitsKHR : int32_t;
enum class ExternalFenceFeatureBitsKHR : int32_t;

using ExternalFenceHandleTypeFlagsKHR = nytl::Flags<ExternalFenceHandleTypeBitsKHR>;
using ExternalFenceFeatureFlagsKHR = nytl::Flags<ExternalFenceFeatureBitsKHR>;

struct PhysicalDeviceExternalFenceInfoKHR;
struct ExternalFencePropertiesKHR;

using PfnGetPhysicalDeviceExternalFencePropertiesKHR = void(*VKAPI_PTR)(PhysicalDevice physicalDevice, const PhysicalDeviceExternalFenceInfoKHR* pExternalFenceInfo, ExternalFencePropertiesKHR* pExternalFenceProperties);

enum class FenceImportBitsKHR : int32_t;

using FenceImportFlagsKHR = nytl::Flags<FenceImportBitsKHR>;

struct ExportFenceCreateInfoKHR;

#ifdef VK_USE_PLATFORM_WIN32_KHR

struct ImportFenceWin32HandleInfoKHR;
struct ExportFenceWin32HandleInfoKHR;
struct FenceGetWin32HandleInfoKHR;

using PfnImportFenceWin32HandleKHR = Result(*VKAPI_PTR)(Device device, const ImportFenceWin32HandleInfoKHR* pImportFenceWin32HandleInfo);
using PfnGetFenceWin32HandleKHR = Result(*VKAPI_PTR)(Device device, const FenceGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle);

#endif //VK_USE_PLATFORM_WIN32_KHR

struct ImportFenceFdInfoKHR;
struct FenceGetFdInfoKHR;

using PfnImportFenceFdKHR = Result(*VKAPI_PTR)(Device device, const ImportFenceFdInfoKHR* pImportFenceFdInfo);
using PfnGetFenceFdKHR = Result(*VKAPI_PTR)(Device device, const FenceGetFdInfoKHR* pGetFdInfo, int* pFd);

enum class PointClippingBehaviorKHR : int32_t;
enum class TessellationDomainOriginKHR : int32_t;

struct PhysicalDevicePointClippingPropertiesKHR;
struct InputAttachmentAspectReferenceKHR;
struct RenderPassInputAttachmentAspectCreateInfoKHR;
struct ImageViewUsageCreateInfoKHR;
struct PipelineTessellationDomainOriginStateCreateInfoKHR;

struct PhysicalDeviceSurfaceInfo2KHR;
struct SurfaceCapabilities2KHR;
struct SurfaceFormat2KHR;

using PfnGetPhysicalDeviceSurfaceCapabilities2KHR = Result(*VKAPI_PTR)(PhysicalDevice physicalDevice, const PhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, SurfaceCapabilities2KHR* pSurfaceCapabilities);
using PfnGetPhysicalDeviceSurfaceFormats2KHR = Result(*VKAPI_PTR)(PhysicalDevice physicalDevice, const PhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, uint32_t* pSurfaceFormatCount, SurfaceFormat2KHR* pSurfaceFormats);

struct PhysicalDeviceVariablePointerFeaturesKHR;

#ifdef VK_USE_PLATFORM_IOS_MVK

using IOSSurfaceCreateFlagsMVK = nytl::Flags<DummyEnum>;

struct IOSSurfaceCreateInfoMVK;

using PfnCreateIOSSurfaceMVK = Result(*VKAPI_PTR)(Instance instance, const IOSSurfaceCreateInfoMVK* pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceKHR* pSurface);

#endif //VK_USE_PLATFORM_IOS_MVK

#ifdef VK_USE_PLATFORM_MACOS_MVK

using MacOSSurfaceCreateFlagsMVK = nytl::Flags<DummyEnum>;

struct MacOSSurfaceCreateInfoMVK;

using PfnCreateMacOSSurfaceMVK = Result(*VKAPI_PTR)(Instance instance, const MacOSSurfaceCreateInfoMVK* pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceKHR* pSurface);

#endif //VK_USE_PLATFORM_MACOS_MVK

struct MemoryDedicatedRequirementsKHR;
struct MemoryDedicatedAllocateInfoKHR;

enum class SamplerReductionModeEXT : int32_t;

struct SamplerReductionModeCreateInfoEXT;
struct PhysicalDeviceSamplerFilterMinmaxPropertiesEXT;

struct SampleLocationEXT;
struct SampleLocationsInfoEXT;
struct AttachmentSampleLocationsEXT;
struct SubpassSampleLocationsEXT;
struct RenderPassSampleLocationsBeginInfoEXT;
struct PipelineSampleLocationsStateCreateInfoEXT;
struct PhysicalDeviceSampleLocationsPropertiesEXT;
struct MultisamplePropertiesEXT;

using PfnCmdSetSampleLocationsEXT = void(*VKAPI_PTR)(CommandBuffer commandBuffer, const SampleLocationsInfoEXT* pSampleLocationsInfo);
using PfnGetPhysicalDeviceMultisamplePropertiesEXT = void(*VKAPI_PTR)(PhysicalDevice physicalDevice, SampleCountBits samples, MultisamplePropertiesEXT* pMultisampleProperties);

struct BufferMemoryRequirementsInfo2KHR;
struct ImageMemoryRequirementsInfo2KHR;
struct ImageSparseMemoryRequirementsInfo2KHR;
struct MemoryRequirements2KHR;
struct SparseImageMemoryRequirements2KHR;

using PfnGetImageMemoryRequirements2KHR = void(*VKAPI_PTR)(Device device, const ImageMemoryRequirementsInfo2KHR* pInfo, MemoryRequirements2KHR* pMemoryRequirements);
using PfnGetBufferMemoryRequirements2KHR = void(*VKAPI_PTR)(Device device, const BufferMemoryRequirementsInfo2KHR* pInfo, MemoryRequirements2KHR* pMemoryRequirements);
using PfnGetImageSparseMemoryRequirements2KHR = void(*VKAPI_PTR)(Device device, const ImageSparseMemoryRequirementsInfo2KHR* pInfo, uint32_t* pSparseMemoryRequirementCount, SparseImageMemoryRequirements2KHR* pSparseMemoryRequirements);

struct ImageFormatListCreateInfoKHR;

enum class BlendOverlapEXT : int32_t;

struct PhysicalDeviceBlendOperationAdvancedFeaturesEXT;
struct PhysicalDeviceBlendOperationAdvancedPropertiesEXT;
struct PipelineColorBlendAdvancedStateCreateInfoEXT;

using PipelineCoverageToColorStateCreateFlagsNV = nytl::Flags<DummyEnum>;

struct PipelineCoverageToColorStateCreateInfoNV;

enum class CoverageModulationModeNV : int32_t;

using PipelineCoverageModulationStateCreateFlagsNV = nytl::Flags<DummyEnum>;

struct PipelineCoverageModulationStateCreateInfoNV;

VK_DEFINE_NON_DISPATCHABLE_HANDLE(SamplerYcbcrConversionKHR)

enum class SamplerYcbcrModelConversionKHR : int32_t;
enum class SamplerYcbcrRangeKHR : int32_t;
enum class ChromaLocationKHR : int32_t;

struct SamplerYcbcrConversionCreateInfoKHR;
struct SamplerYcbcrConversionInfoKHR;
struct BindImagePlaneMemoryInfoKHR;
struct ImagePlaneMemoryRequirementsInfoKHR;
struct PhysicalDeviceSamplerYcbcrConversionFeaturesKHR;
struct SamplerYcbcrConversionImageFormatPropertiesKHR;

using PfnCreateSamplerYcbcrConversionKHR = Result(*VKAPI_PTR)(Device device, const SamplerYcbcrConversionCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, SamplerYcbcrConversionKHR* pYcbcrConversion);
using PfnDestroySamplerYcbcrConversionKHR = void(*VKAPI_PTR)(Device device, SamplerYcbcrConversionKHR ycbcrConversion, const AllocationCallbacks* pAllocator);

struct BindBufferMemoryInfoKHR;
struct BindImageMemoryInfoKHR;

using PfnBindBufferMemory2KHR = Result(*VKAPI_PTR)(Device device, uint32_t bindInfoCount, const BindBufferMemoryInfoKHR* pBindInfos);
using PfnBindImageMemory2KHR = Result(*VKAPI_PTR)(Device device, uint32_t bindInfoCount, const BindImageMemoryInfoKHR* pBindInfos);

VK_DEFINE_NON_DISPATCHABLE_HANDLE(ValidationCacheEXT)

enum class ValidationCacheHeaderVersionEXT : int32_t;

using ValidationCacheCreateFlagsEXT = nytl::Flags<DummyEnum>;

struct ValidationCacheCreateInfoEXT;
struct ShaderModuleValidationCacheCreateInfoEXT;

using PfnCreateValidationCacheEXT = Result(*VKAPI_PTR)(Device device, const ValidationCacheCreateInfoEXT* pCreateInfo, const AllocationCallbacks* pAllocator, ValidationCacheEXT* pValidationCache);
using PfnDestroyValidationCacheEXT = void(*VKAPI_PTR)(Device device, ValidationCacheEXT validationCache, const AllocationCallbacks* pAllocator);
using PfnMergeValidationCachesEXT = Result(*VKAPI_PTR)(Device device, ValidationCacheEXT dstCache, uint32_t srcCacheCount, const ValidationCacheEXT* pSrcCaches);
using PfnGetValidationCacheDataEXT = Result(*VKAPI_PTR)(Device device, ValidationCacheEXT validationCache, size_t* pDataSize, void* pData);


} // namespace vk

// The specification (vk.xml) itself is published under the following license:

// Copyright (c) 2015-2017 The Khronos Group Inc.
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// 
// ------------------------------------------------------------------------
// 
// This file, vk.xml, is the Vulkan API Registry. It is a critically important
// and normative part of the Vulkan Specification, including a canonical
// machine-readable definition of the API, parameter and member validation
// language incorporated into the Specification and reference pages, and other
// material which is registered by Khronos, such as tags used by extension and
// layer authors. The authoritative public version of vk.xml is maintained in
// the master branch of the Khronos Vulkan GitHub project. The authoritative
// private version is maintained in the 1.0 branch of the member gitlab server.
    

