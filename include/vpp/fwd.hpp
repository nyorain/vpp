#pragma once

#include <vpp/vulkan/fwd.hpp>
#include <vpp/config.hpp>

namespace vpp {

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

}
