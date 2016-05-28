#pragma once

#include <vpp/fwd.hpp>
#include <vpp/resource.hpp>
#include <vpp/shader.hpp>

namespace vpp
{

namespace fwd { vk::ShaderStageFlags allShaderStages; }

unsigned int formatSize(vk::Format format);

struct DescriptorBinding
{
	vk::DescriptorType type;
	vk::ShaderStageFlags stages { fwd::allShaderStages };
	unsigned int count {1};
};

struct VertexBufferLayout
{
	std::vector<vk::Format> attributes;
	unsigned int binding {0};
};

///Represents a vulkan desciptor set layout which stores information about the structure of a
///descriptor set. Needed to create a descriptor set. This class will internally store the
///given information about a descriptor set so that they can be retrieved later.
class DescriptorSetLayout : public Resource
{
public:
	DescriptorSetLayout() = default;
	DescriptorSetLayout(const Device& dev, const std::vector<DescriptorBinding>& bindings);
	~DescriptorSetLayout();

	DescriptorSetLayout(DescriptorSetLayout&& other) noexcept;
	DescriptorSetLayout& operator=(DescriptorSetLayout&& other) noexcept;

	void swap(DescriptorSetLayout& other) noexcept;
	void destroy();

	vk::DescriptorSetLayout vkDescriptorSetLayout() const { return layout_; }
	const std::vector<DescriptorBinding> bindings() const { return bindings_; }

protected:
	std::vector<DescriptorBinding> bindings_;
	vk::DescriptorSetLayout layout_;
};

///Represents a vulkan descriptor set.
class DescriptorSet : public Resource
{
public:
	DescriptorSet() = default;
	DescriptorSet(const DescriptorSetLayout& layout, vk::DescriptorPool pool);
	~DescriptorSet();

	DescriptorSet(DescriptorSet&& other) noexcept;
	DescriptorSet& operator=(DescriptorSet&& other) noexcept;

	void swap(DescriptorSet& other) noexcept;

	vk::DescriptorSet vkDescriptorSet() const { return descriptorSet_; }
	const DescriptorSetLayout& layout() const { return *layout_; }

	///Updates the descriptorSet with the given writes and copies.
	void update(const std::vector<vk::WriteDescriptorSet>& writes,
		const std::vector<vk::CopyDescriptorSet>& copies = {}) const;

	///Updates the descriptorSet with the given copies.
	void update(const std::vector<vk::CopyDescriptorSet>& copies) const;

protected:
	const DescriptorSetLayout* layout_;
	vk::DescriptorSet descriptorSet_ {};
};

///Pipeline base class.
class Pipeline : public Resource
{
public:
	~Pipeline();

	void swap(Pipeline& other) noexcept;

	using Resource::init;
	void destroy();

	vk::Pipeline vkPipeline() const { return pipeline_; }
	vk::PipelineLayout vkPipelineLayout() const { return pipelineLayout_; }

protected:
	vk::PipelineLayout pipelineLayout_ {};
	vk::Pipeline pipeline_ {};

protected:
	Pipeline() = default;
	Pipeline(const Device& dev);

	Pipeline(Pipeline&& other) noexcept;
	Pipeline& operator=(Pipeline&& other) noexcept;
};

}
