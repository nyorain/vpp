#default sources
set(vpp-sources
	allocator.cpp
    context.cpp
    surface.cpp
    swapChain.cpp
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
	transfer.cpp
	work.cpp
	provider.cpp)

#add backends
if(${VPP_WINDOWS})
	set(vpp-sources ${vpp-sources} backend/win32.cpp)
elseif(UNIX)
	set(vpp-sources ${vpp-sources} backend/xcb.cpp backend/wayland.cpp)
endif()

#library
add_library(vpp SHARED ${vpp-sources})

#link to vulkan
target_link_libraries(vpp ${vulkan-lib})

#install
install(TARGETS vpp
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
		ARCHIVE DESTINATION lib)
