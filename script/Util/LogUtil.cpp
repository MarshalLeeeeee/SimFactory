#include "LogUtil.h"
#include "FileUtil.h"

// Log::Log(std::string prefix, std::string txt, std::chrono::time_point<std::chrono::system_clock> now) :
//     prefix(prefix), txt(txt), now(now) {}

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
    while (!logTasks.empty()) {
        std::function<void()> logTask = logTasks.front();
        logTasks.pop();
        logTask();
    }
    appendLog("Log", std::chrono::system_clock::now(), std::make_tuple("Ready to close log file..."));
    // close file
    f.close();
}

void Logger::work() {
    while (true) {
        auto now = std::chrono::system_clock::now();
        double dt = static_cast<std::chrono::duration<double>>(now - lastLogTime).count();
        if (dt < 1.0) continue;
        std::queue<std::function<void()>> q;
        {
            std::unique_lock<std::mutex> lock(m);
            cv.wait(lock, [this]{
                return stop || !logTasks.empty();
            });
            if (stop && logTasks.empty()) break;
            q.swap(logTasks);
        }
        while (!q.empty()) {
            std::function<void()> logTask = q.front();
            q.pop();
            logTask();
        }
        lastLogTime = now;
    }
}
