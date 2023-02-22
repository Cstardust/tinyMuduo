# 源文件
aux_source_directory(./ SRC_LIST)
aux_source_directory(../http SRC_LIST)

# 编译动态库 最终生成tinyMuduolib.so
add_library(tinyMuduo SHARED ${SRC_LIST})

# target_link_libraries(muduo pthread -fsanitize=address)