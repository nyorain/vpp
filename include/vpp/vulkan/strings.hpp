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
  static const char * getString(FramebufferCreateFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string getString(FramebufferCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * getString(QueryPoolCreateFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string getString(QueryPoolCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * getString(RenderPassCreateFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string getString(RenderPassCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * getString(SamplerCreateFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string getString(SamplerCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * getString(PipelineLayoutCreateFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string getString(PipelineLayoutCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * getString(PipelineCacheCreateFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string getString(PipelineCacheCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * getString(PipelineDepthStencilStateCreateFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string getString(PipelineDepthStencilStateCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * getString(PipelineDynamicStateCreateFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string getString(PipelineDynamicStateCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * getString(PipelineColorBlendStateCreateFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string getString(PipelineColorBlendStateCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * getString(PipelineMultisampleStateCreateFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string getString(PipelineMultisampleStateCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * getString(PipelineRasterizationStateCreateFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string getString(PipelineRasterizationStateCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * getString(PipelineViewportStateCreateFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string getString(PipelineViewportStateCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * getString(PipelineTessellationStateCreateFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string getString(PipelineTessellationStateCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * getString(PipelineInputAssemblyStateCreateFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string getString(PipelineInputAssemblyStateCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * getString(PipelineVertexInputStateCreateFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string getString(PipelineVertexInputStateCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * getString(PipelineShaderStageCreateFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string getString(PipelineShaderStageCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * getString(DescriptorSetLayoutCreateFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string getString(DescriptorSetLayoutCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * getString(BufferViewCreateFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string getString(BufferViewCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * getString(InstanceCreateFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string getString(InstanceCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * getString(DeviceCreateFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string getString(DeviceCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * getString(DeviceQueueCreateFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string getString(DeviceQueueCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * getString(ImageViewCreateFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string getString(ImageViewCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * getString(SemaphoreCreateFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string getString(SemaphoreCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * getString(ShaderModuleCreateFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string getString(ShaderModuleCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * getString(EventCreateFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string getString(EventCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * getString(MemoryMapFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string getString(MemoryMapFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * getString(SubpassDescriptionFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string getString(SubpassDescriptionFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * getString(DescriptorPoolResetFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string getString(DescriptorPoolResetFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * getString(SwapchainCreateFlagBitsKHR value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string getString(SwapchainCreateFlagsKHR value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * getString(DisplayModeCreateFlagBitsKHR value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string getString(DisplayModeCreateFlagsKHR value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * getString(DisplaySurfaceCreateFlagBitsKHR value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string getString(DisplaySurfaceCreateFlagsKHR value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * getString(AndroidSurfaceCreateFlagBitsKHR value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static const char * getString(MirSurfaceCreateFlagBitsKHR value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static const char * getString(WaylandSurfaceCreateFlagBitsKHR value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static const char * getString(Win32SurfaceCreateFlagBitsKHR value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static const char * getString(XlibSurfaceCreateFlagBitsKHR value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static const char * getString(XcbSurfaceCreateFlagBitsKHR value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static const char * getString(ImageLayout value)
  {
    switch (value)
    {
    case ImageLayout::Undefined: return "ndefined";
    case ImageLayout::General: return "eneral";
    case ImageLayout::ColorAttachmentOptimal: return "olorAttachmentOptimal";
    case ImageLayout::DepthStencilAttachmentOptimal: return "epthStencilAttachmentOptimal";
    case ImageLayout::DepthStencilReadOnlyOptimal: return "epthStencilReadOnlyOptimal";
    case ImageLayout::ShaderReadOnlyOptimal: return "haderReadOnlyOptimal";
    case ImageLayout::TransferSrcOptimal: return "ransferSrcOptimal";
    case ImageLayout::TransferDstOptimal: return "ransferDstOptimal";
    case ImageLayout::Preinitialized: return "reinitialized";
    case ImageLayout::PresentSrcKHR: return "resentSrcKHR";
    default: return "unknown";
    }
  }

  static const char * getString(AttachmentLoadOp value)
  {
    switch (value)
    {
    case AttachmentLoadOp::Load: return "oad";
    case AttachmentLoadOp::Clear: return "lear";
    case AttachmentLoadOp::DontCare: return "ontCare";
    default: return "unknown";
    }
  }

  static const char * getString(AttachmentStoreOp value)
  {
    switch (value)
    {
    case AttachmentStoreOp::Store: return "tore";
    case AttachmentStoreOp::DontCare: return "ontCare";
    default: return "unknown";
    }
  }

  static const char * getString(ImageType value)
  {
    switch (value)
    {
    case ImageType::e1D: return "1D";
    case ImageType::e2D: return "2D";
    case ImageType::e3D: return "3D";
    default: return "unknown";
    }
  }

  static const char * getString(ImageTiling value)
  {
    switch (value)
    {
    case ImageTiling::Optimal: return "ptimal";
    case ImageTiling::Linear: return "inear";
    default: return "unknown";
    }
  }

  static const char * getString(ImageViewType value)
  {
    switch (value)
    {
    case ImageViewType::e1D: return "1D";
    case ImageViewType::e2D: return "2D";
    case ImageViewType::e3D: return "3D";
    case ImageViewType::Cube: return "ube";
    case ImageViewType::e1DArray: return "1DArray";
    case ImageViewType::e2DArray: return "2DArray";
    case ImageViewType::CubeArray: return "ubeArray";
    default: return "unknown";
    }
  }

  static const char * getString(CommandBufferLevel value)
  {
    switch (value)
    {
    case CommandBufferLevel::Primary: return "rimary";
    case CommandBufferLevel::Secondary: return "econdary";
    default: return "unknown";
    }
  }

  static const char * getString(ComponentSwizzle value)
  {
    switch (value)
    {
    case ComponentSwizzle::Identity: return "dentity";
    case ComponentSwizzle::Zero: return "ero";
    case ComponentSwizzle::One: return "ne";
    case ComponentSwizzle::R: return "";
    case ComponentSwizzle::G: return "";
    case ComponentSwizzle::B: return "";
    case ComponentSwizzle::A: return "";
    default: return "unknown";
    }
  }

  static const char * getString(DescriptorType value)
  {
    switch (value)
    {
    case DescriptorType::Sampler: return "ampler";
    case DescriptorType::CombinedImageSampler: return "ombinedImageSampler";
    case DescriptorType::SampledImage: return "ampledImage";
    case DescriptorType::StorageImage: return "torageImage";
    case DescriptorType::UniformTexelBuffer: return "niformTexelBuffer";
    case DescriptorType::StorageTexelBuffer: return "torageTexelBuffer";
    case DescriptorType::UniformBuffer: return "niformBuffer";
    case DescriptorType::StorageBuffer: return "torageBuffer";
    case DescriptorType::UniformBufferDynamic: return "niformBufferDynamic";
    case DescriptorType::StorageBufferDynamic: return "torageBufferDynamic";
    case DescriptorType::InputAttachment: return "nputAttachment";
    default: return "unknown";
    }
  }

  static const char * getString(QueryType value)
  {
    switch (value)
    {
    case QueryType::Occlusion: return "cclusion";
    case QueryType::PipelineStatistics: return "ipelineStatistics";
    case QueryType::Timestamp: return "imestamp";
    default: return "unknown";
    }
  }

  static const char * getString(BorderColor value)
  {
    switch (value)
    {
    case BorderColor::FloatTransparentBlack: return "loatTransparentBlack";
    case BorderColor::IntTransparentBlack: return "ntTransparentBlack";
    case BorderColor::FloatOpaqueBlack: return "loatOpaqueBlack";
    case BorderColor::IntOpaqueBlack: return "ntOpaqueBlack";
    case BorderColor::FloatOpaqueWhite: return "loatOpaqueWhite";
    case BorderColor::IntOpaqueWhite: return "ntOpaqueWhite";
    default: return "unknown";
    }
  }

  static const char * getString(PipelineBindPoint value)
  {
    switch (value)
    {
    case PipelineBindPoint::Graphics: return "raphics";
    case PipelineBindPoint::Compute: return "ompute";
    default: return "unknown";
    }
  }

  static const char * getString(PipelineCacheHeaderVersion value)
  {
    switch (value)
    {
    case PipelineCacheHeaderVersion::One: return "ne";
    default: return "unknown";
    }
  }

  static const char * getString(PrimitiveTopology value)
  {
    switch (value)
    {
    case PrimitiveTopology::PointList: return "ointList";
    case PrimitiveTopology::LineList: return "ineList";
    case PrimitiveTopology::LineStrip: return "ineStrip";
    case PrimitiveTopology::TriangleList: return "riangleList";
    case PrimitiveTopology::TriangleStrip: return "riangleStrip";
    case PrimitiveTopology::TriangleFan: return "riangleFan";
    case PrimitiveTopology::LineListWithAdjacency: return "ineListWithAdjacency";
    case PrimitiveTopology::LineStripWithAdjacency: return "ineStripWithAdjacency";
    case PrimitiveTopology::TriangleListWithAdjacency: return "riangleListWithAdjacency";
    case PrimitiveTopology::TriangleStripWithAdjacency: return "riangleStripWithAdjacency";
    case PrimitiveTopology::PatchList: return "atchList";
    default: return "unknown";
    }
  }

  static const char * getString(SharingMode value)
  {
    switch (value)
    {
    case SharingMode::Exclusive: return "xclusive";
    case SharingMode::Concurrent: return "oncurrent";
    default: return "unknown";
    }
  }

  static const char * getString(IndexType value)
  {
    switch (value)
    {
    case IndexType::Uint16: return "int16";
    case IndexType::Uint32: return "int32";
    default: return "unknown";
    }
  }

  static const char * getString(Filter value)
  {
    switch (value)
    {
    case Filter::Nearest: return "earest";
    case Filter::Linear: return "inear";
    default: return "unknown";
    }
  }

  static const char * getString(SamplerMipmapMode value)
  {
    switch (value)
    {
    case SamplerMipmapMode::Nearest: return "earest";
    case SamplerMipmapMode::Linear: return "inear";
    default: return "unknown";
    }
  }

  static const char * getString(SamplerAddressMode value)
  {
    switch (value)
    {
    case SamplerAddressMode::Repeat: return "epeat";
    case SamplerAddressMode::MirroredRepeat: return "irroredRepeat";
    case SamplerAddressMode::ClampToEdge: return "lampToEdge";
    case SamplerAddressMode::ClampToBorder: return "lampToBorder";
    case SamplerAddressMode::MirrorClampToEdge: return "irrorClampToEdge";
    default: return "unknown";
    }
  }

  static const char * getString(CompareOp value)
  {
    switch (value)
    {
    case CompareOp::Never: return "ever";
    case CompareOp::Less: return "ess";
    case CompareOp::Equal: return "qual";
    case CompareOp::LessOrEqual: return "essOrEqual";
    case CompareOp::Greater: return "reater";
    case CompareOp::NotEqual: return "otEqual";
    case CompareOp::GreaterOrEqual: return "reaterOrEqual";
    case CompareOp::Always: return "lways";
    default: return "unknown";
    }
  }

  static const char * getString(PolygonMode value)
  {
    switch (value)
    {
    case PolygonMode::Fill: return "ill";
    case PolygonMode::Line: return "ine";
    case PolygonMode::Point: return "oint";
    default: return "unknown";
    }
  }

  static const char * getString(CullModeFlagBits value)
  {
    switch (value)
    {
    case CullModeFlagBits::None: return "one";
    case CullModeFlagBits::Front: return "ront";
    case CullModeFlagBits::Back: return "ack";
    case CullModeFlagBits::FrontAndBack: return "rontAndBack";
    default: return "unknown";
    }
  }

  static std::string getString(CullModeFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::CullModeFlagBits::None) result += "one | ";
    if (value & vk::CullModeFlagBits::Front) result += "ront | ";
    if (value & vk::CullModeFlagBits::Back) result += "ack | ";
    if (value & vk::CullModeFlagBits::FrontAndBack) result += "rontAndBack | ";
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * getString(FrontFace value)
  {
    switch (value)
    {
    case FrontFace::CounterClockwise: return "ounterClockwise";
    case FrontFace::Clockwise: return "lockwise";
    default: return "unknown";
    }
  }

  static const char * getString(BlendFactor value)
  {
    switch (value)
    {
    case BlendFactor::Zero: return "ero";
    case BlendFactor::One: return "ne";
    case BlendFactor::SrcColor: return "rcColor";
    case BlendFactor::OneMinusSrcColor: return "neMinusSrcColor";
    case BlendFactor::DstColor: return "stColor";
    case BlendFactor::OneMinusDstColor: return "neMinusDstColor";
    case BlendFactor::SrcAlpha: return "rcAlpha";
    case BlendFactor::OneMinusSrcAlpha: return "neMinusSrcAlpha";
    case BlendFactor::DstAlpha: return "stAlpha";
    case BlendFactor::OneMinusDstAlpha: return "neMinusDstAlpha";
    case BlendFactor::ConstantColor: return "onstantColor";
    case BlendFactor::OneMinusConstantColor: return "neMinusConstantColor";
    case BlendFactor::ConstantAlpha: return "onstantAlpha";
    case BlendFactor::OneMinusConstantAlpha: return "neMinusConstantAlpha";
    case BlendFactor::SrcAlphaSaturate: return "rcAlphaSaturate";
    case BlendFactor::Src1Color: return "rc1Color";
    case BlendFactor::OneMinusSrc1Color: return "neMinusSrc1Color";
    case BlendFactor::Src1Alpha: return "rc1Alpha";
    case BlendFactor::OneMinusSrc1Alpha: return "neMinusSrc1Alpha";
    default: return "unknown";
    }
  }

  static const char * getString(BlendOp value)
  {
    switch (value)
    {
    case BlendOp::Add: return "dd";
    case BlendOp::Subtract: return "ubtract";
    case BlendOp::ReverseSubtract: return "everseSubtract";
    case BlendOp::Min: return "in";
    case BlendOp::Max: return "ax";
    default: return "unknown";
    }
  }

  static const char * getString(StencilOp value)
  {
    switch (value)
    {
    case StencilOp::Keep: return "eep";
    case StencilOp::Zero: return "ero";
    case StencilOp::Replace: return "eplace";
    case StencilOp::IncrementAndClamp: return "ncrementAndClamp";
    case StencilOp::DecrementAndClamp: return "ecrementAndClamp";
    case StencilOp::Invert: return "nvert";
    case StencilOp::IncrementAndWrap: return "ncrementAndWrap";
    case StencilOp::DecrementAndWrap: return "ecrementAndWrap";
    default: return "unknown";
    }
  }

  static const char * getString(LogicOp value)
  {
    switch (value)
    {
    case LogicOp::Clear: return "lear";
    case LogicOp::And: return "nd";
    case LogicOp::AndReverse: return "ndReverse";
    case LogicOp::Copy: return "opy";
    case LogicOp::AndInverted: return "ndInverted";
    case LogicOp::NoOp: return "oOp";
    case LogicOp::Xor: return "or";
    case LogicOp::Or: return "r";
    case LogicOp::Nor: return "or";
    case LogicOp::Equivalent: return "quivalent";
    case LogicOp::Invert: return "nvert";
    case LogicOp::OrReverse: return "rReverse";
    case LogicOp::CopyInverted: return "opyInverted";
    case LogicOp::OrInverted: return "rInverted";
    case LogicOp::Nand: return "and";
    case LogicOp::Set: return "et";
    default: return "unknown";
    }
  }

  static const char * getString(InternalAllocationType value)
  {
    switch (value)
    {
    case InternalAllocationType::Executable: return "xecutable";
    default: return "unknown";
    }
  }

  static const char * getString(SystemAllocationScope value)
  {
    switch (value)
    {
    case SystemAllocationScope::Command: return "ommand";
    case SystemAllocationScope::Object: return "bject";
    case SystemAllocationScope::Cache: return "ache";
    case SystemAllocationScope::Device: return "evice";
    case SystemAllocationScope::Instance: return "nstance";
    default: return "unknown";
    }
  }

  static const char * getString(PhysicalDeviceType value)
  {
    switch (value)
    {
    case PhysicalDeviceType::Other: return "ther";
    case PhysicalDeviceType::IntegratedGpu: return "ntegratedGpu";
    case PhysicalDeviceType::DiscreteGpu: return "iscreteGpu";
    case PhysicalDeviceType::VirtualGpu: return "irtualGpu";
    case PhysicalDeviceType::Cpu: return "pu";
    default: return "unknown";
    }
  }

  static const char * getString(VertexInputRate value)
  {
    switch (value)
    {
    case VertexInputRate::Vertex: return "ertex";
    case VertexInputRate::Instance: return "nstance";
    default: return "unknown";
    }
  }

  static const char * getString(Format value)
  {
    switch (value)
    {
    case Format::Undefined: return "ndefined";
    case Format::R4G4UnormPack8: return "4G4UnormPack8";
    case Format::R4G4B4A4UnormPack16: return "4G4B4A4UnormPack16";
    case Format::B4G4R4A4UnormPack16: return "4G4R4A4UnormPack16";
    case Format::R5G6B5UnormPack16: return "5G6B5UnormPack16";
    case Format::B5G6R5UnormPack16: return "5G6R5UnormPack16";
    case Format::R5G5B5A1UnormPack16: return "5G5B5A1UnormPack16";
    case Format::B5G5R5A1UnormPack16: return "5G5R5A1UnormPack16";
    case Format::A1R5G5B5UnormPack16: return "1R5G5B5UnormPack16";
    case Format::R8Unorm: return "8Unorm";
    case Format::R8Snorm: return "8Snorm";
    case Format::R8Uscaled: return "8Uscaled";
    case Format::R8Sscaled: return "8Sscaled";
    case Format::R8Uint: return "8Uint";
    case Format::R8Sint: return "8Sint";
    case Format::R8Srgb: return "8Srgb";
    case Format::R8G8Unorm: return "8G8Unorm";
    case Format::R8G8Snorm: return "8G8Snorm";
    case Format::R8G8Uscaled: return "8G8Uscaled";
    case Format::R8G8Sscaled: return "8G8Sscaled";
    case Format::R8G8Uint: return "8G8Uint";
    case Format::R8G8Sint: return "8G8Sint";
    case Format::R8G8Srgb: return "8G8Srgb";
    case Format::R8G8B8Unorm: return "8G8B8Unorm";
    case Format::R8G8B8Snorm: return "8G8B8Snorm";
    case Format::R8G8B8Uscaled: return "8G8B8Uscaled";
    case Format::R8G8B8Sscaled: return "8G8B8Sscaled";
    case Format::R8G8B8Uint: return "8G8B8Uint";
    case Format::R8G8B8Sint: return "8G8B8Sint";
    case Format::R8G8B8Srgb: return "8G8B8Srgb";
    case Format::B8G8R8Unorm: return "8G8R8Unorm";
    case Format::B8G8R8Snorm: return "8G8R8Snorm";
    case Format::B8G8R8Uscaled: return "8G8R8Uscaled";
    case Format::B8G8R8Sscaled: return "8G8R8Sscaled";
    case Format::B8G8R8Uint: return "8G8R8Uint";
    case Format::B8G8R8Sint: return "8G8R8Sint";
    case Format::B8G8R8Srgb: return "8G8R8Srgb";
    case Format::R8G8B8A8Unorm: return "8G8B8A8Unorm";
    case Format::R8G8B8A8Snorm: return "8G8B8A8Snorm";
    case Format::R8G8B8A8Uscaled: return "8G8B8A8Uscaled";
    case Format::R8G8B8A8Sscaled: return "8G8B8A8Sscaled";
    case Format::R8G8B8A8Uint: return "8G8B8A8Uint";
    case Format::R8G8B8A8Sint: return "8G8B8A8Sint";
    case Format::R8G8B8A8Srgb: return "8G8B8A8Srgb";
    case Format::B8G8R8A8Unorm: return "8G8R8A8Unorm";
    case Format::B8G8R8A8Snorm: return "8G8R8A8Snorm";
    case Format::B8G8R8A8Uscaled: return "8G8R8A8Uscaled";
    case Format::B8G8R8A8Sscaled: return "8G8R8A8Sscaled";
    case Format::B8G8R8A8Uint: return "8G8R8A8Uint";
    case Format::B8G8R8A8Sint: return "8G8R8A8Sint";
    case Format::B8G8R8A8Srgb: return "8G8R8A8Srgb";
    case Format::A8B8G8R8UnormPack32: return "8B8G8R8UnormPack32";
    case Format::A8B8G8R8SnormPack32: return "8B8G8R8SnormPack32";
    case Format::A8B8G8R8UscaledPack32: return "8B8G8R8UscaledPack32";
    case Format::A8B8G8R8SscaledPack32: return "8B8G8R8SscaledPack32";
    case Format::A8B8G8R8UintPack32: return "8B8G8R8UintPack32";
    case Format::A8B8G8R8SintPack32: return "8B8G8R8SintPack32";
    case Format::A8B8G8R8SrgbPack32: return "8B8G8R8SrgbPack32";
    case Format::A2R10G10B10UnormPack32: return "2R10G10B10UnormPack32";
    case Format::A2R10G10B10SnormPack32: return "2R10G10B10SnormPack32";
    case Format::A2R10G10B10UscaledPack32: return "2R10G10B10UscaledPack32";
    case Format::A2R10G10B10SscaledPack32: return "2R10G10B10SscaledPack32";
    case Format::A2R10G10B10UintPack32: return "2R10G10B10UintPack32";
    case Format::A2R10G10B10SintPack32: return "2R10G10B10SintPack32";
    case Format::A2B10G10R10UnormPack32: return "2B10G10R10UnormPack32";
    case Format::A2B10G10R10SnormPack32: return "2B10G10R10SnormPack32";
    case Format::A2B10G10R10UscaledPack32: return "2B10G10R10UscaledPack32";
    case Format::A2B10G10R10SscaledPack32: return "2B10G10R10SscaledPack32";
    case Format::A2B10G10R10UintPack32: return "2B10G10R10UintPack32";
    case Format::A2B10G10R10SintPack32: return "2B10G10R10SintPack32";
    case Format::R16Unorm: return "16Unorm";
    case Format::R16Snorm: return "16Snorm";
    case Format::R16Uscaled: return "16Uscaled";
    case Format::R16Sscaled: return "16Sscaled";
    case Format::R16Uint: return "16Uint";
    case Format::R16Sint: return "16Sint";
    case Format::R16Sfloat: return "16Sfloat";
    case Format::R16G16Unorm: return "16G16Unorm";
    case Format::R16G16Snorm: return "16G16Snorm";
    case Format::R16G16Uscaled: return "16G16Uscaled";
    case Format::R16G16Sscaled: return "16G16Sscaled";
    case Format::R16G16Uint: return "16G16Uint";
    case Format::R16G16Sint: return "16G16Sint";
    case Format::R16G16Sfloat: return "16G16Sfloat";
    case Format::R16G16B16Unorm: return "16G16B16Unorm";
    case Format::R16G16B16Snorm: return "16G16B16Snorm";
    case Format::R16G16B16Uscaled: return "16G16B16Uscaled";
    case Format::R16G16B16Sscaled: return "16G16B16Sscaled";
    case Format::R16G16B16Uint: return "16G16B16Uint";
    case Format::R16G16B16Sint: return "16G16B16Sint";
    case Format::R16G16B16Sfloat: return "16G16B16Sfloat";
    case Format::R16G16B16A16Unorm: return "16G16B16A16Unorm";
    case Format::R16G16B16A16Snorm: return "16G16B16A16Snorm";
    case Format::R16G16B16A16Uscaled: return "16G16B16A16Uscaled";
    case Format::R16G16B16A16Sscaled: return "16G16B16A16Sscaled";
    case Format::R16G16B16A16Uint: return "16G16B16A16Uint";
    case Format::R16G16B16A16Sint: return "16G16B16A16Sint";
    case Format::R16G16B16A16Sfloat: return "16G16B16A16Sfloat";
    case Format::R32Uint: return "32Uint";
    case Format::R32Sint: return "32Sint";
    case Format::R32Sfloat: return "32Sfloat";
    case Format::R32G32Uint: return "32G32Uint";
    case Format::R32G32Sint: return "32G32Sint";
    case Format::R32G32Sfloat: return "32G32Sfloat";
    case Format::R32G32B32Uint: return "32G32B32Uint";
    case Format::R32G32B32Sint: return "32G32B32Sint";
    case Format::R32G32B32Sfloat: return "32G32B32Sfloat";
    case Format::R32G32B32A32Uint: return "32G32B32A32Uint";
    case Format::R32G32B32A32Sint: return "32G32B32A32Sint";
    case Format::R32G32B32A32Sfloat: return "32G32B32A32Sfloat";
    case Format::R64Uint: return "64Uint";
    case Format::R64Sint: return "64Sint";
    case Format::R64Sfloat: return "64Sfloat";
    case Format::R64G64Uint: return "64G64Uint";
    case Format::R64G64Sint: return "64G64Sint";
    case Format::R64G64Sfloat: return "64G64Sfloat";
    case Format::R64G64B64Uint: return "64G64B64Uint";
    case Format::R64G64B64Sint: return "64G64B64Sint";
    case Format::R64G64B64Sfloat: return "64G64B64Sfloat";
    case Format::R64G64B64A64Uint: return "64G64B64A64Uint";
    case Format::R64G64B64A64Sint: return "64G64B64A64Sint";
    case Format::R64G64B64A64Sfloat: return "64G64B64A64Sfloat";
    case Format::B10G11R11UfloatPack32: return "10G11R11UfloatPack32";
    case Format::E5B9G9R9UfloatPack32: return "5B9G9R9UfloatPack32";
    case Format::D16Unorm: return "16Unorm";
    case Format::X8D24UnormPack32: return "8D24UnormPack32";
    case Format::D32Sfloat: return "32Sfloat";
    case Format::S8Uint: return "8Uint";
    case Format::D16UnormS8Uint: return "16UnormS8Uint";
    case Format::D24UnormS8Uint: return "24UnormS8Uint";
    case Format::D32SfloatS8Uint: return "32SfloatS8Uint";
    case Format::Bc1RgbUnormBlock: return "c1RgbUnormBlock";
    case Format::Bc1RgbSrgbBlock: return "c1RgbSrgbBlock";
    case Format::Bc1RgbaUnormBlock: return "c1RgbaUnormBlock";
    case Format::Bc1RgbaSrgbBlock: return "c1RgbaSrgbBlock";
    case Format::Bc2UnormBlock: return "c2UnormBlock";
    case Format::Bc2SrgbBlock: return "c2SrgbBlock";
    case Format::Bc3UnormBlock: return "c3UnormBlock";
    case Format::Bc3SrgbBlock: return "c3SrgbBlock";
    case Format::Bc4UnormBlock: return "c4UnormBlock";
    case Format::Bc4SnormBlock: return "c4SnormBlock";
    case Format::Bc5UnormBlock: return "c5UnormBlock";
    case Format::Bc5SnormBlock: return "c5SnormBlock";
    case Format::Bc6HUfloatBlock: return "c6HUfloatBlock";
    case Format::Bc6HSfloatBlock: return "c6HSfloatBlock";
    case Format::Bc7UnormBlock: return "c7UnormBlock";
    case Format::Bc7SrgbBlock: return "c7SrgbBlock";
    case Format::Etc2R8G8B8UnormBlock: return "tc2R8G8B8UnormBlock";
    case Format::Etc2R8G8B8SrgbBlock: return "tc2R8G8B8SrgbBlock";
    case Format::Etc2R8G8B8A1UnormBlock: return "tc2R8G8B8A1UnormBlock";
    case Format::Etc2R8G8B8A1SrgbBlock: return "tc2R8G8B8A1SrgbBlock";
    case Format::Etc2R8G8B8A8UnormBlock: return "tc2R8G8B8A8UnormBlock";
    case Format::Etc2R8G8B8A8SrgbBlock: return "tc2R8G8B8A8SrgbBlock";
    case Format::EacR11UnormBlock: return "acR11UnormBlock";
    case Format::EacR11SnormBlock: return "acR11SnormBlock";
    case Format::EacR11G11UnormBlock: return "acR11G11UnormBlock";
    case Format::EacR11G11SnormBlock: return "acR11G11SnormBlock";
    case Format::Astc4x4UnormBlock: return "stc4x4UnormBlock";
    case Format::Astc4x4SrgbBlock: return "stc4x4SrgbBlock";
    case Format::Astc5x4UnormBlock: return "stc5x4UnormBlock";
    case Format::Astc5x4SrgbBlock: return "stc5x4SrgbBlock";
    case Format::Astc5x5UnormBlock: return "stc5x5UnormBlock";
    case Format::Astc5x5SrgbBlock: return "stc5x5SrgbBlock";
    case Format::Astc6x5UnormBlock: return "stc6x5UnormBlock";
    case Format::Astc6x5SrgbBlock: return "stc6x5SrgbBlock";
    case Format::Astc6x6UnormBlock: return "stc6x6UnormBlock";
    case Format::Astc6x6SrgbBlock: return "stc6x6SrgbBlock";
    case Format::Astc8x5UnormBlock: return "stc8x5UnormBlock";
    case Format::Astc8x5SrgbBlock: return "stc8x5SrgbBlock";
    case Format::Astc8x6UnormBlock: return "stc8x6UnormBlock";
    case Format::Astc8x6SrgbBlock: return "stc8x6SrgbBlock";
    case Format::Astc8x8UnormBlock: return "stc8x8UnormBlock";
    case Format::Astc8x8SrgbBlock: return "stc8x8SrgbBlock";
    case Format::Astc10x5UnormBlock: return "stc10x5UnormBlock";
    case Format::Astc10x5SrgbBlock: return "stc10x5SrgbBlock";
    case Format::Astc10x6UnormBlock: return "stc10x6UnormBlock";
    case Format::Astc10x6SrgbBlock: return "stc10x6SrgbBlock";
    case Format::Astc10x8UnormBlock: return "stc10x8UnormBlock";
    case Format::Astc10x8SrgbBlock: return "stc10x8SrgbBlock";
    case Format::Astc10x10UnormBlock: return "stc10x10UnormBlock";
    case Format::Astc10x10SrgbBlock: return "stc10x10SrgbBlock";
    case Format::Astc12x10UnormBlock: return "stc12x10UnormBlock";
    case Format::Astc12x10SrgbBlock: return "stc12x10SrgbBlock";
    case Format::Astc12x12UnormBlock: return "stc12x12UnormBlock";
    case Format::Astc12x12SrgbBlock: return "stc12x12SrgbBlock";
    default: return "unknown";
    }
  }

  static const char * getString(StructureType value)
  {
    switch (value)
    {
    case StructureType::ApplicationInfo: return "pplicationInfo";
    case StructureType::InstanceCreateInfo: return "nstanceCreateInfo";
    case StructureType::DeviceQueueCreateInfo: return "eviceQueueCreateInfo";
    case StructureType::DeviceCreateInfo: return "eviceCreateInfo";
    case StructureType::SubmitInfo: return "ubmitInfo";
    case StructureType::MemoryAllocateInfo: return "emoryAllocateInfo";
    case StructureType::MappedMemoryRange: return "appedMemoryRange";
    case StructureType::BindSparseInfo: return "indSparseInfo";
    case StructureType::FenceCreateInfo: return "enceCreateInfo";
    case StructureType::SemaphoreCreateInfo: return "emaphoreCreateInfo";
    case StructureType::EventCreateInfo: return "ventCreateInfo";
    case StructureType::QueryPoolCreateInfo: return "ueryPoolCreateInfo";
    case StructureType::BufferCreateInfo: return "ufferCreateInfo";
    case StructureType::BufferViewCreateInfo: return "ufferViewCreateInfo";
    case StructureType::ImageCreateInfo: return "mageCreateInfo";
    case StructureType::ImageViewCreateInfo: return "mageViewCreateInfo";
    case StructureType::ShaderModuleCreateInfo: return "haderModuleCreateInfo";
    case StructureType::PipelineCacheCreateInfo: return "ipelineCacheCreateInfo";
    case StructureType::PipelineShaderStageCreateInfo: return "ipelineShaderStageCreateInfo";
    case StructureType::PipelineVertexInputStateCreateInfo: return "ipelineVertexInputStateCreateInfo";
    case StructureType::PipelineInputAssemblyStateCreateInfo: return "ipelineInputAssemblyStateCreateInfo";
    case StructureType::PipelineTessellationStateCreateInfo: return "ipelineTessellationStateCreateInfo";
    case StructureType::PipelineViewportStateCreateInfo: return "ipelineViewportStateCreateInfo";
    case StructureType::PipelineRasterizationStateCreateInfo: return "ipelineRasterizationStateCreateInfo";
    case StructureType::PipelineMultisampleStateCreateInfo: return "ipelineMultisampleStateCreateInfo";
    case StructureType::PipelineDepthStencilStateCreateInfo: return "ipelineDepthStencilStateCreateInfo";
    case StructureType::PipelineColorBlendStateCreateInfo: return "ipelineColorBlendStateCreateInfo";
    case StructureType::PipelineDynamicStateCreateInfo: return "ipelineDynamicStateCreateInfo";
    case StructureType::GraphicsPipelineCreateInfo: return "raphicsPipelineCreateInfo";
    case StructureType::ComputePipelineCreateInfo: return "omputePipelineCreateInfo";
    case StructureType::PipelineLayoutCreateInfo: return "ipelineLayoutCreateInfo";
    case StructureType::SamplerCreateInfo: return "amplerCreateInfo";
    case StructureType::DescriptorSetLayoutCreateInfo: return "escriptorSetLayoutCreateInfo";
    case StructureType::DescriptorPoolCreateInfo: return "escriptorPoolCreateInfo";
    case StructureType::DescriptorSetAllocateInfo: return "escriptorSetAllocateInfo";
    case StructureType::WriteDescriptorSet: return "riteDescriptorSet";
    case StructureType::CopyDescriptorSet: return "opyDescriptorSet";
    case StructureType::FramebufferCreateInfo: return "ramebufferCreateInfo";
    case StructureType::RenderPassCreateInfo: return "enderPassCreateInfo";
    case StructureType::CommandPoolCreateInfo: return "ommandPoolCreateInfo";
    case StructureType::CommandBufferAllocateInfo: return "ommandBufferAllocateInfo";
    case StructureType::CommandBufferInheritanceInfo: return "ommandBufferInheritanceInfo";
    case StructureType::CommandBufferBeginInfo: return "ommandBufferBeginInfo";
    case StructureType::RenderPassBeginInfo: return "enderPassBeginInfo";
    case StructureType::BufferMemoryBarrier: return "ufferMemoryBarrier";
    case StructureType::ImageMemoryBarrier: return "mageMemoryBarrier";
    case StructureType::MemoryBarrier: return "emoryBarrier";
    case StructureType::LoaderInstanceCreateInfo: return "oaderInstanceCreateInfo";
    case StructureType::LoaderDeviceCreateInfo: return "oaderDeviceCreateInfo";
    case StructureType::SwapchainCreateInfoKHR: return "wapchainCreateInfoKHR";
    case StructureType::PresentInfoKHR: return "resentInfoKHR";
    case StructureType::DisplayModeCreateInfoKHR: return "isplayModeCreateInfoKHR";
    case StructureType::DisplaySurfaceCreateInfoKHR: return "isplaySurfaceCreateInfoKHR";
    case StructureType::DisplayPresentInfoKHR: return "isplayPresentInfoKHR";
    case StructureType::XlibSurfaceCreateInfoKHR: return "libSurfaceCreateInfoKHR";
    case StructureType::XcbSurfaceCreateInfoKHR: return "cbSurfaceCreateInfoKHR";
    case StructureType::WaylandSurfaceCreateInfoKHR: return "aylandSurfaceCreateInfoKHR";
    case StructureType::MirSurfaceCreateInfoKHR: return "irSurfaceCreateInfoKHR";
    case StructureType::AndroidSurfaceCreateInfoKHR: return "ndroidSurfaceCreateInfoKHR";
    case StructureType::Win32SurfaceCreateInfoKHR: return "in32SurfaceCreateInfoKHR";
    case StructureType::DebugReportCreateInfoEXT: return "ebugReportCreateInfoEXT";
    default: return "unknown";
    }
  }

  static const char * getString(SubpassContents value)
  {
    switch (value)
    {
    case SubpassContents::Inline: return "nline";
    case SubpassContents::SecondaryCommandBuffers: return "econdaryCommandBuffers";
    default: return "unknown";
    }
  }

  static const char * getString(Result value)
  {
    switch (value)
    {
    case Result::Success: return "uccess";
    case Result::NotReady: return "otReady";
    case Result::Timeout: return "imeout";
    case Result::EventSet: return "ventSet";
    case Result::EventReset: return "ventReset";
    case Result::Incomplete: return "ncomplete";
    case Result::ErrorOutOfHostMemory: return "rrorOutOfHostMemory";
    case Result::ErrorOutOfDeviceMemory: return "rrorOutOfDeviceMemory";
    case Result::ErrorInitializationFailed: return "rrorInitializationFailed";
    case Result::ErrorDeviceLost: return "rrorDeviceLost";
    case Result::ErrorMemoryMapFailed: return "rrorMemoryMapFailed";
    case Result::ErrorLayerNotPresent: return "rrorLayerNotPresent";
    case Result::ErrorExtensionNotPresent: return "rrorExtensionNotPresent";
    case Result::ErrorFeatureNotPresent: return "rrorFeatureNotPresent";
    case Result::ErrorIncompatibleDriver: return "rrorIncompatibleDriver";
    case Result::ErrorTooManyObjects: return "rrorTooManyObjects";
    case Result::ErrorFormatNotSupported: return "rrorFormatNotSupported";
    case Result::ErrorSurfaceLostKHR: return "rrorSurfaceLostKHR";
    case Result::ErrorNativeWindowInUseKHR: return "rrorNativeWindowInUseKHR";
    case Result::SuboptimalKHR: return "uboptimalKHR";
    case Result::ErrorOutOfDateKHR: return "rrorOutOfDateKHR";
    case Result::ErrorIncompatibleDisplayKHR: return "rrorIncompatibleDisplayKHR";
    case Result::ErrorValidationFailedEXT: return "rrorValidationFailedEXT";
    default: return "unknown";
    }
  }

  static const char * getString(DynamicState value)
  {
    switch (value)
    {
    case DynamicState::Viewport: return "iewport";
    case DynamicState::Scissor: return "cissor";
    case DynamicState::LineWidth: return "ineWidth";
    case DynamicState::DepthBias: return "epthBias";
    case DynamicState::BlendConstants: return "lendConstants";
    case DynamicState::DepthBounds: return "epthBounds";
    case DynamicState::StencilCompareMask: return "tencilCompareMask";
    case DynamicState::StencilWriteMask: return "tencilWriteMask";
    case DynamicState::StencilReference: return "tencilReference";
    default: return "unknown";
    }
  }

  static const char * getString(QueueFlagBits value)
  {
    switch (value)
    {
    case QueueFlagBits::Graphics: return "raphics";
    case QueueFlagBits::Compute: return "ompute";
    case QueueFlagBits::Transfer: return "ransfer";
    case QueueFlagBits::SparseBinding: return "parseBinding";
    default: return "unknown";
    }
  }

  static std::string getString(QueueFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::QueueFlagBits::Graphics) result += "raphics | ";
    if (value & vk::QueueFlagBits::Compute) result += "ompute | ";
    if (value & vk::QueueFlagBits::Transfer) result += "ransfer | ";
    if (value & vk::QueueFlagBits::SparseBinding) result += "parseBinding | ";
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * getString(MemoryPropertyFlagBits value)
  {
    switch (value)
    {
    case MemoryPropertyFlagBits::DeviceLocal: return "eviceLocal";
    case MemoryPropertyFlagBits::HostVisible: return "ostVisible";
    case MemoryPropertyFlagBits::HostCoherent: return "ostCoherent";
    case MemoryPropertyFlagBits::HostCached: return "ostCached";
    case MemoryPropertyFlagBits::LazilyAllocated: return "azilyAllocated";
    default: return "unknown";
    }
  }

  static std::string getString(MemoryPropertyFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::MemoryPropertyFlagBits::DeviceLocal) result += "eviceLocal | ";
    if (value & vk::MemoryPropertyFlagBits::HostVisible) result += "ostVisible | ";
    if (value & vk::MemoryPropertyFlagBits::HostCoherent) result += "ostCoherent | ";
    if (value & vk::MemoryPropertyFlagBits::HostCached) result += "ostCached | ";
    if (value & vk::MemoryPropertyFlagBits::LazilyAllocated) result += "azilyAllocated | ";
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * getString(MemoryHeapFlagBits value)
  {
    switch (value)
    {
    case MemoryHeapFlagBits::DeviceLocal: return "eviceLocal";
    default: return "unknown";
    }
  }

  static std::string getString(MemoryHeapFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::MemoryHeapFlagBits::DeviceLocal) result += "eviceLocal | ";
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * getString(AccessFlagBits value)
  {
    switch (value)
    {
    case AccessFlagBits::IndirectCommandRead: return "ndirectCommandRead";
    case AccessFlagBits::IndexRead: return "ndexRead";
    case AccessFlagBits::VertexAttributeRead: return "ertexAttributeRead";
    case AccessFlagBits::UniformRead: return "niformRead";
    case AccessFlagBits::InputAttachmentRead: return "nputAttachmentRead";
    case AccessFlagBits::ShaderRead: return "haderRead";
    case AccessFlagBits::ShaderWrite: return "haderWrite";
    case AccessFlagBits::ColorAttachmentRead: return "olorAttachmentRead";
    case AccessFlagBits::ColorAttachmentWrite: return "olorAttachmentWrite";
    case AccessFlagBits::DepthStencilAttachmentRead: return "epthStencilAttachmentRead";
    case AccessFlagBits::DepthStencilAttachmentWrite: return "epthStencilAttachmentWrite";
    case AccessFlagBits::TransferRead: return "ransferRead";
    case AccessFlagBits::TransferWrite: return "ransferWrite";
    case AccessFlagBits::HostRead: return "ostRead";
    case AccessFlagBits::HostWrite: return "ostWrite";
    case AccessFlagBits::MemoryRead: return "emoryRead";
    case AccessFlagBits::MemoryWrite: return "emoryWrite";
    default: return "unknown";
    }
  }

  static std::string getString(AccessFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::AccessFlagBits::IndirectCommandRead) result += "ndirectCommandRead | ";
    if (value & vk::AccessFlagBits::IndexRead) result += "ndexRead | ";
    if (value & vk::AccessFlagBits::VertexAttributeRead) result += "ertexAttributeRead | ";
    if (value & vk::AccessFlagBits::UniformRead) result += "niformRead | ";
    if (value & vk::AccessFlagBits::InputAttachmentRead) result += "nputAttachmentRead | ";
    if (value & vk::AccessFlagBits::ShaderRead) result += "haderRead | ";
    if (value & vk::AccessFlagBits::ShaderWrite) result += "haderWrite | ";
    if (value & vk::AccessFlagBits::ColorAttachmentRead) result += "olorAttachmentRead | ";
    if (value & vk::AccessFlagBits::ColorAttachmentWrite) result += "olorAttachmentWrite | ";
    if (value & vk::AccessFlagBits::DepthStencilAttachmentRead) result += "epthStencilAttachmentRead | ";
    if (value & vk::AccessFlagBits::DepthStencilAttachmentWrite) result += "epthStencilAttachmentWrite | ";
    if (value & vk::AccessFlagBits::TransferRead) result += "ransferRead | ";
    if (value & vk::AccessFlagBits::TransferWrite) result += "ransferWrite | ";
    if (value & vk::AccessFlagBits::HostRead) result += "ostRead | ";
    if (value & vk::AccessFlagBits::HostWrite) result += "ostWrite | ";
    if (value & vk::AccessFlagBits::MemoryRead) result += "emoryRead | ";
    if (value & vk::AccessFlagBits::MemoryWrite) result += "emoryWrite | ";
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * getString(BufferUsageFlagBits value)
  {
    switch (value)
    {
    case BufferUsageFlagBits::TransferSrc: return "ransferSrc";
    case BufferUsageFlagBits::TransferDst: return "ransferDst";
    case BufferUsageFlagBits::UniformTexelBuffer: return "niformTexelBuffer";
    case BufferUsageFlagBits::StorageTexelBuffer: return "torageTexelBuffer";
    case BufferUsageFlagBits::UniformBuffer: return "niformBuffer";
    case BufferUsageFlagBits::StorageBuffer: return "torageBuffer";
    case BufferUsageFlagBits::IndexBuffer: return "ndexBuffer";
    case BufferUsageFlagBits::VertexBuffer: return "ertexBuffer";
    case BufferUsageFlagBits::IndirectBuffer: return "ndirectBuffer";
    default: return "unknown";
    }
  }

  static std::string getString(BufferUsageFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::BufferUsageFlagBits::TransferSrc) result += "ransferSrc | ";
    if (value & vk::BufferUsageFlagBits::TransferDst) result += "ransferDst | ";
    if (value & vk::BufferUsageFlagBits::UniformTexelBuffer) result += "niformTexelBuffer | ";
    if (value & vk::BufferUsageFlagBits::StorageTexelBuffer) result += "torageTexelBuffer | ";
    if (value & vk::BufferUsageFlagBits::UniformBuffer) result += "niformBuffer | ";
    if (value & vk::BufferUsageFlagBits::StorageBuffer) result += "torageBuffer | ";
    if (value & vk::BufferUsageFlagBits::IndexBuffer) result += "ndexBuffer | ";
    if (value & vk::BufferUsageFlagBits::VertexBuffer) result += "ertexBuffer | ";
    if (value & vk::BufferUsageFlagBits::IndirectBuffer) result += "ndirectBuffer | ";
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * getString(BufferCreateFlagBits value)
  {
    switch (value)
    {
    case BufferCreateFlagBits::SparseBinding: return "parseBinding";
    case BufferCreateFlagBits::SparseResidency: return "parseResidency";
    case BufferCreateFlagBits::SparseAliased: return "parseAliased";
    default: return "unknown";
    }
  }

  static std::string getString(BufferCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::BufferCreateFlagBits::SparseBinding) result += "parseBinding | ";
    if (value & vk::BufferCreateFlagBits::SparseResidency) result += "parseResidency | ";
    if (value & vk::BufferCreateFlagBits::SparseAliased) result += "parseAliased | ";
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * getString(ShaderStageFlagBits value)
  {
    switch (value)
    {
    case ShaderStageFlagBits::Vertex: return "ertex";
    case ShaderStageFlagBits::TessellationControl: return "essellationControl";
    case ShaderStageFlagBits::TessellationEvaluation: return "essellationEvaluation";
    case ShaderStageFlagBits::Geometry: return "eometry";
    case ShaderStageFlagBits::Fragment: return "ragment";
    case ShaderStageFlagBits::Compute: return "ompute";
    case ShaderStageFlagBits::AllGraphics: return "llGraphics";
    case ShaderStageFlagBits::All: return "ll";
    default: return "unknown";
    }
  }

  static std::string getString(ShaderStageFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::ShaderStageFlagBits::Vertex) result += "ertex | ";
    if (value & vk::ShaderStageFlagBits::TessellationControl) result += "essellationControl | ";
    if (value & vk::ShaderStageFlagBits::TessellationEvaluation) result += "essellationEvaluation | ";
    if (value & vk::ShaderStageFlagBits::Geometry) result += "eometry | ";
    if (value & vk::ShaderStageFlagBits::Fragment) result += "ragment | ";
    if (value & vk::ShaderStageFlagBits::Compute) result += "ompute | ";
    if (value & vk::ShaderStageFlagBits::AllGraphics) result += "llGraphics | ";
    if (value & vk::ShaderStageFlagBits::All) result += "ll | ";
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * getString(ImageUsageFlagBits value)
  {
    switch (value)
    {
    case ImageUsageFlagBits::TransferSrc: return "ransferSrc";
    case ImageUsageFlagBits::TransferDst: return "ransferDst";
    case ImageUsageFlagBits::Sampled: return "ampled";
    case ImageUsageFlagBits::Storage: return "torage";
    case ImageUsageFlagBits::ColorAttachment: return "olorAttachment";
    case ImageUsageFlagBits::DepthStencilAttachment: return "epthStencilAttachment";
    case ImageUsageFlagBits::TransientAttachment: return "ransientAttachment";
    case ImageUsageFlagBits::InputAttachment: return "nputAttachment";
    default: return "unknown";
    }
  }

  static std::string getString(ImageUsageFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::ImageUsageFlagBits::TransferSrc) result += "ransferSrc | ";
    if (value & vk::ImageUsageFlagBits::TransferDst) result += "ransferDst | ";
    if (value & vk::ImageUsageFlagBits::Sampled) result += "ampled | ";
    if (value & vk::ImageUsageFlagBits::Storage) result += "torage | ";
    if (value & vk::ImageUsageFlagBits::ColorAttachment) result += "olorAttachment | ";
    if (value & vk::ImageUsageFlagBits::DepthStencilAttachment) result += "epthStencilAttachment | ";
    if (value & vk::ImageUsageFlagBits::TransientAttachment) result += "ransientAttachment | ";
    if (value & vk::ImageUsageFlagBits::InputAttachment) result += "nputAttachment | ";
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * getString(ImageCreateFlagBits value)
  {
    switch (value)
    {
    case ImageCreateFlagBits::SparseBinding: return "parseBinding";
    case ImageCreateFlagBits::SparseResidency: return "parseResidency";
    case ImageCreateFlagBits::SparseAliased: return "parseAliased";
    case ImageCreateFlagBits::MutableFormat: return "utableFormat";
    case ImageCreateFlagBits::CubeCompatible: return "ubeCompatible";
    default: return "unknown";
    }
  }

  static std::string getString(ImageCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::ImageCreateFlagBits::SparseBinding) result += "parseBinding | ";
    if (value & vk::ImageCreateFlagBits::SparseResidency) result += "parseResidency | ";
    if (value & vk::ImageCreateFlagBits::SparseAliased) result += "parseAliased | ";
    if (value & vk::ImageCreateFlagBits::MutableFormat) result += "utableFormat | ";
    if (value & vk::ImageCreateFlagBits::CubeCompatible) result += "ubeCompatible | ";
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * getString(PipelineCreateFlagBits value)
  {
    switch (value)
    {
    case PipelineCreateFlagBits::DisableOptimization: return "isableOptimization";
    case PipelineCreateFlagBits::AllowDerivatives: return "llowDerivatives";
    case PipelineCreateFlagBits::Derivative: return "erivative";
    default: return "unknown";
    }
  }

  static std::string getString(PipelineCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::PipelineCreateFlagBits::DisableOptimization) result += "isableOptimization | ";
    if (value & vk::PipelineCreateFlagBits::AllowDerivatives) result += "llowDerivatives | ";
    if (value & vk::PipelineCreateFlagBits::Derivative) result += "erivative | ";
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * getString(ColorComponentFlagBits value)
  {
    switch (value)
    {
    case ColorComponentFlagBits::R: return "";
    case ColorComponentFlagBits::G: return "";
    case ColorComponentFlagBits::B: return "";
    case ColorComponentFlagBits::A: return "";
    default: return "unknown";
    }
  }

  static std::string getString(ColorComponentFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::ColorComponentFlagBits::R) result += " | ";
    if (value & vk::ColorComponentFlagBits::G) result += " | ";
    if (value & vk::ColorComponentFlagBits::B) result += " | ";
    if (value & vk::ColorComponentFlagBits::A) result += " | ";
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * getString(FenceCreateFlagBits value)
  {
    switch (value)
    {
    case FenceCreateFlagBits::Signaled: return "ignaled";
    default: return "unknown";
    }
  }

  static std::string getString(FenceCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::FenceCreateFlagBits::Signaled) result += "ignaled | ";
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * getString(FormatFeatureFlagBits value)
  {
    switch (value)
    {
    case FormatFeatureFlagBits::SampledImage: return "ampledImage";
    case FormatFeatureFlagBits::StorageImage: return "torageImage";
    case FormatFeatureFlagBits::StorageImageAtomic: return "torageImageAtomic";
    case FormatFeatureFlagBits::UniformTexelBuffer: return "niformTexelBuffer";
    case FormatFeatureFlagBits::StorageTexelBuffer: return "torageTexelBuffer";
    case FormatFeatureFlagBits::StorageTexelBufferAtomic: return "torageTexelBufferAtomic";
    case FormatFeatureFlagBits::VertexBuffer: return "ertexBuffer";
    case FormatFeatureFlagBits::ColorAttachment: return "olorAttachment";
    case FormatFeatureFlagBits::ColorAttachmentBlend: return "olorAttachmentBlend";
    case FormatFeatureFlagBits::DepthStencilAttachment: return "epthStencilAttachment";
    case FormatFeatureFlagBits::BlitSrc: return "litSrc";
    case FormatFeatureFlagBits::BlitDst: return "litDst";
    case FormatFeatureFlagBits::SampledImageFilterLinear: return "ampledImageFilterLinear";
    default: return "unknown";
    }
  }

  static std::string getString(FormatFeatureFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::FormatFeatureFlagBits::SampledImage) result += "ampledImage | ";
    if (value & vk::FormatFeatureFlagBits::StorageImage) result += "torageImage | ";
    if (value & vk::FormatFeatureFlagBits::StorageImageAtomic) result += "torageImageAtomic | ";
    if (value & vk::FormatFeatureFlagBits::UniformTexelBuffer) result += "niformTexelBuffer | ";
    if (value & vk::FormatFeatureFlagBits::StorageTexelBuffer) result += "torageTexelBuffer | ";
    if (value & vk::FormatFeatureFlagBits::StorageTexelBufferAtomic) result += "torageTexelBufferAtomic | ";
    if (value & vk::FormatFeatureFlagBits::VertexBuffer) result += "ertexBuffer | ";
    if (value & vk::FormatFeatureFlagBits::ColorAttachment) result += "olorAttachment | ";
    if (value & vk::FormatFeatureFlagBits::ColorAttachmentBlend) result += "olorAttachmentBlend | ";
    if (value & vk::FormatFeatureFlagBits::DepthStencilAttachment) result += "epthStencilAttachment | ";
    if (value & vk::FormatFeatureFlagBits::BlitSrc) result += "litSrc | ";
    if (value & vk::FormatFeatureFlagBits::BlitDst) result += "litDst | ";
    if (value & vk::FormatFeatureFlagBits::SampledImageFilterLinear) result += "ampledImageFilterLinear | ";
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * getString(QueryControlFlagBits value)
  {
    switch (value)
    {
    case QueryControlFlagBits::Precise: return "recise";
    default: return "unknown";
    }
  }

  static std::string getString(QueryControlFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::QueryControlFlagBits::Precise) result += "recise | ";
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * getString(QueryResultFlagBits value)
  {
    switch (value)
    {
    case QueryResultFlagBits::e64: return "64";
    case QueryResultFlagBits::Wait: return "ait";
    case QueryResultFlagBits::WithAvailability: return "ithAvailability";
    case QueryResultFlagBits::Partial: return "artial";
    default: return "unknown";
    }
  }

  static std::string getString(QueryResultFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::QueryResultFlagBits::e64) result += "64 | ";
    if (value & vk::QueryResultFlagBits::Wait) result += "ait | ";
    if (value & vk::QueryResultFlagBits::WithAvailability) result += "ithAvailability | ";
    if (value & vk::QueryResultFlagBits::Partial) result += "artial | ";
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * getString(CommandBufferUsageFlagBits value)
  {
    switch (value)
    {
    case CommandBufferUsageFlagBits::OneTimeSubmit: return "neTimeSubmit";
    case CommandBufferUsageFlagBits::RenderPassContinue: return "enderPassContinue";
    case CommandBufferUsageFlagBits::SimultaneousUse: return "imultaneousUse";
    default: return "unknown";
    }
  }

  static std::string getString(CommandBufferUsageFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::CommandBufferUsageFlagBits::OneTimeSubmit) result += "neTimeSubmit | ";
    if (value & vk::CommandBufferUsageFlagBits::RenderPassContinue) result += "enderPassContinue | ";
    if (value & vk::CommandBufferUsageFlagBits::SimultaneousUse) result += "imultaneousUse | ";
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * getString(QueryPipelineStatisticFlagBits value)
  {
    switch (value)
    {
    case QueryPipelineStatisticFlagBits::InputAssemblyVertices: return "nputAssemblyVertices";
    case QueryPipelineStatisticFlagBits::InputAssemblyPrimitives: return "nputAssemblyPrimitives";
    case QueryPipelineStatisticFlagBits::VertexShaderInvocations: return "ertexShaderInvocations";
    case QueryPipelineStatisticFlagBits::GeometryShaderInvocations: return "eometryShaderInvocations";
    case QueryPipelineStatisticFlagBits::GeometryShaderPrimitives: return "eometryShaderPrimitives";
    case QueryPipelineStatisticFlagBits::ClippingInvocations: return "lippingInvocations";
    case QueryPipelineStatisticFlagBits::ClippingPrimitives: return "lippingPrimitives";
    case QueryPipelineStatisticFlagBits::FragmentShaderInvocations: return "ragmentShaderInvocations";
    case QueryPipelineStatisticFlagBits::TessellationControlShaderPatches: return "essellationControlShaderPatches";
    case QueryPipelineStatisticFlagBits::TessellationEvaluationShaderInvocations: return "essellationEvaluationShaderInvocations";
    case QueryPipelineStatisticFlagBits::ComputeShaderInvocations: return "omputeShaderInvocations";
    default: return "unknown";
    }
  }

  static std::string getString(QueryPipelineStatisticFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::QueryPipelineStatisticFlagBits::InputAssemblyVertices) result += "nputAssemblyVertices | ";
    if (value & vk::QueryPipelineStatisticFlagBits::InputAssemblyPrimitives) result += "nputAssemblyPrimitives | ";
    if (value & vk::QueryPipelineStatisticFlagBits::VertexShaderInvocations) result += "ertexShaderInvocations | ";
    if (value & vk::QueryPipelineStatisticFlagBits::GeometryShaderInvocations) result += "eometryShaderInvocations | ";
    if (value & vk::QueryPipelineStatisticFlagBits::GeometryShaderPrimitives) result += "eometryShaderPrimitives | ";
    if (value & vk::QueryPipelineStatisticFlagBits::ClippingInvocations) result += "lippingInvocations | ";
    if (value & vk::QueryPipelineStatisticFlagBits::ClippingPrimitives) result += "lippingPrimitives | ";
    if (value & vk::QueryPipelineStatisticFlagBits::FragmentShaderInvocations) result += "ragmentShaderInvocations | ";
    if (value & vk::QueryPipelineStatisticFlagBits::TessellationControlShaderPatches) result += "essellationControlShaderPatches | ";
    if (value & vk::QueryPipelineStatisticFlagBits::TessellationEvaluationShaderInvocations) result += "essellationEvaluationShaderInvocations | ";
    if (value & vk::QueryPipelineStatisticFlagBits::ComputeShaderInvocations) result += "omputeShaderInvocations | ";
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * getString(ImageAspectFlagBits value)
  {
    switch (value)
    {
    case ImageAspectFlagBits::Color: return "olor";
    case ImageAspectFlagBits::Depth: return "epth";
    case ImageAspectFlagBits::Stencil: return "tencil";
    case ImageAspectFlagBits::Metadata: return "etadata";
    default: return "unknown";
    }
  }

  static std::string getString(ImageAspectFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::ImageAspectFlagBits::Color) result += "olor | ";
    if (value & vk::ImageAspectFlagBits::Depth) result += "epth | ";
    if (value & vk::ImageAspectFlagBits::Stencil) result += "tencil | ";
    if (value & vk::ImageAspectFlagBits::Metadata) result += "etadata | ";
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * getString(SparseImageFormatFlagBits value)
  {
    switch (value)
    {
    case SparseImageFormatFlagBits::SingleMiptail: return "ingleMiptail";
    case SparseImageFormatFlagBits::AlignedMipSize: return "lignedMipSize";
    case SparseImageFormatFlagBits::NonstandardBlockSize: return "onstandardBlockSize";
    default: return "unknown";
    }
  }

  static std::string getString(SparseImageFormatFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::SparseImageFormatFlagBits::SingleMiptail) result += "ingleMiptail | ";
    if (value & vk::SparseImageFormatFlagBits::AlignedMipSize) result += "lignedMipSize | ";
    if (value & vk::SparseImageFormatFlagBits::NonstandardBlockSize) result += "onstandardBlockSize | ";
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * getString(SparseMemoryBindFlagBits value)
  {
    switch (value)
    {
    case SparseMemoryBindFlagBits::Metadata: return "etadata";
    default: return "unknown";
    }
  }

  static std::string getString(SparseMemoryBindFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::SparseMemoryBindFlagBits::Metadata) result += "etadata | ";
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * getString(PipelineStageFlagBits value)
  {
    switch (value)
    {
    case PipelineStageFlagBits::TopOfPipe: return "opOfPipe";
    case PipelineStageFlagBits::DrawIndirect: return "rawIndirect";
    case PipelineStageFlagBits::VertexInput: return "ertexInput";
    case PipelineStageFlagBits::VertexShader: return "ertexShader";
    case PipelineStageFlagBits::TessellationControlShader: return "essellationControlShader";
    case PipelineStageFlagBits::TessellationEvaluationShader: return "essellationEvaluationShader";
    case PipelineStageFlagBits::GeometryShader: return "eometryShader";
    case PipelineStageFlagBits::FragmentShader: return "ragmentShader";
    case PipelineStageFlagBits::EarlyFragmentTests: return "arlyFragmentTests";
    case PipelineStageFlagBits::LateFragmentTests: return "ateFragmentTests";
    case PipelineStageFlagBits::ColorAttachmentOutput: return "olorAttachmentOutput";
    case PipelineStageFlagBits::ComputeShader: return "omputeShader";
    case PipelineStageFlagBits::Transfer: return "ransfer";
    case PipelineStageFlagBits::BottomOfPipe: return "ottomOfPipe";
    case PipelineStageFlagBits::Host: return "ost";
    case PipelineStageFlagBits::AllGraphics: return "llGraphics";
    case PipelineStageFlagBits::AllCommands: return "llCommands";
    default: return "unknown";
    }
  }

  static std::string getString(PipelineStageFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::PipelineStageFlagBits::TopOfPipe) result += "opOfPipe | ";
    if (value & vk::PipelineStageFlagBits::DrawIndirect) result += "rawIndirect | ";
    if (value & vk::PipelineStageFlagBits::VertexInput) result += "ertexInput | ";
    if (value & vk::PipelineStageFlagBits::VertexShader) result += "ertexShader | ";
    if (value & vk::PipelineStageFlagBits::TessellationControlShader) result += "essellationControlShader | ";
    if (value & vk::PipelineStageFlagBits::TessellationEvaluationShader) result += "essellationEvaluationShader | ";
    if (value & vk::PipelineStageFlagBits::GeometryShader) result += "eometryShader | ";
    if (value & vk::PipelineStageFlagBits::FragmentShader) result += "ragmentShader | ";
    if (value & vk::PipelineStageFlagBits::EarlyFragmentTests) result += "arlyFragmentTests | ";
    if (value & vk::PipelineStageFlagBits::LateFragmentTests) result += "ateFragmentTests | ";
    if (value & vk::PipelineStageFlagBits::ColorAttachmentOutput) result += "olorAttachmentOutput | ";
    if (value & vk::PipelineStageFlagBits::ComputeShader) result += "omputeShader | ";
    if (value & vk::PipelineStageFlagBits::Transfer) result += "ransfer | ";
    if (value & vk::PipelineStageFlagBits::BottomOfPipe) result += "ottomOfPipe | ";
    if (value & vk::PipelineStageFlagBits::Host) result += "ost | ";
    if (value & vk::PipelineStageFlagBits::AllGraphics) result += "llGraphics | ";
    if (value & vk::PipelineStageFlagBits::AllCommands) result += "llCommands | ";
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * getString(CommandPoolCreateFlagBits value)
  {
    switch (value)
    {
    case CommandPoolCreateFlagBits::Transient: return "ransient";
    case CommandPoolCreateFlagBits::ResetCommandBuffer: return "esetCommandBuffer";
    default: return "unknown";
    }
  }

  static std::string getString(CommandPoolCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::CommandPoolCreateFlagBits::Transient) result += "ransient | ";
    if (value & vk::CommandPoolCreateFlagBits::ResetCommandBuffer) result += "esetCommandBuffer | ";
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * getString(CommandPoolResetFlagBits value)
  {
    switch (value)
    {
    case CommandPoolResetFlagBits::ReleaseResources: return "eleaseResources";
    default: return "unknown";
    }
  }

  static std::string getString(CommandPoolResetFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::CommandPoolResetFlagBits::ReleaseResources) result += "eleaseResources | ";
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * getString(CommandBufferResetFlagBits value)
  {
    switch (value)
    {
    case CommandBufferResetFlagBits::ReleaseResources: return "eleaseResources";
    default: return "unknown";
    }
  }

  static std::string getString(CommandBufferResetFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::CommandBufferResetFlagBits::ReleaseResources) result += "eleaseResources | ";
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * getString(SampleCountFlagBits value)
  {
    switch (value)
    {
    case SampleCountFlagBits::e1: return "1";
    case SampleCountFlagBits::e2: return "2";
    case SampleCountFlagBits::e4: return "4";
    case SampleCountFlagBits::e8: return "8";
    case SampleCountFlagBits::e16: return "16";
    case SampleCountFlagBits::e32: return "32";
    case SampleCountFlagBits::e64: return "64";
    default: return "unknown";
    }
  }

  static std::string getString(SampleCountFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::SampleCountFlagBits::e1) result += "1 | ";
    if (value & vk::SampleCountFlagBits::e2) result += "2 | ";
    if (value & vk::SampleCountFlagBits::e4) result += "4 | ";
    if (value & vk::SampleCountFlagBits::e8) result += "8 | ";
    if (value & vk::SampleCountFlagBits::e16) result += "16 | ";
    if (value & vk::SampleCountFlagBits::e32) result += "32 | ";
    if (value & vk::SampleCountFlagBits::e64) result += "64 | ";
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * getString(AttachmentDescriptionFlagBits value)
  {
    switch (value)
    {
    case AttachmentDescriptionFlagBits::MayAlias: return "ayAlias";
    default: return "unknown";
    }
  }

  static std::string getString(AttachmentDescriptionFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::AttachmentDescriptionFlagBits::MayAlias) result += "ayAlias | ";
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * getString(StencilFaceFlagBits value)
  {
    switch (value)
    {
    case StencilFaceFlagBits::Front: return "ront";
    case StencilFaceFlagBits::Back: return "ack";
    case StencilFaceFlagBits::VkStencilFrontAndBack: return "kStencilFrontAndBack";
    default: return "unknown";
    }
  }

  static std::string getString(StencilFaceFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::StencilFaceFlagBits::Front) result += "ront | ";
    if (value & vk::StencilFaceFlagBits::Back) result += "ack | ";
    if (value & vk::StencilFaceFlagBits::VkStencilFrontAndBack) result += "kStencilFrontAndBack | ";
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * getString(DescriptorPoolCreateFlagBits value)
  {
    switch (value)
    {
    case DescriptorPoolCreateFlagBits::FreeDescriptorSet: return "reeDescriptorSet";
    default: return "unknown";
    }
  }

  static std::string getString(DescriptorPoolCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::DescriptorPoolCreateFlagBits::FreeDescriptorSet) result += "reeDescriptorSet | ";
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * getString(DependencyFlagBits value)
  {
    switch (value)
    {
    case DependencyFlagBits::ByRegion: return "yRegion";
    default: return "unknown";
    }
  }

  static std::string getString(DependencyFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::DependencyFlagBits::ByRegion) result += "yRegion | ";
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * getString(PresentModeKHR value)
  {
    switch (value)
    {
    case PresentModeKHR::ImmediateKHR: return "mmediateKHR";
    case PresentModeKHR::MailboxKHR: return "ailboxKHR";
    case PresentModeKHR::FifoKHR: return "ifoKHR";
    case PresentModeKHR::FifoRelaxedKHR: return "ifoRelaxedKHR";
    default: return "unknown";
    }
  }

  static const char * getString(ColorSpaceKHR value)
  {
    switch (value)
    {
    case ColorSpaceKHR::VkColorspaceSrgbNonlinearKHR: return "kColorspaceSrgbNonlinearKHR";
    default: return "unknown";
    }
  }

  static const char * getString(CompositeAlphaFlagBitsKHR value)
  {
    switch (value)
    {
    case CompositeAlphaFlagBitsKHR::OpaqueKHR: return "paqueKHR";
    case CompositeAlphaFlagBitsKHR::PreMultipliedKHR: return "reMultipliedKHR";
    case CompositeAlphaFlagBitsKHR::PostMultipliedKHR: return "ostMultipliedKHR";
    case CompositeAlphaFlagBitsKHR::InheritKHR: return "nheritKHR";
    default: return "unknown";
    }
  }

  static std::string getString(CompositeAlphaFlagsKHR value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::CompositeAlphaFlagBitsKHR::OpaqueKHR) result += "paqueKHR | ";
    if (value & vk::CompositeAlphaFlagBitsKHR::PreMultipliedKHR) result += "reMultipliedKHR | ";
    if (value & vk::CompositeAlphaFlagBitsKHR::PostMultipliedKHR) result += "ostMultipliedKHR | ";
    if (value & vk::CompositeAlphaFlagBitsKHR::InheritKHR) result += "nheritKHR | ";
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * getString(SurfaceTransformFlagBitsKHR value)
  {
    switch (value)
    {
    case SurfaceTransformFlagBitsKHR::IdentityKHR: return "dentityKHR";
    case SurfaceTransformFlagBitsKHR::Rotate90KHR: return "otate90KHR";
    case SurfaceTransformFlagBitsKHR::Rotate180KHR: return "otate180KHR";
    case SurfaceTransformFlagBitsKHR::Rotate270KHR: return "otate270KHR";
    case SurfaceTransformFlagBitsKHR::HorizontalMirrorKHR: return "orizontalMirrorKHR";
    case SurfaceTransformFlagBitsKHR::HorizontalMirrorRotate90KHR: return "orizontalMirrorRotate90KHR";
    case SurfaceTransformFlagBitsKHR::HorizontalMirrorRotate180KHR: return "orizontalMirrorRotate180KHR";
    case SurfaceTransformFlagBitsKHR::HorizontalMirrorRotate270KHR: return "orizontalMirrorRotate270KHR";
    case SurfaceTransformFlagBitsKHR::InheritKHR: return "nheritKHR";
    default: return "unknown";
    }
  }

  static std::string getString(SurfaceTransformFlagsKHR value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::SurfaceTransformFlagBitsKHR::IdentityKHR) result += "dentityKHR | ";
    if (value & vk::SurfaceTransformFlagBitsKHR::Rotate90KHR) result += "otate90KHR | ";
    if (value & vk::SurfaceTransformFlagBitsKHR::Rotate180KHR) result += "otate180KHR | ";
    if (value & vk::SurfaceTransformFlagBitsKHR::Rotate270KHR) result += "otate270KHR | ";
    if (value & vk::SurfaceTransformFlagBitsKHR::HorizontalMirrorKHR) result += "orizontalMirrorKHR | ";
    if (value & vk::SurfaceTransformFlagBitsKHR::HorizontalMirrorRotate90KHR) result += "orizontalMirrorRotate90KHR | ";
    if (value & vk::SurfaceTransformFlagBitsKHR::HorizontalMirrorRotate180KHR) result += "orizontalMirrorRotate180KHR | ";
    if (value & vk::SurfaceTransformFlagBitsKHR::HorizontalMirrorRotate270KHR) result += "orizontalMirrorRotate270KHR | ";
    if (value & vk::SurfaceTransformFlagBitsKHR::InheritKHR) result += "nheritKHR | ";
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * getString(DebugReportFlagBitsEXT value)
  {
    switch (value)
    {
    case DebugReportFlagBitsEXT::InformationEXT: return "nformationEXT";
    case DebugReportFlagBitsEXT::WarningEXT: return "arningEXT";
    case DebugReportFlagBitsEXT::PerformanceWarningEXT: return "erformanceWarningEXT";
    case DebugReportFlagBitsEXT::ErrorEXT: return "rrorEXT";
    case DebugReportFlagBitsEXT::DebugEXT: return "ebugEXT";
    default: return "unknown";
    }
  }

  static std::string getString(DebugReportFlagsEXT value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::DebugReportFlagBitsEXT::InformationEXT) result += "nformationEXT | ";
    if (value & vk::DebugReportFlagBitsEXT::WarningEXT) result += "arningEXT | ";
    if (value & vk::DebugReportFlagBitsEXT::PerformanceWarningEXT) result += "erformanceWarningEXT | ";
    if (value & vk::DebugReportFlagBitsEXT::ErrorEXT) result += "rrorEXT | ";
    if (value & vk::DebugReportFlagBitsEXT::DebugEXT) result += "ebugEXT | ";
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * getString(DisplayPlaneAlphaFlagBitsKHR value)
  {
    switch (value)
    {
    case DisplayPlaneAlphaFlagBitsKHR::OpaqueKHR: return "paqueKHR";
    case DisplayPlaneAlphaFlagBitsKHR::GlobalKHR: return "lobalKHR";
    case DisplayPlaneAlphaFlagBitsKHR::PerPixelKHR: return "erPixelKHR";
    case DisplayPlaneAlphaFlagBitsKHR::PerPixelPremultipliedKHR: return "erPixelPremultipliedKHR";
    default: return "unknown";
    }
  }

  static std::string getString(DisplayPlaneAlphaFlagsKHR value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::DisplayPlaneAlphaFlagBitsKHR::OpaqueKHR) result += "paqueKHR | ";
    if (value & vk::DisplayPlaneAlphaFlagBitsKHR::GlobalKHR) result += "lobalKHR | ";
    if (value & vk::DisplayPlaneAlphaFlagBitsKHR::PerPixelKHR) result += "erPixelKHR | ";
    if (value & vk::DisplayPlaneAlphaFlagBitsKHR::PerPixelPremultipliedKHR) result += "erPixelPremultipliedKHR | ";
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
#ifdef VK_USE_PLATFORM_XLIB_KHR

  static std::string getString(XlibSurfaceCreateFlagsKHR value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
#endif /* VK_USE_PLATFORM_XLIB_KHR */

#ifdef VK_USE_PLATFORM_XCB_KHR

  static std::string getString(XcbSurfaceCreateFlagsKHR value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
#endif /* VK_USE_PLATFORM_XCB_KHR */

#ifdef VK_USE_PLATFORM_WAYLAND_KHR

  static std::string getString(WaylandSurfaceCreateFlagsKHR value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
#endif /* VK_USE_PLATFORM_WAYLAND_KHR */

#ifdef VK_USE_PLATFORM_MIR_KHR

  static std::string getString(MirSurfaceCreateFlagsKHR value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
#endif /* VK_USE_PLATFORM_MIR_KHR */

#ifdef VK_USE_PLATFORM_ANDROID_KHR

  static std::string getString(AndroidSurfaceCreateFlagsKHR value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
#endif /* VK_USE_PLATFORM_ANDROID_KHR */

#ifdef VK_USE_PLATFORM_WIN32_KHR

  static std::string getString(Win32SurfaceCreateFlagsKHR value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
#endif /* VK_USE_PLATFORM_WIN32_KHR */

  static const char * getString(DebugReportObjectTypeEXT value)
  {
    switch (value)
    {
    case DebugReportObjectTypeEXT::UnknownEXT: return "nknownEXT";
    case DebugReportObjectTypeEXT::InstanceEXT: return "nstanceEXT";
    case DebugReportObjectTypeEXT::PhysicalDeviceEXT: return "hysicalDeviceEXT";
    case DebugReportObjectTypeEXT::DeviceEXT: return "eviceEXT";
    case DebugReportObjectTypeEXT::QueueEXT: return "ueueEXT";
    case DebugReportObjectTypeEXT::SemaphoreEXT: return "emaphoreEXT";
    case DebugReportObjectTypeEXT::CommandBufferEXT: return "ommandBufferEXT";
    case DebugReportObjectTypeEXT::FenceEXT: return "enceEXT";
    case DebugReportObjectTypeEXT::DeviceMemoryEXT: return "eviceMemoryEXT";
    case DebugReportObjectTypeEXT::BufferEXT: return "ufferEXT";
    case DebugReportObjectTypeEXT::ImageEXT: return "mageEXT";
    case DebugReportObjectTypeEXT::EventEXT: return "ventEXT";
    case DebugReportObjectTypeEXT::QueryPoolEXT: return "ueryPoolEXT";
    case DebugReportObjectTypeEXT::BufferViewEXT: return "ufferViewEXT";
    case DebugReportObjectTypeEXT::ImageViewEXT: return "mageViewEXT";
    case DebugReportObjectTypeEXT::ShaderModuleEXT: return "haderModuleEXT";
    case DebugReportObjectTypeEXT::PipelineCacheEXT: return "ipelineCacheEXT";
    case DebugReportObjectTypeEXT::PipelineLayoutEXT: return "ipelineLayoutEXT";
    case DebugReportObjectTypeEXT::RenderPassEXT: return "enderPassEXT";
    case DebugReportObjectTypeEXT::PipelineEXT: return "ipelineEXT";
    case DebugReportObjectTypeEXT::DescriptorSetLayoutEXT: return "escriptorSetLayoutEXT";
    case DebugReportObjectTypeEXT::SamplerEXT: return "amplerEXT";
    case DebugReportObjectTypeEXT::DescriptorPoolEXT: return "escriptorPoolEXT";
    case DebugReportObjectTypeEXT::DescriptorSetEXT: return "escriptorSetEXT";
    case DebugReportObjectTypeEXT::FramebufferEXT: return "ramebufferEXT";
    case DebugReportObjectTypeEXT::CommandPoolEXT: return "ommandPoolEXT";
    case DebugReportObjectTypeEXT::SurfaceKhrEXT: return "urfaceKhrEXT";
    case DebugReportObjectTypeEXT::SwapchainKhrEXT: return "wapchainKhrEXT";
    case DebugReportObjectTypeEXT::DebugReportEXT: return "ebugReportEXT";
    default: return "unknown";
    }
  }

  static const char * getString(DebugReportErrorEXT value)
  {
    switch (value)
    {
    case DebugReportErrorEXT::NoneEXT: return "oneEXT";
    case DebugReportErrorEXT::CallbackRefEXT: return "allbackRefEXT";
    default: return "unknown";
    }
  }

}
