// Copyright (c) 2016-2019 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

/// File defines simple RAII wrappers around vulkan resources.
/// They usually don't have any additional features except more comfortable
/// construction and automatic resource destruction in the destructor.

#pragma once

#include <vpp/fwd.hpp>
#include <vpp/resource.hpp>
#include <vector>

namespace vpp {

/// RAII vulkan Instance wrapper.
/// Will always automatically initialize vk::dispatch on construction.
class Instance : public nytl::NonCopyable {
public:
	Instance() = default;
	Instance(const vk::InstanceCreateInfo& info);
	Instance(vk::Instance instance);
	~Instance();

	Instance(Instance&& other) noexcept;
	Instance& operator=(Instance&& other) noexcept;

	vk::Instance vkInstance() const { return instance_; }
	vk::Instance vkHandle() const { return instance_; }
	operator vk::Instance() const { return instance_; }

protected:
	void release() { instance_ = {}; }

protected:
	vk::Instance instance_ {};
};

/// RAII vk::SurfaceKHR wrapper.
class Surface {
public:
	Surface() = default;
	Surface(vk::Instance instance, vk::SurfaceKHR surface);
	~Surface();

	Surface(Surface&& other) noexcept { swap(*this, other); }
	Surface& operator=(Surface rhs) noexcept {
		swap(*this, rhs);
		return *this;
	}

	vk::Instance vkInstance() const { return instance_; }
	vk::SurfaceKHR vkHandle() const { return surface_; }

	operator vk::SurfaceKHR() const { return surface_; }
	friend void swap(Surface& a, Surface& b) noexcept;

protected:
	void release() { instance_ = {}; surface_ = {}; }

protected:
	vk::Instance instance_ {};
	vk::SurfaceKHR surface_ {};
};

/// RAII vk::QueryPool wrapper.
class QueryPool : public ResourceHandle<vk::QueryPool> {
public:
	QueryPool() = default;
	QueryPool(const Device&, vk::QueryPool);
	QueryPool(const Device&, const vk::QueryPoolCreateInfo&);
	~QueryPool();

	QueryPool(QueryPool&& rhs) noexcept { swap(*this, rhs); }
	auto& operator=(QueryPool rhs) noexcept {
		swap(*this, rhs);
		return *this;
	}
};

/// RAII Fence wrapper
class Fence : public ResourceHandle<vk::Fence> {
public:
	Fence() = default;
	Fence(const Device&);
	Fence(const Device&, vk::Fence);
	Fence(const Device&, const vk::FenceCreateInfo&);
	~Fence();

	Fence(Fence&& rhs) noexcept { swap(*this, rhs); }
	Fence& operator=(Fence rhs) noexcept {
		swap(*this, rhs);
		return *this;
	}
};

/// RAII semaphore wrapper
class Semaphore : public ResourceHandle<vk::Semaphore> {
public:
	Semaphore() = default;
	Semaphore(const Device&);
	Semaphore(const Device&, vk::Semaphore);
	Semaphore(const Device&, const vk::SemaphoreCreateInfo&);
	~Semaphore();

	Semaphore(Semaphore&& rhs) noexcept { swap(*this, rhs); }
	Semaphore& operator=(Semaphore rhs) noexcept {
		swap(*this, rhs);
		return *this;
	}
};

/// RAII event wrapper
class Event : public ResourceHandle<vk::Event> {
public:
	Event() = default;
	Event(const Device&);
	Event(const Device&, vk::Event);
	Event(const Device&, const vk::EventCreateInfo&);
	~Event();

	Event(Event&& rhs) noexcept { swap(*this, rhs); }
	Event& operator=(Event rhs) noexcept {
		swap(*this, rhs);
		return *this;
	}
};

/// RAII Vulkan Renderpass wrapper.
/// Stores no additional information.
class RenderPass : public ResourceHandle<vk::RenderPass> {
public:
	RenderPass() = default;
	RenderPass(const Device& dev, vk::RenderPass pass);
	RenderPass(const Device& dev, const vk::RenderPassCreateInfo&);
	~RenderPass();

	RenderPass(RenderPass&& rhs) noexcept { swap(*this, rhs); }
	RenderPass& operator=(RenderPass&& rhs) noexcept {
		swap(*this, rhs);
		return *this;
	}
};

/// RAII Vulkan Framebuffer wrapper.
/// Stores no additional information.
class Framebuffer : public ResourceHandle<vk::Framebuffer> {
public:
	Framebuffer() = default;
	Framebuffer(const Device&, vk::Framebuffer);
	Framebuffer(const Device&, const vk::FramebufferCreateInfo&);
	~Framebuffer();

	Framebuffer(Framebuffer&& rhs) noexcept { swap(*this, rhs); }
	Framebuffer& operator=(Framebuffer rhs) noexcept {
		swap(*this, rhs);
		return *this;
	}
};

/// RAII Vulkan DescriptorSetLayout wrapper.
class DescriptorSetLayout : public ResourceHandle<vk::DescriptorSetLayout> {
public:
	DescriptorSetLayout() = default;
	DescriptorSetLayout(const Device&, vk::DescriptorSetLayout);
	DescriptorSetLayout(const Device&, const vk::DescriptorSetLayoutCreateInfo&);

	/// Will modify the passed DescriptorSetLayoutBindings, i.e. automatically
	/// numerate them if they have binding == -1, as is the default of
	/// vpp::descriptorBinding
	DescriptorSetLayout(const Device&, nytl::Span<vk::DescriptorSetLayoutBinding>);

	/// Will have to create a temporary copy of the passed bindings, i.e.
	/// allocate memory. Prefer the other constructors if possible.
	DescriptorSetLayout(const Device&,
		std::initializer_list<vk::DescriptorSetLayoutBinding>);
	~DescriptorSetLayout();

	DescriptorSetLayout(DescriptorSetLayout&& rhs) noexcept { swap(*this, rhs); }
	auto& operator=(DescriptorSetLayout rhs) noexcept {
		swap(*this, rhs);
		return *this;
	}
};

/// Vulkan DescriptorSet.
/// Note that this class cannot automatically destroy the DescriptorSet handle
/// on destruction since the vulkan spec does not allow this per default.
/// See also the vpp/trackedDescriptor api for more functionality.
class DescriptorSet : public ResourceHandle<vk::DescriptorSet> {
public:
	DescriptorSet() = default;
	DescriptorSet(const Device&, vk::DescriptorSet);
	DescriptorSet(const DescriptorPool&, const DescriptorSetLayout&);
	DescriptorSet(const DescriptorPool&, vk::DescriptorSetLayout);
	DescriptorSet(vk::DescriptorPool, const DescriptorSetLayout&);
	DescriptorSet(const Device&, vk::DescriptorPool, vk::DescriptorSetLayout);
	~DescriptorSet() = default;

	DescriptorSet(DescriptorSet&& rhs) noexcept { swap(*this, rhs); }
	auto& operator=(DescriptorSet rhs) noexcept {
		swap(*this, rhs);
		return *this;
	}
};

/// RAII vulkan descriptor pool wrapper.
class DescriptorPool : public ResourceHandle<vk::DescriptorPool> {
public:
	DescriptorPool() = default;
	DescriptorPool(const Device&, vk::DescriptorPool);
	DescriptorPool(const Device&, const vk::DescriptorPoolCreateInfo&);
	~DescriptorPool();

	DescriptorPool(DescriptorPool&& rhs) noexcept { swap(*this, rhs); }
	auto& operator=(DescriptorPool rhs) noexcept {
		swap(*this, rhs);
		return *this;
	}
};

/// RAII vulkan pipeline layout wrapper.
class PipelineLayout : public ResourceHandle<vk::PipelineLayout> {
public:
	PipelineLayout() = default;
	PipelineLayout(const Device& dev, vk::PipelineLayout);
	PipelineLayout(const Device& dev, const vk::PipelineLayoutCreateInfo& info);
	PipelineLayout(const Device& dev,
		nytl::Span<const vk::DescriptorSetLayout> layouts,
		nytl::Span<const vk::PushConstantRange> ranges);
	~PipelineLayout();

	PipelineLayout(PipelineLayout&& rhs) noexcept { swap(*this, rhs); }
	auto& operator=(PipelineLayout&& rhs) noexcept {
		swap(*this, rhs);
		return *this;
	}
};

/// RAII Vulkan pipeline wrapper.
class Pipeline : public ResourceHandle<vk::Pipeline> {
public:
	Pipeline() = default;
	Pipeline(const Device& dev, vk::Pipeline pipeline) :
		ResourceHandle(dev, pipeline) {}
	~Pipeline();

	Pipeline(Pipeline&& rhs) noexcept { swap(*this, rhs); }
	auto& operator=(Pipeline&& rhs) noexcept {
		swap(*this, rhs);
		return *this;
	}
};

/// RAII vulkan pipeline cache wrapper.
/// A pipeline cache can be used to decrease pipeline compilation time.
class PipelineCache : public ResourceHandle<vk::PipelineCache> {
public:
	PipelineCache() = default;
	PipelineCache(const Device& dev, vk::PipelineCache);

	/// Creates an empty pipeline cache
	PipelineCache(const Device& dev);

	/// Loads the pipeline cache data from a file.
	/// If except is true and the file cannot be read, throws a
	/// std::runtime_error, otherwise just starts with an empty cache.
	PipelineCache(const Device&, std::string_view file, bool except = false);

	/// Creates the pipeline cache with the given initial data.
	PipelineCache(const Device& dev, nytl::Span<const std::uint8_t> data);
	~PipelineCache();

	PipelineCache(PipelineCache&& rhs) noexcept { swap(*this, rhs); }
	auto& operator=(PipelineCache rhs) noexcept {
		swap(*this, rhs);
		return *this;
	}
};

namespace fwd { extern const vk::CommandBufferLevel primaryCmdBufLevel; }

/// RAII vulkan CommandPool wrapper.
/// CommanPools belong to the thread in which they were created. They as well as
/// any CommandBuffer they allocated must only be used in this thread.
/// The CommandPool object must be destroyed in the thread that created it.
/// See the vulkan spec about command pool synchronization for more information.
class CommandPool : public ResourceHandle<vk::CommandPool> {
public:
	CommandPool() = default;
	CommandPool(const Device& dev, vk::CommandPool);
	CommandPool(const Device& dev, const vk::CommandPoolCreateInfo&);
	CommandPool(const Device& dev, uint32_t qfam,
		vk::CommandPoolCreateFlags flags = {});
	~CommandPool();

	CommandPool(CommandPool&& rhs) noexcept { swap(*this, rhs); }
	auto& operator=(CommandPool&& rhs) noexcept {
		swap(*this, rhs);
		return *this;
	}

	CommandBuffer allocate(vk::CommandBufferLevel = fwd::primaryCmdBufLevel);
	std::vector<CommandBuffer> allocate(unsigned count,
		vk::CommandBufferLevel = fwd::primaryCmdBufLevel);
};

/// RAII vulkan CommandBuffer wrapper.
/// Keeps a reference to the CommandPool it was allocated from (for freeing).
class CommandBuffer : public ResourceHandle<vk::CommandBuffer> {
public:
	CommandBuffer() = default;
	CommandBuffer(const CommandPool&); // primary level
	CommandBuffer(const CommandPool&, vk::CommandBufferLevel);
	CommandBuffer(const CommandPool&, vk::CommandBuffer);
	CommandBuffer(const Device&, vk::CommandPool, vk::CommandBuffer);
	~CommandBuffer();

	CommandBuffer(CommandBuffer&& rhs) noexcept { swap(*this, rhs); }
	CommandBuffer& operator=(CommandBuffer rhs) noexcept {
		swap(*this, rhs);
		return *this;
	}

	const vk::CommandPool& commandPool() const { return commandPool_; }
	friend void swap(CommandBuffer& a, CommandBuffer& b) noexcept;

protected:
	vk::CommandPool commandPool_;
};

/// RAII vk::Image wrapper.
/// Does not handle any additional functionality, see vpp::Image
class ImageHandle : public ResourceHandle<vk::Image> {
public:
	ImageHandle() = default;
	ImageHandle(const Device&, vk::Image);
	ImageHandle(const Device&, const vk::ImageCreateInfo&);
	~ImageHandle();

	ImageHandle(ImageHandle&& rhs) noexcept { swap(*this, rhs); }
	auto& operator=(ImageHandle rhs) noexcept {
		swap(*this, rhs);
		return *this;
	}
};

/// RAII wrapper around a vulkan image view.
class ImageView : public ResourceHandle<vk::ImageView> {
public:
	ImageView() = default;
	ImageView(const Device&, vk::ImageView);
	ImageView(const Device&, const vk::ImageViewCreateInfo&);
	~ImageView();

	ImageView(ImageView&& rhs) noexcept { swap(*this, rhs); }
	ImageView& operator=(ImageView rhs) noexcept {
		swap(*this, rhs);
		return *this;
	}
};

/// RAII vk::Buffer wrapper.
/// Does not handle any additional functionality, see vpp::Buffer
class BufferHandle : public ResourceHandle<vk::Buffer> {
public:
	BufferHandle() = default;
	BufferHandle(const Device&, vk::Buffer);
	BufferHandle(const Device&, const vk::BufferCreateInfo&);
	~BufferHandle();

	BufferHandle(BufferHandle&& rhs) noexcept { swap(*this, rhs); }
	auto& operator=(BufferHandle rhs) noexcept {
		swap(*this, rhs);
		return *this;
	}
};

/// RAII wrapper around a vulkan buffer view.
class BufferView : public ResourceHandle<vk::BufferView> {
public:
	BufferView() = default;
	BufferView(const Device&, vk::BufferView);
	BufferView(const Device&, const vk::BufferViewCreateInfo&);
	~BufferView();

	BufferView(BufferView&& rhs) noexcept { swap(*this, rhs); }
	BufferView& operator=(BufferView rhs) noexcept {
		swap(*this, rhs);
		return *this;
	}
};

/// RAII wrapper around vk::DeviceMemory.
/// Does not carry/manage any additional stuff, see vpp::DeviceMemory.
class DeviceMemoryHandle : public ResourceHandle<vk::DeviceMemory> {
public:
	DeviceMemoryHandle() = default;
	DeviceMemoryHandle(const Device&, vk::DeviceMemory);
	DeviceMemoryHandle(const Device&, const vk::MemoryAllocateInfo&);
	~DeviceMemoryHandle();

	DeviceMemoryHandle(DeviceMemoryHandle&& rhs) { swap(*this, rhs); }
	DeviceMemoryHandle& operator=(DeviceMemoryHandle rhs) {
		swap(*this, rhs);
		return *this;
	}
};

/// RAII wrapper for a vulkan sampler.
class Sampler : public ResourceHandle<vk::Sampler> {
public:
	Sampler() = default;
	Sampler(const Device&, vk::Sampler);
	Sampler(const Device&, const vk::SamplerCreateInfo&);
	~Sampler();

	Sampler(Sampler&& rhs) noexcept { swap(*this, rhs); }
	Sampler& operator=(Sampler rhs) noexcept {
		swap(*this, rhs);
		return *this;
	}
};

} // namespace vpp
