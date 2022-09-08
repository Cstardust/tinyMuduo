#include<iostream>
#include"Logger.h"

using namespace std;

int main()
{
    LOG_INFO("=====TEST LOG %d=====\n",1);
}



# ifdef TEST_MACRO 
#define CHINA "b b b"

#define MAX(a,b) (a>b ? a : b)

#define MIN(a,b) \
    (a<b \
    ? \
    a \
    : \
    b)

int main()
{
    cout<<"hello muduo"<<endl;

    cout<<CHINA<<endl;

    printf("%s\n",CHINA);

    int x = MAX(1,2);

    cout<<MAX(1,2)<<endl;

    cout<<MIN(1,2)<<endl;
    return 0;
}
#endif
