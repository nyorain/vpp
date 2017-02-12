#default sources
set(vpp-sources
	allocator.cpp
	buffer.cpp
	bufferOps.cpp
	device.cpp
	descriptor.cpp
	procAddr.cpp
	renderer.cpp
	memory.cpp
	memoryMap.cpp
	shader.cpp
	framebuffer.cpp
	image.cpp
	instance.cpp
	debug.cpp
	pipeline.cpp
	physicalDevice.cpp
	renderPass.cpp
	resource.cpp
	commandBuffer.cpp
	submit.cpp
	surface.cpp
	swapchain.cpp
	sync.cpp
	transfer.cpp
	work.cpp
	queue.cpp
	util/file.cpp)

#library
add_library(vpp SHARED ${vpp-sources})

if(Codegen)
	add_dependencies(vpp vkHeaders)
endif()

#link to vulkan
target_link_libraries(vpp ${Vulkan_LIBRARY})
target_include_directories(vpp PUBLIC ${Vulkan_INCLUDE_DIR})

#install
install(TARGETS vpp
		RUNTIME DESTINATION bin
		LIBRARY DESTINATION lib
		ARCHIVE DESTINATION lib)
