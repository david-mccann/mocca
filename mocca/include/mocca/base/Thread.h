#pragma once

#include <atomic>
#include <thread>

namespace mocca {
struct Thread {
    Thread();

    template <class F, class... Args>
    Thread(F&& fun, Args&&... args)
        : interrupt_(false)
        , thread_(std::forward<F>(fun), std::forward<Args>(args)...) {}

    Thread(Thread&& other);
    Thread& operator=(Thread&& other);
    ~Thread();

    bool isInterrupted() const;
    void interrupt();

private:
    std::atomic<bool> interrupt_;
    std::thread thread_;
};
}