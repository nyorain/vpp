#pragma once

namespace vpp
{

class Texture2D : public Resource
{
public:
	struct ImageData
	{
		std::uint8_t& data;
		vk::Extent2D size_;
	};

protected:
	Image image_;
	vk::ImageView imageView_;
	vk::Sampler sampler_;
	vk::Extent2D size_;

public:
	Texture2D() = default;
	Texture2D(const Device& dev, const ImageData& data);

	vk::Sampler vkSampler() const { return sampler_; };
	vk::ImageView vkImageView() const { return imageView_; }
	const vk::Extent2D& size() const { return size_; }
	const Image& image() const { return image_; }
};


}
