#ifndef TIME_STAMP
#define TIME_STAMP

#include<string>
#include<time.h>
using std::string;
//  Interace for user : now()
class Timestamp
{
public:
    static const int kMicroSecondsPerSecond = 1000*1000;
    explicit Timestamp(int64_t s = 0);  //  prevent implicit conversion
    static Timestamp now();             //  return a Timestamp object of now
    string toString() const;            //  return string Timestamp -> string
private:
    int64_t  mircoSecondsSinceEpoch_;   //  int64_t <-> time_t <-> long int
};

#endif