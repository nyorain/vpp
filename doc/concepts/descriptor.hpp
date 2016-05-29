///Class that allows to batch together mulitple descriptor set requests to use as few
///vulkan DescriptorPools as possible
class DescriptorAllocator
{
public:
	///Requests to allocate the needed descriptors for the given set.
	void request(vpp::DescriptorSet& set);

	///Does allocate and update all requested descriptors.
	void allocate();
};


struct Initializer
{
	DeviceMemoryProvider& memoryProvider;
	CommandProvider& commandProvider;
	DescriptorProvider& descriptorProvider;

	SubmitManager& submitManager;
	TransferManager& transferManager;
	WorkManager& workManager;
};

///Two step initialization
template<typename T>
class Initialize
{
public:
	template<typename... T>
	Initialization(T&&... args)
	{
		obj_.create(std::forward<T>(args)...);
	}

	template<typename... T>
	T init(T&&... args)
	{
		obj_.init(std::forward<T>(args)...);
		return std::move(obj_);
	}

protected:
	T obj_;
};

//will allocate exactly one memory object for all needed buffers and textures
//will allocate exactly one descriptor pool for all needed descriptor sets (e.g. material data)
//will (depending on settings, gpu or cpu bound?) submit exactly one work batch to the gpu.
//will need exactly one command buffer pool

//creation does load the file from disk into ram and requestes the needed buffers and descriptors
Initialize<Mesh> mesh1Init(device, file1);
Initialize<Mesh> mesh2Init(device, file2);
Initialize<Mesh> mesh3Init(device, file3);

//initialization does queue a buffer fill operation for all needed buffers
auto mesh1 = mesh1Init.init();
auto mesh2 = mesh2Init.init();
auto mesh3 = mesh3Init.init();

///submits the unsubmitted gpu work
///waits for the gpu to finish executing the setup work (copying buffer etc.)
///executes all furthermore needed commands
///setup clean up
device.finishSetup();
