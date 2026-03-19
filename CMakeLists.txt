# ==================================================================================================
# CMake
# ==================================================================================================
cmake_minimum_required (VERSION 3.20)

# ==================================================================================================
# Project declaration
# ==================================================================================================
project ("OcfEngine")

if (CMAKE_GENERATOR MATCHES "Visual Studio")
    set(VS TRUE)
else()
    set(VS FALSE)
endif()

if (CMAKE_GENERATOR MATCHES "Xcode")
    set(XCODE TRUE)
else()
    set(XCODE FALSE)
endif()

# ==================================================================================================
# Options
# ==================================================================================================
option(OCF_BUILD_TESTS "Build cpp-tests" ON)

option(OCF_USE_GL "Use OpenGL" ON)

# ==================================================================================================
#  Paths
# ==================================================================================================
set(OCF_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR})
set(OCF_THIRD_PARTY_PATH ${OCF_ROOT_PATH}/third-party)

# ==================================================================================================
# General compiler flags
# ==================================================================================================
set(CXX_STANDARD "-std=c++20")

if (MSVC)
    set(CXX_STANDARD "/std:c++20")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_STANDARD} /W0 /Zc:__cplusplus")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_STANDARD}
        -fstrict-aliasing -Wno-unknown-pragmas
        -Wno-unused-function -Wno-deprecated-declarations")
endif()

# ==================================================================================================
# Sub-projects
# ==================================================================================================
add_subdirectory(${OCF_THIRD_PARTY_PATH}/fmt)
add_subdirectory(${OCF_THIRD_PARTY_PATH}/freetype)
add_subdirectory(${OCF_THIRD_PARTY_PATH}/glad)
add_subdirectory(${OCF_THIRD_PARTY_PATH}/glfw)
add_subdirectory(${OCF_THIRD_PARTY_PATH}/glm)
add_subdirectory(${OCF_THIRD_PARTY_PATH}/googletest/ocf)
add_subdirectory(${OCF_THIRD_PARTY_PATH}/libjpeg-turbo)
add_subdirectory(${OCF_THIRD_PARTY_PATH}/libogg)
add_subdirectory(${OCF_THIRD_PARTY_PATH}/libpng)
add_subdirectory(${OCF_THIRD_PARTY_PATH}/libvorbis)
add_subdirectory(${OCF_THIRD_PARTY_PATH}/openal-soft)
add_subdirectory(${OCF_THIRD_PARTY_PATH}/minimp3)
add_subdirectory(${OCF_THIRD_PARTY_PATH}/tinyobjloader)
add_subdirectory(${OCF_THIRD_PARTY_PATH}/tinyxml2)
add_subdirectory(${OCF_THIRD_PARTY_PATH}/xxHash)
add_subdirectory(${OCF_THIRD_PARTY_PATH}/zlib)

add_subdirectory (${OCF_ROOT_PATH}/engine)

if (OCF_BUILD_TESTS)
    macro(add_test_target target_name dir)
        add_subdirectory(${dir} ${CMAKE_BINARY_DIR}/tests/${target_name})
        set_target_properties(${target_name} PROPERTIES FOLDER "Apps")
    endmacro()
    
     add_test_target(cpp-tests ${OCF_ROOT_PATH}/tests/cpp-tests)
     # add_test_target(unit-tests ${OCF_ROOT_PATH}/tests/unit-tests)
endif()
