cmake_minimum_required(VERSION 3.9)
project(qml4qwidgets)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC TRUE)
  
add_subdirectory(lib)
add_subdirectory(runner)
add_subdirectory(example)
