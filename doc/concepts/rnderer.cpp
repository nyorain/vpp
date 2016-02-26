int main()
{
	std::vector<vpp::RenderBufferType> staticBuffers = { vpp::RenderBufferType::Depth };
	std::vector<vpp::RenderBufferType> dynamicBuffers = { vpp::RenderBufferType::Color; }

	//
	auto attachments = 
	{
		vk::AttachementDescripttion{},
		vk::AttachementDescripttion{}
	};

	auto subPassesInfo = 
	{
		{
			{vpp::RenderBufferType::Depth, }	
		}
	};

	vpp::Renderer renderer(renderPass, staticBuffers, dynamicBuffers);
}

///RenderSettings
struct RenderSettings
{
	std::vector<vk::AttachmentDescription> attachments_;
	std::vector<vk::SubpassDescription> subpasses_;
	std::vector<vk::SubpassDependency> dependencies_;
	std::vector<vk::AttachmentReference> references_;
};

enum class RendererBufferType
{
	input,
	color,
	resolve,
	depthStencil,
	preserve
};

struct StaticRendererBuffer
{
	RendererBufferType type;
	std::unique_ptr<Image> image;
	vk::ImageView imageView;
};

struct DynamicRendererBuffer
{
	RendererBufferType type;	
};

struct RendererInfo
{
	std::vector<StaticRendererBuffer> static_;
	std::vector<DynamicRendererBuffer> dynamic_;
};

class Framebuffer
{
public:
	struct Buffer
	{
		std::unique_ptr<Image> image;
		vk::ImageView imageView;
	};

protected:
	vk::Framebuffer frameBuffer_ {};
	std::vector<Buffer> buffers_ {};
};

struct RenderBuffer
{
	std::unique_ptr<Image> image;
	vk::ImageView imageView;
};

//
class Renderer : public Resource
{
protected:
	vk::RenderPass renderPass_;
	std::vector<RenderBuffer> static_;

public:
	Renderer(const RendererInfo& info, const RenderSettings& settings);
	void beginDraw(vk::CommandBuffer buffer, const Framebuffer& framebuffer);
};
