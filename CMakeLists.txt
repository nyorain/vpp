CMAKE_MINIMUM_REQUIRED(VERSION 3.1.0)
PROJECT(vpp)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)

include_directories("$ENV{VK_SDK_PATH}/Include")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(codegen)
add_subdirectory(include)
add_subdirectory(src/vpp)
add_subdirectory(tests)
