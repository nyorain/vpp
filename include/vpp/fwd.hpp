// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vpp/vulkan/fwd.hpp>
#include <vpp/config.hpp>

#include <cstdint> // std::uint8_t
#include <cstddef> // std::size_t

namespace vpp {

// treat them as built-in
using std::uint8_t;
using std::uint16_t;
using std::uint32_t;
using std::uint64_t;

using std::int8_t;
using std::int16_t;
using std::int32_t;
using std::int64_t;

using std::size_t;

class Resource;
class WorkBase;

class Buffer;
class Image;
class Surface;
class SwapChain;
class ShaderStage;
class ShaderProgram;
class DeviceMemory;
class Pipeline;
class DebugCallback;
class DescriptorSet;
class DescriptorPool;
class VertexBufferLayout;
class DescriptorSetLayout;
class Framebuffer;
class RenderPass;
class CommandPool;
class CommandBuffer;
class PipelineCache;
class PipelineLayout;

class Context;
class Device;
class Queue;
class RendererBuilder;
class SwapChainRenderer;
class DeviceMemoryAllocator;
class MemoryEntry;
class ViewableImage;
class RenderPassInstance;
class GraphicsPipelineBuilder;
class ComputePipelineBuilder;

class CommandProvider;
class DeviceMemoryProvider;
class HostMemoryProvider;
class SubmitManager;
class WorkManager;
class TransferManager;

template<typename T>
class ThreadStorage;
class DynamicStorageBase;

}
