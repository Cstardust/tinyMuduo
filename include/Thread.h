#ifndef MY_MUDUO_THREAD
#define MY_MUDUO_THREAD

#include<thread>
#include<memory>
#include<functional>
#include<atomic>
#include<string>
#include "noncopyable.h"

//  Thread类
    //  功能：Thread object 负责 1. 开启线程 + 2. 记录线程信息 如tid 
//  Interface
    //  constructor : 初始化跑在thread上的函数
class Thread : noncopyable
{
public:
    using ThreadFunc = std::function<void(void)>;  
public:
    explicit Thread(const ThreadFunc &func,const std::string& name = "");
    //  保证销毁thread
    ~Thread();  
    //  开启thread
    void start();
    //  等待thread结束
    void join();
    //  thread信息
        //  是否开启了thread
    bool started() const {return started_;}
        //  是否在阻塞等待thread结束
    bool joined() const {return joined_;}
        //  获取Thread object创建的thread的tid
    pid_t tid() const {return tid_;}
        //  获取thread name
    const std::string& name() const {return name_;}
        //  Thread object 数量
    static int numCreated() {return numCreated_;}
private:
    void setDefaultName();
private:
    bool started_;                      //  本Thread所管理的线程是否开启运行
    bool joined_;
    std::unique_ptr<std::thread> thread_;    //  std::thread t_; //  不能直接造thread对象。因为thread对象一构造就开始运行。如果构造了thread对象，那么需要在构造函数初始化，那么一初始化线程就开始运行。
    pid_t tid_;                          //  tid of this thread
    ThreadFunc func_;                   //  如果要赋给func的不是一个参数为void的函数，那么可以通过bind绑定成void的函数。
    std::string name_;                 
    static std::atomic<int> numCreated_;//  已经创建的线程数量
};

#endif