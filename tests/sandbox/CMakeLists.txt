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

list(APPEND GAME_HEADER
    ActionTest/ActionTest.h
    AudioEngineTest/AudioEngineTest.h
    ComponentTest/ComponentTest.h
    DrawShapeTest/DrawShapeTest.h
    EventTest/EventTest.h
    FontTest/FontTest.h
    MeshRendererTest/MeshRendererTest.h
    SpriteTest/SpriteTest.h
    TiledMapTest/TiledMapTest.h
    )

list(APPEND GAME_SOURCE
    ActionTest/ActionTest.cpp
    AudioEngineTest/AudioEngineTest.cpp
    ComponentTest/ComponentTest.cpp
    DrawShapeTest/DrawShapeTest.cpp
    EventTest/EventTest.cpp
    FontTest/FontTest.cpp
    MeshRendererTest/MeshRendererTest.cpp
    SpriteTest/SpriteTest.cpp
    TiledMapTest/TiledMapTest.cpp
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
