#include <vpp/vk.hpp>
#include <vpp/device.hpp>

const vpp::Device& device();

auto fence = vk::createFence(device(), {});
std::vector<vk::BindSparseInfo> infos;

//binding a sparse buffer to a single memory object
vk::SparseMemoryBind bind;
bind.resourceOffset = 0;
bind.size = memReqs.size; //the size must be at least memReqs.alignment
bind.memory = memory;
bind.flags = {};

vk::SparseBufferMemoryBindInfo bufferBind;
bufferBind.buffer = buffer;
bufferBind.bindCount = 1;
bufferBind.pBinds = &bind;

infos.emplace_back();
infos.back().bufferBindCount = 1;
infos.back().pBufferBinds = &bufferBind;

//binding an image
//difference between opaque and partially resident images
//sparse image binding is way more complex since one has to care about metadata bindings,
//differentiate between opaque or normal bindings and query/deal with additional requirements
//and device features since some stuff might not be enabled
auto reqs = vk::getImageSparseMemoryRequirements(device(), image);

vk::SparseMemoryBind bind;
bind.resourceOffset = 0;
bind.size = memReqs.size;
bind.memory = memory;
bind.flags = {};


//submitting the sparse binding operations to the device
queue.mutex().lock();
vk::queueBindSparse(queue, infos, fence);
queue.mutex().unlock();

//wait for the fence



//the idea for implementing sparse memory entries is to make the MemoryEntry class virtual and
//then provide multiple implementations (e.g. default, sparse, sparseResidency, sparseAliasing)
//The down-side of this would be that it would result in one extra word per memory resource for
//the virtual table, the need to allocate the memory entry implementation on the heap instead of
//the stack and the fact that sparse memory entries cannot be implemented without huuge host-side
//memory needs.
//The question is if sparse memory resources can be handled in any safe way, so that they would
//not need that much resources on the host side. If there is such a way, leave memor entry as it
//is and make vpp::MemoryResource a class for non-sparse memory resources and instead provide
//helpers for the alternative handling of sparse resources.
//probably there isnt such a way. one has to keep track of memory bindings.

class SparseMemoryEntry : public Resource
{
public:
	///Ensures that the given memory resource range is bound to a device memory object.
	///This function will request the needed memory from the associated DeviceMemoryAllocator.
	WorkPtr ensureBound(const Allocation& allocation);

	WorkPtr bind(const Allocation& allocation, )

protected:
	MemoryResourceType type_; //buffer, image, opaque image
	vk::DeviceSize size_;
};

class ManagedSparseMemoryEntry : public SparseMemoryEntry
{
protected:
	DeviceMemoryAllocator& allocator_;
	std::vector<MemoryBinds> binds_;
};


class MemoryEntry
{
public:
	virtual ~MemoryEntry();
	virtual void ensureBound(vk::DeviceSize offset = 0, vk::DeviceSize size = 0) = 0;
	virtual vk::DeviceSize size() const = 0;
};

class SparseBinder
{
public:
	void add(/* some bind info params*/);
};




class MemoryEntry
{
protected:
	DeviceMemoryAllocator* allocator_;
	unsigned int id_;
};

class MemoryAllocator;
class SparseMemoryAllocator : public MemoryAllocator;


// attempt #2
// sparse header
// TODO
struct OpaqueMemoryBind {
	DeviceMemory* memory;
	Allocation allocation;
	vk::DeviceSize resourceOffset;
};

template<typename Extent3D>
struct ImageMemoryBindT {
	MemoryEntry entry;
	Extent3D offset;
	Extent3D size;
};
using ImageMemoryBind = ImageMemoryBindT<vk::Extent3D>;

class SparseOpaqueMemoryEntry {
protected:
	std::vector<OpaqueMemoryBind> binds_;
};

class SparseImageMemoryEntry {
protected:
	std::vector<std::variant<OpaqueMemoryBind, ImageMemoryBind>> binds_;
};

class SparseImage : public vpp::ImageHandle {
public:

protected:
	SparseImageMemoryEntry entry_;
};

class SparseOpaqueImage : public vpp::ImageHandle {
public:
protected:	
	SparseOpaqueMemoryEntry entry_;
};

class SparseBuffer : public vpp::BufferHandle {
public:

protected:
	SparseOpaqueMemoryEntry entry_;
};
