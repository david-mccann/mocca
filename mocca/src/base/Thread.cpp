#include "mocca/base/Thread.h"

using mocca::Thread;

std::mutex Thread::allThreadsMx_;
std::vector<Thread*> Thread::allThreads_;

Thread::Thread()
    : interrupted_(true)
    , thread_() {}

Thread::Thread(Thread&& other)
    : interrupted_(other.interrupted_.load())
    , thread_(std::move(other.thread_)) {
    other.interrupted_ = true;
    allThreads_.push_back(this);
}

Thread& Thread::operator=(Thread&& other) {
    if (thread_.joinable()) {
        thread_.join();
    }
    thread_ = std::move(other.thread_);
    return *this;
}

Thread::~Thread() {
    interrupted_ = true;
    if (thread_.joinable()) {
        thread_.join();
    }
    std::lock_guard<std::mutex> lock(allThreadsMx_);
    auto it = std::find(begin(allThreads_), end(allThreads_), this);
    if (it != end(allThreads_)) {
        allThreads_.erase(it);
    }
}

bool mocca::Thread::isInterrupted() const {
    return interrupted_;
}

void mocca::Thread::interrupt() {
    interrupted_ = true;
}

void mocca::Thread::interruptAll() {
    std::lock_guard<std::mutex> lock(allThreadsMx_);
    for (auto thread : allThreads_) {
        thread->interrupt();
    }
}