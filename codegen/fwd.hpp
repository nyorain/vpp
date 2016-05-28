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

// The implementation for the Flag tempalte class was taken from nvidias vkcpp project:
// Changes were made to it, which are protected by the above given license.
// link (May 2016): https://github.com/nvpro-pipeline/vkcpp
// license:

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

//The specification (vk.xml) itself is protected by the following license:

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
    
//Automaitcally generated forward header file for the vulkan api for the nyorain/vpp library
//Conatains forward declarations for all vulkan types. Do not edit manually.

#pragma once

#include <vulkan/vulkan.h>
#include <cstdint>

namespace vk
{

enum class DummyEnum {};

template <typename BitType, typename MaskType = std::uint32_t>
class Flags
{
public:
	Flags() = default;
	Flags(BitType bit) : m_mask(static_cast<MaskType>(bit)) {}
	Flags(const Flags<BitType>& rhs) : m_mask(rhs.m_mask) {}

	Flags<BitType> & operator=(const Flags<BitType>& rhs)
	{
		m_mask = rhs.m_mask;
		return *this;
	}

	Flags<BitType> & operator|=(const Flags<BitType>& rhs)
	{
		m_mask |= rhs.m_mask;
		return *this;
	}

    Flags<BitType> & operator&=(const Flags<BitType>& rhs)
    {
    	m_mask &= rhs.m_mask;
    	return *this;
    }

    Flags<BitType> & operator^=(const Flags<BitType>& rhs)
    {
    	m_mask ^= rhs.m_mask;
    	return *this;
    }

    Flags<BitType> operator|(const Flags<BitType>& rhs) const
    {
    	Flags<BitType> result(*this);
    	result |= rhs;
    	return result;
    }

    Flags<BitType> operator&(const Flags<BitType>& rhs) const
    {
    	Flags<BitType> result(*this);
    	result &= rhs;
    	return result;
    }

    Flags<BitType> operator^(const Flags<BitType>& rhs) const
    {
    	Flags<BitType> result(*this);
    	result ^= rhs;
    	return result;
    }

    operator bool() const { return (m_mask); }
    bool operator!() const { return !(m_mask); }
    bool operator==(const Flags<BitType>& rhs) const { return m_mask == rhs.m_mask; }
    bool operator!=(Flags<BitType> const& rhs) const { return m_mask != rhs.m_mask; }

    explicit operator MaskType() const { return m_mask; } //explicit?

	private:
    	MaskType  m_mask;
};

template <typename BitType>
Flags<BitType> operator|(BitType bit, Flags<BitType> const& flags)
{
	return flags | bit;
}

template <typename BitType>
Flags<BitType> operator&(BitType bit, Flags<BitType> const& flags)
{
	return flags & bit;
}

template <typename BitType>
Flags<BitType> operator^(BitType bit, Flags<BitType> const& flags)
{
	return flags ^ bit;
}

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

using SurfaceKHR = VkSurfaceKHR;

enum class SurfaceTransformBitsKHR : std::int32_t;
enum class CompositeAlphaBitsKHR : std::int32_t;
enum class ColorSpaceKHR : std::int32_t;
enum class PresentModeKHR : std::int32_t;

using SurfaceTransformFlagsKHR = Flags<SurfaceTransformBitsKHR>;
using CompositeAlphaFlagsKHR = Flags<CompositeAlphaBitsKHR>;

struct SurfaceCapabilitiesKHR;
struct SurfaceFormatKHR;

using SwapchainKHR = VkSwapchainKHR;

using SwapchainCreateFlagsKHR = Flags<DummyEnum>;

struct SwapchainCreateInfoKHR;
struct PresentInfoKHR;

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

struct DisplayPresentInfoKHR;

#ifdef VK_USE_PLATFORM_XLIB_KHR

using XlibSurfaceCreateFlagsKHR = Flags<DummyEnum>;

struct XlibSurfaceCreateInfoKHR;

#endif //VK_USE_PLATFORM_XLIB_KHR

#ifdef VK_USE_PLATFORM_XCB_KHR

using XcbSurfaceCreateFlagsKHR = Flags<DummyEnum>;

struct XcbSurfaceCreateInfoKHR;

#endif //VK_USE_PLATFORM_XCB_KHR

#ifdef VK_USE_PLATFORM_WAYLAND_KHR

using WaylandSurfaceCreateFlagsKHR = Flags<DummyEnum>;

struct WaylandSurfaceCreateInfoKHR;

#endif //VK_USE_PLATFORM_WAYLAND_KHR

#ifdef VK_USE_PLATFORM_MIR_KHR

using MirSurfaceCreateFlagsKHR = Flags<DummyEnum>;

struct MirSurfaceCreateInfoKHR;

#endif //VK_USE_PLATFORM_MIR_KHR

#ifdef VK_USE_PLATFORM_ANDROID_KHR

using AndroidSurfaceCreateFlagsKHR = Flags<DummyEnum>;

struct AndroidSurfaceCreateInfoKHR;

#endif //VK_USE_PLATFORM_ANDROID_KHR

#ifdef VK_USE_PLATFORM_WIN32_KHR

using Win32SurfaceCreateFlagsKHR = Flags<DummyEnum>;

struct Win32SurfaceCreateInfoKHR;

#endif //VK_USE_PLATFORM_WIN32_KHR

using DebugReportCallbackEXT = VkDebugReportCallbackEXT;

enum class DebugReportObjectTypeEXT : std::int32_t;
enum class DebugReportErrorEXT : std::int32_t;
enum class DebugReportBitsEXT : std::int32_t;

using DebugReportFlagsEXT = Flags<DebugReportBitsEXT>;

struct DebugReportCallbackCreateInfoEXT;

enum class RasterizationOrderAMD : std::int32_t;

struct PipelineRasterizationStateRasterizationOrderAMD;

struct DebugMarkerObjectNameInfoEXT;
struct DebugMarkerObjectTagInfoEXT;
struct DebugMarkerMarkerInfoEXT;


}