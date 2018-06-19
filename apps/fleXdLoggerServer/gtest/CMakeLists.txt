cmake_minimum_required(VERSION 3.0)
project(fleXdLoggerServerTest VERSION "0.0.1")


message(STATUS "***" )
message(STATUS "* " ${PROJECT_NAME})
message(STATUS "***")

find_package(PkgConfig)
pkg_check_modules(GTEST REQUIRED gtest>=1.7.0)
pkg_check_modules(GMOCK REQUIRED gmock>=1.7.0)

message(STATUS "GTEST_LIBRARIES: " ${GTEST_LIBRARIES})
message(STATUS "GTEST_LIBRARIES: " ${GMOCK_LIBRARIES})
message(STATUS "GTEST_LIBRARIES: " ${GTEST_INCLUDE_DIRS})
message(STATUS "GTEST_LIBRARIES: " ${GMOCK_INCLUDE_DIRS})

message(STATUS "fleXdLoggerServerInc: " ${fleXdLoggerServerInc})
message(STATUS "BINSTREAM_LIBRARIES: " ${BITSTREAM_INCLUDE_DIR})
message(STATUS "FLEXDUDS_INCLUDE_DIRS: "  ${FLEXDUDS_INCLUDE_DIRS})
message(STATUS "FLEXDUDS_LIBRARY_DIRS: "  ${FLEXDUDS_LIBRARIES})
message(STATUS "FLEXDEPOLL_INCLUDE_DIRS: "  ${FLEXDEPOLL_INCLUDE_DIRS})
message(STATUS "FLEXDEPOLL_LIBRARIES: "  ${FLEXDEPOLL_LIBRARIES})
message(STATUS "FLEXDIPCCOMMON_INCLUDE_DIRS: "  ${FLEXDIPCCOMMON_INCLUDE_DIRS})
message(STATUS "FLEXDIPCCOMMON_LIBRARIES: "  ${FLEXDIPCCOMMON_LIBRARIES})

SET(privateSources
    ${fleXdLoggerServer_SOURCE_DIR}/src/FleXdAppArray.cpp
    ${fleXdLoggerServer_SOURCE_DIR}/src/FleXdApplication.cpp
    ${fleXdLoggerServer_SOURCE_DIR}/src/FleXdLoggerIPCServer.cpp
    ${fleXdLoggerServer_SOURCE_DIR}/src/FleXdLoggerServer.cpp
    FleXdAppArrayTest.cpp
    FleXdApplicationTest.cpp
    FleXdLoggerServerTest.cpp
)

include_directories(
    ${BITSTREAM_INCLUDE_DIR}
    ${GTEST_INCLUDE_DIRS}
    ${GMOCK_INCLUDE_DIRS}
    ${fleXdLoggerServerInc}
    ${FLEXDUDS_INCLUDE_DIRS}
    ${FLEXDEPOLL_INCLUDE_DIRS}
    ${FLEXDIPCCOMMON_INCLUDE_DIRS}
)

add_executable(${PROJECT_NAME} main.cpp ${privateSources})

target_compile_options(${PROJECT_NAME} PUBLIC -std=c++14 -Wall -Wfloat-conversion)
target_link_libraries( ${PROJECT_NAME}
    ${GTEST_LIBRARIES}
    ${GMOCK_LIBRARIES}
    ${FLEXDUDS_LIBRARIES}
    ${FLEXDEPOLL_LIBRARIES}
    ${FLEXDIPCCOMMON_LIBRARIES}
)

add_dependencies(${PROJECT_NAME} fleXdLoggerServer bitstream)
