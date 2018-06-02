Todo list vor vpp
=================

- fix TrackedDescriptor/SubBuffer swap (union)
- improve BufferAllocator/SharedBuffer/DescriptorAllocator algorithms
- add glfw/sdl examples (option to use sdl from meson wrap db)
- correct syncs, pipeline barriers support
	- when a SharedBuffer range is reallocated, does there have to be
	  a pipeline barrier?
- one_device: store device in Device, not Resource.
  Make sure it can be reset (after destruction) e.g. for device lost or
  multiple devices in sequence + example/test
  (create multiple devices and resources)
- make codestyle consistent everywhere
	- mainly old sources using old styles (devcice.cpp etc)
- clean up usage of nytl
	- just include it as subproject?
- handle problem: memBits (in buffer/sharedBuffer/image) not compatible
  with buffer requirements
  	- solution: might require to be checked/handled by user
  		- then: does sharedBuffer (the hostCoherent checking) implement
  		  it correctly?
- memoryMap: remap smaller range when a certain range is no longer needed?
- imageOps: really allow extent with depth == 0? also handle it for height == 0?
- device: cache supported extensions (see e.g. defaults.cpp: could change
  format querying behavior)
- write basic docs

- improve debug.hpp
	- remove enums.hpp from debug.hpp
		- probably best to implement the handle type switching in some other
		  way; in source file
	- use the new debug extension (debug_utils)?
		- how to handle vulkan 1.1? require it at some point?

low prio / general / ideas
--------------------------

- vpp: better descriptor update overloads.
  e.g. `uniform(nytl::Span<const BufferRange>);`
- add overloads to SubBuffer/TrDs that don't take a <>allocator and just
  use the default one
- BufferAllocator optimize/shrink
- look into simplyfying complicated offset mechanisms (mainly MappedBufferWriter)
  in bufferOps
  	- also: maybe best to not put (even if only couple of lines) the whole
  	  stage buffer allocation handling in header?
- work dependency chaining (-> QueueSubmitter semaphores)
	- would allow to e.g. let Renderer submission depend on
	  (staging, so cmdBuf-based) buffer updates
- offer functionality to select supported extensions/layers from a list
- renderer: rework/remove RecordMode::all. Any way to get around the invalidate
  extra condition? Or at least rather use onDemand mode by default.
  With RecordMode::all we could record all command buffers at once though,
  implement possibility for that, i.e. record member function gets multiple
  command buffers to record at once. Could be more efficient to do for some
  engines (and they can still fall back to separate recording).
- GraphicsPipelineInfo: could provide conversion operator
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
	- also test coherent atom handling in SharedBuffer
- imageOps: fill: option to use direct update instead of mapping?
	- probably not worth it, we have to allocate buffer either way...
	- or is there a way to directly update deviceLocal image?
- respect optimalBufferCopyRowPitchAlignment somehow
	- retrieve: probably not possible if we want to guarantee tightly packed data
	- we could maybe use it when uploading data
- support for checking max available vs used memory
- support for allocation strategies (mainly DeviceMemoryAllocator),
  support massive preallocations (like allocate pretty much the whole device memory)
	- allow to explicity allocate memory on a given memory allocator.
	  To create large (like over 100 mb) buffers of a memory type we know we will need
	  Also something like an additional allocation strategy?
	  Allocate more than needed if the user wants it
- make sure device limits are correct in vpp
	- are there any device limits we currently don't respect?
- more utility for checking device limits?
	- differentiate: assumptions and tests/checks
- physicalDevice: add overload that take already queried physical dev properties
- fill operations: take std::byte& instead of span?
	- probably not a good idea, see issue with memorySize(). We should not use it
- dataWork::data: use non-rvalue & modifier?
	- to disallow something like ```data = retrieve(...)->data```
	- has multiple problems: sometimes it's probably valid to do this.
- bufferOps: add raw fill/retrieve calls?
- rework commandBuffer
	- don't make commandPools store information
- example vulkanType impl for nytl and glm
- maybe expose BufferOperator as independent header?
	- especially BufferSizer, constexpr neededBufferSize
		- maybe even useful in gl
- procAddr: test if local cache really faster than load it every time?
- add sync methods to retrieve/fill/transferWork utility
	- semaphore for submitting work
- cache hostVisible/deviceLocal bits in device?
- rather use 'operator const Handle&()' for resources (also device/queue)?
	- also: make conversion explicit? any possible problems with having them
	  implicit?
- improve memory/buffer allocation algorithm
- support for sparse stuff. Without making non-sparse buffers/images more expensive
- good idea to make everything threadlocal by default? hidden costs
	- we synchronize things that might not need it, implicit
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
- config: vpp_debug vs vpp_ndebug rather messy now
	- configurable from build system?
- work dependencies
	- make it possible (in some way) for work objects to depend on each other
	- the work objects itself will figure out how to do it (e.g. by fence or
		semaphore synchronization, or by simply waiting on the work before
		submitting or in which step ever)
- which information should resources carry around, which not?
- general initializer
	- something about descriptors and descriptor pools
	- think about buffer/image providers (better not)
- display class for vkDisplayKHR extension
	- scope of vpp?
	- must wait until supported somewhere, for tests
- queue constness? (maybe make it related to any operations on the queue?)
- codegen constexpr?
- write deviceLost handling code snippet example
- shader stage construction in place?
	- make it easier to create shader modules that are only once used directly inside
		a ShaderProgram. Is there any way that keeps a reasonable interface?

- further custom exception? like vpp::QueueError if there is no queue that
	can execute the needed operation
	- think about some functionality to handle device lost (how to deal with it?)

- External constructors for all resources (construct them from existing handles)
- further examples/snippets/documentation
- better best physical device querying (vpp/physicalDevice.cpp)

- alternative two step init: safer, easier to use
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

### From sharedbuffer:

// NOTE: it might be useful to go even one step further and add a new
//  SharedBuffer class that makes use of sparse memory bindings and
//  only binds those regions that are currently used by a BufferRange.
//  Will have different costs than this concept (and this is fine
//  in most cases, also sparse stuff is only sparingly supported)
//  so keep this separate.

// NOTE: also implement synchronized versions of these classes?
//  might be useful since often there are multiple threads e.g.
//  uploading or initializing stuff. But again, implement as
//  a separate version, only pay for what you use.

