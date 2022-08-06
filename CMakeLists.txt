cmake_minimum_required(VERSION 3.18)
project(
  sandbox
  VERSION 0.0.1
  LANGUAGES CXX
  DESCRIPTION "C++ Playground")

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_CXX_EXTENSIONS False)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS NO_OUTPUT_DIRS)

find_package(Threads)

add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_20)

add_library(project_warnings INTERFACE)
target_compile_options(project_warnings INTERFACE -Wall -Wextra -Werror
                                                  -pedantic -Wunused -Wshadow)

add_subdirectory(src)
