cmake_minimum_required(VERSION 3.28)

project(Sandbox CXX)

file(GLOB_RECURSE SANDBOX_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

## Create main executable
add_executable(Sandbox ${SANDBOX_SOURCES})

include_directories(
    ${CMAKE_SOURCE_DIR}/src/GravyRenderer
    ${CMAKE_SOURCE_DIR}/src/GravyRenderer/Vendors/SoLoud/include
    ${CMAKE_BINARY_DIR}/vcpkg_installed/x64-windows/include
    )

SET(soloud_static ${CMAKE_BINARY_DIR}/src/GravyRenderer/Vendors/SoLoud/Debug/soloud.lib)

target_link_libraries(Sandbox PUBLIC ${soloud_static} GravyRenderer)

IF (WIN32)
    add_custom_target(Copy_Editor_assets_files ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/src/Sandbox/assets ${CMAKE_BINARY_DIR}/bin/Debug/assets)

ELSEIF (LINUX)
    add_custom_target(Copy_Editor_assets_files ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/src/Sandbox/assets ${CMAKE_BINARY_DIR}/bin/assets)
        
ENDIF()