#pragma once

#include "mocca/base/ByteArray.h"
#include "mocca/net/ConnectionID.h"

#include <chrono>
#include <string>

namespace mocca {
namespace net {

class IMessageConnection {
public:
    virtual ~IMessageConnection() {}

    virtual std::shared_ptr<const ConnectionID> connectionID() const = 0;
    virtual bool isConnected() const = 0;
    virtual void send(ByteArray message, std::chrono::milliseconds timeout = std::chrono::milliseconds(100)) const = 0;
    virtual ByteArray receive(std::chrono::milliseconds timeout = std::chrono::milliseconds(100)) const = 0;
};
}
}