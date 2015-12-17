#pragma once

#include "mocca/base/Nullable.h"

#include <algorithm>
#include <chrono>
#include <condition_variable>
#include <functional>
#include <list>
#include <mutex>

namespace mocca {

/* Implementation of a thread-safe queue for passing messages between threads. */
template <typename T> class MessageQueue {
public:
    /* Enqueue an item and notify all waiting threads. */
    void enqueue(T t) {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push_back(std::move(t));
        condition_.notify_all();
    }

    /* Dequeue operation with a timeout. The calling thread is blocked until either an item is
     * available for being fetched or the timeout expires. If the timeout expires, a null-object is
     * returned. */
    Nullable<T> dequeue(std::chrono::milliseconds timeout) {
        std::unique_lock<std::mutex> lock(mutex_);
        if (!condition_.wait_for(lock, timeout, [&] { return !queue_.empty(); })) {
            return Nullable<T>();
        }
        Nullable<T> val(std::move(queue_.front()));
        queue_.pop_front();
        return val;
    }

    /* Blocking dequeue operation with a predicate for filtering and a timeout. Like 'dequeue',
    * but only items satisfying the predicate will be fetched.  */
    Nullable<T> dequeueFiltered(std::function<bool(const T&)> predicate, std::chrono::milliseconds timeout) {
        std::unique_lock<std::mutex> lock(mutex_);
        typename decltype(queue_)::iterator findIt;
        if (!condition_.wait_for(lock, timeout, [&] {
                findIt = std::find_if(begin(queue_), end(queue_), predicate);
                return findIt != end(queue_);
            })) {
            return Nullable<T>();
        }
        Nullable<T> val(std::move(*findIt));
        queue_.erase(findIt);
        return val;
    }

    Nullable<T> dequeueNoWait() {
        std::lock_guard<std::mutex> lock(mutex_);
        if (!queue_.empty()) {
            T val(std::move(queue_.front()));
            queue_.pop_front();
            return val;
        }
        return Nullable<T>();
    }

    bool isEmpty() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.empty();
    }

    size_t size() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.size();
    }

private:
    std::list<T> queue_;
    mutable std::mutex mutex_;
    std::condition_variable condition_;
};
}