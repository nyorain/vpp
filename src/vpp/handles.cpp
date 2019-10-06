// Copyright (c) 2016-2019 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#include <vpp/handles.hpp>
#include <vpp/vk.hpp>
#include <vpp/util/file.hpp>
#include <dlg/dlg.hpp>

namespace vpp {

// Instance
Instance::Instance(const vk::InstanceCreateInfo& info) {
	instance_ = vk::createInstance(info);

	// init vkpp dynamic dispatch
	// required for all used extensions inside vkpp
	vk::dispatch.init(instance_);
}

Instance::Instance(vk::Instance instance) : instance_(instance) {
}

Instance::~Instance() {
	if(instance_) {
		vk::destroyInstance(instance_);
	}
}

Instance::Instance(Instance&& lhs) noexcept : instance_(lhs.instance_) {
	lhs.instance_ = {};
}

Instance& Instance::operator=(Instance&& lhs) noexcept {
	if(instance_) {
		vk::destroyInstance(instance_);
	}

	instance_ = lhs.instance_;
	lhs.instance_ = {};
	return *this;
}

// DescriptorSetLayout
DescriptorSetLayout::DescriptorSetLayout(const Device& dev,
	std::initializer_list<vk::DescriptorSetLayoutBinding> bindings) :
		ResourceHandle(dev) {
	std::vector<vk::DescriptorSetLayoutBinding> mutCopy(
		bindings.begin(), bindings.end());
	*this = {dev, mutCopy};
}

DescriptorSetLayout::DescriptorSetLayout(const Device& dev,
	nytl::Span<vk::DescriptorSetLayoutBinding> bindings) :
		ResourceHandle(dev) {
	static constexpr auto defaultBinding = std::uint32_t(-1);
	unsigned int highestBinding = 0u;
	for(auto& binding : bindings) {
		auto& bid = binding.binding;
		if(bid == defaultBinding) {
			bid = highestBinding++;
		} else {
			highestBinding = std::max(highestBinding, bid + 1);
		}
	}

	vk::DescriptorSetLayoutCreateInfo descriptorLayout;
	descriptorLayout.bindingCount = bindings.size();
	descriptorLayout.pBindings = bindings.data();
	handle_ = vk::createDescriptorSetLayout(vkDevice(), descriptorLayout);
}

DescriptorSetLayout::DescriptorSetLayout(const Device& dev,
	const vk::DescriptorSetLayoutCreateInfo& info) :
		ResourceHandle(dev, vk::createDescriptorSetLayout(vkDevice(), info)) {
}

DescriptorSetLayout::DescriptorSetLayout(const Device& dev,
	vk::DescriptorSetLayout layout) : ResourceHandle(dev, layout) {
}

DescriptorSetLayout::~DescriptorSetLayout() {
	if(vkHandle()) {
		vk::destroyDescriptorSetLayout(vkDevice(), vkHandle());
	}
}

// DescriptorSet
DescriptorSet::DescriptorSet(const DescriptorPool& p,
		const DescriptorSetLayout& l) : DescriptorSet(p, l.vkHandle()) {
}

DescriptorSet::DescriptorSet(const DescriptorPool& pool,
	vk::DescriptorSetLayout layout) :
	DescriptorSet(pool.device(), pool, layout) {
}

DescriptorSet::DescriptorSet(vk::DescriptorPool pool,
	const DescriptorSetLayout& layout) :
	DescriptorSet(layout.device(), pool, layout) {
}

DescriptorSet::DescriptorSet(const Device& dev, vk::DescriptorPool pool,
		vk::DescriptorSetLayout layout) : ResourceHandle(dev) {

	vk::DescriptorSetAllocateInfo allocInfo;
	allocInfo.descriptorPool = pool;
	allocInfo.descriptorSetCount = 1;
	allocInfo.pSetLayouts = &layout;
	vk::allocateDescriptorSets(vkDevice(), allocInfo, handle_);
}

DescriptorSet::DescriptorSet(const Device& dev, vk::DescriptorSet set)
	: ResourceHandle(dev, set) {
}

// DescriptorPool
DescriptorPool::DescriptorPool(const Device& dev,
		const vk::DescriptorPoolCreateInfo& info) : ResourceHandle(dev) {
	handle_ = vk::createDescriptorPool(dev, info);
}

DescriptorPool::DescriptorPool(const Device& dev, vk::DescriptorPool pool)
	: ResourceHandle(dev, pool) {
}

DescriptorPool::~DescriptorPool() {
	if(vkHandle()) {
		vk::destroyDescriptorPool(device(), vkHandle());
	}
}

// PipelineLayout
PipelineLayout::PipelineLayout(const Device& dev, vk::PipelineLayout layout) :
	ResourceHandle(dev, layout) {
}

PipelineLayout::PipelineLayout(const Device& dev,
		const vk::PipelineLayoutCreateInfo& info) : ResourceHandle(dev) {
	handle_ = vk::createPipelineLayout(dev, info);
}

PipelineLayout::PipelineLayout(const Device& dev,
		nytl::Span<const vk::DescriptorSetLayout> layouts,
		nytl::Span<const vk::PushConstantRange> ranges) : ResourceHandle(dev) {
	vk::PipelineLayoutCreateInfo info;
	info.setLayoutCount = layouts.size();
	info.pSetLayouts = layouts.data();
	info.pushConstantRangeCount = ranges.size();
	info.pPushConstantRanges = ranges.data();

	handle_ = vk::createPipelineLayout(dev, info);
}

PipelineLayout::~PipelineLayout() {
	if(vkHandle()) {
		vk::destroyPipelineLayout(device(), vkHandle());
	}
}

// PipelineCache
PipelineCache::PipelineCache(const Device& dev, vk::PipelineCache cache) :
	ResourceHandle(dev, cache) {
}

PipelineCache::PipelineCache(const Device& dev) : ResourceHandle(dev) {
	handle_ = vk::createPipelineCache(dev, {});
}

PipelineCache::PipelineCache(const Device& dev,
		nytl::Span<const std::uint8_t> data) : ResourceHandle(dev) {
	auto size = std::size_t(data.size());
	handle_ = vk::createPipelineCache(dev, {{}, size, data.data()});
}

PipelineCache::PipelineCache(const Device& dev, std::string_view filename,
		bool except) : ResourceHandle(dev) {

	std::vector<std::byte> data;
	try {
		data = readFile(filename);
	} catch(const std::exception& err) {
		dlg_info("PipelineCache: {}", err.what());
		if(except) {
			throw;
		}
	}

	handle_ = vk::createPipelineCache(dev, {{}, data.size(), data.data()});
}

PipelineCache::~PipelineCache() {
	if(vkHandle()) {
		vk::destroyPipelineCache(device(), vkHandle());
	}
}

bool save(vk::Device dev, vk::PipelineCache cache, std::string_view file) {
	auto data = vk::getPipelineCacheData(dev, cache);
	auto ptr = reinterpret_cast<const std::byte*>(data.data());

	try {
		writeFile(file, {ptr, ptr + data.size()});
	} catch(const std::exception& err) {
		dlg_warn("vpp::save(PipelineCache): {}", err.what());
		return false;
	}

	return true;
}

// Pipeline
Pipeline::Pipeline(const Device& dev,
	const vk::GraphicsPipelineCreateInfo& info, vk::PipelineCache cache) :
		ResourceHandle(dev) {
	vk::Pipeline pipe;
	vk::createGraphicsPipelines(dev, cache, 1, info, nullptr, pipe);
	handle_ = pipe;
}

Pipeline::Pipeline(const Device& dev,
	const vk::ComputePipelineCreateInfo& info, vk::PipelineCache cache) :
		ResourceHandle(dev) {
	vk::Pipeline pipe;
	vk::createComputePipelines(dev, cache, 1, info, nullptr, pipe);
	handle_ = pipe;
}

Pipeline::~Pipeline() {
	if(vkHandle()) {
		vk::destroyPipeline(device(), vkHandle());
	}
}

// Framebuffer
Framebuffer::Framebuffer(const Device& dev, vk::Framebuffer framebuffer)
	: ResourceHandle(dev, framebuffer) {
}

Framebuffer::Framebuffer(const Device& dev, const vk::FramebufferCreateInfo& i)
	: ResourceHandle(dev, vk::createFramebuffer(dev, i)) {
}

Framebuffer::~Framebuffer() {
	if(vkHandle()) {
		vk::destroyFramebuffer(vkDevice(), vkHandle(), nullptr);
	}
}

// QueryPool
QueryPool::QueryPool(const Device& dev, const vk::QueryPoolCreateInfo& info) :
	ResourceHandle(dev, vk::createQueryPool(dev, info)) {
}

QueryPool::QueryPool(const Device& dev, vk::QueryPool pool) :
	ResourceHandle(dev, pool) {
}

QueryPool::~QueryPool() {
	if(vkHandle()) {
		vk::destroyQueryPool(device(), vkHandle());
	}
}

// RenderPass
RenderPass::RenderPass(const Device& dev, const vk::RenderPassCreateInfo& info)
	: RenderPass(dev, vk::createRenderPass(dev, info)) {
}

RenderPass::RenderPass(const Device& dev, vk::RenderPass pass)
	: ResourceHandle(dev, pass) {
}

RenderPass::~RenderPass() {
	if(vkHandle()) {
		vk::destroyRenderPass(vkDevice(), vkHandle());
	}
}

// CommandPool
CommandPool::CommandPool(const Device& dev, vk::CommandPool pool) :
	ResourceHandle(dev, pool) {
}

CommandPool::CommandPool(const Device& dev,
	const vk::CommandPoolCreateInfo& info) :
		ResourceHandle(dev, vk::createCommandPool(dev, info)) {
}

CommandPool::CommandPool(const Device& dev, uint32_t qfam,
	vk::CommandPoolCreateFlags flags) : CommandPool(dev, {flags, qfam}) {
}

CommandPool::~CommandPool() {
	if(vkHandle()) {
		vk::destroyCommandPool(vkDevice(), vkHandle(), nullptr);
	}
}

CommandBuffer CommandPool::allocate(vk::CommandBufferLevel lvl) {
	vk::CommandBufferAllocateInfo info;
	info.commandPool = vkHandle();
	info.level = lvl;
	info.commandBufferCount = 1;

	vk::CommandBuffer buffer;
	vk::allocateCommandBuffers(vkDevice(), info, buffer);
	return {*this, buffer};
}

std::vector<CommandBuffer> CommandPool::allocate(unsigned count,
		vk::CommandBufferLevel lvl) {
	vk::CommandBufferAllocateInfo info;
	info.commandPool = vkHandle();
	info.level = lvl;
	info.commandBufferCount = 1;

	std::vector<vk::CommandBuffer> buffers(count);
	vk::allocateCommandBuffers(device(), info, *buffers.data());

	std::vector<CommandBuffer> ret;
	ret.reserve(count);

	for(auto& buf : buffers) {
		ret.emplace_back(*this, buf);
	}

	return ret;
}

// CommandBuffer
CommandBuffer::CommandBuffer(const CommandPool& pool)
	: CommandBuffer(pool, vk::CommandBufferLevel::primary) {
}

CommandBuffer::CommandBuffer(const CommandPool& pool, vk::CommandBufferLevel lvl)
	: ResourceHandle(pool.device()), commandPool_(pool) {
	vk::allocateCommandBuffers(pool.device(), {pool, lvl, 1u}, handle_);
}

CommandBuffer::CommandBuffer(const CommandPool& pool, vk::CommandBuffer buf)
	: ResourceHandle(pool.device(), buf), commandPool_(pool) {
}

CommandBuffer::CommandBuffer(const Device& dev,
	vk::CommandPool pool, vk::CommandBuffer buf)
		: ResourceHandle(dev, buf), commandPool_(pool) {
}

CommandBuffer::~CommandBuffer() {
	if(vkHandle()) {
		vk::freeCommandBuffers(vkDevice(), commandPool(), {{vkHandle()}});
	}
}

void swap(CommandBuffer& a, CommandBuffer& b) noexcept {
	using std::swap;
	using RH = ResourceHandle<vk::CommandBuffer>;
	swap(static_cast<RH&>(a), static_cast<RH&>(b));
	swap(a.commandPool_, b.commandPool_);
}

// Surface
Surface::Surface(vk::Instance instance, vk::SurfaceKHR surface)
	: instance_(instance), surface_(surface) {
}

Surface::~Surface() {
	if(vkHandle()) {
		vk::destroySurfaceKHR(vkInstance(), vkHandle(), nullptr);
	}
}

void swap(Surface& a, Surface& b) noexcept {
	using std::swap;
	swap(a.instance_, b.instance_);
	swap(a.surface_, b.surface_);
}

// Fence
Fence::Fence(const Device& dev) : Fence(dev, vk::FenceCreateInfo {}) {}
Fence::Fence(const Device& dev, vk::Fence fence) : ResourceHandle(dev, fence) {}
Fence::Fence(const Device& dev, const vk::FenceCreateInfo& info)
		: ResourceHandle(dev) {
	handle_ = vk::createFence(device(), info);
}

Fence::~Fence() {
	if(vkHandle()) {
		vk::destroyFence(device(), vkHandle());
	}
}

// Semaphore
Semaphore::Semaphore(const Device& dev) : Semaphore(dev, vk::SemaphoreCreateInfo {}) {}
Semaphore::Semaphore(const Device& dev, vk::Semaphore sp) : ResourceHandle(dev, sp) {}
Semaphore::Semaphore(const Device& dev, const vk::SemaphoreCreateInfo& info)
		: ResourceHandle(dev) {
	handle_ = vk::createSemaphore(device(), info);
}

Semaphore::~Semaphore() {
	if(vkHandle()) {
		vk::destroySemaphore(device(), vkHandle());
	}
}

// Event
Event::Event(const Device& dev) : Event(dev, vk::EventCreateInfo {}) {}
Event::Event(const Device& dev, vk::Event event) : ResourceHandle(dev, event) {}
Event::Event(const Device& dev, const vk::EventCreateInfo& info)
		: ResourceHandle(dev) {
	handle_ = vk::createEvent(device(), info);
}

Event::~Event() {
	if(vkHandle()) {
		vk::destroyEvent(device(), vkHandle());
	}
}

// ImageHandle
ImageHandle::ImageHandle(const Device& dev, const vk::ImageCreateInfo& info) :
	ResourceHandle(dev, vk::createImage(dev, info)) {
}

ImageHandle::ImageHandle(const Device& dev, vk::Image buf) :
	ResourceHandle(dev, buf) {
}

ImageHandle::~ImageHandle() {
	if(vkHandle()) {
		vk::destroyImage(device(), vkHandle());
	}
}

// ImageView
ImageView::ImageView(const Device& dev, const vk::ImageViewCreateInfo& info) :
	ImageView(dev, vk::createImageView(dev, info)) {
}

ImageView::ImageView(const Device& dev, vk::ImageView view) :
	ResourceHandle(dev, view) {
}

ImageView::~ImageView() {
	if(vkHandle()) {
		vk::destroyImageView(device(), vkHandle());
	}
}

// BufferHandle
BufferHandle::BufferHandle(const Device& dev, const vk::BufferCreateInfo& info) :
		ResourceHandle(dev, vk::createBuffer(dev, info)) {
}

BufferHandle::BufferHandle(const Device& dev, vk::Buffer buf) :
		ResourceHandle(dev, buf) {
}

BufferHandle::~BufferHandle() {
	if(vkHandle()) {
		vk::destroyBuffer(device(), vkHandle());
	}
}

// BufferView
BufferView::BufferView(const Device& dev, const vk::BufferViewCreateInfo& info) :
		BufferView(dev, vk::createBufferView(dev, info)) {
}

BufferView::BufferView(const Device& dev, vk::BufferView view) :
		ResourceHandle(dev, view) {
}

BufferView::~BufferView() {
	if(vkHandle()) {
		vk::destroyBufferView(device(), vkHandle());
	}
}

// DeviceMemoryHandle
DeviceMemoryHandle::DeviceMemoryHandle(const Device& dev,
	const vk::MemoryAllocateInfo& info)
		: DeviceMemoryHandle(dev, vk::allocateMemory(dev, info)) {
}

DeviceMemoryHandle::DeviceMemoryHandle(const Device& dev, vk::DeviceMemory mem)
	: ResourceHandle(dev, mem) {
}

DeviceMemoryHandle::~DeviceMemoryHandle() {
	if(vkHandle()) {
		vk::freeMemory(vkDevice(), vkHandle());
	}
}

// Sampler
Sampler::Sampler(const Device& dev, const vk::SamplerCreateInfo& info) :
	Sampler(dev, vk::createSampler(dev, info)) {
}

Sampler::Sampler(const Device& dev, vk::Sampler sampler) :
	ResourceHandle(dev, sampler) {
}

Sampler::~Sampler() {
	if(vkHandle()) {
		vk::destroySampler(device(), vkHandle());
	}
}

} // namespace vpp
