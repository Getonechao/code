cmake_minimum_required(VERSION 3.1)

project(wiringpi_demo VERSION 0.0.0.1 )

#C/C++标准
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_STANDARD 11)

#设置编译器
set (CMAKE_C_COMPILER "/usr/bin/gcc")
set (CMAKE_CXX_COMPILER "/usr/bin/g++")

#lib&&bin
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

#release debug
set(CMAKE_BUILD_TYPE Release #[[Release | Debug| RelWithDebInfo |MinSizeRel]])
# add_compile_options()#等同CMAKE_CXXFLAGS_RELESE,前者可以对所有的编译器设置，后者只能是C++编译器


# include_directories(
# 目录
# )

# aux_source_directory(目录 变量)

# FIND_LIBRARY(#变量 libceres.so #目录)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} 
/usr/local/lib/libwiringPi.so
/usr/local/lib/libwiringPiDev.so
)