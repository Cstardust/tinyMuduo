# set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -g)  # 2. 编译选项在顶层CMakeLists中已经配置过

aux_source_directory(./ SRC_LIST)           # 3. 源文件
aux_source_directory(./db SRC_LIST)
aux_source_directory(./model SRC_LIST)

add_executable(CatServer ${SRC_LIST})      # 1. 生成可执行文件

target_link_libraries(CatServer mysqlclient tinyMuduo pthread) # 6. 要链接的库文件名