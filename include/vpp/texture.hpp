#pragma once

#include <vpp/fwd.hpp>
#include <vpp/resource.hpp>
#include <vpp/image.hpp>

namespace vpp
{

class Texture2D : public Resource
{
public:
	struct CreateInfo
	{
		vk::Format format = vk::Format::R32G32B32Sfloat;
		vk::ImageViewCreateInfo viewInfo = {};
		vk::SamplerCreateInfo samplerInfo = {};
	};

protected:
	Image image_;
	vk::ImageView imageView_;
	vk::Sampler sampler_;
	vk::Extent2D size_;
	vk::Format format_;

protected:
	void initImage(const ImageData& data);
	void initImageView();
	void initSampler();

public:
	Texture2D() = default;
	Texture2D(const Device& dev, const CreateInfo& info, const vk::Extent2D& size, const std::uint8_t& data);
	Texture2D(DeviceMemoryAllocator& alloc, const CreateInfo& info, const vk::Extent2D& size, const std::uint8_t& data);
	Texture2D(Image&& image, const CreateInfo& info);
	~Texture2D();

	void initMemoryLess(DeviceMemoryAllocator& alloc, const vk::Extent2D& size);
	void initMemoryResources(const CreateInfo& info, const std::uint8_t& data);

	vk::Sampler vkSampler() const { return sampler_; };
	vk::ImageView vkImageView() const { return imageView_; }
	const vk::Extent2D& size() const { return size_; }
	const Image& image() const { return image_; }
};


}
