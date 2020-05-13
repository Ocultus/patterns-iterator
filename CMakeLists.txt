cmake_minimum_required(VERSION 3.16)
project(Task4)

set(CMAKE_CXX_STANDARD 20)

include_directories("${PROJECT_SOURCE_DIR}/src")

add_subdirectory(tst)
add_subdirectory(googletest)