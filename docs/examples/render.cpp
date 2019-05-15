#include "render.hpp"

#include <vpp/shader.hpp>
#include <vpp/pipeline.hpp>
#include <vpp/debug.hpp>

#include "data/intro.frag.h"
#include "data/intro.vert.h"

// initializing the rendering resources
vpp::Pipeline createGraphicsPipeline(const vpp::Device&, vk::RenderPass,
	vk::PipelineLayout);

// MyRenderer
MyRenderer::MyRenderer(vk::RenderPass rp, vk::SwapchainCreateInfoKHR& scInfo,
		const vpp::Queue& present) : vpp::DefaultRenderer(), scInfo_(scInfo) {
	// pipeline
	pipelineLayout_ = {present.device(), vk::PipelineLayoutCreateInfo {}};
	vpp::nameHandle(pipelineLayout_, "MyRenderer:pipelineLayout");

	pipeline_ = createGraphicsPipeline(present.device(), rp, pipelineLayout_);
	vpp::nameHandle(pipeline_, "MyRenderer:pipeline");

	init(rp, scInfo, present);
	vpp::nameHandle(swapchain(), "MyRenderer:swapchain");
}

void MyRenderer::resize(const vk::Extent2D& size) {
	vpp::DefaultRenderer::recreate(size, scInfo_);
}

void MyRenderer::record(const RenderBuffer& buf) {
	static const auto clearValue = vk::ClearValue {{0.f, 0.f, 0.f, 1.f}};
	const auto width = scInfo_.imageExtent.width;
	const auto height = scInfo_.imageExtent.height;

	auto cmdBuf = buf.commandBuffer;

	vk::beginCommandBuffer(cmdBuf, {});
	vk::cmdBeginRenderPass(cmdBuf, {
		renderPass(),
		buf.framebuffer,
		{0u, 0u, width, height},
		1,
		&clearValue
	}, {});

	vk::Viewport vp {0.f, 0.f, (float) width, (float) height, 0.f, 1.f};
	vk::cmdSetViewport(cmdBuf, 0, 1, vp);
	vk::cmdSetScissor(cmdBuf, 0, 1, {0, 0, width, height});

#ifdef RENDERDOC
	vpp::insertDebugMarker(device(), cmdBuf, "finish setup");
	vpp::beginDebugRegion(device(), cmdBuf, "render triangle", {1, 0.5, 0.5, 1});
#endif

	vk::cmdBindPipeline(cmdBuf, vk::PipelineBindPoint::graphics, pipeline_);
	vk::cmdDraw(cmdBuf, 3, 1, 0, 0);

#ifdef RENDERDOC
	vpp::endDebugRegion(device(), cmdBuf);
#endif

	vk::cmdEndRenderPass(cmdBuf);
	vk::endCommandBuffer(cmdBuf);
}

// resources
vpp::Pipeline createGraphicsPipeline(const vpp::Device& dev, vk::RenderPass rp,
		vk::PipelineLayout layout) {
	// first load the shader modules and create the shader program for our pipeline
	// if the shaders cannot be found/compiled, this will throw (and end the application)
	vpp::ShaderModule vertexShader(dev, intro_vert_spv_data);
	vpp::ShaderModule fragmentShader(dev, intro_frag_spv_data);

	vpp::nameHandle(vertexShader, "triangleVertexShader");
	vpp::nameHandle(fragmentShader, "triangleFragmentShader");

	vpp::GraphicsPipelineInfo pipeInfo(rp, layout, {{{
		{vertexShader, vk::ShaderStageBits::vertex},
		{fragmentShader, vk::ShaderStageBits::fragment}
	}}});

	pipeInfo.assembly.topology = vk::PrimitiveTopology::triangleList;

	/* NOTE: if we used a vertex buffer
	constexpr auto stride = (2 + 4) * sizeof(float); // pos (vec2), color(vec4)
	vk::VertexInputBindingDescription bufferBinding {0,
		stride, vk::VertexInputRate::vertex};

	// vertex position, color attributes
	vk::VertexInputAttributeDescription attributes[2];
	attributes[0].format = vk::Format::r32g32Sfloat;

	attributes[1].location = 1;
	attributes[1].format = vk::Format::r32g32b32a32Sfloat;
	attributes[1].offset = 2 * sizeof(float);

	pipeInfo.vertex.vertexBindingDescriptionCount = 1;
	pipeInfo.vertex.pVertexBindingDescriptions = &bufferBinding;
	pipeInfo.vertex.vertexAttributeDescriptionCount = 2;
	pipeInfo.vertex.pVertexAttributeDescriptions = attributes;
	*/

	// we also use the vpp::PipelienCache in this case
	// we try to load it from an already existent cache
	constexpr auto cacheName = "graphicsPipelineCache.bin";
	vpp::PipelineCache cache {dev, cacheName};

	vk::Pipeline vkPipeline;
	vk::createGraphicsPipelines(dev, cache, 1, pipeInfo.info(), nullptr,
		vkPipeline);

	// save the cache to the file we tried to load it from
	vpp::save(cache, cacheName);
	return {dev, vkPipeline};
}

vpp::RenderPass createRenderPass(const vpp::Device& dev, vk::Format format) {
	vk::AttachmentDescription attachment {};

	// color from swapchain
	attachment.format = format;
	attachment.samples = vk::SampleCountBits::e1;
	attachment.loadOp = vk::AttachmentLoadOp::clear;
	attachment.storeOp = vk::AttachmentStoreOp::store;
	attachment.stencilLoadOp = vk::AttachmentLoadOp::dontCare;
	attachment.stencilStoreOp = vk::AttachmentStoreOp::dontCare;
	attachment.initialLayout = vk::ImageLayout::undefined;
	attachment.finalLayout = vk::ImageLayout::presentSrcKHR;

	vk::AttachmentReference colorReference;
	colorReference.attachment = 0;
	colorReference.layout = vk::ImageLayout::colorAttachmentOptimal;

	// only subpass
	vk::SubpassDescription subpass;
	subpass.pipelineBindPoint = vk::PipelineBindPoint::graphics;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorReference;

	vk::RenderPassCreateInfo renderPassInfo;
	renderPassInfo.attachmentCount = 1;
	renderPassInfo.pAttachments = &attachment;
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;

	return {dev, renderPassInfo};
}
