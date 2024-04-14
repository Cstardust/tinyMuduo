# tinyMuduo

- tinyMuduo : 学习muduo设计, 现一个TCP Server
- 采用Multi-Reactor模型. 每个reactor通过nonblocking配合IO多路复用函数epoll实现. 
- 通过eventfd跨线程通信, MainReactor将连接分配给SubReactor
- 线程使用遵循one loop per thread
- 通过runInLoop/queueInLoop(callback)来为Reactor异步的分配任务
- 上层实现了个简易的http服务器, 通过状态机解析http请求, 支持get,支持短连接
- 通过C++11替换对于Boost::bind,function等依赖
- 优化方向:
  - 增加定时器, 实现对于定时事件的处理.
  - 拓展日志模块, 实现线程安全且高效的异步日志(目前仅仅就是宏替换)
- 压测结果如下
  - 云服务器配置 : 2核4G
  - Server设置1个mainReactor,1个subReactor
    ![](./2023-02-22-12-23-54.png)
- 使用 : 
  - `./autobuild.sh` : 在/usr/lib下生成libtinyMuduo.so. 头文件拷贝在/usr/include/tinyMuduo
  - 使用时include头文件 并链接 -ltinyMuduo
  - 使用样例见example
- 博客: https://cstardust.github.io/categories/Muduo/
