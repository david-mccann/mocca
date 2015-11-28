#include "mocca/base/Thread.h"

using mocca::Thread;

Thread::Thread(std::thread t) : thread_(std::move(t)) {}

mocca::Thread::Thread(Thread&& other)
    : thread_(std::move(other.thread_)) {}

Thread& Thread::operator=(Thread&& other) {
    if (thread_.joinable()) {
        thread_.join();
    }
    thread_ = std::move(other.thread_);
    return *this;
}

Thread::~Thread() {
    if (thread_.joinable()) {
        thread_.join();
    }
}