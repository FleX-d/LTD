cmake_minimum_required(VERSION 3.0)
project(fleXdLoggerDlt VERSION "0.0.1")
set(PROJECT_DESCRIPTION "Logger to dlt for fleXd project")

message(STATUS "***" )
message(STATUS "*  " ${PROJECT_NAME})
message(STATUS "***" )

message(STATUS "fleXdLoggerCommonPublicInc: " ${fleXdLoggerCommonPublicInc})

include(GNUInstallDirs)

find_package(PkgConfig)
pkg_check_modules(DLT REQUIRED automotive-dlt)

SET(publicHeaders
    inc/public/FleXdLoggerDlt.h
)

SET(privateHeaders
)

SET(privateSources
    src/FleXdLoggerDlt.cpp
)

add_library(${PROJECT_NAME} SHARED ${privateSources})
set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    PUBLIC_HEADER "${publicHeaders}")
configure_file(${PROJECT_NAME}.pc.in ${PROJECT_NAME}.pc @ONLY)

target_include_directories(${PROJECT_NAME} PUBLIC inc/public ${DLT_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PRIVATE inc/

    ${fleXdLoggerCommonPublicInc}
)
target_compile_options(${PROJECT_NAME} PUBLIC -std=c++14 -Wall -Wfloat-conversion -DDLT_ENABLED)

target_link_libraries(${PROJECT_NAME}
    ${DLT_LIBRARIES}
)

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES ${CMAKE_BINARY_DIR}/libs/fleXdLoggerDlt/${PROJECT_NAME}.pc
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)

SET(fleXdLoggerDltPublicInc "${fleXdLoggerDlt_SOURCE_DIR}/inc/public" CACHE STRING "")
SET(fleXdLoggerDltLib "${PROJECT_NAME}" CACHE STRING "")
message(STATUS "*Export fleXdLoggerDltPublicInc: " ${fleXdLoggerDltPublicInc})
message(STATUS "*Export fleXdLoggerDltLib: " ${fleXdLoggerDltLib})

add_dependencies(${PROJECT_NAME} fleXdLoggerCommon)
