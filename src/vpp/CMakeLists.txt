#default sources
set(vpp-sources
	allocator.cpp
    context.cpp
    device.cpp
    descriptor.cpp
    procAddr.cpp
	renderer.cpp
	memory.cpp
	allocator.cpp
	shader.cpp
	device.cpp
	framebuffer.cpp
	buffer.cpp
	image.cpp
	debug.cpp
	pipeline.cpp
	graphicsPipeline.cpp
	computePipeline.cpp
	renderPass.cpp
	commandBuffer.cpp
	submit.cpp
    surface.cpp
    swapChain.cpp
	transfer.cpp
	work.cpp
	queue.cpp
	provider.cpp

	../../external/boost/src/global_resource.cpp
	../../external/boost/src/pool_resource.cpp
	../../external/boost/src/synchronized_pool_resource.cpp
	../../external/boost/src/unsynchronized_pool_resource.cpp
	../../external/boost/src/monotonic_buffer_resource.cpp
	../../external/boost/src/alloc_lib.c
	../../external/boost/src/dlmalloc.cpp

	utility/file.cpp)

#add backends
if(${VPP_WINDOWS})
	set(vpp-sources ${vpp-sources} backend/win32.cpp)
elseif(UNIX)
	set(vpp-sources ${vpp-sources} backend/xcb.cpp backend/wayland.cpp backend/xlib.cpp)
endif()

#library
add_library(vpp SHARED ${vpp-sources})
add_dependencies(vpp vkHeaders)

#link to vulkan
link_directories("${CMAKE_SOURCE_DIR}/external/lib")
target_link_libraries(vpp ${vulkan-lib})

#install
install(TARGETS vpp
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
		ARCHIVE DESTINATION lib)
