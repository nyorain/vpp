#pragma once

#include <vpp/renderer.hpp> // vpp::DefaultRenderer
#include <vpp/handles.hpp> // vpp::PipelineLayout, vpp::Pipeline, ...
#include <vpp/vk.hpp> // vulkan commands and structs

vpp::RenderPass createRenderPass(const vpp::Device&, vk::Format);

// vpp::Renderer implementation (using DefaultRenderer since we
// don't need special framebuffer stuff).
// Pretty much only initializes the pipeline, implements an easier resize
// function for the window and implements the command buffer recording.
class MyRenderer : public vpp::DefaultRenderer {
public:
	MyRenderer(vk::RenderPass, vk::SwapchainCreateInfoKHR&,
		const vpp::Queue& present);

	void resize(const vk::Extent2D& extent);
	void record(const RenderBuffer& buf) override;

protected:
	vpp::PipelineLayout pipelineLayout_;
	vpp::Pipeline pipeline_;
	vk::SwapchainCreateInfoKHR& scInfo_;
};
