cmake_minimum_required(VERSION 3.10)
project(tread_pool)

set(CMAKE_CXX_STANDARD 14)

add_executable(tread_pool main.cpp thread_safe_queue.cpp thread_safe_queue.h)