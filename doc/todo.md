Todo list vor vpp
=================

Points are only partly sorted by priority.

- testing!
- think about swapchain out of date handling (swapchain/swapchainRenderer)
	- recreate swapchain automatically? how to handle it?
	- further swapchain improvements: see acquire/present todos
		- give appliction possibility to gracefully handle outOfDate errors
	- some bad bugs at the moment in the combination of swapchainrenderer + swapchain
- fix bufferOps!
	- (further) clean up bufferOps.inl
- procAddr: test if local cache really faster than load it every time?
- when mapping images in write/retrieve, first make sure they have the correct layout
	- change it, return command work ptr if needed
- some general SwapChainRenderer improvements/reworking needed
	- better renderer resetting (all command pools at once, just resize the frameRenderers vector)
	- SwapchainRenderer::init: call record?
		- if RendererImpl should call it in init, document it!
	- the concept (kindof) is alright, maybe just add another (more low level) rendering-helper
- cleanups/fixes to the 2-step init concept
	- what about buffers/images?

C++ 17:
------

- procAddr.cpp: use shared_mutex
- clean up the Resource::swap mess
	- base method inheritance
- use a destroy function for resources that is called from ResourceHandle destrctor
- use "using" declarations in the derived resource classes to make the
	protected ResourceHandle constructors visisble
- use std::memory_resource for more efficient host memory allocation
	- Device should store a thread-specific memory resource
	- use it inside vpp for memory heavy operations (see DeviceMemoryAllocator)
- threadlocal.hpp: use shared_mutex instead of shared_timed_mutex
- pmr for performance critical (every-frame) functions.


low prio / general / ideas
--------------------------

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
	[stringParam, range, nonCopyable in utility]
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
