cmake_minimum_required(VERSION 3.30)

include_directories(${CMAKE_SOURCE_DIR}/ZSSLib/include)
add_library(ZSSLib STATIC bsearch_i.c bsearch_r.c barray.c)
