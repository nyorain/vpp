add_executable(vpptest main.cpp)

target_link_libraries(vpptest
    vpp
    "C:/VulkanSDK/1.0.3.1/Source/lib/vulkan-1.dll"
    "gdi32"
    "kernel32")
