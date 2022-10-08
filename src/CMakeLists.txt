# 源文件
aux_source_directory(./ SRC_LIST)

# 编译可执行文件
# add_executable(muduo ${SRC_LIST})

# 编译动态库 最终生成miniMuduolib.so
add_library(miniMuduo SHARED ${SRC_LIST})

# target_link_libraries(muduo pthread -fsanitize=address)