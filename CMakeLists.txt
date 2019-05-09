cmake_minimum_required(VERSION 3.10)
project(tread_pool)

set(CMAKE_CXX_STANDARD 14)

add_executable(thread_pool main.cpp thread_pool.cpp thread_pool.h thread_safe_queue.cpp thread_safe_queue.h)