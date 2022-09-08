#include "Logger.h"
#include "Timestamp.h"
#include <functional>

using std::function;

Logger &Logger::getInstance()
{
    static Logger logger;
    return logger;
}

void Logger::setLogLevel(LogLevel level)
{
    logLevel_ = level;
}

void Logger::log(const string &msg)
{
    cout << levelName_[logLevel_]
         << " "
         << Timestamp::now().toString()
         << " : "
         << msg << endl;
}

Logger::Logger()
{
    levelName_.insert({INFO, "[INFO]"});
    levelName_.insert({ERROR, "[ERROR]"});
    levelName_.insert({FATAL, "[FATAL]"});
    levelName_.insert({DEBUG, "[DEBUG]"});
}