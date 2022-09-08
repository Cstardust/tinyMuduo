# 源文件
aux_source_directory(./ SRC_LIST)
# 编译可执行文件
add_executable(muduo ${SRC_LIST})
# add_executable(muduo main.cpp Logger.cpp)
# 编译动态库 最终生成mymuduolib.so
# add_library(mymuduo SHARED ${SRC_LIST})