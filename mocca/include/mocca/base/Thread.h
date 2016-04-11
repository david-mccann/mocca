/****************************************************************
* Copyright (C) 2016 Andrey Krekhov, David McCann
*
* The content of this file may not be copied and/or distributed
* without the expressed permission of the copyright owner.
*
****************************************************************/

#pragma once

#include "mocca/base/Error.h"

#include <atomic>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

namespace mocca {
class AutoJoinThread {
public:
    template <class F, class... Args>
    AutoJoinThread(F&& fun, Args&&... args)
        : thread_(std::forward<F>(fun), std::forward<Args>(args)...) {}
    ~AutoJoinThread() { thread_.join(); }

private:
    std::thread thread_;
};

class ThreadInterrupt : public Error {
public:
    ThreadInterrupt(const std::string& file, int line)
        : Error("Thread has been interuppted", file, line) {}
};


class Runnable {
public:
    Runnable();
    virtual ~Runnable();

    virtual void run() = 0;
    void start();

    virtual void interrupt();
    bool isInterrupted() const;
    void join();

    void setException(const std::exception_ptr& exception);
    void rethrowException();

    std::thread::id id() const;

    static bool isCurrentInterrupted();

private:
    std::atomic<bool> interrupted_;
    std::mutex exceptionMx_;
    std::exception_ptr exception_;
    std::thread thread_;

    static std::mutex activeMx_;
    static std::vector<Runnable*> active_;
};

class RunnableGroup {
public:
    ~RunnableGroup();

    void addRunnable(std::unique_ptr<Runnable> thread);
    void removeRunnable(const std::thread::id& threadID);

    void interruptAll();
    void joinAll();

    void rethrowException();

private:
    std::vector<std::unique_ptr<Runnable>> threads_;
};
}