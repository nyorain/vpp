add_executable(font font.cpp)
target_link_libraries(font vpp)

add_shader("font.frag" font)
add_shader("font.vert" font)
