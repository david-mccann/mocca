/*The MIT License(MIT)

Copyright(c) 2016 David McCann

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files(the
"Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and / or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to
the following conditions :

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.*/

#pragma once

#include <functional>
#include <memory>
#include <mutex>
#include <vector>

namespace mocca {

template <typename T> class ObjectPool {
public:
    ObjectPool(size_t initialSize)
        : initialSize_(initialSize) {
        for (size_t i = 0; i < initialSize_; ++i) {
            freeObjects_.push_back(new T());
        }
    }

    using ObjectPtr = std::unique_ptr<T, std::function<void(T*)>>;
    ObjectPtr getObject() {
        T* obj = nullptr;
        {
            std::unique_lock<std::mutex> lock(mutex_);
            if (freeObjects_.empty()) {
                for (size_t i = 0; i < initialSize_; ++i) {
                    freeObjects_.push_back(new T());
                }
            }
            obj = freeObjects_.back();
            freeObjects_.pop_back();
        }
        auto deleter = [this](T* obj) {
            obj->clear();
            std::unique_lock<std::mutex> lock(mutex_);
            freeObjects_.push_back(obj);
        };
        return std::unique_ptr<T, decltype(deleter)>(obj, deleter);
    }

    size_t numFreeObjects() {
        std::unique_lock<std::mutex> lock(mutex_);
        return freeObjects_.size();
    }

private:
    std::mutex mutex_;
    std::vector<T*> freeObjects_;
    size_t initialSize_;
};
}