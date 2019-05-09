Todo list vor vpp
=================

- discuss: remove common `init` syntax for deferred initialization?
	- or could that be useful somehow?
- update to new vkpp version
- improve image ops
  - allow compressed formats. Some way to allow filling a compressed image with
	  uncompressed data? doing the compressing on the gpu?
  - support blitting
  - write tests
  - allow to only pass vk::image where it makes sense
- rmeove init.hpp? no useful in its current form
	- introduct new deferred initialization idom from stage?
	  better for vpp?
	- anyways, init.hpp was pretty much *never* used
- fix/rework physical device with needed utility from stage
- pack all simple RAII wrappers into vpp/handles.hpp?
- completely abandon ThreadLocalStorage?
	- when someone uses multiple threads, just use custom allocators
	  and stuff. They probably alreday have comparable solutions
- fix formats.cpp: throws on error (getPhysicalDeviceFormatProperties)
	- fixing the throw probably requires vkpp changes. Maybe two
	  function headers, one throwing and one not?
	  	- while at it, maybe provide wrappers for dynamically loaded
		  functions somehow? maybe add dynamic dispatcher to vpp::Device?
	- are the formats utility functions even useful though?
	- also fix the weird 'depth == 1 or depth == 0, whatever' semantics
- add glfw/sdl examples (option to use sdl from meson wrap db)
- correct syncs, pipeline barriers support
	- when a SharedBuffer range is reallocated, does there have to be
	  a pipeline barrier? should probably be left to user to synchronize
	  before destruction of SubBuffer, but document that somewhere!
- simplify/correct TrackedDescriptor/SubBuffer swap (union)
- one_device: store device in Device, not Resource.
  Make sure it can be reset (after destruction) e.g. for device lost or
  multiple devices in sequence + example/test
  (create multiple devices and resources)
  	- cleanup resource.hpp, ifdef confusing atm
	  maybe split the two implementations into two files
- make codestyle consistent everywhere
	- mainly old sources using old styles (devcice.cpp etc)
- clean up usage of nytl
	- just include it as subproject?
	  or remove it? c++20 has span
- handle problem: memBits (in buffer/sharedBuffer/image) not compatible
  with buffer requirements?
  	- solution: might require to be checked/handled by user
  		- then: does sharedBuffer (the hostCoherent checking) implement
  		  it correctly? probably not, we assume that all memory types
		  can be used by buffer (when we create a new one; alloc algorithm)
- imageOps: really allow extent with depth == 0? also handle it for height == 0?
	- fix consistency with formats.hpp
- device: cache supported extensions (see e.g. defaults.cpp: could change
  format querying behavior)
- write basic docs
- improve debug.hpp
	- remove enums.hpp from debug.hpp if possible
		- probably best to implement the handle type switching in some other
		  way; in source file
		  we could probably forward declare the template in the header
		  and the implement it for all supported types in the source
		  file. When one passes an unsupported handle, compiler generates
		  unresolved symbol
	- also allow to directly tag vpp raii wrappers. Maybe overload
	  that takes a ResourceHandle and simply forwards with vkHandle()?
	- use the new debug extension (debug_utils)?
		- how to handle vulkan 1.1? require it at some point?
		  vkpp has an optional dynamic dispatch layer, if an application
		  wants to support vulkan 1.0 they can always use that instead
		  of linking to 1.1 symbols. And yes, we probably can just
		  require 1.1 headers at some point
	- cache loaded debug marker functions? require them to be present
	  if used? we currently just silently fail, probably not expected
	- how does vpp expect applications to use them? do a manual check
	  if debug is supported every time? probably bad idea.
	  maybe silent fail is ok? or implement two functions, one that
	  expects functions to be avilalbe, the other one to silently (maybe
	  return bool) fail
- rework commandBuffer
	- don't make commandPools store information
	- split in "smart" pools and command buffers and simple raii handles
- procAddr: test if local cache really faster than load it every time?
	- in doubt: remove local cache.
	  see -> DynamicDispacher per vpp::Device
- is size value in MemoryEntry really needed?
	- completely abolish memory size?
- External constructors for all resources (construct them from existing handles)
- config: vpp_debug vs vpp_ndebug rather messy now
	- configurable from build system?
- update readme

low prio / general / ideas
--------------------------

- DescriptorUpdate: any chance to avoid memory allocation? maybe just
  provide another mechanism for more convenient update definition like
  vpp::descriptorBinding; allow to group them somehow
- memoryMap: remap smaller range when a certain range is no longer needed?
	- might otherwise have undefined behavior, mapping memory while used
	  on device is undefined, right? even if not used? read in spec
- improve BufferAllocator/SharedBuffer/DescriptorAllocator algorithms
	- general descriptor algorithms
- add debug barrier from vulkan sync wiki
- stalling commands (in vpp::stall namespace) that are useful
  for debugging/temporary workarounds?
	- clearly mark them as inefficient and not good for production code
	  though... not sure if worth it
- add overloads to SubBuffer/TrDs that don't take a <>allocator and just
  use the default one
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
- respect optimalBufferCopyRowPitchAlignment somehow
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
