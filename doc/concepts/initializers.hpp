struct CompleteRenderPassInfo {
	vk::RenderPassCreateInfo renderPassInfo;
	std::vector<AttachmentDescription> attachments;
	std::vector<SubpassDescription> subpasses;
	std::vector<SubpassDependency> dependencies;

	const vk::RenderPassCreateInfo& get();
	operator vk::RenderPassCreateInfo() { return get(); }
};

const vk::RenderPassCreateInfo& CompleteRenderPassInfo::get()
{
	renderPassInfo.attachmentCount = attachments.size();
	renderPassInfo.pAttachments = attachments.data();
	renderPassInfo.subpassCount = subpasses.size();
	renderPassInfo.pSubpasses = subpasses.data();
	renderPassInfo.dependencyCount = dependencies.size();
	renderPassInfo.pDependencies = dependencies.data();
	return renderPassInfo;
}


namespace init::rp {

CompleteRenderPassInfo defaultColorOnly(vk::Format colorFormat);
CompleteRenderPassInfo defaultColorDepth(vk::Format colorFormat, vk::Format depthFormat = ...);

}


namespace init::gfxpipe {

CompleteGraphicsPipelineInfo simple(const RenderPass& rp);
CompleteGraphicsPipelineInfo simpleDepth(const RenderPass& rp);

}
