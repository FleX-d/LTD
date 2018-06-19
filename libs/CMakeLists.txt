cmake_minimum_required(VERSION 3.0)

add_subdirectory(fleXdLoggerCommon)
add_subdirectory(fleXdLoggerClient)

if(${WITH_DLT})
    add_subdirectory(fleXdLoggerDlt)
endif(${WITH_DLT})
