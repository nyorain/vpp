add_executable(font font.cpp)
target_include_directories(font PUBLIC ${Vulkan_INCLUDE_DIR})
target_link_libraries(font vpp ${Vulkan_LIBRARY})

add_shader("font.frag" font)
add_shader("font.vert" font)
