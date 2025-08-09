cmake_minimum_required(VERSION 3.28)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/lib)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (LINUX)
    set(vcpkg "/home/$ENV{USER}/.local/share/vcpkg/scripts/buildsystems/vcpkg.cmake")
elseif(WIN32)
  set(vcpkg "C:/Users/$ENV{USERNAME}/vcpkg/scripts/buildsystems/vcpkg.cmake")
endif()

if(NOT CMAKE_TOOLCHAIN_FILE AND EXISTS "${vcpkg}")
  set(CMAKE_TOOLCHAIN_FILE "${vcpkg}"
      CACHE FILEPATH "CMake toolchain file")
  message(STATUS "vcpkg toolchain found: ${CMAKE_TOOLCHAIN_FILE}")
endif()

add_subdirectory(${CMAKE_SOURCE_DIR}/src/Renderer)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/Sandbox)