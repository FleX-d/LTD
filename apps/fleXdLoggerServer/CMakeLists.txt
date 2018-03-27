cmake_minimum_required(VERSION 3.0)
project(fleXdLogger VERSION "0.0.1")
set(PROJECT_DESCRIPTION "Logger daemon for fleXd project")

message(STATUS "***" )
message(STATUS "*  " ${PROJECT_NAME})
message(STATUS "***" )

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

SET(publicHeaders
)

SET(privateHeaders
    inc/AppArray.h
    inc/Application.h
    inc/iSocServer.h
    inc/Logger.h
    inc/LogMessage.h
    inc/MessageType.h
)

SET(privateSources
  src/AppArray.cpp
  src/Application.cpp
  src/iSocServer.cpp
  src/Logger.cpp
  src/LogMessage.cpp
  src/mainLogger.cpp
)

add_executable(${PROJECT_NAME} ${privateSources} ${privateHeaders} ${publicHeaders})
target_include_directories(${PROJECT_NAME} PUBLIC inc/public)
target_include_directories(${PROJECT_NAME} PRIVATE inc/ ${BITSTREAM_INCLUDE_DIR})
target_compile_options(${PROJECT_NAME} PUBLIC -std=c++14 -Wall -Wfloat-conversion)
target_link_libraries(${PROJECT_NAME}
)


install(TARGETS ${PROJECT_NAME}
    DESTINATION ${CMAKE_BINARY_DIR})

add_dependencies(${PROJECT_NAME} bitstream)
 
