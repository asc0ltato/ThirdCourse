cmake_minimum_required(VERSION 3.30)

include_directories(${CMAKE_SOURCE_DIR}/ZSSDLib/include)
add_executable(Lab-03b-im WIN32 Lab-03b-im.c)
target_link_libraries(Lab-03b-im ZSSDLib)
