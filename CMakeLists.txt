cmake_minimum_required(VERSION 3.10)

add_library(project_settings INTERFACE)
target_compile_features(project_settings INTERFACE cxx_std_23)

project(playground)

find_package(fmt CONFIG REQUIRED)
find_package(SDL2 CONFIG REQUIRED)

add_subdirectory(src)
