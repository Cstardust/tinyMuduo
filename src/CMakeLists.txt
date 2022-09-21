# 源文件
aux_source_directory(./ SRC_LIST)
# 编译可执行文件
add_executable(muduo ${SRC_LIST})

# 编译动态库 最终生成mymuduolib.so
# add_library(mymuduo SHARED ${SRC_LIST})

# add_link_options(-fsanitize=address)    # for asan

target_link_libraries(muduo pthread -fsanitize=address)