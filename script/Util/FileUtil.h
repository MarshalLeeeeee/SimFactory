#pragma once

#ifndef __FILEUTIL_H__
#define __FILEUTIL_H__

// No project header is allowed
#include <windows.h>
#include <string>
#include <chrono>
#include <ctime>
// No project header is allowed

/* Return year-mon-day-hour-min-sec */
std::string getTimeStampStr();

/* Return year-mon-day-hour-min-sec */
std::string getTimeStampStr(const std::chrono::time_point<std::chrono::system_clock> now);

/* return if the directory exists */
bool directoryExists(const char* dir);

/* create the directory and return the result*/
bool createDirectory(const char* dir);

#endif
