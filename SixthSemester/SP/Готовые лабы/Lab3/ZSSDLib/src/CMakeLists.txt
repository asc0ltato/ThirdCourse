cmake_minimum_required(VERSION 3.30)

include_directories(${CMAKE_SOURCE_DIR}/ZSSDLib/include)
add_library(ZSSDLib SHARED ZSSDLib.c bsearch_i.c bsearch_r.c barray.c)

if (WIN32)
    target_link_options(ZSSDLib PRIVATE "/DEF:${CMAKE_SOURCE_DIR}/ZSSDLib/src/ZSSDLib.def")
endif()
