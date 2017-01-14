#default sources
set(vpp-sources
	allocator.cpp
	buffer.cpp
	bufferOps.cpp
	context.cpp
	device.cpp
	descriptor.cpp
	procAddr.cpp
	renderer.cpp
	memory.cpp
	allocator.cpp
	shader.cpp
	framebuffer.cpp
	image.cpp
	instance.cpp
	debug.cpp
	pipeline.cpp
	graphicsPipeline.cpp
	computePipeline.cpp
	renderPass.cpp
	resource.cpp
	commandBuffer.cpp
	submit.cpp
	surface.cpp
	swapChain.cpp
	transfer.cpp
	work.cpp
	queue.cpp
	provider.cpp
	util/file.cpp)

#add backends
if(WIN32 OR MSYS OR MINGW)
	set(vpp-sources ${vpp-sources} backend/win32.cpp)
elseif(UNIX)
	set(vpp-sources ${vpp-sources} backend/xcb.cpp backend/wayland.cpp backend/xlib.cpp)
endif()

#library
add_library(vpp SHARED ${vpp-sources})
add_dependencies(vpp vkHeaders)

#link to vulkan
target_link_libraries(vpp ${Vulkan_LIBRARY})
target_include_directories(vpp PUBLIC ${Vulkan_INCLUDE_DIR})

#install
install(TARGETS vpp
		RUNTIME DESTINATION bin
		LIBRARY DESTINATION lib
		ARCHIVE DESTINATION lib)
