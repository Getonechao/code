cmake_minimum_required(VERSION 3.0.0)
project(ArrayQueue VERSION 0.1.0)

include_directories(include)

add_executable(ArrayQueue src/Queue.c test/Queue_test.c)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

