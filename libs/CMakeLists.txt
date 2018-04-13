cmake_minimum_required(VERSION 3.0)

add_subdirectory(fleXdLoggerCommon)
add_subdirectory(fleXdLoggerClient)
set(fleXdMessageTypePublicInc "${fleXdMessageTypePublicInc}" PARENT_SCOPE)