cmake_minimum_required(VERSION 3.0)
project(FleXdLogClient VERSION "0.0.1")
set(PROJECT_DESCRIPTION "Logger client for fleXd project")

message(STATUS "***" )
message(STATUS "* " ${PROJECT_NAME})
message(STATUS "***")

include(GNUInstallDirs)
include(ExternalProject)
find_package(Git REQUIRED)

ExternalProject_Add(
    bitstream2
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

ExternalProject_Get_Property(bitstream2 source_dir)
set(BITSTREAM_INCLUDE_DIR ${source_dir}/lib CACHE INTERNAL "Path to include folder for bitstream")
message(STATUS "BITSTREAM_INCLUDE_DIR:" ${BITSTREAM_INCLUDE_DIR})

SET(publicHeaders
    inc/public/FleXdLogger.h
)
SET(privateHeaders
    inc/iSocClient.h
    inc/FleXdLogStream.h
)

SET(privateSources
    src/iSocClient.cpp
    src/FleXdLogStream.cpp
    src/FleXdLogger.cpp
)

add_library(${PROJECT_NAME} SHARED ${privateSources})
set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    PUBLIC_HEADER "${publicHeaders}")
configure_file(${PROJECT_NAME}.pc.in ${PROJECT_NAME}.pc @ONLY)

target_include_directories(${PROJECT_NAME} PUBLIC inc/public)
target_include_directories(${PROJECT_NAME} PRIVATE inc/ ${BITSTREAM_INCLUDE_DIR})
target_compile_options(${PROJECT_NAME} PUBLIC -std=c++14 -Wall -Wfloat-conversion)

target_link_libraries(${PROJECT_NAME}
)

SET(fleXdLogClientPublicInc "${FleXdLogClient_SOURCE_DIR}/inc/public" PARENT_SCOPE)
SET(fleXdLogClientLib "${PROJECT_NAME}" PARENT_SCOPE)

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES ${CMAKE_BINARY_DIR}/libs/fleXdLogClient/${PROJECT_NAME}.pc
DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)

message(STATUS "CMAKE_BINARY_DIR:" ${CMAKE_BINARY_DIR})

message(STATUS "PROJECT_NAME:" ${PROJECT_NAME})

#TODO avoid buil examples by defualt
add_subdirectory(example)

add_dependencies(${PROJECT_NAME} bitstream)


