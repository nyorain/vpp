/// Vulkan RenderPass Instance, i.e. a commandbuffer recording session during
/// a render pass.
class RenderPassInstance : public nytl::NonCopyable {
public:
	RenderPassInstance(vk::CommandBuffer cmdbuf, vk::RenderPass pass, vk::Framebuffer framebuffer);
	~RenderPassInstance();

	const vk::RenderPass& renderPass() const { return renderPass_; }
	const vk::CommandBuffer& vkCommandBuffer() const { return commandBuffer_; }
	const vk::Framebuffer& vkFramebuffer() const { return framebuffer_; }

protected:
	vk::RenderPass renderPass_ {};
	vk::CommandBuffer commandBuffer_ {};
	vk::Framebuffer framebuffer_ {};
};

