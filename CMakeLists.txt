CMAKE_MINIMUM_REQUIRED(VERSION 3.1.0)
PROJECT(vpp)

set(vmajor 0)
set(vminor 1)
set(vpatch 0)

include(CMakeDependentOption)

option(Debug "Compile in debug mode" on)
option(Depend "Depend on needed libraries. Development option" on)

option(Codegen "Regenerate c++ vulkan api" off)
option(ApiVersion "C++ vulkan api version to generate [1-0-20|1-0-40|1-0-latest]" off)

option(Examples "Build Examples" off)
option(OneDevice "Enable the one device optimization. Not recommended" off)
option(Test "Build tests" off)

if(Debug)
	set(CMAKE_BUILD_TYPE Debug)
else()
	set(CMAKE_BUILD_TYPE Release)
endif()

if(NOT ApiVersion)
	set(ApiVersion "1-0-latest")
endif()

# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z")

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED on)

# enable warnings
set(is_clang CMAKE_CXX_COMPILER_ID MATCHES "Clang")
if(MSVC)
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX OR is_clang)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -Wextra -Wpedantic")
endif()

# to disable ninja byproduct warning
cmake_policy(SET CMP0058 NEW)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/include) #e.g. config file

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/cmake")
find_package(Vulkan REQUIRED)

if(Codegen)
	add_subdirectory(src/codegen)
endif()

# dlg for logging
include(ExternalProject)
set(ExternalProjectCMakeArgs
	-DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/external/install
	-DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
	-DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
	-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE})

ExternalProject_Add(dlg_ep
	PREFIX ${CMAKE_CURRENT_BINARY_DIR}/dlg
	GIT_REPOSITORY https://github.com/nyorain/dlg.git
	GIT_TAG master
	INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/external/install
	CMAKE_ARGS ${ExternalProjectCMakeArgs})

include_directories("${CMAKE_BINARY_DIR}/external/install/include")
add_definitions(-DDLG_BASE_PATH="${CMAKE_SOURCE_DIR}/")
add_definitions(-DDLG_HEADER_ONLY=0)
add_definitions(-DDLG_DEBUG=${Debug})

# src/include
add_subdirectory(include/vpp)
add_subdirectory(src/vpp)


if(Test)
	enable_testing()
	add_subdirectory(doc/tests)
endif()

#config file
set(VPP_DEBUG ${Debug})
set(VPP_ONE_DEVICE_OPTIMIZATION ${OneDevice})

configure_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/src/vpp/config.hpp.in"
	"${CMAKE_CURRENT_BINARY_DIR}/include/vpp/config.hpp")

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/include/vpp/config.hpp" DESTINATION include/vpp)

if(Examples)
	if(Win32 OR MSYS OR MINGW)
		add_subdirectory(doc/examples)
	else()
		message("The examples are windows-only at the moment. Pull requests appreciated.")
	endif()
endif()
