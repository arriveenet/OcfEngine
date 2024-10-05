set(APP_NAME cpp-tests)

project(${APP_NAME})

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

target_link_libraries(${APP_NAME} ocfengine)

target_include_directories(${APP_NAME} PRIVATE ${GAME_INC_DIRS})
