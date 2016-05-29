/** UploadHeaps can be used to upload data to more efficient deviceLocal buffers which cannot be
  * mapped. An UploadHeap ist basically a pool of mappable buffers which keeps track of their
  * usage.
  * Threfore you can just ask for such an uploadBuffer and the heap will get you one (if there
  * is enough device memory). It will flag the buffer as used but you can return him a
  * CommandExecutionState instance that will allow the UploadHeap to use the buffer again once
  * the copying is completed. If you do not give him such a control, it will simply assume you
  * have finished using the buffer and use it again.
  * So UploadHeap do basically just offer temporary mappable vulkan buffers.
  *
  * Usually, you do not want to use the UploadHeap directly but rather use the Buffer::/Image::
  * fill functions to upload something to them (even if they are DeviceLocal).
  * Note that UploadHeap objects must be externally synchronized.
  **/

class UploadHeap
{
public:
	class UploadBuffer
	{
	protected:
		Buffer buffer_;
		CommandExecutionState state_;
		bool used_ = false;
	};

protected:
	std::vector<std::unique_ptr<UploadBuffer>> buffers_;

public:
	UploadBuffer& uploadBuffer(std::size_t size);
};


vpp::Buffer buffer;
vpp::UploadHeap heap;
std::vector<float> data;

//#2, more explicit, harder to use and implement
auto uploadBuffer = heap.uploadBuffer(data.size() * 4);

//uploadBuffer.fill(data);
{
	auto map = uploadBuffer.memoryMap();
	std::memcpy(map.ptr(), data.data());
}

//reocrd it on some commandBuffer and care yourself for execution
vk::cmdCopyBuffer(setupCommandBuffer, uploadBuffer.vkBuffer(), buffer.vkBuffer());

//Will mark the buffer as used until the setupCommandBuffer completes.
uploadBuffer.used(setupCommandBuffer.state());


class SubmitManager
{
public:
	void submit();
	vk::Fence submit(vk::Queue queue);
	ExecuteState add(vk::Queue, vk::SubmitInfo info);

protected:
	std::size_t autoSubmitThreshold_;
	std::vector with submissions
};


class ExecuteState
{
public:
	void submit();
	void wait();
};

class SetupCommandBuffer
{
public:
	ExecuteState queue(); //queues the setup buffer for execution
};
