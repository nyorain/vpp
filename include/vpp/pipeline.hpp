#pragma once

#include <vpp/vk.hpp>
#include <vpp/fwd.hpp>
#include <vpp/resource.hpp>
#include <vpp/shader.hpp>

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

class Pipeline : public Resource
{
public:
	enum class Type
	{
		graphic,
		compute
	};

protected:
	vk::PipelineLayout pipelineLayout_ {};
	vk::Pipeline pipeline_ {};

protected:
	Pipeline() = default;
	using Resource::create;

public:
	Pipeline(const Device& dev);
	~Pipeline() = default;

	vk::Pipeline vkPipeline() const { return pipeline_; }
	vk::PipelineLayout vkPipelineLayout() const { return pipelineLayout_; }

	void drawCommands(vk::CommandBuffer cmdBuffer, const std::vector<Buffer*>& vertices,
		const std::vector<DescriptorSet*>& descriptors) const;
};

}
