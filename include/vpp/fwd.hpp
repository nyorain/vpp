#pragma once

#include <vpp/vulkan/fwd.hpp>
#include <vpp/config.hpp>

namespace vpp
{

using vk::Range;

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

//see utility/memory_resource.hpp
//ugly hack to not pull all boost headers from device.hpp
//will be removed with c++17.
namespace boost { namespace container { namespace pmr {
	class memory_resource;
}}}

namespace std { namespace pmr {
	using memory_resource = boost::container::pmr::memory_resource;
}}
