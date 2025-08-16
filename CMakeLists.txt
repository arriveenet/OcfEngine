# CMakeList.txt : 最上位の CMake プロジェクト ファイル。グローバル構成を行います
# また、サブプロジェクトをここに含めます。
#
cmake_minimum_required (VERSION 3.8)

# C++17を使用
set(CMAKE_CXX_STANDARD 17)

# サポートされている場合は、MSVC コンパイラのホット リロードを有効にします。
if (POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("OcfEngine")

set(OCF_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR})

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

option(OCF_BUILD_TESTS "Build cpp-tests" ON)
option(OCF_BUILD_SAMPLES "Build samples" ON)

# サブプロジェクトを含めます。
add_subdirectory (${OCF_ROOT_PATH}/src)

if (OCF_BUILD_TESTS)
    macro(add_test_target target_name dir)
        add_subdirectory(${dir} ${CMAKE_BINARY_DIR}/tests/${target_name})
        set_target_properties(${target_name} PROPERTIES FOLDER "Apps")
    endmacro()
    
     add_test_target(cpp-tests ${OCF_ROOT_PATH}/tests/cpp-tests)
     add_test_target(unit-tests ${OCF_ROOT_PATH}/tests/unit-tests)
endif()

if (OCF_BUILD_SAMPLES)
    add_subdirectory(samples/2d-action)
endif()
