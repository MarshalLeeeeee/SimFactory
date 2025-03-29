#pragma once

#ifndef __THREADUTIL_H__
#define __THREADUTIL_H__

// No project header is allowed
#include <thread>
#include <vector>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <atomic>
// No project header is allowed

/* Abstract class containing multi threads */
class ThreadPool {
protected:
    ThreadPool();
    ~ThreadPool();
    void stopThreadPool();
    std::mutex m;
    std::condition_variable cv;
    std::vector<std::thread> workers;
    std::atomic<bool> stop;
};

///////////////////////////////////////////////

/* Universal task dispatcher
 * with priority controll
 * once dispatched, tasks can be paraly done
 */

#define DISPATCHER_CNT 8

struct Task {
    uint32_t taskId;
    uint32_t priority;
    std::function<void()> func;
    Task(uint32_t tid, uint32_t p, std::function<void()> f);
};

struct CmpTask {
    bool operator()(const Task& t1, const Task& t2);
};

class Dispatcher : public ThreadPool {
public:
    static Dispatcher& getInstance() {
        static Dispatcher instance;
        return instance;
    }
private:
    Dispatcher();
    ~Dispatcher();
    Dispatcher(const Dispatcher&) = delete;
    Dispatcher(Dispatcher&&) = delete;
    Dispatcher& operator=(const Dispatcher&) = delete;
    Dispatcher& operator=(Dispatcher&&) = delete;

public:
    /* called by main thread, submit function (to be done by sub threads) */
    void submit(uint32_t priority, std::function<void()> func);
private:
    /* work function of sub threads */
    void work();
    std::priority_queue<Task, std::vector<Task>, CmpTask> tasks;
    uint32_t taskId;
};

///////////////////////////////////////////////

/*
 * Universal mainthread callcack hub
 */
class MainthreadCallbackHub {
public:
    static MainthreadCallbackHub& getInstance() {
        static MainthreadCallbackHub instance;
        return instance;
    }
private:
    MainthreadCallbackHub();
    ~MainthreadCallbackHub();
    MainthreadCallbackHub(const MainthreadCallbackHub&) = delete;
    MainthreadCallbackHub(MainthreadCallbackHub&&) = delete;
    MainthreadCallbackHub& operator=(const MainthreadCallbackHub&) = delete;
    MainthreadCallbackHub& operator=(MainthreadCallbackHub&&) = delete;

public:
    /* called by sub thread, submit function (to be done by main thread) */
    void submit(std::function<void()> func);
    /* called by main thread, consume functions (adaptive consume speed) */
    void invoke();
private:
    std::mutex m;
    std::queue<std::function<void()>> funcs;
    std::atomic<bool> stop;
    /* max consume function count in one invoke */
    uint32_t invokeBatch;
    /* the accumulated function cnt */
    uint32_t accumulatedFuncCnt;
};

#endif
