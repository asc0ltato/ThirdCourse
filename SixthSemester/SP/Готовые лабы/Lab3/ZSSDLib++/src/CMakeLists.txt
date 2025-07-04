cmake_minimum_required(VERSION 3.30)

include_directories(${CMAKE_SOURCE_DIR}/ZSSDLib++/include)
add_library(ZSSDLib++ SHARED ZSSDLib++.cpp bsearch_i.cpp bsearch_r.cpp barray.cpp)
