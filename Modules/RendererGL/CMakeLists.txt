cmake_minimum_required(VERSION 3.28)

project(RendererGL CXX)

SET(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

if (WIN32)
    SET(CoreUtils_static ${CMAKE_BINARY_DIR}/Modules/CoreUtils/Debug/CoreUtils.lib)
    SET(Tracy_static ${CMAKE_BINARY_DIR}/Modules/RendererGL/Vendors/Tracy/Debug/TracyClient.lib)
endif(WIN32)

if (UNIX)
    #SET(Inputs_static ${CMAKE_BINARY_DIR}/Modules/Inputs/libInputs.a)
    SET(CoreUtils_static ${CMAKE_BINARY_DIR}/Modules/CoreUtils/libCoreUtils.a)
    SET(Tracy_static ${CMAKE_BINARY_DIR}/Modules/RendererGL/Vendors/Tracy/libTracyClient.a)
endif(UNIX)

find_package(OpenGL REQUIRED)
find_package(Vulkan REQUIRED)
find_package(SPIRV-Tools REQUIRED)
find_package(glslang REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)
find_package(imgui REQUIRED)
find_package(spdlog REQUIRED)
find_package(tinyobjloader REQUIRED)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Vendors/Tracy)

file(GLOB_RECURSE RENDERER_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_library(RendererGL SHARED ${RENDERER_SOURCES})

include_directories(
    ${CMAKE_SOURCE_DIR}/Modules/Inputs/src
    ${CMAKE_SOURCE_DIR}/Modules/CoreUtils/src
    ${CMAKE_SOURCE_DIR}/Modules/RendererGL/src
    ${CMAKE_SOURCE_DIR}/Modules/RendererGL/Vendors/Tracy/public
    ${CMAKE_BINARY_DIR}/vcpkg_installed/x64-windows/include
    ${CMAKE_BINARY_DIR}/vcpkg_installed/x64-linux/include
    )

SET(Vulkan_Dep Vulkan::Vulkan SPIRV-Tools glslang::glslang-default-resource-limits glslang::glslang)

target_link_libraries(RendererGL PUBLIC ${CoreUtils_static} ${Tracy_static} ${Vulkan_Dep} OpenGL::GL glfw GLEW::GLEW glm::glm imgui::imgui spdlog::spdlog tinyobjloader::tinyobjloader)

set_target_properties(RendererGL PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/lib"
)