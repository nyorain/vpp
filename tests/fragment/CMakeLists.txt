add_executable(fragment fragment.cpp)
target_link_libraries(fragment vpp)

add_shader("fragment.frag" fragment)
add_shader("fragment.vert" fragment)
