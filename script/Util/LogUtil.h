#pragma once

#ifndef __LOGUTIL_H__
#define __LOGUTIL_H__

// No project header is allowed
#include <fstream>
#include <string>
#include <chrono>
#include "ThreadUtil.h"
#include "FileUtil.h"
// No project header is allowed

template<std::size_t... Is>
struct index_sequence {};

template<std::size_t N, std::size_t... Is>
struct make_index_sequence : make_index_sequence<N-1, N-1, Is...> {};

template<std::size_t... Is>
struct make_index_sequence<0, Is...> : index_sequence<Is...> {};

/* Log:
* prefix: describe log type
* now: describe time stamp of log action
* txt: log content
*/
// struct Log {
//     std::string prefix;
//     std::chrono::time_point<std::chrono::system_clock> now;
//     std::string txt;
//     Log(std::string prefix, std::string txt, std::chrono::time_point<std::chrono::system_clock> now);
// };

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
    /* log info */
    template <typename... Args>
    void info(Args&&... args) {
        if (stop) return;
        {
            std::unique_lock<std::mutex> lock(m);
            Log<Args...> l("Info", std::chrono::system_clock::now(), std::forward<Args>(args)...);
            logTasks.push([l] () { l(); });
        }
        cv.notify_one();
    }

    /* log error */
    template <typename... Args>
    void error(Args&&... args) {
        if (stop) return;
        {
            std::unique_lock<std::mutex> lock(m);
            Log<Args...> l("Error", std::chrono::system_clock::now(), std::forward<Args>(args)...);
            logTasks.push([l]() { l(); });
        }
        cv.notify_one();
    }

    /* log for debug only */
    template <typename... Args>
    void debug(Args&&... args) {
#ifndef NDEBUG
        if (stop) return;
        {
            std::unique_lock<std::mutex> lock(m);
            Log<Args...> l("Debug", std::chrono::system_clock::now(), std::forward<Args>(args)...);
            logTasks.push([l]() { l(); });
        }
        cv.notify_one();
#endif
    }

protected:
    /* append log to file */
    template <typename... Args>
    void appendLog(const std::string& prefix, const std::chrono::time_point<std::chrono::system_clock> now, const std::tuple<Args...>& args) {
       f << getTimeStampStr(now) << " [" << prefix << "] ";
       appendArgTuple(args, make_index_sequence<sizeof...(Args)>{});
    }
    
    /* append args in tuple format to file */
    template <typename... Args, size_t... Is>
    void appendArgTuple(const std::tuple<Args...>& args, index_sequence<Is...>) {
       appendArgs(std::get<Is>(args)...);
    }
    
    /* append args to file */
    void appendArgs() { f << std::endl; }
    template <typename T, typename... Args>
    void appendArgs(T&& t, Args&&... args) {
       f << t;
       appendArgs(std::forward<Args>(args)...);
    }

private:
    /* package args */
    template <typename... Args>
    struct Log {
        std::string prefix;
        std::chrono::time_point<std::chrono::system_clock> now;
        std::tuple<Args...> args;

        Log(std::string prefix_, std::chrono::time_point<std::chrono::system_clock> now_, Args&&... args_) :
            prefix(prefix_), now(now_), args(std::forward<Args>(args_)...) {}

        void operator()() const {
            Logger::getInstance().appendLog(prefix, now, args);
        }
    };

    /* file out stream */
    std::ofstream f;
    /* log task queue */
    std::queue<std::function<void()>> logTasks;
    /* last append time */
    std::chrono::time_point<std::chrono::system_clock> lastLogTime;
};

#endif
