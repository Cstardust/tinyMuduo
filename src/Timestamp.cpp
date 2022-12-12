#include"Timestamp.h"
#include <sys/time.h>



Timestamp::Timestamp(int64_t s /* =0 */):mircoSecondsSinceEpoch_(s){}

Timestamp Timestamp::now()
{
    struct timeval tv;
    gettimeofday(&tv,nullptr);
    return Timestamp(tv.tv_sec * kMicroSecondsPerSecond + tv.tv_usec);
}

string Timestamp::toString() const
{
    int64_t secsSinceEpoch = mircoSecondsSinceEpoch_ / kMicroSecondsPerSecond;
    tm *t = localtime(&secsSinceEpoch);
    char buf[512];
    sprintf(buf,
            "%04d-%02d-%02d-%02d-%02d-%02d.%06ld",
            t->tm_year + 1900,
            t->tm_mon + 1,
            t->tm_mday,
            t->tm_hour,
            t->tm_min,
            t->tm_sec,
            mircoSecondsSinceEpoch_ % kMicroSecondsPerSecond);
    return string(buf);
}