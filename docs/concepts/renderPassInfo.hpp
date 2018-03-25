/// RenderPassCreateInfo utility with defaults, like in pipelineInfo
struct RenderPassInfo {
public:
	std::vector<vk::AttachmentDescription> attachments;
	std::vector<vk::SubpassDescription> subpasses;
	std::vector<vk::SubpassDependency> dependencies;

	const auto& info() const;

protected:
	vk::RenderPassCreateInfo info_;
};

RenderPassInfo rpDefault(vk::Format dst);
RenderPassInfo rpMultisapmle(vk::Format dst, vk::Format msTarget,
	vk::SampleCountBits);
