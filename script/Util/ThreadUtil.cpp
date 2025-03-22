#include "ThreadUtil.h"

//////////////////////////////////////

ThreadPool::ThreadPool(): stop(false) {}

ThreadPool::~ThreadPool() {}

void ThreadPool::stopThreadPool() {
    stop = true;
    cv.notify_all();
    for (std::thread &worker : workers) {
        worker.join();
    }
}

/////////////////////////////////////

Task::Task(uint32_t tid, uint32_t p, std::function<void()> f) :
    taskId(tid), priority(p), func(f) {}

bool CmpTask::operator()(const Task& t1, const Task& t2) {
    if (t1.priority > t2.priority) return true;
    else if (t1.priority < t2.priority) return false;
    else return t1.taskId > t2.taskId;
}

Dispatcher::Dispatcher() : ThreadPool(), taskId(0) {
    for (uint32_t i = 0; i < DISPATCHER_CNT; i++) {
        workers.emplace_back([this]{ work(); });
    }
}

Dispatcher::~Dispatcher() {
    stopThreadPool();
}

void Dispatcher::work() {
    while (true) {
        std::function<void()> func;
        {
            std::unique_lock<std::mutex> lock(m);
            cv.wait(lock, [this]{ return stop || !tasks.empty(); });
            if (stop && tasks.empty()) break;
            auto task = tasks.top();
            tasks.pop();
            func = std::move(task.func);
        }
        func();
    }
}

void Dispatcher::submit(uint32_t priority, std::function<void()> func) {
    if (stop) return;
    {
        std::unique_lock<std::mutex> lock(m);
        tasks.emplace(taskId++, priority, func);
    }
    cv.notify_one();
}

/////////////////////////////////////

MainthreadCallbackHub::MainthreadCallbackHub() :
    stop(false), invokeBatch(0), funcCnt(0) {}

MainthreadCallbackHub::~MainthreadCallbackHub() {
    stop = true;
    while (!funcs.empty()) {
        std::function<void()> func = funcs.front();
        funcs.pop();
        func();
    }
}

void MainthreadCallbackHub::submit(std::function<void()> func) {
    if (stop) return;
    std::unique_lock<std::mutex> lock(m);
    funcs.push(std::move(func));
}

void MainthreadCallbackHub::invoke() {
    std::queue<std::function<void()>> tmp;
    {
        std::unique_lock<std::mutex> lock(m);
        uint32_t cnt = funcs.size();
        if (cnt == 0) return;

        if (cnt >= funcCnt) {
            invokeBatch *= 2;
        }
        else {
            invokeBatch /= 2;
        }
        if (invokeBatch <= 0) {
            invokeBatch = 1;
        }
        
        uint32_t i = 0;
        while (!funcs.empty() && i < invokeBatch) {
            std::function<void()> func = funcs.front();
            funcs.pop();
            ++i;
            tmp.push(std::move(func));
        }
    }
    while (!tmp.empty()) {
        std::function<void()> func = tmp.front();
        tmp.pop();
        func();
    }
}
