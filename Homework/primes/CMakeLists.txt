cmake_minimum_required(VERSION 3.1)

# Project Declaration
project(Primes C CXX)

# Source Declaration
add_executable(Primes
  primes.cpp
  stdafx.cpp
)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(Primes Threads::Threads)

set(CMAKE_CXX_FLAGS "-Wall -std=c++11")
