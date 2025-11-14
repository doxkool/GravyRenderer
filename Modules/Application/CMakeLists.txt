cmake_minimum_required(VERSION 3.28)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/lib)

SET(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

project(Application CXX)

find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(spdlog REQUIRED)

file(GLOB_RECURSE Application_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_library(Application STATIC ${Application_SOURCES})

include_directories(
    ${CMAKE_SOURCE_DIR}/Modules/CoreUtils/src
    ${CMAKE_SOURCE_DIR}/Modules/Application/src
    ${CMAKE_SOURCE_DIR}/Modules/RendererGL/src
    ${CMAKE_BINARY_DIR}/vcpkg_installed/x64-windows/include
    ${CMAKE_BINARY_DIR}/vcpkg_installed/x64-linux/include
    )

target_link_libraries(Application PUBLIC glfw glm::glm spdlog::spdlog)

set_target_properties(Application PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/lib"
)