cmake_minimum_required(VERSION 3.28)

project(CoreUtils CXX)

SET(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

find_package(spdlog REQUIRED)

file(GLOB_RECURSE COREUTILS_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_library(CoreUtils STATIC ${COREUTILS_SOURCES})

include_directories(
    ${CMAKE_SOURCE_DIR}/src/CoreUtils
    )

target_link_libraries(CoreUtils PUBLIC spdlog::spdlog)