#pragma once

#include <functional>
#include <list>
#include <memory>
#include <mutex>

namespace mocca {

template <typename T> class ObjectPool {
    void customDelete(T* obj) {
        obj->clear();
        std::unique_lock<std::mutex> lock(mutex_);
        freeObjects_.push_back(
            std::unique_ptr<T, std::function<void(T*)>>(obj, std::bind(&ObjectPool::customDelete, this, std::placeholders::_1)));
    }

public:
    using ObjectPtr = std::unique_ptr<T, std::function<void(T*)>>;

    ObjectPool(size_t initialSize)
        : initialSize_(initialSize) {
        for (size_t i = 0; i < initialSize_; ++i) {
            freeObjects_.push_back(ObjectPtr(new T(), std::bind(&ObjectPool::customDelete, this, std::placeholders::_1)));
        }
    }

    ObjectPtr getObject() {
        std::unique_lock<std::mutex> lock(mutex_);
        if (freeObjects_.empty()) {
            for (size_t i = 0; i < initialSize_; ++i) {
                freeObjects_.push_back(ObjectPtr(new T(), std::bind(&ObjectPool::customDelete, this, std::placeholders::_1)));
            }
        }
        auto obj = std::move(freeObjects_.front());
        freeObjects_.pop_front();
        return obj;
    }

    size_t numFreeObjects() {
        std::unique_lock<std::mutex> lock(mutex_);
        return freeObjects_.size();
    }

private:
    std::mutex mutex_;
    std::list<ObjectPtr> freeObjects_;
    size_t initialSize_;
};
}