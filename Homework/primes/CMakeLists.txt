cmake_minimum_required(VERSION 2.8)

# Project Declaration
project(Primes CXX)

# Source Declaration
add_executable(Primes
  primes.cpp
  stdafx.cpp
)

set(CMAKE_CXX_FLAGS "-Wall -std=c++11")
