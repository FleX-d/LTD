cmake_minimum_required(VERSION 3.0)
project(fleXdLoggerCommon VERSION "0.0.1")
set(PROJECT_DESCRIPTION "Libraries for logger server & logger client")

message(STATUS "***" )
message(STATUS "*  " ${PROJECT_NAME})
message(STATUS "***" )

SET(publicHeaders
  inc/public/FleXdMessageType.h
)

SET(privateHeaders
)

SET(privateSources
)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} INTERFACE ${publicHeaders})


install(FILES ${publicHeaders}
DESTINATION include)

SET(fleXdLoggerCommonPublicInc "${fleXdLoggerCommon_SOURCE_DIR}/inc/public" CACHE STRING "")
message(STATUS "*Export fleXdLoggerCommonPublicInc: " ${fleXdLoggerCommonPublicInc})