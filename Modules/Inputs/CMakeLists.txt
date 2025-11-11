cmake_minimum_required(VERSION 3.28)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/lib)

SET(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

project(Inputs CXX)

find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

file(GLOB_RECURSE RENDERER_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_library(Inputs STATIC ${RENDERER_SOURCES})

include_directories(
    ${CMAKE_BINARY_DIR}/vcpkg_installed/x64-windows/include
    ${CMAKE_BINARY_DIR}/vcpkg_installed/x64-linux/include
    )

target_link_libraries(Inputs PUBLIC glfw glm::glm)

set_target_properties(Inputs PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/lib"
)