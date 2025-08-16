set(APP_NAME 2d-action)

project(${APP_NAME})

file(GLOB GAME_SOURCE *.cpp)
file(GLOB GAME_HEADER *.h)


set(all_code_files
    ${GAME_HEADER}
    ${GAME_SOURCE}
    )

add_executable(${APP_NAME} ${all_code_files})

target_link_libraries(${APP_NAME} ocfengine)

target_include_directories(${APP_NAME} PRIVATE ${GAME_INC_DIRS})