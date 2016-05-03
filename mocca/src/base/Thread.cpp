/****************************************************************
* Copyright (C) 2016 Andrey Krekhov, David McCann
*
* The content of this file may not be copied and/or distributed
* without the expressed permission of the copyright owner.
*
****************************************************************/

#include "mocca/base/Thread.h"

#include <algorithm>

std::vector<mocca::Runnable*> mocca::Runnable::active_;
std::mutex mocca::Runnable::activeMx_;

mocca::Runnable::Runnable() {
std::lock_guard<std::mutex> lock(activeMx_);
    active_.push_back(this);
}

mocca::Runnable::~Runnable() {
    join();
    std::lock_guard<std::mutex> lock(activeMx_);
    active_.erase(std::remove(begin(active_), end(active_), this), end(active_));
}

void mocca::Runnable::start() {
    interrupted_ = false;
    thread_ = std::thread(&Runnable::run, this);
}

void mocca::Runnable::join() {
    interrupted_ = true;
    if (thread_.joinable()) {
        thread_.join();
    }
}

bool mocca::Runnable::isInterrupted() const {
    return interrupted_;
}

void mocca::Runnable::interrupt() {
    interrupted_ = true;
}

std::thread::id mocca::Runnable::id() const {
    return thread_.get_id();
}

void mocca::Runnable::setException(const std::exception_ptr& exception) {
    std::lock_guard<std::mutex> lock(exceptionMx_);
    exception_ = exception;
}

void mocca::Runnable::rethrowException() {
    std::lock_guard<std::mutex> lock(exceptionMx_);
    if (exception_ != nullptr) {
        auto copy = exception_;
        exception_ = nullptr;
        std::rethrow_exception(copy);
    }
}

bool mocca::Runnable::isCurrentInterrupted() {
    std::lock_guard<std::mutex> lock(activeMx_);
    auto it = std::find_if(begin(active_), end(active_), [](const Runnable* ptr) { return ptr->id() == std::this_thread::get_id(); });
    return it != end(active_) && (*it)->isInterrupted();
}

mocca::RunnableGroup::~RunnableGroup() {
    joinAll();
}

void mocca::RunnableGroup::addRunnable(std::unique_ptr<Runnable> thread) {
    threads_.push_back(std::move(thread));
}

void mocca::RunnableGroup::interruptAll() {
    for (auto& thread : threads_) {
        thread->interrupt();
    }
}

void mocca::RunnableGroup::joinAll() {
    interruptAll();
    threads_.clear();
}

void mocca::RunnableGroup::rethrowException() {
    for (auto& thread : threads_) {
        thread->rethrowException();
    }
}

void mocca::RunnableGroup::removeRunnable(const std::thread::id& threadID) {
    auto it =
        std::find_if(begin(threads_), end(threads_), [&](const std::unique_ptr<Runnable>& thread) { return thread->id() == threadID; });
    (*it)->join();
    threads_.erase(it);
}