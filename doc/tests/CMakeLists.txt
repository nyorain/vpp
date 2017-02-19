add_executable(memory memory.cpp)
target_link_libraries(memory vpp ${Vulkan_LIBRARY})
add_test(memory "bin/memory")
