add_executable(clouds main.cpp clouds.cpp)
target_link_libraries(clouds vpp)

add_shader("clouds.frag" clouds)
add_shader("clouds.vert" clouds)
