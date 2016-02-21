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

namespace vk
{
  enum class FramebufferCreateFlagBits
  {
  };

  typedef Flags<FramebufferCreateFlagBits, VkFramebufferCreateFlags> FramebufferCreateFlags;

  inline FramebufferCreateFlags operator|( FramebufferCreateFlagBits bit0, FramebufferCreateFlagBits bit1 )
  {
    return FramebufferCreateFlags( bit0 ) | bit1;
  }

  enum class QueryPoolCreateFlagBits
  {
  };

  typedef Flags<QueryPoolCreateFlagBits, VkQueryPoolCreateFlags> QueryPoolCreateFlags;

  inline QueryPoolCreateFlags operator|( QueryPoolCreateFlagBits bit0, QueryPoolCreateFlagBits bit1 )
  {
    return QueryPoolCreateFlags( bit0 ) | bit1;
  }

  enum class RenderPassCreateFlagBits
  {
  };

  typedef Flags<RenderPassCreateFlagBits, VkRenderPassCreateFlags> RenderPassCreateFlags;

  inline RenderPassCreateFlags operator|( RenderPassCreateFlagBits bit0, RenderPassCreateFlagBits bit1 )
  {
    return RenderPassCreateFlags( bit0 ) | bit1;
  }

  enum class SamplerCreateFlagBits
  {
  };

  typedef Flags<SamplerCreateFlagBits, VkSamplerCreateFlags> SamplerCreateFlags;

  inline SamplerCreateFlags operator|( SamplerCreateFlagBits bit0, SamplerCreateFlagBits bit1 )
  {
    return SamplerCreateFlags( bit0 ) | bit1;
  }

  enum class PipelineLayoutCreateFlagBits
  {
  };

  typedef Flags<PipelineLayoutCreateFlagBits, VkPipelineLayoutCreateFlags> PipelineLayoutCreateFlags;

  inline PipelineLayoutCreateFlags operator|( PipelineLayoutCreateFlagBits bit0, PipelineLayoutCreateFlagBits bit1 )
  {
    return PipelineLayoutCreateFlags( bit0 ) | bit1;
  }

  enum class PipelineCacheCreateFlagBits
  {
  };

  typedef Flags<PipelineCacheCreateFlagBits, VkPipelineCacheCreateFlags> PipelineCacheCreateFlags;

  inline PipelineCacheCreateFlags operator|( PipelineCacheCreateFlagBits bit0, PipelineCacheCreateFlagBits bit1 )
  {
    return PipelineCacheCreateFlags( bit0 ) | bit1;
  }

  enum class PipelineDepthStencilStateCreateFlagBits
  {
  };

  typedef Flags<PipelineDepthStencilStateCreateFlagBits, VkPipelineDepthStencilStateCreateFlags> PipelineDepthStencilStateCreateFlags;

  inline PipelineDepthStencilStateCreateFlags operator|( PipelineDepthStencilStateCreateFlagBits bit0, PipelineDepthStencilStateCreateFlagBits bit1 )
  {
    return PipelineDepthStencilStateCreateFlags( bit0 ) | bit1;
  }

  enum class PipelineDynamicStateCreateFlagBits
  {
  };

  typedef Flags<PipelineDynamicStateCreateFlagBits, VkPipelineDynamicStateCreateFlags> PipelineDynamicStateCreateFlags;

  inline PipelineDynamicStateCreateFlags operator|( PipelineDynamicStateCreateFlagBits bit0, PipelineDynamicStateCreateFlagBits bit1 )
  {
    return PipelineDynamicStateCreateFlags( bit0 ) | bit1;
  }

  enum class PipelineColorBlendStateCreateFlagBits
  {
  };

  typedef Flags<PipelineColorBlendStateCreateFlagBits, VkPipelineColorBlendStateCreateFlags> PipelineColorBlendStateCreateFlags;

  inline PipelineColorBlendStateCreateFlags operator|( PipelineColorBlendStateCreateFlagBits bit0, PipelineColorBlendStateCreateFlagBits bit1 )
  {
    return PipelineColorBlendStateCreateFlags( bit0 ) | bit1;
  }

  enum class PipelineMultisampleStateCreateFlagBits
  {
  };

  typedef Flags<PipelineMultisampleStateCreateFlagBits, VkPipelineMultisampleStateCreateFlags> PipelineMultisampleStateCreateFlags;

  inline PipelineMultisampleStateCreateFlags operator|( PipelineMultisampleStateCreateFlagBits bit0, PipelineMultisampleStateCreateFlagBits bit1 )
  {
    return PipelineMultisampleStateCreateFlags( bit0 ) | bit1;
  }

  enum class PipelineRasterizationStateCreateFlagBits
  {
  };

  typedef Flags<PipelineRasterizationStateCreateFlagBits, VkPipelineRasterizationStateCreateFlags> PipelineRasterizationStateCreateFlags;

  inline PipelineRasterizationStateCreateFlags operator|( PipelineRasterizationStateCreateFlagBits bit0, PipelineRasterizationStateCreateFlagBits bit1 )
  {
    return PipelineRasterizationStateCreateFlags( bit0 ) | bit1;
  }

  enum class PipelineViewportStateCreateFlagBits
  {
  };

  typedef Flags<PipelineViewportStateCreateFlagBits, VkPipelineViewportStateCreateFlags> PipelineViewportStateCreateFlags;

  inline PipelineViewportStateCreateFlags operator|( PipelineViewportStateCreateFlagBits bit0, PipelineViewportStateCreateFlagBits bit1 )
  {
    return PipelineViewportStateCreateFlags( bit0 ) | bit1;
  }

  enum class PipelineTessellationStateCreateFlagBits
  {
  };

  typedef Flags<PipelineTessellationStateCreateFlagBits, VkPipelineTessellationStateCreateFlags> PipelineTessellationStateCreateFlags;

  inline PipelineTessellationStateCreateFlags operator|( PipelineTessellationStateCreateFlagBits bit0, PipelineTessellationStateCreateFlagBits bit1 )
  {
    return PipelineTessellationStateCreateFlags( bit0 ) | bit1;
  }

  enum class PipelineInputAssemblyStateCreateFlagBits
  {
  };

  typedef Flags<PipelineInputAssemblyStateCreateFlagBits, VkPipelineInputAssemblyStateCreateFlags> PipelineInputAssemblyStateCreateFlags;

  inline PipelineInputAssemblyStateCreateFlags operator|( PipelineInputAssemblyStateCreateFlagBits bit0, PipelineInputAssemblyStateCreateFlagBits bit1 )
  {
    return PipelineInputAssemblyStateCreateFlags( bit0 ) | bit1;
  }

  enum class PipelineVertexInputStateCreateFlagBits
  {
  };

  typedef Flags<PipelineVertexInputStateCreateFlagBits, VkPipelineVertexInputStateCreateFlags> PipelineVertexInputStateCreateFlags;

  inline PipelineVertexInputStateCreateFlags operator|( PipelineVertexInputStateCreateFlagBits bit0, PipelineVertexInputStateCreateFlagBits bit1 )
  {
    return PipelineVertexInputStateCreateFlags( bit0 ) | bit1;
  }

  enum class PipelineShaderStageCreateFlagBits
  {
  };

  typedef Flags<PipelineShaderStageCreateFlagBits, VkPipelineShaderStageCreateFlags> PipelineShaderStageCreateFlags;

  inline PipelineShaderStageCreateFlags operator|( PipelineShaderStageCreateFlagBits bit0, PipelineShaderStageCreateFlagBits bit1 )
  {
    return PipelineShaderStageCreateFlags( bit0 ) | bit1;
  }

  enum class DescriptorSetLayoutCreateFlagBits
  {
  };

  typedef Flags<DescriptorSetLayoutCreateFlagBits, VkDescriptorSetLayoutCreateFlags> DescriptorSetLayoutCreateFlags;

  inline DescriptorSetLayoutCreateFlags operator|( DescriptorSetLayoutCreateFlagBits bit0, DescriptorSetLayoutCreateFlagBits bit1 )
  {
    return DescriptorSetLayoutCreateFlags( bit0 ) | bit1;
  }

  enum class BufferViewCreateFlagBits
  {
  };

  typedef Flags<BufferViewCreateFlagBits, VkBufferViewCreateFlags> BufferViewCreateFlags;

  inline BufferViewCreateFlags operator|( BufferViewCreateFlagBits bit0, BufferViewCreateFlagBits bit1 )
  {
    return BufferViewCreateFlags( bit0 ) | bit1;
  }

  enum class InstanceCreateFlagBits
  {
  };

  typedef Flags<InstanceCreateFlagBits, VkInstanceCreateFlags> InstanceCreateFlags;

  inline InstanceCreateFlags operator|( InstanceCreateFlagBits bit0, InstanceCreateFlagBits bit1 )
  {
    return InstanceCreateFlags( bit0 ) | bit1;
  }

  enum class DeviceCreateFlagBits
  {
  };

  typedef Flags<DeviceCreateFlagBits, VkDeviceCreateFlags> DeviceCreateFlags;

  inline DeviceCreateFlags operator|( DeviceCreateFlagBits bit0, DeviceCreateFlagBits bit1 )
  {
    return DeviceCreateFlags( bit0 ) | bit1;
  }

  enum class DeviceQueueCreateFlagBits
  {
  };

  typedef Flags<DeviceQueueCreateFlagBits, VkDeviceQueueCreateFlags> DeviceQueueCreateFlags;

  inline DeviceQueueCreateFlags operator|( DeviceQueueCreateFlagBits bit0, DeviceQueueCreateFlagBits bit1 )
  {
    return DeviceQueueCreateFlags( bit0 ) | bit1;
  }

  enum class ImageViewCreateFlagBits
  {
  };

  typedef Flags<ImageViewCreateFlagBits, VkImageViewCreateFlags> ImageViewCreateFlags;

  inline ImageViewCreateFlags operator|( ImageViewCreateFlagBits bit0, ImageViewCreateFlagBits bit1 )
  {
    return ImageViewCreateFlags( bit0 ) | bit1;
  }

  enum class SemaphoreCreateFlagBits
  {
  };

  typedef Flags<SemaphoreCreateFlagBits, VkSemaphoreCreateFlags> SemaphoreCreateFlags;

  inline SemaphoreCreateFlags operator|( SemaphoreCreateFlagBits bit0, SemaphoreCreateFlagBits bit1 )
  {
    return SemaphoreCreateFlags( bit0 ) | bit1;
  }

  enum class ShaderModuleCreateFlagBits
  {
  };

  typedef Flags<ShaderModuleCreateFlagBits, VkShaderModuleCreateFlags> ShaderModuleCreateFlags;

  inline ShaderModuleCreateFlags operator|( ShaderModuleCreateFlagBits bit0, ShaderModuleCreateFlagBits bit1 )
  {
    return ShaderModuleCreateFlags( bit0 ) | bit1;
  }

  enum class EventCreateFlagBits
  {
  };

  typedef Flags<EventCreateFlagBits, VkEventCreateFlags> EventCreateFlags;

  inline EventCreateFlags operator|( EventCreateFlagBits bit0, EventCreateFlagBits bit1 )
  {
    return EventCreateFlags( bit0 ) | bit1;
  }

  enum class MemoryMapFlagBits
  {
  };

  typedef Flags<MemoryMapFlagBits, VkMemoryMapFlags> MemoryMapFlags;

  inline MemoryMapFlags operator|( MemoryMapFlagBits bit0, MemoryMapFlagBits bit1 )
  {
    return MemoryMapFlags( bit0 ) | bit1;
  }

  enum class SubpassDescriptionFlagBits
  {
  };

  typedef Flags<SubpassDescriptionFlagBits, VkSubpassDescriptionFlags> SubpassDescriptionFlags;

  inline SubpassDescriptionFlags operator|( SubpassDescriptionFlagBits bit0, SubpassDescriptionFlagBits bit1 )
  {
    return SubpassDescriptionFlags( bit0 ) | bit1;
  }

  enum class DescriptorPoolResetFlagBits
  {
  };

  typedef Flags<DescriptorPoolResetFlagBits, VkDescriptorPoolResetFlags> DescriptorPoolResetFlags;

  inline DescriptorPoolResetFlags operator|( DescriptorPoolResetFlagBits bit0, DescriptorPoolResetFlagBits bit1 )
  {
    return DescriptorPoolResetFlags( bit0 ) | bit1;
  }

  enum class SwapchainCreateFlagBitsKHR
  {
  };

  typedef Flags<SwapchainCreateFlagBitsKHR, VkSwapchainCreateFlagsKHR> SwapchainCreateFlagsKHR;

  inline SwapchainCreateFlagsKHR operator|( SwapchainCreateFlagBitsKHR bit0, SwapchainCreateFlagBitsKHR bit1 )
  {
    return SwapchainCreateFlagsKHR( bit0 ) | bit1;
  }

  enum class DisplayModeCreateFlagBitsKHR
  {
  };

  typedef Flags<DisplayModeCreateFlagBitsKHR, VkDisplayModeCreateFlagsKHR> DisplayModeCreateFlagsKHR;

  inline DisplayModeCreateFlagsKHR operator|( DisplayModeCreateFlagBitsKHR bit0, DisplayModeCreateFlagBitsKHR bit1 )
  {
    return DisplayModeCreateFlagsKHR( bit0 ) | bit1;
  }

  enum class DisplaySurfaceCreateFlagBitsKHR
  {
  };

  typedef Flags<DisplaySurfaceCreateFlagBitsKHR, VkDisplaySurfaceCreateFlagsKHR> DisplaySurfaceCreateFlagsKHR;

  inline DisplaySurfaceCreateFlagsKHR operator|( DisplaySurfaceCreateFlagBitsKHR bit0, DisplaySurfaceCreateFlagBitsKHR bit1 )
  {
    return DisplaySurfaceCreateFlagsKHR( bit0 ) | bit1;
  }

  enum class AndroidSurfaceCreateFlagBitsKHR
  {
  };

  enum class MirSurfaceCreateFlagBitsKHR
  {
  };

  enum class WaylandSurfaceCreateFlagBitsKHR
  {
  };

  enum class Win32SurfaceCreateFlagBitsKHR
  {
  };

  enum class XlibSurfaceCreateFlagBitsKHR
  {
  };

  enum class XcbSurfaceCreateFlagBitsKHR
  {
  };

  enum class ImageLayout
  {
    Undefined = VK_IMAGE_LAYOUT_UNDEFINED,
    General = VK_IMAGE_LAYOUT_GENERAL,
    ColorAttachmentOptimal = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
    DepthStencilAttachmentOptimal = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
    DepthStencilReadOnlyOptimal = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL,
    ShaderReadOnlyOptimal = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
    TransferSrcOptimal = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
    TransferDstOptimal = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
    Preinitialized = VK_IMAGE_LAYOUT_PREINITIALIZED,
    PresentSrcKHR = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
  };

  enum class AttachmentLoadOp
  {
    Load = VK_ATTACHMENT_LOAD_OP_LOAD,
    Clear = VK_ATTACHMENT_LOAD_OP_CLEAR,
    DontCare = VK_ATTACHMENT_LOAD_OP_DONT_CARE
  };

  enum class AttachmentStoreOp
  {
    Store = VK_ATTACHMENT_STORE_OP_STORE,
    DontCare = VK_ATTACHMENT_STORE_OP_DONT_CARE
  };

  enum class ImageType
  {
    e1D = VK_IMAGE_TYPE_1D,
    e2D = VK_IMAGE_TYPE_2D,
    e3D = VK_IMAGE_TYPE_3D
  };

  enum class ImageTiling
  {
    Optimal = VK_IMAGE_TILING_OPTIMAL,
    Linear = VK_IMAGE_TILING_LINEAR
  };

  enum class ImageViewType
  {
    e1D = VK_IMAGE_VIEW_TYPE_1D,
    e2D = VK_IMAGE_VIEW_TYPE_2D,
    e3D = VK_IMAGE_VIEW_TYPE_3D,
    Cube = VK_IMAGE_VIEW_TYPE_CUBE,
    e1DArray = VK_IMAGE_VIEW_TYPE_1D_ARRAY,
    e2DArray = VK_IMAGE_VIEW_TYPE_2D_ARRAY,
    CubeArray = VK_IMAGE_VIEW_TYPE_CUBE_ARRAY
  };

  enum class CommandBufferLevel
  {
    Primary = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
    Secondary = VK_COMMAND_BUFFER_LEVEL_SECONDARY
  };

  enum class ComponentSwizzle
  {
    Identity = VK_COMPONENT_SWIZZLE_IDENTITY,
    Zero = VK_COMPONENT_SWIZZLE_ZERO,
    One = VK_COMPONENT_SWIZZLE_ONE,
    R = VK_COMPONENT_SWIZZLE_R,
    G = VK_COMPONENT_SWIZZLE_G,
    B = VK_COMPONENT_SWIZZLE_B,
    A = VK_COMPONENT_SWIZZLE_A
  };

  enum class DescriptorType
  {
    Sampler = VK_DESCRIPTOR_TYPE_SAMPLER,
    CombinedImageSampler = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
    SampledImage = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,
    StorageImage = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
    UniformTexelBuffer = VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER,
    StorageTexelBuffer = VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER,
    UniformBuffer = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
    StorageBuffer = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
    UniformBufferDynamic = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,
    StorageBufferDynamic = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC,
    InputAttachment = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT
  };

  enum class QueryType
  {
    Occlusion = VK_QUERY_TYPE_OCCLUSION,
    PipelineStatistics = VK_QUERY_TYPE_PIPELINE_STATISTICS,
    Timestamp = VK_QUERY_TYPE_TIMESTAMP
  };

  enum class BorderColor
  {
    FloatTransparentBlack = VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK,
    IntTransparentBlack = VK_BORDER_COLOR_INT_TRANSPARENT_BLACK,
    FloatOpaqueBlack = VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK,
    IntOpaqueBlack = VK_BORDER_COLOR_INT_OPAQUE_BLACK,
    FloatOpaqueWhite = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE,
    IntOpaqueWhite = VK_BORDER_COLOR_INT_OPAQUE_WHITE
  };

  enum class PipelineBindPoint
  {
    Graphics = VK_PIPELINE_BIND_POINT_GRAPHICS,
    Compute = VK_PIPELINE_BIND_POINT_COMPUTE
  };

  enum class PipelineCacheHeaderVersion
  {
    One = VK_PIPELINE_CACHE_HEADER_VERSION_ONE
  };

  enum class PrimitiveTopology
  {
    PointList = VK_PRIMITIVE_TOPOLOGY_POINT_LIST,
    LineList = VK_PRIMITIVE_TOPOLOGY_LINE_LIST,
    LineStrip = VK_PRIMITIVE_TOPOLOGY_LINE_STRIP,
    TriangleList = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
    TriangleStrip = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP,
    TriangleFan = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN,
    LineListWithAdjacency = VK_PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY,
    LineStripWithAdjacency = VK_PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY,
    TriangleListWithAdjacency = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY,
    TriangleStripWithAdjacency = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY,
    PatchList = VK_PRIMITIVE_TOPOLOGY_PATCH_LIST
  };

  enum class SharingMode
  {
    Exclusive = VK_SHARING_MODE_EXCLUSIVE,
    Concurrent = VK_SHARING_MODE_CONCURRENT
  };

  enum class IndexType
  {
    Uint16 = VK_INDEX_TYPE_UINT16,
    Uint32 = VK_INDEX_TYPE_UINT32
  };

  enum class Filter
  {
    Nearest = VK_FILTER_NEAREST,
    Linear = VK_FILTER_LINEAR
  };

  enum class SamplerMipmapMode
  {
    Nearest = VK_SAMPLER_MIPMAP_MODE_NEAREST,
    Linear = VK_SAMPLER_MIPMAP_MODE_LINEAR
  };

  enum class SamplerAddressMode
  {
    Repeat = VK_SAMPLER_ADDRESS_MODE_REPEAT,
    MirroredRepeat = VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT,
    ClampToEdge = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
    ClampToBorder = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER,
    MirrorClampToEdge = VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE
  };

  enum class CompareOp
  {
    Never = VK_COMPARE_OP_NEVER,
    Less = VK_COMPARE_OP_LESS,
    Equal = VK_COMPARE_OP_EQUAL,
    LessOrEqual = VK_COMPARE_OP_LESS_OR_EQUAL,
    Greater = VK_COMPARE_OP_GREATER,
    NotEqual = VK_COMPARE_OP_NOT_EQUAL,
    GreaterOrEqual = VK_COMPARE_OP_GREATER_OR_EQUAL,
    Always = VK_COMPARE_OP_ALWAYS
  };

  enum class PolygonMode
  {
    Fill = VK_POLYGON_MODE_FILL,
    Line = VK_POLYGON_MODE_LINE,
    Point = VK_POLYGON_MODE_POINT
  };

  enum class CullModeFlagBits
  {
    None = VK_CULL_MODE_NONE,
    Front = VK_CULL_MODE_FRONT_BIT,
    Back = VK_CULL_MODE_BACK_BIT,
    FrontAndBack = VK_CULL_MODE_FRONT_AND_BACK
  };

  typedef Flags<CullModeFlagBits, VkCullModeFlags> CullModeFlags;

  inline CullModeFlags operator|( CullModeFlagBits bit0, CullModeFlagBits bit1 )
  {
    return CullModeFlags( bit0 ) | bit1;
  }

  enum class FrontFace
  {
    CounterClockwise = VK_FRONT_FACE_COUNTER_CLOCKWISE,
    Clockwise = VK_FRONT_FACE_CLOCKWISE
  };

  enum class BlendFactor
  {
    Zero = VK_BLEND_FACTOR_ZERO,
    One = VK_BLEND_FACTOR_ONE,
    SrcColor = VK_BLEND_FACTOR_SRC_COLOR,
    OneMinusSrcColor = VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR,
    DstColor = VK_BLEND_FACTOR_DST_COLOR,
    OneMinusDstColor = VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR,
    SrcAlpha = VK_BLEND_FACTOR_SRC_ALPHA,
    OneMinusSrcAlpha = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
    DstAlpha = VK_BLEND_FACTOR_DST_ALPHA,
    OneMinusDstAlpha = VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA,
    ConstantColor = VK_BLEND_FACTOR_CONSTANT_COLOR,
    OneMinusConstantColor = VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR,
    ConstantAlpha = VK_BLEND_FACTOR_CONSTANT_ALPHA,
    OneMinusConstantAlpha = VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA,
    SrcAlphaSaturate = VK_BLEND_FACTOR_SRC_ALPHA_SATURATE,
    Src1Color = VK_BLEND_FACTOR_SRC1_COLOR,
    OneMinusSrc1Color = VK_BLEND_FACTOR_ONE_MINUS_SRC1_COLOR,
    Src1Alpha = VK_BLEND_FACTOR_SRC1_ALPHA,
    OneMinusSrc1Alpha = VK_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA
  };

  enum class BlendOp
  {
    Add = VK_BLEND_OP_ADD,
    Subtract = VK_BLEND_OP_SUBTRACT,
    ReverseSubtract = VK_BLEND_OP_REVERSE_SUBTRACT,
    Min = VK_BLEND_OP_MIN,
    Max = VK_BLEND_OP_MAX
  };

  enum class StencilOp
  {
    Keep = VK_STENCIL_OP_KEEP,
    Zero = VK_STENCIL_OP_ZERO,
    Replace = VK_STENCIL_OP_REPLACE,
    IncrementAndClamp = VK_STENCIL_OP_INCREMENT_AND_CLAMP,
    DecrementAndClamp = VK_STENCIL_OP_DECREMENT_AND_CLAMP,
    Invert = VK_STENCIL_OP_INVERT,
    IncrementAndWrap = VK_STENCIL_OP_INCREMENT_AND_WRAP,
    DecrementAndWrap = VK_STENCIL_OP_DECREMENT_AND_WRAP
  };

  enum class LogicOp
  {
    Clear = VK_LOGIC_OP_CLEAR,
    And = VK_LOGIC_OP_AND,
    AndReverse = VK_LOGIC_OP_AND_REVERSE,
    Copy = VK_LOGIC_OP_COPY,
    AndInverted = VK_LOGIC_OP_AND_INVERTED,
    NoOp = VK_LOGIC_OP_NO_OP,
    Xor = VK_LOGIC_OP_XOR,
    Or = VK_LOGIC_OP_OR,
    Nor = VK_LOGIC_OP_NOR,
    Equivalent = VK_LOGIC_OP_EQUIVALENT,
    Invert = VK_LOGIC_OP_INVERT,
    OrReverse = VK_LOGIC_OP_OR_REVERSE,
    CopyInverted = VK_LOGIC_OP_COPY_INVERTED,
    OrInverted = VK_LOGIC_OP_OR_INVERTED,
    Nand = VK_LOGIC_OP_NAND,
    Set = VK_LOGIC_OP_SET
  };

  enum class InternalAllocationType
  {
    Executable = VK_INTERNAL_ALLOCATION_TYPE_EXECUTABLE
  };

  enum class SystemAllocationScope
  {
    Command = VK_SYSTEM_ALLOCATION_SCOPE_COMMAND,
    Object = VK_SYSTEM_ALLOCATION_SCOPE_OBJECT,
    Cache = VK_SYSTEM_ALLOCATION_SCOPE_CACHE,
    Device = VK_SYSTEM_ALLOCATION_SCOPE_DEVICE,
    Instance = VK_SYSTEM_ALLOCATION_SCOPE_INSTANCE
  };

  enum class PhysicalDeviceType
  {
    Other = VK_PHYSICAL_DEVICE_TYPE_OTHER,
    IntegratedGpu = VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU,
    DiscreteGpu = VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU,
    VirtualGpu = VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU,
    Cpu = VK_PHYSICAL_DEVICE_TYPE_CPU
  };

  enum class VertexInputRate
  {
    Vertex = VK_VERTEX_INPUT_RATE_VERTEX,
    Instance = VK_VERTEX_INPUT_RATE_INSTANCE
  };

  enum class Format
  {
    Undefined = VK_FORMAT_UNDEFINED,
    R4G4UnormPack8 = VK_FORMAT_R4G4_UNORM_PACK8,
    R4G4B4A4UnormPack16 = VK_FORMAT_R4G4B4A4_UNORM_PACK16,
    B4G4R4A4UnormPack16 = VK_FORMAT_B4G4R4A4_UNORM_PACK16,
    R5G6B5UnormPack16 = VK_FORMAT_R5G6B5_UNORM_PACK16,
    B5G6R5UnormPack16 = VK_FORMAT_B5G6R5_UNORM_PACK16,
    R5G5B5A1UnormPack16 = VK_FORMAT_R5G5B5A1_UNORM_PACK16,
    B5G5R5A1UnormPack16 = VK_FORMAT_B5G5R5A1_UNORM_PACK16,
    A1R5G5B5UnormPack16 = VK_FORMAT_A1R5G5B5_UNORM_PACK16,
    R8Unorm = VK_FORMAT_R8_UNORM,
    R8Snorm = VK_FORMAT_R8_SNORM,
    R8Uscaled = VK_FORMAT_R8_USCALED,
    R8Sscaled = VK_FORMAT_R8_SSCALED,
    R8Uint = VK_FORMAT_R8_UINT,
    R8Sint = VK_FORMAT_R8_SINT,
    R8Srgb = VK_FORMAT_R8_SRGB,
    R8G8Unorm = VK_FORMAT_R8G8_UNORM,
    R8G8Snorm = VK_FORMAT_R8G8_SNORM,
    R8G8Uscaled = VK_FORMAT_R8G8_USCALED,
    R8G8Sscaled = VK_FORMAT_R8G8_SSCALED,
    R8G8Uint = VK_FORMAT_R8G8_UINT,
    R8G8Sint = VK_FORMAT_R8G8_SINT,
    R8G8Srgb = VK_FORMAT_R8G8_SRGB,
    R8G8B8Unorm = VK_FORMAT_R8G8B8_UNORM,
    R8G8B8Snorm = VK_FORMAT_R8G8B8_SNORM,
    R8G8B8Uscaled = VK_FORMAT_R8G8B8_USCALED,
    R8G8B8Sscaled = VK_FORMAT_R8G8B8_SSCALED,
    R8G8B8Uint = VK_FORMAT_R8G8B8_UINT,
    R8G8B8Sint = VK_FORMAT_R8G8B8_SINT,
    R8G8B8Srgb = VK_FORMAT_R8G8B8_SRGB,
    B8G8R8Unorm = VK_FORMAT_B8G8R8_UNORM,
    B8G8R8Snorm = VK_FORMAT_B8G8R8_SNORM,
    B8G8R8Uscaled = VK_FORMAT_B8G8R8_USCALED,
    B8G8R8Sscaled = VK_FORMAT_B8G8R8_SSCALED,
    B8G8R8Uint = VK_FORMAT_B8G8R8_UINT,
    B8G8R8Sint = VK_FORMAT_B8G8R8_SINT,
    B8G8R8Srgb = VK_FORMAT_B8G8R8_SRGB,
    R8G8B8A8Unorm = VK_FORMAT_R8G8B8A8_UNORM,
    R8G8B8A8Snorm = VK_FORMAT_R8G8B8A8_SNORM,
    R8G8B8A8Uscaled = VK_FORMAT_R8G8B8A8_USCALED,
    R8G8B8A8Sscaled = VK_FORMAT_R8G8B8A8_SSCALED,
    R8G8B8A8Uint = VK_FORMAT_R8G8B8A8_UINT,
    R8G8B8A8Sint = VK_FORMAT_R8G8B8A8_SINT,
    R8G8B8A8Srgb = VK_FORMAT_R8G8B8A8_SRGB,
    B8G8R8A8Unorm = VK_FORMAT_B8G8R8A8_UNORM,
    B8G8R8A8Snorm = VK_FORMAT_B8G8R8A8_SNORM,
    B8G8R8A8Uscaled = VK_FORMAT_B8G8R8A8_USCALED,
    B8G8R8A8Sscaled = VK_FORMAT_B8G8R8A8_SSCALED,
    B8G8R8A8Uint = VK_FORMAT_B8G8R8A8_UINT,
    B8G8R8A8Sint = VK_FORMAT_B8G8R8A8_SINT,
    B8G8R8A8Srgb = VK_FORMAT_B8G8R8A8_SRGB,
    A8B8G8R8UnormPack32 = VK_FORMAT_A8B8G8R8_UNORM_PACK32,
    A8B8G8R8SnormPack32 = VK_FORMAT_A8B8G8R8_SNORM_PACK32,
    A8B8G8R8UscaledPack32 = VK_FORMAT_A8B8G8R8_USCALED_PACK32,
    A8B8G8R8SscaledPack32 = VK_FORMAT_A8B8G8R8_SSCALED_PACK32,
    A8B8G8R8UintPack32 = VK_FORMAT_A8B8G8R8_UINT_PACK32,
    A8B8G8R8SintPack32 = VK_FORMAT_A8B8G8R8_SINT_PACK32,
    A8B8G8R8SrgbPack32 = VK_FORMAT_A8B8G8R8_SRGB_PACK32,
    A2R10G10B10UnormPack32 = VK_FORMAT_A2R10G10B10_UNORM_PACK32,
    A2R10G10B10SnormPack32 = VK_FORMAT_A2R10G10B10_SNORM_PACK32,
    A2R10G10B10UscaledPack32 = VK_FORMAT_A2R10G10B10_USCALED_PACK32,
    A2R10G10B10SscaledPack32 = VK_FORMAT_A2R10G10B10_SSCALED_PACK32,
    A2R10G10B10UintPack32 = VK_FORMAT_A2R10G10B10_UINT_PACK32,
    A2R10G10B10SintPack32 = VK_FORMAT_A2R10G10B10_SINT_PACK32,
    A2B10G10R10UnormPack32 = VK_FORMAT_A2B10G10R10_UNORM_PACK32,
    A2B10G10R10SnormPack32 = VK_FORMAT_A2B10G10R10_SNORM_PACK32,
    A2B10G10R10UscaledPack32 = VK_FORMAT_A2B10G10R10_USCALED_PACK32,
    A2B10G10R10SscaledPack32 = VK_FORMAT_A2B10G10R10_SSCALED_PACK32,
    A2B10G10R10UintPack32 = VK_FORMAT_A2B10G10R10_UINT_PACK32,
    A2B10G10R10SintPack32 = VK_FORMAT_A2B10G10R10_SINT_PACK32,
    R16Unorm = VK_FORMAT_R16_UNORM,
    R16Snorm = VK_FORMAT_R16_SNORM,
    R16Uscaled = VK_FORMAT_R16_USCALED,
    R16Sscaled = VK_FORMAT_R16_SSCALED,
    R16Uint = VK_FORMAT_R16_UINT,
    R16Sint = VK_FORMAT_R16_SINT,
    R16Sfloat = VK_FORMAT_R16_SFLOAT,
    R16G16Unorm = VK_FORMAT_R16G16_UNORM,
    R16G16Snorm = VK_FORMAT_R16G16_SNORM,
    R16G16Uscaled = VK_FORMAT_R16G16_USCALED,
    R16G16Sscaled = VK_FORMAT_R16G16_SSCALED,
    R16G16Uint = VK_FORMAT_R16G16_UINT,
    R16G16Sint = VK_FORMAT_R16G16_SINT,
    R16G16Sfloat = VK_FORMAT_R16G16_SFLOAT,
    R16G16B16Unorm = VK_FORMAT_R16G16B16_UNORM,
    R16G16B16Snorm = VK_FORMAT_R16G16B16_SNORM,
    R16G16B16Uscaled = VK_FORMAT_R16G16B16_USCALED,
    R16G16B16Sscaled = VK_FORMAT_R16G16B16_SSCALED,
    R16G16B16Uint = VK_FORMAT_R16G16B16_UINT,
    R16G16B16Sint = VK_FORMAT_R16G16B16_SINT,
    R16G16B16Sfloat = VK_FORMAT_R16G16B16_SFLOAT,
    R16G16B16A16Unorm = VK_FORMAT_R16G16B16A16_UNORM,
    R16G16B16A16Snorm = VK_FORMAT_R16G16B16A16_SNORM,
    R16G16B16A16Uscaled = VK_FORMAT_R16G16B16A16_USCALED,
    R16G16B16A16Sscaled = VK_FORMAT_R16G16B16A16_SSCALED,
    R16G16B16A16Uint = VK_FORMAT_R16G16B16A16_UINT,
    R16G16B16A16Sint = VK_FORMAT_R16G16B16A16_SINT,
    R16G16B16A16Sfloat = VK_FORMAT_R16G16B16A16_SFLOAT,
    R32Uint = VK_FORMAT_R32_UINT,
    R32Sint = VK_FORMAT_R32_SINT,
    R32Sfloat = VK_FORMAT_R32_SFLOAT,
    R32G32Uint = VK_FORMAT_R32G32_UINT,
    R32G32Sint = VK_FORMAT_R32G32_SINT,
    R32G32Sfloat = VK_FORMAT_R32G32_SFLOAT,
    R32G32B32Uint = VK_FORMAT_R32G32B32_UINT,
    R32G32B32Sint = VK_FORMAT_R32G32B32_SINT,
    R32G32B32Sfloat = VK_FORMAT_R32G32B32_SFLOAT,
    R32G32B32A32Uint = VK_FORMAT_R32G32B32A32_UINT,
    R32G32B32A32Sint = VK_FORMAT_R32G32B32A32_SINT,
    R32G32B32A32Sfloat = VK_FORMAT_R32G32B32A32_SFLOAT,
    R64Uint = VK_FORMAT_R64_UINT,
    R64Sint = VK_FORMAT_R64_SINT,
    R64Sfloat = VK_FORMAT_R64_SFLOAT,
    R64G64Uint = VK_FORMAT_R64G64_UINT,
    R64G64Sint = VK_FORMAT_R64G64_SINT,
    R64G64Sfloat = VK_FORMAT_R64G64_SFLOAT,
    R64G64B64Uint = VK_FORMAT_R64G64B64_UINT,
    R64G64B64Sint = VK_FORMAT_R64G64B64_SINT,
    R64G64B64Sfloat = VK_FORMAT_R64G64B64_SFLOAT,
    R64G64B64A64Uint = VK_FORMAT_R64G64B64A64_UINT,
    R64G64B64A64Sint = VK_FORMAT_R64G64B64A64_SINT,
    R64G64B64A64Sfloat = VK_FORMAT_R64G64B64A64_SFLOAT,
    B10G11R11UfloatPack32 = VK_FORMAT_B10G11R11_UFLOAT_PACK32,
    E5B9G9R9UfloatPack32 = VK_FORMAT_E5B9G9R9_UFLOAT_PACK32,
    D16Unorm = VK_FORMAT_D16_UNORM,
    X8D24UnormPack32 = VK_FORMAT_X8_D24_UNORM_PACK32,
    D32Sfloat = VK_FORMAT_D32_SFLOAT,
    S8Uint = VK_FORMAT_S8_UINT,
    D16UnormS8Uint = VK_FORMAT_D16_UNORM_S8_UINT,
    D24UnormS8Uint = VK_FORMAT_D24_UNORM_S8_UINT,
    D32SfloatS8Uint = VK_FORMAT_D32_SFLOAT_S8_UINT,
    Bc1RgbUnormBlock = VK_FORMAT_BC1_RGB_UNORM_BLOCK,
    Bc1RgbSrgbBlock = VK_FORMAT_BC1_RGB_SRGB_BLOCK,
    Bc1RgbaUnormBlock = VK_FORMAT_BC1_RGBA_UNORM_BLOCK,
    Bc1RgbaSrgbBlock = VK_FORMAT_BC1_RGBA_SRGB_BLOCK,
    Bc2UnormBlock = VK_FORMAT_BC2_UNORM_BLOCK,
    Bc2SrgbBlock = VK_FORMAT_BC2_SRGB_BLOCK,
    Bc3UnormBlock = VK_FORMAT_BC3_UNORM_BLOCK,
    Bc3SrgbBlock = VK_FORMAT_BC3_SRGB_BLOCK,
    Bc4UnormBlock = VK_FORMAT_BC4_UNORM_BLOCK,
    Bc4SnormBlock = VK_FORMAT_BC4_SNORM_BLOCK,
    Bc5UnormBlock = VK_FORMAT_BC5_UNORM_BLOCK,
    Bc5SnormBlock = VK_FORMAT_BC5_SNORM_BLOCK,
    Bc6HUfloatBlock = VK_FORMAT_BC6H_UFLOAT_BLOCK,
    Bc6HSfloatBlock = VK_FORMAT_BC6H_SFLOAT_BLOCK,
    Bc7UnormBlock = VK_FORMAT_BC7_UNORM_BLOCK,
    Bc7SrgbBlock = VK_FORMAT_BC7_SRGB_BLOCK,
    Etc2R8G8B8UnormBlock = VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK,
    Etc2R8G8B8SrgbBlock = VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK,
    Etc2R8G8B8A1UnormBlock = VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK,
    Etc2R8G8B8A1SrgbBlock = VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK,
    Etc2R8G8B8A8UnormBlock = VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK,
    Etc2R8G8B8A8SrgbBlock = VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK,
    EacR11UnormBlock = VK_FORMAT_EAC_R11_UNORM_BLOCK,
    EacR11SnormBlock = VK_FORMAT_EAC_R11_SNORM_BLOCK,
    EacR11G11UnormBlock = VK_FORMAT_EAC_R11G11_UNORM_BLOCK,
    EacR11G11SnormBlock = VK_FORMAT_EAC_R11G11_SNORM_BLOCK,
    Astc4x4UnormBlock = VK_FORMAT_ASTC_4x4_UNORM_BLOCK,
    Astc4x4SrgbBlock = VK_FORMAT_ASTC_4x4_SRGB_BLOCK,
    Astc5x4UnormBlock = VK_FORMAT_ASTC_5x4_UNORM_BLOCK,
    Astc5x4SrgbBlock = VK_FORMAT_ASTC_5x4_SRGB_BLOCK,
    Astc5x5UnormBlock = VK_FORMAT_ASTC_5x5_UNORM_BLOCK,
    Astc5x5SrgbBlock = VK_FORMAT_ASTC_5x5_SRGB_BLOCK,
    Astc6x5UnormBlock = VK_FORMAT_ASTC_6x5_UNORM_BLOCK,
    Astc6x5SrgbBlock = VK_FORMAT_ASTC_6x5_SRGB_BLOCK,
    Astc6x6UnormBlock = VK_FORMAT_ASTC_6x6_UNORM_BLOCK,
    Astc6x6SrgbBlock = VK_FORMAT_ASTC_6x6_SRGB_BLOCK,
    Astc8x5UnormBlock = VK_FORMAT_ASTC_8x5_UNORM_BLOCK,
    Astc8x5SrgbBlock = VK_FORMAT_ASTC_8x5_SRGB_BLOCK,
    Astc8x6UnormBlock = VK_FORMAT_ASTC_8x6_UNORM_BLOCK,
    Astc8x6SrgbBlock = VK_FORMAT_ASTC_8x6_SRGB_BLOCK,
    Astc8x8UnormBlock = VK_FORMAT_ASTC_8x8_UNORM_BLOCK,
    Astc8x8SrgbBlock = VK_FORMAT_ASTC_8x8_SRGB_BLOCK,
    Astc10x5UnormBlock = VK_FORMAT_ASTC_10x5_UNORM_BLOCK,
    Astc10x5SrgbBlock = VK_FORMAT_ASTC_10x5_SRGB_BLOCK,
    Astc10x6UnormBlock = VK_FORMAT_ASTC_10x6_UNORM_BLOCK,
    Astc10x6SrgbBlock = VK_FORMAT_ASTC_10x6_SRGB_BLOCK,
    Astc10x8UnormBlock = VK_FORMAT_ASTC_10x8_UNORM_BLOCK,
    Astc10x8SrgbBlock = VK_FORMAT_ASTC_10x8_SRGB_BLOCK,
    Astc10x10UnormBlock = VK_FORMAT_ASTC_10x10_UNORM_BLOCK,
    Astc10x10SrgbBlock = VK_FORMAT_ASTC_10x10_SRGB_BLOCK,
    Astc12x10UnormBlock = VK_FORMAT_ASTC_12x10_UNORM_BLOCK,
    Astc12x10SrgbBlock = VK_FORMAT_ASTC_12x10_SRGB_BLOCK,
    Astc12x12UnormBlock = VK_FORMAT_ASTC_12x12_UNORM_BLOCK,
    Astc12x12SrgbBlock = VK_FORMAT_ASTC_12x12_SRGB_BLOCK
  };

  enum class StructureType
  {
    ApplicationInfo = VK_STRUCTURE_TYPE_APPLICATION_INFO,
    InstanceCreateInfo = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
    DeviceQueueCreateInfo = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
    DeviceCreateInfo = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
    SubmitInfo = VK_STRUCTURE_TYPE_SUBMIT_INFO,
    MemoryAllocateInfo = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
    MappedMemoryRange = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE,
    BindSparseInfo = VK_STRUCTURE_TYPE_BIND_SPARSE_INFO,
    FenceCreateInfo = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
    SemaphoreCreateInfo = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
    EventCreateInfo = VK_STRUCTURE_TYPE_EVENT_CREATE_INFO,
    QueryPoolCreateInfo = VK_STRUCTURE_TYPE_QUERY_POOL_CREATE_INFO,
    BufferCreateInfo = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
    BufferViewCreateInfo = VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO,
    ImageCreateInfo = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
    ImageViewCreateInfo = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
    ShaderModuleCreateInfo = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
    PipelineCacheCreateInfo = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO,
    PipelineShaderStageCreateInfo = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
    PipelineVertexInputStateCreateInfo = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
    PipelineInputAssemblyStateCreateInfo = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
    PipelineTessellationStateCreateInfo = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO,
    PipelineViewportStateCreateInfo = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
    PipelineRasterizationStateCreateInfo = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
    PipelineMultisampleStateCreateInfo = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
    PipelineDepthStencilStateCreateInfo = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
    PipelineColorBlendStateCreateInfo = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
    PipelineDynamicStateCreateInfo = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
    GraphicsPipelineCreateInfo = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
    ComputePipelineCreateInfo = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO,
    PipelineLayoutCreateInfo = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
    SamplerCreateInfo = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
    DescriptorSetLayoutCreateInfo = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
    DescriptorPoolCreateInfo = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
    DescriptorSetAllocateInfo = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
    WriteDescriptorSet = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
    CopyDescriptorSet = VK_STRUCTURE_TYPE_COPY_DESCRIPTOR_SET,
    FramebufferCreateInfo = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
    RenderPassCreateInfo = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
    CommandPoolCreateInfo = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
    CommandBufferAllocateInfo = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
    CommandBufferInheritanceInfo = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO,
    CommandBufferBeginInfo = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
    RenderPassBeginInfo = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
    BufferMemoryBarrier = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER,
    ImageMemoryBarrier = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
    MemoryBarrier = VK_STRUCTURE_TYPE_MEMORY_BARRIER,
    LoaderInstanceCreateInfo = VK_STRUCTURE_TYPE_LOADER_INSTANCE_CREATE_INFO,
    LoaderDeviceCreateInfo = VK_STRUCTURE_TYPE_LOADER_DEVICE_CREATE_INFO,
    SwapchainCreateInfoKHR = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
    PresentInfoKHR = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
    DisplayModeCreateInfoKHR = VK_STRUCTURE_TYPE_DISPLAY_MODE_CREATE_INFO_KHR,
    DisplaySurfaceCreateInfoKHR = VK_STRUCTURE_TYPE_DISPLAY_SURFACE_CREATE_INFO_KHR,
    DisplayPresentInfoKHR = VK_STRUCTURE_TYPE_DISPLAY_PRESENT_INFO_KHR,
    XlibSurfaceCreateInfoKHR = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR,
    XcbSurfaceCreateInfoKHR = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR,
    WaylandSurfaceCreateInfoKHR = VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR,
    MirSurfaceCreateInfoKHR = VK_STRUCTURE_TYPE_MIR_SURFACE_CREATE_INFO_KHR,
    AndroidSurfaceCreateInfoKHR = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR,
    Win32SurfaceCreateInfoKHR = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
    DebugReportCreateInfoEXT = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT
  };

  enum class SubpassContents
  {
    Inline = VK_SUBPASS_CONTENTS_INLINE,
    SecondaryCommandBuffers = VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS
  };

  enum class Result
  {
    Success = VK_SUCCESS,
    NotReady = VK_NOT_READY,
    Timeout = VK_TIMEOUT,
    EventSet = VK_EVENT_SET,
    EventReset = VK_EVENT_RESET,
    Incomplete = VK_INCOMPLETE,
    ErrorOutOfHostMemory = VK_ERROR_OUT_OF_HOST_MEMORY,
    ErrorOutOfDeviceMemory = VK_ERROR_OUT_OF_DEVICE_MEMORY,
    ErrorInitializationFailed = VK_ERROR_INITIALIZATION_FAILED,
    ErrorDeviceLost = VK_ERROR_DEVICE_LOST,
    ErrorMemoryMapFailed = VK_ERROR_MEMORY_MAP_FAILED,
    ErrorLayerNotPresent = VK_ERROR_LAYER_NOT_PRESENT,
    ErrorExtensionNotPresent = VK_ERROR_EXTENSION_NOT_PRESENT,
    ErrorFeatureNotPresent = VK_ERROR_FEATURE_NOT_PRESENT,
    ErrorIncompatibleDriver = VK_ERROR_INCOMPATIBLE_DRIVER,
    ErrorTooManyObjects = VK_ERROR_TOO_MANY_OBJECTS,
    ErrorFormatNotSupported = VK_ERROR_FORMAT_NOT_SUPPORTED,
    ErrorSurfaceLostKHR = VK_ERROR_SURFACE_LOST_KHR,
    ErrorNativeWindowInUseKHR = VK_ERROR_NATIVE_WINDOW_IN_USE_KHR,
    SuboptimalKHR = VK_SUBOPTIMAL_KHR,
    ErrorOutOfDateKHR = VK_ERROR_OUT_OF_DATE_KHR,
    ErrorIncompatibleDisplayKHR = VK_ERROR_INCOMPATIBLE_DISPLAY_KHR,
    ErrorValidationFailedEXT = VK_ERROR_VALIDATION_FAILED_EXT
  };

  enum class DynamicState
  {
    Viewport = VK_DYNAMIC_STATE_VIEWPORT,
    Scissor = VK_DYNAMIC_STATE_SCISSOR,
    LineWidth = VK_DYNAMIC_STATE_LINE_WIDTH,
    DepthBias = VK_DYNAMIC_STATE_DEPTH_BIAS,
    BlendConstants = VK_DYNAMIC_STATE_BLEND_CONSTANTS,
    DepthBounds = VK_DYNAMIC_STATE_DEPTH_BOUNDS,
    StencilCompareMask = VK_DYNAMIC_STATE_STENCIL_COMPARE_MASK,
    StencilWriteMask = VK_DYNAMIC_STATE_STENCIL_WRITE_MASK,
    StencilReference = VK_DYNAMIC_STATE_STENCIL_REFERENCE
  };

  enum class QueueFlagBits
  {
    Graphics = VK_QUEUE_GRAPHICS_BIT,
    Compute = VK_QUEUE_COMPUTE_BIT,
    Transfer = VK_QUEUE_TRANSFER_BIT,
    SparseBinding = VK_QUEUE_SPARSE_BINDING_BIT
  };

  typedef Flags<QueueFlagBits, VkQueueFlags> QueueFlags;

  inline QueueFlags operator|( QueueFlagBits bit0, QueueFlagBits bit1 )
  {
    return QueueFlags( bit0 ) | bit1;
  }

  enum class MemoryPropertyFlagBits
  {
    DeviceLocal = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
    HostVisible = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
    HostCoherent = VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
    HostCached = VK_MEMORY_PROPERTY_HOST_CACHED_BIT,
    LazilyAllocated = VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT
  };

  typedef Flags<MemoryPropertyFlagBits, VkMemoryPropertyFlags> MemoryPropertyFlags;

  inline MemoryPropertyFlags operator|( MemoryPropertyFlagBits bit0, MemoryPropertyFlagBits bit1 )
  {
    return MemoryPropertyFlags( bit0 ) | bit1;
  }

  enum class MemoryHeapFlagBits
  {
    DeviceLocal = VK_MEMORY_HEAP_DEVICE_LOCAL_BIT
  };

  typedef Flags<MemoryHeapFlagBits, VkMemoryHeapFlags> MemoryHeapFlags;

  inline MemoryHeapFlags operator|( MemoryHeapFlagBits bit0, MemoryHeapFlagBits bit1 )
  {
    return MemoryHeapFlags( bit0 ) | bit1;
  }

  enum class AccessFlagBits
  {
    IndirectCommandRead = VK_ACCESS_INDIRECT_COMMAND_READ_BIT,
    IndexRead = VK_ACCESS_INDEX_READ_BIT,
    VertexAttributeRead = VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT,
    UniformRead = VK_ACCESS_UNIFORM_READ_BIT,
    InputAttachmentRead = VK_ACCESS_INPUT_ATTACHMENT_READ_BIT,
    ShaderRead = VK_ACCESS_SHADER_READ_BIT,
    ShaderWrite = VK_ACCESS_SHADER_WRITE_BIT,
    ColorAttachmentRead = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT,
    ColorAttachmentWrite = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
    DepthStencilAttachmentRead = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT,
    DepthStencilAttachmentWrite = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
    TransferRead = VK_ACCESS_TRANSFER_READ_BIT,
    TransferWrite = VK_ACCESS_TRANSFER_WRITE_BIT,
    HostRead = VK_ACCESS_HOST_READ_BIT,
    HostWrite = VK_ACCESS_HOST_WRITE_BIT,
    MemoryRead = VK_ACCESS_MEMORY_READ_BIT,
    MemoryWrite = VK_ACCESS_MEMORY_WRITE_BIT
  };

  typedef Flags<AccessFlagBits, VkAccessFlags> AccessFlags;

  inline AccessFlags operator|( AccessFlagBits bit0, AccessFlagBits bit1 )
  {
    return AccessFlags( bit0 ) | bit1;
  }

  enum class BufferUsageFlagBits
  {
    TransferSrc = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
    TransferDst = VK_BUFFER_USAGE_TRANSFER_DST_BIT,
    UniformTexelBuffer = VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT,
    StorageTexelBuffer = VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT,
    UniformBuffer = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
    StorageBuffer = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
    IndexBuffer = VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
    VertexBuffer = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
    IndirectBuffer = VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT
  };

  typedef Flags<BufferUsageFlagBits, VkBufferUsageFlags> BufferUsageFlags;

  inline BufferUsageFlags operator|( BufferUsageFlagBits bit0, BufferUsageFlagBits bit1 )
  {
    return BufferUsageFlags( bit0 ) | bit1;
  }

  enum class BufferCreateFlagBits
  {
    SparseBinding = VK_BUFFER_CREATE_SPARSE_BINDING_BIT,
    SparseResidency = VK_BUFFER_CREATE_SPARSE_RESIDENCY_BIT,
    SparseAliased = VK_BUFFER_CREATE_SPARSE_ALIASED_BIT
  };

  typedef Flags<BufferCreateFlagBits, VkBufferCreateFlags> BufferCreateFlags;

  inline BufferCreateFlags operator|( BufferCreateFlagBits bit0, BufferCreateFlagBits bit1 )
  {
    return BufferCreateFlags( bit0 ) | bit1;
  }

  enum class ShaderStageFlagBits
  {
    Vertex = VK_SHADER_STAGE_VERTEX_BIT,
    TessellationControl = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT,
    TessellationEvaluation = VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT,
    Geometry = VK_SHADER_STAGE_GEOMETRY_BIT,
    Fragment = VK_SHADER_STAGE_FRAGMENT_BIT,
    Compute = VK_SHADER_STAGE_COMPUTE_BIT,
    AllGraphics = VK_SHADER_STAGE_ALL_GRAPHICS,
    All = VK_SHADER_STAGE_ALL
  };

  typedef Flags<ShaderStageFlagBits, VkShaderStageFlags> ShaderStageFlags;

  inline ShaderStageFlags operator|( ShaderStageFlagBits bit0, ShaderStageFlagBits bit1 )
  {
    return ShaderStageFlags( bit0 ) | bit1;
  }

  enum class ImageUsageFlagBits
  {
    TransferSrc = VK_IMAGE_USAGE_TRANSFER_SRC_BIT,
    TransferDst = VK_IMAGE_USAGE_TRANSFER_DST_BIT,
    Sampled = VK_IMAGE_USAGE_SAMPLED_BIT,
    Storage = VK_IMAGE_USAGE_STORAGE_BIT,
    ColorAttachment = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
    DepthStencilAttachment = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
    TransientAttachment = VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT,
    InputAttachment = VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT
  };

  typedef Flags<ImageUsageFlagBits, VkImageUsageFlags> ImageUsageFlags;

  inline ImageUsageFlags operator|( ImageUsageFlagBits bit0, ImageUsageFlagBits bit1 )
  {
    return ImageUsageFlags( bit0 ) | bit1;
  }

  enum class ImageCreateFlagBits
  {
    SparseBinding = VK_IMAGE_CREATE_SPARSE_BINDING_BIT,
    SparseResidency = VK_IMAGE_CREATE_SPARSE_RESIDENCY_BIT,
    SparseAliased = VK_IMAGE_CREATE_SPARSE_ALIASED_BIT,
    MutableFormat = VK_IMAGE_CREATE_MUTABLE_FORMAT_BIT,
    CubeCompatible = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT
  };

  typedef Flags<ImageCreateFlagBits, VkImageCreateFlags> ImageCreateFlags;

  inline ImageCreateFlags operator|( ImageCreateFlagBits bit0, ImageCreateFlagBits bit1 )
  {
    return ImageCreateFlags( bit0 ) | bit1;
  }

  enum class PipelineCreateFlagBits
  {
    DisableOptimization = VK_PIPELINE_CREATE_DISABLE_OPTIMIZATION_BIT,
    AllowDerivatives = VK_PIPELINE_CREATE_ALLOW_DERIVATIVES_BIT,
    Derivative = VK_PIPELINE_CREATE_DERIVATIVE_BIT
  };

  typedef Flags<PipelineCreateFlagBits, VkPipelineCreateFlags> PipelineCreateFlags;

  inline PipelineCreateFlags operator|( PipelineCreateFlagBits bit0, PipelineCreateFlagBits bit1 )
  {
    return PipelineCreateFlags( bit0 ) | bit1;
  }

  enum class ColorComponentFlagBits
  {
    R = VK_COLOR_COMPONENT_R_BIT,
    G = VK_COLOR_COMPONENT_G_BIT,
    B = VK_COLOR_COMPONENT_B_BIT,
    A = VK_COLOR_COMPONENT_A_BIT
  };

  typedef Flags<ColorComponentFlagBits, VkColorComponentFlags> ColorComponentFlags;

  inline ColorComponentFlags operator|( ColorComponentFlagBits bit0, ColorComponentFlagBits bit1 )
  {
    return ColorComponentFlags( bit0 ) | bit1;
  }

  enum class FenceCreateFlagBits
  {
    Signaled = VK_FENCE_CREATE_SIGNALED_BIT
  };

  typedef Flags<FenceCreateFlagBits, VkFenceCreateFlags> FenceCreateFlags;

  inline FenceCreateFlags operator|( FenceCreateFlagBits bit0, FenceCreateFlagBits bit1 )
  {
    return FenceCreateFlags( bit0 ) | bit1;
  }

  enum class FormatFeatureFlagBits
  {
    SampledImage = VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT,
    StorageImage = VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT,
    StorageImageAtomic = VK_FORMAT_FEATURE_STORAGE_IMAGE_ATOMIC_BIT,
    UniformTexelBuffer = VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT,
    StorageTexelBuffer = VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT,
    StorageTexelBufferAtomic = VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_ATOMIC_BIT,
    VertexBuffer = VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT,
    ColorAttachment = VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT,
    ColorAttachmentBlend = VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT,
    DepthStencilAttachment = VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT,
    BlitSrc = VK_FORMAT_FEATURE_BLIT_SRC_BIT,
    BlitDst = VK_FORMAT_FEATURE_BLIT_DST_BIT,
    SampledImageFilterLinear = VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT
  };

  typedef Flags<FormatFeatureFlagBits, VkFormatFeatureFlags> FormatFeatureFlags;

  inline FormatFeatureFlags operator|( FormatFeatureFlagBits bit0, FormatFeatureFlagBits bit1 )
  {
    return FormatFeatureFlags( bit0 ) | bit1;
  }

  enum class QueryControlFlagBits
  {
    Precise = VK_QUERY_CONTROL_PRECISE_BIT
  };

  typedef Flags<QueryControlFlagBits, VkQueryControlFlags> QueryControlFlags;

  inline QueryControlFlags operator|( QueryControlFlagBits bit0, QueryControlFlagBits bit1 )
  {
    return QueryControlFlags( bit0 ) | bit1;
  }

  enum class QueryResultFlagBits
  {
    e64 = VK_QUERY_RESULT_64_BIT,
    Wait = VK_QUERY_RESULT_WAIT_BIT,
    WithAvailability = VK_QUERY_RESULT_WITH_AVAILABILITY_BIT,
    Partial = VK_QUERY_RESULT_PARTIAL_BIT
  };

  typedef Flags<QueryResultFlagBits, VkQueryResultFlags> QueryResultFlags;

  inline QueryResultFlags operator|( QueryResultFlagBits bit0, QueryResultFlagBits bit1 )
  {
    return QueryResultFlags( bit0 ) | bit1;
  }

  enum class CommandBufferUsageFlagBits
  {
    OneTimeSubmit = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
    RenderPassContinue = VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT,
    SimultaneousUse = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT
  };

  typedef Flags<CommandBufferUsageFlagBits, VkCommandBufferUsageFlags> CommandBufferUsageFlags;

  inline CommandBufferUsageFlags operator|( CommandBufferUsageFlagBits bit0, CommandBufferUsageFlagBits bit1 )
  {
    return CommandBufferUsageFlags( bit0 ) | bit1;
  }

  enum class QueryPipelineStatisticFlagBits
  {
    InputAssemblyVertices = VK_QUERY_PIPELINE_STATISTIC_INPUT_ASSEMBLY_VERTICES_BIT,
    InputAssemblyPrimitives = VK_QUERY_PIPELINE_STATISTIC_INPUT_ASSEMBLY_PRIMITIVES_BIT,
    VertexShaderInvocations = VK_QUERY_PIPELINE_STATISTIC_VERTEX_SHADER_INVOCATIONS_BIT,
    GeometryShaderInvocations = VK_QUERY_PIPELINE_STATISTIC_GEOMETRY_SHADER_INVOCATIONS_BIT,
    GeometryShaderPrimitives = VK_QUERY_PIPELINE_STATISTIC_GEOMETRY_SHADER_PRIMITIVES_BIT,
    ClippingInvocations = VK_QUERY_PIPELINE_STATISTIC_CLIPPING_INVOCATIONS_BIT,
    ClippingPrimitives = VK_QUERY_PIPELINE_STATISTIC_CLIPPING_PRIMITIVES_BIT,
    FragmentShaderInvocations = VK_QUERY_PIPELINE_STATISTIC_FRAGMENT_SHADER_INVOCATIONS_BIT,
    TessellationControlShaderPatches = VK_QUERY_PIPELINE_STATISTIC_TESSELLATION_CONTROL_SHADER_PATCHES_BIT,
    TessellationEvaluationShaderInvocations = VK_QUERY_PIPELINE_STATISTIC_TESSELLATION_EVALUATION_SHADER_INVOCATIONS_BIT,
    ComputeShaderInvocations = VK_QUERY_PIPELINE_STATISTIC_COMPUTE_SHADER_INVOCATIONS_BIT
  };

  typedef Flags<QueryPipelineStatisticFlagBits, VkQueryPipelineStatisticFlags> QueryPipelineStatisticFlags;

  inline QueryPipelineStatisticFlags operator|( QueryPipelineStatisticFlagBits bit0, QueryPipelineStatisticFlagBits bit1 )
  {
    return QueryPipelineStatisticFlags( bit0 ) | bit1;
  }

  enum class ImageAspectFlagBits
  {
    Color = VK_IMAGE_ASPECT_COLOR_BIT,
    Depth = VK_IMAGE_ASPECT_DEPTH_BIT,
    Stencil = VK_IMAGE_ASPECT_STENCIL_BIT,
    Metadata = VK_IMAGE_ASPECT_METADATA_BIT
  };

  typedef Flags<ImageAspectFlagBits, VkImageAspectFlags> ImageAspectFlags;

  inline ImageAspectFlags operator|( ImageAspectFlagBits bit0, ImageAspectFlagBits bit1 )
  {
    return ImageAspectFlags( bit0 ) | bit1;
  }

  enum class SparseImageFormatFlagBits
  {
    SingleMiptail = VK_SPARSE_IMAGE_FORMAT_SINGLE_MIPTAIL_BIT,
    AlignedMipSize = VK_SPARSE_IMAGE_FORMAT_ALIGNED_MIP_SIZE_BIT,
    NonstandardBlockSize = VK_SPARSE_IMAGE_FORMAT_NONSTANDARD_BLOCK_SIZE_BIT
  };

  typedef Flags<SparseImageFormatFlagBits, VkSparseImageFormatFlags> SparseImageFormatFlags;

  inline SparseImageFormatFlags operator|( SparseImageFormatFlagBits bit0, SparseImageFormatFlagBits bit1 )
  {
    return SparseImageFormatFlags( bit0 ) | bit1;
  }

  enum class SparseMemoryBindFlagBits
  {
    Metadata = VK_SPARSE_MEMORY_BIND_METADATA_BIT
  };

  typedef Flags<SparseMemoryBindFlagBits, VkSparseMemoryBindFlags> SparseMemoryBindFlags;

  inline SparseMemoryBindFlags operator|( SparseMemoryBindFlagBits bit0, SparseMemoryBindFlagBits bit1 )
  {
    return SparseMemoryBindFlags( bit0 ) | bit1;
  }

  enum class PipelineStageFlagBits
  {
    TopOfPipe = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
    DrawIndirect = VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT,
    VertexInput = VK_PIPELINE_STAGE_VERTEX_INPUT_BIT,
    VertexShader = VK_PIPELINE_STAGE_VERTEX_SHADER_BIT,
    TessellationControlShader = VK_PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT,
    TessellationEvaluationShader = VK_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT,
    GeometryShader = VK_PIPELINE_STAGE_GEOMETRY_SHADER_BIT,
    FragmentShader = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
    EarlyFragmentTests = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
    LateFragmentTests = VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,
    ColorAttachmentOutput = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
    ComputeShader = VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
    Transfer = VK_PIPELINE_STAGE_TRANSFER_BIT,
    BottomOfPipe = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
    Host = VK_PIPELINE_STAGE_HOST_BIT,
    AllGraphics = VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT,
    AllCommands = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT
  };

  typedef Flags<PipelineStageFlagBits, VkPipelineStageFlags> PipelineStageFlags;

  inline PipelineStageFlags operator|( PipelineStageFlagBits bit0, PipelineStageFlagBits bit1 )
  {
    return PipelineStageFlags( bit0 ) | bit1;
  }

  enum class CommandPoolCreateFlagBits
  {
    Transient = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT,
    ResetCommandBuffer = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT
  };

  typedef Flags<CommandPoolCreateFlagBits, VkCommandPoolCreateFlags> CommandPoolCreateFlags;

  inline CommandPoolCreateFlags operator|( CommandPoolCreateFlagBits bit0, CommandPoolCreateFlagBits bit1 )
  {
    return CommandPoolCreateFlags( bit0 ) | bit1;
  }

  enum class CommandPoolResetFlagBits
  {
    ReleaseResources = VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT
  };

  typedef Flags<CommandPoolResetFlagBits, VkCommandPoolResetFlags> CommandPoolResetFlags;

  inline CommandPoolResetFlags operator|( CommandPoolResetFlagBits bit0, CommandPoolResetFlagBits bit1 )
  {
    return CommandPoolResetFlags( bit0 ) | bit1;
  }

  enum class CommandBufferResetFlagBits
  {
    ReleaseResources = VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT
  };

  typedef Flags<CommandBufferResetFlagBits, VkCommandBufferResetFlags> CommandBufferResetFlags;

  inline CommandBufferResetFlags operator|( CommandBufferResetFlagBits bit0, CommandBufferResetFlagBits bit1 )
  {
    return CommandBufferResetFlags( bit0 ) | bit1;
  }

  enum class SampleCountFlagBits
  {
    e1 = VK_SAMPLE_COUNT_1_BIT,
    e2 = VK_SAMPLE_COUNT_2_BIT,
    e4 = VK_SAMPLE_COUNT_4_BIT,
    e8 = VK_SAMPLE_COUNT_8_BIT,
    e16 = VK_SAMPLE_COUNT_16_BIT,
    e32 = VK_SAMPLE_COUNT_32_BIT,
    e64 = VK_SAMPLE_COUNT_64_BIT
  };

  typedef Flags<SampleCountFlagBits, VkSampleCountFlags> SampleCountFlags;

  inline SampleCountFlags operator|( SampleCountFlagBits bit0, SampleCountFlagBits bit1 )
  {
    return SampleCountFlags( bit0 ) | bit1;
  }

  enum class AttachmentDescriptionFlagBits
  {
    MayAlias = VK_ATTACHMENT_DESCRIPTION_MAY_ALIAS_BIT
  };

  typedef Flags<AttachmentDescriptionFlagBits, VkAttachmentDescriptionFlags> AttachmentDescriptionFlags;

  inline AttachmentDescriptionFlags operator|( AttachmentDescriptionFlagBits bit0, AttachmentDescriptionFlagBits bit1 )
  {
    return AttachmentDescriptionFlags( bit0 ) | bit1;
  }

  enum class StencilFaceFlagBits
  {
    Front = VK_STENCIL_FACE_FRONT_BIT,
    Back = VK_STENCIL_FACE_BACK_BIT,
    VkStencilFrontAndBack = VK_STENCIL_FRONT_AND_BACK
  };

  typedef Flags<StencilFaceFlagBits, VkStencilFaceFlags> StencilFaceFlags;

  inline StencilFaceFlags operator|( StencilFaceFlagBits bit0, StencilFaceFlagBits bit1 )
  {
    return StencilFaceFlags( bit0 ) | bit1;
  }

  enum class DescriptorPoolCreateFlagBits
  {
    FreeDescriptorSet = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT
  };

  typedef Flags<DescriptorPoolCreateFlagBits, VkDescriptorPoolCreateFlags> DescriptorPoolCreateFlags;

  inline DescriptorPoolCreateFlags operator|( DescriptorPoolCreateFlagBits bit0, DescriptorPoolCreateFlagBits bit1 )
  {
    return DescriptorPoolCreateFlags( bit0 ) | bit1;
  }

  enum class DependencyFlagBits
  {
    ByRegion = VK_DEPENDENCY_BY_REGION_BIT
  };

  typedef Flags<DependencyFlagBits, VkDependencyFlags> DependencyFlags;

  inline DependencyFlags operator|( DependencyFlagBits bit0, DependencyFlagBits bit1 )
  {
    return DependencyFlags( bit0 ) | bit1;
  }

  enum class PresentModeKHR
  {
    ImmediateKHR = VK_PRESENT_MODE_IMMEDIATE_KHR,
    MailboxKHR = VK_PRESENT_MODE_MAILBOX_KHR,
    FifoKHR = VK_PRESENT_MODE_FIFO_KHR,
    FifoRelaxedKHR = VK_PRESENT_MODE_FIFO_RELAXED_KHR
  };

  enum class ColorSpaceKHR
  {
    VkColorspaceSrgbNonlinearKHR = VK_COLORSPACE_SRGB_NONLINEAR_KHR
  };

  enum class CompositeAlphaFlagBitsKHR
  {
    OpaqueKHR = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
    PreMultipliedKHR = VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR,
    PostMultipliedKHR = VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR,
    InheritKHR = VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR
  };

  typedef Flags<CompositeAlphaFlagBitsKHR, VkCompositeAlphaFlagsKHR> CompositeAlphaFlagsKHR;

  inline CompositeAlphaFlagsKHR operator|( CompositeAlphaFlagBitsKHR bit0, CompositeAlphaFlagBitsKHR bit1 )
  {
    return CompositeAlphaFlagsKHR( bit0 ) | bit1;
  }

  enum class SurfaceTransformFlagBitsKHR
  {
    IdentityKHR = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
    Rotate90KHR = VK_SURFACE_TRANSFORM_ROTATE_90_BIT_KHR,
    Rotate180KHR = VK_SURFACE_TRANSFORM_ROTATE_180_BIT_KHR,
    Rotate270KHR = VK_SURFACE_TRANSFORM_ROTATE_270_BIT_KHR,
    HorizontalMirrorKHR = VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_BIT_KHR,
    HorizontalMirrorRotate90KHR = VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_90_BIT_KHR,
    HorizontalMirrorRotate180KHR = VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_180_BIT_KHR,
    HorizontalMirrorRotate270KHR = VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_270_BIT_KHR,
    InheritKHR = VK_SURFACE_TRANSFORM_INHERIT_BIT_KHR
  };

  typedef Flags<SurfaceTransformFlagBitsKHR, VkSurfaceTransformFlagsKHR> SurfaceTransformFlagsKHR;

  inline SurfaceTransformFlagsKHR operator|( SurfaceTransformFlagBitsKHR bit0, SurfaceTransformFlagBitsKHR bit1 )
  {
    return SurfaceTransformFlagsKHR( bit0 ) | bit1;
  }

  enum class DebugReportFlagBitsEXT
  {
    InformationEXT = VK_DEBUG_REPORT_INFORMATION_BIT_EXT,
    WarningEXT = VK_DEBUG_REPORT_WARNING_BIT_EXT,
    PerformanceWarningEXT = VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT,
    ErrorEXT = VK_DEBUG_REPORT_ERROR_BIT_EXT,
    DebugEXT = VK_DEBUG_REPORT_DEBUG_BIT_EXT
  };

  typedef Flags<DebugReportFlagBitsEXT, VkDebugReportFlagsEXT> DebugReportFlagsEXT;

  inline DebugReportFlagsEXT operator|( DebugReportFlagBitsEXT bit0, DebugReportFlagBitsEXT bit1 )
  {
    return DebugReportFlagsEXT( bit0 ) | bit1;
  }

  enum class DisplayPlaneAlphaFlagBitsKHR
  {
    OpaqueKHR = VK_DISPLAY_PLANE_ALPHA_OPAQUE_BIT_KHR,
    GlobalKHR = VK_DISPLAY_PLANE_ALPHA_GLOBAL_BIT_KHR,
    PerPixelKHR = VK_DISPLAY_PLANE_ALPHA_PER_PIXEL_BIT_KHR,
    PerPixelPremultipliedKHR = VK_DISPLAY_PLANE_ALPHA_PER_PIXEL_PREMULTIPLIED_BIT_KHR
  };

  typedef Flags<DisplayPlaneAlphaFlagBitsKHR, VkDisplayPlaneAlphaFlagsKHR> DisplayPlaneAlphaFlagsKHR;

  inline DisplayPlaneAlphaFlagsKHR operator|( DisplayPlaneAlphaFlagBitsKHR bit0, DisplayPlaneAlphaFlagBitsKHR bit1 )
  {
    return DisplayPlaneAlphaFlagsKHR( bit0 ) | bit1;
  }

#ifdef VK_USE_PLATFORM_XLIB_KHR

  typedef Flags<XlibSurfaceCreateFlagBitsKHR, VkXlibSurfaceCreateFlagsKHR> XlibSurfaceCreateFlagsKHR;

  inline XlibSurfaceCreateFlagsKHR operator|( XlibSurfaceCreateFlagBitsKHR bit0, XlibSurfaceCreateFlagBitsKHR bit1 )
  {
    return XlibSurfaceCreateFlagsKHR( bit0 ) | bit1;
  }

#endif /* VK_USE_PLATFORM_XLIB_KHR */

#ifdef VK_USE_PLATFORM_XCB_KHR

  typedef Flags<XcbSurfaceCreateFlagBitsKHR, VkXcbSurfaceCreateFlagsKHR> XcbSurfaceCreateFlagsKHR;

  inline XcbSurfaceCreateFlagsKHR operator|( XcbSurfaceCreateFlagBitsKHR bit0, XcbSurfaceCreateFlagBitsKHR bit1 )
  {
    return XcbSurfaceCreateFlagsKHR( bit0 ) | bit1;
  }

#endif /* VK_USE_PLATFORM_XCB_KHR */

#ifdef VK_USE_PLATFORM_WAYLAND_KHR

  typedef Flags<WaylandSurfaceCreateFlagBitsKHR, VkWaylandSurfaceCreateFlagsKHR> WaylandSurfaceCreateFlagsKHR;

  inline WaylandSurfaceCreateFlagsKHR operator|( WaylandSurfaceCreateFlagBitsKHR bit0, WaylandSurfaceCreateFlagBitsKHR bit1 )
  {
    return WaylandSurfaceCreateFlagsKHR( bit0 ) | bit1;
  }

#endif /* VK_USE_PLATFORM_WAYLAND_KHR */

#ifdef VK_USE_PLATFORM_MIR_KHR

  typedef Flags<MirSurfaceCreateFlagBitsKHR, VkMirSurfaceCreateFlagsKHR> MirSurfaceCreateFlagsKHR;

  inline MirSurfaceCreateFlagsKHR operator|( MirSurfaceCreateFlagBitsKHR bit0, MirSurfaceCreateFlagBitsKHR bit1 )
  {
    return MirSurfaceCreateFlagsKHR( bit0 ) | bit1;
  }

#endif /* VK_USE_PLATFORM_MIR_KHR */

#ifdef VK_USE_PLATFORM_ANDROID_KHR

  typedef Flags<AndroidSurfaceCreateFlagBitsKHR, VkAndroidSurfaceCreateFlagsKHR> AndroidSurfaceCreateFlagsKHR;

  inline AndroidSurfaceCreateFlagsKHR operator|( AndroidSurfaceCreateFlagBitsKHR bit0, AndroidSurfaceCreateFlagBitsKHR bit1 )
  {
    return AndroidSurfaceCreateFlagsKHR( bit0 ) | bit1;
  }

#endif /* VK_USE_PLATFORM_ANDROID_KHR */

#ifdef VK_USE_PLATFORM_WIN32_KHR

  typedef Flags<Win32SurfaceCreateFlagBitsKHR, VkWin32SurfaceCreateFlagsKHR> Win32SurfaceCreateFlagsKHR;

  inline Win32SurfaceCreateFlagsKHR operator|( Win32SurfaceCreateFlagBitsKHR bit0, Win32SurfaceCreateFlagBitsKHR bit1 )
  {
    return Win32SurfaceCreateFlagsKHR( bit0 ) | bit1;
  }

#endif /* VK_USE_PLATFORM_WIN32_KHR */

  enum class DebugReportObjectTypeEXT
  {
    UnknownEXT = VK_DEBUG_REPORT_OBJECT_TYPE_UNKNOWN_EXT,
    InstanceEXT = VK_DEBUG_REPORT_OBJECT_TYPE_INSTANCE_EXT,
    PhysicalDeviceEXT = VK_DEBUG_REPORT_OBJECT_TYPE_PHYSICAL_DEVICE_EXT,
    DeviceEXT = VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_EXT,
    QueueEXT = VK_DEBUG_REPORT_OBJECT_TYPE_QUEUE_EXT,
    SemaphoreEXT = VK_DEBUG_REPORT_OBJECT_TYPE_SEMAPHORE_EXT,
    CommandBufferEXT = VK_DEBUG_REPORT_OBJECT_TYPE_COMMAND_BUFFER_EXT,
    FenceEXT = VK_DEBUG_REPORT_OBJECT_TYPE_FENCE_EXT,
    DeviceMemoryEXT = VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_MEMORY_EXT,
    BufferEXT = VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_EXT,
    ImageEXT = VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_EXT,
    EventEXT = VK_DEBUG_REPORT_OBJECT_TYPE_EVENT_EXT,
    QueryPoolEXT = VK_DEBUG_REPORT_OBJECT_TYPE_QUERY_POOL_EXT,
    BufferViewEXT = VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_VIEW_EXT,
    ImageViewEXT = VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_VIEW_EXT,
    ShaderModuleEXT = VK_DEBUG_REPORT_OBJECT_TYPE_SHADER_MODULE_EXT,
    PipelineCacheEXT = VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_CACHE_EXT,
    PipelineLayoutEXT = VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_LAYOUT_EXT,
    RenderPassEXT = VK_DEBUG_REPORT_OBJECT_TYPE_RENDER_PASS_EXT,
    PipelineEXT = VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_EXT,
    DescriptorSetLayoutEXT = VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT_EXT,
    SamplerEXT = VK_DEBUG_REPORT_OBJECT_TYPE_SAMPLER_EXT,
    DescriptorPoolEXT = VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_POOL_EXT,
    DescriptorSetEXT = VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_SET_EXT,
    FramebufferEXT = VK_DEBUG_REPORT_OBJECT_TYPE_FRAMEBUFFER_EXT,
    CommandPoolEXT = VK_DEBUG_REPORT_OBJECT_TYPE_COMMAND_POOL_EXT,
    SurfaceKhrEXT = VK_DEBUG_REPORT_OBJECT_TYPE_SURFACE_KHR_EXT,
    SwapchainKhrEXT = VK_DEBUG_REPORT_OBJECT_TYPE_SWAPCHAIN_KHR_EXT,
    DebugReportEXT = VK_DEBUG_REPORT_OBJECT_TYPE_DEBUG_REPORT_EXT
  };

  enum class DebugReportErrorEXT
  {
    NoneEXT = VK_DEBUG_REPORT_ERROR_NONE_EXT,
    CallbackRefEXT = VK_DEBUG_REPORT_ERROR_CALLBACK_REF_EXT
  };

}
