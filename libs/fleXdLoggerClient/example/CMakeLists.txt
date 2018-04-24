cmake_minimum_required(VERSION 3.0)
project(LogExample_1 VERSION "0.0.1")
set(PROJECT_DESCRIPTION "Logger example for fleXd project")

message(STATUS "***" )
message(STATUS "* " ${PROJECT_NAME})
message(STATUS "***")

SET(privateSources
    main.cpp
)

message(STATUS "fleXdLogPublicInc: " ${fleXdLogPublicInc})
message(STATUS "fleXdLogLib: " ${fleXdLogLib})

add_executable(${PROJECT_NAME} ${privateSources})

target_include_directories(${PROJECT_NAME} PUBLIC )
target_include_directories(${PROJECT_NAME} PRIVATE  )
target_compile_options(${PROJECT_NAME} PUBLIC -std=c++14 -Wall -Wfloat-conversion)

target_link_libraries(${PROJECT_NAME} FleXdLogClient
)