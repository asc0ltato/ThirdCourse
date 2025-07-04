cmake_minimum_required(VERSION 3.30)

include_directories(${CMAKE_SOURCE_DIR}/LibZSSd/include)
add_library(LibZSSd SHARED LibZSSd.c)
