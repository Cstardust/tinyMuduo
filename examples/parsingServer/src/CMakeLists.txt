set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -g)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

aux_source_directory(./ SRC_LIST)           # 3. 源文件
# add_subdirectory(/home/shc/Muduo/examples/http/pybind11)

add_executable(InferenceServer ${SRC_LIST})      # 1. 生成可执行文件

find_package(Python 3.10 COMPONENTS Interpreter Development REQUIRED)

# target_link_libraries(InferenceServer tinyMuduo pybind11::embed pthread)
target_link_libraries(InferenceServer pthread tinyMuduo python3.10 dl pthread util)

# https://cloud.tencent.com/developer/ask/sof/107018498
