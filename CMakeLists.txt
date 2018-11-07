cmake_minimum_required(VERSION 3.12)
project(memory_management C)

set(CMAKE_C_STANDARD 11)

add_executable(tester1 tester1.c frame.c kmalloc.c translation.c ll_double.c)
add_executable(tester2 tester2.c frame.c kmalloc.c translation.c ll_double.c)
add_executable(test_frame test_frame.c frame.c)
add_executable(test_translation test_translation.c frame.c translation.c)
add_executable(test_alloc test_alloc.c frame.c translation.c kmalloc.c ll_double.c)