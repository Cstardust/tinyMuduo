#!/bin/bash

# 遇到error即终止
set -e

# create directory build if not
if [ ! -d `pwd`/build ]; then
    mkdir `pwd`/build
fi

# 删除build下所有文件
rm `pwd`/build/* -rf   

cd `pwd`/build &&
    cmake .. && 
    make clean &&
    make

# 回到项目根目录
cd ..

# 头文件拷贝到usr/include/miniMuduo 
# .so 拷贝到 /usr/lib
if [ ! -d /usr/include/miniMuduo ]; then
    mkdir /usr/include/miniMuduo
fi

# 头文件拷贝到usr/include/miniMuduo 
for header in `ls ./include/*.h`
do 
    cp $header /usr/include/miniMuduo
done

# .so 拷贝到 /usr/lib
cp `pwd`/lib/libminiMuduo.so /usr/lib


ldconfig