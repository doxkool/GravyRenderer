cmake_minimum_required(VERSION 3.28)

project(GravyRenderer CXX)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)
find_package(imgui REQUIRED)
find_package(spdlog REQUIRED)
find_package(tinyobjloader REQUIRED)
find_package(Tracy REQUIRED)

file(GLOB_RECURSE RENDERER_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_library(GravyRenderer SHARED ${RENDERER_SOURCES})

include_directories(
    ${CMAKE_SOURCE_DIR}/src/GravyRenderer
    ${CMAKE_BINARY_DIR}/vcpkg_installed/x64-windows/include
    )

target_link_libraries(GravyRenderer PUBLIC OpenGL::GL glfw GLEW::GLEW glm::glm imgui::imgui spdlog::spdlog tinyobjloader::tinyobjloader)