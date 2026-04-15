cmake_minimum_required(VERSION 4.3)
project(voyc VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(voyc
    src/main.cpp
    src/lexer.cpp
)

target_include_directories(voyc PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_compile_options(voyc PRIVATE
    $<$<CXX_COMPILER_ID:Clang>:-Wall;-Wextra;-Wpedantic>
)
