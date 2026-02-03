cmake_minimum_required(VERSION 3.16)

project(logx LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(third_party)

add_library(logx_core
        src/core/analyzer.cpp
)

target_include_directories( logx_core
        PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

add_executable( logx
        src/main.cpp
)

target_link_libraries( logx
        PRIVATE
        logx_core
)

add_subdirectory(tests)