cmake_minimum_required(VERSION 3.16)
project(VermaBookCode LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror -O2")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /WX /std:c++20 /O2")
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(all_tests all_chapters_test.cpp)

enable_testing()
add_test(NAME all_chapters COMMAND all_tests)