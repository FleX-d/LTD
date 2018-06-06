cmake_minimum_required(VERSION 3.0)

add_subdirectory(fleXdLoggerCommon)
add_subdirectory(fleXdLoggerClient)

if(${LOG_TO_DLT})
    add_subdirectory(fleXdLoggerDlt)
endif(${LOG_TO_DLT})
