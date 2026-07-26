cmake_minimum_required(VERSION 3.16)
project(DataStructBookCode LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -O2")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /std:c++20 /O2")
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

enable_testing()

set(TESTS
    ch01_test
    ch02_test
    ch03_test
    ch04_test
    ch05_test
    ch06_test
    ch07_test
    test_ch07_deque
    test_ch11_red_black_tree
    test_ch05_sparse_matrix
    test_ch13_suffix_array
)

foreach(TEST ${TESTS})
    add_executable(${TEST} tests/${TEST}.cpp)
    add_test(NAME ${TEST} COMMAND ${TEST})
endforeach()
