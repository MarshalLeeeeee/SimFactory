#pragma once

#ifndef __LOGUTIL_H__
#define __LOGUTIL_H__

// No project header is allowed
#include <fstream>
#include <string>
#include <chrono>
#include <queue>
#include "ThreadUtil.h"
// No project header is allowed

/* Log:
* prefix: describe log type
* now: describe time stamp of log action
* txt: log content
*/
struct Log {
    std::string prefix;
    std::string txt;
    std::chrono::time_point<std::chrono::system_clock> now;
    Log(std::string prefix, std::string txt, std::chrono::time_point<std::chrono::system_clock> now);
};

/* multi thread logger
 * main thread as producter pushing log to queue
 * off thread as comsumer comsume log to file stream
 */
class Logger : public ThreadPool {
public:
    static Logger& getInstance() {
        static Logger logger;
        return logger;
    }
private:
    Logger();
    ~Logger();
    Logger(const Logger&) = delete;
    Logger(Logger&&) = delete;
    Logger& operator=(const Logger&) = delete;
    Logger& operator=(Logger&&) = delete;
private:
    /* invoke by sub thread, append logs not faster than 1.0 second */
    void work();

public:
    void log(const char* txt);
    void error(const char* txt);
    void debug(const char* txt);
protected:
    /* append log to file */
    void append(const Log& log);
private:
    /* file out stream */
    std::ofstream f;
    /* log queue */
    std::queue<Log> logQ;
    /* last apeend time */
    std::chrono::time_point<std::chrono::system_clock> lastLogTime;
};

#endif
