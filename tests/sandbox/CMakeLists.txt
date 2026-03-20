# ==================================================================================================
# CMake
# ==================================================================================================
cmake_minimum_required (VERSION 3.20)

set(APP_NAME sandbox)

project(${APP_NAME})

# ==================================================================================================
# Sources and headers
# ==================================================================================================

# record sources, headers, resources...
file(GLOB GAME_SOURCE *.cpp)
file(GLOB GAME_HEADER *.h)

set(GAME_INC_DIRS
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

set(all_code_files
    ${GAME_HEADER}
    ${GAME_SOURCE}
    )

# ==================================================================================================
# Includes & target definition
# ==================================================================================================

add_executable(${APP_NAME} ${all_code_files})

target_link_libraries(${APP_NAME} ocfengine)

target_include_directories(${APP_NAME} PRIVATE ${GAME_INC_DIRS})


# ==================================================================================================
# Compiler flags
# ==================================================================================================
if(WIN32)
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
endif()
