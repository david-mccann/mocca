#pragma once

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

class Runnable {
public:
    virtual ~Runnable() { join(); }

    virtual void run() = 0;
    void start();

    virtual void interrupt();
    bool isInterrupted() const;
    void join();

    void setException(const std::exception_ptr& exception);
    void rethrowException();

    std::thread::id id() const;

private:
    std::atomic<bool> interrupted_;
    std::mutex exceptionMx_;
    std::exception_ptr exception_;
    std::thread thread_;
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