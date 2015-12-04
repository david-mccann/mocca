#include "mocca/base/Thread.h"

using mocca::Thread;

Thread::Thread()
    : interrupt_(true)
    , thread_() {}

Thread::Thread(Thread&& other)
    : interrupt_(other.interrupt_.load())
    , thread_(std::move(other.thread_)) {
    other.interrupt_ = true;
}

Thread& Thread::operator=(Thread&& other) {
    if (thread_.joinable()) {
        thread_.join();
    }
    thread_ = std::move(other.thread_);
    return *this;
}

Thread::~Thread() {
    interrupt_ = true;
    if (thread_.joinable()) {
        thread_.join();
    }
}

bool mocca::Thread::isInterrupted() const {
    return interrupt_;
}

void mocca::Thread::interrupt() {
    interrupt_ = true;
}