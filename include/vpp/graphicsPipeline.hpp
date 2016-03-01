#pragma once

#include <vpp/vk.hpp>
#include <vpp/fwd.hpp>
#include <vpp/resource.hpp>
#include <vpp/memory.hpp>
#include <vpp/shader.hpp>
#include <vpp/pipeline.hpp>

#include <memory>

namespace vpp
{

struct DescriptorBinding
{
	vk::DescriptorType type;
	vk::ShaderStageFlags stages {vk::ShaderStageFlagBits::All};
	unsigned int count {1};
};

class VertexBufferLayout
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
	std::vector<vk::Format> attributes;
	unsigned int binding {0};
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
	const DescriptorSetLayout* layout_;
	vk::DescriptorSet descriptorSet_ {};

public:
	DescriptorSet(const DescriptorSetLayout& layout, vk::DescriptorPool pool);
	~DescriptorSet();

	vk::DescriptorSet vkDescriptorSet() const { return descriptorSet_; }
	const DescriptorSetLayout& layout() const { return *layout_; }

	void writeImages(std::size_t binding, const std::vector<vk::DescriptorImageInfo>& updates) const;
	void writeBuffers(std::size_t binding, const std::vector<vk::DescriptorBufferInfo>& updates) const;
	void writeBufferViews(std::size_t binding, const std::vector<vk::BufferView>& updates) const;
};

class GraphicsPipeline : public Pipeline
{
public:
	struct StatesCreateInfo
	{
	public:
		vk::PipelineInputAssemblyStateCreateInfo inputAssembly;
		vk::PipelineTessellationStateCreateInfo tessellation;
		vk::PipelineViewportStateCreateInfo viewport;
		vk::PipelineRasterizationStateCreateInfo rasterization;
		vk::PipelineMultisampleStateCreateInfo multisample;
		vk::PipelineDepthStencilStateCreateInfo depthStencil;
		vk::PipelineColorBlendStateCreateInfo colorBlend;

	public:
		//StatesCreateInfo(const std::vector<vk::DynamicState>& dynamic = {});
	};

	struct CreateInfo
	{
		std::vector<DescriptorSetLayout*> descriptorSetLayouts;
		std::vector<VertexBufferLayout*> vertexBufferLayouts;
		std::vector<vk::DynamicState> dynamicStates;

		vk::RenderPass renderPass;
		ShaderProgram shader;

		vk::PipelineCreateFlags flags {};
		StatesCreateInfo states {};
	};

protected:
	GraphicsPipeline() = default;

	void create(const Device& context, const CreateInfo& createInfo);
	void destroy();

public:
	GraphicsPipeline(const Device& device, const CreateInfo& createInfo);
	~GraphicsPipeline();
};

}
