set(APP_NAME cpp-tests)

project(${APP_NAME})

if(WIN32)
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
endif()

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
    EventTest/EventTest.h
    FontTest/FontTest.h
    MeshRendererTest/MeshRendererTest.h
    SpriteTest/SpriteTest.h
    TiledMapTest/TiledMapTest.h
    AppDelegate.h
    BaseTest.h
    MainScene.h
    )
    
list(APPEND GAME_SOURCE
    ActionTest/ActionTest.cpp
    AudioEngineTest/AudioEngineTest.cpp
    ComponentTest/ComponentTest.cpp
    EventTest/EventTest.cpp
    FontTest/FontTest.cpp
    MeshRendererTest/MeshRendererTest.cpp
    SpriteTest/SpriteTest.cpp
    TiledMapTest/TiledMapTest.cpp
    AppDelegate.cpp
    BaseTest.cpp
    main.cpp
    MainScene.cpp
    )

set(all_code_files
    ${GAME_HEADER}
    ${GAME_SOURCE}
    )

add_executable(${APP_NAME} ${all_code_files})

if(XCODE OR VS)
    foreach(_source IN ITEMS ${all_code_files})
        get_filename_component(_source_path "${_source}" PATH)
        string(REPLACE "${CMAKE_SOURCE_DIR}" "" _group_path "${_source_path}")
        string(REPLACE "/" "\\" _group_path "${_group_path}")
        source_group("${_group_path}" FILES "${_source}")
    endforeach()
endif()

target_link_libraries(${APP_NAME} ocfengine)

target_include_directories(${APP_NAME} PRIVATE ${GAME_INC_DIRS})
