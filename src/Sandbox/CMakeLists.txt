cmake_minimum_required(VERSION 3.28)

project(Sandbox CXX)

file(GLOB_RECURSE SANDBOX_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

## Create main executable
add_executable(SANDBOX ${SANDBOX_SOURCES})

include_directories(${CMAKE_BINARY_DIR}/vcpkg_installed/x64-windows/include)

include_directories(${CMAKE_SOURCE_DIR}/src/Renderer)

target_link_libraries(SANDBOX PRIVATE ${CMAKE_BINARY_DIR}/bin/lib/Renderer.lib)