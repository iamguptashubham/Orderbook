cmake_minimum_required(VERSION 3.15)

project(Orderbook VERSION 1.0 LANGUAGES CXX)

#C++ standards
set(CMAKE_CXX_STANADARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#Enable compile commands on
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#create executable
add_executable(Orderbook
    src/main.cpp
    src/helper.cpp
    src/orderbook.cpp
)

#include directories
target_include_directories(Orderbook PRIVATE include)