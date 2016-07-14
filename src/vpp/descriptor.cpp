#include <vpp/descriptor.hpp>
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
void DescriptorSetUpdate::imageSampler(ImageInfos images, int binding, unsigned int elem)
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
	const Range<vk::DescriptorSetLayoutBinding>& bindings) : ResourceHandle(dev)
{
	std::vector<vk::DescriptorSetLayoutBinding> vkbindings;
	vkbindings.reserve(bindings.size());

	unsigned int highestBinding = 0u;
	for(auto& binding : bindings)
	{
		vkbindings.emplace_back(binding);
		if(vkbindings.back().binding == -1) vkbindings.back().binding = highestBinding++;
		else highestBinding = std::max(highestBinding, vkbindings.back().binding + 1);
	}

	vk::DescriptorSetLayoutCreateInfo descriptorLayout;
	descriptorLayout.bindingCount = vkbindings.size();
	descriptorLayout.pBindings = vkbindings.data();

	vkHandle() = vk::createDescriptorSetLayout(vkDevice(), descriptorLayout);
}

DescriptorSetLayout::~DescriptorSetLayout()
{
	if(vkHandle()) vk::destroyDescriptorSetLayout(vkDevice(), vkHandle());
}

//DescriptorSet
DescriptorSet::DescriptorSet(const DescriptorSetLayout& layout, vk::DescriptorPool pool)
	: ResourceHandle(layout.device())
{
	vk::DescriptorSetAllocateInfo allocInfo;
	allocInfo.descriptorPool = pool;
	allocInfo.descriptorSetCount = 1;
	allocInfo.pSetLayouts = &layout.vkHandle();

	vk::allocateDescriptorSets(vkDevice(), allocInfo, vkHandle());
}

DescriptorSet::~DescriptorSet()
{
	///XXX: something about descriptorSet desctruction/freeing
}

//DescriptorPool
DescriptorPool::DescriptorPool(const Device& dev, const vk::DescriptorPoolCreateInfo& info)
	: ResourceHandle(dev)
{
	vkHandle() = vk::createDescriptorPool(dev, info);
}
DescriptorPool::~DescriptorPool()
{
	if(vkHandle()) vk::destroyDescriptorPool(device(), vkHandle());
}

//utility
vk::DescriptorSetLayoutBinding descriptorBinding(vk::DescriptorType type,
	vk::ShaderStageFlags stages, unsigned int binding,
	unsigned int count, const vk::Sampler* samplers)
{
	return {binding, type, count, stages, samplers};
}

}
