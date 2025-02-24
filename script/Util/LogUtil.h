#pragma once

#ifndef __LOGUTIL_H__
#define __LOGUTIL_H__

// No project header is allowed
#include <fstream>
#include <string>
#include <chrono>
#include <queue>
#include "ThreadUtil.h"
#include "FileUtil.h"
// No project header is allowed

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
    Logger(const Logger&&) = delete;
    Logger& operator=(const Logger&) = delete;
    void work();

public:
    void log(const char* txt);
    void log(const char* txt, const std::chrono::time_point<std::chrono::system_clock> now);
    void error(const char* txt);
    void error(const char* txt, const std::chrono::time_point<std::chrono::system_clock> now);
    void debug(const char* txt);
    void debug(const char* txt, const std::chrono::time_point<std::chrono::system_clock> now);
protected:
    void append(const Log& log);
private:
    std::ofstream f;
    std::queue<Log> logQ;
    std::chrono::time_point<std::chrono::system_clock> lastLogTime;
};

#endif
