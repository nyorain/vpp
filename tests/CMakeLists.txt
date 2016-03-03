add_executable(vpptest main.cpp mesh.cpp)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR})
find_package(ASSIMP REQUIRED)

#message(FATAL_ERROR "yoyo:" ${assimp_FOUND} " -- " ${ASSIMP_INCLUDE_DIRS})

target_include_directories(vpptest PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(vpptest PUBLIC ${ASSIMP_INCLUDE_DIRS})
target_link_libraries(vpptest ${ASSIMP_LIBRARIES})
target_link_libraries(vpptest
    vpp
    "C:/VulkanSDK/1.0.3.1/Source/lib/vulkan-1.dll"
    "gdi32"
    "kernel32")
