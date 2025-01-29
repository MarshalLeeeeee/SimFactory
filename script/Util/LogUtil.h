#pragma once

#ifndef __LOGUTIL_H__
#define __LOGUTIL_H__

// No project header is allowed
#include <fstream>
#include <string>
// No project header is allowed

class Logger {
public:
    static Logger& getInstance() {
        static Logger logger;
        return logger;
    }
private:
    Logger();
    ~Logger();
    Logger(const Logger& logger) = delete;
    Logger& operator=(const Logger& logger) = delete;

public:
    void log(const char* txt);
    void error(const char* txt);
    void debug(const char* txt);
    void append(const char* prefix, const char* txt);
private:
    std::ofstream f;
};

#endif
