project(thirdparty)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_library(thirdparty INTERFACE)

add_subdirectory(fmt)
target_include_directories(thirdparty INTERFACE "fmt/include")
target_link_libraries(thirdparty INTERFACE fmt)
set_target_properties(fmt
                      PROPERTIES
                      ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
                      LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
                      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
                      FOLDER "ThirdParty"
                      )

add_subdirectory(freetype)
target_include_directories(thirdparty INTERFACE "freetype/include")
target_link_libraries(thirdparty INTERFACE freetype)
set_target_properties(freetype
                      PROPERTIES
                      ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
                      LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
                      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
                      FOLDER "ThirdParty"
                      )

add_subdirectory(glad)
target_include_directories(thirdparty INTERFACE "glad/include")
target_link_libraries(thirdparty INTERFACE glad)
set_target_properties(glad
                      PROPERTIES
                      ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
                      LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
                      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
                      FOLDER "ThirdParty"
                      )

set(GLFW_BUILD_DOCS OFF)
set(GLFW_INSTALL OFF)
add_subdirectory(glfw)
target_link_libraries(thirdparty INTERFACE glfw)
target_include_directories(thirdparty INTERFACE "glfw/include")
set_target_properties(glfw
                      PROPERTIES
                      ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
                      LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
                      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
                      FOLDER "ThirdParty"
                      )

add_subdirectory(glm)
target_include_directories(thirdparty INTERFACE "glm")
set_target_properties(glm
                      PROPERTIES
                      ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
                      LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
                      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
                      FOLDER "ThirdParty"
                      )

target_include_directories(thirdparty INTERFACE "minimp3")

set(ALSOFT_DLOPEN OFF)
set(ALSOFT_UTILS OFF)
set(ALSOFT_EXAMPLES OFF)
set(ALSOFT_INSTALL OFF)
add_subdirectory(OpenAL)
target_include_directories(thirdparty INTERFACE "OpenAL/include")
target_link_libraries(thirdparty INTERFACE OpenAL alsoft.common alsoft.excommon)
set_target_properties(OpenAL
                      alsoft.common
                      alsoft.excommon
                      alsoft.fmt
                      clang-tidy-check
                      PROPERTIES
                      ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
                      LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
                      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
                      FOLDER "ThirdParty"
                      )

add_subdirectory(tinyobjloader)
target_include_directories(thirdparty INTERFACE "tinyobjloader")
target_link_libraries(thirdparty INTERFACE tinyobjloader)
set_target_properties(tinyobjloader
                      PROPERTIES
                      ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
                      LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
                      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
                      FOLDER "ThirdParty"
                      )

add_subdirectory(TinyXML2)
target_include_directories(thirdparty INTERFACE "tinyxml2")
target_link_libraries(thirdparty INTERFACE tinyxml2)
set_target_properties(tinyxml2
                      PROPERTIES
                      ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
                      LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
                      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
                      FOLDER "ThirdParty"
                      )

add_subdirectory(xxHash)
target_include_directories(thirdparty INTERFACE "xxHash")
set_target_properties(xxhash
                      PROPERTIES
                      ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
                      LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
                      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
                      FOLDER "ThirdParty"
                      )

add_subdirectory(zlib)
target_include_directories(thirdparty INTERFACE "zlib")
target_link_libraries(thirdparty INTERFACE zlib)
set_target_properties(zlib
                      zlibstatic
                      PROPERTIES
                      ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
                      LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
                      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
                      FOLDER "ThirdParty"
                      )

add_subdirectory(libpng)
target_include_directories(thirdparty INTERFACE "libpng")
target_link_libraries(thirdparty INTERFACE libpng)
set_target_properties(libpng
                      PROPERTIES
                      ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
                      LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
                      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
                      FOLDER "ThirdParty"
                      )
