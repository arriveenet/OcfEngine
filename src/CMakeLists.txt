if(NOT DEFINED _OCF_ENGINE_LIB)
    set(_OCF_ENGINE_LIB ocfengine)
endif()

project("OcfEngine_lib")

if(WIN32)
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(ENGINE_BINARY_PATH ${PROJECT_BINARY_DIR}/engine)

include("2d/CMakeLists.txt")
include("3d/CMakeLists.txt")
include("audio/CMakeLists.txt")
include("base/CMakeLists.txt")
include("input/CMakeLists.txt")
include("platform/CMakeLists.txt")
include("renderer/CMakeLists.txt")
include("ui/CMakeLists.txt")

set("OCF_HEADER"
    "OcfEngine.h"
    ${OCF_2D_HEADER}
    ${OCF_3D_HEADER}
    ${OCF_AUDIO_HEADER}
    ${OCF_BASE_HEADER}
    ${OCF_INPUT_HEADER}
    ${OCF_PLATFORM_HEADER}
    ${OCF_RENDERER_HEADER}
    ${OCF_UI_HEADER}
)

set("OCF_SRC"
    ${OCF_2D_SRC}
    ${OCF_3D_SRC}
    ${OCF_AUDIO_SRC}
    ${OCF_BASE_SRC}
    ${OCF_INPUT_SRC}
    ${OCF_PLATFORM_SRC}
    ${OCF_RENDERER_SRC}
    ${OCF_UI_SRC}
)

list(APPEND OCF_SRC ${OCF_HEADER})

add_library(${_OCF_ENGINE_LIB} ${OCF_SRC})

add_subdirectory(${OCF_ROOT_PATH}/thirdparty ${ENGINE_BINARY_PATH}/thirdparty)
target_link_libraries(${_OCF_ENGINE_LIB} thirdparty)

if(XCODE OR VS)
    foreach(_source IN ITEMS ${OCF_SRC})
        get_filename_component(_source_path "${_source}" PATH)
        string(REPLACE "${CMAKE_SOURCE_DIR}" "" _group_path "${_source_path}")
        string(REPLACE "/" "\\" _group_path "${_group_path}")
        source_group("${_group_path}" FILES "${_source}")
    endforeach()
endif()

target_include_directories(${_OCF_ENGINE_LIB}
    PUBLIC ${OCF_ROOT_PATH}
    PUBLIC ${OCF_ROOT_PATH}/src
    PUBLIC ${OCF_ROOT_PATH}/third_party
    INTERFACE ${OCF_ROOT_PATH}/thirdparty
)

set_target_properties(${_OCF_ENGINE_LIB}
                      PROPERTIES
                      ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
                      LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
                      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
                      FOLDER "Engine"
                      )
