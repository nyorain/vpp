///concept about even manage buffers and images with providers.
///In the worst case one buffer/image is created for each request, in the best case there may
///be lots of bufferRange objects on one buffer which might increase performance significantly.
///look at: https://developer.nvidia.com/vulkan-memory-management
class BufferProvider
{
public:
	class Buffer
	{
	protected:
		Buffer buffer_;
		std::vector<Allocation> allocations_;
	};

public:
	BufferRange request(std::size_t size, vk::MemoryPropertyFlags memFlags, vk::BufferUsageFlags flags);

protected:
	std::vector<ProviderBuffer> buffer_;
};


class BufferRange
{
public:
	const Allocation& allocation() const { return allocation_; }
	Buffer& buffer() const { return buffer_; }

	///Makes sure it is valid
	void init();
};



class ImageProvider
{
public:
	ImageRange request(const vk::Extent3D& extent, /* several options */);

};
