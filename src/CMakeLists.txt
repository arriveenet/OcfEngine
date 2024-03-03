project("OcfEngine_lib")

include("2d/CmakeLists.txt")
include("base/CmakeLists.txt")
include("platform/CmakeLists.txt")
include("renderer/CmakeLists.txt")
include("scene/CmakeLists.txt")

set("OCF_HEADER"
    "OcfEngine.h"
    ${OCF_2D_HEADER}
    ${OCF_BASE_HEADER}
    ${OCF_PLATFORM_HEADER}
    ${OCF_RENDERER_HEADER}
    ${OCF_SCENE_HEADER}
    )

set("OCF_SRC"
    ${OCF_2D_SRC}
    ${OCF_BASE_SRC}
    ${OCF_PLATFORM_SRC}
    ${OCF_RENDERER_SRC}
    ${OCF_SCENE_SRC}
    )

list(APPEND OCF_SRC ${OCF_HEADER})

add_library(ocf_engine ${OCF_SRC})

target_include_directories(ocf_engine
    PUBLIC ${OCF_ROOT_PATH}
    PUBLIC ${OCF_ROOT_PATH}/src
    PUBLIC ${OCF_ROOT_PATH}/third_party/glad/include
    )

