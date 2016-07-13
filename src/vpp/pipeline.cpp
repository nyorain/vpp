#include <vpp/pipeline.hpp>
#include <vpp/vk.hpp>
#include <vpp/utility/file.hpp>

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

void apply(const Range<std::reference_wrapper<DescriptorSetUpdate>>& updates)
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
		auto& update = updateRef.get();

		update.buffers_.clear();
		update.images_.clear();
		update.views_.clear();
	}
}

void DescriptorSetUpdate::uniform(BufferInfos buffers, int binding, unsigned int elem)
{
	if(binding == -1) binding = currentBinding_++;
	else currentBinding_ = binding + 1;

	buffers_.emplace_back(std::move(buffers));
	for(auto& b : buffers_.back()) if(b.range == 0) b.range = vk::wholeSize;

	writes_.emplace_back(*set_, binding, elem, buffers_.back().size(),
		vk::DescriptorType::uniformBuffer, nullptr, buffers_.back().data(), nullptr);
}

void DescriptorSetUpdate::storage(BufferInfos buffers, int binding, unsigned int elem)
{
	if(binding == -1) binding = currentBinding_++;
	else currentBinding_ = binding + 1;

	buffers_.emplace_back(std::move(buffers));
	for(auto& b : buffers_.back()) if(b.range == 0) b.range = vk::wholeSize;

	writes_.emplace_back(*set_, binding, elem, buffers_.back().size(),
		vk::DescriptorType::storageBuffer, nullptr, buffers_.back().data(), nullptr);
}
void DescriptorSetUpdate::uniformDynamic(BufferInfos buffers, int binding, unsigned int elem)
{
	if(binding == -1) binding = currentBinding_++;
	else currentBinding_ = binding + 1;

	buffers_.emplace_back(std::move(buffers));
	for(auto& b : buffers_.back()) if(b.range == 0) b.range = vk::wholeSize;

	writes_.emplace_back(*set_, binding, elem, buffers_.back().size(),
		vk::DescriptorType::uniformBufferDynamic, nullptr, buffers_.back().data(), nullptr);
}
void DescriptorSetUpdate::storageDynamic(BufferInfos buffers, int binding, unsigned int elem)
{
	if(binding == -1) binding = currentBinding_++;
	else currentBinding_ = binding + 1;

	buffers_.emplace_back(std::move(buffers));
	for(auto& b : buffers_.back()) if(b.range == 0) b.range = vk::wholeSize;

	writes_.emplace_back(*set_, binding, elem, buffers_.back().size(),
		vk::DescriptorType::storageBufferDynamic, nullptr, buffers_.back().data(), nullptr);
}

void DescriptorSetUpdate::sampler(ImageInfos images, int binding, unsigned int elem)
{
	if(binding == -1) binding = currentBinding_++;
	else currentBinding_ = binding + 1;

	images_.emplace_back(std::move(images));

	writes_.emplace_back(*set_, binding, elem, images_.back().size(),
		vk::DescriptorType::sampler, images_.back().data(), nullptr, nullptr);
}
void DescriptorSetUpdate::image(ImageInfos images, int binding, unsigned int elem)
{
	if(binding == -1) binding = currentBinding_++;
	else currentBinding_ = binding + 1;

	images_.emplace_back(std::move(images));

	writes_.emplace_back(*set_, binding, elem, images_.back().size(),
		vk::DescriptorType::sampledImage, images_.back().data(), nullptr, nullptr);
}
void DescriptorSetUpdate::storage(ImageInfos images, int binding, unsigned int elem)
{
	if(binding == -1) binding = currentBinding_++;
	else currentBinding_ = binding + 1;

	images_.emplace_back(std::move(images));

	writes_.emplace_back(*set_, binding, elem, images_.back().size(),
		vk::DescriptorType::storageImage, images_.back().data(), nullptr, nullptr);
}
void DescriptorSetUpdate::combinedSampler(ImageInfos images, int binding, unsigned int elem)
{
	if(binding == -1) binding = currentBinding_++;
	else currentBinding_ = binding + 1;

	images_.emplace_back(std::move(images));

	writes_.emplace_back(*set_, binding, elem, images_.back().size(),
		vk::DescriptorType::combinedImageSampler, images_.back().data(), nullptr, nullptr);
}
void DescriptorSetUpdate::inputAttachment(ImageInfos images, int binding, unsigned int elem)
{
	if(binding == -1) binding = currentBinding_++;
	else currentBinding_ = binding + 1;

	images_.emplace_back(std::move(images));

	writes_.emplace_back(*set_, binding, elem, images_.back().size(),
		vk::DescriptorType::inputAttachment, images_.back().data(), nullptr, nullptr);
}

void DescriptorSetUpdate::uniform(BufferViewInfos views, int binding, unsigned int elem)
{
	if(binding == -1) binding = currentBinding_++;
	else currentBinding_ = binding + 1;

	views_.emplace_back(std::move(views));

	writes_.emplace_back(*set_, binding, elem, views_.back().size(),
		vk::DescriptorType::uniformTexelBuffer, nullptr, nullptr, views_.back().data());
}
void DescriptorSetUpdate::storage(BufferViewInfos views, int binding, unsigned int elem)
{
	if(binding == -1) binding = currentBinding_++;
	else currentBinding_ = binding + 1;

	views_.emplace_back(std::move(views));

	writes_.emplace_back(*set_, binding, elem, views_.back().size(),
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
	swap(*this, other);
}

DescriptorSetLayout& DescriptorSetLayout::operator=(DescriptorSetLayout other) noexcept
{
	destroy();
	swap(*this, other);
	return *this;
}

DescriptorSetLayout::~DescriptorSetLayout()
{
	if(device_ && vkDescriptorSetLayout())
	{
		vk::destroyDescriptorSetLayout(vkDevice(), layout_, nullptr);
	}
}

void swap(DescriptorSetLayout& a, DescriptorSetLayout& b) noexcept
{
	using std::swap;

	swap(a.bindings_, b.bindings_);
	swap(a.layout_, b.layout_);
	swap(a.device_, b.device_);
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
	swap(*this, other);
}

DescriptorSet& DescriptorSet::operator=(DescriptorSet other) noexcept
{
	swap(*this, other);
	return *this;
}

DescriptorSet::~DescriptorSet()
{
	layout_ = nullptr;
	descriptorSet_ = {};
}

void swap(DescriptorSet& a, DescriptorSet& b) noexcept
{
	using std::swap;

	swap(a.descriptorSet_, b.descriptorSet_);
	swap(a.layout_, b.layout_);
	swap(a.device_, b.device_);
}

//Pipeline
Pipeline::Pipeline(const Device& dev) : Resource(dev)
{
}

Pipeline::Pipeline(Pipeline&& other) noexcept
{
	swap(*this, other);
}

Pipeline& Pipeline::operator=(Pipeline other) noexcept
{
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

	swap(static_cast<ResourceHandle<vk::Pipeline>&>(a), static_cast<ResourceHandle<vk::Pipelne>&>(b));
	swap(a.pipelineLayout_, b.pipelineLayout_);
	swap(a.layoutOwned_, b.layoutOwned_);
}

//cache
void savePipelineCache(vk::Device dev, vk::PipelineCache cache, const char* filename)
{
	auto data = vk::getPipelineCacheData(dev, cache);
	writeFile(filename, data);
}

vk::PipelineCache loadPipelineCache(vk::Device dev, const char* filename)
{
	auto data = readFile(filename);

	vk::PipelineCacheCreateInfo info;
	info.initialDataSize = data.size();
	info.pInitialData = data.data();

	return vk::createPipelineCache(dev, info);
}

//utility -format size in bits
unsigned int formatSizeBits(vk::Format format)
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

	    case Format::bc1RgbUnormBlock: return 64;
	    case Format::bc1RgbSrgbBlock: return 64;
	    case Format::bc1RgbaUnormBlock: return 64;
	    case Format::bc1RgbaSrgbBlock: return 64;
	    case Format::bc2UnormBlock: return 128;
	    case Format::bc2SrgbBlock: return 128;
	    case Format::bc3UnormBlock: return 128;
	    case Format::bc3SrgbBlock: return 128;
	    case Format::bc4UnormBlock: return 64;
	    case Format::bc4SnormBlock: return 64;
	    case Format::bc5UnormBlock: return 128;
	    case Format::bc5SnormBlock: return 128;
	    case Format::bc6hUfloatBlock: return 128;
	    case Format::bc6hSfloatBlock: return 128;
	    case Format::bc7UnormBlock: return 128;
	    case Format::bc7SrgbBlock: return 128;
	    case Format::etc2R8g8b8UnormBlock: return 64;
	    case Format::etc2R8g8b8SrgbBlock: return 64;
	    case Format::etc2R8g8b8a1UnormBlock: return 64;
	    case Format::etc2R8g8b8a1SrgbBlock: return 64;
	    case Format::etc2R8g8b8a8UnormBlock: return 128;
	    case Format::etc2R8g8b8a8SrgbBlock: return 64;
	    case Format::eacR11UnormBlock: return 64;
	    case Format::eacR11SnormBlock: return 64;
	    case Format::eacR11g11UnormBlock: return 128;
	    case Format::eacR11g11SnormBlock: return 128;
	    case Format::astc4x4UnormBlock: return 128;
	    case Format::astc4x4SrgbBlock: return 128;
	    case Format::astc5x4UnormBlock: return 128;
	    case Format::astc5x4SrgbBlock: return 128;
	    case Format::astc5x5UnormBlock: return 128;
	    case Format::astc5x5SrgbBlock: return 128;
	    case Format::astc6x5UnormBlock: return 128;
	    case Format::astc6x5SrgbBlock: return 128;
	    case Format::astc6x6UnormBlock: return 128;
	    case Format::astc6x6SrgbBlock: return 128;
	    case Format::astc8x5UnormBlock: return 128;
	    case Format::astc8x5SrgbBlock: return 128;
	    case Format::astc8x6UnormBlock: return 128;
	    case Format::astc8x6SrgbBlock: return 128;
	    case Format::astc8x8UnormBlock: return 128;
	    case Format::astc8x8SrgbBlock: return 128;
	    case Format::astc10x5UnormBlock: return 128;
	    case Format::astc10x5SrgbBlock: return 128;
	    case Format::astc10x6UnormBlock: return 128;
	    case Format::astc10x6SrgbBlock: return 128;
	    case Format::astc10x8UnormBlock: return 128;
	    case Format::astc10x8SrgbBlock: return 128;
	    case Format::astc10x10UnormBlock: return 128;
	    case Format::astc10x10SrgbBlock: return 128;
	    case Format::astc12x10UnormBlock: return 128;
	    case Format::astc12x10SrgbBlock: return 128;
	    case Format::astc12x12UnormBlock: return 128;
	    case Format::astc12x12SrgbBlock: return 128;
		default: return 0;
	}
}

unsigned int formatSize(vk::Format format)
{
	return formatSizeBits(format) / 8;
}

vk::Extent2D blockSize(vk::Format format)
{
	using vk::Format;
	switch(format)
	{
		case Format::bc1RgbUnormBlock: return {4, 4};
		case Format::bc1RgbSrgbBlock: return {4, 4};
		case Format::bc1RgbaUnormBlock: return {4, 4};
		case Format::bc1RgbaSrgbBlock: return {4, 4};
		case Format::bc2UnormBlock: return {4, 4};
	    case Format::bc2SrgbBlock: return {4, 4};
	    case Format::bc3UnormBlock: return {4, 4};
	    case Format::bc3SrgbBlock: return {4, 4};
	    case Format::bc4UnormBlock: return {4, 4};
	    case Format::bc4SnormBlock: return {4, 4};
	    case Format::bc5UnormBlock: return {4, 4};
	    case Format::bc5SnormBlock: return {4, 4};
	    case Format::bc6hUfloatBlock: return {4, 4};
	    case Format::bc6hSfloatBlock: return {4, 4};
	    case Format::bc7UnormBlock: return {4, 4};
	    case Format::bc7SrgbBlock: return {4, 4};
	    case Format::etc2R8g8b8UnormBlock: return {4, 4};
	    case Format::etc2R8g8b8SrgbBlock: return {4, 4};
	    case Format::etc2R8g8b8a1UnormBlock: return {4, 4};
	    case Format::etc2R8g8b8a1SrgbBlock: return {4, 4};
	    case Format::etc2R8g8b8a8UnormBlock: return {4, 4};
	    case Format::etc2R8g8b8a8SrgbBlock: return {4, 4};
	    case Format::eacR11UnormBlock: return {4, 4};
	    case Format::eacR11SnormBlock: return {4, 4};
	    case Format::eacR11g11UnormBlock: return {4, 4};
	    case Format::eacR11g11SnormBlock: return {4, 4};
	    case Format::astc4x4UnormBlock: return {4, 4};
	    case Format::astc4x4SrgbBlock: return {4, 4};
	    case Format::astc5x4UnormBlock: return {5, 4};
	    case Format::astc5x4SrgbBlock: return {5, 4};
	    case Format::astc5x5UnormBlock: return {5, 5};
	    case Format::astc5x5SrgbBlock: return {5, 5};
	    case Format::astc6x5UnormBlock: return {6, 5};
	    case Format::astc6x5SrgbBlock: return {6, 5};
	    case Format::astc6x6UnormBlock: return {6, 6};
	    case Format::astc6x6SrgbBlock: return {6, 6};
	    case Format::astc8x5UnormBlock: return {8, 5};
	    case Format::astc8x5SrgbBlock: return {8, 5};
	    case Format::astc8x6UnormBlock: return {8, 6};
	    case Format::astc8x6SrgbBlock: return {8, 6};
	    case Format::astc8x8UnormBlock: return {8, 8};
	    case Format::astc8x8SrgbBlock: return {8, 8};
	    case Format::astc10x5UnormBlock: return {10, 5};
	    case Format::astc10x5SrgbBlock: return {10, 5};
	    case Format::astc10x6UnormBlock: return {10, 6};
	    case Format::astc10x6SrgbBlock: return {10, 6};
	    case Format::astc10x8UnormBlock: return {10, 8};
	    case Format::astc10x8SrgbBlock: return {10, 8};
	    case Format::astc10x10UnormBlock: return {10, 10};
	    case Format::astc10x10SrgbBlock: return {10, 10};
	    case Format::astc12x10UnormBlock: return {12, 10};
	    case Format::astc12x10SrgbBlock: return {12, 10};
	    case Format::astc12x12UnormBlock: return {12, 12};
	    case Format::astc12x12SrgbBlock: return {12, 12};
		case vk::Format::undefined: return {0, 0};
		default: return {1, 1};
	}
}

}
