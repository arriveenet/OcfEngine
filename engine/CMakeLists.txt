cmake_minimum_required (VERSION 3.20)
project(OcfEngine_lib)

set(TARGET ocfengine)
set(PUBLIC_HDR_DIR include)

# ==================================================================================================
# Options
# ==================================================================================================
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

# ==================================================================================================
# functions
# ==================================================================================================
function( add_filtered_public_h filterName fileString )
    file(GLOB TMP_FILES ${fileString})
    set(PUBLIC_HEADER_FILES ${PUBLIC_HEADER_FILES} ${TMP_FILES} PARENT_SCOPE)
    source_group(${filterName} FILES ${TMP_FILES})
endfunction( add_filtered_public_h )

function( add_filtered_private_h filterName fileString )
    file(GLOB TMP_FILES ${fileString})
    set(PRIVATE_HEADER_FILES ${PRIVATE_HEADER_FILES} ${TMP_FILES} PARENT_SCOPE)
    source_group(${filterName} FILES ${TMP_FILES})
endfunction( add_filtered_private_h )

function( add_filtered_src filterName fileString )
    file(GLOB TMP_FILES ${fileString})
    set(SOURCE_FILES ${SOURCE_FILES} ${TMP_FILES} PARENT_SCOPE)
    source_group(${filterName} FILES ${TMP_FILES})
endfunction( add_filtered_src )

function( add_filtered_std relativePath )
    string(REPLACE "/" "\\" filterPart ${relativePath})
    # Add public headers
    add_filtered_public_h(
        "Public Header Files\\${filterPart}"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/ocf/${relativePath}/*.h")
    add_filtered_public_h(
        "Public Header Files\\${filterPart}"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/ocf/${relativePath}/*.inl")
    set(PUBLIC_HEADER_FILES ${PUBLIC_HEADER_FILES} PARENT_SCOPE)

    # Add private headers
    add_filtered_private_h(
        "Private Header Files\\${filterPart}"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/${relativePath}/*.h")
    set(PRIVATE_HEADER_FILES ${PRIVATE_HEADER_FILES} PARENT_SCOPE)

    # Add source files
    add_filtered_src(
        "Source Files\\${filterPart}"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/${relativePath}/*.cpp")
    set(SOURCE_FILES ${SOURCE_FILES} PARENT_SCOPE)
endfunction( add_filtered_std )

# ==================================================================================================
# Sources and headers
# ==================================================================================================
add_filtered_std("2d")
add_filtered_std("3d")
add_filtered_std("audio")
add_filtered_std("core")
add_filtered_std("input")
add_filtered_std("math")
add_filtered_std("platform")
add_filtered_std("renderer")
add_filtered_std("renderer/backend")
add_filtered_std("scene")
add_filtered_std("ui")

if(OCF_USE_GL)
    add_filtered_std("renderer/backend/opengl")
endif()

# ==================================================================================================
# Includes & target definition
# ==================================================================================================
include_directories(${PUBLIC_HEADER_FILES})
include_directories(src)

#building a library
add_library(${TARGET} ${PUBLIC_HEADER_FILES} ${PRIVATE_HEADER_FILES} ${SOURCE_FILES})
if(BUILD_SHARED_LIBS)
    target_compile_definitions(${TARGET} PRIVATE OCF_LIBRARY_EXPORTS)
else()
    target_compile_definitions(${TARGET} PUBLIC OCF_LIBRARY_STATIC)
endif()

target_include_directories(${TARGET} PUBLIC ${PUBLIC_HDR_DIR})

set_target_properties(${TARGET} PROPERTIES FOLDER Engine)

# ==================================================================================================
# Dependencies
# ==================================================================================================
target_link_libraries(${TARGET} PUBLIC fmt)
target_link_libraries(${TARGET} PUBLIC freetype)
target_link_libraries(${TARGET} PUBLIC glad)
target_link_libraries(${TARGET} PUBLIC glfw)
target_link_libraries(${TARGET} PUBLIC glm)
target_link_libraries(${TARGET} PUBLIC Ogg::ogg)
target_link_libraries(${TARGET} PUBLIC libpng)
target_link_libraries(${TARGET} PUBLIC jpeg-static)
target_link_libraries(${TARGET} PUBLIC vorbis vorbisenc vorbisfile)
target_link_libraries(${TARGET} PUBLIC OpenAL)
target_link_libraries(${TARGET} PUBLIC tinyobjloader)
target_link_libraries(${TARGET} PUBLIC tinyxml2)
target_link_libraries(${TARGET} PUBLIC minimp3)
target_link_libraries(${TARGET} PUBLIC xxHash)
target_link_libraries(${TARGET} PUBLIC zlib)

# ==================================================================================================
# Compiler flags
# ==================================================================================================
if (MSVC)
    set(OCF_WARNINGS /W4)
    target_compile_options(${TARGET} PRIVATE "$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    target_compile_options(${TARGET} PRIVATE "$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
else()
    set(OCF_WARNINGS
            -Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers
            -Wextra-semi -Wdeprecated -Wundef
            -Wnon-virtual-dtor -Wimplicit-fallthrough
            -Wno-template-id-cdtor
    )
endif()

target_compile_options(${TARGET} PRIVATE ${OCF_WARNINGS})

# ==================================================================================================
# Sub-projects
# ==================================================================================================
# add_subdirectory(test)
