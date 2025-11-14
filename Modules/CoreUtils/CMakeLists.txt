cmake_minimum_required(VERSION 3.28)

project(CoreUtils CXX)

SET(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} --std=c++11 -fPIC" )

find_package(spdlog REQUIRED)

file(GLOB_RECURSE COREUTILS_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_library(CoreUtils STATIC ${COREUTILS_SOURCES})

include_directories(
    ${CMAKE_SOURCE_DIR}/Projects/CoreUtils/src
    )

target_link_libraries(CoreUtils PUBLIC spdlog::spdlog)

set_target_properties(CoreUtils PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/lib"
)