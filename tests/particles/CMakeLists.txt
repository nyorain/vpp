add_executable(particles particles.cpp)
target_link_libraries(particles vpp)

add_shader("particles.frag" particles)
add_shader("particles.vert" particles)
add_shader("particles.comp" particles)
