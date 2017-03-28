cmake_minimum_required(VERSION 3.6)
project(integral_final)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp)
add_executable(integral_final ${SOURCE_FILES})

TARGET_LINK_LIBRARIES( integral_final pthread)