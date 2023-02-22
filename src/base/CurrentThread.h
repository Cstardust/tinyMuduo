#ifndef CURRENT_THREAD
#define CURRENT_THREAD

#include<unistd.h>
#include<sys/syscall.h>

//  获取当前正在运行的线程的tid
    //  易知 调用currentthread::tid()的线程一定是当前正在cpu上运行的线程
    //  又因为 t_cacheTid是一个 每个线程独立的tid
    //  所以 该线程调用tid 获取 t_cachedTid 得到的一定是该线程自己的tid，而不是别的线程的tid
namespace CurrentThread
{
    //  每个线程独立的全局变量
    extern thread_local int t_cachedTid;        
    //  更新（获取）tid
    void cacheTid();   
    //  系统调用没提供这个接口
    pid_t gettid();                              
    //  inline is used because of it will be included
    inline pid_t tid()
    {
        if(__glibc_unlikely(t_cachedTid == 0))
        // if(__builtin_expect(t_cachedTid == 0, 0))       //  告诉编译器 t_cacheTid == 0这件事的值 很有可能为0. 帮助编译器进行分支预测
        {
            cacheTid();
        }
        return t_cachedTid;
    }
}


#endif