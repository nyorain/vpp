#include "godrays.hpp"

App* gApp = nullptr;

//Renderer
Renderer::Renderer(App& app)
	: Resource(app.context->device()), app_(app)
{
	initDescriptors();
	initDescriptorBuffers();
	initVertexBuffer();
}

Renderer::~Renderer()
{
}

void Renderer::init(const vpp::SwapChainRenderer& renderer)
{
	app_.renderer = const_cast<vpp::SwapChainRenderer*>(&renderer);
	initStencilTexture();
	writeDescriptorSets();
	writeVertexBuffer();
	initPipeline();

	lastUpdate_ = Clock::now();
}

void Renderer::build(const vpp::RenderPassInstance& instance) const
{
	auto cmdBuffer = instance.vkCommandBuffer();
	VkDeviceSize offsets[1] = { 0 };

	auto gd = descriptorSet_.vkDescriptorSet();
	auto vb = vertexBuffer_.vkBuffer();
	auto pgd = postDescriptorSet_.vkDescriptorSet();

	//color (normal) rendering
	vk::cmdBindPipeline(cmdBuffer, vk::PipelineBindPoint::Graphics, pipeline_.vkPipeline());
	vk::cmdBindDescriptorSets(cmdBuffer, vk::PipelineBindPoint::Graphics,
		pipeline_.vkPipelineLayout(), 0, 1, &gd, 0, nullptr);
	vk::cmdBindVertexBuffers(cmdBuffer, 0, 1, &vb, offsets);
	vk::cmdDraw(cmdBuffer, 3, 1, 0, 0);

	//god rays post-processing
	vk::cmdNextSubpass(cmdBuffer, vk::SubpassContents::Inline);
	vk::cmdBindPipeline(cmdBuffer, vk::PipelineBindPoint::Graphics, postPipeline_.vkPipeline());
	vk::cmdBindDescriptorSets(cmdBuffer, vk::PipelineBindPoint::Graphics,
		postPipeline_.vkPipelineLayout(), 0, 1, &pgd, 0, nullptr);
	vk::cmdDraw(cmdBuffer, 6, 1, 0, 0);
}

std::vector<vk::ClearValue> Renderer::clearValues() const
{
	std::vector<vk::ClearValue> ret(2);
	ret[0].color(std::array<float, 4>{{0.f, 0.f, 0.f, 1.f}});
	ret[1].depthStencil({1.f, 1});

	return ret;
}

void Renderer::initPipeline()
{
	vpp::VertexBufferLayout layout = {{vk::Format::R32G32Sfloat}};

	vpp::GraphicsPipeline::CreateInfo info;
	info.descriptorSetLayouts = {&descriptorSetLayout_};
	info.vertexBufferLayouts = {&layout};
	info.dynamicStates = {};
	info.renderPass = app_.renderPass.vkRenderPass();

	info.shader = vpp::ShaderProgram(device());
	info.shader.addStage({"godrays-source.vert.spv", vk::ShaderStageFlagBits::Vertex});
	info.shader.addStage({"godrays-source.frag.spv", vk::ShaderStageFlagBits::Fragment});

	info.states = vpp::GraphicsPipeline::StatesCreateInfo{vk::Viewport{0, 0, 900, 900, 0.f, 1.f}};
	info.states.rasterization.cullMode(vk::CullModeFlagBits::None);
	info.states.inputAssembly.topology(vk::PrimitiveTopology::TriangleList);

	vk::StencilOpState stencilop;
	stencilop.compareOp(vk::CompareOp::Always);
	stencilop.reference(0);
	stencilop.passOp(vk::StencilOp::Replace);

	info.states.depthStencil.stencilTestEnable(1);
	info.states.depthStencil.front(stencilop);
	info.states.depthStencil.back(stencilop);

	pipeline_ = vpp::GraphicsPipeline(device(), info);

	//post

	info.descriptorSetLayouts = {&postDescriptorSetLayout_};

	info.shader = vpp::ShaderProgram(device());
	info.shader.addStage({"godrays-combine.vert.spv", vk::ShaderStageFlagBits::Vertex});
	info.shader.addStage({"godrays-combine.frag.spv", vk::ShaderStageFlagBits::Fragment});

	info.states = vpp::GraphicsPipeline::StatesCreateInfo{vk::Viewport{0, 0, 900, 900, 0.f, 1.f}};
	info.states.rasterization.cullMode(vk::CullModeFlagBits::None);
	info.states.inputAssembly.topology(vk::PrimitiveTopology::TriangleList);

	postPipeline_ = vpp::GraphicsPipeline(device(), info);
}

void Renderer::initVertexBuffer()
{
	vk::BufferCreateInfo info;
	info.size(sizeof(nytl::Vec2f) * 3);
	info.usage(vk::BufferUsageFlagBits::VertexBuffer);

	vertexBuffer_ = vpp::Buffer(device(), info, vk::MemoryPropertyFlagBits::HostVisible);
}

void Renderer::initStencilTexture()
{
	//view
	vk::ImageSubresourceRange range;
	range.aspectMask(vk::ImageAspectFlagBits::Stencil);
	range.baseMipLevel(0);
	range.baseArrayLayer(0);
	range.levelCount(1);
	range.layerCount(1);

	vk::ImageViewCreateInfo view;
	view.subresourceRange(range);
	view.format(vk::Format::D16UnormS8Uint);
	view.viewType(vk::ImageViewType::e2D);
	view.image(app_.renderer->staticAttachments()[0].vkImage());

	vk::createImageView(vkDevice(), &view, nullptr, &stencilView_);

	//sampler
	auto sampler = defaultSamplerInfo();
	vk::createSampler(vkDevice(), &sampler, nullptr, &stencilSampler_);
}

void Renderer::initDescriptors()
{
	//init pool
	vk::DescriptorPoolSize typeCounts[3] {};

	typeCounts[0].type(vk::DescriptorType::UniformBuffer);
	typeCounts[0].descriptorCount(2);

	typeCounts[1].type(vk::DescriptorType::CombinedImageSampler);
	typeCounts[1].descriptorCount(1);

	typeCounts[2].type(vk::DescriptorType::InputAttachment);
	typeCounts[2].descriptorCount(1);

	vk::DescriptorPoolCreateInfo descriptorPoolInfo;
	descriptorPoolInfo.poolSizeCount(3);
	descriptorPoolInfo.pPoolSizes(typeCounts);
	descriptorPoolInfo.maxSets(2);

	vk::createDescriptorPool(vkDevice(), &descriptorPoolInfo, nullptr, &descriptorPool_);


	//graphics set
	std::vector<vpp::DescriptorBinding> gfxBindings =
	{
		{vk::DescriptorType::UniformBuffer, vk::ShaderStageFlagBits::Vertex |
			vk::ShaderStageFlagBits::Fragment}
	};
	descriptorSetLayout_ = vpp::DescriptorSetLayout(device(), gfxBindings);

	descriptorSet_ = vpp::DescriptorSet(descriptorSetLayout_, descriptorPool_);

	//post
	gfxBindings =
	{
		{vk::DescriptorType::UniformBuffer, vk::ShaderStageFlagBits::Fragment},
		{vk::DescriptorType::CombinedImageSampler, vk::ShaderStageFlagBits::Fragment},
		{vk::DescriptorType::InputAttachment, vk::ShaderStageFlagBits::Fragment}
	};
	postDescriptorSetLayout_ = vpp::DescriptorSetLayout(device(), gfxBindings);
	postDescriptorSet_ = vpp::DescriptorSet(postDescriptorSetLayout_, descriptorPool_);
}

void Renderer::initDescriptorBuffers()
{
	vk::BufferCreateInfo gfxInfo;
	gfxInfo.size(sizeof(nytl::Vec3f) + sizeof(float));
	gfxInfo.usage(vk::BufferUsageFlagBits::UniformBuffer);

	ubo_ = vpp::Buffer(device(), gfxInfo, vk::MemoryPropertyFlagBits::HostVisible);

	//post
	gfxInfo.size(sizeof(nytl::Vec2f) + sizeof(float) + sizeof(std::uint32_t));
	gfxInfo.usage(vk::BufferUsageFlagBits::UniformBuffer);

	postUbo_ = vpp::Buffer(device(), gfxInfo, vk::MemoryPropertyFlagBits::HostVisible);
}

void Renderer::updateUBO(const nytl::Vec2ui& mousePos)
{
	//normal
	static float time = 0.f;

	auto now = Clock::now();
	float delta = std::chrono::duration_cast<std::chrono::duration<float, std::ratio<1, 1>>>(now - lastUpdate_).count();
	lastUpdate_ = now;

	time += delta;

	//float attract = (GetKeyState(VK_LBUTTON) < 0);
	//nytl::Vec2f normMousePos = 2 * (mousePos / nytl::Vec2f(app_.width, app_.height)) - 1;

	auto map = ubo_.memoryMap();
	//std::cout << (void*)map.ptr() << " vs " << (void*)map.memoryMap().ptr() << "\n";
	std::memcpy(map.ptr(), &time, sizeof(float));
	map.memoryMap().flushRanges();

	map = {};

	std::cout << time << "\n";

	//post
	nytl::Vec2f lightPos(0.2, 0.7);
	float exposure = 1.f;
	std::uint32_t samples = 128;

	auto map2 = postUbo_.memoryMap();
	std::memcpy(map2.ptr(), &lightPos, sizeof(nytl::Vec2f));
	std::memcpy(map2.ptr() + sizeof(nytl::Vec2f), &exposure, sizeof(float));
	std::memcpy(map2.ptr() + sizeof(nytl::Vec2f) + sizeof(float), &samples, sizeof(std::uint32_t));

	map2.memoryMap().flushRanges();
}

void Renderer::writeDescriptorSets()
{
	ubo_.assureMemory();
	postUbo_.assureMemory();

	vk::WriteDescriptorSet writes[4];

	//normal
	vk::DescriptorBufferInfo binfo1;
	binfo1.buffer(ubo_.vkBuffer());
	binfo1.offset(0);
	binfo1.range(VK_WHOLE_SIZE);

	writes[0].dstSet(descriptorSet_.vkDescriptorSet());
	writes[0].dstBinding(0);
	writes[0].dstArrayElement(0);
	writes[0].descriptorCount(1);
	writes[0].descriptorType(vk::DescriptorType::UniformBuffer);
	writes[0].pBufferInfo(&binfo1);

	//post
	vk::DescriptorBufferInfo binfo2;
	binfo2.buffer(postUbo_.vkBuffer());
	binfo2.offset(0);
	binfo2.range(VK_WHOLE_SIZE);

	vk::DescriptorImageInfo imginfo1;
	imginfo1.imageView(stencilView_);
	imginfo1.sampler(stencilSampler_);
	imginfo1.imageLayout(vk::ImageLayout::DepthStencilAttachmentOptimal);

	vk::DescriptorImageInfo imginfo2;
	imginfo2.imageView(app_.renderer->staticAttachments()[1].vkImageView());
	imginfo2.imageLayout(vk::ImageLayout::ColorAttachmentOptimal);

	writes[1].dstSet(postDescriptorSet_.vkDescriptorSet());
	writes[1].dstBinding(0);
	writes[1].dstArrayElement(0);
	writes[1].descriptorCount(1);
	writes[1].descriptorType(vk::DescriptorType::UniformBuffer);
	writes[1].pBufferInfo(&binfo2);

	writes[2].dstSet(postDescriptorSet_.vkDescriptorSet());
	writes[2].dstBinding(1);
	writes[2].dstArrayElement(0);
	writes[2].descriptorCount(1);
	writes[2].descriptorType(vk::DescriptorType::CombinedImageSampler);
	writes[2].pImageInfo(&imginfo1);

	writes[3].dstSet(postDescriptorSet_.vkDescriptorSet());
	writes[3].dstBinding(2);
	writes[3].dstArrayElement(0);
	writes[3].descriptorCount(1);
	writes[3].descriptorType(vk::DescriptorType::InputAttachment);
	writes[3].pImageInfo(&imginfo2);

	//write
	vk::updateDescriptorSets(vkDevice(), 4, writes, 0, nullptr);
}

void Renderer::writeVertexBuffer()
{
	std::vector<nytl::Vec2f> points = {
		{-0.25, -0.25}, {0.25, -0.25}, {0.25, 0.25}
	};

	vertexBuffer_.assureMemory();
	auto map = vertexBuffer_.memoryMap();

	std::memcpy(map.ptr(), points.data(), points.size() * sizeof(nytl::Vec2f));
}

void Renderer::update(const nytl::Vec2ui& mousePos)
{
	updateUBO(mousePos);
}


//utility
vk::SamplerCreateInfo defaultSamplerInfo()
{
	vk::SamplerCreateInfo ret;

	ret.magFilter(vk::Filter::Linear);
	ret.minFilter(vk::Filter::Linear);
	ret.mipmapMode(vk::SamplerMipmapMode::Linear);
	ret.addressModeU(vk::SamplerAddressMode::ClampToEdge);
	ret.addressModeV(vk::SamplerAddressMode::ClampToEdge);
	ret.addressModeW(vk::SamplerAddressMode::ClampToEdge);
	ret.mipLodBias(0.f);
	ret.anisotropyEnable(1);
	ret.maxAnisotropy(8);
	ret.compareEnable(0);
	ret.compareOp(vk::CompareOp::Always);
	ret.minLod(0.f);
	ret.maxLod(0.25f);
	ret.borderColor(vk::BorderColor::IntOpaqueBlack);
	ret.unnormalizedCoordinates(0);

	return ret;
}

void initRenderPass(App& app)
{
	auto& dev = app.context->device();
	auto& swapChain = app.context->swapChain();

	vk::AttachmentDescription attachments[2] {};

	//color from swapchain
	attachments[0].format(swapChain.format());
	attachments[0].samples(vk::SampleCountFlagBits::e1);
	attachments[0].loadOp(vk::AttachmentLoadOp::Clear);
	attachments[0].storeOp(vk::AttachmentStoreOp::Store);
	attachments[0].stencilLoadOp(vk::AttachmentLoadOp::DontCare);
	attachments[0].stencilStoreOp(vk::AttachmentStoreOp::DontCare);
	attachments[0].initialLayout(vk::ImageLayout::ColorAttachmentOptimal);
	attachments[0].finalLayout(vk::ImageLayout::ColorAttachmentOptimal);

	//depth and stencil from own buffer
	attachments[1].format(vk::Format::D16UnormS8Uint);
	attachments[1].samples(vk::SampleCountFlagBits::e1);
	attachments[1].loadOp(vk::AttachmentLoadOp::Clear);
	attachments[1].storeOp(vk::AttachmentStoreOp::Store);
	attachments[1].stencilLoadOp(vk::AttachmentLoadOp::DontCare);
	attachments[1].stencilStoreOp(vk::AttachmentStoreOp::DontCare);
	attachments[1].initialLayout(vk::ImageLayout::DepthStencilAttachmentOptimal);
	attachments[1].finalLayout(vk::ImageLayout::DepthStencilAttachmentOptimal);

	//own color for pre rendering
	attachments[2].format(swapChain.format());
	attachments[2].samples(vk::SampleCountFlagBits::e1);
	attachments[2].loadOp(vk::AttachmentLoadOp::Clear);
	attachments[2].storeOp(vk::AttachmentStoreOp::Store);
	attachments[2].stencilLoadOp(vk::AttachmentLoadOp::DontCare);
	attachments[2].stencilStoreOp(vk::AttachmentStoreOp::DontCare);
	attachments[2].initialLayout(vk::ImageLayout::ColorAttachmentOptimal);
	attachments[2].finalLayout(vk::ImageLayout::ColorAttachmentOptimal);

	vk::AttachmentReference swapchainColorReference;
	swapchainColorReference.attachment(0);
	swapchainColorReference.layout(vk::ImageLayout::ColorAttachmentOptimal);

	vk::AttachmentReference depthReference;
	depthReference.attachment(1);
	depthReference.layout(vk::ImageLayout::DepthStencilAttachmentOptimal);

	vk::AttachmentReference customColorReference;
	customColorReference.attachment(2);
	customColorReference.layout(vk::ImageLayout::ColorAttachmentOptimal);

	vk::AttachmentReference customColorInputReference;
	customColorReference.attachment(2);
	customColorReference.layout(vk::ImageLayout::ShaderReadOnlyOptimal);

	//subpasses
	vk::SubpassDescription subpasses[2];

	//first subpass for rendring color/stencil
	vk::SubpassDescription& fbSubpass = subpasses[0];
	fbSubpass.pipelineBindPoint(vk::PipelineBindPoint::Graphics);
	fbSubpass.flags({});
	fbSubpass.inputAttachmentCount(0);
	fbSubpass.pInputAttachments(nullptr);
	fbSubpass.colorAttachmentCount(1);
	fbSubpass.pColorAttachments(&customColorReference);
	//fbSubpass.pColorAttachments(&swapchainColorReference);
	fbSubpass.pResolveAttachments(nullptr);
	fbSubpass.pDepthStencilAttachment(&depthReference);
	fbSubpass.preserveAttachmentCount(0);
	fbSubpass.pPreserveAttachments(nullptr);

	//second subpass for rendering blending previous color with god rays (gotten from stenci sampler)
	vk::SubpassDescription& finalSubpass = subpasses[1];
	finalSubpass.pipelineBindPoint(vk::PipelineBindPoint::Graphics);
	finalSubpass.flags({});
	finalSubpass.inputAttachmentCount(1);
	finalSubpass.pInputAttachments(&customColorInputReference);
	finalSubpass.colorAttachmentCount(1);
	finalSubpass.pColorAttachments(&swapchainColorReference);
	finalSubpass.pResolveAttachments(nullptr);
	finalSubpass.pDepthStencilAttachment(nullptr);
	finalSubpass.preserveAttachmentCount(0);
	finalSubpass.pPreserveAttachments(nullptr);

	//subpass deps
	vk::SubpassDependency dependency;
	dependency.srcSubpass(0);
	dependency.dstSubpass(1);
	dependency.srcStageMask(vk::PipelineStageFlagBits::AllCommands);
	dependency.dstStageMask(vk::PipelineStageFlagBits::FragmentShader);
	dependency.srcAccessMask(vk::AccessFlagBits::ColorAttachmentWrite);
	dependency.dstAccessMask(vk::AccessFlagBits::InputAttachmentRead);
	dependency.dependencyFlags(vk::DependencyFlagBits::ByRegion);

	//render pass
	vk::RenderPassCreateInfo renderPassInfo;
	renderPassInfo.attachmentCount(3);
	renderPassInfo.pAttachments(attachments);
	renderPassInfo.subpassCount(2);
	renderPassInfo.pSubpasses(subpasses);
	renderPassInfo.dependencyCount(1);
	renderPassInfo.pDependencies(&dependency);

	app.renderPass = vpp::RenderPass(dev, renderPassInfo);
}