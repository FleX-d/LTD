cmake_minimum_required(VERSION 3.0)

if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "setting default value 'Debug' for CMAKE_BUILD_TYPE")
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type" FORCE)
endif()

set_property(
    CACHE
    CMAKE_BUILD_TYPE
    PROPERTY STRINGS
    "Debug"
    "Release"
)

option(
    ENABLE_TESTS
    "Build tests?"
    OFF
)
option(
    LOG_TO_DLT
    "Build tests?"
    ON
)
message(STATUS "option ENABLE_TESTS=" ${ENABLE_TESTS})
message(STATUS "option LOG_TO_DLT=" ${LOG_TO_DLT})

include(ExternalProject)
find_package(Git REQUIRED)

ExternalProject_Add(
    bitstream
    PREFIX ${CMAKE_BINARY_DIR}/ext
    GIT_REPOSITORY https://github.com/AdrianPeniak/bitstream.git
    TIMEOUT 10
    #UPDATE_COMMAND ${GIT_EXECUTABLE} pull
    UPDATE_COMMAND ""
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    LOG_DOWNLOAD ON
)

ExternalProject_Get_Property(bitstream source_dir)
set(BITSTREAM_INCLUDE_DIR ${source_dir}/lib CACHE INTERNAL "Path to include folder for bitstream")
message(STATUS "BITSTREAM_INCLUDE_DIR:" ${BITSTREAM_INCLUDE_DIR})

add_subdirectory(libs)
add_subdirectory(apps)
