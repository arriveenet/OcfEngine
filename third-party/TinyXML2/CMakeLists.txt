project(tinyxml2)

add_library(tinyxml2 "tinyxml2.h" "tinyxml2.cpp")

target_include_directories(tinyxml2 PUBLIC .)