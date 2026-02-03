cmake_minimum_required(VERSION 3.16)

project(logx LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable( logx
    src/main.cpp
)

target_include_directories(logx
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)
