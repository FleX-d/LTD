cmake_minimum_required(VERSION 3.0)
project(fleXdMessageType)
set(PROJECT_DESCRIPTION "Libraries for logger server & logger client")

message(STATUS "***" )
message(STATUS "*  " ${PROJECT_NAME})
message(STATUS "***" )

SET(publicHeaders
  inc/public/MessageType.h
)

#SET(privateHeaders
#)

#SET(privateSources
#)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} INTERFACE ${publicHeaders})


install(FILES ${publicHeaders}
DESTINATION include)