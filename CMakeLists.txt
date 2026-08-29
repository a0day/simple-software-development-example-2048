cmake_minimum_required(VERSION 3.20)

project(2048 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(2048
    main.cc
    nums.cc
)