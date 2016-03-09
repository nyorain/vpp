#pragma once

namespace vpp
{

class Context;
class Device;
class Resource;
class Surface;
class SwapChain;
class RendererBuilder;
class SwapChainRenderer;
class ShaderStage;
class ShaderProgram;
class DeviceMemory;
class DeviceMemoryAllocator;
class Buffer;
class Image;
class Pipeline;
class GraphicsPipeline;
class ComputePipeline;
class Buffer;
class DescriptorSet;
class VertexBufferLayout;
class DescriptorSetLayout;
class Framebuffer;
class FramebufferAttachment;
class RenderPass;
class RenderPassInstance;
class Texture;
class Allocator;

//utility
template<typename T>
class Range
{
public:
	T* ptr;
	unsigned int size;

public:
	constexpr T* begin() const { return ptr; }
	constexpr T* end() const { return ptr + size; }
};

template<typename T>
Range<T> makeRange(T* ptr, unsigned int size)
{
	return Range<T>{ptr, size};
}

}
