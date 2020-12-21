cmake_minimum_required(VERSION 3.1)

project(sequence_miner LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

set(CATCH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/extern/catch2)
add_library(Catch2::Catch IMPORTED INTERFACE)
set_property(TARGET Catch2::Catch PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${CATCH_INCLUDE_DIR}")

add_subdirectory(extern/yaml)
add_subdirectory(src)
add_subdirectory(app)
add_subdirectory(tests)
