#pragma once

#include "mocca/base/ByteArray.h"

#include <mutex>

namespace mocca {
namespace net {

template <typename Derived>
class IOStreamBase {
public:
    std::string identifier() const {
        return static_cast<const Derived*>(this)->identifierImpl();
    }

    ByteArray receive(uint32_t maxSize, std::chrono::milliseconds timeout = std::chrono::milliseconds(100)) { // fixme: rename to read
        return static_cast<Derived*>(this)->readImpl(maxSize, timeout);
    }

    void send(ByteArray data, std::chrono::milliseconds timeout = std::chrono::milliseconds(100)) { // fixme: rename to write
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