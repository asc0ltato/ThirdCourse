cmake_minimum_required(VERSION 3.30.0)
set(CMAKE_MODULE_PATH "")
project(CheckCSharp CSharp)
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/result.cmake"
  "set(CMAKE_CSharp_COMPILER \"${CMAKE_CSharp_COMPILER}\")\n"
  
  )