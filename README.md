vpp
===

A vulkan abstraction and utility library written in modern C++.

Does not aim to be a framework or full-sized graphics engine, but rather focuses on
providing some useful features that make programming vulkan applications in
C++ more convenient and less repetitive while not introducing unreasonable overhead.
It does not force you to use shared pointers for all your resources,
automagically synchronizes pools or implicitly submits to the device.
Think of vpp more like an independently usable set of utilities.

__Things vpp provides:__

- C++ vulkan api with all its advantages
	- while still keeping the compile time and LOC amount sane
- RAII classes for all handles
	- you only pay for what you use: no shared_ptr lock-in, no virtual abstraction
- memory management utilities
	- automatic yet efficient memory allocation
	- you can still allocate/manage your own memory if you need to
		- e.g. vpp currently does not provide any sparse memory utility
- idioms for efficient vulkan use
	- Deferred initialization: allows to allocate as few resources as possible
	  and share them
	- SharedBuffer and BufferAllocator
	- sharing DeviceMemory, DeviceMemoryAllocator (grouping allocations)
	- batching command buffer submissions via QueueSubmitter
- some utility for retrieving/filling buffers and images
- query valid image usage combinations and format support
	- default image/imageView creation initializers
- various initialization helpers (querying properties)
	- Swapchain
	- Image and ImageView default createInfos
	- physical device querying
- vpp::Renderer: implementing swapchain renderbuffer handling and
  submission/presentation synchronization

__Things vpp does not provide:__

- full renderer/graphical implementations
	- vpp will not provide something like a forward/deferred renderer
	  imlpementation, specific render techniques or shaders
	- no non-vulkan concepts like camera, transform, matrix or vector
- high-cost or non-generic abstractions like Texture/Model
- a Window abstraction (just use sdl/glfw or whatever you want)
	- vpp does not implement any platform-specific bits
- examples for learning vulkan

You can build vpp using meson. Linux (android as well) and Windows are
supported and tested. Needs a solid C++17 compiler, msvc does
currently not implement enough of the standard correctly.
MinGW-w64 works on windows.

Any contributions (feature requests, issues and ideas as well) are appreciated.

# Examples

__[This example](docs/examples/intro_ny.cpp)__ implements the hello-world-triangle
using a cross-platform window abstraction and vpp.
Below a couple of idiomatic usages of the rather high-level interfaces.
You could also check out the [tests](docs/tests) to see basic usage
of those classes with expected results.
For more information on any of them, just check out the usually well-documented
[header files](include/vpp).

#### Initialization

```cpp
// Create instance and debugCallback
vk::InstanceCreateInfo iniInfo = {  ...  };
vpp::Instance instance(iniInfo);
vpp::DebugMessenger debugMessenger(instance); // debug_utils extension must be enabled

auto surface = /* use your favorite window abstraction to create a surface */

// Let vpp choose a default physical device, present, graphics and compute queue
// Note that while this usually works and is convenient at the beginning,
// you can still just choose your own physical device or pass
// your own DeviceCreateInfo with chosen queue families.
const vpp::Queue* presentQueue;
vpp::Device device(instance, surface, present);
```

#### Submission batches

```cpp
// vpp::QueueSubmitter allows to bundle multiple queue submissions
// and then track the submission state.
vpp::QueueSubmitter submitter(queue);
auto id = submitter.add({submitInfo1, submitInfo2, submitInfo3});
submitter.submit(id);
submitter.wait(id);
```

#### Deferred initialization

```cpp
// Create initializers for a couple of memory resources.
// They will already reserve as much memory as they need.
// The api can also be used manually (via vpp::Buffer/Image constructors
// and the init method), without the vpp::Init helper
vpp::Init<vpp::Buffer> initBuf1(device, bufferCreateInfo1);
vpp::Init<vpp::Buffer> initBuf2(device, bufferCreateInfo2);
vpp::Init<vpp::Image> iniImg1(device, imgCreateInfo1);

// Finish deferred initialization for all resources
// If posssible (due to vulkan MemoryRequirements) will allocate all resources
// on a single device memory (respecting alignments etc)
buffer1 = initBuf1.init();
buffer2 = initBuf2.init();
image1 = initImg1.init();
```

#### SharedBuffer and BufferAllocator

```cpp
// Create a bufferAllocator (we could also use the device's default allocator) 
// and allocate a bunch of mappable uniform buffer objects.
// Since we reserve the needed storage, only one buffer will be created.
// Alternatively, we could have directly created one vpp::SharedBuffer.
vpp::BufferAllocator bufAlloc(device);

auto hostBits = device.hostMemoryTypes(); // memory types that are hostVisible
auto usage = vk::BufferUsageBits::uniformBuffer;
auto uboSize = 64u;

bufAlloc.reserve(1024, usage, hostBits); // reserve enough for all ubos

vpp::SubBuffer ubo1(bufAlloc, 64u, usage, hostBits);
vpp::SubBuffer ubo2(bufAlloc, 64u, usage, hostBits);
vpp::SubBuffer ubo3(bufAlloc, 64u, usage, hostBits);
```

#### Image filling/retrieving

```cpp
// Some helpers to fill or read images.
auto subres = vk::ImageSubresource{vk::AspectBits::color};
vpp::fillMap(commandBuffer, img1, img1Size, dataToWrite, subres);

// Creates a stage buffer and copies from/to it via the given
// commandBuffer. The stage buffer must be kept alive until the
// command buffer has finished.
auto stageBuffer = vpp::fillStaging(commandBuffer, img2,
	img2Format, img2Layout, img2Size, dataToWrite, subres);
auto stageBuffer = vpp::retrieveStaging(commandBuffer, img3,
	img3Format, img3Layout, img3Size, subres);
```

#### Testing

To run all tests with valgrind leak checks, execute (in build dir):

```
meson test \
	--print-errorlogs \
	--wrapper 'valgrind
		--leak-check=full
		--error-exitcode=1
		--suppressions=../docs/valgrind.supp'
```
