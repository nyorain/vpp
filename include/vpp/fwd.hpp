#pragma once

#include <vpp/vulkan/fwd.hpp>

//The VPP_DEBUG macro controls whether additional checks should be performed or not.
#ifndef VPP_DEBUG
 #ifdef NDEBUG
  #define VPP_DEBUG 0
 #else
  #define VPP_DEBUG 1
 #endif
#endif

namespace vk { namespace range {} }
namespace vpp
{

using namespace vk::range;

class Context;
class Device;
class Resource;
class Queue;
class Surface;
class SwapChain;
class RendererBuilder;
class SwapChainRenderer;
class ShaderStage;
class ShaderProgram;
class DeviceMemory;
class DeviceMemoryAllocator;
class MemoryEntry;
class Buffer;
class Image;
class DebugCallback;
class Pipeline;
class GraphicsPipeline;
class ComputePipeline;
class Buffer;
class DescriptorSet;
class VertexBufferLayout;
class DescriptorSetLayout;
class Framebuffer;
class ViewableImage;
class RenderPass;
class RenderPassInstance;
class Texture;
class Allocator;
class CommandProvider;
class DeviceMemoryProvider;
class HostMemoryProvider;
class CommandPool;
class CommandBuffer;
class SubmitManager;
class WorkBase;
class WorkManager;
class TransferManager;

}
