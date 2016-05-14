#include <vpp/transfer.hpp>

namespace vpp
{

//TransferManager
TransferManager::TransferManager(const Device& dev) : Resource(dev)
{
}

BufferRange TransferManager::transferBuffer(std::size_t size)
{
	for(auto buffp : buffers_)
	{
		auto alloc = buffp->use(size);
		if(alloc.size > 0) return BufferRange(*buffp, alloc);
	}

	//Allocate new buffer
	buffers_.push_back(std::make_unique<TransferBuffer>(size));
	return buffers_.back()->use(size);
}

}
