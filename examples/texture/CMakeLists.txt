add_executable(texture texture.cpp)
target_include_directories(texture PUBLIC ${Vulkan_INCLUDE_DIR})
target_link_libraries(texture vpp ${Vulkan_LIBRARY})

add_shader("texture.frag" texture)
add_shader("texture.vert" texture)
