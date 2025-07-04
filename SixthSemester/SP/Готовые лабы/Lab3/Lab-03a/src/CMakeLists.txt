cmake_minimum_required(VERSION 3.30)

include_directories(${CMAKE_SOURCE_DIR}/ZSSLib/include)
add_executable(Lab-03a Lab-03a.c)
target_link_libraries(Lab-03a ZSSLib)
