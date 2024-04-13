cmake_minimum_required(VERSION 3.0)
project(InferenceServer)

include_directories(${PROJECT_SOURCE_DIR}/)
# include_directories(/usr/local/include/python3.10)
include_directories(${PROJECT_SOURCE_DIR}/thirdparty/pybind11/include)

add_subdirectory(thirdparty/pybind11)
add_subdirectory(src)