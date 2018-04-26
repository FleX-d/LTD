cmake_minimum_required(VERSION 3.0)
project(fleXdLoggerServer VERSION "0.0.1")
set(PROJECT_DESCRIPTION "Logger daemon for fleXd project")

message(STATUS "***" )
message(STATUS "*  " ${PROJECT_NAME})
message(STATUS "***" )

message(STATUS "fleXdLoggerCommonPublicInc: " ${fleXdLoggerCommonPublicInc})
message(STATUS "BITSTREAM_INCLUDE_DIR:" ${BITSTREAM_INCLUDE_DIR})

#find_package(PkgConfig)
#pkg_check_modules(FLEXDUDS REQUIRED fleXdUDS)
#pkg_check_modules(FLEXDEPOLL REQUIRED fleXdEpoll)
#pkg_check_modules(FLEXDIPCBUFFER REQUIRED fleXdIPCBuffer)

SET(publicHeaders
)

SET(privateHeaders
    inc/FleXdAppArray.h
    inc/FleXdApplication.h
    inc/iSocServer.h
    inc/FleXdLoggerServer.h
    inc/FleXdLogMessage.h
    inc/FleXdLoggerIPC.h
)

SET(privateSources
  src/FleXdAppArray.cpp
  src/FleXdApplication.cpp
  src/iSocServer.cpp
  src/FleXdLoggerServer.cpp
  src/FleXdLogMessage.cpp
  src/mainFleXdLogger.cpp
  src/FleXdLoggerIPC.cpp
)

add_executable(${PROJECT_NAME} ${privateSources} ${privateHeaders} ${publicHeaders})
target_include_directories(${PROJECT_NAME} PUBLIC inc/public)
target_include_directories(${PROJECT_NAME} PRIVATE inc/ 
    ${BITSTREAM_INCLUDE_DIR} 
    ${fleXdLoggerCommonPublicInc}
#    ${FLEXDUDS_INCLUDE_DIRS}
#    ${FLEXDEPOLL_INCLUDE_DIRS}
#    ${FLEXDIPCBUFFER_INCLUDE_DIRS}
)

target_compile_options(${PROJECT_NAME} PUBLIC -std=c++14 -Wall -Wfloat-conversion)
target_link_libraries(${PROJECT_NAME}
#    ${FLEXDUDS_LIBRARIES}
#    ${FLEXDEPOLL_LIBRARIES}
#    ${FLEXDIPCBUFFER_LIBRARIES}
)


install(TARGETS ${PROJECT_NAME}
    DESTINATION ${CMAKE_BINARY_DIR})

add_dependencies(${PROJECT_NAME}
    bitstream
    fleXdLoggerCommon
#    fleXdUDS
#    fleXdEpoll
#    fleXdIPCBuffer
)
 
