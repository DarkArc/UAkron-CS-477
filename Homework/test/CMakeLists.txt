cmake_minimum_required(VERSION 3.1)

# Project Declaration
project(Test C CXX)

# Source Declaration
add_executable(Test
  thread_test.cpp
)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(Test Threads::Threads)

set(CMAKE_CXX_FLAGS "-Wall -std=c++11")
