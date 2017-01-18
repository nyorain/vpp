Todo list vor vpp
=================

- C++17 update (if usable compiler):
	- clean up the Resource::swap mess
	- use a destroy function for resources that is called from ResourceHandle destrctor
	- use "using" declarations in the derived resource classes to make the
		protected ResourceHandle constructors visisble
	- use std::memory_resource for more efficient host memory allocation
		- Device should store a thread-specific memory resource
		- use it inside vpp for memory heavy operations (see DeviceMemoryAllocator)
	- threadlocal.hpp: use shared_mutex instead of shared_timed_mutex

- move example code snippets out of code!
- debug checks: make sure to never throw in destructor!
- pipeline rework
- SwapchainRenderer::init: call record?
	- if REndererImpl should call it in init, document it!
- (further) clean up bufferOps.inl
- better best physical device querying (vpp/physicalDevice.cpp)
- work implementation destructor should call finish (not all do)!
- remove (deprecated) device layers everywhere
- better renderer resetting (all command pools at once, just resize the frameRenderers vector)
- textures (overthing viewable image + sampler)
- fully implement examples!
	- > see code snippets todo
- general initializer
	- something about descriptors and descriptor pools
	- think about buffer/image providers (better not)
- improve classes to use allocators and provider
- backend agnostic createContext function
	- remove them at all?
- display class for vkDisplayKHR extension
- queue management
	- easier querying/creation/using for commands
- some smaller improvements
	- mapMemory: check valid usage (esp. alignment and stuff)
	- pmr for performance critical (every-frame) functions.
- shader stage construction in place?
	- make it easier to create shader modules that are only once used directly inside
		a ShaderProgram. Is there any way that keeps a reasonable interface?

- think about include of nytl headers. Should really the namespace be replaced? header guards?
	projects using nytl outside of vpp should be able to do so (and use it for vpp calls).
	[stringParam, range, nonCopyable in utility]

- custom exception? like vpp::VulkanError if vulkan call (caught exception or result) failed or
	vpp::QueueError if there is not queue that can execute the needed operation

- External constructors for all resources (construct them from existing handles)
