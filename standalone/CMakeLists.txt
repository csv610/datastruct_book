cmake_minimum_required(VERSION 3.20)
project(DataStruct LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SOURCES
    ch01_modern_cpp.cpp
    ch02_performance_analysis.cpp
    ch03_performance_measurement.cpp
    ch04_linear_lists.cpp
    ch05_matrices.cpp
    ch06_stacks.cpp
    ch07_queues.cpp
    ch08_hashing.cpp
    ch09_trees.cpp
    ch10_priority_queues.cpp
    ch11_search_trees.cpp
    ch12_graphs.cpp
    ch13_strings_tries.cpp
    ch14_segment_fenwick_unionfind.cpp
    ch15_greedy.cpp
    ch16_divide_conquer.cpp
    ch17_dynamic_programming.cpp
    ch18_backtracking.cpp
    ch19_probabilistic.cpp
    ch20_maxflow.cpp
)

foreach(src IN LISTS SOURCES)
    # Extract name without extension for target
    get_filename_component(name ${src} NAME_WE)
    add_executable(${name} ${src})
endforeach()
