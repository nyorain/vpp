#include <vpp/buffer.hpp>
#include <utility>

namespace vpp
{

Buffer::Buffer(const Device& dev, const vk::BufferCreateInfo& info, vk::MemoryPropertyFlags mflags)
	 : Resource(dev)
{
	vk::MemoryRequirements reqs;
	vk::createBuffer(vkDevice(), &info, nullptr, &buffer_);
	vk::getBufferMemoryRequirements(vkDevice(), buffer_, &reqs);

	reqs.memoryTypeBits(device().memoryTypeBits(mflags, reqs.memoryTypeBits()));
	device().deviceMemoryAllocator().request(buffer_, reqs, memoryEntry_);
}

Buffer::Buffer(Buffer&& other) noexcept
{
	swap(*this, other);
}

Buffer& Buffer::operator=(Buffer&& other) noexcept
{
	destroy();
	swap(*this, other);
	return *this;
}

Buffer::~Buffer()
{
	destroy();
}

void swap(Buffer& a, Buffer& b) noexcept
{
	using std::swap;

	swap(b.memoryEntry_, a.memoryEntry_);
	swap(b.buffer_, a.buffer_);
	swap(b.device_, a.device_);
}

void Buffer::destroy()
{
	if(vkBuffer()) vk::destroyBuffer(vkDevice(), vkBuffer(), nullptr);

	memoryEntry_ = {};
	buffer_ = {};
	Resource::destroy();
}

MemoryMapView Buffer::memoryMap() const
{
	assureMemory();
	return memoryEntry().map();
}

//todo
std::unique_ptr<Work<void>> Buffer::fill(const std::vector<BufferData>& data) const
{
	assureMemory();

	//mappable?
	if(memoryEntry().memory()->propertyFlags() & vk::MemoryPropertyFlagBits::HostVisible) {
		auto map = memoryEntry().map();

		std::size_t offset = 0;
		for(auto& dataEntry : data) {
			offset += dataEntry.offset;
			std::memcpy(map.ptr() + offset, dataEntry.data, dataEntry.size);
			offset += dataEntry.size;
		}

		//flushMemoryRanges?
		return std::make_unique<FinishedWork<void>>();
	} else {
		class WorkImpl : public Work<void>
		{
		protected:

		public:

		};

		auto cmdBuffer = device().setupCommandBuffer();
		auto uploadBuffer = device().uploadBuffer(memoryEntry().allocation().size);

		vk::BufferCopy region(0, 0, memoryEntry().allocation().size);
		vk::cmdCopyBuffer(cmdBuffer, uploadBuffer.vkBuffer(), vkBuffer(), 1, &region);

		return std::make_unique<WorkImpl>();
	}
}

void Buffer::assureMemory() const
{
	memoryEntry_->allocate();
}

}
