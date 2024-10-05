project(thirdparty)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_library(thirdparty INTERFACE)

add_subdirectory(glad)
target_include_directories(thirdparty INTERFACE "glad/include")
set_target_properties(glad
                      PROPERTIES
                      ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
                      LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
                      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
                      FOLDER "ThirdParty"
                      )

set(ALSOFT_DLOPEN OFF)
set(ALSOFT_UTILS OFF)
set(ALSOFT_EXAMPLES OFF)
set(ALSOFT_INSTALL OFF)
add_subdirectory(OpenAL)
target_include_directories(thirdparty INTERFACE "OpenAL/include")
set_target_properties(OpenAL
                      PROPERTIES
                      ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
                      LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
                      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
                      FOLDER "ThirdParty"
                      )

add_subdirectory(tinyobjloader)
target_include_directories(thirdparty INTERFACE "tinyobjloader")
set_target_properties(tinyobjloader
                      PROPERTIES
                      ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
                      LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
                      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
                      FOLDER "ThirdParty"
                      )

add_subdirectory(TinyXML2)
target_include_directories(thirdparty INTERFACE "tinyxml2")
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
