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
		vk::Format format;
		vk::SamplerCreateInfo samplerInfo;
	};

	struct ImageData
	{
		std::uint8_t* data;
		vk::Extent2D size_;
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
	Texture2D(const Device& dev, const CreateInfo& info, const ImageData& data);
	Texture2D(const DeviceMemoryAllocator& dev, const CreateInfo& info, const ImageData& data);
	Texture2D(Image&& image, const CreateInfo& info);
	~Texture2D();

	void init(Image&& image, const CreateInfo& info);

	vk::Sampler vkSampler() const { return sampler_; };
	vk::ImageView vkImageView() const { return imageView_; }
	const vk::Extent2D& size() const { return size_; }
	const Image& image() const { return image_; }
};


}
