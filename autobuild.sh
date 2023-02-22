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

# 头文件拷贝到usr/include/tinyMuduo 
# .so 拷贝到 /usr/lib
if [ ! -d /usr/include/tinyMuduo ]; then
    mkdir /usr/include/tinyMuduo
fi

# 头文件拷贝到usr/include/tinyMuduo 
for header in `ls ./src/base/*.h`
do 
    cp $header /usr/include/tinyMuduo
done

for header in `ls ./src/http/*.h`
do 
    cp $header /usr/include/tinyMuduo
done

# .so 拷贝到 /usr/lib
cp `pwd`/lib/libtinyMuduo.so /usr/lib

ldconfig