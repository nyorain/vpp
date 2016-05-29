# vpp
vpp is a vulkan abstraction library written in modern C++14 and licensed by the __MIT License__. It does not aim to be a full-sized graphics engine, it rather focuses on providing some useful features that make programming vulkan applications in c++ more convinient and less repetitive.

At the moment this repository does baiscally just contain some ideas and mockups on how the later api could be looking, but is __not stable or ready to use__ in any way. Any contributions (feature requests or recommendations as well) are highly appreciated.

vpp does also include (a forked version of) nvidias [vkcpp] codegenerated c++ vulkan header which provides features like typesafe enums or vulkan function operating on std::vectors. 

## Examples
Some examples to give you an idea on how the api will probably look like and in which way in may be useful to programming vulkan applications. Examples as well as a full documentation will be provided at some first release of a stable api.

### DeviceMemoryAllocator
The DeviceMemoryAllocator can be used to allocate memory for multiple images/buffers.
Notice how on this example 2 buffers are really easily created, yet there will only be one vulkan device memory allocated.

````cpp
vpp::DeviceMemoryAllocator allocator(device);

vpp::Buffer vertexBuffer(allocator, vertexBufferCreateInfo, vk::MemoryPropertyFlagBits::HostVisible);
vpp::Buffer indexBuffer(allocator, indexBufferCreateInfo, vk::MemoryPropertyFlagBits::HostVisible);

//would also do this automatically on desctruction
allocator.allocate();

//after the memory was allocated the buffers are ready to fill and use
vertexBuffer.fill({{vertexData}});
indexBuffer.fill({{indexData}});
````

[vkcpp]: https://github.com/nvpro-pipeline/vkcpp
