#pragma once

#include <list>
#include <memory>

namespace mocca {

template <typename T> class ObjectPool {
    struct Deleter {
        Deleter(ObjectPool<T>& pool)
            : pool_(pool) {}
        void operator()(T* obj) {
            obj->clear();
            pool_.freeObjects_.push_back(std::shared_ptr<T>(new T(), Deleter(*this)));
        }
        ObjectPool<T>& pool_;
    };

public:
    ObjectPool(size_t initialSize)
        : initialSize_(initialSize) {
        for (size_t i = 0; i < initialSize_; ++i) {
            freeObjects_.push_back(std::shared_ptr<T>(new T(), Deleter(*this)));
        }
    }

    std::shared_ptr<T> getObject() {
        if (freeObjects_.empty()) {
            for (size_t i = 0; i < initialSize_; ++i) {
                freeObjects_.push_back(std::shared_ptr<T>(new T(), Deleter(*this)));
            }
        }
        auto obj = freeObjects_.front();
        freeObjects_.pop_front();
        return obj;
    }

    size_t numFreeObjects() { return freeObjects_.size(); }

private:
    std::list<std::shared_ptr<T>> freeObjects_;
    size_t initialSize_;
};
}