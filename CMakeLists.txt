
# gcc *.c -o muduo -g -I include -l library

# cmake 版本
cmake_minimum_required(VERSION 3.0)
# 项目名称
project(mymuduo)

# 配置最终可执行文件输出路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
# mymuduo最终编译成.so动态库，设置动态库路径，放在根目录下的lib文件夹下
# set(LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)

# 添加头文件路径
include_directories(${PROJECT_SOURCE_DIR}/include)

# 配置编译选项
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

# 子目录CMakeLists
add_subdirectory(${PROJECT_SOURCE_DIR}/src)
