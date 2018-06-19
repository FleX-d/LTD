cmake_minimum_required(VERSION 3.0)
project(fleXdLoggerClient VERSION "0.0.1")
set(PROJECT_DESCRIPTION "Logger client for fleXd project")

message(STATUS "***" )
message(STATUS "*  " ${PROJECT_NAME})
message(STATUS "***" )

message(STATUS "fleXdLoggerCommonPublicInc: " ${fleXdLoggerCommonPublicInc})
message(STATUS "BITSTREAM_INCLUDE_DIR:" ${BITSTREAM_INCLUDE_DIR})

include(GNUInstallDirs)

find_package(PkgConfig)
pkg_check_modules(FLEXDUDS REQUIRED fleXdUDS)
pkg_check_modules(FLEXDEPOLL REQUIRED fleXdEpoll)
pkg_check_modules(FLEXDIPCCOMMON REQUIRED fleXdIPCCommon)

SET(publicHeaders
    inc/public/FleXdLogger.h
)

SET(privateHeaders
    inc/FleXdLogBuffer.h
    inc/FleXdLoggerIPCClient.h
)

SET(privateSources
    src/FleXdLogger.cpp
    src/FleXdLogBuffer.cpp
    src/FleXdLoggerIPCClient.cpp
)

add_library(${PROJECT_NAME} SHARED ${privateSources})
set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    PUBLIC_HEADER "${publicHeaders}")
configure_file(${PROJECT_NAME}.pc.in ${PROJECT_NAME}.pc @ONLY)

target_include_directories(${PROJECT_NAME} PUBLIC inc/public )
target_include_directories(${PROJECT_NAME} PRIVATE inc/
    ${BITSTREAM_INCLUDE_DIR}
    ${fleXdLoggerCommonPublicInc}
    ${FLEXDUDS_INCLUDE_DIRS}
    ${FLEXDEPOLL_INCLUDE_DIRS}
    ${FLEXDIPCCOMMON_INCLUDE_DIRS}
)
target_compile_options(${PROJECT_NAME} PUBLIC -std=c++14 -Wall -Wfloat-conversion)

target_link_libraries(${PROJECT_NAME}
    ${FLEXDUDS_LIBRARIES}
    ${FLEXDEPOLL_LIBRARIES}
    ${FLEXDIPCCOMMON_LIBRARIES}
)

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES ${CMAKE_BINARY_DIR}/libs/fleXdLoggerClient/${PROJECT_NAME}.pc
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)

SET(fleXdLoggerClientPublicInc "${fleXdLoggerClient_SOURCE_DIR}/inc/public" CACHE STRING "")
SET(fleXdLoggerClientPrivateInc "${fleXdLoggerClient_SOURCE_DIR}/inc" CACHE STRING "")
SET(fleXdLoggerClientLib "${PROJECT_NAME}" CACHE STRING "")
message(STATUS "*Export fleXdLoggerClientPublicInc: " ${fleXdLoggerClientPublicInc})
message(STATUS "*Export fleXdLoggerClientLib: " ${fleXdLoggerClientLib})

add_dependencies(${PROJECT_NAME}
    bitstream
    fleXdLoggerCommon
)

if(${ENABLE_EXAMPLES})
    add_subdirectory(example)
endif(${ENABLE_EXAMPLES})

if(${ENABLE_TESTS})
    add_subdirectory(gtest)
endif(${ENABLE_TESTS})
