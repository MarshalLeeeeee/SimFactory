#include "FileUtil.h"

std::string getTimeStampStr() {
    std::string ts;
    const std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::tm localTime;
    localtime_s(&localTime, &time);
    ts += std::to_string(localTime.tm_year + 1900);
    ts += "-";
    auto mon = std::to_string(localTime.tm_mon + 1);
    if (mon.size() == 1) ts += "0";
    ts += mon;
    ts += "-";
    auto day = std::to_string(localTime.tm_mday);
    if (day.size() == 1) ts += "0";
    ts += day;
    ts += "-";
    auto hour = std::to_string(localTime.tm_hour);
    if (hour.size() == 1) ts += "0";
    ts += hour;
    ts += "-";
    auto min = std::to_string(localTime.tm_min);
    if (min.size() == 1) ts += "0";
    ts += min;
    ts += "-";
    auto sec = std::to_string(localTime.tm_sec);
    if (sec.size() == 1) ts += "0";
    ts += sec;
    return ts;
}

bool directoryExists(const char* dir) {
    DWORD ftyp = GetFileAttributesA(dir);
    if (ftyp == INVALID_FILE_ATTRIBUTES) {
        return false;
    }
    return (ftyp & FILE_ATTRIBUTE_DIRECTORY) != 0;
}

bool createDirectory(const char* dir) {
    return CreateDirectoryA(dir, NULL) || GetLastError() == ERROR_ALREADY_EXISTS;
}