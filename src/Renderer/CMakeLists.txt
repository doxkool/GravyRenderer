cmake_minimum_required(VERSION 3.28)

project(Renderer CXX)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)

file(GLOB_RECURSE RENDERER_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_library(Renderer SHARED ${RENDERER_SOURCES})

include_directories(${CMAKE_BINARY_DIR}/vcpkg_installed/x64-windows/include)
target_link_libraries(Renderer PUBLIC OpenGL::GL glfw GLEW::GLEW glm::glm)