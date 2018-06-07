cmake_minimum_required(VERSION 3.0)
project(fleXdLoggerServer VERSION "0.0.1")
set(PROJECT_DESCRIPTION "Logger daemon for fleXd project")

message(STATUS "***" )
message(STATUS "*  " ${PROJECT_NAME})
message(STATUS "***" )

message(STATUS "fleXdLoggerCommonPublicInc: " ${fleXdLoggerCommonPublicInc})
message(STATUS "fleXdLoggerDltPublicInc: " ${fleXdLoggerDltPublicInc})
message(STATUS "fleXdLoggerDltLib: " ${fleXdLoggerDltLib})
message(STATUS "BITSTREAM_INCLUDE_DIR:" ${BITSTREAM_INCLUDE_DIR})

find_package(PkgConfig)
pkg_check_modules(FLEXDUDS REQUIRED fleXdUDS)
pkg_check_modules(FLEXDEPOLL REQUIRED fleXdEpoll)
pkg_check_modules(FLEXDIPCCOMMON REQUIRED fleXdIPCCommon)

message(STATUS "FLEXDUDS_INCLUDE_DIRS: "  ${FLEXDUDS_INCLUDE_DIRS})
message(STATUS "FLEXDUDS_LIBRARY_DIRS: "  ${FLEXDUDS_LIBRARIES})
message(STATUS "FLEXDEPOLL_INCLUDE_DIRS: "  ${FLEXDEPOLL_INCLUDE_DIRS})
message(STATUS "FLEXDEPOLL_LIBRARIES: "  ${FLEXDEPOLL_LIBRARIES})
message(STATUS "BITSTREAM_INCLUDE_DIR:" ${BITSTREAM_INCLUDE_DIR})
SET(publicHeaders
)

SET(privateHeaders
    inc/FleXdAppArray.h
    inc/FleXdApplication.h
    inc/FleXdLoggerServer.h
    inc/FleXdLoggerIPCServer.h
)

SET(privateSources
    src/FleXdAppArray.cpp
    src/FleXdApplication.cpp
    src/FleXdLoggerServer.cpp
    src/mainFleXdLogger.cpp
    src/FleXdLoggerIPCServer.cpp
)

add_executable(${PROJECT_NAME} ${privateSources} ${privateHeaders} ${publicHeaders})
target_include_directories(${PROJECT_NAME} PUBLIC inc/public)
target_include_directories(${PROJECT_NAME} PRIVATE inc/
    ${BITSTREAM_INCLUDE_DIR}
    ${fleXdLoggerCommonPublicInc}
    ${FLEXDUDS_INCLUDE_DIRS}
    ${FLEXDEPOLL_INCLUDE_DIRS}
    ${FLEXDIPCCOMMON_INCLUDE_DIRS}
)

if(${WITH_DLT})
    target_include_directories(${PROJECT_NAME} PRIVATE ${fleXdLoggerDltPublicInc})
endif(${WITH_DLT})

target_compile_options(${PROJECT_NAME} PUBLIC -std=c++14 -Wall -Wfloat-conversion)
target_link_libraries(${PROJECT_NAME}
        ${FLEXDUDS_LIBRARIES}
        ${FLEXDEPOLL_LIBRARIES}
        ${FLEXDIPCCOMMON_LIBRARIES}
    )
if(${WITH_DLT})
    target_link_libraries(${PROJECT_NAME}
        ${fleXdLoggerDltLib}
    )
endif(${WITH_DLT})

install(TARGETS ${PROJECT_NAME}
    DESTINATION ${CMAKE_BINARY_DIR})

add_dependencies(${PROJECT_NAME}
    bitstream
    fleXdLoggerCommon
)
if(${WITH_DLT})
    add_dependencies(${PROJECT_NAME} fleXdLoggerDlt)
endif(${WITH_DLT})

SET(fleXdLoggerServerInc "${fleXdLoggerServer_SOURCE_DIR}/inc" CACHE STRING "")

if(${ENABLE_TESTS})
    add_subdirectory(gtest)
endif(${ENABLE_TESTS})
