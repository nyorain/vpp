add_library(vpp SHARED
    context.cpp
    surface.cpp
    swapChain.cpp
    procAddr.cpp
	renderer.cpp
	shader.cpp
    backend/win32.cpp)

target_link_libraries(vpp "$ENV{VK_SDK_PATH}/Source/lib/vulkan-1.dll")
