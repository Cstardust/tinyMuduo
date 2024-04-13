#ifndef TIME_STAMP
#define TIME_STAMP

#include "public.h"
#include <time.h>

//  Interace for user : now()
class Timestamp
{
public:
    explicit Timestamp(int64_t s = 0); //  prevent implicit conversion
    static Timestamp now();            //  return a Timestamp object of now
    string toString() const;           //  return string Timestamp -> string
    int64_t microSecondsSinceEpoch() const { return microSecondsSinceEpoch_; }
    static const int kMicroSecondsPerSecond = 1000 * 1000;      //  1s = 1000 * 1000 us

private:
    int64_t microSecondsSinceEpoch_; //  int64_t <-> time_t <-> long int
};

// 如果是重复定时任务就会对此时间戳进行增加。
inline Timestamp addTime(Timestamp timestamp, double seconds)
{
    // 将延时的秒数转换为微妙
    int64_t delta = static_cast<int64_t>(seconds * Timestamp::kMicroSecondsPerSecond);
    // 返回新增时后的时间戳
    return Timestamp(timestamp.microSecondsSinceEpoch() + delta);
}

inline bool operator<(Timestamp lhs,Timestamp rhs)
{
    return lhs.microSecondsSinceEpoch() < rhs.microSecondsSinceEpoch();
}

inline bool operator==(Timestamp lhs,Timestamp rhs)
{
    return lhs.microSecondsSinceEpoch() == rhs.microSecondsSinceEpoch();
}

#endif