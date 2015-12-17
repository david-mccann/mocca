#pragma once

#include "mocca/base/ByteArray.h"

#include <chrono>
#include <mutex>
#include <string>

namespace mocca {
namespace net {
class IStreamConnection {
public:
    virtual ~IStreamConnection() {}

    virtual std::string identifier() const = 0;
    virtual void send(ByteArray message, std::chrono::milliseconds timeout = std::chrono::milliseconds(100)) const = 0;
    virtual ByteArray receive(uint32_t maxSize, std::chrono::milliseconds timeout = std::chrono::milliseconds(100)) const = 0;
    void lock() { mutex_.lock(); }
    void unlock() { mutex_.unlock(); }

private:
    std::mutex mutex_;
};
}
}