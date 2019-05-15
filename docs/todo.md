Todo list vor vpp
=================

- add glfw/sdl examples (option to use sdl from meson wrap db)

important, later:
- when vkpp has error handling update: fix try/catch/swallowed error
  in swapchain acquire/present and formats.cpp (get format properties)
- improve DeviceMemoryAllocator (cache data structures for fewer allocations,
  improve allocation algorithm)
- allow compressed formats in image ops.
  we can also allow compression on the fly, vkCmdCopyImage supports it
  might be too complex to do/not a nice abstract, make sure it stays
  inside vpp scope
  Shouldn't be too complicated except that we might have to adjust
  the address calculation in fill/retrieve Map and make sure to
  align correctly (in staging). See vulkan spec, has nice
  address/requirements definitions
  	- can we provide blitting utility?
	  maybe just move the initializon from stage here and fix it up
	  for spec correctness
- honor optimalBufferCopyRowPitchAlignment somehow
	- retrieve: probably not possible if we want to guarantee tightly packed data
	- we could maybe use it when uploading data, just pass as alignment
	  to the stage buffer
- when using c++20, switch to std::span (nytl::Span should be fully compatble
  at the moment) and completely abolish using the nytl namespace.
  import NonCopyable/NonMovable into vpp namespace, maybe even completely 
  remove all nytl traces from it. 

low prio / general / ideas
--------------------------

- vpp::PipelineLayout(dev, {}) gives you a pipeline layout with
  an empty handle, while
  vpp::PipelineLayout(dev, vk::PipelineLayoutCreateInfo {}) gives you
  a pipeline layout constructed with default, no/empty layouts.
  Is that expected behavior for users?
- allow to merge allocators
	- started implementing it, see node 621. Problem is though that
	  this requires to move non movable pool resources (TrDsPool,
	  DeviceMemory, SharedBuffer). We could fix it by always storing
	  those wrapped in unique ptrs (instead of using a deque)
	  but i'm not sure that's worth it. Probably no other way though
- optimiziation: currently some smarter resoures deriving handles
  (like TrDs or Buffer, Image) store the Device word in addition
  to a Resource they reference (like DeviceMemory or TrDsPool).
  Could be optimized somehow i guess? but not critical, if users care about
  that word they can always use the one device optimization.
  Similar situation for CommandBuffer, could reference CommandPool instead
  of pool + device (would make CommandPool non movable though, guess that's
  acceptable though)
- device: cache supported extensions?
- DescriptorUpdate: any chance to avoid memory allocation? maybe just
  provide another mechanism for more convenient update definition like
  vpp::descriptorBinding; allow to group them somehow
- improve BufferAllocator/SharedBuffer/DescriptorAllocator algorithms
	- general descriptor algorithms
- add debug barrier from vulkan sync wiki
- stalling commands (in vpp::stall namespace) that are useful
  for debugging/temporary workarounds?
	- clearly mark them as inefficient and not good for production code
	  though... not sure if worth it
- BufferAllocator optimize/shrink
	- basic defragmentation?
- offer functionality to select supported extensions/layers from a list
- renderer: rework/remove RecordMode::all. Any way to get around the invalidate
  extra condition? Or at least rather use onDemand mode by default.
  With RecordMode::all we could record all command buffers at once though,
  implement possibility for that, i.e. record member function gets multiple
  command buffers to record at once. Could be more efficient to do for some
  engines (and they can still fall back to separate recording).
  	- also see the stage workaround for avoiding initial record, maybe
	  allow extra parameter?
- GraphicsPipelineInfo: provide conversion operator?
  if not, document why
- add more assertions everywhere where things are assumed
	- don't overdo, only if potentially useful when debugging
	- see work.inl (something like tryFinish probably best)
- more general testing
	- test NonOwned<T>
	- maybe also automate visual testing (surface,swapchain,renderer?)
	- make sure to really test all classes and functions
		- physicalDevice
		- queue
		- image constructors (simply copy from buffer constructors in objects.cpp)
		- etc...
- support for checking max available vs used memory
	- see vulkan extension
- support for allocation strategies (mainly DeviceMemoryAllocator),
  support massive preallocations (like allocate pretty much the whole device memory)
	- allow to explicity allocate memory on a given memory allocator.
	  To create large (like over 100 mb) buffers of a memory type we know we will need
	  Also something like an additional allocation strategy?
	  Allocate more than needed if the user wants it
- more utility for checking device limits?
	- differentiate: assumptions and tests/checks
- make sure device limits are correct in vpp
	- are there any device limits we currently don't respect?
- physicalDevice: add overload that take already queried physical dev properties
- cache hostVisible/deviceLocal bits in device?
- support for sparse stuff. Without making non-sparse buffers/images more expensive
- use using declarations in the derived resource classes to make the
	protected ResourceHandle constructors visisble
- pmr for performance critical functions.
	- Device to store a thread-specific memory resource?
	- use it inside vpp for memory heavy operations (see DeviceMemoryAllocator)
- document the swap/move idiom used by resource and subclasses
- display class for vkDisplayKHR extension
- queue constness? (maybe make it related to any operations on the queue?)
- write deviceLost handling code snippet example
- shader stage construction in place? [probably not worth it, works alright atm]
	- make it easier to create shader modules that are only once used directly inside
		a ShaderProgram. Is there any way that keeps a reasonable interface?
- further custom exception? like vpp::QueueError if there is no queue that
	can execute the needed operation [probably not worth it]
	- think about some functionality to handle device lost (how to deal with it?)
- further examples/snippets/documentation
- better best physical device querying (vpp/physicalDevice.cpp)
