cmake_minimum_required(VERSION 3.25)
project(stl)

set(CMAKE_CXX_STANDARD 14)

add_executable(stl main.cpp frank_type_traits.h frank_alloc.h frank_iterators.h frank_vector.h frank.h frank_uninitialize.h frank_algobase.h frank_list.h frank_deque.h frank_stack.h frank_queue.h frank_function.h frank_heap.h)
