
cmake_minimum_required(VERSION 3.0)
# project
project(example)
# src
aux_source_directory(./ SRC_LIST)
# 配置编译选项
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/)

add_executable(epoll ${SRC_LIST})