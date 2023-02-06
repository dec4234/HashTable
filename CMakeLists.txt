cmake_minimum_required(VERSION 3.24)
project(HashTable C)

set(CMAKE_C_STANDARD 17)

add_executable(HashTable hash_table.c hash_table.h prime.c prime.h xmalloc.c xmalloc.h)
