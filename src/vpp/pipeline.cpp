#include <vpp/pipeline.hpp>
#include <vpp/vk.hpp>

namespace vpp
{

//decriptorSetUpdate
DescriptorSetUpdate::DescriptorSetUpdate(const DescriptorSet& set) : set_(&set)
{
}

DescriptorSetUpdate::~DescriptorSetUpdate()
{
	apply();
}

void DescriptorSetUpdate::apply()
{
	if((writes_.empty() && copies_.empty()) || !set_) return;
	vk::updateDescriptorSets(device(), writes_, copies_);

	writes_.clear();
	copies_.clear();
	buffers_.clear();
	images_.clear();
	views_.clear();
}

void apply(const std::vector<std::reference_wrapper<DescriptorSetUpdate>>& updates)
{
	if(updates.empty()) return;

	std::vector<vk::WriteDescriptorSet> writes;
	std::vector<vk::CopyDescriptorSet> copies;

	for(auto& updateRef : updates)
	{
		auto& update = updateRef.get();

		writes.insert(writes.end(), update.writes_.begin(), update.writes_.end());
		copies.insert(copies.end(), update.copies_.begin(), update.copies_.end());

		update.writes_.clear();
		update.copies_.clear();
	}

	vk::updateDescriptorSets(updates[0].get().device(), writes, copies);

	for(auto& updateRef : updates)
	{
		updateRef.get().buffers_.clear();
		updateRef.get().images_.clear();
		updateRef.get().views_.clear();
	}
}

void DescriptorSetUpdate::uniform(const BufferInfos& buffers, int binding, unsigned int elem)
{
	if(binding == -1) binding = ++currentBinding_;
	else currentBinding_ = binding;

	buffers_.emplace_back(buffers);
	for(auto& b : buffers_.back()) if(b.range == 0) b.range = vk::wholeSize;

	writes_.emplace_back(*set_, binding, elem, buffers.size(), vk::DescriptorType::uniformBuffer,
		nullptr, buffers_.back().data(), nullptr);
}

void DescriptorSetUpdate::storage(const BufferInfos& buffers, int binding, unsigned int elem)
{
	if(binding == -1) binding = ++currentBinding_;
	else currentBinding_ = binding;

	buffers_.emplace_back(buffers);
	for(auto& b : buffers_.back()) if(b.range == 0) b.range = vk::wholeSize;

	writes_.emplace_back(*set_, binding, elem, buffers.size(), vk::DescriptorType::storageBuffer,
		nullptr, buffers_.back().data(), nullptr);
}
void DescriptorSetUpdate::uniformDynamic(const BufferInfos& buffers, int binding, unsigned int elem)
{
	if(binding == -1) binding = ++currentBinding_;
	else currentBinding_ = binding;

	buffers_.emplace_back(buffers);
	for(auto& b : buffers_.back()) if(b.range == 0) b.range = vk::wholeSize;

	writes_.emplace_back(*set_, binding, elem, buffers.size(),
		vk::DescriptorType::uniformBufferDynamic, nullptr, buffers_.back().data(), nullptr);
}
void DescriptorSetUpdate::storageDynamic(const BufferInfos& buffers, int binding, unsigned int elem)
{
	if(binding == -1) binding = ++currentBinding_;
	else currentBinding_ = binding;

	buffers_.emplace_back(buffers);
	for(auto& b : buffers_.back()) if(b.range == 0) b.range = vk::wholeSize;

	writes_.emplace_back(*set_, binding, elem, buffers.size(),
		vk::DescriptorType::storageBufferDynamic, nullptr, buffers_.back().data(), nullptr);
}

void DescriptorSetUpdate::sampler(const ImageInfos& images, int binding, unsigned int elem)
{
	if(binding == -1) binding = ++currentBinding_;
	else currentBinding_ = binding;

	images_.emplace_back(images);

	writes_.emplace_back(*set_, binding, elem, images.size(),
		vk::DescriptorType::sampler, images_.back().data(), nullptr, nullptr);
}
void DescriptorSetUpdate::sampled(const ImageInfos& images, int binding, unsigned int elem)
{
	if(binding == -1) binding = ++currentBinding_;
	else currentBinding_ = binding;

	images_.emplace_back(images);

	writes_.emplace_back(*set_, binding, elem, images.size(),
		vk::DescriptorType::sampledImage, images_.back().data(), nullptr, nullptr);
}
void DescriptorSetUpdate::storage(const ImageInfos& images, int binding, unsigned int elem)
{
	if(binding == -1) binding = ++currentBinding_;
	else currentBinding_ = binding;

	images_.emplace_back(images);

	writes_.emplace_back(*set_, binding, elem, images.size(),
		vk::DescriptorType::storageImage, images_.back().data(), nullptr, nullptr);
}
void DescriptorSetUpdate::combinedSampler(const ImageInfos& images, int binding, unsigned int elem)
{
	if(binding == -1) binding = ++currentBinding_;
	else currentBinding_ = binding;

	images_.emplace_back(images);

	writes_.emplace_back(*set_, binding, elem, images.size(),
		vk::DescriptorType::combinedImageSampler, images_.back().data(), nullptr, nullptr);
}
void DescriptorSetUpdate::inputAttachment(const ImageInfos& images, int binding, unsigned int elem)
{
	if(binding == -1) binding = ++currentBinding_;
	else currentBinding_ = binding;

	images_.emplace_back(images);

	writes_.emplace_back(*set_, binding, elem, images.size(),
		vk::DescriptorType::inputAttachment, images_.back().data(), nullptr, nullptr);
}

void DescriptorSetUpdate::uniform(const BufferViewInfos& views, int binding, unsigned int elem)
{
	if(binding == -1) binding = ++currentBinding_;
	else currentBinding_ = binding;

	views_.emplace_back(views);

	writes_.emplace_back(*set_, binding, elem, views.size(),
		vk::DescriptorType::uniformTexelBuffer, nullptr, nullptr, views_.back().data());
}
void DescriptorSetUpdate::storage(const BufferViewInfos& views, int binding, unsigned int elem)
{
	if(binding == -1) binding = ++currentBinding_;
	else currentBinding_ = binding;

	views_.emplace_back(views);

	writes_.emplace_back(*set_, binding, elem, views.size(),
		vk::DescriptorType::storageTexelBuffer, nullptr, nullptr, views_.back().data());
}
void DescriptorSetUpdate::copy(const vk::CopyDescriptorSet& copySet)
{
	//current binding?
	copies_.push_back(copySet);
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
		vkbindings.back().descriptorType = binding.type;
		vkbindings.back().stageFlags = binding.stages;
		vkbindings.back().descriptorCount = binding.count;
		vkbindings.back().pImmutableSamplers = nullptr;
		vkbindings.back().binding = vkbindings.size() - 1;
	}

	vk::DescriptorSetLayoutCreateInfo descriptorLayout;
	descriptorLayout.bindingCount = vkbindings.size();
	descriptorLayout.pBindings = vkbindings.data();

	layout_ = vk::createDescriptorSetLayout(vkDevice(), descriptorLayout);
	bindings_ = bindings;
}
DescriptorSetLayout::DescriptorSetLayout(DescriptorSetLayout&& other) noexcept
{
	this->swap(other);
}

DescriptorSetLayout& DescriptorSetLayout::operator=(DescriptorSetLayout&& other) noexcept
{
	destroy();
	this->swap(other);
	return *this;
}

DescriptorSetLayout::~DescriptorSetLayout()
{
	destroy();
}

void DescriptorSetLayout::swap(DescriptorSetLayout& other) noexcept
{
	using std::swap;

	swap(bindings_, other.bindings_);
	swap(layout_, other.layout_);
	swap(device_, other.device_);
}

void DescriptorSetLayout::destroy()
{
	if(device_ && vkDescriptorSetLayout())
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
	allocInfo.descriptorPool = pool;
	allocInfo.descriptorSetCount = 1;
	allocInfo.pSetLayouts = &vklayout;

	vk::allocateDescriptorSets(vkDevice(), allocInfo, descriptorSet_);
}

DescriptorSet::DescriptorSet(DescriptorSet&& other) noexcept
{
	this->swap(other);
}

DescriptorSet& DescriptorSet::operator=(DescriptorSet&& other) noexcept
{
	this->~DescriptorSet();
	this->swap(other);
	return *this;
}

DescriptorSet::~DescriptorSet()
{
	layout_ = nullptr;
	descriptorSet_ = {};
}

void DescriptorSet::swap(DescriptorSet& other) noexcept
{
	using std::swap;

	swap(descriptorSet_, other.descriptorSet_);
	swap(layout_, other.layout_);
	swap(device_, other.device_);
}

void DescriptorSet::update(const std::vector<vk::WriteDescriptorSet>& writes,
	const std::vector<vk::CopyDescriptorSet>& copies) const
{
	vk::updateDescriptorSets(vkDevice(), writes, copies);
}

void DescriptorSet::update(const std::vector<vk::CopyDescriptorSet>& copies) const
{
	vk::updateDescriptorSets(vkDevice(), {}, copies);
}

//Pipeline
Pipeline::Pipeline(const Device& dev) : Resource(dev)
{
}

Pipeline::Pipeline(Pipeline&& other) noexcept
{
	swap(*this, other);
}

Pipeline& Pipeline::operator=(Pipeline&& other) noexcept
{
	this->~Pipeline();
	swap(*this, other);
	return *this;
}

Pipeline::~Pipeline()
{
	if(pipeline_) vk::destroyPipeline(vkDevice(), pipeline_, nullptr);
	if(pipelineLayout_) vk::destroyPipelineLayout(vkDevice(), pipelineLayout_, nullptr);

	pipeline_ = {};
	pipelineLayout_ = {};
}

void swap(Pipeline& a, Pipeline& b) noexcept
{
	using std::swap;

	swap(a.pipelineLayout_, b.pipelineLayout_);
	swap(a.pipeline_, b.pipeline_);
	swap(a.device_, b.device_);
}

//utility -format size in bits
unsigned int formatSize(vk::Format format)
{
	using namespace vk;

	switch(format)
    {
    case Format::undefined: return 0;
	case Format::r4g4UnormPack8: return 8;
	case Format::r4g4b4a4UnormPack16: return 16;
    case Format::b4g4r4a4UnormPack16: return 16;
    case Format::r5g6b5UnormPack16: return 16;
    case Format::b5g6r5UnormPack16: return 16;
    case Format::r5g5b5a1UnormPack16: return 16;
    case Format::b5g5r5a1UnormPack16: return 16;
    case Format::a1r5g5b5UnormPack16: return 16;
	case Format::r8Unorm: return 8;
    case Format::r8Snorm: return 8;
    case Format::r8Uscaled: return 8;
    case Format::r8Sscaled: return 8;
    case Format::r8Uint: return 8;
    case Format::r8Sint: return 8;
    case Format::r8Srgb: return 8;
	case Format::r8g8Unorm: return 16;
    case Format::r8g8Snorm: return 16;
    case Format::r8g8Uscaled: return 16;
    case Format::r8g8Sscaled: return 16;
    case Format::r8g8Uint: return 16;
    case Format::r8g8Sint: return 16;
    case Format::r8g8Srgb: return 16;
	case Format::r8g8b8Unorm: return 24;
    case Format::r8g8b8Snorm: return 24;
    case Format::r8g8b8Uscaled: return 24;
    case Format::r8g8b8Sscaled: return 24;
    case Format::r8g8b8Uint: return 24;
    case Format::r8g8b8Sint: return 24;
    case Format::r8g8b8Srgb: return 24;
    case Format::b8g8r8Unorm: return 24;
    case Format::b8g8r8Snorm: return 24;
    case Format::b8g8r8Uscaled: return 24;
	case Format::b8g8r8Sscaled: return 24;
    case Format::b8g8r8Uint: return 24;
    case Format::b8g8r8Sint: return 24;
    case Format::b8g8r8Srgb: return 24;
	case Format::r8g8b8a8Unorm: return 32;
    case Format::r8g8b8a8Snorm: return 32;
    case Format::r8g8b8a8Uscaled: return 32;
    case Format::r8g8b8a8Sscaled: return 32;
	case Format::r8g8b8a8Uint: return 32;
    case Format::r8g8b8a8Sint: return 32;
    case Format::r8g8b8a8Srgb: return 32;
    case Format::b8g8r8a8Unorm: return 32;
    case Format::b8g8r8a8Snorm: return 32;
    case Format::b8g8r8a8Uscaled: return 32;
    case Format::b8g8r8a8Sscaled: return 32;
    case Format::b8g8r8a8Uint: return 32;
    case Format::b8g8r8a8Sint: return 32;
    case Format::b8g8r8a8Srgb: return 32;
    case Format::a8b8g8r8UnormPack32: return 32;
    case Format::a8b8g8r8SnormPack32: return 32;
    case Format::a8b8g8r8UscaledPack32: return 32;
    case Format::a8b8g8r8SscaledPack32: return 32;
    case Format::a8b8g8r8UintPack32: return 32;
    case Format::a8b8g8r8SintPack32: return 32;
    case Format::a8b8g8r8SrgbPack32: return 32;
    case Format::a2r10g10b10UnormPack32: return 32;
    case Format::a2r10g10b10SnormPack32: return 32;
    case Format::a2r10g10b10UscaledPack32: return 32;
    case Format::a2r10g10b10SscaledPack32: return 32;
    case Format::a2r10g10b10UintPack32: return 32;
    case Format::a2r10g10b10SintPack32: return 32;
    case Format::a2b10g10r10UnormPack32: return 32;
    case Format::a2b10g10r10SnormPack32: return 32;
    case Format::a2b10g10r10UscaledPack32: return 32;
    case Format::a2b10g10r10SscaledPack32: return 32;
    case Format::a2b10g10r10UintPack32: return 32;
    case Format::a2b10g10r10SintPack32: return 32;
    case Format::r16Unorm: return 16;
    case Format::r16Snorm: return 16;
    case Format::r16Uscaled: return 16;
    case Format::r16Sscaled: return 16;
	case Format::r16Uint: return 16;
    case Format::r16Sint: return 16;
    case Format::r16Sfloat: return 16;
    case Format::r16g16Unorm: return 32;
    case Format::r16g16Snorm: return 32;
    case Format::r16g16Uscaled: return 32;
    case Format::r16g16Sscaled: return 32;
    case Format::r16g16Uint: return 32;
    case Format::r16g16Sint: return 32;
    case Format::r16g16Sfloat: return 32;
	case Format::r16g16b16Unorm: return 48;
    case Format::r16g16b16Snorm: return 48;
    case Format::r16g16b16Uscaled: return 48;
    case Format::r16g16b16Sscaled: return 48;
    case Format::r16g16b16Uint: return 48;
    case Format::r16g16b16Sint: return 48;
    case Format::r16g16b16Sfloat: return 48;
	case Format::r16g16b16a16Unorm: return 64;
    case Format::r16g16b16a16Snorm: return 64;
    case Format::r16g16b16a16Uscaled: return 64;
    case Format::r16g16b16a16Sscaled: return 64;
    case Format::r16g16b16a16Uint: return 64;
    case Format::r16g16b16a16Sint: return 64;
    case Format::r16g16b16a16Sfloat: return 64;
	case Format::r32Uint: return 32;
    case Format::r32Sint: return 32;
    case Format::r32Sfloat: return 32;
    case Format::r32g32Uint: return 64;
    case Format::r32g32Sint: return 64;
    case Format::r32g32Sfloat: return 64;
	case Format::r32g32b32Uint: return 96;
    case Format::r32g32b32Sint: return 96;
    case Format::r32g32b32Sfloat: return 96;
	case Format::r32g32b32a32Uint: return 128;
    case Format::r32g32b32a32Sint: return 128;
    case Format::r32g32b32a32Sfloat: return 128;
    case Format::r64Uint: return 64;
    case Format::r64Sint: return 64;
    case Format::r64Sfloat: return 64;
    case Format::r64g64Uint: return 128;
    case Format::r64g64Sint: return 128;
    case Format::r64g64Sfloat: return 128;
	case Format::r64g64b64Uint: return 192;
    case Format::r64g64b64Sint: return 192;
	case Format::r64g64b64Sfloat: return 192;
    case Format::r64g64b64a64Uint: return 256;
    case Format::r64g64b64a64Sint: return 256;
    case Format::r64g64b64a64Sfloat: return 256;
	case Format::b10g11r11UfloatPack32: return 32;
    case Format::e5b9g9r9UfloatPack32: return 32;
	case Format::d16Unorm: return 16;
    case Format::x8D24UnormPack32: return 32;
    case Format::d32Sfloat: return 32;
	case Format::s8Uint: return 8;
	case Format::d16UnormS8Uint: return 24;
	case Format::d24UnormS8Uint: return 32;
	case Format::d32SfloatS8Uint: return 48;
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


}
