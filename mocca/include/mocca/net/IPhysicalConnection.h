#pragma once

#include <string>
#include <chrono>

#include "mocca/base/ByteArray.h"

namespace mocca {
    
namespace net {

class IPhysicalConnection {
public:
    virtual ~IPhysicalConnection() {}

    virtual std::string identifier() const = 0;
    virtual void lock() = 0;
    virtual void unlock() = 0;
    virtual void send(ByteArray message, std::chrono::milliseconds timeout = std::chrono::milliseconds(200)) const = 0;
    virtual ByteArray receive(uint32_t maxSize, std::chrono::milliseconds timeout = std::chrono::milliseconds(200)) const = 0;
};
}
}