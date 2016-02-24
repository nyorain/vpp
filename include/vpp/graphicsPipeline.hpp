#pragma once

#include <vpp/vk.hpp>
#include <vpp/fwd.hpp>
#include <vpp/resource.hpp>
#include <vpp/memory.hpp>

#include <memory>

namespace vpp
{

struct DescriptorBinding
{
	vk::DescriptorType type;
	vk::ShaderStageFlags stages;
};

struct VertexBufferLayout
{
public:
	static VertexBufferLayout Pos4fColor4f;
	static VertexBufferLayout Pos3fColor4f;
	static VertexBufferLayout Pos2fColor4f;
	static VertexBufferLayout Pos4fColor3f;
	static VertexBufferLayout Pos3fColor3f;
	static VertexBufferLayout Pos2fColor3f;
	static VertexBufferLayout Pos4fUV2f;
	static VertexBufferLayout Pos3fUV2f;
	static VertexBufferLayout Pos2fUV2f;
	static VertexBufferLayout Pos4f;
	static VertexBufferLayout Pos3f;
	static VertexBufferLayout Pos2f;

public:
	unsigned int binding {0};
	std::vector<vk::Format> attributes;
};

class Buffer : public Resource
{
protected:
	vk::Buffer buffer_ {};
	Memory::Entry memoryEntry_;

public:
	Buffer(const Device& dev, const vk::BufferCreateInfo& info, vk::MemoryPropertyFlags mflags);
	Buffer(MemoryAllocator& allctr, const vk::BufferCreateInfo& info, vk::MemoryPropertyFlags mflags);
	~Buffer();

	const Memory::Entry& memoryEntry() const { return memoryEntry_; }
	vk::Buffer vkBuffer() const { return buffer_; }
	MemoryMap memoryMap() const;
};

class Image : public Resource
{
protected:
	vk::Image image_ {};
	Memory::Entry memoryEntry_;

public:
	Image(const Device& dev, const vk::ImageCreateInfo& info, vk::MemoryPropertyFlags mflags);
	Image(MemoryAllocator& allctr, const vk::ImageCreateInfo& info, vk::MemoryPropertyFlags mflags);
	~Image();

	const Memory::Entry& memoryEntry() const { return memoryEntry_; }
	vk::Image vkImage() const { return image; }
	MemoryMap memoryMap() const;
};

class DescriptorSetLayout : public Resource
{
protected:
	std::vector<DescriptorBinding> bindings_;
	vk::DescriptorSetLayout layout_;

public:
	DescriptorSetLayout(const Device& dev, const std::vector<DescriptorBinding>& bindings);
	~DescriptorSetLayout();

	vk::DescriptorSetLayout vkDescriptorSetLayout() const { return layout_; }
	const std::vector<DescriptorBinding> bindings() const { return bindings_; }
};

class DescriptorSet : public Resource
{
protected:
	DescriptorSetLayout& layout_;
	vk::DescriptorSet descriptorSet_ {};

public:
	DescriptorSet(const Device& dev, const DescriptorSetLayout& layout, vk::DescriptorPool pool);
	~DescriptorSet();

	void writeImages(std::size_t binding, const std::vector<vk::DescriptorImageInfo>& updates) const;
	void writeBuffers(std::size_t binding, const std::vector<vk::DescriptorBufferInfo>& updates) const;
	void writeBufferViews(std::size_t binding, const std::vector<vk::BufferView>& updates) const;
};

class GraphicsPipeline : public Resource
{
public:
	struct StatesCreateInfo
	{
	public:
		vk::PipelineInputAssemblyStateCreateInfo inputAssemblyState;
		vk::PipelineTessellationStateCreateInfo tessellationState;
		vk::PipelineViewportStateCreateInfo viewportState;
		vk::PipelineRasterizationStateCreateInfo rasterizationState;
		vk::PipelineMultisampleStateCreateInfo multisampleState;
		vk::PipelineDepthStencilStateCreateInfo depthStencilState;
		vk::PipelineColorBlendStateCreateInfo colorBlendState;
		std::vector<vk::DynamicState> dynamicStates;

	public:
		StatesCreateInfo(const std::vector<vk::DynamicState>& dynamic = {});
	};

	struct CreateInfo
	{
		std::vector<DescriptorSetLayout> descriptorSetLayouts;
		std::vector<VertexBufferLayout> vertexBufferLayouts;
		vk::RenderPass renderPass;
		ShaderProgram shader;

		vk::PipelineCreateFlags flags {};
		StatesCreateInfo states {};
	};

protected:
	vk::PipelineLayout pipelineLayout_ {};
	vk::Pipeline pipeline_ {};

protected:
	GraphicsPipeline() = default;

	void create(const Device& context, const CreateInfo& createInfo);
	void destroy();

public:
	GraphicsPipeline(const Device& context, const CreateInfo& createInfo);
	~GraphicsPipeline();

	void drawCommands(vk::CommandBuffer cmdBuffer, const std::vector<VertexBuffer>& vertices,
		const std::vector<DescriptorSet>& descriptors) const;
};

}
