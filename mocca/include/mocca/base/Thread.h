#pragma once

#include <atomic>
#include <mutex>
#include <thread>
#include <vector>

namespace mocca {
struct Thread {
    Thread();

    template <class F, class... Args>
    Thread(F&& fun, Args&&... args)
        : interrupted_(false)
        , thread_(std::forward<F>(fun), std::forward<Args>(args)...) {
        std::lock_guard<std::mutex> lock(allThreadsMx_);
        allThreads_.push_back(this);
    }

    Thread(Thread&& other);
    Thread& operator=(Thread&& other);
    ~Thread();

    bool isInterrupted() const;
    void interrupt();
    static void interruptAll();

private:
    std::atomic<bool> interrupted_;
    std::thread thread_;

    static std::mutex allThreadsMx_;
    static std::vector<Thread*> allThreads_;
};
}