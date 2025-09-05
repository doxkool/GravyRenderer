cmake_minimum_required(VERSION 3.28)

project(Sandbox CXX)

if(WIN32)
    SET(soloud_static ${CMAKE_BINARY_DIR}/src/Sandbox/Vendors/SoLoud/Debug/soloud.lib)
    add_definitions (-DWITH_WASAPI)
endif(WIN32)

if(UNIX)
    SET(soloud_static ${CMAKE_BINARY_DIR}/src/Sandbox/Vendors/SoLoud/libsoloud.a)
    add_definitions (-DWITH_MINIAUDIO)
endif(UNIX)

file(GLOB_RECURSE SANDBOX_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

## Create main executable
add_executable(Sandbox ${SANDBOX_SOURCES})

include_directories(
    ${CMAKE_SOURCE_DIR}/src/Sandbox
    ${CMAKE_SOURCE_DIR}/src/GravyRenderer
    ${CMAKE_SOURCE_DIR}/src/Sandbox/Vendors/SoLoud/include
    ${CMAKE_BINARY_DIR}/vcpkg_installed/x64-windows/include
    )

add_subdirectory(${CMAKE_SOURCE_DIR}/src/Sandbox/Vendors/SoLoud)

target_link_libraries(Sandbox PUBLIC GravyRenderer ${soloud_static})

IF (WIN32)
    add_custom_target(Copy_Editor_assets_files ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/src/Sandbox/assets ${CMAKE_BINARY_DIR}/bin/Debug/assets)

ELSEIF (LINUX)
    add_custom_target(Copy_Editor_assets_files ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/src/Sandbox/assets ${CMAKE_BINARY_DIR}/bin/assets)
        
ENDIF()