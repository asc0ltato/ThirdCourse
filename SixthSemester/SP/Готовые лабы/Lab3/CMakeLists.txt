cmake_minimum_required(VERSION 3.30)
project(Lab-03)

set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/out")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/out")

add_subdirectory(ZSSLib)
add_subdirectory(Lab-03a)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    message("Hello from Windows")

    set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
    set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}")

    add_subdirectory(ZSSDLib)
    add_subdirectory(ZSSDLib++)
    add_subdirectory(Lab-03b-ex)
    add_subdirectory(Lab-03b-im)

    set_target_properties(ZSSDLib++ PROPERTIES LINKER_LANGUAGE CXX)
    set_target_properties(Lab-03b-ex PROPERTIES
        WIN32_EXECUTABLE FALSE
    )
    set_target_properties(Lab-03b-im PROPERTIES
        WIN32_EXECUTABLE FALSE
    )
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    message("Hello from Linux")

    add_subdirectory(LibZSSd)
    add_subdirectory(Lab-03c)
endif()
