#pragma once

#include "mocca/base/ByteArray.h"
#include "mocca/net/ConnectionID.h"

#include <chrono>
#include <mutex>
#include <string>

namespace mocca {
namespace net {
class IStreamConnection {
public:
    IStreamConnection();
    virtual ~IStreamConnection() {}

    virtual std::shared_ptr<const ConnectionID> connectionID() const = 0;
    virtual void send(ByteArray message, std::chrono::milliseconds timeout = std::chrono::milliseconds(100)) const = 0;
    
    ByteArray receive(uint32_t maxSize, std::chrono::milliseconds timeout = std::chrono::milliseconds(100)) const;
    void putBack(const ByteArray& data);

    void lock() { mutex_.lock(); }
    void unlock() { mutex_.unlock(); }

protected:
    virtual ByteArray readFromStream(uint32_t maxSize, std::chrono::milliseconds timeout = std::chrono::milliseconds(100)) const = 0;

private:
    mutable ByteArray putBackData_;
    mutable uint32_t putBackReadPos_;
    std::mutex mutex_;
};
}
}