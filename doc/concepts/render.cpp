class Renderer
{
public:

public:


protected:
	unsigned int qFamily_; ///The queue family the commands will be recorded for.
};

class RendererBuilder
{
public:
	///This function is called before every frame and allows the builder to execute/queue
	///additional operations or to re-record the command buffer for the given id.
	virtual void frame(Renderer& renderer, unsigned int id) = 0;

	///This function is called to record the render commands into the given renderpass instance.
	virtual void build(Renderer& renderer, unsigned int id, const RenderPassInatance& renderini) = 0

	virtual void beforeRender(Renderer& renderer, unsigned int id, const CommandBuffer& buffer) = 0;

	virtual void afterRender(Renderer& renderer, unsigned int id, const CommandBuffer& buffer) = 0;
};

SwapChainRenderer renderer(swapChain, {renderPass, {ViewableImage::defaultDepth2D}}, std::move(builder));
