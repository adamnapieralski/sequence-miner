cmake_minimum_required(VERSION 3.1)

project(sequence_miner LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(extern/yaml)
add_subdirectory(src)
add_subdirectory(app)
