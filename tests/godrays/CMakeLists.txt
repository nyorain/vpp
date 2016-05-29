add_executable(godrays main.cpp godrays.cpp)

target_include_directories(godrays PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(godrays
	vpp "$ENV{VK_SDK_PATH}/Source/lib/vulkan-1.dll")

add_shader("godrays-combine.frag" godrays)
add_shader("godrays-source.frag" godrays)
add_shader("godrays-combine.vert" godrays)
add_shader("godrays-source.vert" godrays)
