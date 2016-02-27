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
	buffer.cpp
	image.cpp
	graphicsPipeline.cpp)


if(WIN32)
	set(vpp-sources ${vpp-sources} backend/win32.cpp)
endif(WIN32)

if(UNIX)
	set(vpp-sources ${vpp-sources} backend/xcb.cpp backend/wayland.cpp)
endif(UNIX)

add_library(vpp SHARED ${vpp-sources})
target_link_libraries(vpp "$ENV{VK_SDK_PATH}/Source/lib/vulkan-1.dll")
