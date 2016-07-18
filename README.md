# vpp
vpp is a vulkan abstraction library written in modern C++14 and licensed by the __MIT License__.
It does not aim to be a full-sized graphics engine, it rather focuses on providing some useful
features that make programming vulkan applications in c++ more convinient and less repetitive.

vpp also includes a c++ codegenerator for the vulkan api which focuses on typesafety, less typing
while still being more explicit as well as it tries to not introduce a huge compile time overhead.
There should be no runtime overhead using the c++ api.

At the moment, vpp is in an experimental pre-alpha state, but the first (usable) alpha is
expected to be released soon.

__Any contributions (feature requests, critics and recommendations as well) are highly appreciated.
If you want to improve the library or add your own ideas, just start a pull request.__

## Examples
Here are some examples to give you an idea on how the api will probably look like and in
which way in may be useful to programming vulkan applications.
Examples as well as a full documentation will be provided at some first release of a stable api.
Note that those examples just represent one part of the library (the highest abstraction
level) while there are also other not-shown parts of the library. The code is more some
kind of concept, it just exists to show how one would do certain things with vpp.
Some parts of the examples are more concepts and not yet fully implemented in the library!

### Contexts
If you just want a vulkan context you can play around with and do not care about the exact
details of creation, you can just use a vpp::Context which will create a vulkan
instance, device, surface and swapchain for a given backend.

`````````````````````````cpp
//Let vpp query the backend and create a context for it.
//Normally you would pass some createInfo as 2nd param wich describes e.g. which
//queues/extensions/layers and
//debug information you want, we just use the defaults here.
//In this case we e.g. create a win32 context by using the win32 createContext overload.
vpp::Context context = vpp::createContext(hwnd, {/* default settings */});

//Context is basically just a all-in-one wrapper for the objects needed to render with vulkan
//If you want fine-grained control over these objects, the Context class is not what you want.
auto& dev = context->device();
auto& ini = context->instance();
auto& swapChain = context->swapChain();
auto& surface = constext->surface();

//create a render pass matching your needs
auto renderPass = createRenderPass();

//To render somethin now, you could e.g. use the vpp::SwapChainRenderer which requires you to
//build your render command buffers, but manages all the rest for you.
class MyRendererBuilder : public vpp::RendererBuilder
{
	//Build the render commands for the given render pass instance.
	virtual void build(const vpp::RenderPassInstance& renderini) const override
	{
		auto cmdBuffer = renderini.vkCommandBuffer();

		vk::cmdBindPipeline(cmdBuffer, vk::PipelineBindPoint::Graphics, <some pipeline>);
		vk::cmdBindDescriptorSets(cmdBuffer, vk::PipelineBindPoint::Graphics, <some descriptors>);
		vk::cmdBindVertexBuffers(cmdBuffer, <some vertex buffers>);
		vk::cmdDraw(cmdBuffer, <some draw params>);
	}

	//Clear the color attachment in black and the depth attachment with value 1.
	virtual std::vector<vk::ClearValues> clearValues() const override
	{
		std::vector<vk::ClearValue> ret(2);
		ret[0].color(std::array<float, 4>{{0.f, 0.f, 0.f, 1.f}});
		ret[1].depthStencil({1.f, 0});
		return ret;
	}
} builderObject;

//The SwapChain renderer can be easily constructed and used with the builder implemention from above.
//We simply pass him the swapChain to render on, the builder implementation, the created render pass
//and the vulkan present queue the context automatically creates.
vpp::SwapChainRenderer renderer(swapChain, builderObject, {renderPass, context->presentQueue()});

//This function will render one frame and present it.
//Usually before rendering there would be some kind of (backend-specific) event handling,
//which would cause the application to quit (break the loop) if the window is closed, and a resize
//of the swapchain (which would result in the need to recreate the SwapChainRenderer) if the
//window is resized. Here, we just render.
while(true) renderer.render();
```````````````````````

The example above does bascially picture the structure of a simple vulkan app using vpp.
If you would rewrite this using just plain vulkan calls, it would have probably
more than 10x the size of this.

### Two Step Initialization
vpp was mainly built for high-perfomance vulkan apps therefore it abstracts the vulkan api while still
making use of its explicitness. One of the main concepts of vpp is two step initialization which can
bring huge performance gains in comparison to naive-written vulkan or opengl apps.

The main idea is to batch the heavy work such as allocating device
resource or executing commands on the device together.
First, as many host objects as needed will be __created__ (which will make them
request the resources and queue the work) and then all the objects will be __initialized__
which means that the requested resources will be allocated, and the work that has to be
done will be submitted all together.

This breaks with RAII and therefore the Initializer template class can be used to initialize
objects in a safer and less error-prone way.

Example using the highest api abstraction:

First create multiple objects
``````````````````````cpp
using namespace vpp;

//dev is a vulkan device in this case, manually created or retrieved by a context
Initializer<Buffer> initBuf1(dev, {{}, 1024 * 2000, vk::BufferUsageBits::vertex});
Initializer<Buffer> initBuf2(dev, {{}, 1024 * 48, vk::BufferUsageBits::index});
Initializer<Buffer> initBuf3(dev, {{}, 96, vk::BufferUsageBits::uniform},
	vk::MemoryPropertyBits::hostVisible);

Initializer<ViewableImage> initImg(dev, {{}, vk::ImageType::e2d, someFormat, {1000, 1000}});
Initializer<FrameBuffer> initFB(dev, {3840, 2160}, {colorImgInfo});
``````````````````

The object initializer will only return the created object to use them when you make sure
they are fully initialized. After this you can normally use them.
Note that initializing objects may need paramters as well.

```````````````cpp
auto buffer1 = initBuf1.init();
auto buffer2 = initBuf2.init();
auto buffer3 = initBuf3.init();
auto image = initImg.init();
auto framebuffer = initFB.init(renderPass, {colorViewInfo});
````````````

The point of this whole initialization process was, that the created objects internally were able to
batch the needed device resources together and (if possible) there was only one memory allocation made
for all of them.

You can use two step initialization without the Initializer template as well
(useful e.g. for class members) but the initializer wrapper makes sure you only really
use fully initialized resources, which makes it safer.

You can as well fully initialize the object directly on destruction by calling some other
constructor than the default one, but this might be less efficient if you are initializing multiple
objects (especially device memory resources as images and buffers).

### Buffers and Images
vpp uses this idiom not only to initialize resources but also to execute other work.
This example shows how to fill or retrieve the data from buffers and images asynchronously.

``````````````````cpp
//Easily fill multiple data segments into buffers and retrieve the async work objects.

//Fill buffer1 with the 32 bit int and the data of a vector (will extract it correctly)
std::uint32_t a = 420;
std::vector<std::int32_t> b = {1, 4, 2, 5, 6, 3, 2, 4, 3};
auto work1 = bufferA.fill({{a}, {b}});


//Fill buffer2 with the float[4] array, and two floats
//The extra number (4) in the last data segment signals that it should have an
//offset of 4 to the previous segment.
float[4] c = {1.0, 7.7, 4.9, 2.9};
auto work2 = bufferB.fill({c}, {34.f}, {45.f, 4}});


//In this case the buffer is just filles using a pointer to data and the size of the data.
void* dData = ...;
std::size_t dSize = ...;
auto work3 = bufferC.fill({{0.f}, {dData, dSize}});


//Retrieve the data from a buffer
auto work4 = bufferD.retrieve();


//If we now want to make sure all the needed work is done, we can simply wait for it to finish.
//Usually one wants to do this later, when there is no more work for the cpu to do.
work1->finish();
work2->finish();
work3->finish();
work4->finish();


//Alternatively, if there is lots of setup to do and one does not want to care about all work objects
//seperatly, it can simply give them to a work manager and then wait for the work manager at the end
//of setup.
WorkManager workManager;
workManager.add({std::move(work1), std::move(work2), std::move(work3), std::move(work4)});

//...
//later on
workManager.finish();


//Now retrieve the data of the last buffer
std::uint8_t& data = work4->data();
`````````````````

Note that here again will usually just one batch of work (CommandBuffers) be submitted to the
vulkan device which may bring huge performance gains since this is considered a heavy operation.

All the examples here are using the highest level of abstraction of vpp (vpp in
NOT an engine, so there will be no abstraction like Mesh, Material or Camera), but the api
can also be accessed on a lower level of abstraction using e.g. the DeviceMemory,
SubmitManager, Device or SwapChain.


## Building
__vpp does not have any external dependencies (excluding vulkan headers and loader lib of course).__
It does require a solid C++14 compiler though which means that it is (at the moment) not
buildable with MSVC, but it works with newer clang versions and gcc 5/6.

The main features that would be required in MSVC to make compiling vpp possible are NSDMIs
for aggregates (uniform initialization of aggregates with default members) and extented constexpr
support. Both features are part of the C++14 standard.

At the moment, vpp does compile a small part of boost (boost::container::pmr) within its own
library for more efficient host memory allocations, but since this feature will part of the
C++17 standard, this will then no longer be needed (and note that you do not have to
have boost installed, the needed files will be compiled together with the vpp source files).
Patches that do not decrease vpp code quality but make it msvc 2015 conformant are highly
appreciated.

Just download the repository or clone it with git, run cmake and then build it
(with make/ninja/...).
For the newest changes and fixes, check out the dev git branch, which might be broken (i.e.
not compiling or not running) at times.

### Troubleshooting

Compiling:
* when installing (e.g. make install) make sure you have the needed rights for the install folder, on
unix you may need to run the command as root and on windows you might have to execute cmd as admin.
* to find the vulkan library, the build system guesses depending on the platform or uses the
VK_SDK_PATH environment variable, so if it fails to find the vulkan header or library make
sure the variable is correctly set.
* to use vpp in your own appliation, make sure that you pass the include/ directory of your install
folder as include dir, the lib/ directory as link dir and link against vpp (libvpp.so or libvpp.dll).
Additionally the vpp dynamic library (.so or .dll) must be located in the same directory as your
executable or in the runtime path of your environment.

For further help or to report any problems (there are probably some at the moment) just open an issue
at this github page.
