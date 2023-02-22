# gcc *.c -o CatServer -g -I include -l library

# cmake 版本
cmake_minimum_required(VERSION 3.0)
# 项目名称
project(CatServer)

# 配置最终可执行文件输出路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# 添加头文件路径
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include/db)
include_directories(${PROJECT_SOURCE_DIR}/include/model)
include_directories(${PROJECT_SOURCE_DIR}/thirdparty)

# 配置编译选项
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11 -Werror")


# 配置编译选项 for asan
# add_compile_options(-O0 -ggdb -std=c++14 -Wall -Wextra -mavx2
#     -fsanitize=address          # 开启内存越界检测
#     -fno-omit-frame-pointer     # 去使能栈溢出保护
#     -fno-optimize-sibling-calls # 
#     -fsanitize-address-use-after-scope
#     -fsanitize-recover=address) # 一般后台程序为保证稳定性，不能遇到错误就简单退出，而是继续运行，采用该选项支持内存出错之后程序继续运行，需要叠加设置ASAN_OPTIONS=halt_on_error=0才会生效；若未设置此选项，则内存出错即报错退出

# 子目录CMakeLists
add_subdirectory(${PROJECT_SOURCE_DIR}/src)


