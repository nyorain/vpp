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
	renderPass.cpp
	commandBuffer.cpp
	submit.cpp
	transfer.cpp
	work.cpp
	provider.cpp)

if(WIN32 OR MSYS OR MINGW)
	set(VPP_WINDOWS 1)
else()
	set(VPP_WINDOWS 0)
endif()

if(${VPP_WINDOWS})
	set(vpp-sources ${vpp-sources} backend/win32.cpp)
elseif(UNIX)
	set(vpp-sources ${vpp-sources} backend/xcb.cpp backend/wayland.cpp)
endif()

add_library(vpp SHARED ${vpp-sources})

if(${VPP_WINDOWS})
	target_link_libraries(vpp "$ENV{VK_SDK_PATH}/Source/lib/vulkan-1.dll")
elseif()
	target_link_libraries(vpp "vulkan")
endif()

install(TARGETS vpp
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
		ARCHIVE DESTINATION lib)
