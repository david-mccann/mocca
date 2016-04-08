/****************************************************************
* Copyright (C) 2016 Andrey Krekhov, David McCann
*
* The content of this file may not be copied and/or distributed
* without the expressed permission of the copyright owner.
*
****************************************************************/

#pragma once

#include <atomic>
#include <future>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

namespace mocca {

extern thread_local std::atomic<bool> this_thread_interrupt_flag;

/* Cf. Anthony Williams, C++ Concurrency in Action, pp. 289 */
class Thread {
public:
    Thread() = default;

    template <typename FunctionType> Thread(FunctionType f) {
        std::promise<std::atomic<bool>*> p;
        thread_ = std::thread([f, &p] {
            p.set_value(&this_thread_interrupt_flag);
            f();
        });
        interruptedRef_ = p.get_future().get();
    }

    void interrupt() {
        if (interruptedRef_ != nullptr) {
            interruptedRef_->store(true);
        }
    }

    static bool isThisInterrupted() { return this_thread_interrupt_flag.load(); }

    bool joinable() const;
    void join();

    std::thread::id id() const;

private:
    std::thread thread_;
    std::atomic<bool>* interruptedRef_ = nullptr;
};

class AutoJoinThread {
public:
    template <class F, class... Args>
    AutoJoinThread(F&& fun, Args&&... args)
        : thread_(std::forward<F>(fun), std::forward<Args>(args)...) {}
    ~AutoJoinThread() { thread_.join(); }

private:
    std::thread thread_;
};

class Runnable {
public:
    virtual ~Runnable() { join(); }

    virtual void run() = 0;
    void start();

    virtual void interrupt();
    void join();

    void setException(const std::exception_ptr& exception);
    void rethrowException();

    std::thread::id id() const;

private:
    std::mutex exceptionMx_;
    std::exception_ptr exception_;
    Thread thread_;
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