cmake_minimum_required(VERSION 3.10)

project(toy_interpreter)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_SKIP_INSTALL_RULES True)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../bin)
add_executable(interpreter ../src/main.cpp)
