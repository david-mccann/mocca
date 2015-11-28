#pragma once

#include <chrono>
#include <memory>

namespace mocca {
namespace net {

class IProtocolConnection;

class IProtocolConnectionAcceptor {
public:
    virtual ~IProtocolConnectionAcceptor() {}

    virtual std::unique_ptr<IProtocolConnection> getConnection(std::chrono::milliseconds timeout = std::chrono::milliseconds(500)) = 0;
};
}
}