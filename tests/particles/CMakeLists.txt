add_executable(particles main.cpp particles.cpp)

target_include_directories(particles PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(particles
vpp "$ENV{VK_SDK_PATH}/Source/lib/vulkan-1.dll" gdi32)

add_shader("particles.frag" particles)
add_shader("particles.vert" particles)
add_shader("particles.comp" particles)
