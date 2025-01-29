#include "LogUtil.h"
#include "FileUtil.h"

Logger::Logger() {
    if (!directoryExists("log")) {
        createDirectory("log");
    }
    std::string filename = "log/SimFactory";
    filename += "-";
    filename += getTimeStampStr();
    filename += ".log";
    f = std::ofstream(filename.c_str(), std::ios::app);
}

Logger::~Logger() {
    f.close();
}

void Logger::log(const char* txt) {
    append("[Log]", txt);
}

void Logger::error(const char* txt) {
    append("[Error]", txt);
}

void Logger::debug(const char* txt) {
    append("[Debug]", txt);
}

void Logger::append(const char* prefix, const char* txt) {
    f << getTimeStampStr() << prefix << txt << std::endl;
}