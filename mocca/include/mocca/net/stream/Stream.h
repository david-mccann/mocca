#pragma once

#include "mocca/base/ByteArray.h"

#include <mutex>

namespace mocca {
namespace net {

template <typename Derived>
class Stream {
public:
    std::string identifier() const {
        return static_cast<const Derived*>(this)->identifierImpl();
    }

    ByteArray read(uint32_t maxSize, std::chrono::milliseconds timeout = std::chrono::milliseconds(100)) {
        return static_cast<Derived*>(this)->readImpl(maxSize, timeout);
    }

    void write(ByteArray data, std::chrono::milliseconds timeout = std::chrono::milliseconds(100)) {
        static_cast<Derived*>(this)->writeImpl(std::move(data), timeout);
    }

    void lock() {
        mutex_.lock();
    }

    void unlock() {
        mutex_.unlock();
    }

private:
    std::mutex mutex_;
};
}
}