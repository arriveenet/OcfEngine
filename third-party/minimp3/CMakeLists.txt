cmake_minimum_required(VERSION 3.20)
project(minimp3)

set(TARGET minimp3)

add_library(${TARGET} "minimp3.h" "minimp3_ex.h" "minimp3.cpp")

target_include_directories(${TARGET} PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(${TARGET} PROPERTIES FOLDER ThirdParty)
