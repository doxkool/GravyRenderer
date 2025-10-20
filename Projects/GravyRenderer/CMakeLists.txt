cmake_minimum_required(VERSION 3.28)

project(GravyRenderer CXX)

SET(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

if (WIN32)
    SET(CoreUtils_static ${CMAKE_BINARY_DIR}/Projects/CoreUtils/Debug/CoreUtils.lib)
endif(WIN32)

if (UNIX)
    SET(CoreUtils_static ${CMAKE_BINARY_DIR}/Projects/CoreUtils/libCoreUtils.a)
endif(UNIX)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)
find_package(imgui REQUIRED)
find_package(spdlog REQUIRED)
find_package(tinyobjloader REQUIRED)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Vendors/Tracy)

file(GLOB_RECURSE RENDERER_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_library(GravyRenderer SHARED ${RENDERER_SOURCES})

include_directories(
    ${CMAKE_SOURCE_DIR}/Projects/CoreUtils/src
    ${CMAKE_SOURCE_DIR}/Projects/GravyRenderer/src
    ${CMAKE_BINARY_DIR}/vcpkg_installed/x64-windows/include
    )

target_link_libraries(GravyRenderer PUBLIC ${CoreUtils_static} OpenGL::GL glfw GLEW::GLEW glm::glm imgui::imgui spdlog::spdlog tinyobjloader::tinyobjloader Tracy::TracyClient)