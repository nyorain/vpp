#pragma once

#include <vpp/fwd.hpp>
#include <vpp/vk.hpp>
#include <vpp/resource.hpp>
#include <vpp/buffer.hpp>
#include <vpp/utility/allocation.hpp>

#include <memory>

namespace vpp
{

///Provides transfer buffers to easily fill large device local buffers and images.
///Basically a pool of mappable vulkan buffers, which can be used for copying.
class TransferManager : public Resource
{
public:
	///A vulkan buffer wrapper that can be used for multiple transferations at the same time.
	class TransferBuffer : ResourceReference<TransferBuffer>
	{
	public:
		TransferBuffer(const Device& dev, std::size_t size);
		~TransferBuffer();

		const Buffer& buffer() const { return buffer_; }

		Allocation use(std::size_t size);
		bool release(const Allocation& alloc);
		std::size_t rangesCount() const { return ranges_.size(); }

		const Buffer& resourceRef() const { return buffer_; }

	protected:
		Buffer buffer_;
		std::vector<Allocation> ranges_;
	};

	///Represents a part of a transfer buffer which can be used for transerfering data to the gpu.
	///The destructor does automatically release the used transfer buffer range.
	class BufferRange : public ResourceReference<BufferRange>
	{
	public:
		BufferRange(TransferBuffer& buf, const Allocation& al) : buffer_(&buf), allocation_(al) {}
		~BufferRange() { if(buffer_) buffer_->release(allocation_); }

		BufferRange(BufferRange&& other) noexcept = default;
		BufferRange& operator=(BufferRange&& other) noexcept = default;

		const Buffer& buffer() const { return buffer_->buffer(); }
		vk::Buffer vkBuffer() const { return buffer().vkBuffer(); }
		const Allocation& allocation() const { return allocation_; }
		std::size_t offset() const { return allocation().offset; }
		std::size_t size() const { return allocation().size; }

		const TransferBuffer& resourceRef() const { return *buffer_; }

	protected:
		TransferBuffer* buffer_;
		Allocation allocation_;
	};

public:
	TransferManager() = default;
	TransferManager(const Device& dev);

	///Returns an avaible upload buffer with the given size (allocates one if not already there).
	BufferRange transferBuffer(std::size_t size);

	///Returns the amount of vulkan buffers managed.
	std::size_t bufferCount() const { return buffers_.size(); }

	///Returns the total buffer size of all owned buffers.
	std::size_t totalSize() const;

	///Returns the amount of currently for transerfing used ranges.
	std::size_t activeRanges() const;

	///Reserves the amount of transfer buffer capacity
	void reserve(std::size_t size);

	///Releases all currently unused buffers.
	void shrink();

	///Optimizes the memory allocation. Will recreate all unused buffers as one big buffer.
	void optimize();

protected:
	//must be a pointer for the BufferRange pointer member to stay valid.
	std::vector<std::unique_ptr<TransferBuffer>> buffers_;
};

///Convinient typedef for TransferManager::BufferRange
using TransferRange = TransferManager::BufferRange;

}
