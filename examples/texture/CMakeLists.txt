add_executable(texture texture.cpp)
target_link_libraries(texture vpp)

add_shader("texture.frag" texture)
add_shader("texture.vert" texture)
