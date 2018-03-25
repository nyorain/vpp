vpp
===

A vulkan abstraction and utility library written in modern C++.

Does not aim to be a framework or full-sized graphics engine, but rather focuses on
providing some useful features that make programming vulkan applications in
C++ more convenient and less repetitive while not introducing unreasonable overhead.

__Things vpp provides:__

- C++ vulkan api with all its advantages
	- while still keeping the compile time and LOC amount sane
- RAII classes for all handles
	- you only pay for what you use: no shared_ptr lock-in, no virtual abstraction
- memory management utilities
	- automatic yet efficient memory allocation (if you want)
	- you can still allocate/manage your own memory if you need to
		- e.g. vpp currently does not provide any sparse memory utility
- idioms for efficient vulkan use
	- Deferred initialization: allows to allocate as few resources as possible
	  and share them
	- SharedBuffer and BufferAllocator
	- sharing DeviceMemory, DeviceMemoryAllocator (grouping allocations)
	- Bundled command buffer submissions
- retrieving/filling buffers and images in various ways
	- utility for correctly filling std140, std430 buffers
- query valid image usage combinations
	- default image/imageView creation initializers
- various initialization helpers (querying properties)
	- Swapchain
	- Image and ImageView default createInfos
	- physical device querying
- vpp::Renderer: implementing swapchain renderbuffer handling and
  submission/presentation synchronization

__Things vpp does not provide:__

- Full renderer/graphical implementations
	- vpp will not provide something like a forward/deferred renderer imlpementation
	- no non-vulkan concepts like camera, transform, matrix or vector
- High-cost or non-generic abstractions like Texture/Model
- a Window abstraction (just use sdl/glfw or whatever you want)
	- vpp does not implement any platform-specific bits
- Examples for learning vulkan

At the moment, vpp is still in an early state, its latest unstable release
is __[v0.2](https://github.com/nyorain/vpp/releases)__
You can build it using meson. Linux (android as well) and Windows are
supported and tested. Needs a solid C++17 compiler, msvc does
currently not implement enough of the standard correctly.
MinGW-w64 works on windows.

Any contributions (feature requests, issues and recommendations as well)
are highly appreciated.

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
vpp::DebugCallback debugCallback(instance); // extension must be enabled

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
// Create a couple of memory resources deferred
// They are not fully initialized, not ready to be used
vpp::Buffer buffer1(vpp::defer, device, bufferCreateInfo1);
vpp::Buffer buffer2(vpp::defer, device, bufferCreateInfo2);
vpp::Image image1(vpp::defer, device, imaCreateInfo1);

// Finish deferred initialization for all resources
// If posssible (due to vulkan MemoryRequirements) will allocate all resources
// on a single device memory (respecting alignments etc)
buffer1.init();
buffer2.init();
image1.init();
```

#### SharedBuffer and BufferAllocator

```cpp
// Create a bufferAllocator (we could also use the device's thread-specific
// default allocator) and allocate a bunch of mappable uniform buffer objects.
// Since we reserve the needed storage, only one buffer will be created.
// Alternatively, we could have directly created one vpp::SharedBuffer.
vpp::BufferAllocator bufferAllocator(device);
bufferAllocator.reserve(true, 1024, vk::BufferUsageBits::uniformBuffer);
auto ubo1 = bufferAllocator.alloc(true, 64u, vk::BufferUsageBits::uniformBuffer);
auto ubo2 = bufferAllocator.alloc(true, 64u, vk::BufferUsageBits::uniformBuffer);
auto ubo3 = bufferAllocator.alloc(true, 64u, vk::BufferUsageBits::uniformBuffer);
auto ubo4 = bufferAllocator.alloc(true, 64u, vk::BufferUsageBits::uniformBuffer);
```

#### Aligned buffer reading/writing

```cpp
// Fill buffers in different ways using vpp.
// Will automatically apply alignment requirements for std140/std430.
// You could also use the oop api with more features (e.g. MappedBufferWriter)
// Note that you have to specialize the vpp::VulkanType template
// to make this work with your structs, vector or matrix types
vpp::writeMap140(ubo, 1.f, someVec2f, 5u); // ubo must be mappable
vpp::writeStaging140(vertexBuffer, verticesContainer); // use a staging buffer
vpp::writeStagin430(storageBuffer, vpp::raw(data, size));
vpp::writeDirect140(smallBuffer, 1.f, 2.f, 3.f); // direct update for few data

// You can also read a buffer respecting the layout/alignment
// will read the values set above
float float1, float2, float3;
/* your favorite vector type */ vec2f;
vpp::readMap140(ubo, float1, vec2f, float2);
auto work = vpp::readStaging140(smallBuffer, float1, float2, float3);

// will submit and wait for the command buffer
// will write float1,float2,float3
// can be used deferred to group multiple commands into one submission
work.wait();
```

#### Image filling/retrieving

```cpp
// You can also easily fill images. When using them, you should check the
// documentation at vpp/imageOps.hpp for requirements
vpp::fillMap(img1, size, dataToWrite, {vk::AspectBits::color});
vpp::fillStaging(img2, format, layout, size, dataToWrite, {vk::AspectBits::color});
auto work = vpp::retrieveStaging(img3, format, layout, size, {vk::AspectBits::color});
auto img3Data = auto work.data();
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
