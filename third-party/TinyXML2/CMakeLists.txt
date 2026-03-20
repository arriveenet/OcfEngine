cmake_minimum_required(VERSION 3.20)
project(tinyxml2)

set(TARGET tinyxml2)

add_library(${TARGET} "tinyxml2.h" "tinyxml2.cpp")

target_include_directories(${TARGET} PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(${TARGET} PROPERTIES FOLDER ThirdParty)