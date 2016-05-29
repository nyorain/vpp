add_executable(godrays main.cpp godrays.cpp)
target_link_libraries(godrays vpp)

add_shader("godrays-combine.frag" godrays)
add_shader("godrays-source.frag" godrays)
add_shader("godrays-combine.vert" godrays)
add_shader("godrays-source.vert" godrays)
