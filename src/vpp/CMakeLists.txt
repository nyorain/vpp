include_directories("$ENV{VK_SDK_PATH}/Include")

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
	renderPass.cpp)


if(WIN32)
	set(vpp-sources ${vpp-sources} backend/win32.cpp)
endif(WIN32)

if(UNIX)
	set(vpp-sources ${vpp-sources} backend/xcb.cpp backend/wayland.cpp)
endif(UNIX)

add_library(vpp SHARED ${vpp-sources})

if(WIN32)
	target_link_libraries(vpp "$ENV{VK_SDK_PATH}/Source/lib/vulkan-1.dll")
elseif(UNIX)
	target_link_libraries(vpp "vulkan")
endif(WIN32)


install(TARGETS vpp
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
		ARCHIVE DESTINATION lib)
