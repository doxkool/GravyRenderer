cmake_minimum_required(VERSION 3.28)

project(GravyRenderer CXX)

if(WIN32)
    SET(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
    SET(soloud_static ${CMAKE_BINARY_DIR}/src/GravyRenderer/Vendors/SoLoud/Debug/soloud.lib)
    add_definitions (-DWITH_WASAPI)
endif(WIN32)

if(UNIX)
SET(soloud_static ${CMAKE_BINARY_DIR}/src/GravyRenderer/Vendors/SoLoud/libsoloud.a)
    add_definitions (-DWITH_MINIAUDIO)
endif(UNIX)


find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)
find_package(imgui REQUIRED)
find_package(spdlog REQUIRED)
find_package(tinyobjloader REQUIRED)
find_package(Tracy REQUIRED)

add_subdirectory(${CMAKE_SOURCE_DIR}/src/GravyRenderer/Vendors/SoLoud)

file(GLOB_RECURSE RENDERER_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_library(GravyRenderer SHARED ${RENDERER_SOURCES})

include_directories(
    ${CMAKE_SOURCE_DIR}/src/GravyRenderer
    ${CMAKE_SOURCE_DIR}/src/GravyRenderer/Vendors/SoLoud/include
    ${CMAKE_BINARY_DIR}/vcpkg_installed/x64-windows/include
    )

target_link_libraries(GravyRenderer PUBLIC ${soloud_static} OpenGL::GL glfw GLEW::GLEW glm::glm imgui::imgui spdlog::spdlog tinyobjloader::tinyobjloader)