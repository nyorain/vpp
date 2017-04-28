vpp
===

A vulkan abstraction library written in modern C++ and licensed by
the __Boost License__ (similar to MIT License).

Does not aim to be a full-sized graphics engine, but rather focuses on
providing some useful features that make programming vulkan applications in
C++ more convenient and less repetitive while not introducing unreasonable overhead.

Includes a C++ code generator for the vulkan api which focuses on typesafety and less typing
while still being more explicit as well as it tries to not introduce a huge compile time overhead.
There should be no runtime overhead using the generated C++ vulkan api.

At the moment, vpp is still in an alpha state, its first unstable release
is __[v0.1-alpha](https://github.com/nyorain/vpp/releases)__

If you want to improve the library or add your own ideas, just start a pull request.
Any contributions (feature requests, critics and recommendations as well)
are highly appreciated.

## Examples

__A good place to start with vpp is the [hello world triangle
example](doc/examples/intro.cpp)__

In addition, here are some smaller code snippets to give you an idea on how the
api will probably look like and in which way in may be useful to program vulkan applications.
Examples as well as a full documentation will be provided at some first release of a stable api.

### Initialization

The snippet below (taken from the intro example) shows how vpp can be used for easier
intializing vulkan resources. Using plain vulkan and C to initialize instance, device,
debugCallback and swapchain needs usually way more (repetitive) lines of code without
expressing any interesting information (note that vpp always offer additional ways for
configuration).

```cpp
// the needed vulkan extensions and debug layer
constexpr char* iniExts[] = {
	VK_KHR_SURFACE_EXTENSION_NAME,
	VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
	VK_EXT_DEBUG_REPORT_EXTENSION_NAME
};

constexpr auto layer = "VK_LAYER_LUNARG_standard_validation";
constexpr auto extCount = sizeof(iniExts) / sizeof(iniExts[0]);

// first, create a vulkan instance
// with all needed extensions and the default debug layers enabled
vk::ApplicationInfo appInfo ("intro", 1, "vpp", 1, VK_API_VERSION_1_0);

vk::InstanceCreateInfo iniInfo;
iniInfo.pApplicationInfo = &appInfo;
iniInfo.enabledExtensionCount = extCount;
iniInfo.ppEnabledExtensionNames = iniExtensions;
instanceInfo.enabledLayerCount = 1;
instanceInfo.ppEnabledLayerNames = &layer;

vpp::Instance instance(iniInfo);

// create a debug callback for our instance and the default layers
// the default implementation will just output to std::cerr
vpp::DebugCallback debugCallback(instance);

// now create a device for the instance and a SurfaceKHR.
// vpp will take care of basic queue and physical device choosing
// We retrieve the queue to present on our surface
const vpp::Queue* presentQueue;
vpp::Device device(instance, vkSurface, presentQueue);

// now we can create a vulkan swapchain
// again, we just use the fast way and let vpp choose sane defaults
vpp::Swapchain swapchain(device, window.surface);
```

### Work batches

The library was mainly built for high-performance vulkan apps therefore it abstracts the
vulkan api while still making use of its explicitness. One of the main concepts of vpp is two step
initialization which can bring huge performance gains in comparison to
naive-written vulkan or opengl apps.

The main idea is to batch the heavy work such as allocating device resource or executing commands
on the device together.
First, as many host objects as needed will be __created__ (which will make them
request the resources and queue the work) and then all the objects will be __initialized__
which means that the requested resources will be allocated, and the work that has to be
done will be submitted all together.

The example below creates multiple vulkan memory resources.
If possible, all of them will automatically be allocated on the same device memory allocation.
This is possible since creating them won't automatically bind them to memory.
Only when they are
used (i.e. vpp::Buffer::assureMemory is called e.g. by filling them), they will
be bound to memory. This defered two-step initialization idiom will improve
performance and the needed number of allocations.

```cpp
vpp::Buffer a(dev, {{}, 1024 * 2000, vk::BufferUsageBits::vertex});
vpp::Buffer b(dev, {{}, 1024 * 48, vk::BufferUsageBits::index});
vpp::Buffer c(dev, {{}, 96, vk::BufferUsageBits::uniform},
```

#### vpp::Work

This idiom is extended beyond initialization to general work.
This example shows how to fill or retrieve the data from buffers and images asynchronously.
There will only be one call to vk::queueSubmit since all command buffers will be batched
together. It also shows the utility vpp provides to easily fill/read images and buffers.

```cpp
// Fill a buffer with the 32 bit int and the data of a
// vector (will extract it correctly) using std140 layout.
std::uint32_t a = 420;
std::vector<std::int32_t> b = {1, 4, 2, 5, 6, 3, 2, 4, 3};
auto work1 = fill140(buffer1, a, b);

// In this case the buffer is just filled the buffer with a raw
// copy of the given type.
MyPodStructData data;
auto work2 = fill140(buffer2, 0.f, vpp::raw(data));

// Read the aligned data using std430 layout into the given variables.
// One can also specify how custom types (e.g. when using glm) should
// be written or read.
float d;
Vec2 e;
std::vector<std::uint32_t> f(200);
auto work3 = retrieve430(buffer3, d, e, f);

// We can now control and receive the state of the work.
// For more information see the vpp::Work class template.
work1->finish();
work2->finish();
work3->finish();

// Alternatively, if there is lots of setup to do and one does
// not want to care about all work objects seperatly, one can simply
// give them to a work manager and then wait for the
// work manager at the end of setup.
WorkManager workManager;
workManager.add({std::move(work1), std::move(work2), std::move(work3)});

// The workManager would automatically finish the owned work on destruction
// or you can call it manually.
workManager.finish();
```

Submitting just one batch of command buffers to the vulkan device for all operations
above can bring massive performance gains, since submitting a queue is considered
a heavy operation.

Good starting points for the vpp documentation are
[vpp::Work](include/vpp/work.hpp), [vpp::Device](include/vpp/device.hpp) or
[the bufferOps header](include/vpp/bufferOps.hpp).

All the examples here are using the highest level of abstraction of vpp (vpp is
NOT an engine, so there will be no abstraction like Mesh, Material or Camera), but the api
can also be accessed on a lower level of abstraction using e.g.
[DeviceMemory](include/vpp/memory.hpp),
[SubmitManager](include/vpp/submit.hpp), or
[DeviceMemoryAllocator](include/vpp/allocator.hpp).

## Building
__vpp does not have any external dependencies (excluding vulkan headers and loader lib of course).__
Since it already uses C++17 it compiles (at the moment) only on the latest gcc and clang
builds.

Just download the repository or clone it with git, run cmake in a
new build folder and then build it (e.g. using make or ninja with gcc/clang).

### Troubleshooting

Compiling:
* to find the vulkan library, the build system guesses depending on the platform or uses the
VK_SDK_PATH environment variable, so if it fails to find the vulkan header or library make
sure the variable is correctly set.
* to use vpp in your own appliation, make sure that you pass the include/ directory of your install
folder as include dir, the lib/ directory as link dir and link
against vpp (libvpp.so or libvpp.dll). Additionally the vpp dynamic library (.so or .dll)
must be located in the same directory as your executable or in the runtime path
of your environment.
* Make sure your have the latest vulkan sdk path installed, otherwise the generated C++ api will
not be compatible with the installed plain vulkan api.

For further help or to report any problems (there are probably some at the moment) just
open an issue at the [github page](https://github.com/nyorain/vpp)
