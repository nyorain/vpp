add_executable(particles particles.cpp)
target_include_directories(particles PUBLIC ${Vulkan_INCLUDE_DIR})
target_link_libraries(particles vpp ${Vulkan_LIBRARY})

add_shader("particles.frag" particles)
add_shader("particles.vert" particles)
add_shader("particles.comp" particles)
