Todo list vor vpp
=================

- when vkpp has error handling update: fix try/catch/swallowed error
  in swapchain acquire/present and formats.cpp (get format properties)

- update readme
- improve image ops
  - allow compressed formats. Some way to allow filling a compressed image with
	  uncompressed data? doing the compressing on the gpu?
  - support blitting
  - write tests
	- imageOps: really allow extent with depth == 0? also handle it for height == 0?
		- fix consistency with formats.hpp
- rmeove init.hpp? no useful in its current form
	- introduct new deferred initialization idom from stage?
	  better for vpp? there are some classes (e.g. TrDs) that have to use
	  additional reservation members only for that purpose
	- anyways, init.hpp was pretty much *never* used
- add glfw/sdl examples (option to use sdl from meson wrap db)
- handle problem: memBits (in buffer/sharedBuffer/image) not compatible
  with buffer requirements?
  	- solution: might require to be checked/handled by user
  		- then: does sharedBuffer (the hostCoherent checking) implement
  		  it correctly? probably not, we assume that all memory types
		  can be used by buffer (when we create a new one; alloc algorithm)
- write basic docs
- is size value in MemoryEntry really needed?
	- completely abolish memory size?

low prio / general / ideas
--------------------------

- device: cache supported extensions?
- when using c++20, switch to std::span (nytl::Span should be fully compatble
  at the moment) and completely abolish using the nytl namespace.
  import NonCopyable/NonMovable into vpp namespace, maybe even completely 
  remove all nytl traces from it. 
- discuss: remove common `init` syntax for deferred initialization?
	- or could that be useful somehow?
- DescriptorUpdate: any chance to avoid memory allocation? maybe just
  provide another mechanism for more convenient update definition like
  vpp::descriptorBinding; allow to group them somehow
- memoryMap: remap smaller range when a certain range is no longer needed?
	- might otherwise have undefined behavior, mapping memory while used
	  on device is undefined, right? even if not used? read in spec
	- but this will invalidate the pointer, no probably can't be done
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
- honor optimalBufferCopyRowPitchAlignment somehow
	- retrieve: probably not possible if we want to guarantee tightly packed data
	- we could maybe use it when uploading data
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
- is there a better way for the Resource::swap mess?
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

- docs

/// General documentation for vpp two-step-initiazation classes.
/// Constructors shall always either fully construct the object or be default constructors.
/// This way users of classes can always be sure that constructing the object with arguments
/// will initialize it wihtout having to look into some documentation.

/// For two-step-initiazation only the two member functions create(...) and init(...) will
/// be used on a default constructed object. Calling create() for an object that was
/// not default constructed and since then unchanged, calling init() for an object
/// on that create() was not called before, or calling one of the functions
/// more than one time is usually undefined behaviour.
/// It therefore might work but class writers are encouraged throw an exception in such
/// a case.
/// Both functions can be const but are not required to be so.
/// There might also be mutliple overloads of both functions, taking 0 or more arguments.
/// Classes should try to avoid redundant information in the both functions,
/// e.g. if both of them need certain information they should simply take it as paramter
/// in create and then store it (if this does not introduce an unacceptable overheat).

/// If one wants to re-two-step-initialized an already initialized object, it must first
/// move assign (or copy assign if available) with a default constructed object and then
/// call the two functions.
/// Using an uninitialized object will result in undefined behaviour. This time class
/// authors are explicitly encouraged to NOT check for this case, since that would result
/// in high overheads, so this will likely lead to a memory error e.g. when dereferencing
/// an nullptr.
/// Destructors (as an exception) should work for default constructed object as well,
/// i.e. they must not assume that the destructing object was ever valid.

/// Classes should generally avoid having something like a destroy method (and if, then protected).
/// In move operators the destructor can be directly called.
/// Usually classes implement a free friend swap function for themselves and then use it
/// for the move operator.
