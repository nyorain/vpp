#include <vpp/graphicsPipeline.hpp>
#include <vpp/context.hpp>
#include <vpp/shader.hpp>
#include <vpp/buffer.hpp>

namespace vpp
{

unsigned int formatSize(vk::Format format)
{
	using namespace vk;

	switch(format)
    {
    case Format::Undefined: return 0;
	case Format::R4G4UnormPack8: return 8;
	case Format::R4G4B4A4UnormPack16: return 16;
    case Format::B4G4R4A4UnormPack16: return 16;
    case Format::R5G6B5UnormPack16: return 16;
    case Format::B5G6R5UnormPack16: return 16;
    case Format::R5G5B5A1UnormPack16: return 16;
    case Format::B5G5R5A1UnormPack16: return 16;
    case Format::A1R5G5B5UnormPack16: return 16;
	case Format::R8Unorm: return 8;
    case Format::R8Snorm: return 8;
    case Format::R8Uscaled: return 8;
    case Format::R8Sscaled: return 8;
    case Format::R8Uint: return 8;
    case Format::R8Sint: return 8;
    case Format::R8Srgb: return 8;
	case Format::R8G8Unorm: return 16;
    case Format::R8G8Snorm: return 16;
    case Format::R8G8Uscaled: return 16;
    case Format::R8G8Sscaled: return 16;
    case Format::R8G8Uint: return 16;
    case Format::R8G8Sint: return 16;
    case Format::R8G8Srgb: return 16;
	case Format::R8G8B8Unorm: return 24;
    case Format::R8G8B8Snorm: return 24;
    case Format::R8G8B8Uscaled: return 24;
    case Format::R8G8B8Sscaled: return 24;
    case Format::R8G8B8Uint: return 24;
    case Format::R8G8B8Sint: return 24;
    case Format::R8G8B8Srgb: return 24;
    case Format::B8G8R8Unorm: return 24;
    case Format::B8G8R8Snorm: return 24;
    case Format::B8G8R8Uscaled: return 24;
	case Format::B8G8R8Sscaled: return 24;
    case Format::B8G8R8Uint: return 24;
    case Format::B8G8R8Sint: return 24;
    case Format::B8G8R8Srgb: return 24;
	case Format::R8G8B8A8Unorm: return 32;
    case Format::R8G8B8A8Snorm: return 32;
    case Format::R8G8B8A8Uscaled: return 32;
    case Format::R8G8B8A8Sscaled: return 32;
	case Format::R8G8B8A8Uint: return 32;
    case Format::R8G8B8A8Sint: return 32;
    case Format::R8G8B8A8Srgb: return 32;
    case Format::B8G8R8A8Unorm: return 32;
    case Format::B8G8R8A8Snorm: return 32;
    case Format::B8G8R8A8Uscaled: return 32;
    case Format::B8G8R8A8Sscaled: return 32;
    case Format::B8G8R8A8Uint: return 32;
    case Format::B8G8R8A8Sint: return 32;
    case Format::B8G8R8A8Srgb: return 32;
    case Format::A8B8G8R8UnormPack32: return 32;
    case Format::A8B8G8R8SnormPack32: return 32;
    case Format::A8B8G8R8UscaledPack32: return 32;
    case Format::A8B8G8R8SscaledPack32: return 32;
    case Format::A8B8G8R8UintPack32: return 32;
    case Format::A8B8G8R8SintPack32: return 32;
    case Format::A8B8G8R8SrgbPack32: return 32;
    case Format::A2R10G10B10UnormPack32: return 32;
    case Format::A2R10G10B10SnormPack32: return 32;
    case Format::A2R10G10B10UscaledPack32: return 32;
    case Format::A2R10G10B10SscaledPack32: return 32;
    case Format::A2R10G10B10UintPack32: return 32;
    case Format::A2R10G10B10SintPack32: return 32;
    case Format::A2B10G10R10UnormPack32: return 32;
    case Format::A2B10G10R10SnormPack32: return 32;
    case Format::A2B10G10R10UscaledPack32: return 32;
    case Format::A2B10G10R10SscaledPack32: return 32;
    case Format::A2B10G10R10UintPack32: return 32;
    case Format::A2B10G10R10SintPack32: return 32;
    case Format::R16Unorm: return 16;
    case Format::R16Snorm: return 16;
    case Format::R16Uscaled: return 16;
    case Format::R16Sscaled: return 16;
	case Format::R16Uint: return 16;
    case Format::R16Sint: return 16;
    case Format::R16Sfloat: return 16;
    case Format::R16G16Unorm: return 32;
    case Format::R16G16Snorm: return 32;
    case Format::R16G16Uscaled: return 32;
    case Format::R16G16Sscaled: return 32;
    case Format::R16G16Uint: return 32;
    case Format::R16G16Sint: return 32;
    case Format::R16G16Sfloat: return 32;
	case Format::R16G16B16Unorm: return 48;
    case Format::R16G16B16Snorm: return 48;
    case Format::R16G16B16Uscaled: return 48;
    case Format::R16G16B16Sscaled: return 48;
    case Format::R16G16B16Uint: return 48;
    case Format::R16G16B16Sint: return 48;
    case Format::R16G16B16Sfloat: return 48;
	case Format::R16G16B16A16Unorm: return 64;
    case Format::R16G16B16A16Snorm: return 64;
    case Format::R16G16B16A16Uscaled: return 64;
    case Format::R16G16B16A16Sscaled: return 64;
    case Format::R16G16B16A16Uint: return 64;
    case Format::R16G16B16A16Sint: return 64;
    case Format::R16G16B16A16Sfloat: return 64;
	case Format::R32Uint: return 32;
    case Format::R32Sint: return 32;
    case Format::R32Sfloat: return 32;
    case Format::R32G32Uint: return 64;
    case Format::R32G32Sint: return 64;
    case Format::R32G32Sfloat: return 64;
	case Format::R32G32B32Uint: return 96;
    case Format::R32G32B32Sint: return 96;
    case Format::R32G32B32Sfloat: return 96;
	case Format::R32G32B32A32Uint: return 128;
    case Format::R32G32B32A32Sint: return 128;
    case Format::R32G32B32A32Sfloat: return 128;
    case Format::R64Uint: return 64;
    case Format::R64Sint: return 64;
    case Format::R64Sfloat: return 64;
    case Format::R64G64Uint: return 128;
    case Format::R64G64Sint: return 128;
    case Format::R64G64Sfloat: return 128;
	case Format::R64G64B64Uint: return 192;
    case Format::R64G64B64Sint: return 192;
	case Format::R64G64B64Sfloat: return 192;
    case Format::R64G64B64A64Uint: return 256;
    case Format::R64G64B64A64Sint: return 256;
    case Format::R64G64B64A64Sfloat: return 256;
	case Format::B10G11R11UfloatPack32: return 32;
    case Format::E5B9G9R9UfloatPack32: return 32;
	case Format::D16Unorm: return 16;
    case Format::X8D24UnormPack32: return 32;
    case Format::D32Sfloat: return 32;
	case Format::S8Uint: return 8;
	case Format::D16UnormS8Uint: return 24;
	case Format::D24UnormS8Uint: return 32;
	case Format::D32SfloatS8Uint: return 48;
	/*
    case Format::Bc1RgbUnormBlock: return "c1RgbUnormBlock";
    case Format::Bc1RgbSrgbBlock: return "c1RgbSrgbBlock";
    case Format::Bc1RgbaUnormBlock: return "c1RgbaUnormBlock";
    case Format::Bc1RgbaSrgbBlock: return "c1RgbaSrgbBlock";
    case Format::Bc2UnormBlock: return "c2UnormBlock";
    case Format::Bc2SrgbBlock: return "c2SrgbBlock";
    case Format::Bc3UnormBlock: return "c3UnormBlock";
    case Format::Bc3SrgbBlock: return "c3SrgbBlock";
    case Format::Bc4UnormBlock: return "c4UnormBlock";
    case Format::Bc4SnormBlock: return "c4SnormBlock";
    case Format::Bc5UnormBlock: return "c5UnormBlock";
    case Format::Bc5SnormBlock: return "c5SnormBlock";
    case Format::Bc6HUfloatBlock: return "c6HUfloatBlock";
    case Format::Bc6HSfloatBlock: return "c6HSfloatBlock";
    case Format::Bc7UnormBlock: return "c7UnormBlock";
    case Format::Bc7SrgbBlock: return "c7SrgbBlock";
    case Format::Etc2R8G8B8UnormBlock: return "tc2R8G8B8UnormBlock";
    case Format::Etc2R8G8B8SrgbBlock: return "tc2R8G8B8SrgbBlock";
    case Format::Etc2R8G8B8A1UnormBlock: return "tc2R8G8B8A1UnormBlock";
    case Format::Etc2R8G8B8A1SrgbBlock: return "tc2R8G8B8A1SrgbBlock";
    case Format::Etc2R8G8B8A8UnormBlock: return "tc2R8G8B8A8UnormBlock";
    case Format::Etc2R8G8B8A8SrgbBlock: return "tc2R8G8B8A8SrgbBlock";
    case Format::EacR11UnormBlock: return "acR11UnormBlock";
    case Format::EacR11SnormBlock: return "acR11SnormBlock";
    case Format::EacR11G11UnormBlock: return "acR11G11UnormBlock";
    case Format::EacR11G11SnormBlock: return "acR11G11SnormBlock";
    case Format::Astc4x4UnormBlock: return "stc4x4UnormBlock";
    case Format::Astc4x4SrgbBlock: return "stc4x4SrgbBlock";
    case Format::Astc5x4UnormBlock: return "stc5x4UnormBlock";
    case Format::Astc5x4SrgbBlock: return "stc5x4SrgbBlock";
    case Format::Astc5x5UnormBlock: return "stc5x5UnormBlock";
    case Format::Astc5x5SrgbBlock: return "stc5x5SrgbBlock";
    case Format::Astc6x5UnormBlock: return "stc6x5UnormBlock";
    case Format::Astc6x5SrgbBlock: return "stc6x5SrgbBlock";
    case Format::Astc6x6UnormBlock: return "stc6x6UnormBlock";
    case Format::Astc6x6SrgbBlock: return "stc6x6SrgbBlock";
    case Format::Astc8x5UnormBlock: return "stc8x5UnormBlock";
    case Format::Astc8x5SrgbBlock: return "stc8x5SrgbBlock";
    case Format::Astc8x6UnormBlock: return "stc8x6UnormBlock";
    case Format::Astc8x6SrgbBlock: return "stc8x6SrgbBlock";
    case Format::Astc8x8UnormBlock: return "stc8x8UnormBlock";
    case Format::Astc8x8SrgbBlock: return "stc8x8SrgbBlock";
    case Format::Astc10x5UnormBlock: return "stc10x5UnormBlock";
    case Format::Astc10x5SrgbBlock: return "stc10x5SrgbBlock";
    case Format::Astc10x6UnormBlock: return "stc10x6UnormBlock";
    case Format::Astc10x6SrgbBlock: return "stc10x6SrgbBlock";
    case Format::Astc10x8UnormBlock: return "stc10x8UnormBlock";
    case Format::Astc10x8SrgbBlock: return "stc10x8SrgbBlock";
    case Format::Astc10x10UnormBlock: return "stc10x10UnormBlock";
    case Format::Astc10x10SrgbBlock: return "stc10x10SrgbBlock";
    case Format::Astc12x10UnormBlock: return "stc12x10UnormBlock";
    case Format::Astc12x10SrgbBlock: return "stc12x10SrgbBlock";
    case Format::Astc12x12UnormBlock: return "stc12x12UnormBlock";
    case Format::Astc12x12SrgbBlock: return "stc12x12SrgbBlock";
	*/
	default: return 0;
	}
}

//descriptorSetLayout
DescriptorSetLayout::DescriptorSetLayout(const Device& dev,
	const std::vector<DescriptorBinding>& bindings) : Resource(dev)
{
	std::vector<vk::DescriptorSetLayoutBinding> vkbindings;
	vkbindings.reserve(bindings.size());

	for(auto& binding : bindings)
	{
		vkbindings.emplace_back();
		vkbindings.back().descriptorType(binding.type);
		vkbindings.back().stageFlags(binding.stages);
		vkbindings.back().descriptorCount(binding.count);
		vkbindings.back().pImmutableSamplers(nullptr);
		vkbindings.back().binding(vkbindings.size() - 1);
	}

	vk::DescriptorSetLayoutCreateInfo descriptorLayout;
	descriptorLayout.bindingCount(vkbindings.size());
	descriptorLayout.pBindings(vkbindings.data());

	vk::createDescriptorSetLayout(vkDevice(), &descriptorLayout, nullptr, &layout_);
	bindings_ = bindings;
}
DescriptorSetLayout::~DescriptorSetLayout()
{
	if(vkDescriptorSetLayout())
	{
		vk::destroyDescriptorSetLayout(vkDevice(), layout_, nullptr);
	}

	layout_ = {};
	bindings_.clear();
}

//DescriptorSet
DescriptorSet::DescriptorSet(const DescriptorSetLayout& layout, vk::DescriptorPool pool)
	: Resource(layout.device()), layout_(&layout)
{
	auto vklayout = layout.vkDescriptorSetLayout();

	vk::DescriptorSetAllocateInfo allocInfo;
	allocInfo.descriptorPool(pool);
	allocInfo.descriptorSetCount(1);
	allocInfo.pSetLayouts(&vklayout);

	vk::allocateDescriptorSets(vkDevice(), &allocInfo, &descriptorSet_);
}
DescriptorSet::~DescriptorSet()
{
	//what to do here? check for free descriptorSet?
}

void DescriptorSet::writeImages(std::size_t binding,
	const std::vector<vk::DescriptorImageInfo>& updates) const
{

}
void DescriptorSet::writeBuffers(std::size_t binding,
	const std::vector<vk::DescriptorBufferInfo>& updates) const
{
	vk::WriteDescriptorSet writeDescriptorSet;
	writeDescriptorSet.dstSet(descriptorSet_);
	writeDescriptorSet.descriptorCount(updates.size());
	writeDescriptorSet.descriptorType(vk::DescriptorType::UniformBuffer);
	writeDescriptorSet.pBufferInfo(updates.data());
	writeDescriptorSet.dstBinding(binding);

	vk::updateDescriptorSets(vkDevice(), 1, &writeDescriptorSet, 0, nullptr);
}
void DescriptorSet::writeBufferViews(std::size_t binding,
	const std::vector<vk::BufferView>& updates) const
{

}

//pipeline
GraphicsPipeline::GraphicsPipeline(const Device& device, const CreateInfo& createInfo)
{
	create(device, createInfo);
}

GraphicsPipeline::~GraphicsPipeline()
{
	destroy();
}

static unsigned int bindID = 0;

void GraphicsPipeline::create(const Device& device, const CreateInfo& createInfo)
{
	destroy();
	Pipeline::create(device);

	//vertexInfo
	vk::PipelineVertexInputStateCreateInfo vertexInfo;

	//Binding description
	std::size_t attributeCount = 0;
	for(auto& layout : createInfo.vertexBufferLayouts)
		attributeCount += layout->attributes.size();

	std::vector<vk::VertexInputBindingDescription> bindingDescriptions;
	bindingDescriptions.reserve(createInfo.vertexBufferLayouts.size());

	std::vector<vk::VertexInputAttributeDescription> attributeDescriptions;
	attributeDescriptions.reserve(attributeCount);

	for(auto& layout : createInfo.vertexBufferLayouts)
	{
		std::size_t location = 0;
		std::size_t offset = 0;

		for(auto& attribute : layout->attributes)
		{
			attributeDescriptions.emplace_back();
			attributeDescriptions.back().location(location++);
			attributeDescriptions.back().binding(layout->binding);
			attributeDescriptions.back().format(attribute);
			attributeDescriptions.back().offset(offset);
			offset += formatSize(attribute) / 8;
		}

		bindingDescriptions.emplace_back();
		bindingDescriptions.back().binding(layout->binding);
		bindingDescriptions.back().stride(offset);
		bindingDescriptions.back().inputRate(vk::VertexInputRate::Vertex);
	}

	vertexInfo.vertexBindingDescriptionCount(bindingDescriptions.size());
	vertexInfo.pVertexBindingDescriptions(bindingDescriptions.data());
	vertexInfo.vertexAttributeDescriptionCount(attributeDescriptions.size());
	vertexInfo.pVertexAttributeDescriptions(attributeDescriptions.data());

	//pipeline layout
	std::vector<vk::DescriptorSetLayout> descriptorSetLayouts;
	descriptorSetLayouts.reserve(createInfo.descriptorSetLayouts.size());

	for(auto& layout : createInfo.descriptorSetLayouts)
		descriptorSetLayouts.push_back(layout->vkDescriptorSetLayout());

	vk::PipelineLayoutCreateInfo pipelineLayoutInfo;
	pipelineLayoutInfo.setLayoutCount(descriptorSetLayouts.size());
	pipelineLayoutInfo.pSetLayouts(descriptorSetLayouts.data());

	vk::createPipelineLayout(vkDevice(), &pipelineLayoutInfo, nullptr, &pipelineLayout_);

	//dynamic state
	vk::PipelineDynamicStateCreateInfo dynamicState;
	dynamicState.pDynamicStates(createInfo.dynamicStates.data());
	dynamicState.dynamicStateCount(createInfo.dynamicStates.size());

	//create it
	vk::GraphicsPipelineCreateInfo pipelineInfo;

	pipelineInfo.layout(pipelineLayout_);
	pipelineInfo.pVertexInputState(&vertexInfo);
	pipelineInfo.pDynamicState(&dynamicState);
	pipelineInfo.renderPass(createInfo.renderPass);
	pipelineInfo.stageCount(createInfo.shader.stages().size());
	pipelineInfo.pStages(createInfo.shader.stages().data());
	pipelineInfo.pInputAssemblyState(&createInfo.states.inputAssembly);
	pipelineInfo.pRasterizationState(&createInfo.states.rasterization);
	pipelineInfo.pColorBlendState(&createInfo.states.colorBlend);
	pipelineInfo.pMultisampleState(&createInfo.states.multisample);
	pipelineInfo.pViewportState(&createInfo.states.viewport);
	pipelineInfo.pDepthStencilState(&createInfo.states.depthStencil);
	pipelineInfo.pTessellationState(nullptr);

	vk::createGraphicsPipelines(vkDevice(), 0, 1, &pipelineInfo, nullptr, &pipeline_);
}

void GraphicsPipeline::destroy()
{
	if(pipeline_) vk::destroyPipeline(vkDevice(), pipeline_, nullptr);
	if(pipelineLayout_) vk::destroyPipelineLayout(vkDevice(), pipelineLayout_, nullptr);

	pipeline_ = {};
	pipelineLayout_ = {};
}

/*
void GraphicsPipeline::drawCommands(vk::CommandBuffer cmdBuffer,
	const std::vector<Buffer*>& vertices, const std::vector<DescriptorSet*>& descriptors) const
{
	if(!descriptors.empty())
	{
		std::vector<vk::DescriptorSet> descs;
		descs.reserve(descriptors.size());
		for(auto& des : descriptors) descs.push_back(des->vkDescriptorSet());

		vk::cmdBindDescriptorSets(cmdBuffer, vk::PipelineBindPoint::Graphics, pipelineLayout_,
			0, descs.size(), descs.data(), 0, nullptr);
	}

	vk::cmdBindPipeline(cmdBuffer, vk::PipelineBindPoint::Graphics, pipeline_);

	if(!vertices.empty())
	{
		vk::DeviceSize offsets[1] {0};

		std::vector<vk::Buffer> buffers;
		buffers.reserve(vertices.size());
		for(auto& vert : vertices) buffers.push_back(vert->vkBuffer());

		vk::cmdBindVertexBuffers(cmdBuffer, 0, buffers.size(), buffers.data(), offsets);
	}

	vk::cmdDraw(cmdBuffer, 3, 1, 0, 0);
};
*/

}
