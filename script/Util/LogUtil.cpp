#include "LogUtil.h"

Log::Log(std::string prefix, std::string txt, std::chrono::time_point<std::chrono::system_clock> now) :
    prefix(prefix), txt(txt), now(now) {}

Logger::Logger() : ThreadPool() {
    // initialize file stream
    if (!directoryExists("log")) {
        createDirectory("log");
    }
    std::string filename = "log/SimFactory";
    filename += "-";
    filename += getTimeStampStr();
    filename += ".log";
    f = std::ofstream(filename.c_str(), std::ios::app);
    // initialize time
    lastLogTime = std::chrono::system_clock::now();
    // initialize thread: only one off thread as consumer
    workers.emplace_back([this]{ work(); });
}

Logger::~Logger() {
    stopThreadPool();
    // flush remaining logs
    logQ.emplace("Log", "Ready to close log file...", std::chrono::system_clock::now());
    while (!logQ.empty()) {
        Log log = logQ.front();
        logQ.pop();
        append(log);
    }
    // close file
    f.close();
}

void Logger::work() {
    while (true) {
        auto now = std::chrono::system_clock::now();
        double dt = static_cast<std::chrono::duration<double>>(now - lastLogTime).count();
        if (dt < 1.0) continue;
        std::queue<Log> q;
        {
            std::unique_lock<std::mutex> lock(m);
            cv.wait(lock, [this]{
                return stop || !logQ.empty();
            });
            if (stop && logQ.empty()) break;
            q.swap(logQ);
        }
        while (!q.empty()) {
            Log log = q.front();
            q.pop();
            append(log);
        }
        lastLogTime = now;
    }
}

void Logger::log(const char* txt) {
    if (stop) return;
    {
        std::unique_lock<std::mutex> lock(m);
        logQ.emplace("Log", txt, std::chrono::system_clock::now());
    }
    cv.notify_one();
}

void Logger::log(const char* txt, const std::chrono::time_point<std::chrono::system_clock> now) {
    if (stop) return;
    {
        std::unique_lock<std::mutex> lock(m);
        logQ.emplace("Log", txt, now);
    }
    cv.notify_one();
}

void Logger::error(const char* txt) {
    if (stop) return;
    {
        std::unique_lock<std::mutex> lock(m);
        logQ.emplace("Error", txt, std::chrono::system_clock::now());
    }
    cv.notify_one();
}

void Logger::error(const char* txt, const std::chrono::time_point<std::chrono::system_clock> now) {
    if (stop) return;
    {
        std::unique_lock<std::mutex> lock(m);
        logQ.emplace("Error", txt, now);
    }
    cv.notify_one();
}

void Logger::debug(const char* txt) {
    if (stop) return;
    {
        std::unique_lock<std::mutex> lock(m);
        logQ.emplace("Debug", txt, std::chrono::system_clock::now());
    }
    cv.notify_one();
}

void Logger::debug(const char* txt, const std::chrono::time_point<std::chrono::system_clock> now) {
    if (stop) return;
    {
        std::unique_lock<std::mutex> lock(m);
        logQ.emplace("Debug", txt, now);
    }
    cv.notify_one();
}

void Logger::append(const Log& log) {
    f << getTimeStampStr(log.now) << " [" << log.prefix << "] " << log.txt << std::endl;
}