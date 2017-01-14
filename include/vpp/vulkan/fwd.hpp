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
using DescriptorSetLayoutCreateFlags = nytl::Flags<DummyEnum>;
using DescriptorPoolCreateFlags = nytl::Flags<DescriptorPoolCreateBits>;
using DescriptorPoolResetFlags = nytl::Flags<DummyEnum>;
using FramebufferCreateFlags = nytl::Flags<DummyEnum>;
using RenderPassCreateFlags = nytl::Flags<DummyEnum>;
using AttachmentDescriptionFlags = nytl::Flags<AttachmentDescriptionBits>;
using SubpassDescriptionFlags = nytl::Flags<DummyEnum>;
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

constexpr auto khrSurfaceSpecVersion = 25;
constexpr auto khrSurfaceExtensionName = "VK_KHR_surface";

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

constexpr auto khrSwapchainSpecVersion = 68;
constexpr auto khrSwapchainExtensionName = "VK_KHR_swapchain";

VK_DEFINE_NON_DISPATCHABLE_HANDLE(SwapchainKHR)

using SwapchainCreateFlagsKHR = nytl::Flags<DummyEnum>;

struct SwapchainCreateInfoKHR;
struct PresentInfoKHR;

using PfnCreateSwapchainKHR = Result(*VKAPI_PTR)(Device device, const SwapchainCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, SwapchainKHR* pSwapchain);
using PfnDestroySwapchainKHR = void(*VKAPI_PTR)(Device device, SwapchainKHR swapchain, const AllocationCallbacks* pAllocator);
using PfnGetSwapchainImagesKHR = Result(*VKAPI_PTR)(Device device, SwapchainKHR swapchain, uint32_t* pSwapchainImageCount, Image* pSwapchainImages);
using PfnAcquireNextImageKHR = Result(*VKAPI_PTR)(Device device, SwapchainKHR swapchain, uint64_t timeout, Semaphore semaphore, Fence fence, uint32_t* pImageIndex);
using PfnQueuePresentKHR = Result(*VKAPI_PTR)(Queue queue, const PresentInfoKHR* pPresentInfo);

constexpr auto khrDisplaySpecVersion = 21;
constexpr auto khrDisplayExtensionName = "VK_KHR_display";

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

constexpr auto khrDisplaySwapchainSpecVersion = 9;
constexpr auto khrDisplaySwapchainExtensionName = "VK_KHR_display_swapchain";

struct DisplayPresentInfoKHR;

using PfnCreateSharedSwapchainsKHR = Result(*VKAPI_PTR)(Device device, uint32_t swapchainCount, const SwapchainCreateInfoKHR* pCreateInfos, const AllocationCallbacks* pAllocator, SwapchainKHR* pSwapchains);

#ifdef VK_USE_PLATFORM_XLIB_KHR

constexpr auto khrXlibSurfaceSpecVersion = 6;
constexpr auto khrXlibSurfaceExtensionName = "VK_KHR_xlib_surface";

using XlibSurfaceCreateFlagsKHR = nytl::Flags<DummyEnum>;

struct XlibSurfaceCreateInfoKHR;

using PfnCreateXlibSurfaceKHR = Result(*VKAPI_PTR)(Instance instance, const XlibSurfaceCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceKHR* pSurface);
using PfnGetPhysicalDeviceXlibPresentationSupportKHR = Bool32(*VKAPI_PTR)(PhysicalDevice physicalDevice, uint32_t queueFamilyIndex, Display* dpy, VisualID visualID);

#endif //VK_USE_PLATFORM_XLIB_KHR

#ifdef VK_USE_PLATFORM_XCB_KHR

constexpr auto khrXcbSurfaceSpecVersion = 6;
constexpr auto khrXcbSurfaceExtensionName = "VK_KHR_xcb_surface";

using XcbSurfaceCreateFlagsKHR = nytl::Flags<DummyEnum>;

struct XcbSurfaceCreateInfoKHR;

using PfnCreateXcbSurfaceKHR = Result(*VKAPI_PTR)(Instance instance, const XcbSurfaceCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceKHR* pSurface);
using PfnGetPhysicalDeviceXcbPresentationSupportKHR = Bool32(*VKAPI_PTR)(PhysicalDevice physicalDevice, uint32_t queueFamilyIndex, xcb_connection_t* connection, xcb_visualid_t visual_id);

#endif //VK_USE_PLATFORM_XCB_KHR

#ifdef VK_USE_PLATFORM_WAYLAND_KHR

constexpr auto khrWaylandSurfaceSpecVersion = 5;
constexpr auto khrWaylandSurfaceExtensionName = "VK_KHR_wayland_surface";

using WaylandSurfaceCreateFlagsKHR = nytl::Flags<DummyEnum>;

struct WaylandSurfaceCreateInfoKHR;

using PfnCreateWaylandSurfaceKHR = Result(*VKAPI_PTR)(Instance instance, const WaylandSurfaceCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceKHR* pSurface);
using PfnGetPhysicalDeviceWaylandPresentationSupportKHR = Bool32(*VKAPI_PTR)(PhysicalDevice physicalDevice, uint32_t queueFamilyIndex, wl_display* display);

#endif //VK_USE_PLATFORM_WAYLAND_KHR

#ifdef VK_USE_PLATFORM_MIR_KHR

constexpr auto khrMirSurfaceSpecVersion = 4;
constexpr auto khrMirSurfaceExtensionName = "VK_KHR_mir_surface";

using MirSurfaceCreateFlagsKHR = nytl::Flags<DummyEnum>;

struct MirSurfaceCreateInfoKHR;

using PfnCreateMirSurfaceKHR = Result(*VKAPI_PTR)(Instance instance, const MirSurfaceCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceKHR* pSurface);
using PfnGetPhysicalDeviceMirPresentationSupportKHR = Bool32(*VKAPI_PTR)(PhysicalDevice physicalDevice, uint32_t queueFamilyIndex, MirConnection* connection);

#endif //VK_USE_PLATFORM_MIR_KHR

#ifdef VK_USE_PLATFORM_ANDROID_KHR

constexpr auto khrAndroidSurfaceSpecVersion = 6;
constexpr auto khrAndroidSurfaceExtensionName = "VK_KHR_android_surface";

using AndroidSurfaceCreateFlagsKHR = nytl::Flags<DummyEnum>;

struct AndroidSurfaceCreateInfoKHR;

using PfnCreateAndroidSurfaceKHR = Result(*VKAPI_PTR)(Instance instance, const AndroidSurfaceCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceKHR* pSurface);

#endif //VK_USE_PLATFORM_ANDROID_KHR

#ifdef VK_USE_PLATFORM_WIN32_KHR

constexpr auto khrWin32SurfaceSpecVersion = 5;
constexpr auto khrWin32SurfaceExtensionName = "VK_KHR_win32_surface";

using Win32SurfaceCreateFlagsKHR = nytl::Flags<DummyEnum>;

struct Win32SurfaceCreateInfoKHR;

using PfnCreateWin32SurfaceKHR = Result(*VKAPI_PTR)(Instance instance, const Win32SurfaceCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceKHR* pSurface);
using PfnGetPhysicalDeviceWin32PresentationSupportKHR = Bool32(*VKAPI_PTR)(PhysicalDevice physicalDevice, uint32_t queueFamilyIndex);

#endif //VK_USE_PLATFORM_WIN32_KHR

constexpr auto androidNativeBufferSpecVersion = 4;
constexpr auto androidNativeBufferNumber = 11;
constexpr auto androidNativeBufferName = "VK_ANDROID_native_buffer";

constexpr auto extDebugReportSpecVersion = 4;
constexpr auto extDebugReportExtensionName = "VK_EXT_debug_report";

VK_DEFINE_NON_DISPATCHABLE_HANDLE(DebugReportCallbackEXT)

enum class DebugReportObjectTypeEXT : int32_t;
enum class DebugReportErrorEXT : int32_t;
enum class DebugReportBitsEXT : int32_t;

using DebugReportFlagsEXT = nytl::Flags<DebugReportBitsEXT>;

struct DebugReportCallbackCreateInfoEXT;

using PfnDebugReportCallbackEXT = Bool32(*VKAPI_PTR)(DebugReportFlagsEXT flags, DebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char* pLayerPrefix, const char* pMessage, void* pUserData);
using PfnCreateDebugReportCallbackEXT = Result(*VKAPI_PTR)(Instance instance, const DebugReportCallbackCreateInfoEXT* pCreateInfo, const AllocationCallbacks* pAllocator, DebugReportCallbackEXT* pCallback);
using PfnDestroyDebugReportCallbackEXT = void(*VKAPI_PTR)(Instance instance, DebugReportCallbackEXT callback, const AllocationCallbacks* pAllocator);
using PfnDebugReportMessageEXT = void(*VKAPI_PTR)(Instance instance, DebugReportFlagsEXT flags, DebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char* pLayerPrefix, const char* pMessage);

constexpr auto nvGlslShaderSpecVersion = 1;
constexpr auto nvGlslShaderExtensionName = "VK_NV_glsl_shader";

constexpr auto nvExtension1SpecVersion = 0;
constexpr auto nvExtension1ExtensionName = "VK_NV_extension_1";

constexpr auto khrSamplerMirrorClampToEdgeSpecVersion = 1;
constexpr auto khrSamplerMirrorClampToEdgeExtensionName = "VK_KHR_sampler_mirror_clamp_to_edge";

constexpr auto imgFilterCubicSpecVersion = 1;
constexpr auto imgFilterCubicExtensionName = "VK_IMG_filter_cubic";

constexpr auto amdExtension17SpecVersion = 0;
constexpr auto amdExtension17ExtensionName = "VK_AMD_extension_17";

constexpr auto amdExtension18SpecVersion = 0;
constexpr auto amdExtension18ExtensionName = "VK_AMD_extension_18";

constexpr auto amdRasterizationOrderSpecVersion = 1;
constexpr auto amdRasterizationOrderExtensionName = "VK_AMD_rasterization_order";

enum class RasterizationOrderAMD : int32_t;

struct PipelineRasterizationStateRasterizationOrderAMD;

constexpr auto amdExtension20SpecVersion = 0;
constexpr auto amdExtension20ExtensionName = "VK_AMD_extension_20";

constexpr auto amdShaderTrinaryMinmaxSpecVersion = 1;
constexpr auto amdShaderTrinaryMinmaxExtensionName = "VK_AMD_shader_trinary_minmax";

constexpr auto amdShaderExplicitVertexParameterSpecVersion = 1;
constexpr auto amdShaderExplicitVertexParameterExtensionName = "VK_AMD_shader_explicit_vertex_parameter";

constexpr auto extDebugMarkerSpecVersion = 3;
constexpr auto extDebugMarkerExtensionName = "VK_EXT_debug_marker";

struct DebugMarkerObjectNameInfoEXT;
struct DebugMarkerObjectTagInfoEXT;
struct DebugMarkerMarkerInfoEXT;

using PfnDebugMarkerSetObjectTagEXT = Result(*VKAPI_PTR)(Device device, DebugMarkerObjectTagInfoEXT* pTagInfo);
using PfnDebugMarkerSetObjectNameEXT = Result(*VKAPI_PTR)(Device device, DebugMarkerObjectNameInfoEXT* pNameInfo);
using PfnCmdDebugMarkerBeginEXT = void(*VKAPI_PTR)(CommandBuffer commandBuffer, DebugMarkerMarkerInfoEXT* pMarkerInfo);
using PfnCmdDebugMarkerEndEXT = void(*VKAPI_PTR)(CommandBuffer commandBuffer);
using PfnCmdDebugMarkerInsertEXT = void(*VKAPI_PTR)(CommandBuffer commandBuffer, DebugMarkerMarkerInfoEXT* pMarkerInfo);

constexpr auto amdExtension24SpecVersion = 0;
constexpr auto amdExtension24ExtensionName = "VK_AMD_extension_24";

constexpr auto amdExtension25SpecVersion = 0;
constexpr auto amdExtension25ExtensionName = "VK_AMD_extension_25";

constexpr auto amdGcnShaderSpecVersion = 1;
constexpr auto amdGcnShaderExtensionName = "VK_AMD_gcn_shader";

constexpr auto nvDedicatedAllocationSpecVersion = 1;
constexpr auto nvDedicatedAllocationExtensionName = "VK_NV_dedicated_allocation";

struct DedicatedAllocationImageCreateInfoNV;
struct DedicatedAllocationBufferCreateInfoNV;
struct DedicatedAllocationMemoryAllocateInfoNV;

constexpr auto extExtension28SpecVersion = 0;
constexpr auto extExtension28ExtensionName = "VK_NV_extension_28";

constexpr auto nvxExtension29SpecVersion = 0;
constexpr auto nvxExtension29ExtensionName = "VK_NVX_extension_29";

constexpr auto nvxExtension30SpecVersion = 0;
constexpr auto nvxExtension30ExtensionName = "VK_NVX_extension_30";

constexpr auto nvxExtension31SpecVersion = 0;
constexpr auto nvxExtension31ExtensionName = "VK_NVX_extension_31";

constexpr auto amdExtension32SpecVersion = 0;
constexpr auto amdExtension32ExtensionName = "VK_AMD_extension_32";

constexpr auto amdExtension33SpecVersion = 0;
constexpr auto amdExtension33ExtensionName = "VK_AMD_extension_33";

constexpr auto amdDrawIndirectCountSpecVersion = 1;
constexpr auto amdDrawIndirectCountExtensionName = "VK_AMD_draw_indirect_count";

using PfnCmdDrawIndirectCountAMD = void(*VKAPI_PTR)(CommandBuffer commandBuffer, Buffer buffer, DeviceSize offset, Buffer countBuffer, DeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride);
using PfnCmdDrawIndexedIndirectCountAMD = void(*VKAPI_PTR)(CommandBuffer commandBuffer, Buffer buffer, DeviceSize offset, Buffer countBuffer, DeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride);

constexpr auto amdExtension35SpecVersion = 0;
constexpr auto amdExtension35ExtensionName = "VK_AMD_extension_35";

constexpr auto amdNegativeViewportHeightSpecVersion = 1;
constexpr auto amdNegativeViewportHeightExtensionName = "VK_AMD_negative_viewport_height";

constexpr auto amdGpuShaderHalfFloatSpecVersion = 1;
constexpr auto amdGpuShaderHalfFloatExtensionName = "VK_AMD_gpu_shader_half_float";

constexpr auto amdShaderBallotSpecVersion = 1;
constexpr auto amdShaderBallotExtensionName = "VK_AMD_shader_ballot";

constexpr auto amdExtension39SpecVersion = 0;
constexpr auto amdExtension39ExtensionName = "VK_AMD_extension_39";

constexpr auto amdExtension40SpecVersion = 0;
constexpr auto amdExtension40ExtensionName = "VK_AMD_extension_40";

constexpr auto amdExtension41SpecVersion = 0;
constexpr auto amdExtension41ExtensionName = "VK_AMD_extension_41";

constexpr auto amdExtension42SpecVersion = 0;
constexpr auto amdExtension42ExtensionName = "VK_AMD_extension_42";

constexpr auto amdExtension43SpecVersion = 0;
constexpr auto amdExtension43ExtensionName = "VK_AMD_extension_43";

constexpr auto amdExtension44SpecVersion = 0;
constexpr auto amdExtension44ExtensionName = "VK_AMD_extension_44";

constexpr auto amdExtension45SpecVersion = 0;
constexpr auto amdExtension45ExtensionName = "VK_AMD_extension_45";

constexpr auto amdExtension46SpecVersion = 0;
constexpr auto amdExtension46ExtensionName = "VK_AMD_extension_46";

constexpr auto amdExtension47SpecVersion = 0;
constexpr auto amdExtension47ExtensionName = "VK_AMD_extension_47";

constexpr auto nvxExtension48SpecVersion = 0;
constexpr auto nvxExtension48ExtensionName = "VK_NVX_extension_48";

constexpr auto googleExtension49SpecVersion = 0;
constexpr auto googleExtension49ExtensionName = "VK_GOOGLE_extension_49";

constexpr auto googleExtension50SpecVersion = 0;
constexpr auto googleExtension50ExtensionName = "VK_GOOGLE_extension_50";

constexpr auto nvxExtension51SpecVersion = 0;
constexpr auto nvxExtension51ExtensionName = "VK_NVX_extension_51";

constexpr auto nvxExtension52SpecVersion = 0;
constexpr auto nvxExtension52ExtensionName = "VK_NVX_extension_52";

constexpr auto nvExtension53SpecVersion = 0;
constexpr auto nvExtension53ExtensionName = "VK_NV_extension_53";

constexpr auto nvExtension54SpecVersion = 0;
constexpr auto nvExtension54ExtensionName = "VK_NV_extension_54";

constexpr auto imgFormatPvrtcSpecVersion = 1;
constexpr auto imgFormatPvrtcExtensionName = "VK_IMG_format_pvrtc";

constexpr auto nvExternalMemoryCapabilitiesSpecVersion = 1;
constexpr auto nvExternalMemoryCapabilitiesExtensionName = "VK_NV_external_memory_capabilities";

enum class ExternalMemoryHandleTypeBitsNV : int32_t;
enum class ExternalMemoryFeatureBitsNV : int32_t;

using ExternalMemoryHandleTypeFlagsNV = nytl::Flags<ExternalMemoryHandleTypeBitsNV>;
using ExternalMemoryFeatureFlagsNV = nytl::Flags<ExternalMemoryFeatureBitsNV>;

struct ExternalImageFormatPropertiesNV;

using PfnGetPhysicalDeviceExternalImageFormatPropertiesNV = Result(*VKAPI_PTR)(PhysicalDevice physicalDevice, Format format, ImageType type, ImageTiling tiling, ImageUsageFlags usage, ImageCreateFlags flags, ExternalMemoryHandleTypeFlagsNV externalHandleType, ExternalImageFormatPropertiesNV* pExternalImageFormatProperties);

constexpr auto nvExternalMemorySpecVersion = 1;
constexpr auto nvExternalMemoryExtensionName = "VK_NV_external_memory";

struct ExternalMemoryImageCreateInfoNV;
struct ExportMemoryAllocateInfoNV;

#ifdef VK_USE_PLATFORM_WIN32_KHR

constexpr auto nvExternalMemoryWin32SpecVersion = 1;
constexpr auto nvExternalMemoryWin32ExtensionName = "VK_NV_external_memory_win32";

struct ImportMemoryWin32HandleInfoNV;
struct ExportMemoryWin32HandleInfoNV;

using PfnGetMemoryWin32HandleNV = Result(*VKAPI_PTR)(Device device, DeviceMemory memory, ExternalMemoryHandleTypeFlagsNV handleType, HANDLE* pHandle);

#endif //VK_USE_PLATFORM_WIN32_KHR

#ifdef VK_USE_PLATFORM_WIN32_KHR

constexpr auto nvWin32KeyedMutexSpecVersion = 1;
constexpr auto nvWin32KeyedMutexExtensionName = "VK_NV_win32_keyed_mutex";

struct Win32KeyedMutexAcquireReleaseInfoNV;

#endif //VK_USE_PLATFORM_WIN32_KHR

constexpr auto khrExtension60SpecVersion = 0;
constexpr auto khrExtension60ExtensionName = "VK_KHR_extension_60";

constexpr auto khrExtension61SpecVersion = 0;
constexpr auto khrExtension61ExtensionName = "VK_KHR_extension_61";

constexpr auto extValidationFlagsSpecVersion = 1;
constexpr auto extValidationFlagsExtensionName = "VK_EXT_validation_flags";

enum class ValidationCheckEXT : int32_t;

struct ValidationFlagsEXT;

constexpr auto nvExtension63SpecVersion = 0;
constexpr auto nvExtension63ExtensionName = "VK_NV_extension_63";

constexpr auto khrExtension64SpecVersion = 0;
constexpr auto khrExtension64ExtensionName = "VK_KHR_extension_64";

constexpr auto nvExtension65SpecVersion = 0;
constexpr auto nvExtension65ExtensionName = "VK_NV_extension_65";

constexpr auto nvExtension66SpecVersion = 0;
constexpr auto nvExtension66ExtensionName = "VK_NV_extension_66";

constexpr auto armExtension01SpecVersion = 0;
constexpr auto armExtension01ExtensionName = "VK_ARM_extension_01";

constexpr auto armExtension02SpecVersion = 0;
constexpr auto armExtension02ExtensionName = "VK_ARM_extension_02";

constexpr auto imgExtension69SpecVersion = 0;
constexpr auto imgExtension69ExtensionName = "VK_IMG_extension_69";

constexpr auto khrExtension70SpecVersion = 0;
constexpr auto khrExtension70ExtensionName = "VK_KHR_extension_70";

constexpr auto khrExtension71SpecVersion = 0;
constexpr auto khrExtension71ExtensionName = "VK_KHR_extension_71";

constexpr auto khrExtension72SpecVersion = 0;
constexpr auto khrExtension72ExtensionName = "VK_KHR_extension_72";

constexpr auto khrExtension73SpecVersion = 0;
constexpr auto khrExtension73ExtensionName = "VK_KHR_extension_73";

constexpr auto khrExtension74SpecVersion = 0;
constexpr auto khrExtension74ExtensionName = "VK_KHR_extension_74";

constexpr auto khrExtension75SpecVersion = 0;
constexpr auto khrExtension75ExtensionName = "VK_KHR_extension_75";

constexpr auto khrExtension76SpecVersion = 0;
constexpr auto khrExtension76ExtensionName = "VK_KHR_extension_76";

constexpr auto khrExtension77SpecVersion = 0;
constexpr auto khrExtension77ExtensionName = "VK_KHR_extension_77";

constexpr auto khrExtension78SpecVersion = 0;
constexpr auto khrExtension78ExtensionName = "VK_KHR_extension_78";

constexpr auto khrExtension79SpecVersion = 0;
constexpr auto khrExtension79ExtensionName = "VK_KHR_extension_79";

constexpr auto khrExtension80SpecVersion = 0;
constexpr auto khrExtension80ExtensionName = "VK_KHR_extension_80";

constexpr auto khrExtension81SpecVersion = 0;
constexpr auto khrExtension81ExtensionName = "VK_KHR_extension_81";

constexpr auto khrExtension82SpecVersion = 0;
constexpr auto khrExtension82ExtensionName = "VK_KHR_extension_82";

constexpr auto khrExtension83SpecVersion = 0;
constexpr auto khrExtension83ExtensionName = "VK_KHR_extension_83";

constexpr auto khrExtension84SpecVersion = 0;
constexpr auto khrExtension84ExtensionName = "VK_KHR_extension_84";

constexpr auto khrExtension85SpecVersion = 0;
constexpr auto khrExtension85ExtensionName = "VK_KHR_extension_85";

constexpr auto khrExtension86SpecVersion = 0;
constexpr auto khrExtension86ExtensionName = "VK_KHR_extension_86";

constexpr auto nvxDeviceGeneratedCommandsSpecVersion = 1;
constexpr auto nvxDeviceGeneratedCommandsExtensionName = "VK_NVX_device_generated_commands";

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
using PfnRegisterObjectsNVX = Result(*VKAPI_PTR)(Device device, ObjectTableNVX objectTable, uint32_t objectCount, const ObjectTableEntryNVX** ppObjectTableEntries, const uint32_t* pObjectIndices);
using PfnUnregisterObjectsNVX = Result(*VKAPI_PTR)(Device device, ObjectTableNVX objectTable, uint32_t objectCount, const ObjectEntryTypeNVX* pObjectEntryTypes, const uint32_t* pObjectIndices);
using PfnGetPhysicalDeviceGeneratedCommandsPropertiesNVX = void(*VKAPI_PTR)(PhysicalDevice physicalDevice, DeviceGeneratedCommandsFeaturesNVX* pFeatures, DeviceGeneratedCommandsLimitsNVX* pLimits);

constexpr auto khrExtension88SpecVersion = 0;
constexpr auto khrExtension88ExtensionName = "VK_KHR_extension_88";

constexpr auto extExtension89SpecVersion = 0;
constexpr auto extExtension89ExtensionName = "VK_EXT_extension_89";

constexpr auto extExtension90SpecVersion = 0;
constexpr auto extExtension90ExtensionName = "VK_EXT_extension_90";

constexpr auto extExtension91SpecVersion = 0;
constexpr auto extExtension91ExtensionName = "VK_EXT_extension_91";

constexpr auto extExtension92SpecVersion = 0;
constexpr auto extExtension92ExtensionName = "VK_EXT_extension_92";

constexpr auto googleExtension93SpecVersion = 0;
constexpr auto googleExtension93ExtensionName = "VK_GOOGLE_extension_93";

constexpr auto khrExtension94SpecVersion = 0;
constexpr auto khrExtension94ExtensionName = "VK_KHR_extension_94";

constexpr auto nvExtension95SpecVersion = 0;
constexpr auto nvExtension95ExtensionName = "VK_NV_extension_95";

constexpr auto nvExtension96SpecVersion = 0;
constexpr auto nvExtension96ExtensionName = "VK_NV_extension_96";

constexpr auto nvExtension97SpecVersion = 0;
constexpr auto nvExtension97ExtensionName = "VK_NV_extension_97";

constexpr auto nvExtension98SpecVersion = 0;
constexpr auto nvExtension98ExtensionName = "VK_NV_extension_98";

constexpr auto nvExtension99SpecVersion = 0;
constexpr auto nvExtension99ExtensionName = "VK_NV_extension_99";

constexpr auto nvExtension100SpecVersion = 0;
constexpr auto nvExtension100ExtensionName = "VK_NV_extension_100";

constexpr auto nvExtension101SpecVersion = 0;
constexpr auto nvExtension101ExtensionName = "VK_NV_extension_101";

constexpr auto nvExtension102SpecVersion = 0;
constexpr auto nvExtension102ExtensionName = "VK_NV_extension_102";

constexpr auto nvExtension103SpecVersion = 0;
constexpr auto nvExtension103ExtensionName = "VK_NV_extension_103";

constexpr auto nvExtension104SpecVersion = 0;
constexpr auto nvExtension104ExtensionName = "VK_NV_extension_104";

constexpr auto khrExtension105SpecVersion = 0;
constexpr auto khrExtension105ExtensionName = "VK_EXT_extension_105";



} // namespace vk

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
// maintained in the master branch of the Khronos Vulkan GitHub project.
	

