add_executable(clouds main.cpp clouds.cpp)

target_include_directories(clouds PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(clouds
	vpp "$ENV{VK_SDK_PATH}/Source/lib/vulkan-1.dll")

add_shader("clouds.frag" clouds)
add_shader("clouds.vert" clouds)
