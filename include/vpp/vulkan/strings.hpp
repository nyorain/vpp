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
  static const char * enumString(FramebufferCreateFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string enumString(FramebufferCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * enumString(QueryPoolCreateFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string enumString(QueryPoolCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * enumString(RenderPassCreateFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string enumString(RenderPassCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * enumString(SamplerCreateFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string enumString(SamplerCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * enumString(PipelineLayoutCreateFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string enumString(PipelineLayoutCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * enumString(PipelineCacheCreateFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string enumString(PipelineCacheCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * enumString(PipelineDepthStencilStateCreateFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string enumString(PipelineDepthStencilStateCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * enumString(PipelineDynamicStateCreateFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string enumString(PipelineDynamicStateCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * enumString(PipelineColorBlendStateCreateFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string enumString(PipelineColorBlendStateCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * enumString(PipelineMultisampleStateCreateFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string enumString(PipelineMultisampleStateCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * enumString(PipelineRasterizationStateCreateFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string enumString(PipelineRasterizationStateCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * enumString(PipelineViewportStateCreateFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string enumString(PipelineViewportStateCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * enumString(PipelineTessellationStateCreateFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string enumString(PipelineTessellationStateCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * enumString(PipelineInputAssemblyStateCreateFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string enumString(PipelineInputAssemblyStateCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * enumString(PipelineVertexInputStateCreateFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string enumString(PipelineVertexInputStateCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * enumString(PipelineShaderStageCreateFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string enumString(PipelineShaderStageCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * enumString(DescriptorSetLayoutCreateFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string enumString(DescriptorSetLayoutCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * enumString(BufferViewCreateFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string enumString(BufferViewCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * enumString(InstanceCreateFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string enumString(InstanceCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * enumString(DeviceCreateFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string enumString(DeviceCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * enumString(DeviceQueueCreateFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string enumString(DeviceQueueCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * enumString(ImageViewCreateFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string enumString(ImageViewCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * enumString(SemaphoreCreateFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string enumString(SemaphoreCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * enumString(ShaderModuleCreateFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string enumString(ShaderModuleCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * enumString(EventCreateFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string enumString(EventCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * enumString(MemoryMapFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string enumString(MemoryMapFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * enumString(SubpassDescriptionFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string enumString(SubpassDescriptionFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * enumString(DescriptorPoolResetFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string enumString(DescriptorPoolResetFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * enumString(SwapchainCreateFlagBitsKHR value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string enumString(SwapchainCreateFlagsKHR value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * enumString(DisplayModeCreateFlagBitsKHR value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string enumString(DisplayModeCreateFlagsKHR value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * enumString(DisplaySurfaceCreateFlagBitsKHR value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string enumString(DisplaySurfaceCreateFlagsKHR value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * enumString(AndroidSurfaceCreateFlagBitsKHR value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static const char * enumString(MirSurfaceCreateFlagBitsKHR value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static const char * enumString(WaylandSurfaceCreateFlagBitsKHR value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static const char * enumString(Win32SurfaceCreateFlagBitsKHR value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static const char * enumString(XlibSurfaceCreateFlagBitsKHR value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static const char * enumString(XcbSurfaceCreateFlagBitsKHR value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static const char * enumString(ImageLayout value)
  {
    switch (value)
    {
    case ImageLayout::Undefined: return "Undefined";
    case ImageLayout::General: return "General";
    case ImageLayout::ColorAttachmentOptimal: return "ColorAttachmentOptimal";
    case ImageLayout::DepthStencilAttachmentOptimal: return "DepthStencilAttachmentOptimal";
    case ImageLayout::DepthStencilReadOnlyOptimal: return "DepthStencilReadOnlyOptimal";
    case ImageLayout::ShaderReadOnlyOptimal: return "ShaderReadOnlyOptimal";
    case ImageLayout::TransferSrcOptimal: return "TransferSrcOptimal";
    case ImageLayout::TransferDstOptimal: return "TransferDstOptimal";
    case ImageLayout::Preinitialized: return "Preinitialized";
    case ImageLayout::PresentSrcKHR: return "PresentSrcKHR";
    default: return "unknown";
    }
  }

  static const char * enumString(AttachmentLoadOp value)
  {
    switch (value)
    {
    case AttachmentLoadOp::Load: return "Load";
    case AttachmentLoadOp::Clear: return "Clear";
    case AttachmentLoadOp::DontCare: return "DontCare";
    default: return "unknown";
    }
  }

  static const char * enumString(AttachmentStoreOp value)
  {
    switch (value)
    {
    case AttachmentStoreOp::Store: return "Store";
    case AttachmentStoreOp::DontCare: return "DontCare";
    default: return "unknown";
    }
  }

  static const char * enumString(ImageType value)
  {
    switch (value)
    {
    case ImageType::e1D: return "e1D";
    case ImageType::e2D: return "e2D";
    case ImageType::e3D: return "e3D";
    default: return "unknown";
    }
  }

  static const char * enumString(ImageTiling value)
  {
    switch (value)
    {
    case ImageTiling::Optimal: return "Optimal";
    case ImageTiling::Linear: return "Linear";
    default: return "unknown";
    }
  }

  static const char * enumString(ImageViewType value)
  {
    switch (value)
    {
    case ImageViewType::e1D: return "e1D";
    case ImageViewType::e2D: return "e2D";
    case ImageViewType::e3D: return "e3D";
    case ImageViewType::Cube: return "Cube";
    case ImageViewType::e1DArray: return "e1DArray";
    case ImageViewType::e2DArray: return "e2DArray";
    case ImageViewType::CubeArray: return "CubeArray";
    default: return "unknown";
    }
  }

  static const char * enumString(CommandBufferLevel value)
  {
    switch (value)
    {
    case CommandBufferLevel::Primary: return "Primary";
    case CommandBufferLevel::Secondary: return "Secondary";
    default: return "unknown";
    }
  }

  static const char * enumString(ComponentSwizzle value)
  {
    switch (value)
    {
    case ComponentSwizzle::Identity: return "Identity";
    case ComponentSwizzle::Zero: return "Zero";
    case ComponentSwizzle::One: return "One";
    case ComponentSwizzle::R: return "R";
    case ComponentSwizzle::G: return "G";
    case ComponentSwizzle::B: return "B";
    case ComponentSwizzle::A: return "A";
    default: return "unknown";
    }
  }

  static const char * enumString(DescriptorType value)
  {
    switch (value)
    {
    case DescriptorType::Sampler: return "Sampler";
    case DescriptorType::CombinedImageSampler: return "CombinedImageSampler";
    case DescriptorType::SampledImage: return "SampledImage";
    case DescriptorType::StorageImage: return "StorageImage";
    case DescriptorType::UniformTexelBuffer: return "UniformTexelBuffer";
    case DescriptorType::StorageTexelBuffer: return "StorageTexelBuffer";
    case DescriptorType::UniformBuffer: return "UniformBuffer";
    case DescriptorType::StorageBuffer: return "StorageBuffer";
    case DescriptorType::UniformBufferDynamic: return "UniformBufferDynamic";
    case DescriptorType::StorageBufferDynamic: return "StorageBufferDynamic";
    case DescriptorType::InputAttachment: return "InputAttachment";
    default: return "unknown";
    }
  }

  static const char * enumString(QueryType value)
  {
    switch (value)
    {
    case QueryType::Occlusion: return "Occlusion";
    case QueryType::PipelineStatistics: return "PipelineStatistics";
    case QueryType::Timestamp: return "Timestamp";
    default: return "unknown";
    }
  }

  static const char * enumString(BorderColor value)
  {
    switch (value)
    {
    case BorderColor::FloatTransparentBlack: return "FloatTransparentBlack";
    case BorderColor::IntTransparentBlack: return "IntTransparentBlack";
    case BorderColor::FloatOpaqueBlack: return "FloatOpaqueBlack";
    case BorderColor::IntOpaqueBlack: return "IntOpaqueBlack";
    case BorderColor::FloatOpaqueWhite: return "FloatOpaqueWhite";
    case BorderColor::IntOpaqueWhite: return "IntOpaqueWhite";
    default: return "unknown";
    }
  }

  static const char * enumString(PipelineBindPoint value)
  {
    switch (value)
    {
    case PipelineBindPoint::Graphics: return "Graphics";
    case PipelineBindPoint::Compute: return "Compute";
    default: return "unknown";
    }
  }

  static const char * enumString(PipelineCacheHeaderVersion value)
  {
    switch (value)
    {
    case PipelineCacheHeaderVersion::One: return "One";
    default: return "unknown";
    }
  }

  static const char * enumString(PrimitiveTopology value)
  {
    switch (value)
    {
    case PrimitiveTopology::PointList: return "PointList";
    case PrimitiveTopology::LineList: return "LineList";
    case PrimitiveTopology::LineStrip: return "LineStrip";
    case PrimitiveTopology::TriangleList: return "TriangleList";
    case PrimitiveTopology::TriangleStrip: return "TriangleStrip";
    case PrimitiveTopology::TriangleFan: return "TriangleFan";
    case PrimitiveTopology::LineListWithAdjacency: return "LineListWithAdjacency";
    case PrimitiveTopology::LineStripWithAdjacency: return "LineStripWithAdjacency";
    case PrimitiveTopology::TriangleListWithAdjacency: return "TriangleListWithAdjacency";
    case PrimitiveTopology::TriangleStripWithAdjacency: return "TriangleStripWithAdjacency";
    case PrimitiveTopology::PatchList: return "PatchList";
    default: return "unknown";
    }
  }

  static const char * enumString(SharingMode value)
  {
    switch (value)
    {
    case SharingMode::Exclusive: return "Exclusive";
    case SharingMode::Concurrent: return "Concurrent";
    default: return "unknown";
    }
  }

  static const char * enumString(IndexType value)
  {
    switch (value)
    {
    case IndexType::Uint16: return "Uint16";
    case IndexType::Uint32: return "Uint32";
    default: return "unknown";
    }
  }

  static const char * enumString(Filter value)
  {
    switch (value)
    {
    case Filter::Nearest: return "Nearest";
    case Filter::Linear: return "Linear";
    default: return "unknown";
    }
  }

  static const char * enumString(SamplerMipmapMode value)
  {
    switch (value)
    {
    case SamplerMipmapMode::Nearest: return "Nearest";
    case SamplerMipmapMode::Linear: return "Linear";
    default: return "unknown";
    }
  }

  static const char * enumString(SamplerAddressMode value)
  {
    switch (value)
    {
    case SamplerAddressMode::Repeat: return "Repeat";
    case SamplerAddressMode::MirroredRepeat: return "MirroredRepeat";
    case SamplerAddressMode::ClampToEdge: return "ClampToEdge";
    case SamplerAddressMode::ClampToBorder: return "ClampToBorder";
    case SamplerAddressMode::MirrorClampToEdge: return "MirrorClampToEdge";
    default: return "unknown";
    }
  }

  static const char * enumString(CompareOp value)
  {
    switch (value)
    {
    case CompareOp::Never: return "Never";
    case CompareOp::Less: return "Less";
    case CompareOp::Equal: return "Equal";
    case CompareOp::LessOrEqual: return "LessOrEqual";
    case CompareOp::Greater: return "Greater";
    case CompareOp::NotEqual: return "NotEqual";
    case CompareOp::GreaterOrEqual: return "GreaterOrEqual";
    case CompareOp::Always: return "Always";
    default: return "unknown";
    }
  }

  static const char * enumString(PolygonMode value)
  {
    switch (value)
    {
    case PolygonMode::Fill: return "Fill";
    case PolygonMode::Line: return "Line";
    case PolygonMode::Point: return "Point";
    default: return "unknown";
    }
  }

  static const char * enumString(CullModeFlagBits value)
  {
    switch (value)
    {
    case CullModeFlagBits::None: return "None";
    case CullModeFlagBits::Front: return "Front";
    case CullModeFlagBits::Back: return "Back";
    case CullModeFlagBits::FrontAndBack: return "FrontAndBack";
    default: return "unknown";
    }
  }

  static std::string enumString(CullModeFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::CullModeFlagBits::None) result += "None | ";
    if (value & vk::CullModeFlagBits::Front) result += "Front | ";
    if (value & vk::CullModeFlagBits::Back) result += "Back | ";
    if (value & vk::CullModeFlagBits::FrontAndBack) result += "FrontAndBack | ";
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * enumString(FrontFace value)
  {
    switch (value)
    {
    case FrontFace::CounterClockwise: return "CounterClockwise";
    case FrontFace::Clockwise: return "Clockwise";
    default: return "unknown";
    }
  }

  static const char * enumString(BlendFactor value)
  {
    switch (value)
    {
    case BlendFactor::Zero: return "Zero";
    case BlendFactor::One: return "One";
    case BlendFactor::SrcColor: return "SrcColor";
    case BlendFactor::OneMinusSrcColor: return "OneMinusSrcColor";
    case BlendFactor::DstColor: return "DstColor";
    case BlendFactor::OneMinusDstColor: return "OneMinusDstColor";
    case BlendFactor::SrcAlpha: return "SrcAlpha";
    case BlendFactor::OneMinusSrcAlpha: return "OneMinusSrcAlpha";
    case BlendFactor::DstAlpha: return "DstAlpha";
    case BlendFactor::OneMinusDstAlpha: return "OneMinusDstAlpha";
    case BlendFactor::ConstantColor: return "ConstantColor";
    case BlendFactor::OneMinusConstantColor: return "OneMinusConstantColor";
    case BlendFactor::ConstantAlpha: return "ConstantAlpha";
    case BlendFactor::OneMinusConstantAlpha: return "OneMinusConstantAlpha";
    case BlendFactor::SrcAlphaSaturate: return "SrcAlphaSaturate";
    case BlendFactor::Src1Color: return "Src1Color";
    case BlendFactor::OneMinusSrc1Color: return "OneMinusSrc1Color";
    case BlendFactor::Src1Alpha: return "Src1Alpha";
    case BlendFactor::OneMinusSrc1Alpha: return "OneMinusSrc1Alpha";
    default: return "unknown";
    }
  }

  static const char * enumString(BlendOp value)
  {
    switch (value)
    {
    case BlendOp::Add: return "Add";
    case BlendOp::Subtract: return "Subtract";
    case BlendOp::ReverseSubtract: return "ReverseSubtract";
    case BlendOp::Min: return "Min";
    case BlendOp::Max: return "Max";
    default: return "unknown";
    }
  }

  static const char * enumString(StencilOp value)
  {
    switch (value)
    {
    case StencilOp::Keep: return "Keep";
    case StencilOp::Zero: return "Zero";
    case StencilOp::Replace: return "Replace";
    case StencilOp::IncrementAndClamp: return "IncrementAndClamp";
    case StencilOp::DecrementAndClamp: return "DecrementAndClamp";
    case StencilOp::Invert: return "Invert";
    case StencilOp::IncrementAndWrap: return "IncrementAndWrap";
    case StencilOp::DecrementAndWrap: return "DecrementAndWrap";
    default: return "unknown";
    }
  }

  static const char * enumString(LogicOp value)
  {
    switch (value)
    {
    case LogicOp::Clear: return "Clear";
    case LogicOp::And: return "And";
    case LogicOp::AndReverse: return "AndReverse";
    case LogicOp::Copy: return "Copy";
    case LogicOp::AndInverted: return "AndInverted";
    case LogicOp::NoOp: return "NoOp";
    case LogicOp::Xor: return "Xor";
    case LogicOp::Or: return "Or";
    case LogicOp::Nor: return "Nor";
    case LogicOp::Equivalent: return "Equivalent";
    case LogicOp::Invert: return "Invert";
    case LogicOp::OrReverse: return "OrReverse";
    case LogicOp::CopyInverted: return "CopyInverted";
    case LogicOp::OrInverted: return "OrInverted";
    case LogicOp::Nand: return "Nand";
    case LogicOp::Set: return "Set";
    default: return "unknown";
    }
  }

  static const char * enumString(InternalAllocationType value)
  {
    switch (value)
    {
    case InternalAllocationType::Executable: return "Executable";
    default: return "unknown";
    }
  }

  static const char * enumString(SystemAllocationScope value)
  {
    switch (value)
    {
    case SystemAllocationScope::Command: return "Command";
    case SystemAllocationScope::Object: return "Object";
    case SystemAllocationScope::Cache: return "Cache";
    case SystemAllocationScope::Device: return "Device";
    case SystemAllocationScope::Instance: return "Instance";
    default: return "unknown";
    }
  }

  static const char * enumString(PhysicalDeviceType value)
  {
    switch (value)
    {
    case PhysicalDeviceType::Other: return "Other";
    case PhysicalDeviceType::IntegratedGpu: return "IntegratedGpu";
    case PhysicalDeviceType::DiscreteGpu: return "DiscreteGpu";
    case PhysicalDeviceType::VirtualGpu: return "VirtualGpu";
    case PhysicalDeviceType::Cpu: return "Cpu";
    default: return "unknown";
    }
  }

  static const char * enumString(VertexInputRate value)
  {
    switch (value)
    {
    case VertexInputRate::Vertex: return "Vertex";
    case VertexInputRate::Instance: return "Instance";
    default: return "unknown";
    }
  }

  static const char * enumString(Format value)
  {
    switch (value)
    {
    case Format::Undefined: return "Undefined";
    case Format::R4G4UnormPack8: return "R4G4UnormPack8";
    case Format::R4G4B4A4UnormPack16: return "R4G4B4A4UnormPack16";
    case Format::B4G4R4A4UnormPack16: return "B4G4R4A4UnormPack16";
    case Format::R5G6B5UnormPack16: return "R5G6B5UnormPack16";
    case Format::B5G6R5UnormPack16: return "B5G6R5UnormPack16";
    case Format::R5G5B5A1UnormPack16: return "R5G5B5A1UnormPack16";
    case Format::B5G5R5A1UnormPack16: return "B5G5R5A1UnormPack16";
    case Format::A1R5G5B5UnormPack16: return "A1R5G5B5UnormPack16";
    case Format::R8Unorm: return "R8Unorm";
    case Format::R8Snorm: return "R8Snorm";
    case Format::R8Uscaled: return "R8Uscaled";
    case Format::R8Sscaled: return "R8Sscaled";
    case Format::R8Uint: return "R8Uint";
    case Format::R8Sint: return "R8Sint";
    case Format::R8Srgb: return "R8Srgb";
    case Format::R8G8Unorm: return "R8G8Unorm";
    case Format::R8G8Snorm: return "R8G8Snorm";
    case Format::R8G8Uscaled: return "R8G8Uscaled";
    case Format::R8G8Sscaled: return "R8G8Sscaled";
    case Format::R8G8Uint: return "R8G8Uint";
    case Format::R8G8Sint: return "R8G8Sint";
    case Format::R8G8Srgb: return "R8G8Srgb";
    case Format::R8G8B8Unorm: return "R8G8B8Unorm";
    case Format::R8G8B8Snorm: return "R8G8B8Snorm";
    case Format::R8G8B8Uscaled: return "R8G8B8Uscaled";
    case Format::R8G8B8Sscaled: return "R8G8B8Sscaled";
    case Format::R8G8B8Uint: return "R8G8B8Uint";
    case Format::R8G8B8Sint: return "R8G8B8Sint";
    case Format::R8G8B8Srgb: return "R8G8B8Srgb";
    case Format::B8G8R8Unorm: return "B8G8R8Unorm";
    case Format::B8G8R8Snorm: return "B8G8R8Snorm";
    case Format::B8G8R8Uscaled: return "B8G8R8Uscaled";
    case Format::B8G8R8Sscaled: return "B8G8R8Sscaled";
    case Format::B8G8R8Uint: return "B8G8R8Uint";
    case Format::B8G8R8Sint: return "B8G8R8Sint";
    case Format::B8G8R8Srgb: return "B8G8R8Srgb";
    case Format::R8G8B8A8Unorm: return "R8G8B8A8Unorm";
    case Format::R8G8B8A8Snorm: return "R8G8B8A8Snorm";
    case Format::R8G8B8A8Uscaled: return "R8G8B8A8Uscaled";
    case Format::R8G8B8A8Sscaled: return "R8G8B8A8Sscaled";
    case Format::R8G8B8A8Uint: return "R8G8B8A8Uint";
    case Format::R8G8B8A8Sint: return "R8G8B8A8Sint";
    case Format::R8G8B8A8Srgb: return "R8G8B8A8Srgb";
    case Format::B8G8R8A8Unorm: return "B8G8R8A8Unorm";
    case Format::B8G8R8A8Snorm: return "B8G8R8A8Snorm";
    case Format::B8G8R8A8Uscaled: return "B8G8R8A8Uscaled";
    case Format::B8G8R8A8Sscaled: return "B8G8R8A8Sscaled";
    case Format::B8G8R8A8Uint: return "B8G8R8A8Uint";
    case Format::B8G8R8A8Sint: return "B8G8R8A8Sint";
    case Format::B8G8R8A8Srgb: return "B8G8R8A8Srgb";
    case Format::A8B8G8R8UnormPack32: return "A8B8G8R8UnormPack32";
    case Format::A8B8G8R8SnormPack32: return "A8B8G8R8SnormPack32";
    case Format::A8B8G8R8UscaledPack32: return "A8B8G8R8UscaledPack32";
    case Format::A8B8G8R8SscaledPack32: return "A8B8G8R8SscaledPack32";
    case Format::A8B8G8R8UintPack32: return "A8B8G8R8UintPack32";
    case Format::A8B8G8R8SintPack32: return "A8B8G8R8SintPack32";
    case Format::A8B8G8R8SrgbPack32: return "A8B8G8R8SrgbPack32";
    case Format::A2R10G10B10UnormPack32: return "A2R10G10B10UnormPack32";
    case Format::A2R10G10B10SnormPack32: return "A2R10G10B10SnormPack32";
    case Format::A2R10G10B10UscaledPack32: return "A2R10G10B10UscaledPack32";
    case Format::A2R10G10B10SscaledPack32: return "A2R10G10B10SscaledPack32";
    case Format::A2R10G10B10UintPack32: return "A2R10G10B10UintPack32";
    case Format::A2R10G10B10SintPack32: return "A2R10G10B10SintPack32";
    case Format::A2B10G10R10UnormPack32: return "A2B10G10R10UnormPack32";
    case Format::A2B10G10R10SnormPack32: return "A2B10G10R10SnormPack32";
    case Format::A2B10G10R10UscaledPack32: return "A2B10G10R10UscaledPack32";
    case Format::A2B10G10R10SscaledPack32: return "A2B10G10R10SscaledPack32";
    case Format::A2B10G10R10UintPack32: return "A2B10G10R10UintPack32";
    case Format::A2B10G10R10SintPack32: return "A2B10G10R10SintPack32";
    case Format::R16Unorm: return "R16Unorm";
    case Format::R16Snorm: return "R16Snorm";
    case Format::R16Uscaled: return "R16Uscaled";
    case Format::R16Sscaled: return "R16Sscaled";
    case Format::R16Uint: return "R16Uint";
    case Format::R16Sint: return "R16Sint";
    case Format::R16Sfloat: return "R16Sfloat";
    case Format::R16G16Unorm: return "R16G16Unorm";
    case Format::R16G16Snorm: return "R16G16Snorm";
    case Format::R16G16Uscaled: return "R16G16Uscaled";
    case Format::R16G16Sscaled: return "R16G16Sscaled";
    case Format::R16G16Uint: return "R16G16Uint";
    case Format::R16G16Sint: return "R16G16Sint";
    case Format::R16G16Sfloat: return "R16G16Sfloat";
    case Format::R16G16B16Unorm: return "R16G16B16Unorm";
    case Format::R16G16B16Snorm: return "R16G16B16Snorm";
    case Format::R16G16B16Uscaled: return "R16G16B16Uscaled";
    case Format::R16G16B16Sscaled: return "R16G16B16Sscaled";
    case Format::R16G16B16Uint: return "R16G16B16Uint";
    case Format::R16G16B16Sint: return "R16G16B16Sint";
    case Format::R16G16B16Sfloat: return "R16G16B16Sfloat";
    case Format::R16G16B16A16Unorm: return "R16G16B16A16Unorm";
    case Format::R16G16B16A16Snorm: return "R16G16B16A16Snorm";
    case Format::R16G16B16A16Uscaled: return "R16G16B16A16Uscaled";
    case Format::R16G16B16A16Sscaled: return "R16G16B16A16Sscaled";
    case Format::R16G16B16A16Uint: return "R16G16B16A16Uint";
    case Format::R16G16B16A16Sint: return "R16G16B16A16Sint";
    case Format::R16G16B16A16Sfloat: return "R16G16B16A16Sfloat";
    case Format::R32Uint: return "R32Uint";
    case Format::R32Sint: return "R32Sint";
    case Format::R32Sfloat: return "R32Sfloat";
    case Format::R32G32Uint: return "R32G32Uint";
    case Format::R32G32Sint: return "R32G32Sint";
    case Format::R32G32Sfloat: return "R32G32Sfloat";
    case Format::R32G32B32Uint: return "R32G32B32Uint";
    case Format::R32G32B32Sint: return "R32G32B32Sint";
    case Format::R32G32B32Sfloat: return "R32G32B32Sfloat";
    case Format::R32G32B32A32Uint: return "R32G32B32A32Uint";
    case Format::R32G32B32A32Sint: return "R32G32B32A32Sint";
    case Format::R32G32B32A32Sfloat: return "R32G32B32A32Sfloat";
    case Format::R64Uint: return "R64Uint";
    case Format::R64Sint: return "R64Sint";
    case Format::R64Sfloat: return "R64Sfloat";
    case Format::R64G64Uint: return "R64G64Uint";
    case Format::R64G64Sint: return "R64G64Sint";
    case Format::R64G64Sfloat: return "R64G64Sfloat";
    case Format::R64G64B64Uint: return "R64G64B64Uint";
    case Format::R64G64B64Sint: return "R64G64B64Sint";
    case Format::R64G64B64Sfloat: return "R64G64B64Sfloat";
    case Format::R64G64B64A64Uint: return "R64G64B64A64Uint";
    case Format::R64G64B64A64Sint: return "R64G64B64A64Sint";
    case Format::R64G64B64A64Sfloat: return "R64G64B64A64Sfloat";
    case Format::B10G11R11UfloatPack32: return "B10G11R11UfloatPack32";
    case Format::E5B9G9R9UfloatPack32: return "E5B9G9R9UfloatPack32";
    case Format::D16Unorm: return "D16Unorm";
    case Format::X8D24UnormPack32: return "X8D24UnormPack32";
    case Format::D32Sfloat: return "D32Sfloat";
    case Format::S8Uint: return "S8Uint";
    case Format::D16UnormS8Uint: return "D16UnormS8Uint";
    case Format::D24UnormS8Uint: return "D24UnormS8Uint";
    case Format::D32SfloatS8Uint: return "D32SfloatS8Uint";
    case Format::Bc1RgbUnormBlock: return "Bc1RgbUnormBlock";
    case Format::Bc1RgbSrgbBlock: return "Bc1RgbSrgbBlock";
    case Format::Bc1RgbaUnormBlock: return "Bc1RgbaUnormBlock";
    case Format::Bc1RgbaSrgbBlock: return "Bc1RgbaSrgbBlock";
    case Format::Bc2UnormBlock: return "Bc2UnormBlock";
    case Format::Bc2SrgbBlock: return "Bc2SrgbBlock";
    case Format::Bc3UnormBlock: return "Bc3UnormBlock";
    case Format::Bc3SrgbBlock: return "Bc3SrgbBlock";
    case Format::Bc4UnormBlock: return "Bc4UnormBlock";
    case Format::Bc4SnormBlock: return "Bc4SnormBlock";
    case Format::Bc5UnormBlock: return "Bc5UnormBlock";
    case Format::Bc5SnormBlock: return "Bc5SnormBlock";
    case Format::Bc6HUfloatBlock: return "Bc6HUfloatBlock";
    case Format::Bc6HSfloatBlock: return "Bc6HSfloatBlock";
    case Format::Bc7UnormBlock: return "Bc7UnormBlock";
    case Format::Bc7SrgbBlock: return "Bc7SrgbBlock";
    case Format::Etc2R8G8B8UnormBlock: return "Etc2R8G8B8UnormBlock";
    case Format::Etc2R8G8B8SrgbBlock: return "Etc2R8G8B8SrgbBlock";
    case Format::Etc2R8G8B8A1UnormBlock: return "Etc2R8G8B8A1UnormBlock";
    case Format::Etc2R8G8B8A1SrgbBlock: return "Etc2R8G8B8A1SrgbBlock";
    case Format::Etc2R8G8B8A8UnormBlock: return "Etc2R8G8B8A8UnormBlock";
    case Format::Etc2R8G8B8A8SrgbBlock: return "Etc2R8G8B8A8SrgbBlock";
    case Format::EacR11UnormBlock: return "EacR11UnormBlock";
    case Format::EacR11SnormBlock: return "EacR11SnormBlock";
    case Format::EacR11G11UnormBlock: return "EacR11G11UnormBlock";
    case Format::EacR11G11SnormBlock: return "EacR11G11SnormBlock";
    case Format::Astc4x4UnormBlock: return "Astc4x4UnormBlock";
    case Format::Astc4x4SrgbBlock: return "Astc4x4SrgbBlock";
    case Format::Astc5x4UnormBlock: return "Astc5x4UnormBlock";
    case Format::Astc5x4SrgbBlock: return "Astc5x4SrgbBlock";
    case Format::Astc5x5UnormBlock: return "Astc5x5UnormBlock";
    case Format::Astc5x5SrgbBlock: return "Astc5x5SrgbBlock";
    case Format::Astc6x5UnormBlock: return "Astc6x5UnormBlock";
    case Format::Astc6x5SrgbBlock: return "Astc6x5SrgbBlock";
    case Format::Astc6x6UnormBlock: return "Astc6x6UnormBlock";
    case Format::Astc6x6SrgbBlock: return "Astc6x6SrgbBlock";
    case Format::Astc8x5UnormBlock: return "Astc8x5UnormBlock";
    case Format::Astc8x5SrgbBlock: return "Astc8x5SrgbBlock";
    case Format::Astc8x6UnormBlock: return "Astc8x6UnormBlock";
    case Format::Astc8x6SrgbBlock: return "Astc8x6SrgbBlock";
    case Format::Astc8x8UnormBlock: return "Astc8x8UnormBlock";
    case Format::Astc8x8SrgbBlock: return "Astc8x8SrgbBlock";
    case Format::Astc10x5UnormBlock: return "Astc10x5UnormBlock";
    case Format::Astc10x5SrgbBlock: return "Astc10x5SrgbBlock";
    case Format::Astc10x6UnormBlock: return "Astc10x6UnormBlock";
    case Format::Astc10x6SrgbBlock: return "Astc10x6SrgbBlock";
    case Format::Astc10x8UnormBlock: return "Astc10x8UnormBlock";
    case Format::Astc10x8SrgbBlock: return "Astc10x8SrgbBlock";
    case Format::Astc10x10UnormBlock: return "Astc10x10UnormBlock";
    case Format::Astc10x10SrgbBlock: return "Astc10x10SrgbBlock";
    case Format::Astc12x10UnormBlock: return "Astc12x10UnormBlock";
    case Format::Astc12x10SrgbBlock: return "Astc12x10SrgbBlock";
    case Format::Astc12x12UnormBlock: return "Astc12x12UnormBlock";
    case Format::Astc12x12SrgbBlock: return "Astc12x12SrgbBlock";
    default: return "unknown";
    }
  }

  static const char * enumString(StructureType value)
  {
    switch (value)
    {
    case StructureType::ApplicationInfo: return "ApplicationInfo";
    case StructureType::InstanceCreateInfo: return "InstanceCreateInfo";
    case StructureType::DeviceQueueCreateInfo: return "DeviceQueueCreateInfo";
    case StructureType::DeviceCreateInfo: return "DeviceCreateInfo";
    case StructureType::SubmitInfo: return "SubmitInfo";
    case StructureType::MemoryAllocateInfo: return "MemoryAllocateInfo";
    case StructureType::MappedMemoryRange: return "MappedMemoryRange";
    case StructureType::BindSparseInfo: return "BindSparseInfo";
    case StructureType::FenceCreateInfo: return "FenceCreateInfo";
    case StructureType::SemaphoreCreateInfo: return "SemaphoreCreateInfo";
    case StructureType::EventCreateInfo: return "EventCreateInfo";
    case StructureType::QueryPoolCreateInfo: return "QueryPoolCreateInfo";
    case StructureType::BufferCreateInfo: return "BufferCreateInfo";
    case StructureType::BufferViewCreateInfo: return "BufferViewCreateInfo";
    case StructureType::ImageCreateInfo: return "ImageCreateInfo";
    case StructureType::ImageViewCreateInfo: return "ImageViewCreateInfo";
    case StructureType::ShaderModuleCreateInfo: return "ShaderModuleCreateInfo";
    case StructureType::PipelineCacheCreateInfo: return "PipelineCacheCreateInfo";
    case StructureType::PipelineShaderStageCreateInfo: return "PipelineShaderStageCreateInfo";
    case StructureType::PipelineVertexInputStateCreateInfo: return "PipelineVertexInputStateCreateInfo";
    case StructureType::PipelineInputAssemblyStateCreateInfo: return "PipelineInputAssemblyStateCreateInfo";
    case StructureType::PipelineTessellationStateCreateInfo: return "PipelineTessellationStateCreateInfo";
    case StructureType::PipelineViewportStateCreateInfo: return "PipelineViewportStateCreateInfo";
    case StructureType::PipelineRasterizationStateCreateInfo: return "PipelineRasterizationStateCreateInfo";
    case StructureType::PipelineMultisampleStateCreateInfo: return "PipelineMultisampleStateCreateInfo";
    case StructureType::PipelineDepthStencilStateCreateInfo: return "PipelineDepthStencilStateCreateInfo";
    case StructureType::PipelineColorBlendStateCreateInfo: return "PipelineColorBlendStateCreateInfo";
    case StructureType::PipelineDynamicStateCreateInfo: return "PipelineDynamicStateCreateInfo";
    case StructureType::GraphicsPipelineCreateInfo: return "GraphicsPipelineCreateInfo";
    case StructureType::ComputePipelineCreateInfo: return "ComputePipelineCreateInfo";
    case StructureType::PipelineLayoutCreateInfo: return "PipelineLayoutCreateInfo";
    case StructureType::SamplerCreateInfo: return "SamplerCreateInfo";
    case StructureType::DescriptorSetLayoutCreateInfo: return "DescriptorSetLayoutCreateInfo";
    case StructureType::DescriptorPoolCreateInfo: return "DescriptorPoolCreateInfo";
    case StructureType::DescriptorSetAllocateInfo: return "DescriptorSetAllocateInfo";
    case StructureType::WriteDescriptorSet: return "WriteDescriptorSet";
    case StructureType::CopyDescriptorSet: return "CopyDescriptorSet";
    case StructureType::FramebufferCreateInfo: return "FramebufferCreateInfo";
    case StructureType::RenderPassCreateInfo: return "RenderPassCreateInfo";
    case StructureType::CommandPoolCreateInfo: return "CommandPoolCreateInfo";
    case StructureType::CommandBufferAllocateInfo: return "CommandBufferAllocateInfo";
    case StructureType::CommandBufferInheritanceInfo: return "CommandBufferInheritanceInfo";
    case StructureType::CommandBufferBeginInfo: return "CommandBufferBeginInfo";
    case StructureType::RenderPassBeginInfo: return "RenderPassBeginInfo";
    case StructureType::BufferMemoryBarrier: return "BufferMemoryBarrier";
    case StructureType::ImageMemoryBarrier: return "ImageMemoryBarrier";
    case StructureType::MemoryBarrier: return "MemoryBarrier";
    case StructureType::LoaderInstanceCreateInfo: return "LoaderInstanceCreateInfo";
    case StructureType::LoaderDeviceCreateInfo: return "LoaderDeviceCreateInfo";
    case StructureType::SwapchainCreateInfoKHR: return "SwapchainCreateInfoKHR";
    case StructureType::PresentInfoKHR: return "PresentInfoKHR";
    case StructureType::DisplayModeCreateInfoKHR: return "DisplayModeCreateInfoKHR";
    case StructureType::DisplaySurfaceCreateInfoKHR: return "DisplaySurfaceCreateInfoKHR";
    case StructureType::DisplayPresentInfoKHR: return "DisplayPresentInfoKHR";
    case StructureType::XlibSurfaceCreateInfoKHR: return "XlibSurfaceCreateInfoKHR";
    case StructureType::XcbSurfaceCreateInfoKHR: return "XcbSurfaceCreateInfoKHR";
    case StructureType::WaylandSurfaceCreateInfoKHR: return "WaylandSurfaceCreateInfoKHR";
    case StructureType::MirSurfaceCreateInfoKHR: return "MirSurfaceCreateInfoKHR";
    case StructureType::AndroidSurfaceCreateInfoKHR: return "AndroidSurfaceCreateInfoKHR";
    case StructureType::Win32SurfaceCreateInfoKHR: return "Win32SurfaceCreateInfoKHR";
    case StructureType::DebugReportCreateInfoEXT: return "DebugReportCreateInfoEXT";
    default: return "unknown";
    }
  }

  static const char * enumString(SubpassContents value)
  {
    switch (value)
    {
    case SubpassContents::Inline: return "Inline";
    case SubpassContents::SecondaryCommandBuffers: return "SecondaryCommandBuffers";
    default: return "unknown";
    }
  }

  static const char * enumString(Result value)
  {
    switch (value)
    {
    case Result::Success: return "Success";
    case Result::NotReady: return "NotReady";
    case Result::Timeout: return "Timeout";
    case Result::EventSet: return "EventSet";
    case Result::EventReset: return "EventReset";
    case Result::Incomplete: return "Incomplete";
    case Result::ErrorOutOfHostMemory: return "ErrorOutOfHostMemory";
    case Result::ErrorOutOfDeviceMemory: return "ErrorOutOfDeviceMemory";
    case Result::ErrorInitializationFailed: return "ErrorInitializationFailed";
    case Result::ErrorDeviceLost: return "ErrorDeviceLost";
    case Result::ErrorMemoryMapFailed: return "ErrorMemoryMapFailed";
    case Result::ErrorLayerNotPresent: return "ErrorLayerNotPresent";
    case Result::ErrorExtensionNotPresent: return "ErrorExtensionNotPresent";
    case Result::ErrorFeatureNotPresent: return "ErrorFeatureNotPresent";
    case Result::ErrorIncompatibleDriver: return "ErrorIncompatibleDriver";
    case Result::ErrorTooManyObjects: return "ErrorTooManyObjects";
    case Result::ErrorFormatNotSupported: return "ErrorFormatNotSupported";
    case Result::ErrorSurfaceLostKHR: return "ErrorSurfaceLostKHR";
    case Result::ErrorNativeWindowInUseKHR: return "ErrorNativeWindowInUseKHR";
    case Result::SuboptimalKHR: return "SuboptimalKHR";
    case Result::ErrorOutOfDateKHR: return "ErrorOutOfDateKHR";
    case Result::ErrorIncompatibleDisplayKHR: return "ErrorIncompatibleDisplayKHR";
    case Result::ErrorValidationFailedEXT: return "ErrorValidationFailedEXT";
    default: return "unknown";
    }
  }

  static const char * enumString(DynamicState value)
  {
    switch (value)
    {
    case DynamicState::Viewport: return "Viewport";
    case DynamicState::Scissor: return "Scissor";
    case DynamicState::LineWidth: return "LineWidth";
    case DynamicState::DepthBias: return "DepthBias";
    case DynamicState::BlendConstants: return "BlendConstants";
    case DynamicState::DepthBounds: return "DepthBounds";
    case DynamicState::StencilCompareMask: return "StencilCompareMask";
    case DynamicState::StencilWriteMask: return "StencilWriteMask";
    case DynamicState::StencilReference: return "StencilReference";
    default: return "unknown";
    }
  }

  static const char * enumString(QueueFlagBits value)
  {
    switch (value)
    {
    case QueueFlagBits::Graphics: return "Graphics";
    case QueueFlagBits::Compute: return "Compute";
    case QueueFlagBits::Transfer: return "Transfer";
    case QueueFlagBits::SparseBinding: return "SparseBinding";
    default: return "unknown";
    }
  }

  static std::string enumString(QueueFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::QueueFlagBits::Graphics) result += "Graphics | ";
    if (value & vk::QueueFlagBits::Compute) result += "Compute | ";
    if (value & vk::QueueFlagBits::Transfer) result += "Transfer | ";
    if (value & vk::QueueFlagBits::SparseBinding) result += "SparseBinding | ";
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * enumString(MemoryPropertyFlagBits value)
  {
    switch (value)
    {
    case MemoryPropertyFlagBits::DeviceLocal: return "DeviceLocal";
    case MemoryPropertyFlagBits::HostVisible: return "HostVisible";
    case MemoryPropertyFlagBits::HostCoherent: return "HostCoherent";
    case MemoryPropertyFlagBits::HostCached: return "HostCached";
    case MemoryPropertyFlagBits::LazilyAllocated: return "LazilyAllocated";
    default: return "unknown";
    }
  }

  static std::string enumString(MemoryPropertyFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::MemoryPropertyFlagBits::DeviceLocal) result += "DeviceLocal | ";
    if (value & vk::MemoryPropertyFlagBits::HostVisible) result += "HostVisible | ";
    if (value & vk::MemoryPropertyFlagBits::HostCoherent) result += "HostCoherent | ";
    if (value & vk::MemoryPropertyFlagBits::HostCached) result += "HostCached | ";
    if (value & vk::MemoryPropertyFlagBits::LazilyAllocated) result += "LazilyAllocated | ";
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * enumString(MemoryHeapFlagBits value)
  {
    switch (value)
    {
    case MemoryHeapFlagBits::DeviceLocal: return "DeviceLocal";
    default: return "unknown";
    }
  }

  static std::string enumString(MemoryHeapFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::MemoryHeapFlagBits::DeviceLocal) result += "DeviceLocal | ";
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * enumString(AccessFlagBits value)
  {
    switch (value)
    {
    case AccessFlagBits::IndirectCommandRead: return "IndirectCommandRead";
    case AccessFlagBits::IndexRead: return "IndexRead";
    case AccessFlagBits::VertexAttributeRead: return "VertexAttributeRead";
    case AccessFlagBits::UniformRead: return "UniformRead";
    case AccessFlagBits::InputAttachmentRead: return "InputAttachmentRead";
    case AccessFlagBits::ShaderRead: return "ShaderRead";
    case AccessFlagBits::ShaderWrite: return "ShaderWrite";
    case AccessFlagBits::ColorAttachmentRead: return "ColorAttachmentRead";
    case AccessFlagBits::ColorAttachmentWrite: return "ColorAttachmentWrite";
    case AccessFlagBits::DepthStencilAttachmentRead: return "DepthStencilAttachmentRead";
    case AccessFlagBits::DepthStencilAttachmentWrite: return "DepthStencilAttachmentWrite";
    case AccessFlagBits::TransferRead: return "TransferRead";
    case AccessFlagBits::TransferWrite: return "TransferWrite";
    case AccessFlagBits::HostRead: return "HostRead";
    case AccessFlagBits::HostWrite: return "HostWrite";
    case AccessFlagBits::MemoryRead: return "MemoryRead";
    case AccessFlagBits::MemoryWrite: return "MemoryWrite";
    default: return "unknown";
    }
  }

  static std::string enumString(AccessFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::AccessFlagBits::IndirectCommandRead) result += "IndirectCommandRead | ";
    if (value & vk::AccessFlagBits::IndexRead) result += "IndexRead | ";
    if (value & vk::AccessFlagBits::VertexAttributeRead) result += "VertexAttributeRead | ";
    if (value & vk::AccessFlagBits::UniformRead) result += "UniformRead | ";
    if (value & vk::AccessFlagBits::InputAttachmentRead) result += "InputAttachmentRead | ";
    if (value & vk::AccessFlagBits::ShaderRead) result += "ShaderRead | ";
    if (value & vk::AccessFlagBits::ShaderWrite) result += "ShaderWrite | ";
    if (value & vk::AccessFlagBits::ColorAttachmentRead) result += "ColorAttachmentRead | ";
    if (value & vk::AccessFlagBits::ColorAttachmentWrite) result += "ColorAttachmentWrite | ";
    if (value & vk::AccessFlagBits::DepthStencilAttachmentRead) result += "DepthStencilAttachmentRead | ";
    if (value & vk::AccessFlagBits::DepthStencilAttachmentWrite) result += "DepthStencilAttachmentWrite | ";
    if (value & vk::AccessFlagBits::TransferRead) result += "TransferRead | ";
    if (value & vk::AccessFlagBits::TransferWrite) result += "TransferWrite | ";
    if (value & vk::AccessFlagBits::HostRead) result += "HostRead | ";
    if (value & vk::AccessFlagBits::HostWrite) result += "HostWrite | ";
    if (value & vk::AccessFlagBits::MemoryRead) result += "MemoryRead | ";
    if (value & vk::AccessFlagBits::MemoryWrite) result += "MemoryWrite | ";
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * enumString(BufferUsageFlagBits value)
  {
    switch (value)
    {
    case BufferUsageFlagBits::TransferSrc: return "TransferSrc";
    case BufferUsageFlagBits::TransferDst: return "TransferDst";
    case BufferUsageFlagBits::UniformTexelBuffer: return "UniformTexelBuffer";
    case BufferUsageFlagBits::StorageTexelBuffer: return "StorageTexelBuffer";
    case BufferUsageFlagBits::UniformBuffer: return "UniformBuffer";
    case BufferUsageFlagBits::StorageBuffer: return "StorageBuffer";
    case BufferUsageFlagBits::IndexBuffer: return "IndexBuffer";
    case BufferUsageFlagBits::VertexBuffer: return "VertexBuffer";
    case BufferUsageFlagBits::IndirectBuffer: return "IndirectBuffer";
    default: return "unknown";
    }
  }

  static std::string enumString(BufferUsageFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::BufferUsageFlagBits::TransferSrc) result += "TransferSrc | ";
    if (value & vk::BufferUsageFlagBits::TransferDst) result += "TransferDst | ";
    if (value & vk::BufferUsageFlagBits::UniformTexelBuffer) result += "UniformTexelBuffer | ";
    if (value & vk::BufferUsageFlagBits::StorageTexelBuffer) result += "StorageTexelBuffer | ";
    if (value & vk::BufferUsageFlagBits::UniformBuffer) result += "UniformBuffer | ";
    if (value & vk::BufferUsageFlagBits::StorageBuffer) result += "StorageBuffer | ";
    if (value & vk::BufferUsageFlagBits::IndexBuffer) result += "IndexBuffer | ";
    if (value & vk::BufferUsageFlagBits::VertexBuffer) result += "VertexBuffer | ";
    if (value & vk::BufferUsageFlagBits::IndirectBuffer) result += "IndirectBuffer | ";
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * enumString(BufferCreateFlagBits value)
  {
    switch (value)
    {
    case BufferCreateFlagBits::SparseBinding: return "SparseBinding";
    case BufferCreateFlagBits::SparseResidency: return "SparseResidency";
    case BufferCreateFlagBits::SparseAliased: return "SparseAliased";
    default: return "unknown";
    }
  }

  static std::string enumString(BufferCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::BufferCreateFlagBits::SparseBinding) result += "SparseBinding | ";
    if (value & vk::BufferCreateFlagBits::SparseResidency) result += "SparseResidency | ";
    if (value & vk::BufferCreateFlagBits::SparseAliased) result += "SparseAliased | ";
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * enumString(ShaderStageFlagBits value)
  {
    switch (value)
    {
    case ShaderStageFlagBits::Vertex: return "Vertex";
    case ShaderStageFlagBits::TessellationControl: return "TessellationControl";
    case ShaderStageFlagBits::TessellationEvaluation: return "TessellationEvaluation";
    case ShaderStageFlagBits::Geometry: return "Geometry";
    case ShaderStageFlagBits::Fragment: return "Fragment";
    case ShaderStageFlagBits::Compute: return "Compute";
    case ShaderStageFlagBits::AllGraphics: return "AllGraphics";
    case ShaderStageFlagBits::All: return "All";
    default: return "unknown";
    }
  }

  static std::string enumString(ShaderStageFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::ShaderStageFlagBits::Vertex) result += "Vertex | ";
    if (value & vk::ShaderStageFlagBits::TessellationControl) result += "TessellationControl | ";
    if (value & vk::ShaderStageFlagBits::TessellationEvaluation) result += "TessellationEvaluation | ";
    if (value & vk::ShaderStageFlagBits::Geometry) result += "Geometry | ";
    if (value & vk::ShaderStageFlagBits::Fragment) result += "Fragment | ";
    if (value & vk::ShaderStageFlagBits::Compute) result += "Compute | ";
    if (value & vk::ShaderStageFlagBits::AllGraphics) result += "AllGraphics | ";
    if (value & vk::ShaderStageFlagBits::All) result += "All | ";
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * enumString(ImageUsageFlagBits value)
  {
    switch (value)
    {
    case ImageUsageFlagBits::TransferSrc: return "TransferSrc";
    case ImageUsageFlagBits::TransferDst: return "TransferDst";
    case ImageUsageFlagBits::Sampled: return "Sampled";
    case ImageUsageFlagBits::Storage: return "Storage";
    case ImageUsageFlagBits::ColorAttachment: return "ColorAttachment";
    case ImageUsageFlagBits::DepthStencilAttachment: return "DepthStencilAttachment";
    case ImageUsageFlagBits::TransientAttachment: return "TransientAttachment";
    case ImageUsageFlagBits::InputAttachment: return "InputAttachment";
    default: return "unknown";
    }
  }

  static std::string enumString(ImageUsageFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::ImageUsageFlagBits::TransferSrc) result += "TransferSrc | ";
    if (value & vk::ImageUsageFlagBits::TransferDst) result += "TransferDst | ";
    if (value & vk::ImageUsageFlagBits::Sampled) result += "Sampled | ";
    if (value & vk::ImageUsageFlagBits::Storage) result += "Storage | ";
    if (value & vk::ImageUsageFlagBits::ColorAttachment) result += "ColorAttachment | ";
    if (value & vk::ImageUsageFlagBits::DepthStencilAttachment) result += "DepthStencilAttachment | ";
    if (value & vk::ImageUsageFlagBits::TransientAttachment) result += "TransientAttachment | ";
    if (value & vk::ImageUsageFlagBits::InputAttachment) result += "InputAttachment | ";
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * enumString(ImageCreateFlagBits value)
  {
    switch (value)
    {
    case ImageCreateFlagBits::SparseBinding: return "SparseBinding";
    case ImageCreateFlagBits::SparseResidency: return "SparseResidency";
    case ImageCreateFlagBits::SparseAliased: return "SparseAliased";
    case ImageCreateFlagBits::MutableFormat: return "MutableFormat";
    case ImageCreateFlagBits::CubeCompatible: return "CubeCompatible";
    default: return "unknown";
    }
  }

  static std::string enumString(ImageCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::ImageCreateFlagBits::SparseBinding) result += "SparseBinding | ";
    if (value & vk::ImageCreateFlagBits::SparseResidency) result += "SparseResidency | ";
    if (value & vk::ImageCreateFlagBits::SparseAliased) result += "SparseAliased | ";
    if (value & vk::ImageCreateFlagBits::MutableFormat) result += "MutableFormat | ";
    if (value & vk::ImageCreateFlagBits::CubeCompatible) result += "CubeCompatible | ";
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * enumString(PipelineCreateFlagBits value)
  {
    switch (value)
    {
    case PipelineCreateFlagBits::DisableOptimization: return "DisableOptimization";
    case PipelineCreateFlagBits::AllowDerivatives: return "AllowDerivatives";
    case PipelineCreateFlagBits::Derivative: return "Derivative";
    default: return "unknown";
    }
  }

  static std::string enumString(PipelineCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::PipelineCreateFlagBits::DisableOptimization) result += "DisableOptimization | ";
    if (value & vk::PipelineCreateFlagBits::AllowDerivatives) result += "AllowDerivatives | ";
    if (value & vk::PipelineCreateFlagBits::Derivative) result += "Derivative | ";
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * enumString(ColorComponentFlagBits value)
  {
    switch (value)
    {
    case ColorComponentFlagBits::R: return "R";
    case ColorComponentFlagBits::G: return "G";
    case ColorComponentFlagBits::B: return "B";
    case ColorComponentFlagBits::A: return "A";
    default: return "unknown";
    }
  }

  static std::string enumString(ColorComponentFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::ColorComponentFlagBits::R) result += "R | ";
    if (value & vk::ColorComponentFlagBits::G) result += "G | ";
    if (value & vk::ColorComponentFlagBits::B) result += "B | ";
    if (value & vk::ColorComponentFlagBits::A) result += "A | ";
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * enumString(FenceCreateFlagBits value)
  {
    switch (value)
    {
    case FenceCreateFlagBits::Signaled: return "Signaled";
    default: return "unknown";
    }
  }

  static std::string enumString(FenceCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::FenceCreateFlagBits::Signaled) result += "Signaled | ";
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * enumString(FormatFeatureFlagBits value)
  {
    switch (value)
    {
    case FormatFeatureFlagBits::SampledImage: return "SampledImage";
    case FormatFeatureFlagBits::StorageImage: return "StorageImage";
    case FormatFeatureFlagBits::StorageImageAtomic: return "StorageImageAtomic";
    case FormatFeatureFlagBits::UniformTexelBuffer: return "UniformTexelBuffer";
    case FormatFeatureFlagBits::StorageTexelBuffer: return "StorageTexelBuffer";
    case FormatFeatureFlagBits::StorageTexelBufferAtomic: return "StorageTexelBufferAtomic";
    case FormatFeatureFlagBits::VertexBuffer: return "VertexBuffer";
    case FormatFeatureFlagBits::ColorAttachment: return "ColorAttachment";
    case FormatFeatureFlagBits::ColorAttachmentBlend: return "ColorAttachmentBlend";
    case FormatFeatureFlagBits::DepthStencilAttachment: return "DepthStencilAttachment";
    case FormatFeatureFlagBits::BlitSrc: return "BlitSrc";
    case FormatFeatureFlagBits::BlitDst: return "BlitDst";
    case FormatFeatureFlagBits::SampledImageFilterLinear: return "SampledImageFilterLinear";
    default: return "unknown";
    }
  }

  static std::string enumString(FormatFeatureFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::FormatFeatureFlagBits::SampledImage) result += "SampledImage | ";
    if (value & vk::FormatFeatureFlagBits::StorageImage) result += "StorageImage | ";
    if (value & vk::FormatFeatureFlagBits::StorageImageAtomic) result += "StorageImageAtomic | ";
    if (value & vk::FormatFeatureFlagBits::UniformTexelBuffer) result += "UniformTexelBuffer | ";
    if (value & vk::FormatFeatureFlagBits::StorageTexelBuffer) result += "StorageTexelBuffer | ";
    if (value & vk::FormatFeatureFlagBits::StorageTexelBufferAtomic) result += "StorageTexelBufferAtomic | ";
    if (value & vk::FormatFeatureFlagBits::VertexBuffer) result += "VertexBuffer | ";
    if (value & vk::FormatFeatureFlagBits::ColorAttachment) result += "ColorAttachment | ";
    if (value & vk::FormatFeatureFlagBits::ColorAttachmentBlend) result += "ColorAttachmentBlend | ";
    if (value & vk::FormatFeatureFlagBits::DepthStencilAttachment) result += "DepthStencilAttachment | ";
    if (value & vk::FormatFeatureFlagBits::BlitSrc) result += "BlitSrc | ";
    if (value & vk::FormatFeatureFlagBits::BlitDst) result += "BlitDst | ";
    if (value & vk::FormatFeatureFlagBits::SampledImageFilterLinear) result += "SampledImageFilterLinear | ";
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * enumString(QueryControlFlagBits value)
  {
    switch (value)
    {
    case QueryControlFlagBits::Precise: return "Precise";
    default: return "unknown";
    }
  }

  static std::string enumString(QueryControlFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::QueryControlFlagBits::Precise) result += "Precise | ";
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * enumString(QueryResultFlagBits value)
  {
    switch (value)
    {
    case QueryResultFlagBits::e64: return "e64";
    case QueryResultFlagBits::Wait: return "Wait";
    case QueryResultFlagBits::WithAvailability: return "WithAvailability";
    case QueryResultFlagBits::Partial: return "Partial";
    default: return "unknown";
    }
  }

  static std::string enumString(QueryResultFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::QueryResultFlagBits::e64) result += "e64 | ";
    if (value & vk::QueryResultFlagBits::Wait) result += "Wait | ";
    if (value & vk::QueryResultFlagBits::WithAvailability) result += "WithAvailability | ";
    if (value & vk::QueryResultFlagBits::Partial) result += "Partial | ";
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * enumString(CommandBufferUsageFlagBits value)
  {
    switch (value)
    {
    case CommandBufferUsageFlagBits::OneTimeSubmit: return "OneTimeSubmit";
    case CommandBufferUsageFlagBits::RenderPassContinue: return "RenderPassContinue";
    case CommandBufferUsageFlagBits::SimultaneousUse: return "SimultaneousUse";
    default: return "unknown";
    }
  }

  static std::string enumString(CommandBufferUsageFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::CommandBufferUsageFlagBits::OneTimeSubmit) result += "OneTimeSubmit | ";
    if (value & vk::CommandBufferUsageFlagBits::RenderPassContinue) result += "RenderPassContinue | ";
    if (value & vk::CommandBufferUsageFlagBits::SimultaneousUse) result += "SimultaneousUse | ";
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * enumString(QueryPipelineStatisticFlagBits value)
  {
    switch (value)
    {
    case QueryPipelineStatisticFlagBits::InputAssemblyVertices: return "InputAssemblyVertices";
    case QueryPipelineStatisticFlagBits::InputAssemblyPrimitives: return "InputAssemblyPrimitives";
    case QueryPipelineStatisticFlagBits::VertexShaderInvocations: return "VertexShaderInvocations";
    case QueryPipelineStatisticFlagBits::GeometryShaderInvocations: return "GeometryShaderInvocations";
    case QueryPipelineStatisticFlagBits::GeometryShaderPrimitives: return "GeometryShaderPrimitives";
    case QueryPipelineStatisticFlagBits::ClippingInvocations: return "ClippingInvocations";
    case QueryPipelineStatisticFlagBits::ClippingPrimitives: return "ClippingPrimitives";
    case QueryPipelineStatisticFlagBits::FragmentShaderInvocations: return "FragmentShaderInvocations";
    case QueryPipelineStatisticFlagBits::TessellationControlShaderPatches: return "TessellationControlShaderPatches";
    case QueryPipelineStatisticFlagBits::TessellationEvaluationShaderInvocations: return "TessellationEvaluationShaderInvocations";
    case QueryPipelineStatisticFlagBits::ComputeShaderInvocations: return "ComputeShaderInvocations";
    default: return "unknown";
    }
  }

  static std::string enumString(QueryPipelineStatisticFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::QueryPipelineStatisticFlagBits::InputAssemblyVertices) result += "InputAssemblyVertices | ";
    if (value & vk::QueryPipelineStatisticFlagBits::InputAssemblyPrimitives) result += "InputAssemblyPrimitives | ";
    if (value & vk::QueryPipelineStatisticFlagBits::VertexShaderInvocations) result += "VertexShaderInvocations | ";
    if (value & vk::QueryPipelineStatisticFlagBits::GeometryShaderInvocations) result += "GeometryShaderInvocations | ";
    if (value & vk::QueryPipelineStatisticFlagBits::GeometryShaderPrimitives) result += "GeometryShaderPrimitives | ";
    if (value & vk::QueryPipelineStatisticFlagBits::ClippingInvocations) result += "ClippingInvocations | ";
    if (value & vk::QueryPipelineStatisticFlagBits::ClippingPrimitives) result += "ClippingPrimitives | ";
    if (value & vk::QueryPipelineStatisticFlagBits::FragmentShaderInvocations) result += "FragmentShaderInvocations | ";
    if (value & vk::QueryPipelineStatisticFlagBits::TessellationControlShaderPatches) result += "TessellationControlShaderPatches | ";
    if (value & vk::QueryPipelineStatisticFlagBits::TessellationEvaluationShaderInvocations) result += "TessellationEvaluationShaderInvocations | ";
    if (value & vk::QueryPipelineStatisticFlagBits::ComputeShaderInvocations) result += "ComputeShaderInvocations | ";
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * enumString(ImageAspectFlagBits value)
  {
    switch (value)
    {
    case ImageAspectFlagBits::Color: return "Color";
    case ImageAspectFlagBits::Depth: return "Depth";
    case ImageAspectFlagBits::Stencil: return "Stencil";
    case ImageAspectFlagBits::Metadata: return "Metadata";
    default: return "unknown";
    }
  }

  static std::string enumString(ImageAspectFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::ImageAspectFlagBits::Color) result += "Color | ";
    if (value & vk::ImageAspectFlagBits::Depth) result += "Depth | ";
    if (value & vk::ImageAspectFlagBits::Stencil) result += "Stencil | ";
    if (value & vk::ImageAspectFlagBits::Metadata) result += "Metadata | ";
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * enumString(SparseImageFormatFlagBits value)
  {
    switch (value)
    {
    case SparseImageFormatFlagBits::SingleMiptail: return "SingleMiptail";
    case SparseImageFormatFlagBits::AlignedMipSize: return "AlignedMipSize";
    case SparseImageFormatFlagBits::NonstandardBlockSize: return "NonstandardBlockSize";
    default: return "unknown";
    }
  }

  static std::string enumString(SparseImageFormatFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::SparseImageFormatFlagBits::SingleMiptail) result += "SingleMiptail | ";
    if (value & vk::SparseImageFormatFlagBits::AlignedMipSize) result += "AlignedMipSize | ";
    if (value & vk::SparseImageFormatFlagBits::NonstandardBlockSize) result += "NonstandardBlockSize | ";
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * enumString(SparseMemoryBindFlagBits value)
  {
    switch (value)
    {
    case SparseMemoryBindFlagBits::Metadata: return "Metadata";
    default: return "unknown";
    }
  }

  static std::string enumString(SparseMemoryBindFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::SparseMemoryBindFlagBits::Metadata) result += "Metadata | ";
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * enumString(PipelineStageFlagBits value)
  {
    switch (value)
    {
    case PipelineStageFlagBits::TopOfPipe: return "TopOfPipe";
    case PipelineStageFlagBits::DrawIndirect: return "DrawIndirect";
    case PipelineStageFlagBits::VertexInput: return "VertexInput";
    case PipelineStageFlagBits::VertexShader: return "VertexShader";
    case PipelineStageFlagBits::TessellationControlShader: return "TessellationControlShader";
    case PipelineStageFlagBits::TessellationEvaluationShader: return "TessellationEvaluationShader";
    case PipelineStageFlagBits::GeometryShader: return "GeometryShader";
    case PipelineStageFlagBits::FragmentShader: return "FragmentShader";
    case PipelineStageFlagBits::EarlyFragmentTests: return "EarlyFragmentTests";
    case PipelineStageFlagBits::LateFragmentTests: return "LateFragmentTests";
    case PipelineStageFlagBits::ColorAttachmentOutput: return "ColorAttachmentOutput";
    case PipelineStageFlagBits::ComputeShader: return "ComputeShader";
    case PipelineStageFlagBits::Transfer: return "Transfer";
    case PipelineStageFlagBits::BottomOfPipe: return "BottomOfPipe";
    case PipelineStageFlagBits::Host: return "Host";
    case PipelineStageFlagBits::AllGraphics: return "AllGraphics";
    case PipelineStageFlagBits::AllCommands: return "AllCommands";
    default: return "unknown";
    }
  }

  static std::string enumString(PipelineStageFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::PipelineStageFlagBits::TopOfPipe) result += "TopOfPipe | ";
    if (value & vk::PipelineStageFlagBits::DrawIndirect) result += "DrawIndirect | ";
    if (value & vk::PipelineStageFlagBits::VertexInput) result += "VertexInput | ";
    if (value & vk::PipelineStageFlagBits::VertexShader) result += "VertexShader | ";
    if (value & vk::PipelineStageFlagBits::TessellationControlShader) result += "TessellationControlShader | ";
    if (value & vk::PipelineStageFlagBits::TessellationEvaluationShader) result += "TessellationEvaluationShader | ";
    if (value & vk::PipelineStageFlagBits::GeometryShader) result += "GeometryShader | ";
    if (value & vk::PipelineStageFlagBits::FragmentShader) result += "FragmentShader | ";
    if (value & vk::PipelineStageFlagBits::EarlyFragmentTests) result += "EarlyFragmentTests | ";
    if (value & vk::PipelineStageFlagBits::LateFragmentTests) result += "LateFragmentTests | ";
    if (value & vk::PipelineStageFlagBits::ColorAttachmentOutput) result += "ColorAttachmentOutput | ";
    if (value & vk::PipelineStageFlagBits::ComputeShader) result += "ComputeShader | ";
    if (value & vk::PipelineStageFlagBits::Transfer) result += "Transfer | ";
    if (value & vk::PipelineStageFlagBits::BottomOfPipe) result += "BottomOfPipe | ";
    if (value & vk::PipelineStageFlagBits::Host) result += "Host | ";
    if (value & vk::PipelineStageFlagBits::AllGraphics) result += "AllGraphics | ";
    if (value & vk::PipelineStageFlagBits::AllCommands) result += "AllCommands | ";
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * enumString(CommandPoolCreateFlagBits value)
  {
    switch (value)
    {
    case CommandPoolCreateFlagBits::Transient: return "Transient";
    case CommandPoolCreateFlagBits::ResetCommandBuffer: return "ResetCommandBuffer";
    default: return "unknown";
    }
  }

  static std::string enumString(CommandPoolCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::CommandPoolCreateFlagBits::Transient) result += "Transient | ";
    if (value & vk::CommandPoolCreateFlagBits::ResetCommandBuffer) result += "ResetCommandBuffer | ";
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * enumString(CommandPoolResetFlagBits value)
  {
    switch (value)
    {
    case CommandPoolResetFlagBits::ReleaseResources: return "ReleaseResources";
    default: return "unknown";
    }
  }

  static std::string enumString(CommandPoolResetFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::CommandPoolResetFlagBits::ReleaseResources) result += "ReleaseResources | ";
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * enumString(CommandBufferResetFlagBits value)
  {
    switch (value)
    {
    case CommandBufferResetFlagBits::ReleaseResources: return "ReleaseResources";
    default: return "unknown";
    }
  }

  static std::string enumString(CommandBufferResetFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::CommandBufferResetFlagBits::ReleaseResources) result += "ReleaseResources | ";
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * enumString(SampleCountFlagBits value)
  {
    switch (value)
    {
    case SampleCountFlagBits::e1: return "e1";
    case SampleCountFlagBits::e2: return "e2";
    case SampleCountFlagBits::e4: return "e4";
    case SampleCountFlagBits::e8: return "e8";
    case SampleCountFlagBits::e16: return "e16";
    case SampleCountFlagBits::e32: return "e32";
    case SampleCountFlagBits::e64: return "e64";
    default: return "unknown";
    }
  }

  static std::string enumString(SampleCountFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::SampleCountFlagBits::e1) result += "e1 | ";
    if (value & vk::SampleCountFlagBits::e2) result += "e2 | ";
    if (value & vk::SampleCountFlagBits::e4) result += "e4 | ";
    if (value & vk::SampleCountFlagBits::e8) result += "e8 | ";
    if (value & vk::SampleCountFlagBits::e16) result += "e16 | ";
    if (value & vk::SampleCountFlagBits::e32) result += "e32 | ";
    if (value & vk::SampleCountFlagBits::e64) result += "e64 | ";
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * enumString(AttachmentDescriptionFlagBits value)
  {
    switch (value)
    {
    case AttachmentDescriptionFlagBits::MayAlias: return "MayAlias";
    default: return "unknown";
    }
  }

  static std::string enumString(AttachmentDescriptionFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::AttachmentDescriptionFlagBits::MayAlias) result += "MayAlias | ";
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * enumString(StencilFaceFlagBits value)
  {
    switch (value)
    {
    case StencilFaceFlagBits::Front: return "Front";
    case StencilFaceFlagBits::Back: return "Back";
    case StencilFaceFlagBits::VkStencilFrontAndBack: return "VkStencilFrontAndBack";
    default: return "unknown";
    }
  }

  static std::string enumString(StencilFaceFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::StencilFaceFlagBits::Front) result += "Front | ";
    if (value & vk::StencilFaceFlagBits::Back) result += "Back | ";
    if (value & vk::StencilFaceFlagBits::VkStencilFrontAndBack) result += "VkStencilFrontAndBack | ";
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * enumString(DescriptorPoolCreateFlagBits value)
  {
    switch (value)
    {
    case DescriptorPoolCreateFlagBits::FreeDescriptorSet: return "FreeDescriptorSet";
    default: return "unknown";
    }
  }

  static std::string enumString(DescriptorPoolCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::DescriptorPoolCreateFlagBits::FreeDescriptorSet) result += "FreeDescriptorSet | ";
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * enumString(DependencyFlagBits value)
  {
    switch (value)
    {
    case DependencyFlagBits::ByRegion: return "ByRegion";
    default: return "unknown";
    }
  }

  static std::string enumString(DependencyFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::DependencyFlagBits::ByRegion) result += "ByRegion | ";
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * enumString(PresentModeKHR value)
  {
    switch (value)
    {
    case PresentModeKHR::ImmediateKHR: return "ImmediateKHR";
    case PresentModeKHR::MailboxKHR: return "MailboxKHR";
    case PresentModeKHR::FifoKHR: return "FifoKHR";
    case PresentModeKHR::FifoRelaxedKHR: return "FifoRelaxedKHR";
    default: return "unknown";
    }
  }

  static const char * enumString(ColorSpaceKHR value)
  {
    switch (value)
    {
    case ColorSpaceKHR::VkColorspaceSrgbNonlinearKHR: return "VkColorspaceSrgbNonlinearKHR";
    default: return "unknown";
    }
  }

  static const char * enumString(CompositeAlphaFlagBitsKHR value)
  {
    switch (value)
    {
    case CompositeAlphaFlagBitsKHR::OpaqueKHR: return "OpaqueKHR";
    case CompositeAlphaFlagBitsKHR::PreMultipliedKHR: return "PreMultipliedKHR";
    case CompositeAlphaFlagBitsKHR::PostMultipliedKHR: return "PostMultipliedKHR";
    case CompositeAlphaFlagBitsKHR::InheritKHR: return "InheritKHR";
    default: return "unknown";
    }
  }

  static std::string enumString(CompositeAlphaFlagsKHR value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::CompositeAlphaFlagBitsKHR::OpaqueKHR) result += "OpaqueKHR | ";
    if (value & vk::CompositeAlphaFlagBitsKHR::PreMultipliedKHR) result += "PreMultipliedKHR | ";
    if (value & vk::CompositeAlphaFlagBitsKHR::PostMultipliedKHR) result += "PostMultipliedKHR | ";
    if (value & vk::CompositeAlphaFlagBitsKHR::InheritKHR) result += "InheritKHR | ";
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * enumString(SurfaceTransformFlagBitsKHR value)
  {
    switch (value)
    {
    case SurfaceTransformFlagBitsKHR::IdentityKHR: return "IdentityKHR";
    case SurfaceTransformFlagBitsKHR::Rotate90KHR: return "Rotate90KHR";
    case SurfaceTransformFlagBitsKHR::Rotate180KHR: return "Rotate180KHR";
    case SurfaceTransformFlagBitsKHR::Rotate270KHR: return "Rotate270KHR";
    case SurfaceTransformFlagBitsKHR::HorizontalMirrorKHR: return "HorizontalMirrorKHR";
    case SurfaceTransformFlagBitsKHR::HorizontalMirrorRotate90KHR: return "HorizontalMirrorRotate90KHR";
    case SurfaceTransformFlagBitsKHR::HorizontalMirrorRotate180KHR: return "HorizontalMirrorRotate180KHR";
    case SurfaceTransformFlagBitsKHR::HorizontalMirrorRotate270KHR: return "HorizontalMirrorRotate270KHR";
    case SurfaceTransformFlagBitsKHR::InheritKHR: return "InheritKHR";
    default: return "unknown";
    }
  }

  static std::string enumString(SurfaceTransformFlagsKHR value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::SurfaceTransformFlagBitsKHR::IdentityKHR) result += "IdentityKHR | ";
    if (value & vk::SurfaceTransformFlagBitsKHR::Rotate90KHR) result += "Rotate90KHR | ";
    if (value & vk::SurfaceTransformFlagBitsKHR::Rotate180KHR) result += "Rotate180KHR | ";
    if (value & vk::SurfaceTransformFlagBitsKHR::Rotate270KHR) result += "Rotate270KHR | ";
    if (value & vk::SurfaceTransformFlagBitsKHR::HorizontalMirrorKHR) result += "HorizontalMirrorKHR | ";
    if (value & vk::SurfaceTransformFlagBitsKHR::HorizontalMirrorRotate90KHR) result += "HorizontalMirrorRotate90KHR | ";
    if (value & vk::SurfaceTransformFlagBitsKHR::HorizontalMirrorRotate180KHR) result += "HorizontalMirrorRotate180KHR | ";
    if (value & vk::SurfaceTransformFlagBitsKHR::HorizontalMirrorRotate270KHR) result += "HorizontalMirrorRotate270KHR | ";
    if (value & vk::SurfaceTransformFlagBitsKHR::InheritKHR) result += "InheritKHR | ";
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * enumString(DebugReportFlagBitsEXT value)
  {
    switch (value)
    {
    case DebugReportFlagBitsEXT::InformationEXT: return "InformationEXT";
    case DebugReportFlagBitsEXT::WarningEXT: return "WarningEXT";
    case DebugReportFlagBitsEXT::PerformanceWarningEXT: return "PerformanceWarningEXT";
    case DebugReportFlagBitsEXT::ErrorEXT: return "ErrorEXT";
    case DebugReportFlagBitsEXT::DebugEXT: return "DebugEXT";
    default: return "unknown";
    }
  }

  static std::string enumString(DebugReportFlagsEXT value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::DebugReportFlagBitsEXT::InformationEXT) result += "InformationEXT | ";
    if (value & vk::DebugReportFlagBitsEXT::WarningEXT) result += "WarningEXT | ";
    if (value & vk::DebugReportFlagBitsEXT::PerformanceWarningEXT) result += "PerformanceWarningEXT | ";
    if (value & vk::DebugReportFlagBitsEXT::ErrorEXT) result += "ErrorEXT | ";
    if (value & vk::DebugReportFlagBitsEXT::DebugEXT) result += "DebugEXT | ";
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
  static const char * enumString(DisplayPlaneAlphaFlagBitsKHR value)
  {
    switch (value)
    {
    case DisplayPlaneAlphaFlagBitsKHR::OpaqueKHR: return "OpaqueKHR";
    case DisplayPlaneAlphaFlagBitsKHR::GlobalKHR: return "GlobalKHR";
    case DisplayPlaneAlphaFlagBitsKHR::PerPixelKHR: return "PerPixelKHR";
    case DisplayPlaneAlphaFlagBitsKHR::PerPixelPremultipliedKHR: return "PerPixelPremultipliedKHR";
    default: return "unknown";
    }
  }

  static std::string enumString(DisplayPlaneAlphaFlagsKHR value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::DisplayPlaneAlphaFlagBitsKHR::OpaqueKHR) result += "OpaqueKHR | ";
    if (value & vk::DisplayPlaneAlphaFlagBitsKHR::GlobalKHR) result += "GlobalKHR | ";
    if (value & vk::DisplayPlaneAlphaFlagBitsKHR::PerPixelKHR) result += "PerPixelKHR | ";
    if (value & vk::DisplayPlaneAlphaFlagBitsKHR::PerPixelPremultipliedKHR) result += "PerPixelPremultipliedKHR | ";
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
#ifdef VK_USE_PLATFORM_XLIB_KHR

  static std::string enumString(XlibSurfaceCreateFlagsKHR value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
#endif /* VK_USE_PLATFORM_XLIB_KHR */

#ifdef VK_USE_PLATFORM_XCB_KHR

  static std::string enumString(XcbSurfaceCreateFlagsKHR value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
#endif /* VK_USE_PLATFORM_XCB_KHR */

#ifdef VK_USE_PLATFORM_WAYLAND_KHR

  static std::string enumString(WaylandSurfaceCreateFlagsKHR value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
#endif /* VK_USE_PLATFORM_WAYLAND_KHR */

#ifdef VK_USE_PLATFORM_MIR_KHR

  static std::string enumString(MirSurfaceCreateFlagsKHR value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
#endif /* VK_USE_PLATFORM_MIR_KHR */

#ifdef VK_USE_PLATFORM_ANDROID_KHR

  static std::string enumString(AndroidSurfaceCreateFlagsKHR value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
#endif /* VK_USE_PLATFORM_ANDROID_KHR */

#ifdef VK_USE_PLATFORM_WIN32_KHR

  static std::string enumString(Win32SurfaceCreateFlagsKHR value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - (result.size() > 3 ? 3 : 0));
  }
#endif /* VK_USE_PLATFORM_WIN32_KHR */

  static const char * enumString(DebugReportObjectTypeEXT value)
  {
    switch (value)
    {
    case DebugReportObjectTypeEXT::UnknownEXT: return "UnknownEXT";
    case DebugReportObjectTypeEXT::InstanceEXT: return "InstanceEXT";
    case DebugReportObjectTypeEXT::PhysicalDeviceEXT: return "PhysicalDeviceEXT";
    case DebugReportObjectTypeEXT::DeviceEXT: return "DeviceEXT";
    case DebugReportObjectTypeEXT::QueueEXT: return "QueueEXT";
    case DebugReportObjectTypeEXT::SemaphoreEXT: return "SemaphoreEXT";
    case DebugReportObjectTypeEXT::CommandBufferEXT: return "CommandBufferEXT";
    case DebugReportObjectTypeEXT::FenceEXT: return "FenceEXT";
    case DebugReportObjectTypeEXT::DeviceMemoryEXT: return "DeviceMemoryEXT";
    case DebugReportObjectTypeEXT::BufferEXT: return "BufferEXT";
    case DebugReportObjectTypeEXT::ImageEXT: return "ImageEXT";
    case DebugReportObjectTypeEXT::EventEXT: return "EventEXT";
    case DebugReportObjectTypeEXT::QueryPoolEXT: return "QueryPoolEXT";
    case DebugReportObjectTypeEXT::BufferViewEXT: return "BufferViewEXT";
    case DebugReportObjectTypeEXT::ImageViewEXT: return "ImageViewEXT";
    case DebugReportObjectTypeEXT::ShaderModuleEXT: return "ShaderModuleEXT";
    case DebugReportObjectTypeEXT::PipelineCacheEXT: return "PipelineCacheEXT";
    case DebugReportObjectTypeEXT::PipelineLayoutEXT: return "PipelineLayoutEXT";
    case DebugReportObjectTypeEXT::RenderPassEXT: return "RenderPassEXT";
    case DebugReportObjectTypeEXT::PipelineEXT: return "PipelineEXT";
    case DebugReportObjectTypeEXT::DescriptorSetLayoutEXT: return "DescriptorSetLayoutEXT";
    case DebugReportObjectTypeEXT::SamplerEXT: return "SamplerEXT";
    case DebugReportObjectTypeEXT::DescriptorPoolEXT: return "DescriptorPoolEXT";
    case DebugReportObjectTypeEXT::DescriptorSetEXT: return "DescriptorSetEXT";
    case DebugReportObjectTypeEXT::FramebufferEXT: return "FramebufferEXT";
    case DebugReportObjectTypeEXT::CommandPoolEXT: return "CommandPoolEXT";
    case DebugReportObjectTypeEXT::SurfaceKhrEXT: return "SurfaceKhrEXT";
    case DebugReportObjectTypeEXT::SwapchainKhrEXT: return "SwapchainKhrEXT";
    case DebugReportObjectTypeEXT::DebugReportEXT: return "DebugReportEXT";
    default: return "unknown";
    }
  }

  static const char * enumString(DebugReportErrorEXT value)
  {
    switch (value)
    {
    case DebugReportErrorEXT::NoneEXT: return "NoneEXT";
    case DebugReportErrorEXT::CallbackRefEXT: return "CallbackRefEXT";
    default: return "unknown";
    }
  }

}
