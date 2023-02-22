#include "CatServer.h"
#include<signal.h>
#include "CatService.h"

//  对于Server的ctrl + c异常退出
    //  业务类应当负责重置一些状态和信息
        //  比如sql表中用户是否在线的状态
void sigIntHandler(int sig)     //  ctrl + c SIG_INT
{
    CatService::getInstance()->reset();
    exit(0);
}


int main()
{
    signal(SIGINT,sigIntHandler);

    EventLoop loop;
    InetAddress addr(6667);
    CatServer cs(&loop,addr,"CatPlatForm");

    cs.setSubThreadNum(0);
    cs.start();
    //  开启mainloop(mainReactor)
    loop.loop();
}