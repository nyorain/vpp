- better renderer resetting (all command pools at once, just resize the frameRenderers vector)
- textures (overthing viewable image + sampler)
- fully implement examples!
- general initializer
	- something about descriptors and descriptor pools
	- think about buffer/image providers (better not)
- improve classes to use allocators and provider
- backend agnostic createContext function
- display class for vkDisplayKHR extension
- queue management
	- easier querying/creation/using for commands
- some smaller improvements
	- mapMemory: check valid usage (esp. alignment and stuff)
	- pmr for performance critical (every-frame) functions.

- think about include of nytl headers. Should really the namespace be replaced? header guards?
	projects using nytl outside of vpp should be able to do so (and use it for vpp calls).
	[stringParam, range, nonCopyable in utility]

- custom exception? like vpp::VulkanError if vulkan call (caught exception or result) failed or
	vpp::QueueError if there is not queue that can execute the needed operation
