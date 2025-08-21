set(APP_NAME unit-tests)

cmake_minimum_required(VERSION 3.14)
project(${APP_NAME})

# GoogleTest requires at least C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
  DOWNLOAD_EXTRACT_TIMESTAMP true
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

if(WIN32)
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
endif()

# record sources, headers, resources...
file(GLOB GAME_SOURCE *.cpp)
file(GLOB GAME_HEADER *.h)

set(TEST_INC_DIRS
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

enable_testing()

list(APPEND TEST_HEADER
    )
    
list(APPEND TEST_SOURCE
    main.cpp
    )

set(all_code_files
    ${TEST_HEADER}
    ${TEST_SOURCE}
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
target_link_libraries(${APP_NAME} GTest::gtest_main)

target_include_directories(${APP_NAME} PRIVATE ${TEST_INC_DIRS})

include(GoogleTest)
gtest_discover_tests(${APP_NAME})

set_target_properties(gmock gmock_main gtest gtest_main
                      PROPERTIES
                      ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
                      LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
                      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
                      FOLDER "ThirdParty"
                      )
