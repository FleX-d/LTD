cmake_minimum_required(VERSION 3.0)
project(LogExample VERSION "0.0.1")
set(PROJECT_DESCRIPTION "Logger example for fleXd project")

message(STATUS "***" )
message(STATUS "*  " ${PROJECT_NAME})
message(STATUS "***" )
SET(privateSources
    main.cpp
)

message(STATUS "fleXdLoggerClientPublicInc: " ${fleXdLoggerClientPublicInc})
message(STATUS "fleXdLoggerClientLib: " ${fleXdLoggerClientLib})

add_executable(${PROJECT_NAME} ${privateSources})
target_include_directories(${PROJECT_NAME} PUBLIC )
target_include_directories(${PROJECT_NAME} PRIVATE ${fleXdLoggerClientPublicInc})
target_compile_options(${PROJECT_NAME} PUBLIC -std=c++14 -Wall -Wfloat-conversion)

target_link_libraries(${PROJECT_NAME} fleXdLoggerClient
)

add_dependencies(${PROJECT_NAME} fleXdLoggerClient)