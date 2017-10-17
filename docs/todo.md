Todo list vor vpp
=================

- testing!
	- continue bufferops testing/fixing
	- test everything using valgrind (with/without layers) to find potential leaks/errors
	- test & fix image upload (layout) bug
- fix image fill/retrieve
	- data size bug (maybe in transferbuffer already)
	- take into account that image does not only have its data in memory
		- couldn't the same be the case for buffer?
- clean up usage of dlg
	- check can often be replaced with assert
- clean up usage of nytl
	- just include it as subproject?
- make codestyle consistent everywhere
- procAddr: test if local cache really faster than load it every time?
- when mapping images in write/retrieve, first make sure they have the correct layout
	- change it, return command work ptr if needed?
- cleanups/fixes to the 2-step init concept
	- what about buffers/images?
		- would it make sense for them to behave the way everything else does?
			- i.e. Buffer() + create(param) + init(param) OR Buffer(params)
				- Change the constructor semantics to already initialize the memory
- separate header for stuff that requires the generated vulkan headers
	- don't pull them in in other headers
- release next version


low prio / general / ideas
--------------------------

- is size value in MemoryEntry really needed?
	- rather work with some id's or something?
- add TrackedDescriptor* from kyo
	- also DescriptorAllocate
- use using declarations in the derived resource classes to make the
	protected ResourceHandle constructors visisble
- pmr for performance critical functions.
	- Device to store a thread-specific memory resource?
	- use it inside vpp for memory heavy operations (see DeviceMemoryAllocator)
- is there a better way for the Resource::swap mess?
- generalize TransferBuffer into some shared buffer
- allow to explicity allocate memory on a given memory allocator.
  To create large (like over 100 mb) buffers of a memory type we know we will need
  Also something like an additional allocation strategy?
  Allocate more than needed if the user wants it
- vpp: don't output all extensions. Only required (via settings) ones
- vpp: some way to detect installed vulkan version and automatically generate for it?
	- should be doable with meson (python vulkan module; get version; download spec; parse it)
	- Would probably require some spec version testing for codegen (fix issues with ALL spec version...)
- codegen: queueFamilyExternalKhr (i.e. constants of extensions) -> ...KHR
- config: vpp_debug vs vpp_ndebug rather messy now
	- configurable from build system?
- work dependencies
	- make it possible (in some way) for work objects to depend on each other
	- the work objects itself will figure out how to do it (e.g. by fence or
		semaphore synchronization, or by simply waiting on the work before
		submitting or in which step ever)
- which information should resources carry around, which not?
- seperate interface/implementation for header-only interfaces
	- see: bufferOps, Resource
	- make it more explicit (make sure to only fill types if sure, see ShaderType::none handling)
	- __really?__ needs discussion
- textures (overthink viewable image + sampler)
	- scope of vpp?
- general initializer
	- something about descriptors and descriptor pools
	- think about buffer/image providers (better not)
- display class for vkDisplayKHR extension
	- scope of vpp?
	- must wait until supported somewhere, for tests
- pipeline init helper functions (really useful/needed?)
	- pipeline rework
		- see initializers concept...
		- rather unintuitive at the moment to explicitly call record (?)
- queue constness? (maybe make it related to any operations on the queue?)
- codegen constexpr?
- write deviceLost handling code snippet example
- shader stage construction in place?
	- make it easier to create shader modules that are only once used directly inside
		a ShaderProgram. Is there any way that keeps a reasonable interface?

- think about include of nytl headers. Should really the namespace be replaced? header guards?
	projects using nytl outside of vpp should be able to do so (and use it for vpp calls).
	[range, nonCopyable in utility]
	- where (if) to use namespace nytl (fwd.hpp? already in the vk headers?)

- further custom exception? like vpp::QueueError if there is not queue that
	can execute the needed operation
	- think about some functionality to handle device lost (how to deal with it?)

- External constructors for all resources (construct them from existing handles)
- further examples/snippets/documentation
- better best physical device querying (vpp/physicalDevice.cpp)

- alternative two step init
	- safer
	- see framebuffer, rather unsafe initializattion spliiting
	- create/init methods for buffer and image?
		- would be consistent at least
		- make the constructor fully initialize it (like specified in init.hpp?)
		- rework/rethink init.hpp specifications

- idea moved from init.hpp:

// TODO: asynchronous two step initialization concept
// some operations do need more than 2 steps to be fully initialized.
// Make their waiting asynchronous in a way that it makes sense (?)
// e.g. wait 5ms for someone else to ask for an uploadBuffer, if no one is doing, ask yourself
// then record the needed commands, then wait 20ms for someone else to submit work on a valid
// queue, if no one is doing submit youself.
// TODO (e.g. for the point above): dynamic queue mangement. Sometimes more than one queue
// would be able to execute commands, make it possible to just give some expression that is
// available to use the first matching queue.
