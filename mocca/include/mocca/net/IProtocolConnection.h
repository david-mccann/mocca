#pragma once

#include <chrono>
#include <string>

#include "mocca/base/ByteArray.h"

namespace mocca {

namespace net {

class IProtocolConnection {
public:
    virtual ~IProtocolConnection() {}

    virtual std::string identifier() const = 0;
    virtual void send(ByteArray message) const = 0;
    virtual ByteArray receive(std::chrono::milliseconds timeout = std::chrono::milliseconds(200)) const = 0;
};
}
}